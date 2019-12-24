#include "virtualport.h"
#include <QDebug>
#include <QByteArray>
VirtualPort::VirtualPort(QObject *parent) :
    QObject(parent)
{
    m_port = "";
}

VirtualPort::~VirtualPort()
{

}

void VirtualPort::createCRCWord(char *data, int len, char &high, char &low)
{
    static unsigned int crcl6_table[16] = /* CRC-16s */
    {
        0x0000, 0xCC01, 0xD801, 0x1400,
        0xF001, 0x3C00, 0x2800, 0xE401,
        0xA001, 0X6C00, 0x7800, 0xB401,
        0x5000, 0x9C01, 0x8801, 0x4400
    };
    unsigned short CRC = 0xFFFF;
    unsigned short tem;

    while ( len-- > 0 )
    {
        /* do the lower four bits */
        tem = crcl6_table[ CRC & 0xF ];
        CRC = ( CRC >> 4 ) & 0x0FFF;
        CRC = CRC ^ tem ^ crcl6_table[ *data & 0xF ];
        /* do the upper four bits */
        tem = crcl6_table[ CRC & 0xF ];
        CRC = ( CRC >> 4 ) & 0x0FFF;
        CRC = CRC ^ tem ^ crcl6_table[ ( *data >> 4 ) & 0xF ];
        /* advance to next byte */
        data++;
    }

    high = CRC;
    low = CRC >> 8;
}

bool VirtualPort::checkCRCByte(char *data, int len)
{
    char crc_high;
    char crc_low;
    createCRCWord(data, len - 2, crc_high, crc_low);
    bool flag = (crc_high == data[len - 2])
        && (crc_low == data[len - 1]);

    return flag;
}

void VirtualPort::addCRCWord(char *data, int len)
{
    char high,low;
    createCRCWord(data, len, high, low);
    data[len++] = high;
    data[len++] = low;
}

bool VirtualPort::checkCRCByteHigh(char *data, int len)
{
    char crc_high;
    char crc_low;
    createCRCWord(data, len - 2, crc_high, crc_low);
    bool flag = (crc_low == data[len - 2])
                && (crc_high == data[len - 1]);

    return flag;
}

void VirtualPort::addCRCWordHigh(char *data, int len)
{
    char high,low;
    createCRCWord(data, len, high, low);
    data[len++] = low;
    data[len++] = high;
}
