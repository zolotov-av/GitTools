#pragma once

#include <QQmlApplicationEngine>
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

class LogWindow: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool minimizeToTray READ minimizeToTray WRITE setMinimizeToTray NOTIFY minimizeToTrayChanged FINAL)
    Q_PROPERTY(bool showAllBranches READ showAllBranches WRITE setShowAllBranches NOTIFY showAllBranchesChanged FINAL)
    Q_PROPERTY(bool showTags READ showTags WRITE setShowTags NOTIFY showTagsChanged FINAL)
    Q_PROPERTY(int currentCommitIndex READ currentCommitIndex WRITE setCurrentCommitIndex NOTIFY currentCommitChanged FINAL)
    Q_PROPERTY(QString commitMessage READ commitMessage WRITE setCommitMessage NOTIFY commitMessageChanged FINAL)
    Q_PROPERTY(QAbstractItemModel* logModel READ logModel CONSTANT FINAL)
    Q_PROPERTY(QAbstractItemModel* filesModel READ filesModel CONSTANT FINAL)
    Q_PROPERTY(QAbstractItemModel* diffModel READ diffModel CONSTANT FINAL)

private:

    QQmlApplicationEngine m_qml_engine { this };
    bool m_minimize_to_tray { false };
    bool m_show_all_branches { false };
    bool m_show_tags { false };
    int m_current_commit_index { -1 };
    QString m_commit_message { "some text" };
    GitLogModel *m_log_model { nullptr };
    DiffModel m_diff_model { this };

public:

    explicit LogWindow(QWidget *parent = 0);
    LogWindow(const LogWindow &) = delete;
    LogWindow(LogWindow &&) = delete;
    ~LogWindow();

    LogWindow& operator = (const LogWindow &) = delete;
    LogWindow& operator = (LogWindow &&) = delete;

    Q_INVOKABLE QVariant configValue(const QString &key, const QVariant &defaultValue = QVariant());
    Q_INVOKABLE void setConfigValue(const QString &key, const QVariant &value);
    bool minimizeToTray() const { return m_minimize_to_tray; }
    void setMinimizeToTray(bool value);
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
    void refresh(bool checked);
    void openRepository(const QString &path);
    void exit();
    void showCommit(int index);
    void openDiff(int index);
    void closeDiff();
    void openCommitDialog();

private slots:

    void on_actionCreateBranch_triggered();
    void on_actionDeleteBranch_triggered();
    void doCommit();

private:

    QSettings *cache;
    GitCommitFiles *m_files_model { nullptr };
    GitLogView *logView;
    CommitDialog *commitDialog { nullptr };

    git::repository repo;

signals:

    void minimizeToTrayChanged();
    void showAllBranchesChanged();
    void showTagsChanged();
    void currentCommitChanged();
    void commitMessageChanged();
    void showWindowRequested();

};
