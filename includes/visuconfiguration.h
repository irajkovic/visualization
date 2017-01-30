#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "visusignal.h"
#include "visuinstrument.h"
#include "ctrlbutton.h"
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
        QVector<QPointer<VisuWidget>> widgetsList;

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

        // General widget methods
        QPointer<VisuWidget> createWidgetFromToken(QXmlStreamReader& xmlReader, QWidget *parent, QString tag);
        void addWidget(QPointer<VisuWidget> widget);
        void deleteWidget(QPointer<VisuWidget> widget);
        QVector<QPointer<VisuWidget>> getWidgets();

        // Instrument specific methods
        QPointer<VisuInstrument> getInstrument(quint16 instrument_id);
        void detachInstrumentFromSignal(QPointer<VisuInstrument> instrument);
        void attachInstrumentToSignal(QPointer<VisuInstrument> instrument);
        void detachInstrumentFromSignal(QPointer<VisuInstrument> instrument, int signalId);
        void attachInstrumentToSignal(QPointer<VisuInstrument> instrument, int signalId);

        // Signal specific methods
        void addSignal(QPointer<VisuSignal> signal);
        void deleteSignal(QPointer<VisuSignal> signal);
        void deleteSignal(int signalId);
        QPointer<VisuSignal> getSignal(quint16 signalId);
        QVector<QPointer<VisuSignal>> &getSignals();

        // Getters
        QMap<QString, QString>& getProperties();
        quint16 getPort();
        quint16 getWidth();
        quint16 getHeight();
        QColor getBackgroundColor();
        QString getName();

        static const QString TAG_WIDGET;
        static const QString TAG_SIGNAL;
        static const QString TAG_CONFIGURATION;
        static const QString ATTR_TYPE;
        static const QString TAG_VISU_CONFIG;
        static const QString TAG_WIDGETS_PLACEHOLDER;
        static const QString TAG_SIGNALS_PLACEHOLDER;
};

#endif // CONFIGURATION_H
