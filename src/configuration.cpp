#include "configuration.h"

#include <QVector>
#include "signal.h"
#include "instruments/instanalog.h"
#include "instruments/instdigital.h"
#include "instruments/instlinear.h"
#include "instruments/insttimeplot.h"
#include "instruments/instxyplot.h"
#include "instruments/instled.h"
#include "visu_helper.h"
#include <algorithm>
#include "exceptions/configloadexception.h"

#define DBG_XML qDebug("XML-%d>> %s", xml_reader.tokenType(), xml_reader.name().toString().toStdString().c_str())


const QString VisuConfiguration::TAG_INSTRUMENT = "instrument";
const QString VisuConfiguration::TAG_SIGNAL = "signal";
const QString VisuConfiguration::TAG_CONFIGURATION = "configuration";
const QString VisuConfiguration::ATTR_TYPE = "type";

// These tags are not used, except for purpose of verification of XML structure.
// Perhaps some kind of load hooks can be later triggered when reading these tags.
const QString VisuConfiguration::TAG_VISU_CONFIG = "visu_config";
const QString VisuConfiguration::TAG_SIGNALS_PLACEHOLDER = "signals";
const QString VisuConfiguration::TAG_INSTRUMENTS_PLACEHOLDER = "instruments";

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

QMap<QString, QString> VisuConfiguration::parseToMap(QXmlStreamReader& xml_reader, QString element)
{
    QMap<QString, QString> map;
    QString name;
    QString value;

    while ( xml_reader.tokenType() != QXmlStreamReader::EndElement
             || xml_reader.name() != element) {

        if (xml_reader.tokenType() == QXmlStreamReader::Invalid) {
            QString errorMsg = xml_reader.errorString() + " Near node: \"%1\"";
            throw ConfigLoadException(errorMsg, xml_reader.name().toString());
        }

        if (xml_reader.tokenType() == QXmlStreamReader::StartElement) {
            name = xml_reader.name().toString();
        }

        if (xml_reader.tokenType() == QXmlStreamReader::Characters
                && !xml_reader.isWhitespace()) {
            value = xml_reader.text().toString();

            map[name] = value;

            //DBG_XML;
        }

        xml_reader.readNext();

    }

    return map;

}

void VisuConfiguration::createSignalFromToken(QXmlStreamReader& xml_reader)
{
    QMap<QString, QString> properties = parseToMap(xml_reader, TAG_SIGNAL);
    VisuSignal* signal = new VisuSignal(properties);
    signalsList.push_back(signal);
}

void VisuConfiguration::createInstrumentFromToken(QXmlStreamReader& xml_reader, QWidget *parent)
{
    QMap<QString, QString> properties = parseToMap(xml_reader, TAG_INSTRUMENT);
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

void VisuConfiguration::initializeInstruments()
{
    for (unsigned int i=0; i<signalsList.size(); i++) {
        signalsList[i]->initialUpdate();
    }
}

void VisuConfiguration::createConfigurationFromToken(QXmlStreamReader& xml_reader)
{
    QMap<QString, QString> properties = parseToMap(xml_reader, TAG_CONFIGURATION);
    GET_PROPERTY(port, quint16);
    GET_PROPERTY(width, quint16);
    GET_PROPERTY(height, quint16);
    GET_PROPERTY(color_background, QColor);
    GET_PROPERTY(name, QString);

    qDebug("Loading configuration, size: %dx%d", width, height);
}


void VisuConfiguration::loadFromXML(QWidget *parent, QString xml_string)
{
    QXmlStreamReader xml_reader(xml_string);

    while (xml_reader.tokenType() != QXmlStreamReader::EndDocument
           && xml_reader.tokenType() != QXmlStreamReader::Invalid) {

        if (xml_reader.tokenType() == QXmlStreamReader::StartElement) {

            ConfigLoadException::setContext("loading configuration");

            if (xml_reader.name() == TAG_SIGNAL) {
                createSignalFromToken(xml_reader);
            }
            else if (xml_reader.name() == TAG_INSTRUMENT) {
                createInstrumentFromToken(xml_reader, parent);
            }
            else if (xml_reader.name() == TAG_CONFIGURATION) {
                createConfigurationFromToken(xml_reader);
            }
            else if (xml_reader.name() == TAG_VISU_CONFIG) {
                // No actions needed.
            }
            else if (xml_reader.name() == TAG_INSTRUMENTS_PLACEHOLDER) {
                // No actions needed.
            }
            else if (xml_reader.name() == TAG_SIGNALS_PLACEHOLDER) {
                // No actions needed.
            }
            else
            {
                throw ConfigLoadException("Unknown XML node \"%1\"", xml_reader.name().toString());
            }
        }

        xml_reader.readNext();

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
