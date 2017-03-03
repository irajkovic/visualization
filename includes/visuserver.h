#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QSerialPort>
#include "visuappinfo.h"
#include "visusignal.h"
#include "visudatagram.h"
#include "visuconfiguration.h"

class VisuServer : public QObject
{
    Q_OBJECT

    public:

    enum Connectivity
    {
        UDP_AND_SERIAL,
        UDP_ONLY,
        SERIAL_ONLY
    };

    VisuServer(VisuConfiguration* configuration) : configuration(configuration)
    {
        port = configuration->getPort();
        conectivity = (enum Connectivity)configuration->getConectivity();

        if (conectivity != SERIAL_ONLY)
        {
            QObject::connect(&socket, SIGNAL(readyRead()), this, SLOT(handleDatagram()));
        }

        if (conectivity != UDP_ONLY)
        {
            serialPort = new QSerialPort(this);
            QObject::connect(serialPort, SIGNAL(readyRead()), this, SLOT(handleSerial()));
            QObject::connect(serialPort,
                    static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
                    this,
                    &VisuServer::handleSerialError );
        }

        VisuAppInfo::setServer(this);
    }
    void start();
    void stop();

    private:

        quint16 port;
        enum Connectivity conectivity;

        static const quint8 DATAGRAM_SIZE = 21;

        QUdpSocket socket;
        VisuConfiguration *configuration;

        QSerialPort* serialPort;

        void updateSignal(const VisuDatagram& datagram);
        VisuDatagram createDatagramFromBuffer(const quint8* buffer);

        QByteArray serialBuffer;
        static const QByteArray delimiter;

    public slots:
        void handleDatagram();
        void handleSerial();
        void sendSerial(const QByteArray& data);
        void handleSerialError(QSerialPort::SerialPortError serialPortError);


};

#endif // SERVER_H
