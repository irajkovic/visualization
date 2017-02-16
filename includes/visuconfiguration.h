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

        template <typename T>
        static void append(T* elem, QString& xml, int tabs);
        void createSignalFromToken(QXmlStreamReader& xml_reader);
        void createConfigurationFromToken(QXmlStreamReader& xmlReader);
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
        void initializeInstruments();

        // General widget methods
        QPointer<VisuWidget> createWidgetFromToken(QXmlStreamReader& xmlReader, QWidget *parent);
        void addWidget(QPointer<VisuWidget> widget);
        void deleteWidget(QPointer<VisuWidget> widget);
        QVector<QPointer<VisuWidget>> getWidgets();
        QPointer<VisuWidget> getWidget(int id);
        void moveWidgetUp(int id);
        void moveWidgetDown(int id);

        // Instrument specific methods
        QPointer<VisuInstrument> getInstrument(quint16 instrument_id);

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
        QSize getSize() const;
        QColor getBackgroundColor();
        QString getName();

        static const QString TAG_WIDGET;
        static const QString TAG_SIGNAL;
        static const QString TAG_NAME;
        static const QString ATTR_TYPE;
        static const QString TAG_VISU_CONFIG;
        static const QString TAG_WIDGETS_PLACEHOLDER;
        static const QString TAG_SIGNALS_PLACEHOLDER;
};

#endif // CONFIGURATION_H
