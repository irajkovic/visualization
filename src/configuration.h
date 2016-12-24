#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "signal.h"
#include "instrument.h"
#include <QWidget>
#include <QXmlStreamReader>
#include <vector>

class VisuConfiguration
{

    private:
        std::vector<VisuSignal*> signalsList;
        QVector<VisuInstrument*> instrumentsList;
        QMap<QString, QString> parseToMap(QXmlStreamReader& xml_reader, QString element);
        void attachInstrumentToSignal(VisuInstrument* instrument);
        void attachInstrumentToSignal(VisuInstrument* instrument, int signal_id);
        void createSignalFromToken(QXmlStreamReader& xml_reader);
        void createInstrumentFromToken(QXmlStreamReader& xml_reader, QWidget *parent);
        void createConfigurationFromToken(QXmlStreamReader& xml_reader);
        void initializeInstruments();

        // Properties
        quint16 port;
        quint16 width;
        quint16 height;
        QColor color_background;
        QString name;

    public:
        VisuConfiguration();
        void loadFromXML(QWidget *parent, QString xml);
        VisuSignal* getSignal(quint16 signal_id);
        VisuInstrument* getInstrument(quint16 instrument_id);
        QVector<VisuInstrument*>& getInstruments();

        quint16 getPort();
        quint16 getWidth();
        quint16 getHeight();
        QColor getBackgroundColor();
        QString getName();

        static const QString TAG_INSTRUMENT;
        static const QString TAG_SIGNAL;
        static const QString TAG_CONFIGURATION;
        static const QString ATTR_TYPE;
        static const QString TAG_VISU_CONFIG;
        static const QString TAG_SIGNALS_PLACEHOLDER;
        static const QString TAG_INSTRUMENTS_PLACEHOLDER;

};

#endif // CONFIGURATION_H
