#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QSystemTrayIcon>
#include <GitTools/base.h>

namespace Ui {
class LogWindow;
}

class GitLogView;
class GitLogModel;
class GitCommitFiles;
class GitLogDelegate;

class LogWindow : public QMainWindow
{
    Q_OBJECT

private:

    void closeToTray()
    {
        closeToTray(isMaximized());
    }
    void closeToTray(bool was_maximized);
    void openFromTray();

public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();

    void update();

public slots:

    void systrayActivated(QSystemTrayIcon::ActivationReason reason);
    void refresh(bool checked);
    void displayTagsToggled(bool checked);
    void openRepository();
    void openRepository(const QString &path);
    void exitTriggered(bool);

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
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::LogWindow *ui;

    QSettings *cache;
    GitLogModel *logModel;
    GitLogDelegate* logDelegate;
    GitCommitFiles *filesModel;
    GitLogView *logView;
    QSystemTrayIcon *m_systrayIcon { new QSystemTrayIcon(this) };

    git::repository repo;

    bool m_minimized {false};
    bool m_maximized {false};
    bool m_minimizeTray {true};
    bool m_maximizedTray {false};

};

#endif // LOGWINDOW_H
