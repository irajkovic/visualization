#include "wysiwyg/visuwidgetfactory.h"

#include "visusignal.h"
#include "mainwindow.h"
#include "instled.h"
#include "insttimeplot.h"
#include "instlinear.h"
#include "instdigital.h"
#include "instanalog.h"
#include "visuconfigloader.h"

VisuWidgetFactory::VisuWidgetFactory()
{

}

VisuWidget* VisuWidgetFactory::createWidget(QWidget* parent, VisuSignal* signal, QString type)
{
    VisuWidget* widget = nullptr;

    if (type == InstAnalog::TAG_NAME)
    {
        CREATE_INSTRUMENT_OBJECT(parent, InstAnalog, widget, signal);
    }
    else if (type == InstDigital::TAG_NAME)
    {
        CREATE_INSTRUMENT_OBJECT(parent, InstDigital, widget, signal);
    }
    else if (type == InstLinear::TAG_NAME)
    {
        CREATE_INSTRUMENT_OBJECT(parent, InstLinear, widget, signal);
    }
    else if (type == InstTimePlot::TAG_NAME)
    {
        CREATE_INSTRUMENT_OBJECT(parent, InstTimePlot, widget, signal);
    }
    else if (type == InstLED::TAG_NAME)
    {
        CREATE_INSTRUMENT_OBJECT(parent, InstLED, widget, signal);
    }
    else
    {
        qDebug("Instrument of type %s not recognized.", type.toStdString().c_str())    ;
    }

    Q_ASSERT(widget != nullptr);

    //widget->show();
    signal->initialUpdate();

    return widget;
}
