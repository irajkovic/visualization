#include "visuserver.h"
#include <QUdpSocket>
#include <qendian.h>
#include "visuappinfo.h"

const QByteArray VisuServer::delimiter = QByteArray("\r\n");

void VisuServer::handleSerialError(QSerialPort::SerialPortError serialPortError)
{
    //qDebug() << serialPortError;
    qDebug("Error!");
}

void VisuServer::handleSerial()
{
    serialBuffer.append(serialPort->readAll());

    int pos = serialBuffer.indexOf(delimiter);
    if (pos > 0)
    {
        serialBuffer.remove(0, serialBuffer.lastIndexOf(">") + 1);

        VisuDatagram datagram;
        QString serialBufferString(serialBuffer);
        QTextStream serialText(&serialBufferString);

        uint cs;
        serialText >> datagram.signalId >> datagram.packetNumber >> datagram.rawValue >> datagram.timestamp >> cs;
        datagram.checksum = (quint8) cs;

        if (datagram.checksumOk())
        {
            updateSignal(datagram);
        }
        else
        {
            qDebug("Bad serial package.");
        }

        serialBuffer.remove(0, pos + delimiter.size());
    }
}

void VisuServer::start()
{
    if (conectivity != SERIAL_ONLY)
    {
        qDebug("Started UDP server on port %d.", port);
        socket.bind(QHostAddress::LocalHost, port);
    }

    if (conectivity != UDP_ONLY)
    {
        if (VisuAppInfo::argsSize() < 4)
        {
            throw ConfigLoadException("Serial port name or baud rate not specified");
        }

        QString serialPortName = VisuAppInfo::getCLIArg(VisuAppInfo::RunArgs::SERIAL_PORT_NAME);
        int baudRate = VisuAppInfo::getCLIArg(VisuAppInfo::RunArgs::BAUD_RATE).toInt();

        serialPort->setPortName(serialPortName);
        if (    !serialPort->setBaudRate(baudRate) ||
                !serialPort->setDataBits(QSerialPort::Data8) ||
                !serialPort->setParity(QSerialPort::NoParity) ||
                !serialPort->setStopBits(QSerialPort::OneStop) ||
                !serialPort->setFlowControl(QSerialPort::NoFlowControl) )
        {
            throw ConfigLoadException(QObject::tr("serial port setup failed"));
        }

        if (!serialPort->open(QIODevice::ReadOnly))
        {
            throw ConfigLoadException(QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort->errorString()));
        }
        else
        {
            qDebug("Listening on serial port %s at baud rate %d", serialPortName.toStdString().c_str(), baudRate);
        }
    }
}

void VisuServer::stop()
{
    socket.close();
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

        VisuDatagram datagram = createDatagramFromBuffer((const quint8*)bufferRawData);

        if (datagram.checksumOk())
        {
            updateSignal(datagram);
        }
        else
        {
            qDebug("Bad UDP package.");
        }
    }
}

void VisuServer::updateSignal(const VisuDatagram& datagram)
{
    VisuSignal* signal = configuration->getSignal(datagram.signalId);
    if (signal != nullptr) {
        signal->datagramUpdate(datagram);
    }
}
