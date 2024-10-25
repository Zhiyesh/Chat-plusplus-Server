#-------------------------------------------------
#
# Project created by QtCreator 2024-02-19T18:19:52
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11

TARGET = Chat-plusplus-Server
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    src/widget.cpp \
    src/sleep.cpp \
    src/socketthread.cpp

HEADERS  += inc/widget.h \
    inc/sleep.h \
    inc/listenip.h \
    inc/socketthread.h

FORMS    += ui/widget.ui

RESOURCES += \
    font.qrc
