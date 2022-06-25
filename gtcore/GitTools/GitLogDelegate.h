#ifndef GT_GITLOG_DELEGATE_H
#define GT_GITLOG_DELEGATE_H

#include <QStyledItemDelegate>
#include <GitTools/base.h>
#include <GitTools/CommitInfo.h>

class GitLogDelegate final: public QStyledItemDelegate
{
    Q_OBJECT

private:

    using GraphLane = git::GraphLane;

    bool m_display_tags {false};

    void paintBackground(QPainter* p, const QStyleOptionViewItem& opt) const;
    void paintGraph(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& i) const;
    void paintRef(QPainter *p, QStyleOptionViewItem &opt, const git::reference_info &ref) const;
    void paintLog(QPainter *p, const QStyleOptionViewItem& o, const QModelIndex &i) const;

public:

    GitLogDelegate(QObject *parent = nullptr);
    GitLogDelegate(const GitLogDelegate &) = delete;
    GitLogDelegate(GitLogDelegate &&) = delete;

    ~GitLogDelegate();

    GitLogDelegate& operator = (const GitLogDelegate &) = delete;
    GitLogDelegate& operator = (GitLogDelegate &&) = delete;

    bool displayTags() const { return m_display_tags; }
    void setDisplayTags(bool value)
    {
        m_display_tags = value;
    }

    virtual QSize sizeHint(const QStyleOptionViewItem& o, const QModelIndex &i) const;
    virtual void paint(QPainter* p, const QStyleOptionViewItem& o, const QModelIndex &i) const;

};

#endif // GT_GITLOG_DELEGATE_H
