# -------------------------------------------------
# Project created by QtCreator 2008-12-11T11:01:36
# -------------------------------------------------
TARGET = TelldusCenter
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    tellduscenterapplication.cpp
HEADERS += mainwindow.h \
    tellduscenterapplication.h
FORMS += 
RESOURCES += resource.qrc
macx:LIBS += -lTelldusGui \
    -L../TelldusGui
!macx:LIBS += -ltelldus-gui
