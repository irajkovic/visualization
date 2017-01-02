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
    setFont(painter, fontSize);
    setPen(painter, colorForeground);

    QString text = QString("%1").arg(mSignal->getRealValue());

    if (showSignalName) {
        text = mSignal->getName() + " = " + text;
    }

    if (showSignalUnit) {
        text += mSignal->getUnit();
    }

    painter->drawText(padding, height - padding, text);
}
