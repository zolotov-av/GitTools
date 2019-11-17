#include "gitdiffprocess.h"

#include <QTimer>
#include <QProcess>
#include <QTemporaryFile>
#include <QDebug>

using namespace LibQGit2;

GitDiffProcess::GitDiffProcess(QObject *parent) : QObject(parent)
{

}

GitDiffProcess::~GitDiffProcess()
{
    qDebug() << "GitDiffProcess destroy";
}

bool GitDiffProcess::open(LibQGit2::Blob a, LibQGit2::Blob b)
{
    aFile = new QTemporaryFile(this);
    bFile = new QTemporaryFile(this);

    aFile->open();
    bFile->open();

    QString oldPath = aFile->fileName();
    QString newPath = bFile->fileName();

    aFile->write(a.content());
    aFile->flush();

    bFile->write(b.content());
    bFile->flush();

    QString command = QString("/usr/bin/meld %1 %2").arg(oldPath).arg(newPath);
    qDebug() << "command" << command;

    if ( QProcess::startDetached(command) )
    {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(deleteLater()));

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
