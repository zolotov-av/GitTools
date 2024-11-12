#pragma once

#include <QQuickPaintedItem>
#include <QAbstractItemModel>
#include <QDateTime>

class GraphItem: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(QAbstractItemModel* model READ model WRITE setModel NOTIFY modelChanged FINAL)
    Q_PROPERTY(int itemIndex READ index WRITE setIndex NOTIFY indexChanged FINAL)
    Q_PROPERTY(QDateTime time READ time WRITE setTime NOTIFY timeChanged FINAL)

private:

    QAbstractItemModel* m_model { nullptr };
    int m_index { -1 };
    QColor m_color { Qt::black };
    QDateTime m_time;

public:

    explicit GraphItem(QQuickItem *parent = nullptr);
    GraphItem(const GraphItem &) = delete;
    GraphItem(GraphItem &&) = delete;
    ~GraphItem();

    GraphItem& operator = (const GraphItem &) = delete;
    GraphItem& operator = (GraphItem &&) = delete;

    const QColor& color() const { return m_color; }
    void setColor(QColor c);

    QAbstractItemModel* model() { return m_model; }
    void setModel(QAbstractItemModel *m);

    int index() const { return m_index; }
    void setIndex(int idx);

    const QDateTime& time() const { return m_time; }
    void setTime(const QDateTime &tm);

    void paint(QPainter *p) override;

signals:

    void colorChanged();
    void modelChanged();
    void indexChanged();
    void timeChanged();

};
