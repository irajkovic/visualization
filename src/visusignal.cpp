#include "visusignal.h"

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
    return id;
}

/**
 * @brief Signal::getRawValue
 * Returns signal raw value
 * @return
 */
quint64 VisuSignal::getRawValue() const
{
    return raw_value;
}

/**
 * @brief Signal::getFactor
 * Returns signal factor.
 * @return
 */
double VisuSignal::getFactor() const
{
    return factor;
}

/**
 * @brief Signal::getOffset
 * Returns signal offset.
 * @return
 */
double VisuSignal::getOffset() const
{
    return offset;
}

/**
 * @brief Signal::getRealValue
 * Returns signal real value.
 * @return
 */
double VisuSignal::getRealValue() const
{
    return raw_value * factor + offset;
}

double VisuSignal::getNormalizedValue() const
{
    double value = (double)raw_value / (max - min);
    return value >= 0.0 && value <= 1.0 ? value : 0.0;
}

/**
 * @brief Signal::datagramUpdate
 * @param datagram
 */
void VisuSignal::datagramUpdate(const VisuDatagram& datagram)
{

    raw_value = datagram.rawValue;
    timestamp = datagram.timestamp;

    notifyInstruments();
}

/**
 * @brief VisuSignal::initialUpdate
 * Called during instrument initialization, so instrument can pickup
 * pointer to signal and adjust its properties accordingly.
 */
void VisuSignal::initialUpdate()
{
    raw_value = 0;
    timestamp = 0;

    notifyInstruments();
}

double VisuSignal::getMin() const
{
    return min;
}

double VisuSignal::getMax() const
{
    return max;
}

QString VisuSignal::getName() const
{
    return name;
}

QString VisuSignal::getUnit() const
{
    return unit;
}

quint64 VisuSignal::getTimestamp() const
{
    return timestamp;
}
