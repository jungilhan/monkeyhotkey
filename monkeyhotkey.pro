#-------------------------------------------------
#
# Project created by QtCreator 2012-02-19T23:20:08
#
#-------------------------------------------------

QT       += core gui webkit console network

TARGET = monkeyhotkey
TEMPLATE = app


SOURCES += main.cpp\
        mainWindow.cpp \
    header.cpp \
    roundButton.cpp \
    footer.cpp \
    aboutDialog.cpp \
    webView.cpp \
    nativeAdapter.cpp \
    settings.cpp \
    hotkeyInfo.cpp \
    hotkeyManager.cpp \
    updater.cpp \
    http.cpp \
    progressDialog.cpp

HEADERS  += mainWindow.h \
    config.h \
    header.h \
    roundButton.h \
    footer.h \
    aboutDialog.h \
    webView.h \
    nativeAdapter.h \
    settings.h \
    hotkeyInfo.h \
    hotkeyManager.h \
    updater.h \
    http.h \
    progressDialog.h

RESOURCES += \
    monkeyhotkey.qrc

RC_FILE += monkeyhotkey.rc

INCLUDEPATH += qtsingleapplication/src
LIBS += -Lqtsingleapplication/lib
LIBS += -lQtSolutions_SingleApplication-head

OTHER_FILES += \
    monkeyhotkey.rc
