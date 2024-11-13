#include "LogWindow.h"
#include "ui_LogWindow.h"

#include <GitTools/GitLogModel.h>
#include <GitTools/GitCommitFiles.h>
#include <GitTools/GitLogDelegate.h>
#include <GitTools/CreateBranchDialog.h>
#include <GitTools/DeleteBranchDialog.h>

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QResizeEvent>
#include <QQmlContext>
#include <QQuickItem>

void LogWindow::closeToTray(bool was_maximized)
{
    m_maximizedTray = was_maximized;
    setWindowState(windowState() & ~Qt::WindowMaximized);
    hide();
}

void LogWindow::openFromTray()
{
    if ( m_maximizedTray ) showMaximized();
    else if ( isMinimized() ) showNormal();
    else show();
    raise();
    activateWindow();
    update();
}

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

LogWindow::LogWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);

    m_systrayIcon->setIcon(QIcon(":/icons/folder-blue-git-icon.png"));
    m_systrayIcon->setToolTip(QStringLiteral("GitLog"));
    const auto systrayMenu = new QMenu(this);
    systrayMenu->addAction(ui->actionExit);
    m_systrayIcon->setContextMenu(systrayMenu);
    m_systrayIcon->show();

    cache = new QSettings(QString("%1/.cache/GitTools/GitLog.ini").arg(QDir::homePath()), QSettings::IniFormat, this);
    qDebug() << cache->fileName();

    m_log_model = new GitLogModel(this);
    m_log_model->setRepository(&repo);

    m_files_model = new GitCommitFiles(this);

    connect(m_systrayIcon, &QSystemTrayIcon::activated, this, &LogWindow::systrayActivated);
    connect(ui->actionRepoOpen, SIGNAL(triggered(bool)), this, SLOT(openRepository()));
    connect(ui->actionAllBranches, &QAction::toggled, this, &LogWindow::refresh);
    connect(ui->actionRefresh, &QAction::triggered, this, &LogWindow::refresh);
    connect(ui->actionCommit, &QAction::triggered, this, &LogWindow::openCommitDialog);
    connect(ui->actionExit, &QAction::triggered, this, &LogWindow::exit);
    connect(commitDialog, &CommitDialog::accepted, this, &LogWindow::doCommit);

    if ( cache->value("window/maximized", "no").toString() == "yes" )
    {
        this->showMaximized();
    }
    else
    {
        int width = cache->value("window/width", -1).toInt();
        int height = cache->value("window/height", -1).toInt();
        if ( width > 0 && height >= 0 )
        {
            resize(width, height);
        }

    }

    commitDialog->setAuthorName(cache->value("AuthorName").toString());
    commitDialog->setAuthorEmail(cache->value("AuthorEmail").toString());

    QString path = cache->value("repo/path", "").toString();
    if ( !path.isEmpty() )
    {
        openRepository(path);
    }

    const auto ctx = ui->quickWidget->rootContext();
    ctx->setContextProperty("gitlog", this);
    ui->quickWidget->setSource(QUrl{"qrc:/qml/GitLog.qml"});
}

LogWindow::~LogWindow()
{
    ui->quickWidget->setSource({});
    delete commitDialog;
    delete ui;
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
    refresh(ui->actionAllBranches->isChecked());
}

void LogWindow::systrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if ( reason == QSystemTrayIcon::Trigger )
    {
        if ( isVisible() ) closeToTray();
        else openFromTray();
    }
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

void LogWindow::openRepository()
{
    qDebug() << "openRepository";
    QString path = QFileDialog::getExistingDirectory(this, "Choose repository");

    openRepository(path);
}

void LogWindow::openRepository(const QString &path)
{
    repo.open(path);
    cache->setValue("repo/path", path);
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

    auto dlg = new CreateBranchDialog(this);
    dlg->setModel(m_log_model);
    dlg->setRepositiory(&repo);
    dlg->setCommitId(commit.oid().toString());
    dlg->show();

}

void LogWindow::on_actionDeleteBranch_triggered()
{
    qDebug() << "on_actionDeleteBranch_triggered()";
    const auto commit = m_log_model->commitInfoByIndex(currentCommitIndex());

    auto dlg = new git::DeleteBranchDialog(this);
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

void LogWindow::resizeEvent(QResizeEvent *event)
{
    cache->setValue("window/maximized", isMaximized() ? "yes" : "no");
    cache->setValue("window/width", event->size().width());
    cache->setValue("window/height", event->size().height());
}

void LogWindow::changeEvent(QEvent *event)
{
    if ( event->type() == QEvent::WindowStateChange )
    {
        const bool was_maximized = m_maximized;
        m_maximized = isMaximized();
        const bool now_minimized = isMinimized();

        if ( !now_minimized && m_minimized )
        {
            m_minimized = false;
        }

        if ( now_minimized )
        {
            if ( m_minimizeTray )
            {
                event->ignore();
                closeToTray(was_maximized);
                return;
            }

            if ( !m_minimized )
            {
                m_minimized = true;
                m_maximizedTray = isMaximized();
            }
        }
    }
}

void LogWindow::closeEvent(QCloseEvent *event)
{
    //event->ignore();
    if ( m_minimizeTray )
    {
        closeToTray();
    }
}
