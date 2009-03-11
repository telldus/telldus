# -------------------------------------------------
# Project created by QtCreator 2009-03-10T12:16:44
# -------------------------------------------------
QT += core \
    gui
TARGET = TouchInterface
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../TelldusCenter/plugins
SOURCES += touchplugin.cpp \
    panel.cpp \
    button.cpp
HEADERS += touchplugin.h \
    panel.h \
    button.h
RESOURCES += TouchInterface.qrc
contains(QT_CONFIG, opengl):QT += opengl
