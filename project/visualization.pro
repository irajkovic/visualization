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
    instrument.cpp \
    signal.cpp \
    server.cpp \
    visuapplication.cpp \
    configuration.cpp \
    instruments/instanalog.cpp \
    instruments/instdigital.cpp \
    ../src/instruments/instlinear.cpp \
    ../src/instruments/insttimeplot.cpp \
    ../src/exceptions/configloadexception.cpp \
    ../src/instruments/instxyplot.cpp \
    ../src/instruments/instled.cpp \
    ../src/visu_helper.cpp \
    ../src/controls/button.cpp

HEADERS  += mainwindow.h \
    ../src/instrument.h \
    ../src/signal.h \
    ../src/server.h \
    ../src/datagram.h \
    ../src/visuapplication.h \
    ../src/configuration.h \
    ../src/instruments/instanalog.h \
    ../src/visu_helper.h \
    ../src/instruments/instdigital.h \
    ../src/instruments/instlinear.h \
    ../src/instruments/insttimeplot.h \
    ../src/exceptions/configloadexception.h \
    ../src/instruments/instxyplot.h \
    ../src/instruments/instled.h \
    ../src/controls/button.h

FORMS    += ../src/mainwindow.ui
