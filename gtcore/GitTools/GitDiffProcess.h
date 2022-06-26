#ifndef GT_GITDIFF_PROCESS_H
#define GT_GITDIFF_PROCESS_H

#include <QObject>
#include <QTimer>
#include <QTemporaryFile>
#include <GitTools/base.h>

class GitDiffProcess : public QObject
{
    Q_OBJECT
public:
    explicit GitDiffProcess(QObject *parent = nullptr);
    ~GitDiffProcess();

    bool open(const git::blob &a, const git::blob &b);
    bool open(const QString &a, const git::blob &b);
    bool open(const git::blob &a, const QString &b);
    bool open(const QString &a, const QString &b);

signals:

public slots:

private:
    class QTimer *timer;
    class QTemporaryFile *aFile;
    class QTemporaryFile *bFile;

};

#endif // GT_GITDIFF_PROCESS_H
