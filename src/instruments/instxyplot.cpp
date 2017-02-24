#include "instxyplot.h"

const QString InstXYPlot::TAG_NAME = "XY_PLOT";

void InstXYPlot::reloadProperties(QMap<QString, QString>& properties)
{
    InstXYPlot::loadProperties(properties);
}

void InstXYPlot::renderSingleAxis(QPainter* painter, int sigInd, int divisions, int length)
{
    VisuSignal* sig = connectedSignals[sigInd];

    double pos = 0.0;
    double posDelta = (double)length / divisions;
    double lbl = sig->getMin();
    double lblDelta = (sig->getMax() - sig->getMin()) / divisions;

    for (int i=0; i<divisions + 1; ++i)
    {
        if (sigInd == SIGNAL_FIRST)
        {
            painter->drawLine(pos, mCenterY-cMajorLen, pos, mCenterY+cMajorLen);
            painter->drawText(pos, mCenterY-cMajorLen, QString("%1").arg(lbl));
        }
        else
        {
            painter->drawLine(mCenterX-cMajorLen, pos, mCenterX+cMajorLen, pos);
            painter->drawText(mCenterX+cMajorLen, pos, QString("%1").arg(lbl));
        }

        pos += posDelta;
        lbl += lblDelta;
    }
}

void InstXYPlot::renderAxis(QPainter* painter)
{
    renderSingleAxis(painter, SIGNAL_FIRST, cMajorCntX, cWidth);
    renderSingleAxis(painter, SIGNAL_SECOND, cMajorCntY, cHeight);
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
    setFont(painter);
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

    renderBall(painter);
}
