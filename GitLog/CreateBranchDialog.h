#ifndef GT_CREATEBRANCHDIALOG_H
#define GT_CREATEBRANCHDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QCloseEvent>
#include <QHideEvent>
#include <QToolTip>
#include <QTimer>
#include "ui_CreateBranchDialog.h"
#include <GitTools/base.h>
#include <QMessageBox>
#include "gitlogmodel.h"
#include "ExceptionTooltip.h"

class CreateBranchDialog : public QDialog, Ui::CreateBranchDialog
{
    Q_OBJECT

protected:

    git::object_id commit_id;
    git::repository *repo;
    GitLogModel *model;

    void hideEvent(QHideEvent *) override
    {
        qDebug() << "CreateBranchDialog::HideEvent()";
        deleteLater();
    }

    git::commit lookup_target()
    {
        if ( rbHead->isChecked() )
        {
            qDebug() << "on HEAD";

            return repo->get_commit( repo->get_head().target() );
        }

        if ( rbBranch->isChecked() )
        {
            const QString on_branch = cbBranch->currentText();
            qDebug() << QString("on other branch: '%1'").arg(on_branch);
            return repo->get_commit( repo->get_branch(on_branch).resolve().target() );
        }

        if ( rbTag->isChecked() )
        {
            const QString on_tag = cbTag->currentText();
            qDebug() << QString("on tag: '%1'").arg(on_tag);
            return repo->get_commit( repo->get_reference(on_tag).resolve().target() );
        }

        if ( rbCommit->isChecked() )
        {
            const QString on_commit = cbCommit->currentText();
            qDebug() << QString("on tag: '%1'").arg(on_commit);
            return repo->get_commit(on_commit);
        }

        throw git::exception("nothing selected");

    }

protected slots:

    void on_accepted()
    {
        qDebug() << "CreateBranchDialog::on_accepted";


    }

    void on_pbOk_clicked()
    {
        qDebug() << "on_pbOk_clicked()";

        try
        {
            const QString name = leBranch->text();
            qDebug() << QString("create branch: %1").arg(name);
            if ( name.isEmpty() )
            {
                ExceptionTooltip::showBelow(leBranch, "Error", "branch name is empty");
                return;
            }

            const bool force = cbForce->checkState();
            qDebug() << QString("force: %1").arg(force ? "yes" : "no");

            const git::commit target = lookup_target();
            repo->create_branch(name, target, force);

            if ( model ) model->update();

            if ( cbSwitchToNew->checkState() )
            {
                ExceptionTooltip::showBelow(leBranch, "TODO", "switch not implemented yet");
            }

            accept();

        }
        catch (const std::exception &e)
        {
            ExceptionTooltip::show(this, e);
        }
    }

public:

    explicit CreateBranchDialog(QWidget *parent = nullptr): QDialog(parent)
    {
        qDebug() << "create CreateBranchDialog";
        setupUi(this);
        rbCommit->setChecked(true);
        connect(this, SIGNAL(accepted()), this, SLOT(on_accepted()));
    }

    ~CreateBranchDialog()
    {
        qDebug() << "destroy CreateBranchDialog";
    }

    void setModel(GitLogModel *m)
    {
        model = m;
    }

    void setRepositiory(git::repository *r)
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
                if ( r.isBranch() )
                {
                    cbBranch->addItem(r.shortName());
                }
                else if ( r.isTag() )
                {
                    cbTag->addItem(r.shortName());
                }
            }

            git_reference_iterator_free(iter);
        }

    }

    void setCommitId(const git::object_id &id)
    {
        commit_id = id;
        rbCommit->setChecked(true);
        cbCommit->addItem(id.toString());
        cbCommit->setCurrentIndex(0);
    }

};

#endif // GT_CREATEBRANCHDIALOG_H
