#include "visuconfiguration.h"

#include <QVector>
#include <QPushButton>
#include "visusignal.h"
#include "instruments/instanalog.h"
#include "instruments/instdigital.h"
#include "instruments/instlinear.h"
#include "instruments/insttimeplot.h"
#include "instruments/instxyplot.h"
#include "instruments/instled.h"
#include "visuhelper.h"
#include "controls/button.h"
#include "statics/staticimage.h"
#include <algorithm>
#include "exceptions/configloadexception.h"
#include "visuconfigloader.h"

#define DBG_XML qDebug("XML-%d>> %s", xmlReader.tokenType(), xmlReader.name().toString().toStdString().c_str())

const QString VisuConfiguration::TAG_INSTRUMENT = "instrument";
const QString VisuConfiguration::TAG_CONTROL = "control";
const QString VisuConfiguration::TAG_SIGNAL = "signal";
const QString VisuConfiguration::TAG_STATIC = "static";
const QString VisuConfiguration::TAG_CONFIGURATION = "configuration";
const QString VisuConfiguration::ATTR_TYPE = "type";

// These tags are not used, except for purpose of verification of XML structure.
// Perhaps some kind of load hooks can be later triggered when reading these tags.
const QString VisuConfiguration::TAG_VISU_CONFIG = "visu_config";
const QString VisuConfiguration::TAG_STATICS_PLACEHOLDER = "statics";
const QString VisuConfiguration::TAG_SIGNALS_PLACEHOLDER = "signals";
const QString VisuConfiguration::TAG_INSTRUMENTS_PLACEHOLDER = "instruments";
const QString VisuConfiguration::TAG_CONTROLS_PLACEHOLDER = "controls";

#include "wysiwyg/visuwidgetfactory.h"

VisuConfiguration::VisuConfiguration()
{


}

void VisuConfiguration::attachInstrumentToSignal(VisuInstrument* instrument, int signalId)
{
    getSignal(signalId)->connectInstrument(instrument);
}

void VisuConfiguration::detachInstrumentFromSignal(VisuInstrument* instrument, int signalId)
{
    getSignal(signalId)->disconnectInstrument(instrument);
}

void VisuConfiguration::attachInstrumentToSignal(VisuInstrument* instrument)
{
    quint16 signalId = instrument->getSignalId();
    attachInstrumentToSignal(instrument, signalId);
}

void VisuConfiguration::detachInstrumentFromSignal(VisuInstrument* instrument)
{
    quint16 signalId = instrument->getSignalId();
    detachInstrumentFromSignal(instrument, signalId);
}

void VisuConfiguration::createSignalFromToken(QXmlStreamReader& xmlReader)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, TAG_SIGNAL);
    VisuSignal* signal = new VisuSignal(properties);
    signalsList.push_back(signal);
}

void VisuConfiguration::createInstrumentFromToken(QXmlStreamReader& xmlReader, QWidget *parent)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, TAG_INSTRUMENT);

    VisuWidget* widget = VisuWidgetFactory::createWidget(parent, properties[ATTR_TYPE], properties);
    VisuInstrument* instrument = static_cast<VisuInstrument*>(widget);

    attachInstrumentToSignal(instrument);
    instrument->show();
    instrumentsList.append(instrument);
}

void VisuConfiguration::createControlFromToken(QXmlStreamReader& xmlReader, QWidget *parent)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, TAG_CONTROL);

    if (properties[ATTR_TYPE] == Button::TAG_NAME) {
        new Button(parent, properties);
    }
}

void VisuConfiguration::createStaticFromToken(QXmlStreamReader& xmlReader, QWidget *parent)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, TAG_STATIC);

    if (properties[ATTR_TYPE] == StaticImage::TAG_NAME) {
        new StaticImage(parent, properties);
    }
}

void VisuConfiguration::initializeInstruments()
{
    for (unsigned int i=0; i<signalsList.size(); i++) {
        signalsList[i]->initialUpdate();
    }
}

void VisuConfiguration::createConfigurationFromToken(QXmlStreamReader& xmlReader)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, TAG_CONFIGURATION);
    GET_PROPERTY(cPort, quint16, properties);
    GET_PROPERTY(cWidth, quint16, properties);
    GET_PROPERTY(cHeight, quint16, properties);
    GET_PROPERTY(cColorBackground, QColor, properties);
    GET_PROPERTY(cName, QString, properties);

    qDebug("Loading configuration, size: %dx%d", cWidth, cHeight);
}


void VisuConfiguration::loadFromXML(QWidget *parent, QString xmlString)
{
    QXmlStreamReader xmlReader(xmlString);

    while (xmlReader.tokenType() != QXmlStreamReader::EndDocument
           && xmlReader.tokenType() != QXmlStreamReader::Invalid) {

        if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {

            ConfigLoadException::setContext("loading configuration");

            if (xmlReader.name() == TAG_SIGNAL) {
                createSignalFromToken(xmlReader);
            }
            else if (xmlReader.name() == TAG_INSTRUMENT) {
                createInstrumentFromToken(xmlReader, parent);
            }
            else if (xmlReader.name() == TAG_CONTROL) {
                createControlFromToken(xmlReader, parent);
            }
            else if (xmlReader.name() == TAG_STATIC) {
                createStaticFromToken(xmlReader, parent);
            }
            else if (xmlReader.name() == TAG_CONFIGURATION) {
                createConfigurationFromToken(xmlReader);
            }
            else if (xmlReader.name() == TAG_VISU_CONFIG) {
                // No actions needed.
            }
            else if (xmlReader.name() == TAG_INSTRUMENTS_PLACEHOLDER) {
                // No actions needed.
            }
            else if (xmlReader.name() == TAG_CONTROLS_PLACEHOLDER) {
                // No actions needed.
            }
            else if (xmlReader.name() == TAG_STATICS_PLACEHOLDER) {
                // No actions needed.
            }
            else if (xmlReader.name() == TAG_SIGNALS_PLACEHOLDER) {
                // No actions needed.
            }
            else
            {
                throw ConfigLoadException("Unknown XML node \"%1\"", xmlReader.name().toString());
            }
        }

        xmlReader.readNext();

    }

    initializeInstruments();
}

VisuSignal* VisuConfiguration::getSignal(quint16 signalId)
{
    if(signalId >= signalsList.size())
    {
        throw ConfigLoadException(QString("Signal id %1 too large!").arg(signalId));
    }
    return signalsList[signalId];
}

VisuInstrument* VisuConfiguration::getInstrument(quint16 instrument_id)
{
    return instrumentsList[instrument_id];
}

QVector<VisuInstrument*>& VisuConfiguration::getInstruments()
{
    return instrumentsList;
}

QVector<VisuSignal*>& VisuConfiguration::getSignals()
{
    return signalsList;
}

quint16 VisuConfiguration::getPort()
{
    return cPort;
}

quint16 VisuConfiguration::getWidth()
{
    return cWidth;
}

quint16 VisuConfiguration::getHeight()
{
    return cHeight;
}

QColor VisuConfiguration::getBackgroundColor()
{
    return cColorBackground;
}

QString VisuConfiguration::getName()
{
    return cName;
}
