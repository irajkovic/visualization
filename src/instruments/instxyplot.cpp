#include "instxyplot.h"

const QString InstXYPlot::TAG_NAME = "XY_PLOT";

bool InstXYPlot::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    InstXYPlot::loadProperties();
    return VisuInstrument::refresh(key);
}

void InstXYPlot::loadProperties()
{
    VisuInstrument::loadProperties();

    GET_PROPERTY(cSignalIdY, mProperties, mPropertiesMeta);
    GET_PROPERTY(cBallSize, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMajorCntX, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMajorCntY, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMajorLenX, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMajorLenY, mProperties, mPropertiesMeta);
    GET_PROPERTY(cPadding, mProperties, mPropertiesMeta);
    GET_PROPERTY(cDecimals, mProperties, mPropertiesMeta);
    GET_PROPERTY(cReverseX, mProperties, mPropertiesMeta);
    GET_PROPERTY(cReverseY, mProperties, mPropertiesMeta);

    mTagName = InstXYPlot::TAG_NAME;
}

void InstXYPlot::renderSingleAxis(QPainter* painter, int sigInd, int divisions, int length)
{
    VisuSignal* sig = connectedSignals[sigInd];

    double pos = cPadding;
    double posDelta = (double)(length - 2*cPadding) / divisions;
    double lbl = sig->getMin();
    double lblDelta = (sig->getMax() - sig->getMin()) / divisions;

    QFontMetrics fm = painter->fontMetrics();

    if (cReverseX && sigInd == SIGNAL_FIRST)
    {
        pos = cWidth - cPadding;
        posDelta = -posDelta;
    }
    else if (cReverseY && sigInd == SIGNAL_SECOND)
    {
        pos = cHeight - cPadding;
        posDelta = -posDelta;
    }

    for (int i=0; i<divisions + 1; ++i)
    {
        QString lblStr = QString::number(lbl, 'f', cDecimals);

        if (sigInd == SIGNAL_FIRST)
        {
            int lblHalfWidth = fm.width(lblStr) / 2;
            painter->drawLine(pos, mCenterY-cMajorLenY, pos, mCenterY+cMajorLenY);
            painter->drawText(pos - lblHalfWidth, mCenterY-cMajorLenY-MARGIN, lblStr);
        }
        else
        {
            int lblHalfHeight = fm.height()/3;
            painter->drawLine(mCenterX-cMajorLenX, pos, mCenterX+cMajorLenX, pos);
            painter->drawText(mCenterX+cMajorLenX+MARGIN, pos + lblHalfHeight, lblStr);
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

    int x = mLastValX * (cWidth - 2 * cPadding) + cPadding - cBallSize / 2;
    int y = mLastValY * (cHeight - 2 * cPadding) + cPadding - cBallSize / 2;

    if (cReverseX)
    {
        x = cWidth - cBallSize - x;
    }

    if (cReverseY)
    {
        y = cHeight - cBallSize - y;
    }

    QRect rect(x, y, cBallSize, cBallSize);
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
