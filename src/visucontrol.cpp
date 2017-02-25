#include "visucontrol.h"

bool VisuControl::updateProperties(const QString &key, const QString &value)
{
    mProperties[key] = value;
    VisuControl::loadProperties(mProperties);
    return VisuControl::refresh(key);
}

void VisuControl::loadProperties(QMap<QString, QString> &properties)
{
    VisuWidget::loadProperties(properties);

    GET_PROPERTY(cActionIp, QString, properties, mPropertiesMeta);
    GET_PROPERTY(cActionPort, quint16, properties, mPropertiesMeta);

}

