#include "visuinstrument.h"

#include <QPainter>
#include <QStyleOption>
#include "visumisc.h"

void VisuInstrument::loadProperties(QMap<QString, QString> properties)
{
    VisuWidget::loadProperties(properties);

    ConfigLoadException::setInstrumentLoadContext(properties);

    GET_PROPERTY(cSignalId, quint16, properties);
    GET_PROPERTY(cColorBackground, QColor, properties);
    GET_PROPERTY(cColorStatic, QColor, properties);
    GET_PROPERTY(cColorForeground, QColor, properties);

    this->mActive = false;
    this->mFirstRun = true;
    this->mPixmap = QPixmap(cWidth, cHeight);
    this->mPixmapStatic = QPixmap(cWidth, cHeight);

    clearPixmaps();
    setAttribute(Qt::WA_TranslucentBackground);

    setGeometry(cX, cY, cWidth, cHeight);
}


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

void VisuInstrument::initialUpdate(const VisuSignal* const signal)
{
    mFirstRun = true;
    signalUpdated(signal);
}

quint16 VisuInstrument::getId()
{
    return cId;
}

quint16 VisuInstrument::getSignalId()
{
    return cSignalId;
}

void VisuInstrument::render()
{
    mPixmap.fill(Qt::transparent);

    if (mFirstRun)
    {
        mPixmapStatic.fill(Qt::transparent);
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
#if 0
    // allow for custom stylesheets
    QStyleOption option;
    option.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
#endif
    // draw instrument
    painter.drawPixmap(0, 0, mPixmap);

    if (mActive)
    {
        setPen(&painter, Qt::GlobalColor::black, 4);
        painter.drawRect(0, 0, cWidth, cHeight);
        painter.setPen(VisuMisc::getDashedPen(Qt::GlobalColor::white, 4, {5, 5}));
        painter.drawRect(0, 0, cWidth, cHeight);
    }
}

void VisuInstrument::setFont(QPainter* painter, int fontSize)
{
    QFont font;
    font.setPixelSize(fontSize);
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
    painter->fillRect(0, 0, cWidth, cHeight, cColorBackground);
}

void VisuInstrument::connectSignals(const QVector<QPointer<VisuSignal>>& signalsList)
{
    disconnectSignals();

    auto itr = mPropertiesMeta.begin();
    while (itr != mPropertiesMeta.end())
    {
        VisuPropertyMeta meta = itr.value();
        if (meta.type == VisuPropertyMeta::TYPE_SIGNAL)
        {
            int signalId = mProperties.value(itr.key()).toInt();
            VisuSignal* sig = signalsList.at(signalId);
            if (sig != nullptr)
            {
                connectedSignals.append(sig);
                sig->connectInstrument(this);
            }
        }

        ++itr;
    }
}

void VisuInstrument::disconnectSignals()
{
    std::for_each(connectedSignals.begin(),
                  connectedSignals.end(),
                  [this](QPointer<VisuSignal> sig){ sig->disconnectInstrument(this); } );
    connectedSignals.clear();
}

void VisuInstrument::initializeInstrument()
{
    std::for_each(connectedSignals.begin(),
                  connectedSignals.end(),
                  [this](QPointer<VisuSignal> sig){ sig->initializeInstruments(); } );
}
