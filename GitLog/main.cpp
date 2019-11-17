#include "logwindow.h"
#include <qgit2e.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    LibQGit2::initLibQGit2();

    QApplication a(argc, argv);
    LogWindow w;
    w.show();

    int exitCode = a.exec();

    LibQGit2::shutdownLibQGit2();

    return exitCode;
}
