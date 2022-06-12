#include "GitLogDelegate.h"

#include <QPainter>
#include <QColor>
#include <QDebug>

#include <GitTools/GitLogModel.h>

constexpr int GRAPH_COL = 0;
constexpr int LOG_COL = 1;

// colors
const QColor BROWN        = QColor(150, 75, 0);
const QColor ORANGE       = QColor(255, 160, 50);
const QColor DARK_ORANGE  = QColor(216, 144, 0);
const QColor LIGHT_ORANGE = QColor(255, 221, 170);
const QColor LIGHT_BLUE   = QColor(85, 255, 255);
const QColor PURPLE       = QColor(221, 221, 255);
const QColor DARK_GREEN   = QColor(0, 205, 0);

const QColor GT_Green("#00B64F");
const QColor GT_Red("#FF3500");
const QColor GT_Orange("#FF8900");
const QColor GT_Blue("#086FA1");
const QColor GT_Tag("#febc70");

GitLogDelegate::GitLogDelegate(QObject *parent): QStyledItemDelegate(parent)
{
    qDebug() << "QGitLogDelegate create";
}

static inline int get_border(const QFontMetrics &fm)
{
    return fm.lineWidth();
}

static inline int get_spacing(const QFontMetrics &fm)
{
    return fm.lineWidth() * 5 / 2;
}

static inline int get_row_height(const QFontMetrics &fm)
{
    return get_border(fm)*6 + fm.height();
}


void GitLogDelegate::paintBackground(QPainter *p, const QStyleOptionViewItem &opt) const
{
    const auto brush = (opt.state & QStyle::State_Selected) ? opt.palette.highlight() : opt.backgroundBrush;
    p->fillRect(opt.rect, brush);
}

void GitLogDelegate::paintGraph(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    paintBackground(p, opt);

    const GitLogModel *model = static_cast<const GitLogModel*>(index.model());

    git::CommitInfo commit = model->getCommitInfo(index);

    QFontMetrics fm(opt.font);
    //qDebug() << "fm.height=" << fm.height() << "box.height=" << box.height();

    const int rowHeight = get_row_height(fm);
    const int spacing = get_spacing(fm);
    //const int border = get_border(fm);

    p->save();
    p->setClipRect(opt.rect, Qt::IntersectClip);
    p->translate(opt.rect.topLeft());

    auto size = spacing * 5;
    auto offset = (rowHeight - size) / 2;
    auto half = rowHeight / 2;
    auto xlane = commit.lane * rowHeight;

    p->setPen(QPen{Qt::black, spacing * 2.0, Qt::SolidLine});
    for(int li = 0; li < commit.lanes.size(); li++)
    {
        auto lane = commit.lanes[li];
        if ( !lane.isFree() && li != commit.lane )
        {
            int offset = li * rowHeight;
            switch (lane.type)
            {
            case GraphLane::start:
                p->drawLine(offset, half, offset + half, rowHeight);
                break;
            case GraphLane::line:
                p->drawLine(offset + half, 0, offset + half, rowHeight);
                break;
            case GraphLane::end:
                p->drawLine(offset + half, 0, offset, half);
                break;
            default:
                p->drawLine(offset + half, rowHeight, offset + half*2, half);
            }

        }
    }

    p->setPen(Qt::NoPen);
    Qt::GlobalColor clr = Qt::black;
    if ( commit.isTail() ) clr = Qt::blue;
    else if ( commit.isRoot() ) clr = Qt::darkGreen;
    else if ( commit.isMerge() || commit.isBranch() ) clr = Qt::red;
    p->setBrush(clr);

    if ( commit.isMerge() )
    {
        p->drawRect(xlane + offset, offset, size, size);
    }
    else
    {
        p->drawEllipse(xlane + offset, offset, size, size);
    }

    p->setPen(QPen{Qt::black, spacing * 2.0, Qt::SolidLine});

    if ( commit.down )
    {
        p->drawLine(xlane + half, half, xlane + half, rowHeight);
    }

    if ( commit.up )
    {
        p->drawLine(xlane + half, half, xlane + half, 0);
    }

    if ( commit.isMerge() || commit.isBranch() )
    {
        int i = commit.lanes.size()-1;
        while ( i > commit.lane && (commit.lanes[i].type == GraphLane::free || commit.lanes[i].type == GraphLane::line) ) i--;
        p->drawLine(xlane + half, half, xlane + rowHeight + (i - commit.lane - 1)*rowHeight, half);
        //qDebug() << "merge " << commit.message() << "lane:" << commit.lane << "last:" << i;
    }

    p->restore();
}

