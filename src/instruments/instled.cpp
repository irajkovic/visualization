#include "instled.h"

const QString InstLED::TAG_NAME = "LED";

void InstLED::renderStatic(QPainter *painter)
{
    clear(painter);

    if (cShowSignalName)
    {
        setPen(painter, cColorStatic);
        setFont(painter, cFontSize);
        painter->drawText(cRadius+5, cHeight-5, mSignal->getName());
    }
}

void InstLED::renderDynamic(QPainter *painter)
{
    setPen(painter, cColorStatic);
    double value = mSignal->getRealValue();
    bool conditionOn = false;

    switch(cCondition)
    {
        case BETWEEN:       conditionOn = (value > cVal1) && (value < cVal2); break;
        case LESS_THAN:     conditionOn = value < cVal1; break;
        case LESS_EQUAL:    conditionOn = value <= cVal1; break;
        case MORE_EQUAL:    conditionOn = value >= cVal1; break;
        case MORE_THAN:     conditionOn = value > cVal1; break;
    }

    setBrush(painter, conditionOn ? cColorOn : cColorOff);
    QRect rect(0, 0, cRadius, cRadius);
    painter->drawEllipse(rect);
}

