#include "visudatagram.h"

bool VisuDatagram::checksumOk()
{
    quint8* ptr = (quint8*)this;
    quint8* end = &(checksum);
    quint8 sum = 0x0;

    while (ptr != end)
    {
        sum ^= *ptr;
        ++ptr;
    }

    return sum == checksum;
}
