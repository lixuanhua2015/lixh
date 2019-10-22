/**************************************************
 * 文件名 :NewIoTObject.h
 * 文件描述:物联网测点万能对象
 * 创建人 ：McGrady Li
 * 创建时间：2019/10/17
***************************************************/
#include "IoTObject.h"

NewIoTObject::NewIoTObject(QObject *parent)
    : QObject(parent)
{
    m_configParam = NULL;
    m_paramNum = 0;
    m_comMode = 0;
    m_recvLen = 0;
    m_dataLen = 0;
    m_protocolType = 0;
    m_nextCMD = W_SHORT_NAME_CMD;
}

NewIoTObject::~NewIoTObject()
{
    if (m_configParam != NULL) {
        delete []m_configParam;
        m_configParam = NULL;
    }
    clearReadList();
    clearWriteList();
}

// 创建对象时，一定要setParameter，后续很多功能都必须在调用setParameter的前提下才能执行
bool NewIoTObject::setParameter(IoTConfigParam *paramList, const int &len)
{
    if (len <= 0) {
        return false;
    }
    if (m_configParam == NULL) {
        m_configParam = paramList;
        m_paramNum = len;
    }
    for (int i = 0; i < len; ++i) {
        uint8_t writeOrRead = m_configParam[i].paramRdOrWt;
        if (writeOrRead == kWriteOnly || writeOrRead == kWriteAndRead) {
            addParamId2WriteList(i);
        }
        if (writeOrRead == kReadOnly || writeOrRead == kWriteAndRead) {
            addParamId2ReadList(i);
        }
    }
    return true;
}

void NewIoTObject::getSendBuffer(char *sendBuf, const int sendLen)
{
    memcpy(sendBuf, m_sendBuf, sendLen);
}

bool NewIoTObject::parseRecvCMD(char *recvBuf, const int recvLen)
{
    if (recvLen < 1) {
        return false;
    }
    if (getRecvLen() != recvLen) {
        return false;
    }
    memcpy(m_recvBuf, recvBuf, recvLen);
    // 用来CRC校验
    if (true) {
        return false;
    }
    // 校验CMD主机地址
    int hostAddr = (m_recvBuf[0] << 8) + m_recvBuf[1];
    if (hostAddr != m_configParam[0].paramValue.toInt()) {
        return false;
    }
    uint8_t sendCmdType = m_recvBuf[2] - 1;
    uint8_t dataLen = m_recvBuf[3];
    switch (sendCmdType) {
    case W_LONG_NAME_CMD:
        if (dataLen != m_dataLen) {
            return false;
        }
        break;
    case W_SHORT_NAME_CMD:
        if (dataLen != m_dataLen) {
            return false;
        } else {
            if (!m_writeList.isEmpty()) {
                m_nextCMD = W_MULTI_PROP_CMD;
            }
            if (!m_readList.isEmpty()) {
                m_nextCMD = R_MULTI_PROP_CMD;
            }
        }
        break;
    case W_MULTI_PROP_CMD:
        if (dataLen != m_dataLen) {
            return false;
        } else {
            if (!m_readList.isEmpty()) {
                m_nextCMD = R_MULTI_PROP_CMD;
            }
        }
        break;
    case R_MULTI_PROP_CMD:
        if (dataLen != m_dataLen) {
            return false;
        } else {
            if (!m_writeList.isEmpty()) {
                m_nextCMD = W_MULTI_PROP_CMD;
            }
            recordRecvData();
        }
        break;
    case UPDATE_APP_CMD: // 目前更新APP不知道怎么实现

        break;
    default:

        break;
    }
    // 通信协议为被动接收，需要特殊处理
    if (getProtocolType() == kProtocolPassivity) {
        m_nextCMD = sendCmdType;
    } else {
        if ((m_recvBuf[2] & 0xFF) != (m_sendBuf[2] & 0xFF) + 1) {
            return false;
        }
    }
    prepareNextCMD(m_nextCMD);
    return true;
}

void NewIoTObject::prepareNextCMD(const uint8_t &nextCMD)
{
    int sendLen = 0;
    switch ((nextCMD & 0xFF))
    {
    case W_SHORT_NAME_CMD:
        sendLen = prepareWriteShortName();
        break;
    case W_MULTI_PROP_CMD:
        sendLen = prepareWriteProperties();
        break;
    case R_MULTI_PROP_CMD:
        sendLen = prepareReadProperties();
        break;
    default:
        break;
    }
    setSendLen(sendLen);
}

