#ifndef VISUWIDGETFACTORY_H
#define VISUWIDGETFACTORY_H

#include "visuwidget.h"
#include "visusignal.h"

#define CREATE_INSTRUMENT_OBJECT(PARENT, CLASS, OBJECT, SIG)\
    OBJECT = new CLASS(PARENT, VisuConfigLoader::getTagFromFile("system/"#CLASS".xml", "instrument")); \
    OBJECT->setName(#OBJECT); \
    SIG->connectInstrument(static_cast<CLASS*>(OBJECT));

class VisuWidgetFactory
{
public:
    VisuWidgetFactory();
    static VisuWidget* createWidget(QWidget* parent, VisuSignal* signal, QString type);
};

#endif // VISUWIDGETFACTORY_H
