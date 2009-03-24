# -------------------------------------------------
# Project created by QtCreator 2008-12-11T15:48:29
# -------------------------------------------------
TEMPLATE = lib
CONFIG += release
DEFINES += TELLDUSGUI_LIBRARY
SOURCES += telldusgui.cpp \
    devicewidget.cpp \
    devicemodel.cpp \
    device.cpp \
    editdevicedialog.cpp \
    editgroupdialog.cpp \
    vendordevicemodel.cpp \
    vendordevicetreeitem.cpp \
    devicesettingikea.cpp \
    devicesettingnexa.cpp \
    devicesettingnexabell.cpp \
    devicesettingsartano.cpp \
    devicesetting.cpp \
    methodwidget.cpp \
    deviceview.cpp
HEADERS += telldusgui.h \
    devicewidget.h \
    devicemodel.h \
    device.h \
    editdevicedialog.h \
    editgroupdialog.h \
    vendordevicemodel.h \
    vendordevicetreeitem.h \
    devicesettingikea.h \
    devicesettingnexa.h \
    devicesettingnexabell.h \
    devicesettingsartano.h \
    devicesetting.h \
    methodwidget.h \
    deviceview.h
FORMS += 
macx { 
    LIBS += -framework \
        TelldusCore
    ICON = images/telldussetup.icns
    TARGET = TelldusGui
    CONFIG += lib_bundle
    DESTDIR = ../TelldusCenter/TelldusCenter.app/Contents/Frameworks
}
!macx { 
    LIBS += -l \
        telldus-core
    TARGET = telldus-gui
}
win32:LIBS += -L \
    .
RESOURCES += telldusgui.qrc
