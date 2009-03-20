# -------------------------------------------------
# Project created by QtCreator 2009-03-10T12:16:44
# -------------------------------------------------
QT += core \
    gui
TARGET = TouchInterface
TEMPLATE = lib
CONFIG += plugin
SOURCES += touchplugin.cpp \
    panel.cpp \
    button.cpp
HEADERS += touchplugin.h \
    panel.h \
    button.h
RESOURCES += TouchInterface.qrc
macx {
    DESTDIR = ../../TelldusCenter/TelldusCenter.app/Contents/Plugins
}
!macx {
    DESTDIR = ../../TelldusCenter/Plugins
}

contains(QT_CONFIG, opengl):QT += opengl