void NewIoTObject::addParamId2WriteList(const uint8_t &pId)
{
    if (!m_writeList.contains(pId)) {
        m_writeList.push_back(pId);
    }
}

void NewIoTObject::addParamId2ReadList(const uint8_t &pId)
{
    if (!m_readList.contains(pId)) {
        m_readList.push_back(pId);
    }
}

void NewIoTObject::delParamIdFromWrist(const uint8_t &pId)
{
    m_writeList.removeOne(pId);
}

void NewIoTObject::delParamIdFromRdist(const uint8_t &pId)
{
    m_readList.removeOne(pId);
}

void NewIoTObject::clearWriteList()
{
    m_writeList.clear();
}

void NewIoTObject::clearReadList()
{
    m_readList.clear();
}

void NewIoTObject::recordRecvData()
{
    uint8_t recvDataNum = m_recvBuf[CMD_HEAD_LEN];
    uint8_t idStartOffset = CMD_HEAD_LEN + 1;
    uint8_t valueStartOffset = idStartOffset + recvDataNum;
    for (int index = 0; index < recvDataNum; ++index) {
        uint8_t paramId = m_recvBuf[idStartOffset + index];
        uint8_t dataType = m_configParam[paramId].paramDataType;
        uint8_t paramLen = m_configParam[paramId].paramLen;
        uint8_t analysisType = m_configParam[paramId].analysisType;
        switch (dataType) {
        case kDataTypeInt:
        {
            int value = 0;
            if (analysisType == kAnalTypeH) {
                for (int i = 0; i < paramLen; ++i) {
                    value += m_recvBuf[valueStartOffset + i] << (8 * (paramLen - 1 - i));
                }
            } else if (analysisType == kAnalTypeL) {
                for (int i = 0; i < paramLen; ++i) {
                    value += m_recvBuf[valueStartOffset + i] << (8 * i);
                }
            }
            m_configParam->paramValue = value;
        }
            break;
        case kDataTypeFloat:
        {
            char chValue[4];
            if (analysisType == kAnalTypeH) {
                for (int i = 0; i < paramLen; ++i) {
                    chValue[i] = m_recvBuf[valueStartOffset + i];
                }
            } else if (analysisType == kAnalTypeL) {
                for (int i = 0; i < paramLen; ++i) {
                    chValue[i] = m_recvBuf[valueStartOffset + paramLen - 1 - i];
                }
            }
            float floValue = *(float*)chValue;
            m_configParam->paramValue = floValue;
        }
            break;
        case kDataTypeString:
        {
            QByteArray byteArrValue;
            byteArrValue.resize(paramLen);
            if (analysisType == kAnalTypeH) {
                for (int i = 0; i < paramLen; ++i) {
                    byteArrValue[i] = m_recvBuf[valueStartOffset + i];
                }
            } else if (analysisType == kAnalTypeL) {
                for (int i = 0; i < paramLen; ++i) {
                    byteArrValue[i] = m_recvBuf[valueStartOffset + paramLen - 1 - i];
                }
            }
            m_configParam->paramValue = byteArrValue;
        }
            break;
        default:
            break;
        }
       valueStartOffset += paramLen;
    }
}

int NewIoTObject::prepareWriteShortName()
{
    m_sendBuf[0] = (m_configParam[kInitAddr].paramValue.toInt() >> 8) & 0xFF;
    m_sendBuf[1] = m_configParam[kInitAddr].paramValue.toInt() & 0xFF;
    m_sendBuf[2] = W_SHORT_NAME_CMD;
    m_sendBuf[3] = LONG_ADDR_LEN + SHORT_ADDR_LEN;
    int startId = CMD_HEAD_LEN;
    writeParam2SendBuffer(startId, kLongAddr);
    startId += m_configParam[kLongAddr].paramLen;
    writeParam2SendBuffer(startId, kShortAddr);
    int len = CMD_HEAD_LEN + LONG_ADDR_LEN + SHORT_ADDR_LEN;

    // 添加CRC校验码
    //VirtualPort::addCRCWord(m_sendBuf, len);
    len += CRC_LEN; // 长度加上2位CRC校验码
    {// 计算该命令发送后，接收数据的长度
        int recvLen = CMD_HEAD_LEN + CRC_LEN;
        setRecvLen(recvLen);
    }
    return len;
}

