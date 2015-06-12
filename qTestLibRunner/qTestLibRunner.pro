#-------------------------------------------------
#
# Project created by QtCreator 2015-06-09T23:18:54
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qTestLibRunner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingswindow.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    settingswindow.h \
    settings.h

FORMS    += mainwindow.ui \
    settingswindow.ui

QMAKE_CXXFLAGS += -std=c++14

RESOURCES = resoureces.qrc
