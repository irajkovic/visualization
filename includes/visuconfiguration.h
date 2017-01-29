#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "visusignal.h"
#include "visuinstrument.h"
#include "button.h"
#include "statics/staticimage.h"
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
        QVector<QPointer<StaticImage>> staticsList;

        void createSignalFromToken(QXmlStreamReader& xml_reader);
        void createConfigurationFromToken(QXmlStreamReader& xmlReader);
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
        void fromXML(QWidget *parent, const QString& xml);
        QString toXML();

        // Instruments
        QPointer<VisuInstrument> createInstrumentFromToken(QXmlStreamReader& xml_reader, QWidget *parent);
        void addInstrument(QPointer<VisuInstrument> instrument);
        void deleteInstrument(QPointer<VisuInstrument> instrument);
        QPointer<VisuInstrument> getInstrument(quint16 instrument_id);
        QVector<QPointer<VisuInstrument>>& getInstruments();
        void detachInstrumentFromSignal(QPointer<VisuInstrument> instrument);
        void attachInstrumentToSignal(QPointer<VisuInstrument> instrument);
        void detachInstrumentFromSignal(QPointer<VisuInstrument> instrument, int signalId);
        void attachInstrumentToSignal(QPointer<VisuInstrument> instrument, int signalId);

        // Signals
        void addSignal(QPointer<VisuSignal> signal);
        void deleteSignal(QPointer<VisuSignal> signal);
        void deleteSignal(int signalId);
        QPointer<VisuSignal> getSignal(quint16 signalId);
        QVector<QPointer<VisuSignal>> &getSignals();

        // Controls
        Button* createControlFromToken(QXmlStreamReader& xmlReader, QWidget *parent);
        void addControl(QPointer<Button> control);
        void deleteControl(QPointer<Button> control);
        void deleteImage(QPointer<StaticImage> image);
        QVector<QPointer<Button>>& getControls();

        // Statics
        StaticImage* createStaticFromToken(QXmlStreamReader& xmlReader, QWidget *parent);
        void addStatic(QPointer<StaticImage> image);
        void deleteStatic(int staticId);
        QVector<QPointer<StaticImage>>& getStatics();

        QMap<QString, QString>& getProperties();

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
