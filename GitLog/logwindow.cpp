#include "logwindow.h"
#include "ui_logwindow.h"

#include "gitlogview.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

LogWindow::LogWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);

    cache = new QSettings(QString("%1/.cache/GitTools/GitLog.ini").arg(QDir::homePath()), QSettings::IniFormat, this);
    qDebug() << cache->fileName();

    logView = new GitLogView(this);
    ui->verticalLayout->addWidget(logView);

    connect(ui->actionRepoOpen, SIGNAL(triggered(bool)), this, SLOT(openRepository()));

    QString path = cache->value("repo/path", "").toString();
    if ( path != "" )
    {
        logView->openRepository(path);
    }
}

LogWindow::~LogWindow()
{
    delete ui;
}

void LogWindow::openRepository()
{
    qDebug() << "openRepository";
    QString path = QFileDialog::getExistingDirectory(this, "Choose repository");

    if ( logView->openRepository(path) )
    {
        cache->setValue("repo/path", path);
    }

}
