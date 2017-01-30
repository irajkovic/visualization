#include "visucontrol.h"


void VisuControl::loadProperties(QMap<QString, QString> properties)
{
    VisuWidget::loadProperties(properties);

    GET_PROPERTY(cActionIp, QString, properties);
    GET_PROPERTY(cActionPort, quint16, properties);

}

