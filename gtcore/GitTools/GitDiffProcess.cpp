#include "GitDiffProcess.h"

#include <QProcess>
#include <QDebug>

GitDiffProcess::GitDiffProcess(QObject *parent) : QObject(parent)
{

}

GitDiffProcess::~GitDiffProcess()
{
    qDebug() << "GitDiffProcess destroy";
}

bool GitDiffProcess::open(const git::blob &a, const git::blob &b)
{
    aFile = new QTemporaryFile(this);
    aFile->open();
    aFile->write(a.rawcontent());
    aFile->flush();

    bFile = new QTemporaryFile(this);
    bFile->open();
    bFile->write(b.rawcontent());
    bFile->flush();

    return open(aFile->fileName(), bFile->fileName());
}

bool GitDiffProcess::open(const QString &a, const git::blob &b)
{
    bFile = new QTemporaryFile(this);
    bFile->open();
    bFile->write(b.rawcontent());
    bFile->flush();

    return open(a, bFile->fileName());
}

bool GitDiffProcess::open(const git::blob &a, const QString &b)
{
    aFile = new QTemporaryFile(this);
    aFile->open();
    aFile->write(a.rawcontent());
    aFile->flush();

    return open(aFile->fileName(), b);
}

bool GitDiffProcess::open(const QString &a, const QString &b)
{
    const QString command = QString("/usr/bin/meld %1 %2").arg(a, b);
    qDebug() << "command" << command;

    if ( QProcess::startDetached(command) )
    {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &GitDiffProcess::deleteLater);
        timer->setInterval(2000);
        timer->start();
        return true;
    }
    else
    {
        delete this;
        return false;
    }
}
