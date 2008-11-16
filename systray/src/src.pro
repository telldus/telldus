TEMPLATE = app

SOURCES += main.cpp \
icon.cpp

!macx:LIBS += -ltelldus-core -L../bin
macx{
    LIBS += -framework telldus-core
    ICON = images/systray.icns
}
win32{
    TARGET = ../../bin/SysTray
    CONFIG += release
}
unix{
    TARGET = ../bin/systray
    CONFIG += debug
}
HEADERS += icon.h
#menu.h
RESOURCES += resource.qrc
RC_FILE = systray.rc
TRANSLATIONS = SysTray_sv.ts
