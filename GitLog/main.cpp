#include "LogWindow.h"
#include "DBusInterface.h"
#include <QApplication>
#include <GitTools/GitApplication.h>
#include <GitTools/GraphItem.h>

using namespace git;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GitApplication gitApp;

    DBusInterface dbus;

    if ( dbus.isRunning() )
    {
        printf("already running...\n");
        dbus.showGitLog();
        return 0;
    }

    qmlRegisterType<GraphItem>("GitTools", 1, 0, "GraphItem");

    LogWindow w;

    QObject::connect(&dbus, &DBusInterface::showGitLogRequested, &w, &LogWindow::showWindowRequested, Qt::DirectConnection);
    QObject::connect(&dbus, &DBusInterface::exitRequested, &w, &LogWindow::exit, Qt::DirectConnection);

    if ( !dbus.registerObject() )
    {
        printf("cannot register dbus service...");
        return 1;
    }

    return a.exec();
}
