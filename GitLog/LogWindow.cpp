#include "LogWindow.h"
#include "ui_LogWindow.h"

#include <GitTools/GitLogModel.h>
#include <GitTools/GitCommitFiles.h>
#include <GitTools/GitLogDelegate.h>
#include <GitTools/CreateBranchDialog.h>
#include <GitTools/DeleteBranchDialog.h>

#include <QMenu>
#include <QDebug>
#include <QDir>
#include <QResizeEvent>
#include <QQmlContext>

void LogWindow::openDiff(int index)
{
    qDebug().noquote() << "openDiff: " << index;
    const auto delta = m_files_model->getDelta(index);
    m_diff_model.setGitDelta(&repo, delta, m_files_model->isWorktree());
}

void LogWindow::closeDiff()
{
    qDebug().noquote() << "closeDiff";
    m_diff_model.clear();
}

LogWindow::LogWindow(QWidget *parent): QObject{parent}
{
    cache = new QSettings(QString("%1/.cache/GitTools/GitLog.ini").arg(QDir::homePath()), QSettings::IniFormat, this);
    qDebug() << cache->fileName();

    m_log_model = new GitLogModel(this);
    m_log_model->setRepository(&repo);

    m_files_model = new GitCommitFiles(this);

    connect(commitDialog, &CommitDialog::accepted, this, &LogWindow::doCommit);

    commitDialog->setAuthorName(cache->value("AuthorName").toString());
    commitDialog->setAuthorEmail(cache->value("AuthorEmail").toString());

    const QString path = configValue("repo/path", "").toString();
    if ( !path.isEmpty() )
    {
        openRepository(path);
    }

    m_qml_engine.rootContext()->setContextProperty("gitlog", this);
    m_qml_engine.load(QUrl{"qrc:/qml/MainWindow.qml"});

    if (m_qml_engine.rootObjects().isEmpty())
    {
        qDebug().noquote() << "ERROR: m_qml_engine.rootObjects().isEmpty()";
    }
}

LogWindow::~LogWindow()
{
    delete commitDialog;
}

QVariant LogWindow::configValue(const QString &key, const QVariant &defaultValue)
{
    return cache->value(key, defaultValue);
}

void LogWindow::setConfigValue(const QString &key, const QVariant &value)
{
    qDebug().noquote() << "setConfigValue()" << key << ":" << value;
    cache->setValue(key, value);
}

void LogWindow::setMinimizeToTray(bool value)
{
    if ( m_minimize_to_tray == value )
        return;

    m_minimize_to_tray = value;
    emit minimizeToTrayChanged();
}

void LogWindow::setShowAllBranches(bool value)
{
    if ( m_show_all_branches == value )
        return;

    m_show_all_branches = value;
    emit showAllBranchesChanged();

    update();
}

void LogWindow::setShowTags(bool value)
{
    if ( m_show_tags == value )
        return;

    m_show_tags = value;
    emit showTagsChanged();

    update();
}

void LogWindow::setCurrentCommitIndex(int idx)
{
    if ( m_current_commit_index == idx )
        return;

    m_current_commit_index = idx;
    emit currentCommitChanged();
}

void LogWindow::setCommitMessage(const QString &text)
{
    if ( m_commit_message == text )
        return;

    m_commit_message = text;
    emit commitMessageChanged();
}

void LogWindow::update()
{
    qDebug().noquote() << "LogWindow::update()";
    refresh(m_show_all_branches);
}

void LogWindow::refresh(bool checked)
{
    if ( repo.isOpened() )
    {
        if ( checked )
        {
            m_log_model->openAllRefs();
        }
        else
        {
            m_log_model->open(repo.get_head());
        }
    }
}

static QString urlToLocalFile(const QString &file)
{
    const QUrl url{ file };
    if ( url.scheme().isEmpty() )
        return file;

    qDebug().noquote() << "url: " << url << "scheme:" << url.scheme();
    if ( !url.isLocalFile() )
    {
        qDebug().noquote() << "wrong URL: " << url.scheme();
        return { };
    }

    return url.toLocalFile();
}

void LogWindow::openRepository(const QString &path)
{
    qDebug().noquote() << "openRepository(" << path << ")";
    if ( path.isEmpty() )
        return;

    const auto file = urlToLocalFile(path);
    if ( file.isEmpty() )
        return;

    repo.open(file);
    cache->setValue("repo/path", file);
    update();
}

void LogWindow::exit()
{
    printf("LogWindow::exit()\n");
    qApp->quit();
}

void LogWindow::showCommit(int index)
{
    setCurrentCommitIndex(index);

    const auto commit = m_log_model->commitInfoByIndex(index);
    if ( commit.isCommit() )
    {
        QString message = QString("SHA-1: %1\n\n%2").arg(commit.oid().toString()).arg(commit.message());
        setCommitMessage(message);
        m_files_model->open(&repo, commit.oid());
    }
    else if ( commit.isIndex() )
    {
        setCommitMessage(tr("Changes to be committed"));
        m_files_model->open_cached(&repo);
    }
    else if ( commit.isWorktree() )
    {
        setCommitMessage(tr("Changes not staged for commit"));
        m_files_model->open_worktree(&repo);
    }
    else
    {
        setCommitMessage(commit.message());
        m_files_model->close();
    }
}

void LogWindow::on_actionCreateBranch_triggered()
{
    qDebug() << "on_actionCreateBranch_triggered()";
    const auto commit = m_log_model->commitInfoByIndex(currentCommitIndex());

    auto dlg = new CreateBranchDialog(nullptr);
    dlg->setModel(m_log_model);
    dlg->setRepositiory(&repo);
    dlg->setCommitId(commit.oid().toString());
    dlg->show();

}

void LogWindow::on_actionDeleteBranch_triggered()
{
    qDebug() << "on_actionDeleteBranch_triggered()";
    const auto commit = m_log_model->commitInfoByIndex(currentCommitIndex());

    auto dlg = new git::DeleteBranchDialog(nullptr);
    dlg->setModel(m_log_model);
    dlg->setCommitId(&repo, commit.oid().toString());
    dlg->show();

}

void LogWindow::openCommitDialog()
{
    {
        git::config cfg { repo.r };
        commitDialog->setAuthorName(cfg.getString("user.name"));
        commitDialog->setAuthorEmail(cfg.getString("user.email"));
    }
    commitDialog->setFiles(&repo);
    commitDialog->exec();
}

void LogWindow::doCommit()
{
    const auto authorName = commitDialog->authorName();
    const auto authorEmail = commitDialog->authorEmail();
    repo.make_commit(authorName, authorEmail, commitDialog->message());
    cache->setValue("AuthorName", authorName);
    cache->setValue("AuthorEmail", authorEmail);
    commitDialog->clearMessage();
    update();
}
