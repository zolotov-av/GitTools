#ifndef GT_COMMIT_INFO_H
#define GT_COMMIT_INFO_H

#include <QVector>
#include <GitTools/base.h>

namespace git
{

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

    class CommitInfo
    {
    private:

        git::object_id m_oid;
        QString m_message;
        QString m_author_name;
        QDateTime m_commit_time;
        bool m_isValid = false;

    public:

        bool up = false;
        bool down = false;
        int lane = -1;
        QVector<git::object_id> childs;
        QVector<git::object_id> parents;
        QVector<GraphLane> lanes;

        bool isValid() const { return m_isValid; }

        CommitInfo() = default;
        CommitInfo(const git::commit &commit):
            m_oid(commit.id()),
            m_message(commit.message()),
            m_author_name(commit.author().name()),
            m_commit_time(commit.dateTime()),
            m_isValid(true),
            parents(commit.parentCount())
        {
            for(unsigned i = 0; i < commit.parentCount(); i++)
            {
                parents[i] = commit.parentId(i);
            }
        }

        ~CommitInfo() = default;

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

        bool childOf(const CommitInfo &commit) const {
            for(const auto &p_id : parents)
            {
                if ( p_id == commit.oid() ) return true;
            }
            return false;
        }

        bool parentOf(const CommitInfo &commit) const {
            return commit.childOf(*this);
        }

    };

}

#endif // GT_COMMIT_INFO_H
