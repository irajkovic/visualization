#ifndef VISUCONTROL_H
#define VISUCONTROL_H

#include <QMap>
#include <QHostAddress>
#include <QUdpSocket>
#include "visuwidget.h"

class VisuControl : public VisuWidget
{
    Q_OBJECT
public:

    explicit VisuControl(QWidget* parent, QMap<QString, QString> properties) : VisuWidget(parent, properties)
    {
        mAddress = QHostAddress(cActionIp);
    }

    void loadProperties(QMap<QString, QString> properties);

protected:

    QString cActionIp;
    quint16 cActionPort;

    QHostAddress mAddress;
    QUdpSocket mSocket;



};

#endif // VISUCONTROL_H
