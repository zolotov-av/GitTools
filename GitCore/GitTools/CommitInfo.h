#ifndef GT_COMMIT_INFO_H
#define GT_COMMIT_INFO_H

#include <QVector>
#include <GitTools/base.h>
#include "CommitRefsModel.h"

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

    enum class CommitType {
        invalid,
        index,
        worktree,
        commit
    };

    inline const char* commitTypeName(CommitType type)
    {
        switch(type)
        {
        case CommitType::invalid: return "invalid";
        case CommitType::index: return "Changes to be committed";
        case CommitType::worktree: return "Changes not staged for commit";
        case CommitType::commit: return "commit";
        default: return "unknown";
        }
    }

    class CommitInfo
    {
    private:

        CommitType m_type { CommitType::invalid };
        git::object_id m_oid { };
        QString m_message;
        QString m_author_name;
        QDateTime m_commit_time;
        CommitRefsModel m_refs_model;

    public:

        bool up = false;
        bool down = false;
        int lane = -1;
        QVector<git::object_id> childs;
        QVector<git::object_id> parents;
        QVector<GraphLane> lanes;

        bool isValid() const { return m_type != CommitType::invalid; }
        bool isCommit() const { return m_type == CommitType::commit; }
        bool isIndex() const { return m_type == CommitType::index; }
        bool isWorktree() const { return m_type == CommitType::worktree; }

        CommitInfo() = default;
        CommitInfo(CommitType t): m_type(t), m_message(commitTypeName(t)) { }
        CommitInfo(const git::commit &commit):
            m_type(CommitType::commit),
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

        const QString& message() const
        {
            return m_message;
        }

        QString shortMessage(int maxLen = 80) const
        {
            return m_message.left(maxLen).split(QRegExp("(\\r|\\n)")).first();
        }

        const QString& author_name() const
        {
            return m_author_name;
        }

        const QDateTime& commit_time() const
        {
            return m_commit_time;
        }

        const git::object_id& oid() const
        {
            return m_oid;
        }

        const git::object_id& parentId(unsigned i) const
        {
            return parents[i];
        }

        unsigned parentCount() const
        {
            return parents.size();
        }

        bool isRoot() const
        {
            return parentCount() == 0;
        }

        bool isTail() const
        {
            return childs.size() == 0;
        }

        bool isBranch() const
        {
            return childs.size() > 1;
        }

        bool isMerge() const
        {
            return parentCount() > 1;
        }

        bool childOf(const CommitInfo &commit) const
        {
            for(const auto &p_id : parents)
            {
                if ( p_id == commit.oid() ) return true;
            }
            return false;
        }

        bool parentOf(const CommitInfo &commit) const
        {
            return commit.childOf(*this);
        }

        CommitRefsModel* refsModel() const
        {
            return const_cast<CommitRefsModel*>(&m_refs_model);
        }

    };

}

#endif // GT_COMMIT_INFO_H
