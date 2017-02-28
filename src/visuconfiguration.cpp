#include "visuconfiguration.h"

#include <QVector>
#include <QPushButton>
#include <algorithm>
#include <QSize>
#include <functional>
#include "visusignal.h"
#include "visupropertyloader.h"
#include "visuconfigloader.h"
#include "visumisc.h"
#include "exceptions/configloadexception.h"
#include "instruments/instanalog.h"
#include "instruments/instdigital.h"
#include "instruments/instlinear.h"
#include "instruments/insttimeplot.h"
#include "instruments/instxyplot.h"
#include "instruments/instled.h"
#include "controls/ctrlbutton.h"
#include "statics/staticimage.h"

VisuConfiguration* VisuConfiguration::instance = nullptr;

const QString VisuConfiguration::TAG_NAME = "configuration";

// These tags are not used, except for purpose of verification of XML structure.
// Perhaps some kind of load hooks can be later triggered when reading these tags.
const QString VisuConfiguration::TAG_VISU_CONFIG = "visu_config";
const QString VisuConfiguration::TAG_WIDGETS_PLACEHOLDER = "widgets";
const QString VisuConfiguration::TAG_SIGNALS_PLACEHOLDER = "signals";

#include "wysiwyg/visuwidgetfactory.h"

VisuConfiguration* VisuConfiguration::get()
{
    if (instance == nullptr)
    {
        instance = new VisuConfiguration();
    }
    return instance;
}

VisuConfiguration* VisuConfiguration::getClean()
{
    delete instance;
    instance = nullptr;
    return VisuConfiguration::get();
}


VisuConfiguration::~VisuConfiguration()
{
    for (VisuWidget* widget : widgetsList)
    {
        delete widget;
    }
}

void VisuConfiguration::createSignalFromToken(QXmlStreamReader& xmlReader)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, VisuSignal::TAG_NAME);
    VisuSignal* signal = new VisuSignal(properties);
    signalsList.push_back(signal);
}

QPointer<VisuWidget> VisuConfiguration::createWidgetFromToken(QXmlStreamReader& xmlReader, QWidget *parent)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, VisuWidget::TAG_NAME);
    VisuWidget* widget = VisuWidgetFactory::createWidget(parent,
                                                         properties);
    addWidget(widget);
    widget->show();

    return widget;
}

void VisuConfiguration::initializeInstruments()
{
    for (int i=0; i<signalsList.size(); i++) {
        signalsList[i]->initializeInstruments();
    }
}

void VisuConfiguration::createConfigurationFromToken(QXmlStreamReader& xmlReader)
{
    QMap<QString, QString> properties = VisuConfigLoader::parseToMap(xmlReader, TAG_NAME);
    setConfigValues(properties);
}

void VisuConfiguration::setConfigValues(QMap<QString, QString>& properties)
{
    mProperties = properties;
    QMap<QString, VisuPropertyMeta> emptyMap;
    GET_PROPERTY(cPort, quint16, properties, emptyMap);
    GET_PROPERTY(cWidth, quint16, properties, emptyMap);
    GET_PROPERTY(cHeight, quint16, properties, emptyMap);
    GET_PROPERTY(cColorBackground, QColor, properties, emptyMap);
    GET_PROPERTY(cName, QString, properties, emptyMap);
    GET_PROPERTY(cConectivity, quint8, properties, emptyMap);
}

