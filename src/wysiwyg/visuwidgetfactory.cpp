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

VisuWidget* VisuWidgetFactory::createWidget(QWidget* parent,
                                            QString type)
{
    QMap<QString, QString> properties = VisuConfigLoader::getMapFromFile(type, VisuWidget::TAG_NAME);
    return VisuWidgetFactory::createWidget(parent, type, properties);
}

// TODO :: Check if type can be replaced by properties["type"]
VisuWidget* VisuWidgetFactory::createWidget(QWidget* parent,
                                            QString type,
                                            QMap<QString, QString> properties)
{
    QMap<QString, VisuPropertyMeta> metaProperties = VisuConfigLoader::getMetaMapFromFile(type, VisuWidget::TAG_NAME);

    VisuWidget* widget = nullptr;

    if (type == InstAnalog::TAG_NAME)
    {
        widget = new InstAnalog(parent, properties, metaProperties);
    }
    else if (type == InstDigital::TAG_NAME)
    {
        widget = new InstDigital(parent, properties, metaProperties);
    }
    else if (type == InstLinear::TAG_NAME)
    {
        widget = new InstLinear(parent, properties, metaProperties);
    }
    else if (type == InstTimePlot::TAG_NAME)
    {
        widget = new InstTimePlot(parent, properties, metaProperties);
    }
    else if (type == InstLED::TAG_NAME)
    {
        widget = new InstLED(parent, properties, metaProperties);
    }
    else if (type == InstXYPlot::TAG_NAME)
    {
        widget = new InstXYPlot(parent, properties, metaProperties);
    }
    else if (type == CtrlButton::TAG_NAME)
    {
        widget = new CtrlButton(parent, properties, metaProperties);
    }
    else if (type == StaticImage::TAG_NAME)
    {
        widget = new StaticImage(parent, properties, metaProperties);
    }
    else
    {
        ; // Do nothing
    }

    if (widget != nullptr)
    {
        widget->setPropertiesMeta(metaProperties);
        VisuInstrument* visuWidget = qobject_cast<VisuInstrument*>(widget);
        if (visuWidget != nullptr)
        {
            visuWidget->connectSignals();
        }
    }

    return widget;
}

