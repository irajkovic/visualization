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

    explicit VisuControl(QWidget* parent,
                         QMap<QString, QString> properties,
                         QMap<QString, VisuPropertyMeta> metaProperties) : VisuWidget(parent, properties, metaProperties)
    {
        mAddress = QHostAddress(cActionIp);
    }

    virtual bool updateProperties(const QString &key, const QString &value);
    void loadProperties(QMap<QString, QString>& properties);
    virtual bool refresh(const QString& key) { (void)key; return false; }

protected:

    QString cActionIp;
    quint16 cActionPort;

    QHostAddress mAddress;
    QUdpSocket mSocket;



};

#endif // VISUCONTROL_H
