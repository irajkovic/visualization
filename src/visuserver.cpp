#include "visuserver.h"
#include <QUdpSocket>
#include <qendian.h>
#include "visuappinfo.h"
#include <QRegularExpressionMatch>
#include <QDateTime>

const QByteArray VisuServer::DELIMITER = QByteArray("\n");

void VisuServer::handleSerialError(QSerialPort::SerialPortError serialPortError)
{
    (void) serialPortError; // Currently unused parameter
    qDebug("Error!");
}

VisuServer::VisuServer()
{
    mConfiguration = VisuConfiguration::get();
    mConectivity = (enum Connectivity)mConfiguration->getConectivity();
    if (mConfiguration->isSerialBindToSignal())
    {
        mSerialRegex = QRegularExpression(mConfiguration->getSerialRegex());
    }

    if (mConectivity != SERIAL_ONLY)
    {
        mPort = mConfiguration->getPort();
        QObject::connect(&socket, SIGNAL(readyRead()), this, SLOT(handleDatagram()));
    }

    if (mConectivity != UDP_ONLY)
    {
        mSerialPort = new QSerialPort(this);
        QObject::connect(mSerialPort, SIGNAL(readyRead()), this, SLOT(handleSerial()));
        QObject::connect(mSerialPort,
                static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
                this,
                &VisuServer::handleSerialError );
    }

    VisuAppInfo::setServer(this);
}

void VisuServer::sendSerial(const QByteArray& data)
{
    if (mSerialPort != nullptr)
    {
        qint64 bytesWritten = mSerialPort->write(data);

        if (bytesWritten == -1)
        {
            qDebug() << QObject::tr("Failed to write the data to port %1, error: %2")
                        .arg(mSerialPort->portName())
                        .arg(mSerialPort->errorString());
        }
        else if (bytesWritten != data.size())
        {
            qDebug() << QObject::tr("Failed to write all the data to port %1, error: %2")
                        .arg(mSerialPort->portName())
                        .arg(mSerialPort->errorString());
        }
    }
}

void VisuServer::parseVisuSerial()
{
    mSerialBuffer.remove(0, mSerialBuffer.lastIndexOf(">") + 1);

    VisuDatagram datagram;
    QString serialBufferString(mSerialBuffer);
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
}

void VisuServer::parseRegexSerial()
{
    QString serialBufferString(mSerialBuffer);
    QRegularExpressionMatch match = mSerialRegex.match(serialBufferString);
    if (match.hasMatch())
    {
        QStringList values = match.capturedTexts();

        QVector<QPointer<VisuSignal>> signalsList = mConfiguration->getSignals();
        for (VisuSignal* signal : signalsList)
        {
            int valueIndex = signal->getSerialPlaceholder();
            if (valueIndex > 0)
            {
                VisuDatagram datagram;
                datagram.signalId = signal->getId();

                QString value = values[valueIndex];
                if (signal->getSerialTransform())
                {
                    datagram.rawValue = value.toInt();
                }
                else
                {
                    datagram.rawValue = (int)((value.toDouble() - signal->getOffset()) / signal->getFactor());
                }
                datagram.timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
                updateSignal(datagram);
            }
        }
    }
}

void VisuServer::handleSerial()
{
    mSerialBuffer.append(mSerialPort->readAll());

    int pos = mSerialBuffer.indexOf(DELIMITER);
    if (pos > 0)
    {
        if (mConfiguration->isSerialBindToSignal() && mSerialRegex.isValid())
        {
            parseRegexSerial();
        }
        else
        {
            parseVisuSerial();
        }

        mSerialBuffer.remove(0, pos + DELIMITER.size());
    }
}

void VisuServer::start()
{
    if (mConectivity != SERIAL_ONLY)
    {
        qDebug("Started UDP server on port %d.", mPort);
        socket.bind(QHostAddress::LocalHost, mPort);
    }

    if (mConectivity != UDP_ONLY)
    {
        ConfigLoadException::setContext("setting up serial port");

        if (VisuAppInfo::argsSize() < 4)
        {
            throw ConfigLoadException("Serial port name or baud rate not specified");
        }

        QString serialPortName = VisuAppInfo::getCLIArg(VisuAppInfo::RunArgs::SERIAL_PORT_NAME);
        int baudRate = VisuAppInfo::getCLIArg(VisuAppInfo::RunArgs::BAUD_RATE).toInt();

        mSerialPort->setPortName(serialPortName);
        if (    !mSerialPort->setBaudRate(baudRate) ||
                !mSerialPort->setDataBits(QSerialPort::Data8) ||
                !mSerialPort->setParity(QSerialPort::NoParity) ||
                !mSerialPort->setStopBits(QSerialPort::OneStop) ||
                !mSerialPort->setFlowControl(QSerialPort::NoFlowControl) )
        {
            throw ConfigLoadException(QObject::tr("Setup of %1 failed"), serialPortName);
        }

        if (!mSerialPort->open(QIODevice::ReadWrite))
        {
            throw ConfigLoadException(QObject::tr("Failed to open port %1, error: %2")
                                      .arg(serialPortName)
                                      .arg(mSerialPort->errorString()));
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
    VisuSignal* signal = mConfiguration->getSignal(datagram.signalId);
    if (signal != nullptr) {
        signal->datagramUpdate(datagram);
    }
}
