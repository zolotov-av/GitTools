#ifndef GITDIFFPROCESS_H
#define GITDIFFPROCESS_H

#include <qgit2e.h>

class GitDiffProcess : public QObject
{
    Q_OBJECT
public:
    explicit GitDiffProcess(QObject *parent = nullptr);
    ~GitDiffProcess();

    bool open(LibQGit2::Blob a, LibQGit2::Blob b);

signals:

public slots:

private:
    class QTimer *timer;
    class QTemporaryFile *aFile;
    class QTemporaryFile *bFile;

};

#endif // GITDIFFPROCESS_H
