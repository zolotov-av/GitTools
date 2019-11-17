#ifndef GITCOMMITFILES_H
#define GITCOMMITFILES_H

#include <QAbstractItemModel>
#include <QVector>
#include <qgit2e.h>

class GitCommitFiles : public QAbstractItemModel
{
public:
    GitCommitFiles(QObject *parent);
    ~GitCommitFiles() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void open(const LibQGit2::Commit &commit, LibQGit2::Repository *repo);
    void close();

    void execute(const QModelIndex &index);

protected:

    bool active;
    LibQGit2::Repository *repo;
    LibQGit2::Commit commit;
    LibQGit2::Diff diff;

    QVariant GetDiffStatus(int index) const;
    QString GetDiffPath(int index) const;
    QString GetDiffExtension(int index) const;

    void showDelta(LibQGit2::DiffDelta delta);
};

#endif // GITCOMMITFILES_H
