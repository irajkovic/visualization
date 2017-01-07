#ifndef VISUWIDGETFACTORY_H
#define VISUWIDGETFACTORY_H

#include "visuwidget.h"
#include "visusignal.h"

#define CREATE_INSTRUMENT_OBJECT(PARENT, CLASS, OBJECT, SIG)\
    CLASS* OBJECT = new CLASS(PARENT, VisuConfigLoader::getTagFromFile("system/"#OBJECT".xml", "instrument")); \
    OBJECT->setName(#OBJECT); \
    SIG->connectInstrument(OBJECT); \

#define CREATE_DRAGGABLE_OBJECT(PARENT, CLASS, OBJECT, SIG)\
    CREATE_INSTRUMENT_OBJECT(PARENT, CLASS, OBJECT, SIG)\
    DraggableWidget* draggable##CLASS = new DraggableWidget(PARENT); \
    OBJECT->setParent(draggable##CLASS); \
    draggable##CLASS->setWidget(OBJECT); \

#define ADD_INSTRUMENT_TO_LAYOUT(PARENT, CLASS, OBJECT, SIG, LAYOUT)\
    CREATE_DRAGGABLE_OBJECT(PARENT, CLASS, OBJECT, SIG) \
    LAYOUT->addWidget(draggable##CLASS);

#define ADD_INSTRUMENT_TO_WIDGET(PARENT, CLASS, OBJECT, SIG, POSITION)\
    CREATE_DRAGGABLE_OBJECT(PARENT, CLASS, OBJECT, SIG) \
    OBJECT->setPosition(POSITION); \
    OBJECT->show(); \
    draggable##CLASS->show();

class VisuWidgetFactory
{
public:
    VisuWidgetFactory();
    static VisuWidget* createWidget(QWidget* parent, VisuSignal* signal, QString type);
};

#endif // VISUWIDGETFACTORY_H
