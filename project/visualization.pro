#-------------------------------------------------
#
# Project created by QtCreator 2015-03-13T21:18:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visualization
TEMPLATE = app

VPATH = ../src
INCLUDEPATH = ../src/

SOURCES += main.cpp\
    mainwindow.cpp \
    visuinstrument.cpp \
    visusignal.cpp \
    visuserver.cpp \
    visuapplication.cpp \
    visuconfiguration.cpp \
    instruments/instanalog.cpp \
    instruments/instdigital.cpp \
    instruments/instlinear.cpp \
    instruments/insttimeplot.cpp \
    exceptions/configloadexception.cpp \
    instruments/instxyplot.cpp \
    instruments/instled.cpp \
    visuhelper.cpp \
    controls/button.cpp \
    visuwidget.cpp

HEADERS  += mainwindow.h \
    visuinstrument.h \
    visusignal.h \
    visuserver.h \
    visudatagram.h \
    visuapplication.h \
    visuconfiguration.h \
    instruments/instanalog.h \
    visuhelper.h \
    instruments/instdigital.h \
    instruments/instlinear.h \
    instruments/insttimeplot.h \
    exceptions/configloadexception.h \
    instruments/instxyplot.h \
    instruments/instled.h \
    controls/button.h \
    visuwidget.h

FORMS    += ../src/mainwindow.ui
