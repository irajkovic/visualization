#include "insttimeplot.h"
#include "visumisc.h"

const QString InstTimePlot::TAG_NAME = "TIME_PLOT";

bool InstTimePlot::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    InstTimePlot::loadProperties();
    return VisuInstrument::refresh(key);
}

void InstTimePlot::loadProperties()
{
    VisuInstrument::loadProperties();

    GET_PROPERTY(cLineThickness, quint8, mProperties, mPropertiesMeta);
    GET_PROPERTY(cStaticThickness, quint8, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMarkerThickness, quint8, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMajorCnt, quint8, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMinorCnt, quint8, mProperties, mPropertiesMeta);
    GET_PROPERTY(cTicksInSecond, quint64, mProperties, mPropertiesMeta);
    GET_PROPERTY(cTimespan, quint64, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMarkerDt, quint64, mProperties, mPropertiesMeta);
    GET_PROPERTY(cDecimals, quint64, mProperties, mPropertiesMeta);
    GET_PROPERTY(cDivisionFormat, QString, mProperties, mPropertiesMeta);
    GET_PROPERTY(cMasterTimeFormat, QString, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorGraphBackground, QColor, mProperties, mPropertiesMeta);

    mTagName = InstTimePlot::TAG_NAME;
}

int InstTimePlot::getFontHeight()
{
    QFont font;
    font.setPointSize(cFontSize);
    QFontMetrics fm(font);
    return fm.height();
}

void InstTimePlot::init(QPainter* painter)
{
    mMargin = getFontHeight();
    mSigStep = (mSignal->getMax() - mSignal->getMin()) / (cMajorCnt * cMinorCnt);

    mMaxLabelWidth = getLabelMaxWidth(painter);

    mPlotStartX = mMaxLabelWidth + 2 * PADDING;
    mPlotEndX = cWidth - mMargin;
    mPlotStartY = cHeight - mMargin;
    mPlotEndY = mMargin;
    mPlotRangeX = mPlotEndX - mPlotStartX;
    mPlotRangeY = mPlotStartY - mPlotEndY;

    mLastUpdateX = mPlotStartX;
    mLastUpdateY = mPlotStartY;
    mLastMarkerTime = 0;
}

quint16 InstTimePlot::getLabelMaxWidth(QPainter* painter)
{
    QFontMetrics fontMetrics = painter->fontMetrics();
    double sigTmpVal = mSignal->getMin();
    int labelWidth;
    QString label;
    int maxWidth = 0;
    int cnt = cMajorCnt * cMinorCnt;
    for (int i=0; i<=cnt; ++i) {
        label = getLabel(sigTmpVal);
        sigTmpVal += mSigStep;
        labelWidth = fontMetrics.width(label);
        maxWidth = maxWidth < labelWidth ? labelWidth : maxWidth;
    }
    return maxWidth;
}

QString InstTimePlot::getLabel(double value)
{
    return QString::number(value, 'f', cDecimals) + mSignal->getUnit();
}

void InstTimePlot::renderLabel(QPainter* painter, double sigCur, qint32 yPos)
{
    QString label = getLabel(sigCur);
    QFontMetrics fontMetrics = painter->fontMetrics();
    int labelWidth = fontMetrics.width(label);
    int labelHeight = fontMetrics.height();
    painter->drawText(mMaxLabelWidth - labelWidth + PADDING, yPos + labelHeight / 2, label);
}

void InstTimePlot::renderLabelsAndMajors(QPainter* painter)
{
    double sigCur = mSignal->getMin();
    int cnt = cMajorCnt * cMinorCnt;
    double yPos = mPlotStartY;
    double yStep = (double)(cHeight - 2 * mMargin) / cnt;

    QPen dashed = VisuMisc::getDashedPen(cColorStatic, cStaticThickness);

    for (int i=0; i<=cnt; ++i)
    {
        painter->setPen(dashed);

        if (i % cMinorCnt == 0)
        {
            renderLabel(painter, sigCur, yPos);
            setPen(painter, cColorStatic, cStaticThickness);
        }

        painter->drawLine(mPlotStartX, yPos, mPlotEndX, yPos);
        yPos -= yStep;
        sigCur += mSigStep;
    }
}

