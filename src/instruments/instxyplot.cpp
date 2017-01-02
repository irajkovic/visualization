#include "instxyplot.h"

const QString InstXYPlot::TAG_NAME = "XY_PLOT";

void InstXYPlot::renderAxis(QPainter* painter)
{
    int dx = width / majorCnt;
    int dy = height / majorCnt;
    int x = 0;
    int y = 0;
    for (int i=0; i<majorCnt + 1; ++i)
    {
        painter->drawLine(x, mCenterY-majorLen, x, mCenterY+majorLen);
        painter->drawLine(mCenterX-majorLen, y, mCenterX+majorLen, y);
        x += dx;
        y += dy;
    }
}

void InstXYPlot::renderBall(QPainter* painter)
{
    setPen(painter, colorStatic);
    setBrush(painter, colorForeground);
    QRect rect(lastValX * (width-ballSize), lastValY * (height-ballSize), ballSize, ballSize);
    painter->drawEllipse(rect);
}

void InstXYPlot::renderStatic(QPainter *painter)
{
    painter->fillRect(0, 0, width, height, colorBackground);

    mCenterX = width / 2;
    mCenterY = height / 2;

    painter->drawLine(0, mCenterY, width, mCenterY);
    painter->drawLine(mCenterX, 0, mCenterX, height);

    renderAxis(painter);

    painter->drawText(0, mCenterY-majorLen, labelX);
    painter->drawText(mCenterX+majorLen, height, labelY);
}

void InstXYPlot::renderDynamic(QPainter *painter)
{
    if (mSignal->getId() == signalId)
    {
        lastValX = mSignal->getNormalizedValue();   // primary signal shown on X axis
    }
    else
    {
        lastValY = mSignal->getNormalizedValue();   // additional signal shown on Y axis
    }

    renderBall(painter);
}

quint16 InstXYPlot::getSignalYId()
{
    return signalYId;
}
