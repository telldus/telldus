# -------------------------------------------------
# Project created by QtCreator 2009-03-10T12:16:44
# -------------------------------------------------
QT += core \
    gui \
    script
TARGET = SystrayIcon
TEMPLATE = lib
CONFIG += plugin
SOURCES += systrayicon.cpp \
    systrayplugin.cpp \
    systrayobject.cpp
HEADERS += systrayplugin.h \
    systrayicon.h \
    systrayobject.h
macx { 
    LIBS += -framework \
        TelldusCore
    DESTDIR = ../../TelldusCenter/TelldusCenter.app/Contents/Plugins/script
}
!macx { 
    LIBS += -ltelldus-core
    DESTDIR = ../../TelldusCenter/Plugins/script
}
