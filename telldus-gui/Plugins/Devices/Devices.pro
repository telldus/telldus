# -------------------------------------------------
# Project created by QtCreator 2009-03-10T12:16:44
# -------------------------------------------------
QT += core \
    gui
TARGET = Devices
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../TelldusCenter/plugins
SOURCES += devicesplugin.cpp
HEADERS += devicesplugin.h
#RESOURCES += Devices.qrc
macx {
    LIBS += -framework \
        TelldusGui
}
!macx:LIBS += -ltelldus-gui
