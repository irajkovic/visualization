#ifndef INSTANALOG_H
#define INSTANALOG_H

#include "visuinstrument.h"

class InstAnalog : public VisuInstrument
{
    Q_OBJECT
    public:
        explicit InstAnalog(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuInstrument(parent, properties)
        {

            // custom properties initializer
            GET_PROPERTY(line_thickness, quint8, properties);
            GET_PROPERTY(major_len, quint8, properties);
            GET_PROPERTY(minor_len, quint8, properties);
            GET_PROPERTY(major_cnt, quint8, properties);
            GET_PROPERTY(minor_cnt, quint8, properties);
            GET_PROPERTY(font_size, quint8, properties);
            GET_PROPERTY(arrow_width, quint8, properties);
            GET_PROPERTY(draw_circle, quint8, properties);
            GET_PROPERTY(label_radius, quint16, properties);
            GET_PROPERTY(angle_start, double, properties);
            GET_PROPERTY(angle_end, double, properties);
            GET_PROPERTY(name_x, quint16, properties);
            GET_PROPERTY(name_y, quint16, properties);
            GET_PROPERTY(offset_x, qint16, properties);
            GET_PROPERTY(offset_y, qint16, properties);
        }
        static const QString TAG_NAME;

    private:

        static const double PI = 3.141592653589793238463;

        // custom properties
        quint8 line_thickness;  // Thickness of geometric primitives drawn
        quint8 major_len;       // Length, in pixels, of major division marks
        quint8 minor_len;       // Length, in pixels, of minor division marks
        quint8 major_cnt;       // Number of major count divisions
        quint8 minor_cnt;       // Number of minor count divisions
        quint8 font_size;       // Size of font used on labels
        quint8 arrow_width;     // Width of arrow at it's base
        quint8 draw_circle;     // 1 / 0 to draw circle allong pointer tip
        quint16 label_radius;   // Radius of circle on which major labels are drawn
        double angle_start;     // Angle of minimum value
        double angle_end;       // Angle of maximum value
        quint16 name_x;         // Signal label X coordinate
        quint16 name_y;         // Signal label Y coordinate
        qint16 offset_x;        // X offset of instrument center
        qint16 offset_y;        // Y offset of instrument center

        // aux propertis
        double mAngleSin;
        double mAngleCos;
        double mDivisionAngle;
        double mDivisionAngleStep;
        double mSignalMajorDivisionValue;
        double mSignalMajorDivisionStep;
        quint16 mStartLen;
        quint16 mEndLen;
        quint16 mStartPointX;
        quint16 mStartPointY;
        quint16 mEndPointX;
        quint16 mEndPointY;
        quint16 mCenterX;
        quint16 mCenterY;
        quint16 mArrowLen;

        void renderOutterCircle(QPainter* painter);
        void renderMajorDivision(QPainter* painter);
        void renderMinorDivision(QPainter* painter);
        void renderDivisionLine(QPainter* painter, double lengthFactor);
        void initDivisionProperties(double factor = 1.0);
        void renderMajorLabel(QPainter* painter);
        double getMajorDivisionStep();
        void updateDivisionAngles();
        void setPen(QPainter* painter, QColor color);
        void renderCircularFeatures(QPainter* painter);
        void renderLabel(QPainter* painter);
        void setupStaticRenderProperties(quint16 totalDivisions);
        bool isMajorDevision(int divisionCnt);
        void renderMajor(QPainter* painter);
        void updateMajorValue();
        void renderDivision(QPainter* painter, int divisionCnt);
        void setupProperties();
        void calculateAngleOffset();
        void drawTrianglePointer(QPainter* painter);
        void setBrush(QPainter* painter);


    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTANALOG_H
