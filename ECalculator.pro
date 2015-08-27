#-------------------------------------------------
#
# Project created by QtCreator 2015-08-26T21:11:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ECalculator
TEMPLATE = app


SOURCES += \
    Sources/main.cpp \
    Sources/mainwindow.cpp \
    Sources/plusmath.cpp \
    Sources/resistor.cpp

HEADERS  += \
    Headers/mainwindow.h \
    Headers/plusmath.h \
    Headers/resistor.h

FORMS    += \
    Forms/mainwindow.ui

RESOURCES += \
    Forms/resource.qrc
