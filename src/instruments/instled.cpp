#include "instled.h"
#include "visuconfiguration.h"

const QString InstLED::TAG_NAME = "LED";

bool InstLED::updateProperties(const QString& key, const QString& value)
{
    mProperties[key] = value;
    InstLED::loadProperties();
    return VisuInstrument::refresh(key);
}

void InstLED::loadProperties()
{
    VisuInstrument::loadProperties();

    GET_PROPERTY(cRadius, mProperties, mPropertiesMeta);
    GET_PROPERTY(cVal1, mProperties, mPropertiesMeta);
    GET_PROPERTY(cVal2, mProperties, mPropertiesMeta);
    GET_PROPERTY(cCondition, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorOn, mProperties, mPropertiesMeta);
    GET_PROPERTY(cColorOff, mProperties, mPropertiesMeta);
    GET_PROPERTY(cImageOn, mProperties, mPropertiesMeta);
    GET_PROPERTY(cImageOff, mProperties, mPropertiesMeta);
    GET_PROPERTY(cShowSignalName, mProperties, mPropertiesMeta);

    mTagName = InstLED::TAG_NAME;
}

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

