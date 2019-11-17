#include "gitlogview.h"
#include "gitlogmodel.h"
#include "gitcommitfiles.h"

#include <QDebug>
#include <QPainter>

#include <QVBoxLayout>
#include <QAbstractListModel>

GitLogView::GitLogView(QWidget *parent) : QWidget(parent)
{
    qDebug() << "GitLogView Created";

    model = new GitLogModel(this);

    view = new QTreeView(this);
    view->setModel(model);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(view);

    filesModel = new GitCommitFiles(this);
    filesView = new QTreeView(this);
    filesView->setModel(filesModel);
    vbox->addWidget(filesView);

    setLayout(vbox);

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(commitSelected(QModelIndex)));
    connect(filesView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));
}

GitLogView::~GitLogView()
{
    qDebug() << "GitLogView destroy";
}

QSize GitLogView::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize GitLogView::sizeHint() const
{
    return QSize(400, 200);
}

bool GitLogView::openRepository(const QString &path)
{
    return model->openRepository(path);
}

void GitLogView::paintEvent(QPaintEvent * /* event */)
{
    /*
    QPainter painter(this);
    painter.setPen(palette().dark().color());
    painter.setBrush(QColor(127, 191, 144));
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
    */
}

void GitLogView::commitSelected(const QModelIndex &index)
{
    if ( !index.isValid() )
    {
        return;
    }

    filesModel->open(model->getCommit(index), model->repo);
}

void GitLogView::doubleClicked(const QModelIndex &index)
{
    filesModel->execute(index);
}

