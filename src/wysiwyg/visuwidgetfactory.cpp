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
                                            QString type,
                                            const QVector<QPointer<VisuSignal>>& signalsList)
{
    QMap<QString, QString> properties = VisuConfigLoader::getMapFromFile(type, VisuWidget::TAG_NAME);
    QMap<QString, VisuPropertyMeta>* metaProperties = VisuConfigLoader::getMetaMapFromFile(type, VisuWidget::TAG_NAME);
    return VisuWidgetFactory::createWidget(parent, type, properties, metaProperties, signalsList);
}

// TODO :: Check if type can be replaced by properties["type"]
VisuWidget* VisuWidgetFactory::createWidget(QWidget* parent,
                                            QString type,
                                            QMap<QString, QString> properties,
                                            QMap<QString, VisuPropertyMeta>* metaProperties,
                                            const QVector<QPointer<VisuSignal>>& signalsList)
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
        //signalsList.append();
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

    if (widget != nullptr)
    {
        widget->setPropertiesMeta(metaProperties);
        VisuInstrument* visuWidget = qobject_cast<VisuInstrument*>(widget);
        if (visuWidget != nullptr)
        {
            visuWidget->connectSignals(signalsList);
        }
    }

    return widget;
}

