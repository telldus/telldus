# -------------------------------------------------
# Project created by QtCreator 2009-03-10T12:16:44
# -------------------------------------------------
QT += core \
    gui
TARGET = Devices
TEMPLATE = lib
CONFIG += plugin
SOURCES += devicesplugin.cpp
HEADERS += devicesplugin.h
#RESOURCES += Devices.qrc
macx {
    LIBS += -framework \
        TelldusGui
    DESTDIR = ../../TelldusCenter/TelldusCenter.app/Contents/Plugins
}
!macx {
    LIBS += -ltelldus-gui
    DESTDIR = ../../TelldusCenter/Plugins
}
