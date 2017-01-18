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
        QVector<QPointer<VisuInstrument>> instrumentsList;
        QVector<QPointer<Button>> controlsList;
        void createSignalFromToken(QXmlStreamReader& xml_reader);
        void createConfigurationFromToken(QXmlStreamReader& xmlReader);
        void createStaticFromToken(QXmlStreamReader& xmlReader, QWidget *parent);
        void initializeInstruments();
        int getFreeId(QVector<QPointer<QObject> > &list);

        // Properties
        quint16 cPort;
        quint16 cWidth;
        quint16 cHeight;
        QColor cColorBackground;
        QString cName;

        QMap<QString, QString> mProperties;

    public:
        virtual ~VisuConfiguration();
        void setConfigValues(const QMap<QString, QString>& properties);

        void loadFromXML(QWidget *parent, const QString& xml);
        QPointer<VisuInstrument> createInstrumentFromToken(QXmlStreamReader& xml_reader, QWidget *parent);
        Button* createControlFromToken(QXmlStreamReader& xmlReader, QWidget *parent);

        QPointer<VisuSignal> getSignal(quint16 signalId);
        QVector<QPointer<VisuSignal>> &getSignals();
        QPointer<VisuInstrument> getInstrument(quint16 instrument_id);
        QVector<QPointer<VisuInstrument>>& getInstruments();

        void detachInstrumentFromSignal(QPointer<VisuInstrument> instrument);
        void attachInstrumentToSignal(QPointer<VisuInstrument> instrument);
        void detachInstrumentFromSignal(QPointer<VisuInstrument> instrument, int signalId);
        void attachInstrumentToSignal(QPointer<VisuInstrument> instrument, int signalId);

        void addInstrument(QPointer<VisuInstrument> instrument);
        void deleteInstrument(QPointer<VisuInstrument> instrument);

        void addSignal(QPointer<VisuSignal> signal);
        void deleteSignal(QPointer<VisuSignal> signal);
        void deleteSignal(int signalId);

        void addControl(QPointer<Button> control);
        void deleteControl(QPointer<Button> control);

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
