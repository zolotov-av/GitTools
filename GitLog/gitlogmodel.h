#ifndef GITLOGMODEL_H
#define GITLOGMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QList>
#include <qgit2e.h>

namespace LibQGit2
{
    class Repository;
}

struct GraphLane
{
    enum {
        free,
        start,
        line,
        end
    } type = GraphLane::free;

    LibQGit2::OId parent;

    bool isFree() const { return type == GraphLane::free; }

    bool operator == (const GraphLane &other) const {
        return this == &other;
    }

};

class GitCommitInfo: public LibQGit2::Commit
{
public:

    bool up = false;
    bool down = false;
    int lane = -1;
    QVector<LibQGit2::OId> childs;
    QVector<GraphLane> lanes;

    GitCommitInfo() = default;
    GitCommitInfo(const GitCommitInfo &other) = default;
    GitCommitInfo(const LibQGit2::Commit &commit): LibQGit2::Commit(commit) { }
    ~GitCommitInfo() = default;

    bool isRoot() const {
        return parentCount() == 0;
    }

    bool isTail() const {
        return childs.size() == 0;
    }

    bool isBranch() const {
        return childs.size() > 1;
    }

    bool isMerge() const {
        return parentCount() > 1;
    }

    bool childOf(const GitCommitInfo &commit) const {
        auto count = parentCount();
        for(decltype(count) i = 0; i < count; i++)
        {
            if ( parentId(i) == commit.oid() ) return true;
        }
        return false;
    }

    bool parentOf(const GitCommitInfo &commit) const {
        return commit.childOf(*this);
    }

};

class GitLogModel : public QAbstractItemModel
{
friend class QGitLogDelegate;
public:
    GitLogModel(QObject *parent);
    ~GitLogModel() override;

    void setRepository(LibQGit2::Repository *repo);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool open(const LibQGit2::Reference& reference);
    bool openAllRefs();

    GitCommitInfo getCommit(const QModelIndex &index) const;

    LibQGit2::Repository *repo = nullptr;

protected:


    QVector<GitCommitInfo> history;
    QList<LibQGit2::Reference> refs;

    void clear();
    void updateRefs();
    void updateGraph();

};

#endif // GITLOGMODEL_H
