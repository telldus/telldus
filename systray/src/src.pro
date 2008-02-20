TEMPLATE = app

SOURCES += main.cpp \
icon.cpp
#menu.cpp
LIBS += -ltellusbd101 -L../bin
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