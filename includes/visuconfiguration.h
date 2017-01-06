#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "signal.h"
#include "visuinstrument.h"
#include <QWidget>
#include <QXmlStreamReader>
#include <vector>

class VisuConfiguration
{

    private:
        std::vector<VisuSignal*> signalsList;
        QVector<VisuInstrument*> instrumentsList;
        void attachInstrumentToSignal(VisuInstrument* instrument);
        void attachInstrumentToSignal(VisuInstrument* instrument, int signalId);
        void createSignalFromToken(QXmlStreamReader& xml_reader);
        void createInstrumentFromToken(QXmlStreamReader& xml_reader, QWidget *parent);
        void createConfigurationFromToken(QXmlStreamReader& xmlReader);
        void createControlFromToken(QXmlStreamReader& xmlReader, QWidget *parent);
        void createStaticFromToken(QXmlStreamReader& xmlReader, QWidget *parent);
        void initializeInstruments();

        // Properties
        quint16 cPort;
        quint16 cWidth;
        quint16 cHeight;
        QColor cColorBackground;
        QString cName;

    public:
        VisuConfiguration();
        void loadFromXML(QWidget *parent, QString xml);
        VisuSignal* getSignal(quint16 signalId);
        VisuInstrument* getInstrument(quint16 instrument_id);
        QVector<VisuInstrument*>& getInstruments();

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
