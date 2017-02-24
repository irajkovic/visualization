#include "instanalog.h"

#include <QPainter>
#include <QFontMetrics>
#include <QtCore>
#include <math.h>

const QString InstAnalog::TAG_NAME = "ANALOG";

void InstAnalog::reloadProperties(QMap<QString, QString>& properties)
{
    InstAnalog::loadProperties(properties);
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
    painter->drawArc( cLineThickness
                    , cLineThickness
                    , cWidth - 2 * cLineThickness
                    , cHeight - 2 * cLineThickness
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
    QString label = QString("%1").arg((int)mSignalMajorDivisionValue * cLabelMultiplier);
    QFontMetrics font_metrics = painter->fontMetrics();
    quint16 labelWidth = font_metrics.width(label);

    if (cRotateLabels)
    {
        double lblHalfAngleWidth = qAsin((double)labelWidth/cLabelRadius/2);
        double lblPositionOffsetAngle = mDivisionAngle-mDivisionAngleStep-lblHalfAngleWidth;
        double lblAngleDeg = (mDivisionAngle-mDivisionAngleStep) * 360 / 3.14159 / 2 + 180 ;

        quint16 labelX = -cLabelRadius * qSin(lblPositionOffsetAngle) + cWidth / 2;
        quint16 labelY = cLabelRadius * qCos(lblPositionOffsetAngle) + cHeight / 2;

        painter->translate(labelX + cOffsetX, labelY + cOffsetY);
        painter->rotate(lblAngleDeg);
        painter->drawText(0, 0, label);
        painter->rotate(-lblAngleDeg);
        painter->translate(-labelX - cOffsetX, -labelY - cOffsetY);
    }
    else
    {
        quint16 labelHeight = font_metrics.height();
        quint16 labelX = -cLabelRadius * mAngleSin + (cWidth - labelWidth) / 2;
        quint16 labelY = cLabelRadius * mAngleCos + (cHeight + labelHeight) / 2;
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
