#include "visuconfiguration.h"

#include <QVector>
#include <QPushButton>
#include <algorithm>

#include "visusignal.h"
#include "visuhelper.h"
#include "visuconfigloader.h"
#include "visumisc.h"
#include "exceptions/configloadexception.h"
#include "instruments/instanalog.h"
#include "instruments/instdigital.h"
#include "instruments/instlinear.h"
#include "instruments/insttimeplot.h"
#include "instruments/instxyplot.h"
#include "instruments/instled.h"
#include "controls/button.h"
#include "statics/staticimage.h"

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

VisuConfiguration::~VisuConfiguration()
{
    for (VisuInstrument* instrument : instrumentsList)
    {
        delete instrument;
    }

    for (VisuSignal* visuSig : signalsList)
    {
        delete visuSig;
    }

    for (Button* button : controlsList)
    {
        delete button;
    }
}

void VisuConfiguration::attachInstrumentToSignal(QPointer<VisuInstrument> instrument, int signalId)
{
    getSignal(signalId)->connectInstrument(instrument);
}

void VisuConfiguration::detachInstrumentFromSignal(QPointer<VisuInstrument> instrument, int signalId)
{
    getSignal(signalId)->disconnectInstrument(instrument);
}

void VisuConfiguration::attachInstrumentToSignal(QPointer<VisuInstrument> instrument)
{
    quint16 signalId = instrument->getSignalId();
    attachInstrumentToSignal(instrument, signalId);
}

void VisuConfiguration::detachInstrumentFromSignal(QPointer<VisuInstrument> instrument)
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

void VisuConfiguration::deleteInstrument(QPointer<VisuInstrument> instrument)
{
    detachInstrumentFromSignal(instrument);
    instrumentsList[instrument->getId()] = nullptr;
    delete instrument;
}

void VisuConfiguration::deleteControl(QPointer<Button> control)
{
    controlsList[control->getId()] = nullptr;
    delete control;
}

QPointer<VisuInstrument> VisuConfiguration::createInstrumentFromToken(QXmlStreamReader& xmlReader, QWidget *parent)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, TAG_INSTRUMENT);

    VisuWidget* widget = VisuWidgetFactory::createInstrument(parent, properties[ATTR_TYPE], properties);
    VisuInstrument* instrument = static_cast<VisuInstrument*>(widget);

    attachInstrumentToSignal(instrument);
    addInstrument(instrument);
    widget->show();

    return instrument;
}

Button* VisuConfiguration::createControlFromToken(QXmlStreamReader& xmlReader, QWidget *parent)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, TAG_CONTROL);
    Button* control;

    if (properties[ATTR_TYPE] == Button::TAG_NAME) {
        control = new Button(parent, properties);
    }

    addControl(control);

    return control;
}

StaticImage* VisuConfiguration::createStaticFromToken(QXmlStreamReader& xmlReader, QWidget *parent)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, TAG_STATIC);
    StaticImage* image;

    if (properties[ATTR_TYPE] == StaticImage::TAG_NAME) {
        image = new StaticImage(parent, properties);
    }

    addStatic(image);

    return image;
}

void VisuConfiguration::initializeInstruments()
{
    for (int i=0; i<signalsList.size(); i++) {
        signalsList[i]->initializeInstruments();
    }
}

void VisuConfiguration::createConfigurationFromToken(QXmlStreamReader& xmlReader)
{
    setConfigValues(VisuConfigLoader::parseToMap(xmlReader, TAG_CONFIGURATION));
    qDebug("Loading configuration, size: %dx%d", cWidth, cHeight);
}

void VisuConfiguration::setConfigValues(const QMap<QString, QString>& properties)
{
    mProperties = properties;
    GET_PROPERTY(cPort, quint16, properties);
    GET_PROPERTY(cWidth, quint16, properties);
    GET_PROPERTY(cHeight, quint16, properties);
    GET_PROPERTY(cColorBackground, QColor, properties);
    GET_PROPERTY(cName, QString, properties);
}

void VisuConfiguration::loadFromXML(QWidget *parent, const QString& xmlString)
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

QPointer<VisuSignal> VisuConfiguration::getSignal(quint16 signalId)
{
    if(signalId >= signalsList.size())
    {
        // We shouldn't throw unhandled exception here, as that would
        // mean that signal source can crash application if wrong
        // signal id is sent.
        qDebug("Signal id %d too large!", signalId);
    }
    return signalsList[signalId];
}

