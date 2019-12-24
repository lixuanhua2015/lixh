#include "serialport.h"
#include <QTextCodec>
#include <QDateTime>

serialPort::serialPort(QObject *parent) :
    VirtualPort(parent)
{
    //Default baudrate
    m_baud = BAUD19200;
    m_serialPort = new QextSerialPort(QextSerialPort::Polling);

    m_baudHash.insert(9600, BAUD9600);
    m_baudHash.insert(19200, BAUD19200);
    m_baudHash.insert(115200, BAUD115200);

#if defined(Q_OS_UNIX)
    m_baudHash.insert(230400, BAUD230400);
#endif

    connect(
            m_serialPort, SIGNAL(readyRead()),
            this, SIGNAL(readyRead())
           );

}

serialPort::~serialPort()
{
    delete m_serialPort;
    m_serialPort = NULL;
}

bool serialPort::open()
{   
    m_serialPort->setPortName(m_port);
    m_serialPort->setBaudRate(m_baud);
    m_serialPort->setDataBits(DATA_8);
    m_serialPort->setParity(PAR_NONE);
    m_serialPort->setStopBits(STOP_1);
    m_serialPort->setFlowControl(FLOW_OFF);

    if(m_serialPort->queryMode() == QextSerialPort::Polling)
        m_serialPort->setTimeout(m_timeout);

    if (m_serialPort->isOpen()) m_serialPort->close();

    bool f = m_serialPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    return f;
}

void serialPort::setParam(const QString &port, int timeout)
{
    this->m_timeout = timeout;
    this->m_port = port;
}

void serialPort::readSerial()
{
    QByteArray array = m_serialPort->readAll();
    //QTextCodec *codec = QTextCodec::codecForName("UTF-16Le");
    //QString tmpS = codec->toUnicode(array);
    QString tmpS = "";
    for (int i = 0; i < array.length(); i++) tmpS += QString::number(array[i] & 0xFF,16);
    qDebug() << tmpS;
    emit receiveMsg(tmpS);
}

void serialPort::close()
{
    if (m_serialPort->isOpen()) m_serialPort->close();
}

int serialPort::write(char *data, int len, char *recvCMD, int recvLen)
{
    m_flag = SUCCESS;
    m_errorMsg = "";

    int BytesInQue = m_serialPort->bytesAvailable();
    if(BytesInQue) m_serialPort->flush();

    char *curPtr = NULL;
    int recvBytes = 0;

    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    for(int tries=0; tries<1; tries++)
    {
        QString sendCMD = "[" + datetime + "] Send (" + QString::number(tries) + "): ";
        for(int i=0; i<len; i++)
            sendCMD += QString::number(data[i] & 0xFF, 16) + " ";
        qDebug() << sendCMD;
        if(m_serialPort->write(data, len) < len)
        {
            qDebug()<<"[qextserial]: "<<m_serialPort->errorString();
            m_flag = SEND_ERROR;
            m_errorMsg = "发送信息失败！";
            continue;
        }

        curPtr = recvCMD;

        while( (recvBytes = m_serialPort->read(curPtr, recvCMD + recvLen - curPtr)) >0 )
        {
            curPtr += recvBytes;
            if(curPtr == recvCMD + recvLen)
            {
                break;
            }
        }
        QString recvCMD = "[" + datetime + "] recv (" + QString::number(tries) + "): ";
        for(int i=0; i<recvLen; i++)
            recvCMD += QString::number(curPtr[i] & 0xFF, 16) + " ";
        qDebug() << recvCMD;
        if(curPtr == recvCMD + recvLen)
                    break;
    }

    if(curPtr != recvCMD + recvLen )
    {
        m_flag = REPLY_TIMEOUT;
        m_errorMsg = "通讯异常";
        qDebug() << "[qextserial]: " << m_serialPort->errorString()<< ", REPLY TIME OUT!";
    }

    if(m_flag != SUCCESS)
            qDebug()<<"CMD ERROR: ["<<m_flag<<"]";

    return m_flag;
}

QByteArray serialPort::readAll()
{
    return m_serialPort->readAll();
}

QByteArray serialPort::read(qint64 maxLen)
{
    return m_serialPort->read(maxLen);
}

qint64 serialPort::read(char* data, qint64 maxLen)
{
    return m_serialPort->read(data, maxLen);
}

qint64 serialPort::bytesAvailable() const
{
    return m_serialPort->bytesAvailable();
}

void serialPort::flush()
{
    m_serialPort->flush();
}

void serialPort::setQueryMode(QextSerialPort::QueryMode mode)
{
    m_serialPort->setQueryMode(mode);
}

qint64 serialPort::write(const char *array, qint64 len)
{
    return m_serialPort->write(array, len);
}

void serialPort::setBaudValue(int baud)
{
    if(m_baudHash.contains(baud))
        m_baud = m_baudHash.value(baud);

    qDebug() << "Set baudrate: " << baud << m_baud;
}
