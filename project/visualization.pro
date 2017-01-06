#-------------------------------------------------
#
# Project created by QtCreator 2015-03-13T21:18:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visualization
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

VPATH = ../src
INCLUDEPATH = ../includes
INCLUDEPATH += ../includes/controls
INCLUDEPATH += ../includes/exceptions
INCLUDEPATH += ../includes/instruments


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
    visuwidget.cpp \
    ../src/statics/staticimage.cpp \
    ../src/visuconfigloader.cpp \
    ../src/wysiwyg/draggablewidget.cpp \
    ../src/wysiwyg/stage.cpp

HEADERS  += ../includes/mainwindow.h \
    ../includes/visuinstrument.h \
    ../includes/visusignal.h \
    ../includes/visuserver.h \
    ../includes/visudatagram.h \
    ../includes/visuapplication.h \
    ../includes/visuconfiguration.h \
    ../includes/instruments/instanalog.h \
    ../includes/visuhelper.h \
    ../includes/instruments/instdigital.h \
    ../includes/instruments/instlinear.h \
    ../includes/instruments/insttimeplot.h \
    ../includes/exceptions/configloadexception.h \
    ../includes/instruments/instxyplot.h \
    ../includes/instruments/instled.h \
    ../includes/controls/button.h \
    ../includes/visuwidget.h \
    ../includes/statics/staticimage.h \
    ../includes/visuconfigloader.h \
    ../includes/wysiwyg/draggablewidget.h \
    ../includes/wysiwyg/stage.h

FORMS    += ../src/mainwindow.ui
