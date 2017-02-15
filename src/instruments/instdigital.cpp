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
    setFont(painter);
    setPen(painter, cColorForeground);

    //QString text = QString("%1").arg(mSignal->getRealValue(), cLeadingDigits, 10, QChar('0'));

    QString text =  QString::number(mSignal->getRealValue()).rightJustified(cLeadingDigits, '0');

    if (cShowSignalName) {
        text = mSignal->getName() + " = " + text;
    }

    if (cShowSignalUnit) {
        text += " " + mSignal->getUnit();
    }

    painter->drawText(cPadding, cHeight - cPadding, text);
}
