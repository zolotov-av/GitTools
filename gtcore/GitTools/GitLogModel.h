#ifndef GT_GITLOG_MODEL_H
#define GT_GITLOG_MODEL_H

#include <QAbstractItemModel>
#include <GitTools/base.h>
#include <GitTools/CommitInfo.h>

class GitLogModel final: public QAbstractItemModel
{
    Q_OBJECT

private:

    using GraphLane = git::GraphLane;

    git::repository *repo { nullptr };

    QVector<git::CommitInfo> history;
    QList<git::reference_info> m_refs;

    void clear();
    void updateRefs();
    void updateGraph();

public:

    GitLogModel(QObject *parent);
    GitLogModel(const GitLogModel &) = delete;
    GitLogModel(GitLogModel &&) = delete;

    ~GitLogModel();

    GitLogModel& operator = (const GitLogModel &) = delete;
    GitLogModel& operator = (GitLogModel &&) = delete;

    const QList<git::reference_info>& refs() const
    {
        return m_refs;
    }

    void setRepository(git::repository *repo);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool open(const git::reference &ref);
    bool openAllRefs();
    void update();

    git::CommitInfo getCommitInfo(const QModelIndex &index) const;

};

#endif // GT_GITLOG_MODEL_H
