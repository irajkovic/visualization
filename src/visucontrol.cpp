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

    GET_PROPERTY(cActionIp, QString, mProperties, mPropertiesMeta);
    GET_PROPERTY(cActionPort, quint16, mProperties, mPropertiesMeta);

}

