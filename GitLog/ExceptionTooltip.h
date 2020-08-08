#ifndef GT_EXCEPTION_TOOLTIP_H
#define GT_EXCEPTION_TOOLTIP_H

#include <QWidget>
#include <QTimer>
#include "ui_ExceptionTooltip.h"
#include <GitTools/exception.h>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

class ExceptionTooltip: public QWidget, protected Ui::ExceptionTooltip
{
    Q_OBJECT

protected:

    static constexpr auto CLOSE_TIMEOUT = 4 * 1000;

    QTimer *closeTimer = nullptr;

    void keyPressEvent(QKeyEvent *event) override
    {
        qDebug() << "ExceptionTooltip keyPressEvent";
        if ( event->key() == Qt::Key_Escape && event->key() == Qt::Key_Return )
        {
            deleteLater();
            return;
        }
    }

    void showEvent(QShowEvent *) override
    {
        qDebug() << "ExceptionTooltip showEvent";
    }

    void hideEvent(QHideEvent *) override
    {
        qDebug() << "ExceptionTooltip hideEvent";
        deleteLater();
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        qDebug() << "ExceptionTooltip mousePressEvent";
        QWidget::mousePressEvent(event);
        deleteLater();
    }

    void placeCentered(const QRect &rect)
    {
        qDebug() << "place centered";

        QSize sz = size();
        const int x = rect.x() + (rect.width() - sz.width()) / 2;
        const int y = rect.y() + (rect.height() - sz.height()) / 2;
        move(x, y);
    }

    void placeBelow(const QRect &rect)
    {
        qDebug() << "place below";

        move(rect.x(), rect.bottom() + 4);
    }

public:

    explicit ExceptionTooltip(): QWidget(nullptr, Qt::Popup)
    {
        qDebug() << "ExceptionTooltip created";
        setupUi(this);

        setAutoFillBackground(true);
        setBackgroundRole(QPalette::ToolTipBase);
        lCaption->setForegroundRole(QPalette::ToolTipText);
        lMessage->setForegroundRole(QPalette::ToolTipText);

        closeTimer = new QTimer(this);
        closeTimer->setSingleShot(true);
        closeTimer->start(CLOSE_TIMEOUT);
        connect(closeTimer, SIGNAL(timeout()), this, SLOT(deleteLater()));
    }

    ~ExceptionTooltip()
    {
        qDebug() << "ExceptionTooltip destroyed";
    }

    void setMessage(const QString &caption, const QString &message)
    {
        lCaption->setText(caption);
        lMessage->setTextFormat(Qt::PlainText);
        lMessage->setText(message);
    }

    void show()
    {
        QWidget::show();
    }

    void showCentered(const QWidget *parent)
    {
        placeCentered(parent->geometry());
        show();
    }

    void showBelow(const QWidget *parent)
    {
        placeBelow(parent->geometry());
    }

    static void show(const QWidget *parent, const QString &caption, const QString &message)
    {
        auto form = new ExceptionTooltip();
        form->setMessage(caption, message);
        if ( parent )
        {

            form->placeCentered(parent->geometry());
        }
        form->show();
    }

    static void show(const QWidget *parent, const std::exception &e)
    {
        show(parent, tr("Error"), e.what());
    }

    static void showBelow(QWidget *parent, const QString &caption, const QString &message)
    {
        auto form = new ExceptionTooltip();
        form->setMessage(caption, message);
        QPoint pos = parent->mapToGlobal({0, 0});
        QRect rect;
        rect.setX(pos.x());
        rect.setY(pos.y());
        rect.setWidth(parent->width());
        rect.setHeight(parent->height());
        form->resize(rect.width(), form->height());
        form->placeBelow( rect );
        form->show();
    }

    static void showBelow(QWidget *parent, const std::exception &e)
    {
        showBelow(parent, tr("Error"), e.what());
    }

    static void showMessage(QWidget *parent, const QString &message)
    {
        show(parent, tr("Message"), message);
    }

    static void showError(QWidget *parent, const QString &message)
    {
        show(parent, tr("Error"), message);
    }

    static void showException(QWidget *parent, const std::exception &e)
    {
        show(parent, tr("Exception"), e.what());
    }

};

#endif // GT_EXCEPTION_TOOLTIP_H
