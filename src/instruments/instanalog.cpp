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
    if (1 == drawCircle) {
        renderOutterCircle(painter);
    }
    renderCircularFeatures(painter);
}

void InstAnalog::setPen(QPainter* painter, QColor color)
{
    QPen pen;
    pen.setWidth(lineThickness);
    pen.setColor(color);
    painter->setPen(pen);
}

void InstAnalog::renderLabel(QPainter* painter)
{
    setPen(painter, colorStatic);
    setFont(painter, fontSize);
    painter->drawText(nameX, nameY, mSignal->getName());
}

void InstAnalog::renderOutterCircle(QPainter* painter)
{
    painter->drawArc( lineThickness + offsetX
                    , lineThickness + offsetY
                    , width - 2 * lineThickness + offsetX
                    , height - 2 * lineThickness + offsetY
                    , 0
                    , 360 * 16);
}

void InstAnalog::setupStaticRenderProperties(quint16 totalDivisions)
{
    mSignalMajorDivisionValue = mSignal->getMin();
    mDivisionAngle = angleStart;
    mDivisionAngleStep = (2 * PI - angleStart - angleEnd) / totalDivisions;
    mSignalMajorDivisionStep = getMajorDivisionStep();
}

bool InstAnalog::isMajorDevision(int divisionCnt)
{
    return (divisionCnt % minorCnt) == 0;
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
    quint16 totalDivisions = majorCnt * minorCnt;
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
    return (sigMax - sigMin) / (majorCnt);
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
    quint16 labelX = -labelRadius * mAngleSin + (width - labelWidth) / 2;
    quint16 labelY = labelRadius * mAngleCos + (height + labelHeight) / 2;

    painter->drawText(labelX + offsetX, labelY + offsetY, label);
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
    painter->drawLine( mStartPointX + offsetX
                     , mStartPointY + offsetY
                     , mEndPointX + offsetX
                     , mEndPointY + offsetY);
}

void InstAnalog::initDivisionProperties(double factor)
{
    mStartLen = width / 2 * factor - lineThickness * 2;
    mEndLen = width / 2 - lineThickness * 2;
    mStartPointX = -mStartLen * 0.9 * mAngleSin + width / 2;
    mStartPointY = mStartLen * 0.9 * mAngleCos + height / 2;
    mEndPointX = -mEndLen * mAngleSin + width / 2;
    mEndPointY = mEndLen * mAngleCos + height / 2;
}

void InstAnalog::calculateAngleOffset()
{
    double value = mSignal->getNormalizedValue();
    double angleValue = (2 * PI - angleStart - angleEnd) * value + angleStart;
    mAngleSin = qSin(angleValue);
    mAngleCos = qCos(angleValue);
}

void InstAnalog::setupProperties()
{
    mCenterX = width / 2;
    mCenterY = height / 2;
    mArrowLen = mCenterX - lineThickness;
}

void InstAnalog::setBrush(QPainter* painter)
{
    painter->setBrush(colorForeground);
}

void InstAnalog::renderDynamic(QPainter* painter)
{    
    setPen(painter, colorForeground);
    setBrush(painter);
    calculateAngleOffset();
    setupProperties();
    drawTrianglePointer(painter);
}

void InstAnalog::drawTrianglePointer(QPainter* painter)
{
    quint16 endPointX = -mAngleSin * mArrowLen + mCenterX;
    quint16 endPointY = mAngleCos * mArrowLen + mCenterY;
    quint16 leftPointX = -mAngleCos * arrowWidth + mCenterX;
    quint16 leftPointY = -mAngleSin * arrowWidth + mCenterY;
    quint16 rightPointX = mAngleCos * arrowWidth + mCenterX;
    quint16 rightPointY = mAngleSin * arrowWidth + mCenterY;

    const QPointF points[3] = {
        QPointF(endPointX + offsetX, endPointY + offsetY),
        QPointF(leftPointX + offsetX, leftPointY + offsetY),
        QPointF(rightPointX + offsetX, rightPointY + offsetY)
    };

    painter->drawPolygon(points, 3);

}
