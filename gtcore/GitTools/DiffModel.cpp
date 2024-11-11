#include "DiffModel.h"
#include <QFile>
#include <QBuffer>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <GitTools/base.h>

namespace
{

    struct PayloadData
    {
        QList<DiffModel::LineInfo> *items;
        QStringList left;
        QStringList right;
        QColor addedColor;
        QColor removedColor;
        int start;

        void append(const QString &line, QColor color)
        {
            DiffModel::LineInfo item;
            item.lineNumber = items->size() + 1;
            item.lineText = line;
            item.lineColor = color;
            items->append(std::move(item));
        }
    };

    QByteArray readFromFile(const QString &filePath)
    {
        QFile file{filePath};
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return { };
        }

        return file.readAll();
    }

    QStringList splitLines(const QByteArray &data)
    {
        QStringList items;

        QByteArray data2 { data };
        QBuffer file(&data2);
        if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            while ( !file.atEnd() )
            {
                QString line = QString::fromUtf8( file.readLine() );
                if ( line.endsWith(QChar{'\n'}) )
                    line = line.mid(0, line.length()-1);

                items.append(std::move(line));
            }

        }
        else
        {
            QMessageBox::warning(nullptr, "Error", "Could not open file: " + file.errorString());
        }

        return items;
    }

    QString joinLines(const QList<DiffModel::LineInfo> &items)
    {
        int size = items.size();
        for(const auto &item : items)
        {
            size += item.lineText.length();
        }

        QString text;
        text.reserve(size);

        for(const auto &item : items)
        {
            text.append(item.lineText).append(QChar('\n'));
        }

        return text;
    }

    int hunk_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, void *payload)
    {
        const auto data = reinterpret_cast<PayloadData*>(payload);

        if ( hunk->old_start > data->start )
        {
            for(int i = data->start; i <= hunk->old_start; i++)
            {
                data->append(data->left[i-1], Qt::transparent);
            }
        }

        data->start = hunk->old_start + hunk->old_lines;

        return 0;
    }

    int line_cb(const git_diff_delta *delta, const git_diff_hunk *hunk, const git_diff_line *line, void *payload)
    {
        const auto data = reinterpret_cast<PayloadData*>(payload);

        if ( line->old_lineno == -1 )
        {
            data->append(data->right[line->new_lineno-1], data->addedColor);
        }
        else if ( line->new_lineno == -1 )
        {
            data->append(data->left[line->old_lineno-1], data->removedColor);
        }
        else
        {
            data->append(data->left[line->old_lineno-1], Qt::transparent);
        }

        return 0;
    }

} // namespace

DiffModel::DiffModel(QObject *parent): QAbstractListModel{parent}
{

}

DiffModel::~DiffModel()
{

}

QModelIndex DiffModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || row >= m_items.size() || column != 0 || parent.isValid()) {
        return QModelIndex();
    }

    return createIndex(row, column);
}

QModelIndex DiffModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int DiffModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
        return 0;

    return m_items.size();
}

QVariant DiffModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) {
        return QVariant{};
    }

    const LineInfo &line = m_items[index.row()];

    switch (role)
    {
    case LineNumberRole:
        return line.lineNumber;
    case LineTextRole:
        return line.lineText;
    case LineColorRole:
        return line.lineColor;
    default:
        return QVariant{};
    }
}

QHash<int, QByteArray> DiffModel::roleNames() const
{
    return {
        {LineNumberRole, "lineNumber"},
        {LineTextRole, "lineText"},
        {LineColorRole, "lineColor"}
    };
}

void DiffModel::loadFromFile(const QString &path)
{
    beginResetModel();
    m_items.clear();

    int lineNumber = 0;
    QFile file{path};
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        while ( !file.atEnd() )
        {
            LineInfo line;
            line.lineNumber = ++lineNumber;
            line.lineText = file.readLine();
            if ( line.lineText.endsWith(QChar{'\n'}) )
                line.lineText = line.lineText.mid(0, line.lineText.length()-1);

            line.lineColor = QColor{"transparent"};
            m_items.append(std::move(line));
        }

    }
    else
    {
        QMessageBox::warning(nullptr, "Error", "Could not open file: " + path + "\nError: " + file.errorString());
    }

    file.close();

    if ( m_items.size() > 3 )
    {
        m_items[2].lineColor = QColor{"red"};
    }

    endResetModel();
}

