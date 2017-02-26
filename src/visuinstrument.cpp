#include "visuinstrument.h"

#include <QPainter>
#include <QStyleOption>
#include "visumisc.h"

bool VisuInstrument::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    VisuInstrument::loadProperties();
    return VisuInstrument::refresh(key);
}

void VisuInstrument::loadProperties()
{
    VisuWidget::loadProperties();

    ConfigLoadException::setInstrumentLoadContext(mProperties);

    GET_PROPERTY(cSignalId, quint16, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorBackground, QColor, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorStatic, QColor, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorForeground, QColor, mProperties, mPropertiesMeta);
    GET_PROPERTY(cFontSize, quint8, mProperties, mPropertiesMeta);
    GET_PROPERTY(cFontType, QString, mProperties, mPropertiesMeta);

    setup();
}

void VisuInstrument::setup()
{
    VisuWidget::setup();
    mFirstRun = true;
    mPixmap = QPixmap(cWidth, cHeight);
    mPixmapStatic = QPixmap(cWidth, cHeight);
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

    // draw instrument
    painter.drawPixmap(0, 0, mPixmap);

    drawActiveBox(&painter);
}

void VisuInstrument::setFont(QPainter* painter)
{
    QFont font;
    font.setPixelSize(cFontSize);
    font.setFamily(cFontType);
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

void VisuInstrument::connectSignals()
{
    disconnectSignals();

    const QVector<QPointer<VisuSignal>>& signalsList = VisuConfiguration::get()->getSignals();

    auto itr = mPropertiesMeta.begin();
    while (itr != mPropertiesMeta.end())
    {
        VisuPropertyMeta meta = itr.value();
        if (meta.type == VisuPropertyMeta::INSTSIGNAL)
        {
            int signalId = mProperties.value(itr.key()).toInt();
            VisuSignal* sig = VisuConfiguration::get()->getSignal(signalId);
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

bool VisuInstrument::refresh(const QString& key)
{
    VisuWidget::refresh(key);
    // Signal assigment changed, update signals map
    if (mPropertiesMeta[key].type == VisuPropertyMeta::INSTSIGNAL)
    {
        connectSignals();
    }
    initializeInstrument();
    return false;
}
