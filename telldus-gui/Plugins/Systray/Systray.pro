# -------------------------------------------------
# Project created by QtCreator 2009-03-10T12:16:44
# -------------------------------------------------
QT += core \
    gui
TARGET = SystrayIcon
TEMPLATE = lib
CONFIG += plugin
SOURCES += systrayicon.cpp \
    systrayplugin.cpp
HEADERS += systrayplugin.h \
    systrayicon.h
macx {
    LIBS += -framework TelldusCore
    DESTDIR = ../../TelldusCenter/TelldusCenter.app/Contents/Plugins
}
!macx {
    LIBS += -ltelldus-core
    DESTDIR = ../../TelldusCenter/Plugins
}

