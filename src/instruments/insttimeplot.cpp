#include "insttimeplot.h"

const QString InstTimePlot::TAG_NAME = "TIME_PLOT";

int InstTimePlot::getFontHeight()
{
    QFont font;
    font.setPointSize(cFontSize);
    QFontMetrics fm(font);
    return fm.height();
}

void InstTimePlot::setTimestampRect(int fontHeight)
{
    mTimestampRect.setX(cWidth - 100);
    mTimestampRect.setWidth(90);
    mTimestampRect.setY(cHeight - fontHeight);
    mTimestampRect.setHeight(fontHeight);
}

void InstTimePlot::init()
{
    int fontHeight = getFontHeight();
    setTimestampRect(fontHeight);
}

void InstTimePlot::setLabelMaxWidth(QPainter* painter)
{
    QFontMetrics fontMetrics = painter->fontMetrics();
    int sigTmpVal = mSignal->getMin();
    int labelWidth;
    QString label;
    mMaxLabelWidth = 0;
    for (int i=0; i<=cMajorCnt; ++i) {
        label = getLabel(sigTmpVal);
        sigTmpVal += mSigStep;
        labelWidth = fontMetrics.width(label);
        mMaxLabelWidth = mMaxLabelWidth < labelWidth ? labelWidth : mMaxLabelWidth;
    }
}

QString InstTimePlot::getLabel(double value)
{
    return QString::number(value, 'f', cDecimals);
}

void InstTimePlot::renderLabel(QPainter* painter, double sigCur, qint32 yPos)
{
    QString label = getLabel(sigCur);
    QFontMetrics fontMetrics = painter->fontMetrics();
    int labelWidth = fontMetrics.width(label);
    int labelHeight = fontMetrics.height();
    painter->drawText(mMaxLabelWidth - labelWidth, yPos + labelHeight / 2, label);
}

quint16 InstTimePlot::renderLabelsAndMajors(QPainter* painter)
{
    double sigCur = mSignal->getMin();
    qint32 yPos = mPlotStartY;
    quint16 yStep = (cHeight - 2 * mMargin) / cMajorCnt;

    setPen(painter, cColorStatic, 1);

    for (int i=0; i<=cMajorCnt; ++i)
    {
        renderLabel(painter, sigCur, yPos);
        painter->drawLine(mMaxLabelWidth + mMargin / 2, yPos, cWidth - mMargin, yPos);
        yPos -= yStep;
        sigCur += mSigStep;
    }

    return yPos + yStep;
}

void InstTimePlot::renderStatic(QPainter* painter)
{
    painter->fillRect(0, 0, cWidth, cHeight, cColorBackground);

    setPen(painter, cColorStatic);
    setFont(painter, cFontSize);

    // make sure we make enough space so bottom line does not hit the timestamp display
    mMargin = mTimestampRect.height() + 2;
    mSigStep = (mSignal->getMax() - mSignal->getMin()) / (cMajorCnt);

    setLabelMaxWidth(painter);

    mPlotStartX = mMargin + mMaxLabelWidth;
    mPlotStartY = cHeight - mMargin;
    mPlotEndX = cWidth - mMargin;
    mPlotEndY = renderLabelsAndMajors(painter);
    mPlotRangeX = mPlotEndX - mPlotStartX;
    mPlotRangeY = mPlotStartY - mPlotEndY;

    // render signal name
    painter->drawText(cWidth/2,
                      mPlotEndY-PADDING,
                      QString("%1 (%2)").arg(mSignal->getName()).arg(mSignal->getUnit()));

    // init values
    mLastUpdateX = mPlotStartX;
    mLastUpdateY = mPlotStartY;
    mLastMarkerTime = 0;

    mGraphPixmap = QPixmap(cWidth, cHeight);
    mGraphPixmap.fill(Qt::transparent);
    this->setAttribute(Qt::WA_TranslucentBackground);
    mGraphPainter = new QPainter(&mGraphPixmap);
    mGraphPainter->setRenderHint(QPainter::Antialiasing);
}

QString InstTimePlot::getDisplayTime(int ticks, QString format)
{
    return QTime(0, 0, 0).addSecs(ticks / cTicksInSecond).toString(format);
}

void InstTimePlot::renderMarker(QPainter* painter, quint64 timestamp)
{
    quint64 markerTime = timestamp - (timestamp % cMarkerDt) + cMarkerDt;   // round up
    double cor = ((double)markerTime - timestamp - cMarkerDt) * mPlotRangeX / cTimespan;
    double markerX = mNewUpdateX + cor;

    if (markerX > mPlotStartX && markerX < mPlotEndX)
    {
        setPen(mGraphPainter, cColorStatic);
        painter->drawLine(markerX, mPlotStartY, markerX, mPlotEndY);

        setFont(mGraphPainter, cFontSize);
        painter->drawText(markerX,
                                cHeight,
                                getDisplayTime(mLastMarkerTime, cDivisionFormat));
    }
    mLastMarkerTime = timestamp;
}

bool InstTimePlot::shouldRenderMarker(quint64 timestamp)
{
    return (timestamp >= mLastMarkerTime + cMarkerDt);
}

void InstTimePlot::renderTimeLabel(QPainter* painter)
{
    setPen(painter, cColorStatic);
    setFont(painter, cFontSize);
    quint64 timestamp = mSignal->getTimestamp();
    painter->drawText(mPlotStartX,
                      mPlotEndY - 5,
                      "Time " + getDisplayTime(timestamp, cMasterTimeFormat));
}

void InstTimePlot::renderGraphSegment(QPainter* painter)
{
    setPen(mGraphPainter, cColorForeground);
    mGraphPainter->drawLine(mLastUpdateX, mLastUpdateY, mNewUpdateX, mNewUpdateY);
    painter->drawPixmap(0, 0, mGraphPixmap);
}

void InstTimePlot::resetPlotToStart()
{
    mGraphPixmap.fill(Qt::transparent);
    mNewUpdateX = mPlotStartX + (mNewUpdateX - mLastUpdateX);
    mLastUpdateX = mPlotStartX;
}

bool InstTimePlot::noSpaceLeftOnRight()
{
    return (mNewUpdateX > mPlotEndX);
}

void InstTimePlot::renderDynamic(QPainter* painter)
{
    double value = mSignal->getNormalizedValue();
    quint64 timestamp = mSignal->getTimestamp();
    quint64 dt = timestamp > mLastUpdateTime ? (timestamp - mLastUpdateTime) : 0;
    double dx = mPlotRangeX * dt / (cTimespan);

    mLastUpdateTime = timestamp;
    mNewUpdateX = mLastUpdateX + dx;
    mNewUpdateY = mPlotStartY - mPlotRangeY * value;

    if (noSpaceLeftOnRight())
    {
        resetPlotToStart();
    }

    if (shouldRenderMarker(timestamp))
    {
        renderMarker(mGraphPainter, timestamp);
    }

    renderTimeLabel(painter);
    renderGraphSegment(painter);

    mLastUpdateX = mNewUpdateX;
    mLastUpdateY = mNewUpdateY;
}

