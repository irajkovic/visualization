#include "server.h"
#include <QUdpSocket>
#include <qendian.h>

VisuServer::VisuServer()
{
    QObject::connect(&socket, SIGNAL(readyRead()), this, SLOT(handleDatagram()));
}

void VisuServer::start()
{
    qDebug("Server is running on port %d.", port);
    socket.bind(QHostAddress::LocalHost, port);
}

void VisuServer::stop()
{
    socket.close();
}

bool VisuServer::datagramValid(const quint8* buffer, quint64 size)
{
    return DATAGRAM_SIZE == size &&
      VisuDatagram::getChecksum(buffer, DATAGRAM_SIZE - 1) == (quint8)buffer[DATAGRAM_SIZE - 1];
}

VisuDatagram VisuServer::createDatagramFromBuffer(const quint8* buffer)
{
    VisuDatagram datagram;

    datagram.signalId = qFromBigEndian<quint16>((uchar*)buffer);
    buffer += 2;

    datagram.packetNumber = qFromBigEndian<quint16>((uchar*)buffer);
    buffer += 2;

    datagram.timestamp = qFromBigEndian<quint64>((uchar*)buffer);
    buffer += 8;

    datagram.rawValue = qFromBigEndian<quint64>((uchar*)buffer);
    buffer += 8;

    datagram.checksum = *buffer;

    return datagram;
}

void VisuServer::handleDatagram()
{
    quint64 receivedSize;
    QByteArray buffer(DATAGRAM_SIZE, 0);
    char* bufferRawData = (char*)buffer.data();
    QHostAddress senderAddress;
    quint16 senderPort;

    while(socket.hasPendingDatagrams()) {

        receivedSize = socket.pendingDatagramSize();
        socket.readDatagram(bufferRawData, DATAGRAM_SIZE, &senderAddress, &senderPort);

        if (datagramValid((const quint8*)bufferRawData, receivedSize)) {

            VisuDatagram datagram = createDatagramFromBuffer((const quint8*)bufferRawData);
            updateSignal(datagram);
        }
        else {
            qDebug("Bad package.");
        }
    }
}

void VisuServer::updateSignal(const VisuDatagram& datagram)
{
    VisuSignal* signal = configuration->getSignal(datagram.signalId);
    if (signal != NULL) {
        signal->datagramUpdate(datagram);
    }
}
