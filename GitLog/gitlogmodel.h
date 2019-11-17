#ifndef GITLOGMODEL_H
#define GITLOGMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <qgit2e.h>

namespace LibQGit2
{
    class Repository;
}


class GitLogModel : public QAbstractItemModel
{
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

    LibQGit2::Commit getCommit(const QModelIndex &index);

    LibQGit2::Repository *repo = nullptr;

protected:


    QVector<LibQGit2::Commit> history;

    void clear();
};

#endif // GITLOGMODEL_H
