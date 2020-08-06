#include "logwindow.h"
#include <QApplication>
#include <git2.h>

int main(int argc, char *argv[])
{
    git_libgit2_init();

    QApplication a(argc, argv);
    LogWindow w;
    w.show();

    int exitCode = a.exec();

    git_libgit2_shutdown();

    return exitCode;
}