void GitLogDelegate::paintRef(QPainter *p, QStyleOptionViewItem &opt, const git::reference_info &ref) const
{
    QFontMetrics fm(opt.font);
    const QString name = ref.short_name;
    QRect box = fm.boundingRect(name);
    //qDebug() << "fm.height=" << fm.height() << "box.height=" << box.height();

    const int spacing = get_spacing(fm);
    const int border = get_border(fm);

    p->save();
    QPen pen = p->pen();
    pen.setColor(Qt::black);
    pen.setWidth(border);
    p->setPen(pen);

    QRect pbox(opt.rect.left() + border, opt.rect.top() + border, box.width() + 2*spacing + border, fm.height() + 3*border);

    if ( ref.isHead ) p->setBrush(GT_Red);
    else if ( ref.isBranch ) p->setBrush(GT_Green);
    else if ( ref.isTag ) p->setBrush(GT_Tag);
    else p->setBrush(GT_Orange);
    p->setRenderHints(QPainter::Antialiasing, false);
    //p->drawRoundedRect(pbox, 2*spacing, 2*spacing, Qt::AbsoluteSize);
    p->drawRect(pbox);

    p->setRenderHints(QPainter::Antialiasing, true);
    p->drawText(pbox, Qt::AlignCenter, name);
    p->restore();

    opt.rect.adjust(pbox.width() + spacing, 0, 0, 0);
}

void GitLogDelegate::paintLog(QPainter *p, const QStyleOptionViewItem &o, const QModelIndex &index) const
{
    paintBackground(p, o);

    const GitLogModel *model = static_cast<const GitLogModel*>(index.model());
    auto commit = model->getCommitInfo(index);

    QStyleOptionViewItem opt(o); // we need a copy

    for(const auto &ref : model->refs)
    {
        if ( (ref.isBranch || ref.isRemote || (ref.isTag && m_display_tags)) && (ref.target == commit.oid()) )
        {
            paintRef(p, opt, ref);
        }
    }

    //if (r->isDiffCache)
    //	p->fillRect(opt.rect, changedFiles(ZERO_SHA) ? ORANGE : DARK_ORANGE);

    //if (diffTargetRow == row)
    //    p->fillRect(opt.rect, LIGHT_BLUE);

    //bool isHighlighted = lp->isHighlighted(row);
    //QPixmap* pm = getTagMarks(r->sha(), opt);

    //if (!pm && !isHighlighted) { // fast path in common case
    //	QItemDelegate::paint(p, opt, index);
    //	return;
    //}
    //if (pm) {
    //	p->drawPixmap(newOpt.rect.x(), newOpt.rect.y(), *pm);
        //newOpt.rect.adjust(box.width() + 2*spacing, 0, 0, 0);
    //	delete pm;
    //}
    //if (isHighlighted)
    //	newOpt.font.setBold(true);

    //QPen pen = p->pen();
    //pen.setWidth(border);
    //p->setPen(Qt::NoPen);
    //p->setBrush(DARK_GREEN);
    //p->drawRoundedRect(spacing*2, spacing*2, pw, ph, border*4, border*4, Qt::AbsoluteSize);
    //p->setPen(Qt::NoPen);
    //p->setBrush(LIGHT_BLUE);
    //p->drawRect(spacing, spacing, pw, ph);

    QFontMetrics fm(opt.font);
    const int spacing = get_spacing(fm);
    p->save();
    if ( opt.state & QStyle::State_Selected )
    {
        p->setPen(opt.palette.color(QPalette::Active, QPalette::HighlightedText));
    }
    else
    {
        p->setPen(opt.palette.color(QPalette::Active, QPalette::Text));
    }

    //p->translate(opt.rect.left(), opt.rect.top());
    p->drawText(opt.rect.adjusted(spacing, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
    p->restore();
    //QStyledItemDelegate::paint(p, opt, index);
}

void GitLogDelegate::paint(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    p->setRenderHints(QPainter::Antialiasing);

    if (index.column() == GRAPH_COL)
        return paintGraph(p, opt, index);

    if (index.column() == LOG_COL)
        return paintLog(p, opt, index);

    return QStyledItemDelegate::paint(p, opt, index);
}

QSize GitLogDelegate::sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &) const
{
    QFontMetrics fm(opt.font);
    return QSize( 15*fm.averageCharWidth(), get_row_height(fm) );
    //return QSize(laneWidth(), laneHeight);
}
