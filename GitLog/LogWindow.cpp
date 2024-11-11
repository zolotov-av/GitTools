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

    logDelegate = new GitLogDelegate(this);
    logDelegate->setDisplayTags(ui->actionDisplayTags->isChecked());
    //logDelegate->setLaneHeight(fontMetrics().height());
    ui->logView->setItemDelegate(logDelegate);
    ui->logView->addAction(ui->actionCreateBranch);
    ui->logView->addAction(ui->actionDeleteBranch);

    logModel = new GitLogModel(this);
    logModel->setRepository(&repo);
    ui->logView->setModel(logModel);

    m_files_model = new GitCommitFiles(this);

    connect(m_systrayIcon, &QSystemTrayIcon::activated, this, &LogWindow::systrayActivated);
    connect(ui->actionRepoOpen, SIGNAL(triggered(bool)), this, SLOT(openRepository()));
    connect(ui->actionAllBranches, &QAction::toggled, this, &LogWindow::refresh);
    connect(ui->actionRefresh, &QAction::triggered, this, &LogWindow::refresh);
    connect(ui->actionCommit, &QAction::triggered, this, &LogWindow::openCommitDialog);
    connect(ui->actionDisplayTags, &QAction::toggled, this, &LogWindow::displayTagsToggled);
    connect(ui->actionExit, &QAction::triggered, this, &LogWindow::exit);
    connect(ui->logView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(commitSelected(QModelIndex)));
    connect(ui->logView, SIGNAL(activated(QModelIndex)), this, SLOT(onActivate(QModelIndex)));
    connect(ui->splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(splitterMoved(int,int)));
    connect(ui->logView->header(), SIGNAL(sectionResized(int,int,int)), this, SLOT(logViewColumnResized(int,int,int)));
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

    const auto header = ui->logView->header();
    for(int i = 0; i < 3; i++)
    {
        int size = cache->value(QString("LogView/size%1").arg(i), -1).toInt();
        if ( size > 0 )
        {
            header->resizeSection(i, size);
        }
    }

    ui->splitter->restoreState(cache->value("window/splitter").toByteArray());

    QString path = cache->value("repo/path", "").toString();
    if ( path != "" )
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
            logModel->openAllRefs();
        }
        else
        {
            logModel->open(repo.get_head());
        }
    }
}

void LogWindow::displayTagsToggled(bool checked)
{
    logDelegate->setDisplayTags(checked);
    update();
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

void LogWindow::commitSelected(const QModelIndex &index)
{
    if ( !index.isValid() )
    {
        return;
    }

    auto commit = logModel->getCommitInfo(index);
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

void LogWindow::splitterMoved(int, int)
{
    cache->setValue("window/splitter", ui->splitter->saveState());
}

void LogWindow::logViewColumnResized(int index, int, int newSize)
{
    cache->setValue(QString("LogView/size%1").arg(index), newSize);
}

void LogWindow::onActivate(const QModelIndex &index)
{
    qDebug() << "activated " << index.row();
    ui->logView->clearSelection();
    //ui->commitView->setFocus();
    //ui->commitView->setCurrentIndex(filesModel->index(0, 0));
    //ui->commitView->setCurrentIndex(m_files_model->index(0, 0));
    ui->quickWidget->setFocus();

    QMetaObject::invokeMethod(
        ui->quickWidget->rootObject(),
        "debugMessage",
        Q_ARG(QVariant, "test")
        );
}

void LogWindow::on_actionCreateBranch_triggered()
{
    qDebug() << "on_actionCreateBranch_triggered()";
    auto commit = logModel->getCommitInfo(ui->logView->currentIndex());

    auto dlg = new CreateBranchDialog(this);
    dlg->setModel(logModel);
    dlg->setRepositiory(&repo);
    dlg->setCommitId(commit.oid().toString());
    dlg->show();

}

void LogWindow::on_actionDeleteBranch_triggered()
{
    qDebug() << "on_actionDeleteBranch_triggered()";
    auto commit = logModel->getCommitInfo(ui->logView->currentIndex());

    auto dlg = new git::DeleteBranchDialog(this);
    dlg->setModel(logModel);
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
