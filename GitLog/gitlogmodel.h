#ifndef GITLOGMODEL_H
#define GITLOGMODEL_H

#include <QAbstractItemModel>
#include <GitTools/base.h>
#include "GitCommitInfo.h"

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
    void update();

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
