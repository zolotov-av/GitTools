#ifndef GT_COMMIT_DIALOG_H
#define GT_COMMIT_DIALOG_H

#include <GitTools/QuickDialog.h>
#include <GitTools/GitCommitFiles.h>
#include <QSortFilterProxyModel>

class CommitDialog: public QuickDialog
{
    Q_OBJECT
    Q_PROPERTY(QString authorName READ authorName WRITE setAuthorName NOTIFY authorNameChanged FINAL)
    Q_PROPERTY(QString authorEmail READ authorEmail WRITE setAuthorEmail NOTIFY authorEmailChanged FINAL)

private:

    QString m_author_name { };
    QString m_author_email { };
    QString m_commit_message { };

    QSortFilterProxyModel m_proxy_model { this };
    GitCommitFiles *filesModel { nullptr };

public:

    explicit CommitDialog(QQmlEngine *parent = nullptr);
    CommitDialog(const CommitDialog &) = delete;
    CommitDialog(CommitDialog &&) = delete;

    CommitDialog& operator = (const CommitDialog &) = delete;
    CommitDialog& operator = (CommitDialog &&) = delete;

    QString authorName() const { return m_author_name; }
    QString authorEmail() const { return m_author_email; }
    QString message() const { return m_commit_message; }

    void setAuthorName(const QString &name);
    void setAuthorEmail(const QString &email);
    void setCommitMessage(const QString &message);

    void clearMessage();

    void setFiles(git::repository *repo);
    void exec();

public slots:

    void accept();
    void reject();

signals:

    void authorNameChanged();
    void authorEmailChanged();
    void commitMessageChanged();
    void accepted();
    void rejected();

};

#endif // GT_COMMIT_DIALOG_H
