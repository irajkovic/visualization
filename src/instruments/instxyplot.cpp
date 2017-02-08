#include "instxyplot.h"

const QString InstXYPlot::TAG_NAME = "XY_PLOT";

void InstXYPlot::renderAxis(QPainter* painter)
{
    double dx = (double)cWidth / cMajorCntX;
    double dy = (double)cHeight / cMajorCntY;
    double x = 0;
    double y = 0;

    double lblX = 0.0;
    double lblDx = 0.0;
    double lblY = 0.0;
    double lblDy = 0.0;

    mSignalX = connectedSignals[0];
    lblX = mSignalX->getMin();
    lblDx = (mSignalX->getMax() - mSignalX->getMin()) / cMajorCntX;
    for (int i=0; i<cMajorCntX + 1; ++i)
    {
        painter->drawLine(x, mCenterY-cMajorLen, x, mCenterY+cMajorLen);
        if (lblDx != 0.0)
        {
            painter->drawText(x, mCenterY-cMajorLen, QString("%1").arg(lblX));
        }
        x += dx;
        lblX += lblDx;
    }

    mSignalY = connectedSignals[1];
    lblY = mSignalY->getMin();
    lblDy = (mSignalY->getMax() - mSignalY->getMin()) / cMajorCntY;
    for (int i=0; i<cMajorCntY + 1; ++i)
    {
        painter->drawLine(mCenterX-cMajorLen, y, mCenterX+cMajorLen, y);
        if (lblDy != 0.0)
        {
            painter->drawText(mCenterX+cMajorLen, y, QString("%1").arg(lblY));
        }
        y += dy;
        lblY += lblDy;
    }
}

void InstXYPlot::renderBall(QPainter* painter)
{
    setPen(painter, cColorStatic);
    setBrush(painter, cColorForeground);
    QRect rect(mLastValX * (cWidth-cBallSize), mLastValY * (cHeight-cBallSize), cBallSize, cBallSize);
    painter->drawEllipse(rect);
}

void InstXYPlot::renderStatic(QPainter *painter)
{
    setFont(painter, cFontSize);
    setPen(painter, cColorStatic);
    clear(painter);

    mCenterX = cWidth / 2;
    mCenterY = cHeight / 2;

    painter->drawLine(0, mCenterY, cWidth, mCenterY);
    painter->drawLine(mCenterX, 0, mCenterX, cHeight);

    renderAxis(painter);
}

void InstXYPlot::renderDynamic(QPainter *painter)
{
    if (mSignal->getId() == cSignalId)
    {
        mLastValX = mSignal->getNormalizedValue();   // primary signal shown on X axis
        mSignalX = mSignal;
    }
    else
    {
        mLastValY = mSignal->getNormalizedValue();   // additional signal shown on Y axis
        mSignalY = mSignal;
    }
    qDebug("Sigid: %d,%d %d ", mSignal->getId(), cSignalId, cSignalIdY);

    renderBall(painter);
}
