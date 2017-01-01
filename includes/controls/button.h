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
            GET_PROPERTY(action_ip, QString);
            GET_PROPERTY(action_port, quint16);
            GET_PROPERTY(action_message, QString);
            GET_PROPERTY(css, QString);

            mAddress = QHostAddress(action_ip);
            setupButton(parent);
        }
        static const QString TAG_NAME;


private:

    Q_OBJECT

    QString action_ip;
    quint16 action_port;
    QString action_message;
    QString css;

    QPushButton* mButton;
    QHostAddress mAddress;
    QUdpSocket mSocket;

    void setupButton(QWidget* parent);

private slots:
    void sendCommand();

};

#endif // BUTTON_H
