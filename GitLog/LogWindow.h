#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QSystemTrayIcon>
#include <GitTools/base.h>
#include <GitTools/DiffModel.h>
#include <GitTools/GitLogModel.h>
#include "CommitDialog.h"

namespace Ui {
class LogWindow;
}

class GitLogView;
class GitLogModel;
class GitCommitFiles;
class GitLogDelegate;

class LogWindow: public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString commitMessage READ commitMessage WRITE setCommitMessage NOTIFY commitMessageChanged FINAL)
    Q_PROPERTY(QAbstractItemModel* logModel READ logModel CONSTANT FINAL)
    Q_PROPERTY(QAbstractItemModel* filesModel READ filesModel CONSTANT FINAL)
    Q_PROPERTY(QAbstractItemModel* diffModel READ diffModel CONSTANT FINAL)

private:

    QString m_commit_message;
    GitLogModel *m_log_model { nullptr };
    DiffModel m_diff_model { this };

    void closeToTray()
    {
        closeToTray(isMaximized());
    }
    void closeToTray(bool was_maximized);

public:
    explicit LogWindow(QWidget *parent = 0);
    LogWindow(const LogWindow &) = delete;
    LogWindow(LogWindow &&) = delete;
    ~LogWindow();

    LogWindow& operator = (const LogWindow &) = delete;
    LogWindow& operator = (LogWindow &&) = delete;

    const QString& commitMessage() const { return m_commit_message; }
    void setCommitMessage(const QString &text);

    QAbstractItemModel* logModel() { return m_log_model; }
    QAbstractItemModel* filesModel() { return m_files_model; }
    QAbstractItemModel* diffModel() { return &m_diff_model; }

    void update();

public slots:

    void systrayActivated(QSystemTrayIcon::ActivationReason reason);
    void refresh(bool checked);
    void displayTagsToggled(bool checked);
    void openRepository();
    void openRepository(const QString &path);
    void exit();
    void openFromTray();
    void openDiff(int index);
    void closeDiff();

private slots:

    void commitSelected(const QModelIndex &index);

    void splitterMoved(int pos, int index);
    void logViewColumnResized(int index, int oldSize, int newSize);

    void onActivate(const QModelIndex &index);

    void on_actionCreateBranch_triggered();
    void on_actionDeleteBranch_triggered();
    void openCommitDialog();
    void doCommit();

protected:

    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::LogWindow *ui;

    QSettings *cache;
    GitLogDelegate* logDelegate;
    GitCommitFiles *m_files_model { nullptr };
    GitLogView *logView;
    CommitDialog *commitDialog { new CommitDialog(nullptr) };
    QSystemTrayIcon *m_systrayIcon { new QSystemTrayIcon(this) };

    git::repository repo;

    bool m_minimized {false};
    bool m_maximized {false};
    bool m_minimizeTray {true};
    bool m_maximizedTray {false};

signals:

    void commitMessageChanged();

};

#endif // LOGWINDOW_H
