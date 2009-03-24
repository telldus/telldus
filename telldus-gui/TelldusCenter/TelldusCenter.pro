# -------------------------------------------------
# Project created by QtCreator 2008-12-11T11:01:36
# -------------------------------------------------
TEMPLATE = app
CONFIG += release
QT += script
SOURCES += main.cpp \
    mainwindow.cpp \
    tellduscenterapplication.cpp \
    autoupdater.cpp \
    message.cpp
HEADERS += mainwindow.h \
    tellduscenterapplication.h \
    autoupdater.h \
    CocoaInitializer.h \
    message.h \
    tellduscenterplugin.h
RESOURCES += resource.qrc
TARGET = TelldusCenter
macx { 
    # HEADERS += sparkleautoupdater.h
    LIBS += -framework \
        TelldusCore
    
    # -framework \
    # Sparkle
    # OBJECTIVE_SOURCES += SparkleAutoUpdater.mm \
    # CocoaInitializer.mm
    QMAKE_INFO_PLIST = Info.plist
    ICON = TelldusCenter.icns
}
!macx:LIBS += -ltelldus-gui
win32:LIBS += -L \
    .
VERSION = 2.0.0_alpha1
