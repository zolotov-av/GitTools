TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS+=QGit
SUBDIRS+=GitLog

GitLog.depends = QGit
