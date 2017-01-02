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
            GET_PROPERTY(cLineThickness, quint8, properties);
            GET_PROPERTY(cMajorLen, quint8, properties);
            GET_PROPERTY(cMinorLen, quint8, properties);
            GET_PROPERTY(cMajorCnt, quint8, properties);
            GET_PROPERTY(cMinorCnt, quint8, properties);
            GET_PROPERTY(cFontSize, quint8, properties);
            GET_PROPERTY(cTicksInSecond, quint64, properties);
            GET_PROPERTY(cTimespan, quint64, properties);
            GET_PROPERTY(cMarkerDt, quint64, properties);
            GET_PROPERTY(cDecimals, quint64, properties);
            GET_PROPERTY(cDivisionFormat, QString, properties);
            GET_PROPERTY(cMasterTimeFormat, QString, properties);

            // call default constructor after properties are set in case they are needed
            init();
        }
        static const QString TAG_NAME;

    private:

        // configuration properties
        quint8 cLineThickness;
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
        quint64 cMarkerDt;          // time between markers

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
        void setTimestampRect(int fontHeight);
        void setPen(QPainter* painter, QColor color);
        void setLabelMaxWidth(QPainter* painter);
        quint16 renderLabelsAndMajors(QPainter* painter);
        QString getLabel(double value);
        QString getDisplayTime(int ticks, QString format);
        void renderLabel(QPainter* painter, double sigCur, qint32 yPos);
        void renderMarker(QPainter* painter, quint64 timestamp);
        bool shouldRenderMarker(quint64 timestamp);
        void renderTimeLabel(QPainter* painter);
        void renderGraphSegment(QPainter* painter);
        void resetPlotToStart();
        bool noSpaceLeftOnRight();

    protected:
        virtual void init();
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap

};

#endif // INSTTIMEPLOT_H
