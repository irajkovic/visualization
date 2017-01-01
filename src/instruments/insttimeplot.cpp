#include "insttimeplot.h"

const QString InstTimePlot::TAG_NAME = "TIME_PLOT";

int InstTimePlot::getFontHeight()
{
    QFont font;
    font.setPointSize(font_size);
    QFontMetrics fm(font);
    return fm.height();
}

void InstTimePlot::setTimestampRect(int fontHeight)
{
    mTimestampRect.setX(width - 100);
    mTimestampRect.setWidth(90);
    mTimestampRect.setY(height - fontHeight);
    mTimestampRect.setHeight(fontHeight);
}

void InstTimePlot::init()
{
    int fontHeight = getFontHeight();
    setTimestampRect(fontHeight);
}

void InstTimePlot::setPen(QPainter* painter, QColor color)
{
    QPen pen;
    pen.setWidth(line_thickness);
    pen.setColor(color);
    painter->setPen(pen);
}

void InstTimePlot::setLabelMaxWidth(QPainter* painter)
{
    QFontMetrics font_metrics = painter->fontMetrics();
    int sig_cur = mSignal->getMin();
    int label_width;
    QString label;
    mMaxLabelWidth = 0;
    for (int i=0; i<=major_cnt; ++i) {
        label = getLabel(sig_cur);
        sig_cur += mSigStep;
        label_width = font_metrics.width(label);
        mMaxLabelWidth = mMaxLabelWidth < label_width ? label_width : mMaxLabelWidth;
    }
}

QString InstTimePlot::getLabel(double value)
{
    return QString::number(value, 'f', decimals);
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
    quint16 yStep = (height - 2 * mMargin) / major_cnt;

    for (int i=0; i<=major_cnt; ++i)
    {
        renderLabel(painter, sigCur, yPos);
        painter->drawLine(mMaxLabelWidth + mMargin / 2, yPos, width - mMargin, yPos);
        yPos -= yStep;
        sigCur += mSigStep;
    }

    return yPos + yStep;
}

void InstTimePlot::renderStatic(QPainter* painter)
{
    painter->fillRect(0, 0, width, height, color_background);

    setPen(painter, color_static);
    setFont(painter, font_size);

    // make sure we make enough space so bottom line does not hit the timestamp display
    mMargin = mTimestampRect.height() + 2;
    mSigStep = (mSignal->getMax() - mSignal->getMin()) / (major_cnt);

    setLabelMaxWidth(painter);

    mPlotStartX = mMargin + mMaxLabelWidth;
    mPlotStartY = height - mMargin;
    mPlotEndX = width - mMargin;
    mPlotEndY = renderLabelsAndMajors(painter);
    mPlotRangeX = mPlotEndX - mPlotStartX;
    mPlotRangeY = mPlotStartY - mPlotEndY;

    // render signal name
    painter->drawText(width/2,
                      mPlotEndY-PADDING,
                      QString("%1 (%2)").arg(mSignal->getName()).arg(mSignal->getUnit()));

    // init values
    mLastUpdateX = mPlotStartX;
    mLastUpdateY = mPlotStartY;
    mLastMarkerTime = 0;

    mGraphPixmap = QPixmap(width, height);
    mGraphPixmap.fill(Qt::transparent);
    this->setAttribute(Qt::WA_TranslucentBackground);
    mGraphPainter = new QPainter(&mGraphPixmap);
    mGraphPainter->setRenderHint(QPainter::Antialiasing);
}

QString InstTimePlot::getDisplayTime(int ticks, QString format)
{
    return QTime(0, 0, 0).addSecs(ticks / ticks_in_second).toString(format);
}

void InstTimePlot::renderMarker(QPainter* painter, quint64 timestamp)
{
    int markerTime = timestamp - (timestamp % marker_dt) + marker_dt;   // round up
    double cor = ((double)markerTime - timestamp - marker_dt) * mPlotRangeX / timespan;
    double markerX = mNewUpdateX + cor;

    if (markerX > mPlotStartX && markerX < mPlotEndX)
    {
        setPen(mGraphPainter, color_static);
        painter->drawLine(markerX, mPlotStartY, markerX, mPlotEndY);

        setFont(mGraphPainter, font_size);
        painter->drawText(markerX,
                                height,
                                getDisplayTime(mLastMarkerTime, division_format));
    }
    mLastMarkerTime = timestamp;
}

bool InstTimePlot::shouldRenderMarker(quint64 timestamp)
{
    return (timestamp >= mLastMarkerTime + marker_dt);
}

void InstTimePlot::renderTimeLabel(QPainter* painter)
{
    setPen(painter, color_static);
    setFont(painter, font_size);
    quint64 timestamp = mSignal->getTimestamp();
    painter->drawText(mPlotStartX,
                      mPlotEndY - 5,
                      "Time " + getDisplayTime(timestamp, master_time_format));
}

void InstTimePlot::renderGraphSegment(QPainter* painter)
{
    setPen(mGraphPainter, color_foreground);
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
    double dx = mPlotRangeX * dt / (timespan);

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

