# -------------------------------------------------
# Project created by QtCreator 2008-12-11T15:48:29
# -------------------------------------------------
TARGET = TelldusGui
TEMPLATE = lib
DEFINES += TELLDUSGUI_LIBRARY
SOURCES += telldusgui.cpp \
    devicewidget.cpp \
    devicemodel.cpp \
    devicesetting/devicesetting.cpp \
    device.cpp
HEADERS += telldusgui.h \
    TelldusGui_global.h \
    devicewidget.h \
    devicemodel.h \
    devicesetting/devicesetting.h \
    device.h
FORMS += 
macx { 
    LIBS += -framework \
        telldus-core
    ICON = images/telldussetup.icns
}
RESOURCES += resource.qrc
