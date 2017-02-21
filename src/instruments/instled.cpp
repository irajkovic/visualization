#include "instled.h"
#include "visuconfiguration.h"

const QString InstLED::TAG_NAME = "LED";

void InstLED::renderStatic(QPainter *painter)
{
    clear(painter);

    cCenterH = (cHeight - cRadius) / 2;

    if (cShowSignalName)
    {
        setPen(painter, cColorStatic);
        setFont(painter);
        painter->drawText(cRadius+5, cCenterH + cFontSize, mSignal->getName());
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

    int imageId = conditionOn ? cImageOn : cImageOff;
    if (imageId >= 0)
    {
        VisuWidget* imageWidget = VisuConfiguration::get()->getWidget(imageId);
        StaticImage* image;
        if ( (image = qobject_cast<StaticImage*>(imageWidget)) != nullptr)
        {
            painter->drawImage(0, 0, image->getImage());
        }
    }
    else
    {
        setBrush(painter, conditionOn ? cColorOn : cColorOff);
        QRect rect(2, cCenterH, cRadius, cRadius);
        painter->drawEllipse(rect);
    }
}

