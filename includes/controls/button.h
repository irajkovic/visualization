#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QUdpSocket>
#include "visuhelper.h"
#include "visuwidget.h"

class Button : public VisuWidget
{
    public:
        explicit Button(
                QWidget *parent,
                QMap<QString, QString> properties) : VisuWidget(parent, properties)
        {

            // custom properties initializer
            GET_PROPERTY(actionIp, QString, properties);
            GET_PROPERTY(actionPort, quint16, properties);
            GET_PROPERTY(actionMessage, QString, properties);
            GET_PROPERTY(css, QString, properties);

            mAddress = QHostAddress(actionIp);
            setupButton(parent);
        }
        static const QString TAG_NAME;


private:

    Q_OBJECT

    QString actionIp;
    quint16 actionPort;
    QString actionMessage;
    QString css;

    QPushButton* mButton;
    QHostAddress mAddress;
    QUdpSocket mSocket;

    void setupButton(QWidget* parent);

private slots:
    void sendCommand();

};

#endif // BUTTON_H
