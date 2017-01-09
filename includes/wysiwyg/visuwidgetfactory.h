#ifndef VISUWIDGETFACTORY_H
#define VISUWIDGETFACTORY_H

#include "visuwidget.h"
#include "visusignal.h"

#define CREATE_INSTRUMENT_OBJECT(PARENT, CLASS, OBJECT, SIG)\
    CLASS* OBJECT = new CLASS(PARENT, VisuConfigLoader::getTagFromFile("system/"#OBJECT".xml", "instrument")); \
    OBJECT->setName(#OBJECT); \
    SIG->connectInstrument(OBJECT); \

#define ADD_INSTRUMENT_TO_LAYOUT(PARENT, CLASS, OBJECT, SIG, LAYOUT)\
    CREATE_INSTRUMENT_OBJECT(PARENT, CLASS, OBJECT, SIG) \
    LAYOUT->addWidget(OBJECT);

#define ADD_INSTRUMENT_TO_WIDGET(PARENT, CLASS, OBJECT, SIG, POSITION)\
    CREATE_INSTRUMENT_OBJECT(PARENT, CLASS, OBJECT, SIG) \
    OBJECT->setPosition(POSITION); \
    OBJECT->show(); \

class VisuWidgetFactory
{
public:
    VisuWidgetFactory();
    static VisuWidget* createWidget(QWidget* parent, VisuSignal* signal, QString type);
};

#endif // VISUWIDGETFACTORY_H
