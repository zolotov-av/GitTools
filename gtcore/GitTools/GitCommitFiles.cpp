#include "GitCommitFiles.h"
#include <QColor>

GitCommitFiles::GitCommitFiles(QObject *parent): QAbstractItemModel(parent), active(false), commit(), diff()
{

}

GitCommitFiles::~GitCommitFiles()
{

}

int GitCommitFiles::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
    {
        return 0;
    }

    return active ? diff.deltaCount() : 0;
}

int GitCommitFiles::columnCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
    {
        return 0;
    }

    return 3;
}

QModelIndex GitCommitFiles::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
    {
        return createIndex(row, column, 1);
    }

    return QModelIndex();
}

QModelIndex GitCommitFiles::parent(const QModelIndex &) const
{
    return QModelIndex();
}

QVariant GitCommitFiles::headerData(int section, Qt::Orientation, int role) const
{
    if ( role == Qt::DisplayRole )
    {
        switch ( section )
        {
        case 0:
            return "Path";
        case 1:
            return "Extension";
        case 2:
            return "Status";
        default:
            return section;
        }
    }

    return QVariant();

}

QVariant GitCommitFiles::GetDiffStatus(int index) const
{
    auto delta = diff.get_delta(index);
    switch ( delta.type() )
    {
    case GIT_DELTA_UNMODIFIED:
        return "unmodified";
    case GIT_DELTA_ADDED:
        return "new";
    case GIT_DELTA_DELETED:
        return "deleted";
    case GIT_DELTA_MODIFIED:
    case GIT_DELTA_RENAMED:
    case GIT_DELTA_COPIED:
        return "modified";
    case GIT_DELTA_IGNORED:
        return "ignored";
    case GIT_DELTA_UNTRACKED:
        return "untracked";
    case GIT_DELTA_TYPECHANGE:
        return "typechange";
    default:
        return "unknown";
    }
}

QString GitCommitFiles::GetDiffPath(int index) const
{
    auto delta = diff.get_delta(index);
    switch ( delta.type() )
    {
    case GIT_DELTA_IGNORED:
    case GIT_DELTA_UNTRACKED:
    case GIT_DELTA_TYPECHANGE:
    case GIT_DELTA_ADDED:
    case GIT_DELTA_COPIED:
        return delta.newFile().path();
    case GIT_DELTA_UNMODIFIED:
    case GIT_DELTA_DELETED:
    case GIT_DELTA_MODIFIED:
    case GIT_DELTA_RENAMED:
        return delta.oldFile().path();
    default:
        return QStringLiteral("N/A");
    }
}

QString GitCommitFiles::GetDiffExtension(int index) const
{
    QString path = GetDiffPath(index);
    int pos = path.indexOf('.');
    if ( pos > 0 )
    {
        return path.mid(pos);
    }

    return "";

}

QVariant GitCommitFiles::data(const QModelIndex &index, int role) const
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
            return GetDiffPath(index.row());
        case 1:
            return GetDiffExtension(index.row());
        case 2:
            return GetDiffStatus(index.row());
        default:
            return index.column() + 1;
        }
    }

    if ( role == Qt::ForegroundRole )
    {
        switch (diff.get_delta(index.row()).type() )
        {
        case GIT_DELTA_IGNORED: return QColor(Qt::gray);
        case GIT_DELTA_UNTRACKED: return QColor(Qt::red);
        case GIT_DELTA_COPIED:
        case GIT_DELTA_UNMODIFIED:
        case GIT_DELTA_RENAMED:
        case GIT_DELTA_TYPECHANGE: return QColor(Qt::green);
        case GIT_DELTA_DELETED: return QColor(Qt::darkRed);
        case GIT_DELTA_MODIFIED:
        case GIT_DELTA_ADDED: return QColor(Qt::blue);
        default: return QColor(Qt::black);
        }
    }

    if ( role == statusRole )
    {
        return GetDiffStatus(index.row());
    }

    return QVariant();
}

QHash<int, QByteArray> GitCommitFiles::roleNames() const
{
    return {
        {fileNameRole, "fileName"},
        {colorRole, "fileColor"},
        {statusRole, "fileStatus"}
    };
}

void GitCommitFiles::open(git::repository *repo, const git::object_id &commit_oid)
{
    this->beginResetModel();
    this->commit = repo->get_commit(commit_oid);
    if ( commit.parentCount() > 0 )
    {
        this->diff = repo->diff(repo->get_commit(commit.parentId(0)), commit);
        this->repo = repo;
        active = true;
    }
    else
    {
        this->diff = {};
        active = false;
    }
    this->endResetModel();
}

void GitCommitFiles::open_cached(git::repository *repo)
{
    beginResetModel();
    commit = repo->get_commit(repo->get_head().target());
    diff = repo->diff_cached(commit);
    this->repo = repo;
    active = true;
    endResetModel();
}

void GitCommitFiles::open_worktree(git::repository *repo)
{
    beginResetModel();
    commit = { };
    diff = repo->diff();
    this->repo = repo;
    active = true;
    endResetModel();
}

git::delta GitCommitFiles::getDelta(int index)
{
    return diff.get_delta(index);
}

void GitCommitFiles::close()
{
    beginResetModel();
    active = false;
    endResetModel();
}
