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

    constexpr auto GitToolsModName = "GitTools";
    constexpr int GitToolsVerMajor = 1;
    constexpr int GitToolsVerMinor = 0;
    // @uri GitTools
    qmlRegisterModule(GitToolsModName, GitToolsVerMajor, GitToolsVerMinor);
    qmlRegisterType<GraphItem>(GitToolsModName, GitToolsVerMajor, GitToolsVerMinor, "GraphItem");
    qmlRegisterType(QUrl{"qrc:/qml/LineInput.qml"}, GitToolsModName, GitToolsVerMajor, GitToolsVerMinor, "LineInput");
    qmlRegisterType(QUrl{"qrc:/qml/MultiLineInput.qml"}, GitToolsModName, GitToolsVerMajor, GitToolsVerMinor, "MultiLineInput");
    qmlRegisterSingletonType(QUrl{"qrc:/qml/GitTheme.qml"}, GitToolsModName, GitToolsVerMajor, GitToolsVerMinor, "GitTheme");

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
