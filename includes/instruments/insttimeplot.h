#ifndef INSTTIMEPLOT_H
#define INSTTIMEPLOT_H

#include "visuinstrument.h"
#include <QTime>

class InstTimePlot : public VisuInstrument
{
    Q_OBJECT
    public:
        explicit InstTimePlot(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuInstrument(parent, properties)
        {
            loadProperties(properties);
            mGraphPainter = nullptr;
        }
        static const QString TAG_NAME;

        virtual void loadProperties(QMap<QString, QString> properties)
        {
            VisuInstrument::loadProperties(properties);

            GET_PROPERTY(cLineThickness, quint8, properties);
            GET_PROPERTY(cStaticThickness, quint8, properties);
            GET_PROPERTY(cMarkerThickness, quint8, properties);
            GET_PROPERTY(cMajorCnt, quint8, properties);
            GET_PROPERTY(cMinorCnt, quint8, properties);
            GET_PROPERTY(cFontSize, quint8, properties);
            GET_PROPERTY(cTicksInSecond, quint64, properties);
            GET_PROPERTY(cTimespan, quint64, properties);
            GET_PROPERTY(cMarkerDt, quint64, properties);
            GET_PROPERTY(cDecimals, quint64, properties);
            GET_PROPERTY(cDivisionFormat, QString, properties);
            GET_PROPERTY(cMasterTimeFormat, QString, properties);
            GET_PROPERTY(cColorGraphBackground, QColor, properties);

            mTagName = InstTimePlot::TAG_NAME;
        }

    private:

        // configuration properties
        quint8 cLineThickness;
        quint8 cStaticThickness;
        quint8 cMarkerThickness;
        quint8 cMajorLen;
        quint8 cMinorLen;
        quint8 cMajorCnt;
        quint8 cMinorCnt;
        quint8 cFontSize;
        quint8 cDecimals;
        QString cDivisionFormat;
        QString cMasterTimeFormat;
        quint64 cTicksInSecond;
        quint64 cTimespan;           // total time
        quint64 cMarkerDt;           // time between markers
        QColor cColorGraphBackground;

        // other properties
        quint16 mPlotStartX;
        quint16 mPlotStartY;
        quint16 mPlotEndX;
        quint16 mPlotEndY;
        quint16 mPlotRangeX;
        quint16 mPlotRangeY;
        quint64 mLastUpdateTime;
        quint64 mLastMarkerTime;
        double  mLastUpdateX;
        double  mLastUpdateY;
        double  mNewUpdateX;
        double  mNewUpdateY;
        QRect   mTimestampRect;
        QPixmap mGraphPixmap;              // pixmap to contain graph
        QPainter* mGraphPainter;
        quint16 mMargin;
        quint16 mMaxLabelWidth;
        double mSigStep;

        static const int PADDING = 5;   //px

        int getFontHeight();
        quint16 getLabelMaxWidth(QPainter* painter);
        void renderLabelsAndMajors(QPainter* painter);
        QString getLabel(double value);
        QString getDisplayTime(int ticks, QString format);
        void renderLabel(QPainter* painter, double sigCur, qint32 yPos);
        void renderMarker(QPainter* painter, quint64 timestamp);
        bool shouldRenderMarker(quint64 timestamp);
        void renderTimeLabel(QPainter* painter);
        void renderGraphSegment(QPainter* painter);
        void resetPlotToStart();
        bool noSpaceLeftOnRight();
        QPen getDashedPen();
        void init(QPainter* painter);
        void setupGraphObjects();
        void renderGraphAreaBackground(QPainter* painter);
        void renderSignalName(QPainter* painter);
        void setupPainter(QPainter* painter);
        double getMarkerX(quint64 timestamp);
        void calculateNewGraphPoint(quint64 timestamp);
        void updateLastValues(quint64 timestamp);

    protected:

        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap

};

#endif // INSTTIMEPLOT_H
