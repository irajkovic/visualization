#include "wysiwyg/visuwidgetfactory.h"

#include "visusignal.h"
#include "mainwindow.h"
#include "instled.h"
#include "insttimeplot.h"
#include "instlinear.h"
#include "instdigital.h"
#include "instanalog.h"
#include "instxyplot.h"
#include "visuconfigloader.h"

VisuWidgetFactory::VisuWidgetFactory()
{

}

VisuWidget* VisuWidgetFactory::createWidget(QWidget* parent, VisuSignal* signal, QString type)
{
    VisuWidget* widget = nullptr;

    if (type == InstAnalog::TAG_NAME)
    {
        widget = CREATE_INSTRUMENT_OBJECT(parent, InstAnalog, signal);
    }
    else if (type == InstDigital::TAG_NAME)
    {
        widget = CREATE_INSTRUMENT_OBJECT(parent, InstDigital, signal);
    }
    else if (type == InstLinear::TAG_NAME)
    {
        widget = CREATE_INSTRUMENT_OBJECT(parent, InstLinear, signal);
    }
    else if (type == InstTimePlot::TAG_NAME)
    {
        widget = CREATE_INSTRUMENT_OBJECT(parent, InstTimePlot, signal);
    }
    else if (type == InstLED::TAG_NAME)
    {
        widget = CREATE_INSTRUMENT_OBJECT(parent, InstLED, signal);
    }
    else if (type == InstXYPlot::TAG_NAME)
    {
        widget = CREATE_INSTRUMENT_OBJECT(parent, InstXYPlot, signal);
    }
    else
    {
        qDebug("Instrument of type %s not recognized.", type.toStdString().c_str());
    }

    Q_ASSERT(widget != nullptr);

    signal->connectInstrument(static_cast<VisuInstrument*>(widget));
    signal->initialUpdate();

    return widget;
}
