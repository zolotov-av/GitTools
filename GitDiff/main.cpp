#include <QApplication>
#include <GitTools/GitApplication.h>
#include <git2.h>
#include "GitDiff.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GitApplication gitApp;

    GitDiff view;
    view.show();

    return a.exec();
}
