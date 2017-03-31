#ifndef INSTANALOG_H
#define INSTANALOG_H

#include "visuinstrument.h"

class InstAnalog : public VisuInstrument
{
    Q_OBJECT
    public:
        explicit InstAnalog(
                QWidget *parent,
                QMap<QString, QString> properties,
                QMap<QString, VisuPropertyMeta> metaProperties) : VisuInstrument(parent, properties, metaProperties)
        {
            loadProperties();
        }
        static const QString TAG_NAME;

        virtual bool updateProperties(const QString &key, const QString &value);
        void loadProperties();

    private:

        static constexpr double PI = 3.141592653589793238463;
        static constexpr double STEPS_IN_DEGREE = 16;

        // configuration properties
        QColor cColorCircle;
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
        quint16 cDivisionRadius;
        quint16 cCircleRadius;
        quint16 cArrowLen;
        double cLabelMultiplier;
        bool cRotateLabels;
        bool cCircleOffset;
        bool cCircleTrim;

        // aux propertis
        double mAngleSin;
        double mAngleCos;
        double mAngleLabel;
        double mDivisionAngle;
        double mDivisionAngleStep;
        double mSignalMajorDivisionValue;
        double mSignalMajorDivisionStep;
        double mStartLen;
        double mEndLen;
        double mStartPointX;
        double mStartPointY;
        double mEndPointX;
        double mEndPointY;
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
