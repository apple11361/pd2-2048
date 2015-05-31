#-------------------------------------------------
#
# Project created by QtCreator 2015-05-08T13:51:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_2048
TEMPLATE = app


SOURCES += main.cpp\
        game2048.cpp \
    tile.cpp

HEADERS  += game2048.h \
    tile.h

FORMS    += game2048.ui

RESOURCES += \
    game2048.qrc
