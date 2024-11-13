#include "QuickDialog.h"
#include <QQmlComponent>
#include <QQmlContext>

QuickDialog::QuickDialog(QQmlEngine *engine): QObject{engine}
{
    m_context = new QQmlContext(engine->rootContext(), this);
}

QuickDialog::~QuickDialog()
{
    unload();

    if (m_context)
    {
        m_context->setParent(nullptr);
        m_context->deleteLater();
        m_context = nullptr;
    }
}

void QuickDialog::load(QQmlEngine *engine, const QUrl &url)
{
    QQmlComponent component(engine, url);
    if (component.status() == QQmlComponent::Ready)
    {
        m_root_object = component.create(m_context);
        if (m_root_object)
        {
            m_root_object->setParent(this);
            return;
        }
    }
    else
    {
        qWarning() << "Failed to create dialog:" << component.errorString();
    }
}

void QuickDialog::unload()
{
    if ( m_root_object == nullptr )
        return;

    m_root_object->setParent(nullptr);

    QQmlEngine *engine = qmlEngine(m_root_object);

    m_root_object->deleteLater();
    m_root_object = nullptr;

    if (engine)
        engine->collectGarbage();
}

void QuickDialog::show()
{
    if (m_root_object)
    {
        QMetaObject::invokeMethod(m_root_object, "show");
    }
}

void QuickDialog::hide()
{
    if (m_root_object)
    {
        QMetaObject::invokeMethod(m_root_object, "hide");
    }
}

void QuickDialog::close()
{
    if (m_root_object)
    {
        QMetaObject::invokeMethod(m_root_object, "close");
    }
}
