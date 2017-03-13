#include "visusignal.h"

const QString VisuSignal::TAG_NAME = "signal";

VisuSignal::VisuSignal(const QMap<QString, QString>& properties)
{
    mProperties = properties;
    mPropertiesMeta = VisuConfigLoader::getMetaMapFromFile( VisuSignal::TAG_NAME,
                                                            VisuSignal::TAG_NAME);
    load();
}

const QMap<QString, QString>& VisuSignal::getProperties()
{
    return mProperties;
}

/**
 * @brief Signal::connectInstrument
 * Adds instrument to notify list.
 * @param instrument
 */
void VisuSignal::connectInstrument(VisuInstrument* instrument)
{
    QObject::connect(this,
                     SIGNAL(initialUpdate(const VisuSignal* const)),
                     instrument,
                     SLOT(initialUpdate(const VisuSignal* const)));
    QObject::connect(this,
                     SIGNAL(signalUpdated(const VisuSignal* const)),
                     instrument,
                     SLOT(signalUpdated(const VisuSignal* const)));
}

/**
 * @brief Signal::disconnectInstrument
 * Removes instrument from notify list.
 * @param instrument
 */
void VisuSignal::disconnectInstrument(VisuInstrument* instrument)
{
    QObject::disconnect(this,
                     SIGNAL(initialUpdate(const VisuSignal* const)),
                     instrument,
                     SLOT(initialUpdate(const VisuSignal* const)));
    QObject::disconnect(this,
                     SIGNAL(signalUpdated(const VisuSignal* const)),
                     instrument,
                     SLOT(signalUpdated(const VisuSignal* const)));
}

/**
 * @brief Signal::notifyInstruments
 * Notifies all instruments that signal value is changed.
 */
void VisuSignal::notifyInstruments()
{
    emit signalUpdated(this);
}

quint16 VisuSignal::getId() const
{
    return cId;
}

void VisuSignal::setId(quint16 id)
{
    cId = id;
    mProperties["id"] = QString("%1").arg(id);
}

/**
 * @brief Signal::getRawValue
 * Returns signal raw value
 * @return
 */
quint64 VisuSignal::getRawValue() const
{
    return mRawValue;
}

/**
 * @brief Signal::getFactor
 * Returns signal factor.
 * @return
 */
double VisuSignal::getFactor() const
{
    return cFactor;
}

/**
 * @brief Signal::getOffset
 * Returns signal offset.
 * @return
 */
double VisuSignal::getOffset() const
{
    return cOffset;
}

/**
 * @brief Signal::getRealValue
 * Returns signal real value.
 * @return
 */
double VisuSignal::getRealValue() const
{
    return mRawValue * cFactor + cOffset;
}

double VisuSignal::getNormalizedValue() const
{
    double value = (getRealValue() - cMin) / (cMax - cMin);
    if (value < 0.0 || value > 1.0)
    {
        value = 0.0;
        qDebug("Signal id=%d outside of range (min=%f, max=%f, received=%f.", cId, cMin, cMax, getRealValue());
    }
    return value;
}

/**
 * @brief Signal::datagramUpdate
 * @param datagram
 */
void VisuSignal::datagramUpdate(const VisuDatagram& datagram)
{

    mRawValue = datagram.rawValue;
    mTimestamp = datagram.timestamp;

    notifyInstruments();
}

/**
 * @brief VisuSignal::initialUpdate
 * Called during instrument initialization, so instrument can pickup
 * pointer to signal and adjust its properties accordingly.
 */
void VisuSignal::initializeInstruments()
{
    mRawValue = (cMin - cOffset) / cFactor;  // TODO :: Use default value
    mTimestamp = 0;

    emit(initialUpdate(this));
}

double VisuSignal::getMin() const
{
    return cMin;
}

double VisuSignal::getMax() const
{
    return cMax;
}

QString VisuSignal::getName() const
{
    return cName;
}

QString VisuSignal::getUnit() const
{
    return cUnit;
}

quint64 VisuSignal::getTimestamp() const
{
    return mTimestamp;
}

int VisuSignal::getSerialPlaceholder() const
{
    return cSerialPlaceholder;
}

bool VisuSignal::getSerialTransform() const
{
    return cSerialTransform;
}

const QMap<QString, VisuPropertyMeta>& VisuSignal::getPropertiesMeta()
{
    return mPropertiesMeta;
}


void VisuSignal::setPropertiesMeta(const QMap<QString, VisuPropertyMeta>& meta)
{
    mPropertiesMeta = meta;
}


void VisuSignal::updateProperty(QString key, QString value)
{
    mProperties[key] = value;
    load();
}

void VisuSignal::load()
{
    GET_PROPERTY(cId, mProperties, mPropertiesMeta);
    GET_PROPERTY(cName, mProperties, mPropertiesMeta);
    GET_PROPERTY(cUnit, mProperties, mPropertiesMeta);
    GET_PROPERTY(cFactor, mProperties, mPropertiesMeta);
    GET_PROPERTY(cOffset, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMax, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMin, mProperties, mPropertiesMeta);
    GET_PROPERTY(cSerialPlaceholder, mProperties, mPropertiesMeta);
    GET_PROPERTY(cSerialTransform, mProperties, mPropertiesMeta);
}
