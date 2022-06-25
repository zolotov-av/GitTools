#-------------------------------------------------
#
# Project created by QtCreator 2019-11-10T16:40:43
#
#-------------------------------------------------

TEMPLATE = app
TARGET = GitLog

QT       += core gui widgets dbus

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    DBusInterface.cpp \
    LogWindow.cpp \
    main.cpp

HEADERS += \
    DBusInterface.h \
    LogWindow.h

FORMS += \
    LogWindow.ui

RESOURCES += \
    ../resources.qrc

INCLUDEPATH += $$PWD/../gtcore $$OUT_PWD/../gtcore
DEPENDPATH += $$PWD/../gtcore $$OUT_PWD/../gtcore

LIBS += -lgit2
unix: LIBS += -L$$OUT_PWD/../gtcore/ -lgtcore
unix: PRE_TARGETDEPS += $$OUT_PWD/../gtcore/libgtcore.a

target.path = /usr/local/bin
INSTALLS += target
