#ifndef GITLOGVIEW_H
#define GITLOGVIEW_H

#include <QWidget>
#include <QTreeView>

class GitLogModel;
class GitCommitFiles;

class GitLogView : public QWidget
{
    Q_OBJECT
public:
    explicit GitLogView(QWidget *parent = nullptr);
    ~GitLogView();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    bool openRepository(const QString &path);

protected:

    GitLogModel *model;
    QTreeView *view;

    GitCommitFiles *filesModel;
    QTreeView *filesView;

    void paintEvent(QPaintEvent *) override;

signals:

public slots:

private slots:

    void commitSelected(const QModelIndex &index);
    void doubleClicked(const QModelIndex &index);
};

#endif // GITLOGVIEW_H
