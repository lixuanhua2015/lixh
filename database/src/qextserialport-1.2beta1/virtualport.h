#ifndef VIRTUALPORT_H
#define VIRTUALPORT_H

#include <QObject>
#include <QByteArray>

#include "qextserialport.h"

class VirtualPort : public QObject
{
    Q_OBJECT
public:
    explicit VirtualPort(QObject *parent = 0);
    ~VirtualPort();

    enum {
            REPLY_TIMEOUT,
            SEND_ERROR,
            SUCCESS
        };

    /**
     * @brief 设置参数
     * @param port
     * @param timeout
     */
    virtual void setParam(const QString &port, int timeout)
    {
        m_port = port;
        m_timeout = timeout;
    }

    /**
     * @brief open
     */
    virtual bool open() = 0;

    /**
     * @brief close
     */
    virtual void close(){}

    /**
     * @brief write
     * @param array
     */
    virtual int write(char *data, int len, char *recvCMD, int recvLen) = 0;

    virtual qint64 write(const char *array, qint64 len) = 0;
	
    virtual QByteArray readAll() = 0;
    virtual qint64 bytesAvailable() const = 0;
    virtual void setQueryMode(QextSerialPort::QueryMode mode) = 0;

    /**
     * @brief 创建CRC校验位
     */
	static void createCRCWord(char *data, int len, char &high, char &low);

    /**
     * @brief 核对CRC校验位，低字节在前
     */
    static bool checkCRCByte(char *data, int len);

    /**
     * @brief 添加CRC校验位，低字节在前
     */
    static void addCRCWord(char *data, int len);

    /**
     * @brief 核对CRC校验位，高字节在前
     */
    static bool checkCRCByteHigh(char *data, int len);

    /**
     * @brief 添加CRC校验位,高字节在前
     */
    static void addCRCWordHigh(char *data, int len);

    inline QString getPortName() const
    {
        return m_port;
    }

    inline QString getErrorMsg() const
    {
        return m_errorMsg;
    }

signals:
    /**
     * @brief 收到的消息
     * @param msg
     */
    void receiveMsg(QString msg);

    void readyRead();   //QIoDevice

public slots:
    /**
     * @brief 收到串口消息后的槽
     * @param msg
     */
    virtual void readSerial(){}

protected:
    QString m_port;
    QString m_sendMsg;
    int m_portIndex;
    int m_timeout;

    int m_flag;

    QString m_errorMsg;

};

#endif // VIRTUALPORT_H
