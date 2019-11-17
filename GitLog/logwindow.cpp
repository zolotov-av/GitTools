#include "logwindow.h"
#include "ui_logwindow.h"

#include "gitlogview.h"

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

    logView = new GitLogView(this);
    ui->verticalLayout->addWidget(logView);

    connect(ui->actionRepoOpen, SIGNAL(triggered(bool)), this, SLOT(openRepository()));

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

void LogWindow::resizeEvent(QResizeEvent *event)
{
    cache->setValue("window/maximized", isMaximized() ? "yes" : "no");
    cache->setValue("window/width", event->size().width());
    cache->setValue("window/height", event->size().height());
}
