#ifndef QGITLOGDELEGATE_H
#define QGITLOGDELEGATE_H

#include <QStyledItemDelegate>
#include <qgit2e.h>

class QGitLogDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    QGitLogDelegate(QObject *parent = nullptr);

    //ListViewDelegate(Git* git, ListViewProxy* lp, QObject* parent);

    virtual void paint(QPainter* p, const QStyleOptionViewItem& o, const QModelIndex &i) const;
    virtual QSize sizeHint(const QStyleOptionViewItem& o, const QModelIndex &i) const;
    //int laneWidth() const { return 3 * laneHeight / 4; }
    //void setLaneHeight(int h) { laneHeight = h; }

signals:
    //void updateView();

public slots:
    //void diffTargetChanged(int);

private:
    //const Rev* revLookup(int row, FileHistory** fhPtr = NULL) const;
    void paintGraph(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& i) const;
    void paintRef(QPainter *p, QStyleOptionViewItem &opt, const LibQGit2::Reference &ref) const;
    void paintLog(QPainter *p, const QStyleOptionViewItem& o, const QModelIndex &i) const;
    //void paintGraph(QPainter* p, const QStyleOptionViewItem& o, const QModelIndex &i) const;
    //void paintGraphLane(QPainter* p, int type, int x1, int x2, const QColor& col, const QColor& activeCol, const QBrush& back) const;
    //QPixmap* getTagMarks(SCRef sha, const QStyleOptionViewItem& opt) const;
    //void addRefPixmap(QPixmap** pp, SCRef sha, int type, QStyleOptionViewItem opt) const;
    //void addTextPixmap(QPixmap** pp, SCRef txt, const QStyleOptionViewItem& opt) const;
    //bool changedFiles(SCRef sha) const;

    //Git* git;
    //ListViewProxy* lp;
    //int laneHeight;
    //int diffTargetRow;
};

#endif // QGITLOGDELEGATE_H
