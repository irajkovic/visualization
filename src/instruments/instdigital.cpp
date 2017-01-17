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
    setFont(painter, cFontSize);
    setPen(painter, cColorForeground);

    QString text = QString("%1").arg(mSignal->getRealValue());

    if (cShowSignalName) {
        text = mSignal->getName() + " = " + text;
    }

    if (cShowSignalUnit) {
        text += " " + mSignal->getUnit();
    }

    painter->drawText(cPadding, cHeight - cPadding, text);
}