void VisuConfiguration::fromXML(QWidget *parent, const QString& xmlString)
{
    QXmlStreamReader xmlReader(xmlString);

    while (xmlReader.tokenType() != QXmlStreamReader::EndDocument
           && xmlReader.tokenType() != QXmlStreamReader::Invalid) {

        if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {

            ConfigLoadException::setContext("loading configuration");

            if (xmlReader.name() == VisuSignal::TAG_NAME) {
                createSignalFromToken(xmlReader);
            }
            else if (xmlReader.name() == VisuWidget::TAG_NAME) {
                createWidgetFromToken(xmlReader, parent);
            }
            else if (xmlReader.name() == TAG_NAME) {
                createConfigurationFromToken(xmlReader);
            }
            else if (xmlReader.name() == TAG_VISU_CONFIG) {
                // No actions needed.
            }
            else if (xmlReader.name() == TAG_WIDGETS_PLACEHOLDER) {
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
        return nullptr;
    }
    return signalsList[signalId];
}

QPointer<VisuInstrument> VisuConfiguration::getInstrument(quint16 instrument_id)
{
    return qobject_cast<VisuInstrument*>(widgetsList[instrument_id]);
}

QVector<QPointer<VisuSignal> >& VisuConfiguration::getSignals()
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

QSize VisuConfiguration::getSize() const
{
    return QSize(cWidth, cHeight);
}

QString VisuConfiguration::getName()
{
    return cName;
}

quint8 VisuConfiguration::getConectivity()
{
    return cConectivity;
}


QVector<QPointer<VisuWidget> > VisuConfiguration::getWidgets()
{
    return widgetsList;
}

QPointer<VisuWidget> VisuConfiguration::getWidget(int id)
{
    if (id >= 0 && widgetsList.size() > id)
    {
        return widgetsList.at(id);
    }
    else
    {
        return nullptr;
    }
}

void VisuConfiguration::moveWidgetUp(int id)
{
    int swapId = id + 1;
    int size =  widgetsList.size();
    while (swapId < size)
    {
        if (widgetsList[swapId] != nullptr)
        {
            std::swap(widgetsList[id], widgetsList[swapId]);
            widgetsList[id]->setId(id);
            widgetsList[swapId]->setId(swapId);
            break;
        }
        ++swapId;
    }
}

void VisuConfiguration::moveWidgetDown(int id)
{
    int swapId = id - 1;
    while (id > 0)
    {
        if (widgetsList[swapId] != nullptr)
        {
            std::swap(widgetsList[id], widgetsList[swapId]);
            widgetsList[id]->setId(id);
            widgetsList[swapId]->setId(swapId);
            break;
        }
        --swapId;
    }
}

void VisuConfiguration::addWidget(QPointer<VisuWidget> widget)
{
    int id = getFreeId((QVector<QPointer<QObject>>&)widgetsList);
    widget->setId(id);
    widgetsList[id] = widget;
}

void VisuConfiguration::deleteWidget(QPointer<VisuWidget> widget)
{
    VisuInstrument* instrument;
    if ( (instrument = qobject_cast<VisuInstrument*>(widget)) != nullptr)
    {
        instrument->disconnectSignals();
    }

    delete(widgetsList[widget->getId()]);
}

void VisuConfiguration::addSignal(QPointer<VisuSignal> signal)
{
    int signalId = getFreeId((QVector<QPointer<QObject>>&)signalsList);
    signal->setId(signalId);
    signalsList[signalId] = signal;
}

int VisuConfiguration::getFreeId(QVector<QPointer<QObject>> &list)
{
    auto it = std::find_if(list.begin(), list.end(),
                           [](const QPointer<QObject>& ptr){return ptr == nullptr; });
    int id = it - list.begin();
    if (it == list.end())
    {
        list.resize(id + 1);
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
    delete (signalsList[signalId]);
}

QMap<QString, QString>& VisuConfiguration::getProperties()
{
    return mProperties;
}

template <typename T>
void VisuConfiguration::append(T* elem, QString& xml, int tabs)
{
    if (elem != nullptr)
    {
        xml += VisuMisc::addElement(T::TAG_NAME, elem->getProperties(), tabs);
    }
}

QString VisuConfiguration::toXML()
{
    QString xml;

    xml = VisuMisc::getXMLDeclaration();
    xml += VisuMisc::openTag(TAG_VISU_CONFIG);

    // Configuration properties
    xml += VisuMisc::addElement(TAG_NAME, mProperties, 1);

    // Signals
    xml += VisuMisc::openTag(TAG_SIGNALS_PLACEHOLDER, 1);
    auto appendSignal = std::bind(append<VisuSignal>, std::placeholders::_1, std::ref(xml), 2);
    std::for_each(signalsList.begin(), signalsList.end(), appendSignal);
    xml += VisuMisc::closeTag(TAG_SIGNALS_PLACEHOLDER, 1);

    // Widgets
    xml += VisuMisc::openTag(TAG_WIDGETS_PLACEHOLDER, 1);
    auto appendWidget = std::bind(append<VisuWidget>, std::placeholders::_1, std::ref(xml), 2);
    std::for_each(widgetsList.begin(), widgetsList.end(), appendWidget);
    xml += VisuMisc::closeTag(TAG_WIDGETS_PLACEHOLDER, 1);

    xml += VisuMisc::closeTag(TAG_VISU_CONFIG);

    return xml;
}
