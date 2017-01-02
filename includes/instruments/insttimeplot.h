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
            GET_PROPERTY(lineThickness, quint8, properties);
            GET_PROPERTY(majorLen, quint8, properties);
            GET_PROPERTY(minorLen, quint8, properties);
            GET_PROPERTY(majorCnt, quint8, properties);
            GET_PROPERTY(minorCnt, quint8, properties);
            GET_PROPERTY(fontSize, quint8, properties);
            GET_PROPERTY(ticksInSecond, quint64, properties);
            GET_PROPERTY(timespan, quint64, properties);
            GET_PROPERTY(markerDt, quint64, properties);
            GET_PROPERTY(decimals, quint64, properties);
            GET_PROPERTY(divisionFormat, QString, properties);
            GET_PROPERTY(masterTimeFormat, QString, properties);

            // call default constructor after properties are set in case they are needed
            init();
        }
        static const QString TAG_NAME;

    private:

        // custom properties
        quint8 lineThickness;
        quint8 majorLen;
        quint8 minorLen;
        quint8 majorCnt;
        quint8 minorCnt;
        quint8 fontSize;
        quint8 decimals;
        QString divisionFormat;
        QString masterTimeFormat;
        quint64 ticksInSecond;
        quint64 timespan;           // total time
        quint64 markerDt;          // time between markers

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