void InstTimePlot::setupGraphObjects()
{
    delete mGraphPainter;
    mGraphPixmap = QPixmap(cWidth, cHeight);
    mGraphPixmap.fill(Qt::transparent);
    setAttribute(Qt::WA_TranslucentBackground);
    mGraphPainter = new QPainter(&mGraphPixmap);
    mGraphPainter->setRenderHint(QPainter::Antialiasing);
}

void InstTimePlot::renderGraphAreaBackground(QPainter* painter)
{
    setBrush(painter, cColorGraphBackground);
    painter->drawRect(QRect(mPlotStartX, mPlotEndY, mPlotRangeX, mPlotRangeY));
}

void InstTimePlot::renderSignalName(QPainter* painter)
{
    painter->drawText(cWidth/2,
                      mPlotEndY-PADDING,
                      QString("%1").arg(mSignal->getName()));
}

void InstTimePlot::setupPainter(QPainter* painter)
{
    setPen(painter, cColorStatic, cStaticThickness);
    setFont(painter);
}

void InstTimePlot::renderStatic(QPainter* painter)
{
    clear(painter);
    setupPainter(painter);
    init(painter);

    renderGraphAreaBackground(painter);
    renderLabelsAndMajors(painter);
    renderSignalName(painter);

    setupGraphObjects();
}

QString InstTimePlot::getDisplayTime(int ticks, QString format)
{
    return QTime(0, 0, 0).addSecs(ticks / cTicksInSecond).toString(format);
}

double InstTimePlot::getMarkerX(quint64 timestamp)
{
    quint64 markerTime = timestamp - (timestamp % cMarkerDt) + cMarkerDt;   // round up
    double cor = ((double)markerTime - timestamp - cMarkerDt) * mPlotRangeX / cTimespan;
    return mNewUpdateX + cor;
}

void InstTimePlot::renderMarker(QPainter* painter, quint64 timestamp)
{
    double markerX = getMarkerX(timestamp);

    if (markerX > mPlotStartX && markerX < mPlotEndX)
    {
        setPen(mGraphPainter, cColorStatic, cMarkerThickness);
        painter->drawLine(markerX, mPlotStartY, markerX, mPlotEndY);

        setFont(mGraphPainter);
        painter->drawText(markerX,
                            cHeight,
                            getDisplayTime(timestamp, cDivisionFormat));
        mLastMarkerTime = timestamp;
    }
}

bool InstTimePlot::shouldRenderMarker(quint64 timestamp)
{
    return (timestamp >= mLastMarkerTime + cMarkerDt);
}

void InstTimePlot::renderTimeLabel(QPainter* painter)
{
    setPen(painter, cColorStatic);
    setFont(painter);
    quint64 timestamp = mSignal->getTimestamp();
    painter->drawText(mPlotStartX,
                      mPlotEndY - 5,
                      "Time " + getDisplayTime(timestamp, cMasterTimeFormat));
}

void InstTimePlot::renderGraphSegment(QPainter* painter)
{
    setPen(mGraphPainter, cColorForeground, cLineThickness);
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

void InstTimePlot::calculateNewGraphPoint(quint64 timestamp)
{
    double value = mSignal->getNormalizedValue();
    quint64 dt = timestamp > mLastUpdateTime ? (timestamp - mLastUpdateTime) : 0;
    double dx = (double)mPlotRangeX * dt / (cTimespan);

    mNewUpdateX = mLastUpdateX + dx;
    mNewUpdateY = mPlotStartY - mPlotRangeY * value;
}

void InstTimePlot::updateLastValues(quint64 timestamp)
{
    mLastUpdateX = mNewUpdateX;
    mLastUpdateY = mNewUpdateY;
    mLastUpdateTime = timestamp;
}

void InstTimePlot::renderDynamic(QPainter* painter)
{
    quint64 timestamp = mSignal->getTimestamp();
    calculateNewGraphPoint(timestamp);

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

    updateLastValues(timestamp);
}

