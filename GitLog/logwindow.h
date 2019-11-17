#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <qgit2e.h>

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

    void openRepository();
    void openRepository(const QString &path);

private slots:

    void commitSelected(const QModelIndex &index);
    void fileClicked(const QModelIndex &index);

    void splitterMoved(int pos, int index);
    void logViewColumnResized(int index, int oldSize, int newSize);
    void commitViewColumnResized(int index, int oldSize, int newSize);

    void onActivate(const QModelIndex &index);

protected:

    void resizeEvent(QResizeEvent *event) override;


private:
    Ui::LogWindow *ui;

    QSettings *cache;
    GitLogModel *logModel;
    GitCommitFiles *filesModel;
    GitLogView *logView;

    LibQGit2::Repository *repo = new LibQGit2::Repository();

};

#endif // LOGWINDOW_H
