#ifndef INSTTIMEPLOT_H
#define INSTTIMEPLOT_H

#include "instrument.h"
#include <QTime>

class InstTimePlot : public VisuInstrument
{
    public:
        InstTimePlot();
        explicit InstTimePlot(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuInstrument(parent, properties)
        {
            GET_PROPERTY(line_thickness, quint8);
            GET_PROPERTY(major_len, quint8);
            GET_PROPERTY(minor_len, quint8);
            GET_PROPERTY(major_cnt, quint8);
            GET_PROPERTY(minor_cnt, quint8);
            GET_PROPERTY(font_size, quint8);
            GET_PROPERTY(ticks_in_second, quint64);
            GET_PROPERTY(timespan, quint64);
            GET_PROPERTY(marker_dt, quint64);
            GET_PROPERTY(decimals, quint64);
            GET_PROPERTY(division_format, QString);
            GET_PROPERTY(master_time_format, QString);

            // call default constructor after properties are set in case they are needed
            init();
        }
        static const QString TAG_NAME;

    private:

        // custom properties
        quint8 line_thickness;
        quint8 major_len;
        quint8 minor_len;
        quint8 major_cnt;
        quint8 minor_cnt;
        quint8 font_size;
        quint8 decimals;
        QString division_format;
        QString master_time_format;
        quint64 ticks_in_second;
        quint64 timespan;           // total time
        quint64 marker_dt;          // time between markers

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
