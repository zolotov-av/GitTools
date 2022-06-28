#ifndef GT_COMMIT_DIALOG_H
#define GT_COMMIT_DIALOG_H

#include "ui_CommitDialog.h"

class CommitDialog final: public QDialog
{
    Q_OBJECT

private:

    Ui::CommitDialog ui;

public:

    explicit CommitDialog(QWidget *parent = nullptr);
    CommitDialog(const CommitDialog &) = delete;
    CommitDialog(CommitDialog &&) = delete;

    CommitDialog& operator = (const CommitDialog &) = delete;
    CommitDialog& operator = (CommitDialog &&) = delete;

    QString authorName() const { return ui.leAuthorName->text(); }
    QString authorEmail() const { return ui.leAuthorEmail->text(); }
    QString message() const { return ui.teMessage->toPlainText(); }

    void setAuthorName(const QString &name)
    {
        ui.leAuthorName->setText(name);
    }

    void setAuthorEmail(const QString &email)
    {
        ui.leAuthorEmail->setText(email);
    }

    void clearMessage()
    {
        ui.teMessage->clear();
    }

};

#endif // GT_COMMIT_DIALOG_H
