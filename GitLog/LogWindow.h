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
    Q_PROPERTY(bool showAllBranches READ showAllBranches WRITE setShowAllBranches NOTIFY showAllBranchesChanged FINAL)
    Q_PROPERTY(bool showTags READ showTags WRITE setShowTags NOTIFY showTagsChanged FINAL)
    Q_PROPERTY(int currentCommitIndex READ currentCommitIndex WRITE setCurrentCommitIndex NOTIFY currentCommitChanged FINAL)
    Q_PROPERTY(QString commitMessage READ commitMessage WRITE setCommitMessage NOTIFY commitMessageChanged FINAL)
    Q_PROPERTY(QAbstractItemModel* logModel READ logModel CONSTANT FINAL)
    Q_PROPERTY(QAbstractItemModel* filesModel READ filesModel CONSTANT FINAL)
    Q_PROPERTY(QAbstractItemModel* diffModel READ diffModel CONSTANT FINAL)

private:

    bool m_show_all_branches { false };
    bool m_show_tags { false };
    int m_current_commit_index { -1 };
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

    bool showAllBranches() const { return m_show_all_branches; }
    void setShowAllBranches(bool value);
    bool showTags() const { return m_show_tags; }
    void setShowTags(bool value);

    int currentCommitIndex() const { return m_current_commit_index; }
    void setCurrentCommitIndex(int idx);
    const QString& commitMessage() const { return m_commit_message; }
    void setCommitMessage(const QString &text);

    QAbstractItemModel* logModel() { return m_log_model; }
    QAbstractItemModel* filesModel() { return m_files_model; }
    QAbstractItemModel* diffModel() { return &m_diff_model; }

public slots:

    void update();
    void systrayActivated(QSystemTrayIcon::ActivationReason reason);
    void refresh(bool checked);
    void openRepository();
    void openRepository(const QString &path);
    void exit();
    void openFromTray();
    void showCommit(int index);
    void openDiff(int index);
    void closeDiff();
    void openCommitDialog();

private slots:

    void on_actionCreateBranch_triggered();
    void on_actionDeleteBranch_triggered();
    void doCommit();

protected:

    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::LogWindow *ui;

    QSettings *cache;
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

    void showAllBranchesChanged();
    void showTagsChanged();
    void currentCommitChanged();
    void commitMessageChanged();

};

#endif // LOGWINDOW_H
