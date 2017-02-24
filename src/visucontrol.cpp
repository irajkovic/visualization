#include "visucontrol.h"

void VisuControl::reloadProperties(QMap<QString, QString>& properties)
{
    VisuControl::loadProperties(properties);
}

void VisuControl::loadProperties(QMap<QString, QString> &properties)
{
    VisuWidget::loadProperties(properties);

    GET_PROPERTY(cActionIp, QString, properties, mPropertiesMeta);
    GET_PROPERTY(cActionPort, quint16, properties, mPropertiesMeta);

}

