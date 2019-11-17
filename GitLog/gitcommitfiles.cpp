#include "gitcommitfiles.h"

#include <QProcess>
#include <QTemporaryFile>
#include <QDebug>

#include "gitdiffprocess.h"

using namespace LibQGit2;

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

    return active ? diff.numDeltas() : 0;
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

QModelIndex GitCommitFiles::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QVariant GitCommitFiles::headerData(int section, Qt::Orientation orientation, int role) const
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
    DiffDelta delta = diff.delta(index);
    switch ( delta.type() )
    {
    case DiffDelta::Unknown:
        return "unknown";
    case DiffDelta::Unmodified:
        return "unmodified";
    case DiffDelta::Added:
        return "new";
    case DiffDelta::Deleted:
        return "deleted";
    case DiffDelta::Modified:
    case DiffDelta::Renamed:
    case DiffDelta::Copied:
        return "modified";
    case DiffDelta::Ignored:
        return "ignored";
    case DiffDelta::Untracked:
        return "untracked";
    case DiffDelta::Typechange:
        return "typechange";
    }

    return "";
}

QString GitCommitFiles::GetDiffPath(int index) const
{
    DiffDelta delta = diff.delta(index);
    switch ( delta.type() )
    {
    case DiffDelta::Unknown:
    case DiffDelta::Ignored:
    case DiffDelta::Untracked:
    case DiffDelta::Typechange:
        return "N/A";
    case DiffDelta::Unmodified:
    case DiffDelta::Deleted:
    case DiffDelta::Modified:
    case DiffDelta::Renamed:
        return delta.oldFile().path();
    case DiffDelta::Added:
    case DiffDelta::Copied:
        return delta.newFile().path();
    }

    return "N/A";
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

    return QVariant();
}

void GitCommitFiles::open(const Commit &commit, LibQGit2::Repository *repo)
{
    this->beginResetModel();
    this->commit = commit;
    if ( commit.parentCount() > 0 )
    {
        this->diff = repo->diffTrees(commit.parent(0).tree(), commit.tree());
        this->repo = repo;
        active = true;
    }
    else
    {
        this->diff = Diff();
        active = false;
    }
    this->endResetModel();
}

void GitCommitFiles::close()
{
    beginResetModel();
    active = false;
    endResetModel();
}

void GitCommitFiles::execute(const QModelIndex &index)
{
    if ( !index.isValid() )
    {
        return;
    }

    DiffDelta delta = diff.delta(index.row());
    switch ( delta.type() )
    {
    case DiffDelta::Added:
        // TODO;
        return;
    case DiffDelta::Deleted:
        // TODO
        return;
    case DiffDelta::Modified:
    case DiffDelta::Renamed:
    case DiffDelta::Copied:
        showDelta(delta);
    default:
        // do nothing
        return;
    }
}

void GitCommitFiles::showDelta(DiffDelta delta)
{
    TreeEntry te = commit.parent(0).tree().entryByName(delta.oldFile().path());
    git_blob *blob = NULL;
    int error = git_blob_lookup(&blob, repo->data(), te.oid().data());
    qDebug() << "git_blob_lookup() = " << error;
    if ( error != 0 )
    {
        return;
    }
    Blob oldBlob(blob);

    te = commit.tree().entryByName(delta.oldFile().path());
    error = git_blob_lookup(&blob, repo->data(), te.oid().data());
    qDebug() << "git_blob_lookup() = " << error;
    if ( error != 0 )
    {
        return;
    }
    Blob newBlob(blob);

    GitDiffProcess *process = new GitDiffProcess(this);
    process->open(oldBlob, newBlob);
}
