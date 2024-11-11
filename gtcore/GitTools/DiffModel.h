#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QColor>
#include <QList>
#include <GitTools/base.h>

class DiffModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text NOTIFY textChanged FINAL)

public:

    struct LineInfo
    {
        int lineNumber;
        QString lineText;
        QColor lineColor;
    };

    enum Roles
    {
        LineTextRole = Qt::DisplayRole,
        LineColorRole = Qt::UserRole,
        LineNumberRole
    };

private:

    QList<LineInfo> m_items;
    QString m_text;

public:

    explicit DiffModel(QObject *parent = nullptr);
    DiffModel(const DiffModel &) = delete;
    DiffModel(DiffModel &&) = delete;
    ~DiffModel();

    DiffModel& operator = (const DiffModel &) = delete;
    DiffModel& operator = (DiffModel &&) = delete;

    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void loadFromFile(const QString &path);
    void setContent(const QByteArray &data);

    static QColor addedColor() { return QColor{"aquamarine"}; }
    static QColor removedColor() { return QColor{"crimson"}; }

    void setLineColor(int row, QColor color);

    void clear();
    void addLine(const QString &text, QColor color);

    void setDiff(const QByteArray &left, const QByteArray &right);
    void setGitDelta(git::repository *repo, git::delta delta);

    QString text() { return m_text; }

signals:

    void textChanged();

};
