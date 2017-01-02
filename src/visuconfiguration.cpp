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
#include <algorithm>
#include "exceptions/configloadexception.h"

#define DBG_XML qDebug("XML-%d>> %s", xmlReader.tokenType(), xmlReader.name().toString().toStdString().c_str())

const QString VisuConfiguration::TAG_INSTRUMENT = "instrument";
const QString VisuConfiguration::TAG_CONTROL = "control";
const QString VisuConfiguration::TAG_SIGNAL = "signal";
const QString VisuConfiguration::TAG_CONFIGURATION = "configuration";
const QString VisuConfiguration::ATTR_TYPE = "type";

// These tags are not used, except for purpose of verification of XML structure.
// Perhaps some kind of load hooks can be later triggered when reading these tags.
const QString VisuConfiguration::TAG_VISU_CONFIG = "visu_config";
const QString VisuConfiguration::TAG_SIGNALS_PLACEHOLDER = "signals";
const QString VisuConfiguration::TAG_INSTRUMENTS_PLACEHOLDER = "instruments";
const QString VisuConfiguration::TAG_CONTROLS_PLACEHOLDER = "controls";

VisuConfiguration::VisuConfiguration()
{


}

void VisuConfiguration::attachInstrumentToSignal(VisuInstrument* instrument, int signal_id)
{
    // TODO :: optimize
    int size = signalsList.size();
    for (int i=0; i<size; i++) {
        if (signalsList[i]->getId() == signal_id) {
            signalsList[i]->connectInstrument(instrument);
        }
    }
    // std::find_if(signals_list.begin(), signals_list.end(), [instrument_id](VisuSignal* s){ return s->get_id() == instrument_id; } );
}

void VisuConfiguration::attachInstrumentToSignal(VisuInstrument* instrument)
{
    quint16 signal_id = instrument->getSignalId();
    attachInstrumentToSignal(instrument, signal_id);
}

QMap<QString, QString> VisuConfiguration::parseToMap(QXmlStreamReader& xmlReader, QString element)
{
    QMap<QString, QString> map;
    QString name;
    QString value;

    while ( xmlReader.tokenType() != QXmlStreamReader::EndElement
             || xmlReader.name() != element) {

        if (xmlReader.tokenType() == QXmlStreamReader::Invalid) {
            QString errorMsg = xmlReader.errorString() + " Near node: \"%1\"";
            throw ConfigLoadException(errorMsg, xmlReader.name().toString());
        }

        if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {
            name = xmlReader.name().toString();
        }

        if (xmlReader.tokenType() == QXmlStreamReader::Characters
                && !xmlReader.isWhitespace()) {
            value = xmlReader.text().toString();

            map[name] = value;

            //DBG_XML;
        }

        xmlReader.readNext();

    }

    return map;

}

void VisuConfiguration::createSignalFromToken(QXmlStreamReader& xmlReader)
{
    QMap<QString, QString> properties = parseToMap(xmlReader, TAG_SIGNAL);
    VisuSignal* signal = new VisuSignal(properties);
    signalsList.push_back(signal);
}

void VisuConfiguration::createInstrumentFromToken(QXmlStreamReader& xmlReader, QWidget *parent)
{
    QMap<QString, QString> properties = parseToMap(xmlReader, TAG_INSTRUMENT);
    VisuInstrument* instrument;

    if (properties[ATTR_TYPE] == InstAnalog::TAG_NAME) {
        instrument = new InstAnalog(parent, properties);
    }
    else if (properties[ATTR_TYPE] == InstDigital::TAG_NAME) {
        instrument = new InstDigital(parent, properties);
    }
    else if (properties[ATTR_TYPE] == InstLinear::TAG_NAME) {
        instrument = new InstLinear(parent, properties);
    }
    else if (properties[ATTR_TYPE] == InstTimePlot::TAG_NAME) {
        instrument = new InstTimePlot(parent, properties);
    }
    else if (properties[ATTR_TYPE] == InstXYPlot::TAG_NAME) {
        instrument = new InstXYPlot(parent, properties);
        quint16 signalYId = ((InstXYPlot*)instrument)->getSignalYId();
        attachInstrumentToSignal(instrument, signalYId);
    }
    else if (properties[ATTR_TYPE] == InstLED::TAG_NAME) {
        instrument = new InstLED(parent, properties);
    }
    else {
        throw ConfigLoadException("Instrument %1 not recognized", properties[ATTR_TYPE]);
    }

    attachInstrumentToSignal(instrument);
    instrument->show();
    instrumentsList.append(instrument);
}

void VisuConfiguration::createControlFromToken(QXmlStreamReader& xmlReader, QWidget *parent)
{
    QMap<QString, QString> properties = parseToMap(xmlReader, TAG_CONTROL);

    if (properties[ATTR_TYPE] == Button::TAG_NAME) {
        new Button(parent, properties);
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
    QMap<QString, QString> properties = parseToMap(xmlReader, TAG_CONFIGURATION);
    GET_PROPERTY(port, quint16, properties);
    GET_PROPERTY(width, quint16, properties);
    GET_PROPERTY(height, quint16, properties);
    GET_PROPERTY(color_background, QColor, properties);
    GET_PROPERTY(name, QString, properties);

    qDebug("Loading configuration, size: %dx%d", width, height);
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

VisuSignal* VisuConfiguration::getSignal(quint16 signal_id)
{
    if (signal_id < signalsList.size()) {
        return signalsList[signal_id];
    }

    return NULL;
}

VisuInstrument* VisuConfiguration::getInstrument(quint16 instrument_id)
{
    return instrumentsList[instrument_id];
}

QVector<VisuInstrument*>& VisuConfiguration::getInstruments()
{
    return instrumentsList;
}

quint16 VisuConfiguration::getPort()
{
    return port;
}

quint16 VisuConfiguration::getWidth()
{
    return width;
}

quint16 VisuConfiguration::getHeight()
{
    return height;
}

QColor VisuConfiguration::getBackgroundColor()
{
    return color_background;
}

QString VisuConfiguration::getName()
{
    return name;
}
