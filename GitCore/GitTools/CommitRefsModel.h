#pragma once

#include <QAbstractItemModel>
#include <QList>
#include <QColor>
#include <GitTools/base.h>

class CommitRefsModel final: public QAbstractItemModel
{
    Q_OBJECT

public:

    enum Roles
    {
        RefNameRole = Qt::DisplayRole,
        RefColorRole = Qt::BackgroundRole,
        RefTypeRole = Qt::UserRole
    };

    struct Item
    {
        QString refName;
        const char *refType;
        QColor refColor;
    };

private:

    QList<Item> m_refs;

public:

    explicit CommitRefsModel(QObject *parent = nullptr);
    CommitRefsModel(const CommitRefsModel &other);
    CommitRefsModel(CommitRefsModel &&other);
    ~CommitRefsModel();

    CommitRefsModel& operator = (const CommitRefsModel &) = delete;
    CommitRefsModel& operator = (CommitRefsModel &&) = delete;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void append(const git::reference_info &ref);

};
