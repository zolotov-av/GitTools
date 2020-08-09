TEMPLATE = subdirs

SUBDIRS+=gtcore
SUBDIRS+=GitLog

GitLog.depends = gtcore
