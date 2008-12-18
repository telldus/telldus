# -------------------------------------------------
# Project created by QtCreator 2008-12-11T15:48:29
# -------------------------------------------------
TEMPLATE = lib
DEFINES += TELLDUSGUI_LIBRARY
SOURCES += telldusgui.cpp \
    devicewidget.cpp \
    devicemodel.cpp \
    \ \
    \ \
    \ \
    \ \ # devicesetting/devicesetting.cpp \
    device.cpp \
    editdevicedialog.cpp \
    vendordevicemodel.cpp \
    vendordevicetreeitem.cpp
HEADERS += telldusgui.h \
    devicewidget.h \
    devicemodel.h \
    \ \
    \ \
    \ \
    \ \ # devicesetting/devicesetting.h \
    device.h \
    editdevicedialog.h \
    vendordevicemodel.h \
    vendordevicetreeitem.h
FORMS += 
macx { 
    LIBS += -framework \
        telldus-core
    ICON = images/telldussetup.icns
    TARGET = TelldusGui
}
!macx { 
    LIBS += -l \
        telldus-core
    TARGET = telldus-gui
}
RESOURCES += telldusgui.qrc
