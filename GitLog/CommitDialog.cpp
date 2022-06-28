#include "CommitDialog.h"
#include <QQmlContext>

CommitDialog::CommitDialog(QWindow *parent): QQuickView{parent}
{
    setResizeMode(QQuickView::SizeRootObjectToView);
    resize(QSize{400, 600});

    const auto ctx = rootContext();
    ctx->setContextProperty("form", this);
    ctx->setContextProperty("filesModel", &m_proxy_model);
    setSource(QUrl{"qrc:/qml/CommitDialog.qml"});
}

void CommitDialog::setAuthorName(const QString &name)
{
    if ( m_author_name == name )
        return;

    m_author_name = name;
    emit authorNameChanged();
}

void CommitDialog::setAuthorEmail(const QString &email)
{
    if ( m_author_email == email )
        return;

    m_author_email = email;
    emit authorEmailChanged();
}

void CommitDialog::setCommitMessage(const QString &message)
{
    if ( m_commit_message == message )
        return;

    m_commit_message = message;
    emit commitMessageChanged();
}

void CommitDialog::clearMessage()
{
    if ( m_commit_message.isEmpty() )
        return;

    m_commit_message.clear();
    emit commitMessageChanged();
}

void CommitDialog::setFiles(git::repository *repo)
{
    if ( filesModel == nullptr )
    {
        filesModel = new GitCommitFiles(this);
        m_proxy_model.setSourceModel(filesModel);
    }

    filesModel->open_worktree(repo);
}

void CommitDialog::exec()
{
    setModality(Qt::ApplicationModal);
    show();
}

void CommitDialog::accept()
{
    hide();
    emit accepted();
}

void CommitDialog::reject()
{
    hide();
    emit rejected();
}
