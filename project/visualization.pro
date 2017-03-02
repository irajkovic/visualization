#-------------------------------------------------
#
# Project created by QtCreator 2015-03-13T21:18:19
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visualization
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall

VPATH = ../src
INCLUDEPATH = ../includes
INCLUDEPATH += ../includes/controls
INCLUDEPATH += ../includes/exceptions
INCLUDEPATH += ../includes/instruments


SOURCES += main.cpp\
    mainwindow.cpp \
    visuwidget.cpp \
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
    visupropertyloader.cpp \
    controls/ctrlbutton.cpp \
    controls/ctrlslider.cpp \
    statics/staticimage.cpp \
    visuconfigloader.cpp \
    wysiwyg/stage.cpp \
    wysiwyg/visuwidgetfactory.cpp \
    visumisc.cpp \
    wysiwyg/editsignal.cpp \
    wysiwyg/editconfiguration.cpp \
    visucontrol.cpp \
    visupropertymeta.cpp \
    wysiwyg/visupropertieshelper.cpp \
    visuappinfo.cpp \
    visudatagram.cpp

HEADERS  += ../includes/mainwindow.h \
    ../includes/visuinstrument.h \
    ../includes/visusignal.h \
    ../includes/visuserver.h \
    ../includes/visudatagram.h \
    ../includes/visuapplication.h \
    ../includes/visuconfiguration.h \
    ../includes/instruments/instanalog.h \
    ../includes/instruments/instdigital.h \
    ../includes/instruments/instlinear.h \
    ../includes/instruments/insttimeplot.h \
    ../includes/exceptions/configloadexception.h \
    ../includes/instruments/instxyplot.h \
    ../includes/instruments/instled.h \
    ../includes/visuwidget.h \
    ../includes/statics/staticimage.h \
    ../includes/visuconfigloader.h \
    ../includes/wysiwyg/stage.h \
    ../includes/wysiwyg/visuwidgetfactory.h \
    ../includes/visumisc.h \
    ../includes/wysiwyg/editsignal.h \
    ../includes/wysiwyg/editconfiguration.h \
    ../includes/visucontrol.h \
    ../includes/controls/ctrlbutton.h \
    ../includes/controls/ctrlslider.h \
    ../includes/visupropertymeta.h \
    ../includes/wysiwyg/visupropertieshelper.h \
    ../includes/visupropertyloader.h \
    ../includes/visuappinfo.h

FORMS    += ../src/mainwindow.ui
