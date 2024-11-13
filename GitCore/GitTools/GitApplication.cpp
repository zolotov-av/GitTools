#include "GitApplication.h"
#include <QtGlobal>
#include <git2.h>

GitApplication::GitApplication()
{
    Q_INIT_RESOURCE(GitCoreResources);

    git_libgit2_init();
}

GitApplication::~GitApplication()
{
    git_libgit2_shutdown();

    Q_CLEANUP_RESOURCE(GitCoreResources);
}
