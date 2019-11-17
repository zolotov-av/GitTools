#include "logwindow.h"
#include "ui_logwindow.h"

#include "gitlogmodel.h"
#include "gitcommitfiles.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QResizeEvent>

using namespace LibQGit2;

LogWindow::LogWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);

    cache = new QSettings(QString("%1/.cache/GitTools/GitLog.ini").arg(QDir::homePath()), QSettings::IniFormat, this);
    qDebug() << cache->fileName();

    logModel = new GitLogModel(this);
    logModel->setRepository(repo);
    ui->logView->setModel(logModel);

    filesModel = new GitCommitFiles(this);
    ui->commitView->setModel(filesModel);

    connect(ui->actionRepoOpen, SIGNAL(triggered(bool)), this, SLOT(openRepository()));
    connect(ui->logView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(commitSelected(QModelIndex)));
    connect(ui->commitView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(fileClicked(QModelIndex)));

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

    QString path = cache->value("repo/path", "").toString();
    if ( path != "" )
    {
        openRepository(path);
    }
}

LogWindow::~LogWindow()
{
    delete ui;
    delete repo;
}

void LogWindow::openRepository()
{
    qDebug() << "openRepository";
    QString path = QFileDialog::getExistingDirectory(this, "Choose repository");

    openRepository(path);
}

void LogWindow::openRepository(const QString &path)
{
    if ( repo->open(path) )
    {
        cache->setValue("repo/path", path);
        logModel->open(repo->head());
    }
}

void LogWindow::commitSelected(const QModelIndex &index)
{
    if ( !index.isValid() )
    {
        return;
    }

    Commit commit = logModel->getCommit(index);

    QString message = QString("SHA-1: %1\n\n%2").arg(QString(commit.oid().format())).arg(commit.message());
    ui->commitMessage->setText(message);
    filesModel->open(repo, commit);
}

void LogWindow::fileClicked(const QModelIndex &index)
{
    filesModel->execute(index);
}

void LogWindow::resizeEvent(QResizeEvent *event)
{
    cache->setValue("window/maximized", isMaximized() ? "yes" : "no");
    cache->setValue("window/width", event->size().width());
    cache->setValue("window/height", event->size().height());
}
