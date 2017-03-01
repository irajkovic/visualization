#include "visucontrol.h"

bool VisuControl::updateProperties(const QString &key, const QString &value)
{
    mProperties[key] = value;
    VisuControl::loadProperties();
    return VisuControl::refresh(key);
}

void VisuControl::loadProperties()
{
    VisuWidget::loadProperties();

    GET_PROPERTY(cActionIp, mProperties, mPropertiesMeta);
    GET_PROPERTY(cActionPort, mProperties, mPropertiesMeta);

}

