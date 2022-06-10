#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <GitTools/base.h>

namespace Ui {
class LogWindow;
}

class GitLogView;
class GitLogModel;
class GitCommitFiles;

class LogWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();

public slots:

    void refresh(bool checked);
    void openRepository();
    void openRepository(const QString &path);

private slots:

    void commitSelected(const QModelIndex &index);
    void fileClicked(const QModelIndex &index);

    void splitterMoved(int pos, int index);
    void logViewColumnResized(int index, int oldSize, int newSize);
    void commitViewColumnResized(int index, int oldSize, int newSize);

    void onActivate(const QModelIndex &index);

    void on_actionCreateBranch_triggered();
    void on_actionDeleteBranch_triggered();

protected:

    bool eventFilter(QObject *obj, QEvent *ev) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::LogWindow *ui;

    QSettings *cache;
    GitLogModel *logModel;
    GitCommitFiles *filesModel;
    GitLogView *logView;

    git::repository repo;

};

#endif // LOGWINDOW_H
