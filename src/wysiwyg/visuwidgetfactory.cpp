#include "wysiwyg/visuwidgetfactory.h"

#include "visusignal.h"
#include "mainwindow.h"
#include "instled.h"
#include "insttimeplot.h"
#include "instlinear.h"
#include "instdigital.h"
#include "instanalog.h"
#include "instxyplot.h"
#include "ctrlbutton.h"
#include "visuconfigloader.h"

VisuWidgetFactory::VisuWidgetFactory()
{

}


VisuWidget* VisuWidgetFactory::createWidget(QWidget* parent,
                                            QString type,
                                            VisuSignal* signal)
{
    QString path = QString("system/%1.xml").arg(type);
    QMap<QString, QString> properties = VisuConfigLoader::getMapFromFile(path, VisuConfiguration::TAG_WIDGET);
    return VisuWidgetFactory::createWidget(parent, type, properties, signal);
}

// TODO :: Check if type can be replaced by properties["type"]
VisuWidget* VisuWidgetFactory::createWidget(QWidget* parent,
                                            QString type,
                                            QMap<QString, QString> properties,
                                            VisuSignal* signal)
{
    VisuWidget* widget = nullptr;

    if (type == InstAnalog::TAG_NAME)
    {
        widget = new InstAnalog(parent, properties);
    }
    else if (type == InstDigital::TAG_NAME)
    {
        widget = new InstDigital(parent, properties);
    }
    else if (type == InstLinear::TAG_NAME)
    {
        widget = new InstLinear(parent, properties);
    }
    else if (type == InstTimePlot::TAG_NAME)
    {
        widget = new InstTimePlot(parent, properties);
    }
    else if (type == InstLED::TAG_NAME)
    {
        widget = new InstLED(parent, properties);
    }
    else if (type == InstXYPlot::TAG_NAME)
    {
        widget = new InstXYPlot(parent, properties);
    }
    else if (type == CtrlButton::TAG_NAME)
    {
        widget = new CtrlButton(parent, properties);
    }
    else if (type == StaticImage::TAG_NAME)
    {
        widget = new StaticImage(parent, properties);
    }
    else
    {
        ; // Do nothing
    }

    if (signal != nullptr && widget != nullptr)
    {
        signal->connectInstrument(static_cast<VisuInstrument*>(widget));
        signal->initializeInstruments();
    }

    return widget;
}

