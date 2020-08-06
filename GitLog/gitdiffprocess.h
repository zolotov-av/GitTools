#ifndef GITDIFFPROCESS_H
#define GITDIFFPROCESS_H

#include <QObject>
#include <GitTools/base.h>

class GitDiffProcess : public QObject
{
    Q_OBJECT
public:
    explicit GitDiffProcess(QObject *parent = nullptr);
    ~GitDiffProcess();

    bool open(const git::blob &a, const git::blob &b);

signals:

public slots:

private:
    class QTimer *timer;
    class QTemporaryFile *aFile;
    class QTemporaryFile *bFile;

};

#endif // GITDIFFPROCESS_H
