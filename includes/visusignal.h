#ifndef SIGNAL_H
#define SIGNAL_H

#include <QtGlobal>
#include <QVector>
#include <QString>

#include "visuinstrument.h"
#include "visudatagram.h"
#include "visuhelper.h"

class VisuInstrument;   // forward declare Instrument class
class VisuSignal : public QObject
{
    Q_OBJECT

private:

    // configuration properties
    quint16 cId;                             // Signal ID
    QString cName;                           // Signal name
    QString cUnit;                           // Signal unit
    double  cFactor;                         // Signal factor used in real value calculation
    double  cOffset;                         // Signal offset used in real value calculation
    double  cMax;                            // Maximum signal value
    double  cMin;                            // Minimum signal value

    quint64 mTimestamp;                      // Last update timestamp
    quint64 mRawValue;                      // Last value
    QMap<QString, QString> mProperties;

    // methods
    void notifyInstruments();

signals:
    void signalUpdated(const VisuSignal* const);

public:
    VisuSignal(QMap<QString, QString> properties)
    {
        load(properties);
    }

    QMap<QString, QString>& getProperties();
    void load(QMap<QString, QString> properties);
    void initialUpdate();
    void datagramUpdate(const VisuDatagram& datagram);
    void set_raw_ralue(quint64 value);
    quint64 getRawValue() const;
    void set_timestamp(quint64 mTimestamp);
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
