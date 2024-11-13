#pragma once

#include <QObject>
#include <QQmlEngine>

class QuickDialog: public QObject
{
    Q_OBJECT

private:

    QQmlContext *m_context { nullptr };
    QObject *m_root_object { nullptr };

public:

    explicit QuickDialog(QQmlEngine *engine);
    QuickDialog(const QuickDialog &) = delete;
    QuickDialog(QuickDialog &&) = delete;
    ~QuickDialog();

    QuickDialog& operator = (const QuickDialog &) = delete;
    QuickDialog& operator = (QuickDialog &&) = delete;

    QQmlContext* context() { return m_context; }
    QObject* rootObject() { return m_root_object; }
    void load(QQmlEngine *engine, const QUrl &url);
    void unload();

    Q_INVOKABLE void show();
    Q_INVOKABLE void hide();
    Q_INVOKABLE void close();

};
