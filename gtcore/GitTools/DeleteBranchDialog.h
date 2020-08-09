#ifndef GT_DELETEBRANCHDIALOG_H
#define GT_DELETEBRANCHDIALOG_H

#include <QDebug>
#include <QDialog>
#include <GitTools/base.h>
#include <GitTools/ExceptionTooltip.h>
#include <GitTools/GitLogModel.h>
#include "ui_DeleteBranchDialog.h"

namespace git
{

    class DeleteBranchDialog : public QDialog
    {
        Q_OBJECT

    protected:

        Ui::DeleteBranchDialog ui;

        GitLogModel *model = nullptr;
        git::repository *repo = nullptr;

    public:

        explicit DeleteBranchDialog(QWidget *parent = nullptr): QDialog(parent)
        {
            ui.setupUi(this);
        }

        void setModel(GitLogModel *m)
        {
            model = m;
        }

        void setCommitId(git::repository *r, const git::object_id &commit_id)
        {
            repo = r;
            if ( repo == nullptr ) throw git::exception("setRepository(null)");

            git_reference_iterator *iter;
            int status = git_reference_iterator_new(&iter, repo->data());
            if ( status == 0 )
            {
                git_reference *ref;

                while ( true )
                {
                    status = git_reference_next(&ref, iter);
                    if ( status != 0 )
                    {
                        break;
                    }
                    const git::reference r = ref;
                    if ( r.isBranch() && r.resolve().target() == commit_id )
                    {
                        ui.cbBranch->addItem(r.shortName());
                    }
                }

                git_reference_iterator_free(iter);
            }

            if ( ui.cbBranch->count() > 0 ) return;

            ui.cbBranch->addItem("-- select branch --", "select");
            status = git_reference_iterator_new(&iter, repo->data());
            if ( status == 0 )
            {
                git_reference *ref;

                while ( true )
                {
                    status = git_reference_next(&ref, iter);
                    if ( status != 0 )
                    {
                        break;
                    }
                    const git::reference r = ref;
                    if ( r.isBranch() )
                    {
                        ui.cbBranch->addItem(r.shortName());
                    }
                }

                git_reference_iterator_free(iter);
            }

        }



    protected slots:

        void on_pbOk_clicked()
        {
            qDebug() << "DeleteBranchDialog::on_pbOk_clicked()";

            try
            {

                if ( qvariant_cast<QString>(ui.cbBranch->currentData()) == "select" )
                {
                    qDebug() << "nothing selected";
                    reject();
                    return;
                }

                repo->delete_branch(ui.cbBranch->currentText());

                if ( model ) model->update();

                accept();

                return;
            }
            catch (const std::exception &e)
            {
                ExceptionTooltip::show(this, e);
                return;
            }
        }

    };

}


#endif // GT_DELETEBRANCHDIALOG_H
