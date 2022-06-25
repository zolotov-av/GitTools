#include "DBusInterface.h"

#include <QDBusReply>
#include <stdio.h>

namespace git
{

    DBusAdaptor::DBusAdaptor(DBusInterface *obj): QDBusAbstractAdaptor(obj), m_obj(obj)
    {

    }

    DBusAdaptor::~DBusAdaptor()
    {

    }

    bool DBusAdaptor::isRunning()
    {
        return true;
    }

    bool DBusAdaptor::showGitLog()
    {
        printf("emit showGitLogRequested()\n");
        emit m_obj->showGitLogRequested();
        return true;
    }

    bool DBusAdaptor::exit()
    {
        printf("emit exitRequested()\n");
        emit m_obj->exitRequested();
        return true;
    }

    DBusInterface::DBusInterface(QObject *parent):
        QDBusInterface(DBusServiceName, DBusServicePath, DBusServiceName, QDBusConnection::sessionBus(), parent)
    {

    }

    DBusInterface::~DBusInterface()
    {

    }

    bool DBusInterface::registerObject()
    {
        m_service = new DBusAdaptor(this);
        return QDBusConnection::sessionBus().registerObject(DBusServicePath, this) &&
                QDBusConnection::sessionBus().registerService(DBusServiceName);
    }

    bool DBusInterface::isRunning()
    {
        const QDBusReply<bool> reply = call("isRunning");
        if ( reply.isValid() )
        {
            return reply.value();
        }
        else
        {
            return false;
        }
    }

    bool DBusInterface::showGitLog()
    {
        const QDBusReply<bool> reply = call("showGitLog");
        if ( reply.isValid() )
        {
            return reply.value();
        }
        else
        {
            return false;
        }
    }

}
