#include "gitlogmodel.h"

#include <QDebug>

#include <git2/refs.h>

using namespace LibQGit2;

GitLogModel::GitLogModel(QObject *parent): QAbstractItemModel(parent)
{
}

GitLogModel::~GitLogModel()
{
}

void GitLogModel::setRepository(Repository *repo)
{
    this->repo = repo;
}

int GitLogModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
    {
        return 0;
    }
    return history.count();
}

int GitLogModel::columnCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
    {
        return 0;
    }

    return 4;
}

QModelIndex GitLogModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
    {
        return createIndex(row, column, 1);
    }

    return QModelIndex();
}

QModelIndex GitLogModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

QVariant GitLogModel::headerData(int section, Qt::Orientation, int role) const
{
    if ( role == Qt::DisplayRole )
    {
        switch ( section )
        {
        case 0:
            return "Graph";
        case 1:
            return "Commit";
        case 2:
            return "Date";
        case 3:
            return "Author";
        default:
            return section;
        }
    }

    return QVariant();
}

QVariant GitLogModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        switch ( index.column() )
        {
        case 0:
            return index.column() + 1;
        case 1:
            return history[index.row()].shortMessage();
        case 2:
            return history[index.row()].dateTime();
        case 3:
            return history[index.row()].author().name();
        default:
            return index.column() + 1;
        }
    }

    return QVariant();
}

bool GitLogModel::open(const Reference& reference)
{
    beginResetModel();
    Commit commit;
    RevWalk revwalk(*repo);
    revwalk.push(reference);
    history.clear();
    while ( revwalk.next(commit) )
    {
        GitCommitInfo info { commit };
        if ( history.size() > 0 )
        {
            info.up = history[history.size()-1].childOf(info);
            history[history.size()-1].down = info.up;
        }
        history.append(info);

    }

    updateGraph();

    updateRefs();

    endResetModel();

    return true;
}

bool GitLogModel::openAllRefs()
{
    beginResetModel();

    updateRefs();

    RevWalk revwalk(*repo);
    revwalk.setSorting(RevWalk::Topological);
    revwalk.push(repo->head());
    for(auto ref : refs)
    {
        try {
            revwalk.push(ref);
        } catch (const LibQGit2::Exception &e) {
            qDebug() << "revwalk.push() exception: " << e.what();
        }
    }

    history.clear();
    Commit commit;
    while ( revwalk.next(commit) )
    {
        history.append(commit);
    }

    updateGraph();

    endResetModel();

    return true;
}

void GitLogModel::clear()
{
    beginResetModel();
    history.clear();
    refs.clear();
    endResetModel();
}

void GitLogModel::updateRefs()
{
    refs.clear();
    git_reference_iterator *iter;
    int status = git_reference_iterator_new(&iter, repo->data());
    if ( status == 0 )
    {
        git_reference *ref;

        while ( true )
        {
            status = git_reference_next(&ref, iter);
            if ( status != 0 )
            {
                break;
            }
            refs.append(Reference(ref));
        }

        git_reference_iterator_free(iter);
    }
}

static GraphLane& getFreeLane(QVector<GraphLane> &lanes, int offset = 0)
{
    int count = lanes.size();
    for(int i = offset; i < count; i++)
    {
        auto &lane = lanes[i];
        if ( lane.type == GraphLane::free ) return lane;
    }

    lanes.append(GraphLane());
    return lanes[lanes.size()-1];
}

void GitLogModel::updateGraph()
{
    for(const auto &commit : history)
    {
        int parentCount = commit.parentCount();
        for(int i = 0; i < parentCount; i++)
        {
            auto parentId = commit.parentId(i);
            for(auto &c : history)
            {
                if ( c.oid() == parentId )
                {
                    c.childs.append(commit.oid());
                }
            }
        }
    }

    QVector<GraphLane> lanes;
    int count = history.size();
    for(int i = 0; i < count; i++)
    {
        GitCommitInfo &commit = history[i];

        bool found = false;
        for(int li = 0; li < lanes.size(); li++)
        {
            auto lane = lanes[li];
            if ( commit.oid() == lane.parent )
            {
                commit.up = true;
                if ( !found )
                {
                    found = true;
                    commit.lane = li;
                    if ( commit.parentCount() > 0 )
                    {
                        commit.down = true;
                        lanes[li] = {GraphLane::line, commit.parent(0).oid()};
                    }
                    else
                    {
                        commit.down = false;
                        lanes[li] = {GraphLane::end, LibQGit2::OId{}};
                    }
                }
                else
                {
                    lanes[li] = {GraphLane::end, LibQGit2::OId{}};
                    //qDebug() << "found again: " << commit.message();
                }
            }
            else
            {
                if ( lane.type == GraphLane::end )
                    lanes[li].type = GraphLane::free;
                else if ( lane.type == GraphLane::start )
                    lanes[li].type = GraphLane::line;
            }
        }
        if ( !found && !commit.isTail() ) qDebug() << "     not found commit: " << commit.message();

        if ( commit.isTail() )
        {
            // allocate free lane
            GraphLane &lane = getFreeLane(lanes);
            commit.lane = lanes.indexOf(lane);
            commit.down = true;
            lane.type = GraphLane::start;
            lane.parent = commit.parent(0).oid();
            int parentCount = commit.parentCount();
            for(int p = 1; p < parentCount; p++)
            {
                auto &lane = getFreeLane(lanes);
                lane.type = GraphLane::start;
                lane.parent = commit.parent(p).oid();
            }
            commit.lanes = lanes;
            continue;
        }

        int parentCount = commit.parentCount();
        for(int p = 1; p < parentCount; p++)
        {
            // allocate free lane
            GraphLane &lane = getFreeLane(lanes, commit.lane);
            lane.type = GraphLane::start;
            lane.parent = commit.parent(p).oid();
        }

        commit.lanes = lanes;
    }
}

GitCommitInfo GitLogModel::getCommit(const QModelIndex &index) const
{
    if ( index.isValid() )
    {
        return history[index.row()];
    }

    return {};
}
