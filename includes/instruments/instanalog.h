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
            GET_PROPERTY(lineThickness, quint8, properties);
            GET_PROPERTY(majorLen, quint8, properties);
            GET_PROPERTY(minorLen, quint8, properties);
            GET_PROPERTY(majorCnt, quint8, properties);
            GET_PROPERTY(minorCnt, quint8, properties);
            GET_PROPERTY(fontSize, quint8, properties);
            GET_PROPERTY(arrowWidth, quint8, properties);
            GET_PROPERTY(drawCircle, quint8, properties);
            GET_PROPERTY(labelRadius, quint16, properties);
            GET_PROPERTY(angleStart, double, properties);
            GET_PROPERTY(angleEnd, double, properties);
            GET_PROPERTY(nameX, quint16, properties);
            GET_PROPERTY(nameY, quint16, properties);
            GET_PROPERTY(offsetX, qint16, properties);
            GET_PROPERTY(offsetY, qint16, properties);
        }
        static const QString TAG_NAME;

    private:

        static const double PI = 3.141592653589793238463;

        // custom properties
        quint8 lineThickness;  // Thickness of geometric primitives drawn
        quint8 majorLen;       // Length, in pixels, of major division marks
        quint8 minorLen;       // Length, in pixels, of minor division marks
        quint8 majorCnt;       // Number of major count divisions
        quint8 minorCnt;       // Number of minor count divisions
        quint8 fontSize;       // Size of font used on labels
        quint8 arrowWidth;     // Width of arrow at it's base
        quint8 drawCircle;     // 1 / 0 to draw circle allong pointer tip
        quint16 labelRadius;   // Radius of circle on which major labels are drawn
        double angleStart;     // Angle of minimum value
        double angleEnd;       // Angle of maximum value
        quint16 nameX;         // Signal label X coordinate
        quint16 nameY;         // Signal label Y coordinate
        qint16 offsetX;        // X offset of instrument center
        qint16 offsetY;        // Y offset of instrument center

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
