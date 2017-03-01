#include "instanalog.h"

#include <QPainter>
#include <QFontMetrics>
#include <QtCore>
#include <math.h>

const QString InstAnalog::TAG_NAME = "ANALOG";

bool InstAnalog::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    InstAnalog::loadProperties();
    return VisuInstrument::refresh(key);
}

void InstAnalog::loadProperties()
{
    VisuInstrument::loadProperties();

    // custom properties initializer
    GET_PROPERTY(cLineThickness, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMajorLen, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMinorLen, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMajorCnt, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMinorCnt, mProperties, mPropertiesMeta);
    GET_PROPERTY(cArrowWidth, mProperties, mPropertiesMeta);
    GET_PROPERTY(cDrawCircle, mProperties, mPropertiesMeta);
    GET_PROPERTY(cLabelRadius, mProperties, mPropertiesMeta);
    GET_PROPERTY(cAngleStart, mProperties, mPropertiesMeta);
    GET_PROPERTY(cAngleEnd, mProperties, mPropertiesMeta);
    GET_PROPERTY(cNameX, mProperties, mPropertiesMeta);
    GET_PROPERTY(cNameY, mProperties, mPropertiesMeta);
    GET_PROPERTY(cOffsetX, mProperties, mPropertiesMeta);
    GET_PROPERTY(cOffsetY, mProperties, mPropertiesMeta);
    GET_PROPERTY(cShowLabel, mProperties, mPropertiesMeta);
    GET_PROPERTY(cDivisionRadius, mProperties, mPropertiesMeta);
    GET_PROPERTY(cArrowLen, mProperties, mPropertiesMeta);
    GET_PROPERTY(cLabelMultiplier, mProperties, mPropertiesMeta);
    GET_PROPERTY(cRotateLabels, mProperties, mPropertiesMeta);
    GET_PROPERTY(cCircleOffset, mProperties, mPropertiesMeta);
    GET_PROPERTY(cCircleTrim, mProperties, mPropertiesMeta);

    mTagName = InstAnalog::TAG_NAME;
}

void InstAnalog::renderStatic(QPainter* painter)
{
    clear(painter);
    setPen(painter, cColorStatic);
    setFont(painter);
    setupProperties();

    if (cShowLabel)
    {
        renderLabel(painter);
    }

    if (cDrawCircle) {

        renderOutterCircle(painter);
    }
    renderCircularFeatures(painter);
}

void InstAnalog::setPen(QPainter* painter, QColor color)
{
    QPen pen;
    pen.setWidth(cLineThickness);
    pen.setColor(color);
    painter->setPen(pen);
}

void InstAnalog::renderLabel(QPainter* painter)
{
    painter->drawText(cNameX, cNameY, mSignal->getName());
}

void InstAnalog::renderOutterCircle(QPainter* painter)
{
    int x = cWidth / 2 - cDivisionRadius;
    int y = cHeight / 2 - cDivisionRadius;
    int angleStart = 0;
    int angleSpan = 360 * 16;

    if (cCircleOffset)
    {
        x += cOffsetX;
        y += cOffsetY;
    }

    if (cCircleTrim)
    {
        angleStart = -90 * 16 + (cAngleEnd / 3.14159 * 180 * 16);
        angleSpan = (360 - (cAngleStart + cAngleEnd) / 3.14159 * 180) * 16;
    }

    painter->drawArc( x
                    , y
                    , cDivisionRadius * 2
                    , cDivisionRadius * 2
                    , angleStart
                    , angleSpan);
}

void InstAnalog::setupStaticRenderProperties(quint16 totalDivisions)
{
    mSignalMajorDivisionValue = mSignal->getMin();
    mDivisionAngle = cAngleStart;
    mDivisionAngleStep = (2 * PI - cAngleStart - cAngleEnd) / totalDivisions;
    mSignalMajorDivisionStep = getMajorDivisionStep();
}

bool InstAnalog::isMajorDevision(int divisionCnt)
{
    return (divisionCnt % cMinorCnt) == 0;
}

void InstAnalog::renderMajor(QPainter* painter)
{
    renderMajorLabel(painter);
    renderMajorDivision(painter);
}

void InstAnalog::updateMajorValue()
{
    mSignalMajorDivisionValue += mSignalMajorDivisionStep;
}

void InstAnalog::renderCircularFeatures(QPainter* painter)
{
    quint16 totalDivisions = cMajorCnt * cMinorCnt;
    setupStaticRenderProperties(totalDivisions);

    for (int i = 0; i <= totalDivisions; ++i)
    {
        renderDivision(painter, i);
    }
}

void InstAnalog::renderDivision(QPainter* painter, int divisionCnt)
{
    updateDivisionAngles();
    if (isMajorDevision(divisionCnt))
    {
        renderMajor(painter);
        updateMajorValue();
    }
    else
    {
        renderMinorDivision(painter);
    }
}

