#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "visusignal.h"
#include "visuinstrument.h"
#include "button.h"
#include <QWidget>
#include <QObject>
#include <QXmlStreamReader>
#include <QPointer>
#include <vector>

class VisuConfiguration : public QObject
{
    Q_OBJECT

    private:
        QVector<QPointer<VisuSignal>> signalsList;
        QVector<VisuInstrument*> instrumentsList;
        QVector<Button*> controlsList;
        void createSignalFromToken(QXmlStreamReader& xml_reader);
        void createConfigurationFromToken(QXmlStreamReader& xmlReader);
        void createStaticFromToken(QXmlStreamReader& xmlReader, QWidget *parent);
        void initializeInstruments();
        int getFreeId(QVector<void *>& list);

        // Properties
        quint16 cPort;
        quint16 cWidth;
        quint16 cHeight;
        QColor cColorBackground;
        QString cName;

        QMap<QString, QString> mProperties;

    public:
        void setConfigValues(const QMap<QString, QString>& properties);

        void loadFromXML(QWidget *parent, const QString& xml);
        VisuInstrument* createInstrumentFromToken(QXmlStreamReader& xml_reader, QWidget *parent);
        Button* createControlFromToken(QXmlStreamReader& xmlReader, QWidget *parent);
        void deleteInstrument(QPointer<VisuInstrument> instrument);

        QPointer<VisuSignal> getSignal(quint16 signalId);
        QVector<QPointer<VisuSignal>> &getSignals();
        VisuInstrument* getInstrument(quint16 instrument_id);
        QVector<VisuInstrument*>& getInstruments();

        void detachInstrumentFromSignal(VisuInstrument* instrument);
        void attachInstrumentToSignal(VisuInstrument* instrument);
        void detachInstrumentFromSignal(VisuInstrument* instrument, int signalId);
        void attachInstrumentToSignal(VisuInstrument* instrument, int signalId);

        void addInstrument(VisuInstrument* instrument);
        void addSignal(QPointer<VisuSignal> signal);
        void deleteSignal(QPointer<VisuSignal> signal);
        void deleteSignal(int signalId);

        QMap<QString, QString>& getProperties();
        QString toXML();

        quint16 getPort();
        quint16 getWidth();
        quint16 getHeight();
        QColor getBackgroundColor();
        QString getName();

        static const QString TAG_STATIC;
        static const QString TAG_CONTROL;
        static const QString TAG_INSTRUMENT;
        static const QString TAG_SIGNAL;
        static const QString TAG_CONFIGURATION;
        static const QString ATTR_TYPE;
        static const QString TAG_VISU_CONFIG;
        static const QString TAG_STATICS_PLACEHOLDER;
        static const QString TAG_SIGNALS_PLACEHOLDER;
        static const QString TAG_INSTRUMENTS_PLACEHOLDER;
        static const QString TAG_CONTROLS_PLACEHOLDER;

};

#endif // CONFIGURATION_H
