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
    ../src/instruments/instlinear.cpp \
    ../src/instruments/insttimeplot.cpp \
    ../src/exceptions/configloadexception.cpp \
    ../src/instruments/instxyplot.cpp \
    ../src/instruments/instled.cpp \
    ../src/visuhelper.cpp \
    ../src/controls/button.cpp \
    ../src/visuwidget.cpp

HEADERS  += mainwindow.h \
    ../src/visuinstrument.h \
    ../src/visusignal.h \
    ../src/visuserver.h \
    ../src/visudatagram.h \
    ../src/visuapplication.h \
    ../src/visuconfiguration.h \
    ../src/instruments/instanalog.h \
    ../src/visuhelper.h \
    ../src/instruments/instdigital.h \
    ../src/instruments/instlinear.h \
    ../src/instruments/insttimeplot.h \
    ../src/exceptions/configloadexception.h \
    ../src/instruments/instxyplot.h \
    ../src/instruments/instled.h \
    ../src/controls/button.h \
    ../src/visuwidget.h

FORMS    += ../src/mainwindow.ui
