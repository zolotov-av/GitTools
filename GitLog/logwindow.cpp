#include "logwindow.h"
#include "ui_logwindow.h"

#include "gitlogview.h"

LogWindow::LogWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);

    logView = new GitLogView(this);
    ui->verticalLayout->addWidget(logView);

}

LogWindow::~LogWindow()
{
    delete ui;
}
