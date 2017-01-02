#include "visuinstrument.h"

#include <QPainter>

/**
 * @brief Instrument::signalUpdated
 * Method used by signal to notify instrument of change.
 * @param signal
 */
void VisuInstrument::signalUpdated(const VisuSignal* const signal)
{
    this->mSignal = signal;
    render();
}

quint16 VisuInstrument::getId()
{
    return id;
}

quint16 VisuInstrument::getSignalId()
{
    return signalId;
}

void VisuInstrument::render()
{

    if (mFirstRun)
    {
        QPainter painter_static(&mPixmapStatic);
        painter_static.setRenderHint(QPainter::Antialiasing);
        renderStatic(&painter_static);
        mFirstRun = false;
    }

    QPainter painter_dynamic(&mPixmap);
    painter_dynamic.setRenderHint(QPainter::Antialiasing);
    painter_dynamic.drawPixmap(0, 0, mPixmapStatic);
    renderDynamic(&painter_dynamic);

    update();
}

void VisuInstrument::paintEvent(QPaintEvent* event)
{
    (void)event;    // supress compiler warning about unused parameter
    QPainter painter(this);
    painter.drawPixmap(0, 0, mPixmap);
}

void VisuInstrument::setFont(QPainter* painter, int fontSize)
{
    QFont font;
    font.setPointSize(fontSize);
    painter->setFont(font);
}

void VisuInstrument::setPen(QPainter* painter, QColor color, int thickness)
{
    QPen pen;
    pen.setColor(color);
    pen.setWidth(thickness);
    painter->setPen(pen);
}

void VisuInstrument::setBrush(QPainter* painter, QColor color)
{
    painter->setBrush(color);
}

void VisuInstrument::clear(QPainter* painter)
{
    painter->fillRect(0, 0, width, height, colorBackground);
}
