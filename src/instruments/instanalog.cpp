#include "instanalog.h"

#include <QPainter>
#include <QFontMetrics>
#include <QtCore>
#include <math.h>

const QString InstAnalog::TAG_NAME = "ANALOG";

void InstAnalog::renderStatic(QPainter* painter)
{
    clear(painter);
    renderLabel(painter);
    if (1 == cDrawCircle) {
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
    setPen(painter, cColorStatic);
    setFont(painter, cFontSize);
    painter->drawText(cNameX, cNameY, mSignal->getName());
}

void InstAnalog::renderOutterCircle(QPainter* painter)
{
    painter->drawArc( cLineThickness + cOffsetX
                    , cLineThickness + cOffsetY
                    , cWidth - 2 * cLineThickness + cOffsetX
                    , cHeight - 2 * cLineThickness + cOffsetY
                    , 0
                    , 360 * 16);
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
    // Draw label
    QString label = QString("%1").arg((int)mSignalMajorDivisionValue);

    QFontMetrics font_metrics = painter->fontMetrics();
    quint16 labelWidth = font_metrics.width(label);
    quint16 labelHeight = font_metrics.height();
    quint16 labelX = -cLabelRadius * mAngleSin + (cWidth - labelWidth) / 2;
    quint16 labelY = cLabelRadius * mAngleCos + (cHeight + labelHeight) / 2;

    painter->drawText(labelX + cOffsetX, labelY + cOffsetY, label);
}

void InstAnalog::renderMajorDivision(QPainter* painter)
{
    renderDivisionLine(painter, 0.9);
}

void InstAnalog::renderMinorDivision(QPainter* painter)
{
    renderDivisionLine(painter, 1.0);
}

void InstAnalog::renderDivisionLine(QPainter* painter, double contractFactor)
{
    initDivisionProperties(contractFactor);
    painter->drawLine( mStartPointX + cOffsetX
                     , mStartPointY + cOffsetY
                     , mEndPointX + cOffsetX
                     , mEndPointY + cOffsetY);
}

void InstAnalog::initDivisionProperties(double factor)
{
    mStartLen = cWidth / 2 * factor - cLineThickness * 2;
    mEndLen = cWidth / 2 - cLineThickness * 2;
    mStartPointX = -mStartLen * 0.9 * mAngleSin + cWidth / 2;
    mStartPointY = mStartLen * 0.9 * mAngleCos + cHeight / 2;
    mEndPointX = -mEndLen * mAngleSin + cWidth / 2;
    mEndPointY = mEndLen * mAngleCos + cHeight / 2;
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
    mArrowLen = mCenterX - cLineThickness;
}

void InstAnalog::setBrush(QPainter* painter)
{
    painter->setBrush(cColorForeground);
}

void InstAnalog::renderDynamic(QPainter* painter)
{    
    setPen(painter, cColorForeground);
    setBrush(painter);
    calculateAngleOffset();
    setupProperties();
    drawTrianglePointer(painter);
}

void InstAnalog::drawTrianglePointer(QPainter* painter)
{
    quint16 endPointX = -mAngleSin * mArrowLen + mCenterX;
    quint16 endPointY = mAngleCos * mArrowLen + mCenterY;
    quint16 leftPointX = -mAngleCos * cArrowWidth + mCenterX;
    quint16 leftPointY = -mAngleSin * cArrowWidth + mCenterY;
    quint16 rightPointX = mAngleCos * cArrowWidth + mCenterX;
    quint16 rightPointY = mAngleSin * cArrowWidth + mCenterY;

    const QPointF points[3] = {
        QPointF(endPointX + cOffsetX, endPointY + cOffsetY),
        QPointF(leftPointX + cOffsetX, leftPointY + cOffsetY),
        QPointF(rightPointX + cOffsetX, rightPointY + cOffsetY)
    };

    painter->drawPolygon(points, 3);

}
