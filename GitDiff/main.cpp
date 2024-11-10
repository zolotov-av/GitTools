#include <QApplication>
#include <git2.h>
#include "GitDiff.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    git_libgit2_init();

    GitDiff view;
    view.show();

    int exitCode = a.exec();

    git_libgit2_shutdown();

    return exitCode;
}
