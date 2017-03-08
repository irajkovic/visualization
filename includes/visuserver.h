#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QSerialPort>
#include <QRegularExpression>
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

    VisuServer();
    void start();
    void stop();

    private:

        quint16 mPort;
        enum Connectivity mConectivity;

        static const quint8 DATAGRAM_SIZE = 21;

        QUdpSocket socket;
        VisuConfiguration *mConfiguration;

        QSerialPort* mSerialPort;

        void updateSignal(const VisuDatagram& datagram);
        VisuDatagram createDatagramFromBuffer(const quint8* buffer);

        QByteArray mSerialBuffer;
        QRegularExpression mSerialRegex;

        static const QByteArray DELIMITER;

    public slots:
        void handleDatagram();
        void handleSerial();
        void parseVisuSerial();
        void parseRegexSerial();
        void sendSerial(const QByteArray& data);
        void handleSerialError(QSerialPort::SerialPortError serialPortError);


};

#endif // SERVER_H