int NewIoTObject::prepareWriteProperties()
{
    int len = 0;
    if (!m_writeList.isEmpty()) {
        m_sendBuf[0] = (m_configParam[kShortAddr].paramValue.toInt() >> 8) & 0xFF;
        m_sendBuf[1] = m_configParam[kShortAddr].paramValue.toInt() & 0xFF;
        m_sendBuf[2] = W_MULTI_PROP_CMD;
        int writeListSize = m_writeList.size();
        m_sendBuf[4] = writeListSize;
        int startId = 5;
        int valueStartId = startId + writeListSize;
        for (int i = 0; i < writeListSize; ++i) {
            m_sendBuf[startId + i] = m_writeList.at(i);
            writeParam2SendBuffer(valueStartId, m_writeList.at(i));
            valueStartId += m_configParam[m_writeList.at(i)].paramLen;
        }
        len = valueStartId;
        m_sendBuf[3] = len - CMD_HEAD_LEN;
        // 添加CRC校验码
        // VirtualPort::addCRCWord(m_sendBuf, len);
        len += CRC_LEN; // 长度加上2位CRC校验码
        {// 计算该命令发送后，接收数据的长度
            int recvLen = CMD_HEAD_LEN + CRC_LEN;
            setRecvLen(recvLen);
        }
    }
    return len;
}

int NewIoTObject::prepareReadProperties()
{
    int len = 0;
    if (!m_readList.isEmpty()) {
        m_sendBuf[0] = (m_configParam[kShortAddr].paramValue.toInt() >> 8) & 0xFF;
        m_sendBuf[1] = m_configParam[kShortAddr].paramValue.toInt() & 0xFF;
        m_sendBuf[2] = R_MULTI_PROP_CMD;
        int readListSize = m_readList.size();
        m_sendBuf[3] = readListSize;
        int startId = CMD_HEAD_LEN;
        int valueByteNum = 0;
        for (int i = 0; i < readListSize; ++i) {
            m_sendBuf[startId + i] = m_readList.at(i);
            valueByteNum += m_configParam[m_readList.at(i)].paramLen;
        }
        len = startId + readListSize;
        // 添加CRC校验码
        // VirtualPort::addCRCWord(m_sendBuf, len);
        len += CRC_LEN; // 长度加上2位CRC校验码
        {// 计算该命令发送后，接收数据的长度
            int recvLen = CMD_HEAD_LEN + (1 + readListSize + valueByteNum) + CRC_LEN;
            setRecvLen(recvLen);
        }
    }
    return len;
}

void NewIoTObject::writeParam2SendBuffer(const int &startId, const int &paramId)
{
    uint8_t dataType = m_configParam[paramId].paramDataType;
    uint8_t paramLen = m_configParam[paramId].paramLen;
    uint8_t analysisType = m_configParam[paramId].analysisType;
    QVariant value = m_configParam[paramId].paramValue;
    switch (dataType) {
    case kDataTypeInt:
    {
        if (analysisType == kAnalTypeH) {
            for (int i = 0; i < paramLen; ++i) {
                m_sendBuf[startId + i] = (value.toInt() >> (8 * (paramLen - 1 - i))) & 0xFF;
            }
        } else if (analysisType == kAnalTypeL) {
            for (int i = 0; i < paramLen; ++i) {
                m_sendBuf[startId + i] = (value.toInt() >> (8 * i)) & 0xFF;
            }
        }
    }
        break;
    case kDataTypeFloat:
    {
        float floatValue = value.toFloat();
        char *chValue = (char*)(&floatValue);
        if (analysisType == kAnalTypeH) {
            for (int i = 0; i < paramLen; ++i) {
                m_sendBuf[startId + i] = chValue[i];
            }
        } else if (analysisType == kAnalTypeL) {
            for (int i = 0; i < paramLen; ++i) {
                m_sendBuf[startId + i] = chValue[paramLen - 1 - i];
            }
        }
    }
        break;
    case kDataTypeString:
    {
        QByteArray strValue = value.toByteArray();
        if (analysisType == kAnalTypeH) {
            for (int i = 0; i < paramLen; ++i) {
                m_sendBuf[startId + i] = strValue[i];
            }
        } else if (analysisType == kAnalTypeL) {
            for (int i = 0; i < paramLen; ++i) {
                m_sendBuf[startId + i] = strValue[paramLen - 1 - i];
            }
        }
    }
        break;
    default:
        break;
    }
}





