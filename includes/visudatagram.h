#ifndef DATAGRAM_H
#define DATAGRAM_H

#include <QtGlobal>

struct VisuDatagram
{
    quint64 timestamp;
    quint64 rawValue;
    quint16 signalId;
    quint16 packetNumber;
    quint8 checksum;

    bool checksumOk();
};

#endif // DATAGRAM_H
