#include "instxyplot.h"

const QString InstXYPlot::TAG_NAME = "XY_PLOT";

bool InstXYPlot::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    InstXYPlot::loadProperties(mProperties);
    return VisuInstrument::refresh(key);
}

void InstXYPlot::renderSingleAxis(QPainter* painter, int sigInd, int divisions, int length)
{
    VisuSignal* sig = connectedSignals[sigInd];

    double pos = cPadding;
    double posDelta = (double)(length - 2*cPadding) / divisions;
    double lbl = sig->getMin();
    double lblDelta = (sig->getMax() - sig->getMin()) / divisions;

    QFontMetrics fm = painter->fontMetrics();

    for (int i=0; i<divisions + 1; ++i)
    {
        QString lblStr = QString::number(lbl, 'f', cDecimals);

        if (sigInd == SIGNAL_FIRST)
        {
            int lblHalfWidth = fm.width(lblStr) / 2;
            painter->drawLine(pos, mCenterY-cMajorLen, pos, mCenterY+cMajorLen);
            painter->drawText(pos - lblHalfWidth, mCenterY-cMajorLen-MARGIN, lblStr);
        }
        else
        {
            int lblHalfHeight = fm.height()/3;
            painter->drawLine(mCenterX-cMajorLen, pos, mCenterX+cMajorLen, pos);
            painter->drawText(mCenterX+cMajorLen+MARGIN, pos + lblHalfHeight, lblStr);
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

    painter->drawLine(cPadding, mCenterY, cWidth - cPadding, mCenterY);
    painter->drawLine(mCenterX, cPadding, mCenterX, cHeight - cPadding);

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
