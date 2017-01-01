#ifndef SIGNAL_H
#define SIGNAL_H

#include <QtGlobal>
#include <QVector>
#include <QString>

#include "visuinstrument.h"
#include "visudatagram.h"
#include "visuhelper.h"

class VisuInstrument;   // forward declare Instrument class
class VisuSignal : QObject
{
    Q_OBJECT

private:

    // properties
    quint16 id;                             // Signal ID
    QString name;                           // Signal name
    QString unit;                           // Signal unit
    double  factor;                         // Signal factor used in real value calculation
    double  offset;                         // Signal offset used in real value calculation
    double  max;                            // Maximum signal value
    double  min;                            // Minimum signal value

    quint64 timestamp;                      // Last update timestamp
    quint64 raw_value;                      // Last value

    // methods
    void notifyInstruments();

signals:
    void signalUpdated(const VisuSignal* const);

public:
    VisuSignal();
    VisuSignal(QMap<QString, QString> properties)
    {
        GET_PROPERTY(id, quint16);
        GET_PROPERTY(name, QString);
        GET_PROPERTY(unit, QString);
        GET_PROPERTY(factor, double);
        GET_PROPERTY(offset, double);
        GET_PROPERTY(max, double);
        GET_PROPERTY(min, double);
    }

    void initialUpdate();
    void datagramUpdate(const VisuDatagram& datagram);
    void set_raw_ralue(quint64 value);
    quint64 getRawValue() const;
    void set_timestamp(quint64 timestamp);
    quint64 getTimestamp() const;
    quint16 getId() const;
    double getFactor() const;
    double getOffset() const;
    double getRealValue() const;
    double getNormalizedValue() const;
    double getMin() const;
    double getMax() const;
    QString getName() const;
    QString getUnit() const;

    // observer interface
    void connectInstrument(VisuInstrument* instrument);
    void disconnectInstrument(VisuInstrument* instrument);
};

#endif // SIGNAL_H
