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


VisuWidget* VisuWidgetFactory::createWidget(QWidget* parent,
                                            QString type,
                                            VisuSignal* signal)
{
    QString path = QString("system/%1.xml").arg(type);
    QMap<QString, QString> properties = VisuConfigLoader::getMapFromFile(path, "instrument");
    return VisuWidgetFactory::createWidget(parent, type, properties, signal);
}

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
    else
    {
        qDebug("Instrument of type %s not recognized.", type.toStdString().c_str());
    }

    Q_ASSERT(widget != nullptr);

    if (signal != nullptr)
    {
        signal->connectInstrument(static_cast<VisuInstrument*>(widget));
        signal->initializeInstruments();
    }

    return widget;
}
