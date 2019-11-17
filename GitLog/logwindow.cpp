#include "logwindow.h"
#include "ui_logwindow.h"

#include "gitlogview.h"

#include <QDebug>
#include <QFileDialog>

LogWindow::LogWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);

    logView = new GitLogView(this);
    ui->verticalLayout->addWidget(logView);

    connect(ui->actionRepoOpen, SIGNAL(triggered(bool)), this, SLOT(openRepository()));

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

    }

}
