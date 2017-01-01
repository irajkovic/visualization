#include "instdigital.h"
#include <QPainter>
#include <QFont>

const QString InstDigital::TAG_NAME = "DIGITAL";

void InstDigital::renderStatic(QPainter* painter)
{
    clear(painter);
}

void InstDigital::renderDynamic(QPainter* painter)
{
    setFont(painter, font_size);
    setPen(painter, color_foreground);

    QString text = QString("%1").arg(mSignal->getRealValue());

    if (show_signal_name) {
        text = mSignal->getName() + " = " + text;
    }

    if (show_signal_unit) {
        text += mSignal->getUnit();
    }

    painter->drawText(padding, height - padding, text);
}