void DiffModel::setContent(const QByteArray &data)
{
    beginResetModel();
    m_items.clear();

    m_text = QString::fromUtf8(data);

    int lineNumber = 0;
    QByteArray data2 { data };
    QBuffer file(&data2);
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        while ( !file.atEnd() )
        {
            LineInfo line;
            line.lineNumber = ++lineNumber;
            line.lineText = file.readLine();
            if ( line.lineText.endsWith(QChar{'\n'}) )
                line.lineText = line.lineText.mid(0, line.lineText.length()-1);

            line.lineColor = QColor{"transparent"};
            m_items.append(std::move(line));
        }

    }
    else
    {
        QMessageBox::warning(nullptr, "Error", "Could not open file: " + file.errorString());
    }

    endResetModel();

    emit textChanged();
}

void DiffModel::setLineColor(int row, QColor color)
{
    if ( row < 0 || row >= m_items.size() )
        return;

    m_items[row].lineColor = color;

    //const auto modelIndex = index(row);
    //emit dataChanged(modelIndex, modelIndex);
}

void DiffModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}

void DiffModel::addLine(const QString &text, QColor color)
{
    beginInsertRows({}, m_items.size(), m_items.size());

    LineInfo line;
    line.lineNumber = m_items.size() + 1;
    line.lineText = text;
    if ( line.lineText.endsWith(QChar{'\n'}) )
        line.lineText = line.lineText.mid(0, line.lineText.length()-1);

    m_items.append(std::move(line));

    endInsertRows();
}

void DiffModel::setDiff(const QByteArray &left, const QByteArray &right)
{
    beginResetModel();
    m_items.clear();

    PayloadData payload {
        &m_items,
        splitLines(left),
        splitLines(right),
        addedColor(),
        removedColor(),
        1 // start
    };

    const auto ret = git_diff_buffers(left.constData(), left.size(), nullptr,
                                      right.constData(), right.size(), nullptr,
                                      nullptr, nullptr, nullptr, &hunk_cb, &line_cb, &payload);


    if ( ret == 0 )
    {
        for(int i = payload.start; i <= payload.left.size(); i++)
        {
            payload.append(payload.left[i-1], Qt::transparent);
        }

        m_text = joinLines(m_items);
    }
    else
    {
        m_items.clear();
        m_text = git::errorString(ret);
    }

    endResetModel();
}

void DiffModel::setGitDelta(git::repository *repo, git::delta delta)
{

    switch (delta.type())
    {
    case GIT_DELTA_IGNORED:
    case GIT_DELTA_UNTRACKED:
    {
        const QString path = QDir(repo->workdir()).filePath(delta.newFile().path());
        if ( QFileInfo(path).isFile() )
        {
            //const QString command = QStringLiteral("xdg-open %1").arg(path);
            qDebug().noquote() << "GIT_DELTA_UNTRACKED";
            //QProcess::startDetached(command);
            setDiff(QByteArray{}, readFromFile(path));
            return;
        }
        else
        {
            qDebug() << path << "is not file";
        }
        return;
    }
    default:
        break;
    }

    const auto oldFile = delta.oldFile();
    const auto newFile = delta.newFile();

    const auto oldOid = oldFile.oid();
    const auto newOid = newFile.oid();
    qDebug().noquote() << "old oid[" << oldOid.toString() << "] " << oldFile.path();
    qDebug().noquote() << "new oid[" << newOid.toString() << "] " << newFile.path();

    if ( oldOid.isNull() )
    {
        if ( newOid.isNull() )
        {
            qDebug().noquote() << "unexpected old oid & new oid both is zero";
            return;
        }
        else
        {
            const QString oldPath = QDir(repo->workdir()).filePath(delta.oldFile().path());
            const git::blob newBlob = repo->get_blob(newFile.oid().data());
            setDiff(readFromFile(oldPath), newBlob.rawcontent());
            return;
        }
    }
    else
    {
        const git::blob oldBlob = repo->get_blob(oldFile.oid().data());
        if ( newOid.isNull() )
        {
            const QString newPath = QDir(repo->workdir()).filePath(delta.newFile().path());
            setDiff(oldBlob.rawcontent(), readFromFile(newPath));
            return;
        }
        else
        {
            const git::blob newBlob = repo->get_blob(newFile.oid().data());
            setDiff(oldBlob.rawcontent(), newBlob.rawcontent());
        }
    }

}
