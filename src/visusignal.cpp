#include "visusignal.h"

QMap<QString, QString>& VisuSignal::getProperties()
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
    double value = getRealValue() / (cMax - cMin);
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
void VisuSignal::initialUpdate()
{
    mRawValue = 0;
    mTimestamp = 0;

    notifyInstruments();
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

void VisuSignal::load(QMap<QString, QString> properties)
{
    mProperties = properties;

    GET_PROPERTY(cId, quint16, properties);
    GET_PROPERTY(cName, QString, properties);
    GET_PROPERTY(cUnit, QString, properties);
    GET_PROPERTY(cFactor, double, properties);
    GET_PROPERTY(cOffset, double, properties);
    GET_PROPERTY(cMax, double, properties);
    GET_PROPERTY(cMin, double, properties);
}
