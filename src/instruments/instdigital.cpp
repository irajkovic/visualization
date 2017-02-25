#include "instdigital.h"
#include <QPainter>
#include <QFont>

const QString InstDigital::TAG_NAME = "DIGITAL";

bool InstDigital::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    InstDigital::loadProperties(mProperties);
    return VisuInstrument::refresh(key);
}

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
