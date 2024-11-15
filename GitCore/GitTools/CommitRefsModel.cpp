#include "CommitRefsModel.h"

CommitRefsModel::CommitRefsModel(QObject *parent): QAbstractItemModel{parent}
{

}

CommitRefsModel::CommitRefsModel(const CommitRefsModel &other):
    QAbstractItemModel{nullptr}, m_refs(other.m_refs)
{

}

CommitRefsModel::CommitRefsModel(CommitRefsModel &&other):
    QAbstractItemModel{nullptr}, m_refs(other.m_refs)
{

}

CommitRefsModel::~CommitRefsModel()
{

}

int CommitRefsModel::rowCount(const QModelIndex &parent) const
{
    return m_refs.size();
}

int CommitRefsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex CommitRefsModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
    {
        return createIndex(row, column, 1);
    }

    return QModelIndex{ };
}

QModelIndex CommitRefsModel::parent(const QModelIndex &child) const
{
    return QModelIndex{ };
}

QVariant CommitRefsModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= m_refs.size() || index.column() != 0 )
        return { };

    switch ( role )
    {
    case RefNameRole:
        return m_refs[index.row()].refName;
    case RefColorRole:
        return m_refs[index.row()].refColor;
    case RefTypeRole:
        return m_refs[index.row()].refType;
    default:
        return { };
    }
}

QHash<int, QByteArray> CommitRefsModel::roleNames() const
{
    return {
        {RefNameRole, "refName"},
        {RefColorRole, "refColor"},
        {RefTypeRole, "refType"}
    };
}

static const char* getRefType(const git::reference_info &ref)
{
    if ( ref.isHead )
        return "HEAD";

    if ( ref.isBranch )
        return "branch";

    if ( ref.isTag )
        return "tag";

    return "other";
}

static QColor getRefColor(const git::reference_info &ref)
{
    const QColor GT_Green("#00B64F");
    const QColor GT_Red("#FF3500");
    const QColor GT_Orange("#FF8900");
    const QColor GT_Blue("#086FA1");
    const QColor GT_Tag("#febc70");

    if ( ref.isHead )
        return GT_Red;

    if ( ref.isBranch )
        return GT_Green;

    if ( ref.isTag )
        return GT_Tag;

    return GT_Orange;
}

void CommitRefsModel::append(const git::reference_info &ref)
{
    m_refs.append({ref.short_name, getRefType(ref), getRefColor(ref)});
}
