#ifndef GT_DBUS_INTERFACE_H
#define GT_DBUS_INTERFACE_H

#include <QDBusInterface>
#include <QDBusAbstractAdaptor>
#include <QDBusVariant>

namespace git
{

    constexpr auto DBusServiceName = "net.shamangrad.GitTools";
    constexpr auto DBusServicePath = "/";

    class DBusInterface;

    class DBusAdaptor: public QDBusAbstractAdaptor
    {
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", "net.shamangrad.GitTools")

        friend DBusInterface;

    private:

        DBusInterface *m_obj;

        explicit DBusAdaptor(DBusInterface *obj);
        DBusAdaptor(const DBusAdaptor &) = delete;
        DBusAdaptor(DBusAdaptor &&) = delete;

    public:

        ~DBusAdaptor();

        DBusAdaptor& operator = (DBusAdaptor &) = delete;
        DBusAdaptor& operator = (DBusAdaptor &&) = delete;

    public slots:

        bool isRunning();
        bool showGitLog();
        bool exit();

    };

    class DBusInterface: public QDBusInterface
    {
        Q_OBJECT

    private:

        DBusAdaptor *m_service {nullptr};

    public:

        explicit DBusInterface(QObject *parent = nullptr);
        DBusInterface(DBusInterface &) = delete;
        DBusInterface(DBusInterface &&) = delete;

        ~DBusInterface();

        DBusInterface& operator = (DBusInterface &) = delete;
        DBusInterface& operator = (DBusInterface &&) = delete;

        bool registerObject();
        bool isRunning();
        bool showGitLog();

    signals:

        void showGitLogRequested();
        void exitRequested();

    };

}

#endif // GT_DBUS_INTERFACE_H
