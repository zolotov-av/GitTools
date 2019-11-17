#include "gitlogmodel.h"

#include <QDebug>

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

    return 3;
}

QModelIndex GitLogModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
    {
        return createIndex(row, column, 1);
    }

    return QModelIndex();
}

QModelIndex GitLogModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QVariant GitLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role == Qt::DisplayRole )
    {
        switch ( section )
        {
        case 0:
            return "Commit";
        case 1:
            return "Date";
        case 2:
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
            return history[index.row()].shortMessage();
        case 1:
            return history[index.row()].dateTime();
        case 2:
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
        history.append(commit);
    }
    endResetModel();

    return true;
}

void GitLogModel::clear()
{
    beginResetModel();
    history.clear();
    endResetModel();
}

Commit GitLogModel::getCommit(const QModelIndex &index)
{
    if ( index.isValid() )
    {
        return history[index.row()];
    }

    return Commit();
}
