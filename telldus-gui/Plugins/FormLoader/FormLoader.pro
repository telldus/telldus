# -------------------------------------------------
# Project created by QtCreator 2009-03-10T12:16:44
# -------------------------------------------------
QT += core \
    gui \
    script
TARGET = FormLoader
TEMPLATE = lib
CONFIG += plugin uitools
SOURCES += formloaderplugin.cpp \
    formloaderobject.cpp
HEADERS += formloaderplugin.h \
    formloaderobject.h
macx { 
    DESTDIR = ../../TelldusCenter/TelldusCenter.app/Contents/Plugins/script
}
!macx { 
    DESTDIR = ../../TelldusCenter/Plugins/script
}
