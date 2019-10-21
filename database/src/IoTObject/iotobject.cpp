/**************************************************
 * 文件名 :iotobject.h
 * 文件描述:物联网测点万能对象
 * 创建人 ：McGrady Li
 * 创建时间：2019/10/17
***************************************************/
#include "iotobject.h"

IoTObject::IoTObject(QObject *parent)
    : QObject(parent)
{
    m_configParam = NULL;
    m_paramNum = 0;
    m_comMode = 0;
    m_recvLen = 0;
    m_dataLen = 0;
}

IoTObject::~IoTObject()
{
    if (m_configParam != NULL) {
        delete []m_configParam;
        m_configParam = NULL;
    }
}

// 创建对象时，一定要setParameter，后续很多功能都必须在调用setParameter的前提下才能执行
bool IoTObject::setParameter(const IoTConfigParam *paramList, const int &len)
{
    if (len <= 0) {
        return false;
    }
    if (m_configParam == NULL) {
        m_configParam = paramList;
        m_paramNum = len;
    }
    return true;
}

void IoTObject::getSendBuffer(char *sendBuf, const int sendLen)
{
    memcpy(sendBuf, m_sendBuf, sendLen);
}

bool IoTObject::parseRecvCMD(char *recvBuf, const int recvLen)
{
    if (recvLen < 1) {
        return false;
    }
    if (m_recvLen != recvLen) {
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
    // 校验接收数据的命令头和发送数据的命令头，lora通信模式是被动接收，需要特殊处理
    if (getComMode() == 0x20) {

    } else {
        if ((m_recvBuf[2] & 0xFF) != (m_sendBuf[2] & 0xFF) + 1) {
            return false;
        }
    }
    uint8_t dataLen = m_recvBuf[3];
    uint8_t sendCmdType = m_recvBuf[2] - 1;
    switch (sendCmdType) {
    case W_LONG_NAME_CMD:
    case W_SHORT_NAME_CMD:
    case W_MULTI_PROP_CMD:
        if (dataLen != m_dataLen) {
            return false;
        }
        break;
    case R_MULTI_PROP_CMD:
        if (dataLen != m_dataLen) {
            return false;
        } else {
            recordRecvData();
        }
        break;
    case UPDATE_APP_CMD:
        break;
    default:

        break;
    }
}

void IoTObject::recordRecvData()
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

            } else if (analysisType == kAnalTypeHL) {

            }
            for (int i = 0; i < paramLen; ++i) {
                value += m_recvBuf[valueStartOffset + i] << 8 * (paramLen - 1 - i);
            }
            m_configParam->paramValue = value;
        }
            break;
        case kDataTypeFloat:

            break;
        case kDataTypeString:

            break;
        default:
            break;
        }
       valueStartOffset += paramLen;
    }
}





