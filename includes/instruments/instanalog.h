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
            loadProperties(properties);
        }
        static const QString TAG_NAME;

        virtual void loadProperties(QMap<QString, QString> properties)
        {
            VisuInstrument::loadProperties(properties);

            // custom properties initializer
            GET_PROPERTY(cLineThickness, quint8, properties);
            GET_PROPERTY(cMajorLen, quint8, properties);
            GET_PROPERTY(cMinorLen, quint8, properties);
            GET_PROPERTY(cMajorCnt, quint8, properties);
            GET_PROPERTY(cMinorCnt, quint8, properties);
            GET_PROPERTY(cArrowWidth, quint8, properties);
            GET_PROPERTY(cDrawCircle, bool, properties);
            GET_PROPERTY(cLabelRadius, quint16, properties);
            GET_PROPERTY(cAngleStart, double, properties);
            GET_PROPERTY(cAngleEnd, double, properties);
            GET_PROPERTY(cNameX, quint16, properties);
            GET_PROPERTY(cNameY, quint16, properties);
            GET_PROPERTY(cOffsetX, qint16, properties);
            GET_PROPERTY(cOffsetY, qint16, properties);
            GET_PROPERTY(cShowLabel, bool, properties);
            GET_PROPERTY(cDivisionRadius, qint16, properties);
            GET_PROPERTY(cArrowLen, quint16, properties);
            GET_PROPERTY(cLabelMultiplier, double, properties);

            mTagName = InstAnalog::TAG_NAME;
        }

    private:

        static constexpr double PI = 3.141592653589793238463;

        // configuration properties
        quint8 cLineThickness;  // Thickness of geometric primitives drawn
        quint8 cMajorLen;       // Length, in pixels, of major division marks
        quint8 cMinorLen;       // Length, in pixels, of minor division marks
        quint8 cMajorCnt;       // Number of major count divisions
        quint8 cMinorCnt;       // Number of minor count divisions
        quint8 cArrowWidth;     // Width of arrow at it's base
        bool cDrawCircle;     // 1 / 0 to draw circle allong pointer tip
        quint16 cLabelRadius;   // Radius of circle on which major labels are drawn
        double cAngleStart;     // Angle of minimum value
        double cAngleEnd;       // Angle of maximum value
        quint16 cNameX;         // Signal label X coordinate
        quint16 cNameY;         // Signal label Y coordinate
        qint16 cOffsetX;        // X offset of instrument center
        qint16 cOffsetY;        // Y offset of instrument center
        bool cShowLabel;
        qint16 cDivisionRadius;
        quint16 cArrowLen;
        double cLabelMultiplier;

        // aux propertis
        double mAngleSin;
        double mAngleCos;
        double mDivisionAngle;
        double mDivisionAngleStep;
        double mSignalMajorDivisionValue;
        double mSignalMajorDivisionStep;
        quint16 mStartLen;
        quint16 mEndLen;
        qint32 mStartPointX;
        qint32 mStartPointY;
        qint32 mEndPointX;
        qint32 mEndPointY;
        quint16 mCenterX;
        quint16 mCenterY;

        void renderOutterCircle(QPainter* painter);
        void renderMajorDivision(QPainter* painter);
        void renderMinorDivision(QPainter* painter);
        void renderDivisionLine(QPainter* painter, int length);
        void initDivisionProperties(int length);
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

    protected:
        virtual void renderStatic(QPainter *painter);   // Renders to pixmap_static
        virtual void renderDynamic(QPainter *painter);  // Renders to pixmap
};

#endif // INSTANALOG_H
