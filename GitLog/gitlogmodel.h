#ifndef GITLOGMODEL_H
#define GITLOGMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QList>
#include <GitTools/base.h>
#include <vector>

struct GraphLane
{
    enum {
        free,
        start,
        line,
        end
    } type = GraphLane::free;

    git::object_id parent;

    bool isFree() const { return type == GraphLane::free; }

    bool operator == (const GraphLane &other) const {
        return this == &other;
    }

};

class GitCommitInfo
{
private:

    git::object_id m_oid;
    QString m_message;
    QString m_author_name;
    QDateTime m_commit_time;

public:

    bool up = false;
    bool down = false;
    int lane = -1;
    QVector<git::object_id> childs;
    QVector<git::object_id> parents;
    QVector<GraphLane> lanes;

    GitCommitInfo() = default;
    GitCommitInfo(const git::commit &commit):
        m_oid(commit.id()),
        m_message(commit.message()),
        m_author_name(commit.author().name()),
        m_commit_time(commit.dateTime()),
        parents(commit.parentCount())
    {
        for(unsigned i = 0; i < commit.parentCount(); i++)
        {
            parents[i] = commit.parentId(i);
        }
    }

    ~GitCommitInfo() = default;

    QString message() const
    {
        return m_message;
    }

    QString shortMessage(int maxLen = 80) const
    {
        return m_message.left(maxLen).split(QRegExp("(\\r|\\n)")).first();
    }

    QString author_name() const
    {
        return m_author_name;
    }

    QDateTime commit_time() const
    {
        return m_commit_time;
    }

    git::object_id oid() const
    {
        return m_oid;
    }

    git::object_id parentId(unsigned i) const
    {
        return parents[i];
    }

    unsigned parentCount() const
    {
        return parents.size();
    }

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
        for(const auto &p_id : parents)
        {
            if ( p_id == commit.oid() ) return true;
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

    void setRepository(git::repository *repo);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool open(const git::reference &ref);
    bool openAllRefs();

    GitCommitInfo getCommitInfo(const QModelIndex &index) const;

protected:

    git::repository *repo = nullptr;

    QVector<GitCommitInfo> history;
    QList<git::reference_info> refs;

    void clear();
    void updateRefs();
    void updateGraph();

};

#endif // GITLOGMODEL_H
