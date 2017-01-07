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

    if (type == "analog")
    {
        CREATE_INSTRUMENT_OBJECT(parent, InstAnalog, analog, signal);
        widget = analog;
    }
    else if (type == "digital")
    {
        CREATE_INSTRUMENT_OBJECT(parent, InstDigital, digital, signal);
        widget = digital;
    }
    else if (type == "linear")
    {
        CREATE_INSTRUMENT_OBJECT(parent, InstLinear, linear, signal);
        widget = linear;
    }
    else if (type == "timeplot")
    {
        CREATE_INSTRUMENT_OBJECT(parent, InstTimePlot, timeplot, signal);
        widget = timeplot;
    }
    else if (type == "led")
    {
        CREATE_INSTRUMENT_OBJECT(parent, InstLED, led, signal);
        widget = led;
    }

    Q_ASSERT(widget != nullptr);

    widget->show();
    signal->initialUpdate();

    return widget;
}