QPointer<VisuInstrument> VisuConfiguration::getInstrument(quint16 instrument_id)
{
    return instrumentsList[instrument_id];
}

QVector<QPointer<VisuInstrument>>& VisuConfiguration::getInstruments()
{
    return instrumentsList;
}

QVector<QPointer<VisuSignal>>& VisuConfiguration::getSignals()
{
    return signalsList;
}


QVector<QPointer<Button>>& VisuConfiguration::getControls()
{
    return controlsList;
}

QVector<QPointer<StaticImage>>& VisuConfiguration::getStatics()
{
    return staticsList;
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

void VisuConfiguration::addStatic(QPointer<StaticImage> image)
{
    int staticId = getFreeId((QVector<QPointer<QObject>>&)staticsList);
    image->setId(staticId);
    staticsList[staticId] = image;
}

void VisuConfiguration::deleteStatic(int staticId)
{
    delete (staticsList[staticId]);
}

void VisuConfiguration::addControl(QPointer<Button> control)
{
    int controlId = getFreeId((QVector<QPointer<QObject>>&)controlsList);
    control->setId(controlId);
    controlsList[controlId] = control;
}

void VisuConfiguration::addInstrument(QPointer<VisuInstrument> instrument)
{
    int instId = getFreeId((QVector<QPointer<QObject>>&)instrumentsList);
    instrument->setId(instId);
    instrumentsList[instId] = instrument;
}

void VisuConfiguration::addSignal(QPointer<VisuSignal> signal)
{
    int signalId = getFreeId((QVector<QPointer<QObject>>&)signalsList);
    signal->setId(signalId);
    signalsList[signalId] = signal;
}

int VisuConfiguration::getFreeId(QVector<QPointer<QObject>> &list)
{
    int id = -1;
    int size = list.size();
    for (int i=0 ; i<size; ++i)
    {
        if (list[i] == nullptr)
        {
            id = i;
        }
    }

    if (id == -1)
    {
        id = size;
        list.resize(size + 1);
    }

    return id;
}

void VisuConfiguration::deleteSignal(QPointer<VisuSignal> signal)
{
    int signalId = signal->getId();
    deleteSignal(signalId);
}

void VisuConfiguration::deleteSignal(int signalId)
{
    // pointer will be cleared automaticaly by QPointer
    QPointer<VisuSignal> ptr = signalsList[signalId];
    delete ptr;
    //delete (signalsList[signalId]);
    qDebug("deleted");
}

QMap<QString, QString>& VisuConfiguration::getProperties()
{
    return mProperties;
}

QString VisuConfiguration::toXML()
{
    QString xml;

    xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    xml += "<visu_config>\n";
    xml += "\t<configuration>\n";
    xml += VisuMisc::mapToString(mProperties, 2);
    xml += "\t</configuration>\n";

    xml += "\t<signals>\n";
    for (VisuSignal* signal : signalsList)
    {
        if (signal != nullptr)
        {
            xml += "\t\t<signal>\n";
            xml += VisuMisc::mapToString(signal->getProperties(), 3);
            xml += "\t\t</signal>\n";
        }
    }
    xml += "\t</signals>\n";

    xml += "\t<instruments>\n";
    for (VisuInstrument* instrument : getInstruments())
    {
        if (instrument != nullptr)
        {
            xml += "\t\t<instrument>\n";
            xml += VisuMisc::mapToString(instrument->getProperties(), 3);
            xml += "\t\t</instrument>\n";
        }
    }
    xml += "\t</instruments>\n";

    xml += "\t<controls>\n";
    for (Button* control : controlsList)
    {
        if (control != nullptr)
        {
            xml += "\t\t<control>\n";
            xml += VisuMisc::mapToString(control->getProperties(), 3);
            xml += "\t\t</control>\n";
        }
    }
    xml += "\t</controls>\n";

    xml += "\t<statics>\n";
    for (StaticImage* image : staticsList)
    {
        if (image != nullptr)
        {
            xml += "\t\t<static>\n";
            xml += VisuMisc::mapToString(image->getProperties(), 3);
            xml += "\t\t</static>\n";
        }
    }
    xml += "\t</statics>\n";

    xml += "<visu_config>\n";

    return xml;
}