double InstAnalog::getMajorDivisionStep()
{
    double sigMax = mSignal->getMax();
    double sigMin = mSignal->getMin();
    return (sigMax - sigMin) / (cMajorCnt);
}

void InstAnalog::updateDivisionAngles()
{
    mAngleSin = qSin(mDivisionAngle);
    mAngleCos = qCos(mDivisionAngle);
    mDivisionAngle += mDivisionAngleStep;
}

void InstAnalog::renderMajorLabel(QPainter* painter)
{
    QString label = QString("%1").arg((int)mSignalMajorDivisionValue * cLabelMultiplier);
    QFontMetrics font_metrics = painter->fontMetrics();
    quint16 labelWidth = font_metrics.width(label);

    if (cRotateLabels)
    {
        double lblHalfAngleWidth = qAsin((double)labelWidth/cLabelRadius/2);
        double lblPositionOffsetAngle = mDivisionAngle-mDivisionAngleStep-lblHalfAngleWidth;
        double lblAngleDeg = (mDivisionAngle-mDivisionAngleStep) * 360 / 3.14159 / 2 + 180 ;

        qint16 labelX = -cLabelRadius * qSin(lblPositionOffsetAngle) + cWidth / 2;
        qint16 labelY = cLabelRadius * qCos(lblPositionOffsetAngle) + cHeight / 2;

        painter->translate(labelX + cOffsetX, labelY + cOffsetY);
        painter->rotate(lblAngleDeg);
        painter->drawText(0, 0, label);
        painter->rotate(-lblAngleDeg);
        painter->translate(-labelX - cOffsetX, -labelY - cOffsetY);
    }
    else
    {
        quint16 labelHeight = font_metrics.height();
        qint16 labelX = -cLabelRadius * mAngleSin + (cWidth - labelWidth) / 2;
        qint16 labelY = cLabelRadius * mAngleCos + (cHeight + labelHeight) / 2;
        painter->drawText(labelX + cOffsetX, labelY + cOffsetY, label);
    }
}

void InstAnalog::renderMajorDivision(QPainter* painter)
{
    renderDivisionLine(painter, cMajorLen);
}

void InstAnalog::renderMinorDivision(QPainter* painter)
{
    renderDivisionLine(painter, cMinorLen);
}

void InstAnalog::renderDivisionLine(QPainter* painter, int length)
{
    initDivisionProperties(length);
    painter->drawLine( QPointF(mStartPointX + cOffsetX, mStartPointY + cOffsetY),
                       QPointF(mEndPointX + cOffsetX, mEndPointY + cOffsetY) );
}

void InstAnalog::initDivisionProperties(int length)
{
    mStartLen = cDivisionRadius - cLineThickness * 2;
    mEndLen = mStartLen - length;
    mStartPointX = -mStartLen * mAngleSin + mCenterX;
    mStartPointY = mStartLen * mAngleCos + mCenterY;
    mEndPointX = -mEndLen * mAngleSin + mCenterX;
    mEndPointY = mEndLen * mAngleCos + mCenterY;
}

void InstAnalog::calculateAngleOffset()
{
    double value = mSignal->getNormalizedValue();
    double angleValue = (2 * PI - cAngleStart - cAngleEnd) * value + cAngleStart;
    mAngleSin = qSin(angleValue);
    mAngleCos = qCos(angleValue);
}

void InstAnalog::setupProperties()
{
    mCenterX = cWidth / 2;
    mCenterY = cHeight / 2;
    //mArrowLen = mCenterX - cLineThickness;
}

void InstAnalog::renderDynamic(QPainter* painter)
{    
    setPen(painter, cColorForeground);
    setBrush(painter, cColorForeground);
    calculateAngleOffset();
    drawTrianglePointer(painter);
}

void InstAnalog::drawTrianglePointer(QPainter* painter)
{
    qint16 endPointX = -mAngleSin * cArrowLen + mCenterX;
    qint16 endPointY = mAngleCos * cArrowLen + mCenterY;
    qint16 leftPointX = -mAngleCos * cArrowWidth + mCenterX;
    qint16 leftPointY = -mAngleSin * cArrowWidth + mCenterY;
    qint16 rightPointX = mAngleCos * cArrowWidth + mCenterX;
    qint16 rightPointY = mAngleSin * cArrowWidth + mCenterY;

    const QPointF points[3] = {
        QPointF(endPointX + cOffsetX, endPointY + cOffsetY),
        QPointF(leftPointX + cOffsetX, leftPointY + cOffsetY),
        QPointF(rightPointX + cOffsetX, rightPointY + cOffsetY)
    };

    painter->drawPolygon(points, 3);

}
