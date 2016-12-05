#-------------------------------------------------
#
# Project created by QtCreator 2016-08-04T16:39:07
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = K4000_CPU_TOOLS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    port.cpp \
    form2.cpp \
    form3.cpp

HEADERS  += mainwindow.h \
    port.h \
    form2.h \
    form3.h

FORMS    += mainwindow.ui \
    form2.ui \
    form3.ui

//RC_FILE = resources.rc


RC_FILE = file.rc
