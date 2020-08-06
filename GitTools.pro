TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS+=libgt
SUBDIRS+=GitLog

GitLog.depends = libgt
