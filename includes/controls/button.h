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
            GET_PROPERTY(cActionIp, QString, properties);
            GET_PROPERTY(cActionPort, quint16, properties);
            GET_PROPERTY(cActionMessage, QString, properties);
            GET_PROPERTY(cCss, QString, properties);

            mAddress = QHostAddress(cActionIp);
            setupButton(parent);
        }
        static const QString TAG_NAME;


private:

    Q_OBJECT

    QString cActionIp;
    quint16 cActionPort;
    QString cActionMessage;
    QString cCss;

    QPushButton* mButton;
    QHostAddress mAddress;
    QUdpSocket mSocket;

    void setupButton(QWidget* parent);

private slots:
    void sendCommand();

};

#endif // BUTTON_H
