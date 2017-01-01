#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QUdpSocket>
#include "visu_helper.h"

class Button : QObject
{
    Q_OBJECT

    public:
        explicit Button(
                QWidget *parent,
                QMap<QString, QString> properties)
        {

            // custom properties initializer
            GET_PROPERTY(id, quint16);
            GET_PROPERTY(label, QString);
            GET_PROPERTY(action_ip, QString);
            GET_PROPERTY(action_port, quint16);
            GET_PROPERTY(action_message, QString);
            GET_PROPERTY(css, QString);
            GET_PROPERTY(x, quint16);
            GET_PROPERTY(y, quint16);
            GET_PROPERTY(width, quint16);
            GET_PROPERTY(height, quint16);

            mAddress = QHostAddress(action_ip);
            setupButton(parent);
        }
        static const QString TAG_NAME;


private:
    quint16 id;
    quint16 x;
    quint16 y;
    quint16 width;
    quint16 height;

    QString label;
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
