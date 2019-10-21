/**************************************************
 * 文件名 :iotobject.h
 * 文件描述:物联网测点万能对象
 * 创建人 ：McGrady Li
 * 创建时间：2019/10/17
***************************************************/
#ifndef IOTOBJECT_H
#define IOTOBJECT_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QVariant>
#include <QDebug>
#include <QVariant>

#define W_LONG_NAME_CMD     0x01
#define W_SHORT_NAME_CMD    0x03
#define W_ONE_PROP_CMD      0x05
#define W_MULTI_PROP_CMD    0x07

#define R_ONE_PROP_CMD      0x81
#define R_MULTI_PROP_CMD    0x83

#define UPDATE_APP_CMD      0xA1

#define CMD_HEAD_LEN        4   //|SHORT_ADD_H|SHORT_ADD_L|CMD|DATA_LEN|

const int TRANSMIT_BUFFER_SIZE = 255;

struct IoTConfigParam{
    uint8_t  paramId;
    QVariant paramValue;
    uint8_t paramLen;
    uint8_t paramDataType; // int、float、字符
    uint8_t analysisType; // 高字节在前还是低字节在前
    uint8_t paramRdOrWt; // 参数的读写情况
    QVariant paramMin;  // 参数的最小值
    QVariant paramMax;  // 参数的最大值
};

class IoTObject : public QObject
{
    Q_OBJECT
public:
    explicit IoTObject(QObject *parent = NULL);

    ~IoTObject();

    enum {
        kInitAddr,
        kLongAddr,
        kShortAddr
    };
    // 数据类型int、float，string
    enum {
        kDataTypeInt,
        kDataTypeFloat,
        kDataTypeString
    };
    // analysisType
    enum {
        kAnalTypeH, //高字节在前
        kAnalTypeL // 低字节在前
    };
    bool setParameter(const IoTConfigParam *paramList, const int &len);

    void getSendBuffer(char *sendBuf, const int sendLen);

    bool parseRecvCMD(char *recvBuf, const int recvLen);

    int getComMode() const
    {
        return m_comMode;
    }

    void setComMode(int &comMode)
    {
        m_comMode = comMode;
    }

    int getShortAddr() const
    {
        m_configParam[kShortAddr].paramValue.toInt();
    }

    void setShortAddr(int &shortAddr)
    {
        m_configParam[kShortAddr].paramValue = shortAddr;
    }

    int getSendLen() const
    {
        return m_sendLen;
    }

    int getRecvLen() const
    {
        return m_recvLen;
    }
private:
    void setRecvLen(const int &recvLen)
    {
        m_recvLen = recvLen;
    }


    void recordRecvData();
private:
    IoTConfigParam *m_configParam;

    char m_sendBuf[TRANSMIT_BUFFER_SIZE];
    char m_recvBuf[TRANSMIT_BUFFER_SIZE];

    int m_paramNum;
    int m_comMode;

    int m_recvLen;
    int m_sendLen;

    uint8_t m_dataLen;
};

#endif // IOTOBJECT_H
