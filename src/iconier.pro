#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T10:39:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iconier
TEMPLATE = app
DEFINES += _MAPPDEBUG CONF_INI
CONFIG += c++11

SOURCES += main.cpp\
        icondialog.cpp \
    miconwidget.cpp \
    fontaws.cpp \
    mmenu.cpp \
    micon.cpp \
    mdefinedwidget.cpp

HEADERS  += icondialog.h \
    miconwidget.h \
    fontaws.h \
    mmenu.h \
    mytypedef.h \
    bmptypes.h \
    icontypes.h \
    micon.h \
    mdefinedwidget.h

FORMS    += icondialog.ui

RESOURCES += \
    skins.qrc \
    font.qrc

RC_FILE += res/icon.rc

OTHER_FILES += \
    res/icon.rc
