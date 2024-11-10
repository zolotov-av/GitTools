#include "GitDiff.h"
#include <QQmlContext>
#include <QMessageBox>
#include <QDir>
#include <QFile>

static QByteArray loadFromFile(const QString &filePath)
{
    QFile file{filePath};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Could not open file: " + filePath + "\nError: " + file.errorString());
        return { };
    }

    return file.readAll();
}

GitDiff::GitDiff(QWindow *parent): QQuickView{parent}
{
    setTitle(tr("Git Diff"));
    setResizeMode(QQuickView::SizeRootObjectToView);
    resize(QSize{1200, 800});

    diffFiles(":/tmp/left.txt", ":/tmp/right.txt");

    rootContext()->setContextProperty("lineView", this);
    rootContext()->setContextProperty("diff", this);

    setSource(QUrl{"qrc:/qml/GitDiff.qml"});
}

GitDiff::~GitDiff()
{

}

void GitDiff::diffData(const QByteArray &oldData, const QByteArray &newData)
{
    m_line_model.setDiff(oldData, newData);

    emit diffChanged();
}

void GitDiff::diffFiles(const QString &oldFile, const QString &newFile)
{
    m_old_file_name = oldFile;
    m_new_file_name = newFile;

    const QByteArray oldData = loadFromFile(oldFile);
    const QByteArray newData = loadFromFile(newFile);

    diffData(oldData, newData);
}
