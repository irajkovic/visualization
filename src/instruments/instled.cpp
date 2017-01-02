#include "instled.h"

const QString InstLED::TAG_NAME = "LED";

void InstLED::renderStatic(QPainter *painter)
{
    clear(painter);

    if (showSignalName)
    {
        setPen(painter, colorStatic);
        setFont(painter, fontSize);
        painter->drawText(radius+5, height-5, mSignal->getName());
    }
}

void InstLED::renderDynamic(QPainter *painter)
{
    setPen(painter, colorStatic);
    double value = mSignal->getRealValue();
    bool conditionOn = false;

    switch(condition)
    {
        case BETWEEN:       conditionOn = (value > val1) && (value < val2); break;
        case LESS_THAN:     conditionOn = value < val1; break;
        case LESS_EQUAL:    conditionOn = value <= val1; break;
        case MORE_EQUAL:    conditionOn = value >= val1; break;
        case MORE_THAN:     conditionOn = value > val1; break;
    }

    setBrush(painter, conditionOn ? colorOn : colorOff);
    QRect rect(0, 0, radius, radius);
    painter->drawEllipse(rect);
}

