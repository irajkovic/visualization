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
    if (1 == draw_circle) {
        renderOutterCircle(painter);
    }
    renderCircularFeatures(painter);
}

void InstAnalog::setPen(QPainter* painter, QColor color)
{
    QPen pen;
    pen.setWidth(line_thickness);
    pen.setColor(color);
    painter->setPen(pen);
}

void InstAnalog::renderLabel(QPainter* painter)
{
    setPen(painter, color_static);
    setFont(painter, font_size);
    painter->drawText(name_x, name_y, mSignal->getName());
}

void InstAnalog::renderOutterCircle(QPainter* painter)
{
    painter->drawArc( line_thickness + offset_x
                    , line_thickness + offset_y
                    , width - 2 * line_thickness + offset_x
                    , height - 2 * line_thickness + offset_y
                    , 0
                    , 360 * 16);
}

void InstAnalog::setupStaticRenderProperties(quint16 totalDivisions)
{
    mSignalMajorDivisionValue = mSignal->getMin();
    mDivisionAngle = angle_start;
    mDivisionAngleStep = (2 * PI - angle_start - angle_end) / totalDivisions;
    mSignalMajorDivisionStep = getMajorDivisionStep();
}

bool InstAnalog::isMajorDevision(int divisionCnt)
{
    return (divisionCnt % minor_cnt) == 0;
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
    quint16 totalDivisions = major_cnt * minor_cnt;
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
    return (sigMax - sigMin) / (major_cnt);
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
    quint16 labelX = -label_radius * mAngleSin + (width - labelWidth) / 2;
    quint16 labelY = label_radius * mAngleCos + (height + labelHeight) / 2;

    painter->drawText(labelX + offset_x, labelY + offset_y, label);
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
    painter->drawLine( mStartPointX + offset_x
                     , mStartPointY + offset_y
                     , mEndPointX + offset_x
                     , mEndPointY + offset_y);
}

void InstAnalog::initDivisionProperties(double factor)
{
    mStartLen = width / 2 * factor - line_thickness * 2;
    mEndLen = width / 2 - line_thickness * 2;
    mStartPointX = -mStartLen * 0.9 * mAngleSin + width / 2;
    mStartPointY = mStartLen * 0.9 * mAngleCos + height / 2;
    mEndPointX = -mEndLen * mAngleSin + width / 2;
    mEndPointY = mEndLen * mAngleCos + height / 2;
}

void InstAnalog::calculateAngleOffset()
{
    double value = mSignal->getNormalizedValue();
    double angleValue = (2 * PI - angle_start - angle_end) * value + angle_start;
    mAngleSin = qSin(angleValue);
    mAngleCos = qCos(angleValue);
}

void InstAnalog::setupProperties()
{
    mCenterX = width / 2;
    mCenterY = height / 2;
    mArrowLen = mCenterX - line_thickness;
}

void InstAnalog::setBrush(QPainter* painter)
{
    painter->setBrush(color_foreground);
}

void InstAnalog::renderDynamic(QPainter* painter)
{    
    setPen(painter, color_foreground);
    setBrush(painter);
    calculateAngleOffset();
    setupProperties();
    drawTrianglePointer(painter);
}

void InstAnalog::drawTrianglePointer(QPainter* painter)
{
    quint16 endPointX = -mAngleSin * mArrowLen + mCenterX;
    quint16 endPointY = mAngleCos * mArrowLen + mCenterY;
    quint16 leftPointX = -mAngleCos * arrow_width + mCenterX;
    quint16 leftPointY = -mAngleSin * arrow_width + mCenterY;
    quint16 rightPointX = mAngleCos * arrow_width + mCenterX;
    quint16 rightPointY = mAngleSin * arrow_width + mCenterY;

    const QPointF points[3] = {
        QPointF(endPointX + offset_x, endPointY + offset_y),
        QPointF(leftPointX + offset_x, leftPointY + offset_y),
        QPointF(rightPointX + offset_x, rightPointY + offset_y)
    };

    painter->drawPolygon(points, 3);

}
