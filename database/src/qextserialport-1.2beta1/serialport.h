#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QDebug>
#include "virtualport.h"

class serialPort : public VirtualPort
{
    Q_OBJECT
public:
    explicit serialPort(QObject *parent = 0);
    ~serialPort();

    void setParam(const QString &port, int timeout);
    bool open();
    void close();
    int write(char *data, int len, char *recvCMD, int recvLen);

    qint64 write(const char *array, qint64 len);
    QByteArray readAll();
    QByteArray read(qint64 maxLen);
    qint64 read(char* data, qint64 maxLen);
    qint64 bytesAvailable() const;
    //Clear receive buffer
    void flush();
    void setQueryMode(QextSerialPort::QueryMode mode);

    void setBaudValue(int baud);

signals:

public slots:
    void readSerial();

private:
    QextSerialPort *m_serialPort;
    BaudRateType m_baud;
    QHash<int, BaudRateType> m_baudHash;

};

#endif // SERIALPORT_H
