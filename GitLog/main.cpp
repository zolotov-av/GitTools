#include "LogWindow.h"
#include "DBusInterface.h"
#include <QApplication>
#include <GitTools/GraphItem.h>
#include <git2.h>

using namespace git;

int main(int argc, char *argv[])
{
    git_libgit2_init();

    QApplication a(argc, argv);
    DBusInterface dbus;

    if ( dbus.isRunning() )
    {
        printf("already running...\n");
        dbus.showGitLog();
        return 0;
    }

    qmlRegisterType<GraphItem>("GitTools", 1, 0, "GraphItem");

    LogWindow w;

    QObject::connect(&dbus, &DBusInterface::showGitLogRequested, &w, &LogWindow::openFromTray);
    QObject::connect(&dbus, &DBusInterface::exitRequested, &w, &LogWindow::exit);

    if ( !dbus.registerObject() )
    {
        printf("cannot register dbus service...");
        return 1;
    }

    w.show();

    int exitCode = a.exec();

    git_libgit2_shutdown();

    return exitCode;
}
