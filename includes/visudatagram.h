#ifndef DATAGRAM_H
#define DATAGRAM_H

struct VisuDatagram
{
    quint64 timestamp;
    quint64 rawValue;
    quint16 signalId;
    quint16 packetNumber;
    quint8 checksum;

    static quint8 getChecksum(const quint8* data, quint16 datagram_size)
    {
        quint8 i = 0;
        quint8 checksum = 0x0;

        for (i = 0; i<datagram_size; i++)
        {
            checksum ^= *data;
            ++data;
        }

        // qDebug("Checksum is: %x", checksum);

        return checksum;
    }
};

#endif // DATAGRAM_H
