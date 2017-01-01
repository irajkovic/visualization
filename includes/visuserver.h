#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>

#include "visusignal.h"
#include "visudatagram.h"
#include "visuconfiguration.h"

class VisuServer : public QObject
{
    Q_OBJECT

    private:

        quint16 port;
        static const quint8 DATAGRAM_SIZE = 21;

        QUdpSocket socket;
        VisuConfiguration *configuration;

        void updateSignal(const VisuDatagram& datagram);
        bool datagramValid(const quint8* buffer, quint64 size);
        VisuDatagram createDatagramFromBuffer(const quint8* buffer);

    public slots:
        void handleDatagram();

    public:
        VisuServer();
        VisuServer(VisuConfiguration* configuration) : configuration(configuration)
        {
            port = configuration->getPort();
            QObject::connect(&socket, SIGNAL(readyRead()), this, SLOT(handleDatagram()));
        }
        void start();
        void stop();
};

#endif // SERVER_H
