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

LogWindow::LogWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);

    cache = new QSettings(QString("%1/.cache/GitTools/GitLog.ini").arg(QDir::homePath()), QSettings::IniFormat, this);
    qDebug() << cache->fileName();

    GitLogDelegate* gld = new GitLogDelegate(this);
    //gld->setLaneHeight(fontMetrics().height());
    ui->logView->setItemDelegate(gld);
    ui->logView->addAction(ui->actionCreateBranch);
    ui->logView->addAction(ui->actionDeleteBranch);

    logModel = new GitLogModel(this);
    logModel->setRepository(&repo);
    ui->logView->setModel(logModel);

    filesModel = new GitCommitFiles(this);
    ui->commitView->setModel(filesModel);
    ui->commitView->installEventFilter(this);

    connect(ui->actionRepoOpen, SIGNAL(triggered(bool)), this, SLOT(openRepository()));
    connect(ui->actionAllBranches, &QAction::toggled, this, &LogWindow::refresh);
    connect(ui->actionRefresh, &QAction::triggered, this, &LogWindow::refresh);
    connect(ui->logView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(commitSelected(QModelIndex)));
    connect(ui->logView, SIGNAL(activated(QModelIndex)), this, SLOT(onActivate(QModelIndex)));
    connect(ui->commitView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(fileClicked(QModelIndex)));
    connect(ui->splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(splitterMoved(int,int)));
    connect(ui->logView->header(), SIGNAL(sectionResized(int,int,int)), this, SLOT(logViewColumnResized(int,int,int)));
    connect(ui->commitView->header(), SIGNAL(sectionResized(int,int,int)), this, SLOT(commitViewColumnResized(int,int,int)));

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

    auto header = ui->logView->header();
    for(int i = 0; i < 3; i++)
    {
        int size = cache->value(QString("LogView/size%1").arg(i), -1).toInt();
        if ( size > 0 )
        {
            header->resizeSection(i, size);
        }
    }

    header = ui->commitView->header();
    for(int i = 0; i < 3; i++)
    {
        int size = cache->value(QString("CommitView/size%1").arg(i), -1).toInt();
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
}

LogWindow::~LogWindow()
{
    delete ui;
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
    if ( ui->actionAllBranches->isChecked() )
    {
        logModel->openAllRefs();
    }
    else
    {
        logModel->open(repo.get_head());
    }
}

void LogWindow::commitSelected(const QModelIndex &index)
{
    if ( !index.isValid() )
    {
        return;
    }

    auto commit = logModel->getCommitInfo(index);

    QString message = QString("SHA-1: %1\n\n%2").arg(commit.oid().toString()).arg(commit.message());
    ui->commitMessage->setText(message);
    filesModel->open(&repo, commit.oid());
}

void LogWindow::fileClicked(const QModelIndex &index)
{
    filesModel->execute(index);
}

void LogWindow::splitterMoved(int, int)
{
    cache->setValue("window/splitter", ui->splitter->saveState());
}

void LogWindow::logViewColumnResized(int index, int, int newSize)
{
    cache->setValue(QString("LogView/size%1").arg(index), newSize);
}

void LogWindow::commitViewColumnResized(int index, int, int newSize)
{
    cache->setValue(QString("CommitView/size%1").arg(index), newSize);
}

void LogWindow::onActivate(const QModelIndex &index)
{
    qDebug() << "activated " << index.row();
    ui->logView->clearSelection();
    ui->commitView->setFocus();
    //ui->commitView->setCurrentIndex(filesModel->index(0, 0));
    ui->commitView->setCurrentIndex(filesModel->index(0, 0));
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

bool LogWindow::eventFilter(QObject *obj, QEvent *event)
{
    if ( event->type() == QEvent::KeyPress )
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if ( keyEvent->key() == Qt::Key_Backspace )
        {
            if ( obj == ui->commitView )
            {
                ui->logView->setFocus();
                ui->logView->setCurrentIndex(ui->logView->currentIndex());
                ui->commitView->clearSelection();
                return true;
            }
        }
    }

    return false;
}

void LogWindow::resizeEvent(QResizeEvent *event)
{
    cache->setValue("window/maximized", isMaximized() ? "yes" : "no");
    cache->setValue("window/width", event->size().width());
    cache->setValue("window/height", event->size().height());
}
