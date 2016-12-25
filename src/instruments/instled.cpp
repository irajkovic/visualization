#include "instled.h"

const QString InstLED::TAG_NAME = "LED";

InstLED::InstLED()
{
}

void InstLED::renderStatic(QPainter *painter)
{
    clear(painter);

    if (show_signal_lbl)
    {
        setPen(painter, color_static);
        setFont(painter, font_size);
        painter->drawText(radius+5, height, mSignal->getName());
    }
}

void InstLED::renderDynamic(QPainter *painter)
{
    setPen(painter, color_static);
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

    setBrush(painter, conditionOn ? color_on : color_off);
    QRect rect(0, 0, radius, radius);
    painter->drawEllipse(rect);
}

