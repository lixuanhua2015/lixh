#include <QDebug>
#include <QDateTime>
#include <QTextCodec>
#include <QNetworkInterface>
#include "dial3g.h"

Dial3G::Dial3G()
{
    m_3gBar = 0;
    m_creg = false;
    m_simState = false;
    m_dialFlag = false;
    m_dialState = false;
    m_initFlag = true;

    m_serialPort = new serialPort(this);

    m_process = new QProcess(this);

    m_dialFailureCnt = 0;
    m_sendMsgFlag = false;
    m_sendMsg = "";
    m_recvPhone = "";
}

Dial3G::~Dial3G()
{
    m_process->close();
    delete m_process;
    m_process = NULL;

    m_serialPort->close();
    delete m_serialPort;
    m_serialPort = NULL;
}

void Dial3G::setPortParam(const QString &portName, int timeout)
{
    m_serialPort->setParam(portName, timeout);
}

bool Dial3G::openPort()
{
    RTU_DEBUG << "open port thread : " << QThread::currentThreadId();
    return m_serialPort->open();
}

int Dial3G::getState()
{
    int state = 0;
    if(m_simState)
        state |= 0x04;

    if(m_creg)
        state |= 0x02;

    if(m_dialState)
        state |= 0x01;

    RTU_DEBUG << "3G State = " << state;
    return state;
}

void Dial3G::restarting()
{
    RTU_DEBUG << "Restarting 3G Module!";

    if(!m_creg && m_regStatus != kIsRegistering)
        RTU_DEBUG << "Cannot register-->restarting!";
    else if(!m_simState)
        RTU_DEBUG << "Cannot find sim card-->restarting!";
    else if(m_dialFailureCnt >= 4)
        RTU_DEBUG << "Too many failures in dialing-->restarting!";

    //reset 3g state flag
    m_dialState = false;

    //get current mode
    readPhoneMode();
    RTU_DEBUG << "Current mode = " << m_funStatus;

    if(m_funStatus == kMinFunc)    //minimum function
        writePhoneMode(kFullFunc);     //restart
    else
        writePhoneMode(kMinFunc);
}

void Dial3G::run()
{
    if(m_initFlag)
    {
        initATE();
        m_initFlag = false;
    }
//    if (m_sendMsgFlag) {
//        m_sendMsgFlag = false;
//        sendMsg(m_sendMsg, m_recvPhone);
//    }
    //Get reg status
    refreshCREG();

    if(!m_creg)
        m_dialState = false;

    //valid simcard
    if(refreshSysInfo())
    {
        refresh3GBar();
        refreshDialState();

        if(!m_dialFlag) //need dialing
        {
            if(m_dialFailureCnt < 4)    //try 0, 1, 2, 3: four times
            {
                RTU_DEBUG << "DIAL 3G!";
                dial3G();
                m_dialFailureCnt++;
            }

            m_dialFlag = true;
        }
    }
    else
        //reset 3g state flag
        m_dialState = false;

    if(
            (!m_creg && m_regStatus != kIsRegistering)  //not registered & is not in register process
            || !m_simState  //no sim card
            || (m_dialFailureCnt >= 4)  //cannot dial
            )
        restarting();

    emit refresh3GSig();
}

void Dial3G::sendShortMessage(const QByteArray &message, const QByteArray &phoneNum)
{
    m_sendMsgFlag = true;
    m_sendMsg = message;
    m_recvPhone = phoneNum;
    sendMsg(message, phoneNum);
}

void Dial3G::sendMsg(const QByteArray &message, const QByteArray &phoneNum)
{
    RTU_DEBUG << "sendShortMessage" << message.size() << phoneNum.size();
    if (message.size() <= 0 || phoneNum.size() != 11) {
        return;
    }
    char read[255];
    int readLen = 0;
    // 发送短信为文本模式
    QByteArray megMode("AT+CMGF=0\r");
    m_serialPort->write(megMode.data(), megMode.size());
    QThread::sleep(1);
    readLen = m_serialPort->read(read,255);
    RTU_DEBUG << QByteArray(read, readLen);
    if (!QByteArray(read, readLen).contains("OK")) {
        return;
    }
    // 确定字符集
    QByteArray cscsMode("AT+CSCS=\"UCS2\"\r");
    m_serialPort->write(cscsMode.data(), cscsMode.size());
    QThread::sleep(1);
    readLen = m_serialPort->read(read,255);
    RTU_DEBUG<< QByteArray(read, readLen);
    if (!QByteArray(read, readLen).contains("OK")) {
        return;
    }
    // 设置sms模式
    QByteArray smsMode("AT+SMSMODE=1\r");
    m_serialPort->write(smsMode.data(), smsMode.size());
    QThread::sleep(1);
    readLen = m_serialPort->read(read,255);
    RTU_DEBUG<< QByteArray(read, readLen);
    if (!QByteArray(read, readLen).contains("OK")) {
        return;
    }
    // 设置GSMPDU使能
    QByteArray gsmPduEn("AT+GSMPDUEN=1\r");
    m_serialPort->write(gsmPduEn.data(), gsmPduEn.size());
    QThread::sleep(1);
    readLen = m_serialPort->read(read,255);
    RTU_DEBUG<< QByteArray(read, readLen);
    if (!QByteArray(read, readLen).contains("OK")) {
        return;
    }
    QString msg = strToUnicode(message);
    int msgLen = msg.size() / 2;
    int tpduLen = 15 + msgLen;
    RTU_DEBUG << tpduLen << msg.size() << message.size();
    QByteArray tpduNum = QString::number(tpduLen).toLatin1();
    // 设置pdu的长度
    QByteArray cmdPhoneNum = QByteArray("AT+CMGS=\r");
    cmdPhoneNum.insert(8, tpduNum.data());
    RTU_DEBUG<<cmdPhoneNum;
    m_serialPort->write(cmdPhoneNum.data(), cmdPhoneNum.size());
    QThread::sleep(1);
    // 短信内容
    QByteArray messageCmd = "0891683108100005F011000D9168";
    QByteArray pduPhoneNum = phoneNumToPdu(phoneNum);
    messageCmd.append(pduPhoneNum);
    QByteArray pid = "0008B0";
    messageCmd.append(pid);
    QByteArray msgNumHex = QString::number(msgLen, 16).toLatin1();
    // 短信长度未超过16时，转换为16进制时为0-F，前面必须加上0才是一个字节；
    if (msgLen < 16) {
        msgNumHex.insert(0, "0");
    }
    messageCmd.append(msgNumHex);
    messageCmd.append(msg.toLatin1());
    messageCmd.append(0x1a);
    RTU_DEBUG<<messageCmd.size();
    RTU_DEBUG<<messageCmd;
    m_serialPort->write(messageCmd.data(), messageCmd.size());
    QThread::sleep(1);
    readLen = m_serialPort->read(read,255);
    RTU_DEBUG<< QByteArray(read, readLen);
}

bool Dial3G::sendTextMsg(const QByteArray &message, const QByteArray &phoneNum)
{
    RTU_DEBUG << "sendShortMessage" << message.size() << phoneNum.size();
    if (message.size() <= 0 || phoneNum.size() != 11) {
        return false;
    }
    char read[255];
    int readLen = 0;
    // 发送短信为文本模式
    QByteArray megMode("AT+CMGF=1\r");
    m_serialPort->write(megMode.data(), megMode.size());
    readLen = m_serialPort->read(read,255);
    if (!QByteArray(read, readLen).contains("OK")) {
        return false;
    }
    // 确定字符集
    QByteArray cscsMode("AT+CSCS=\"GSM\"\r");
    m_serialPort->write(cscsMode.data(), cscsMode.size());
    readLen = m_serialPort->read(read,255);
    if (!QByteArray(read, readLen).contains("OK")) {
        return false;
    }
    // 设置短信接收电话号码
    QByteArray smsMode("AT+CMGS=\"15328241869\"\r");
    m_serialPort->write(smsMode.data(), smsMode.size());
    readLen = m_serialPort->read(read,255);
    RTU_DEBUG<< QByteArray(read, readLen);
    if (!QByteArray(read, readLen).contains(">")) {
        return false;
    }
    // 短信内容
    QByteArray messageCmd = "TEST";
    messageCmd.append(0x1a); // 添加短信结束符<ctrl-Z>
    m_serialPort->write(messageCmd.data(), messageCmd.size());
    QThread::sleep(1);
    readLen = m_serialPort->read(read,255);
    if (!QByteArray(read, readLen).contains("OK")) {
        return false;
    }
    return true;
}

QString Dial3G::strToUnicode(const QString &str)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QString name = codec->toUnicode(str.toUtf8().data());
    QString strout;
    for (int i=0; i<name.length(); ++i)
    {
        ushort num = name[i].unicode();
        if (num < 255)
            strout += "00";

        strout += QString::number(num,16);

    }
    return strout;
}

QByteArray Dial3G::phoneNumToPdu(QByteArray phoneNum)
{
    if ((phoneNum.size() % 2) == 1) {
        phoneNum.append('F');
    }
    for (int i = 0; i < phoneNum.size(); i += 2) {
        char temp = phoneNum.data()[i];
        phoneNum.data()[i] = phoneNum.data()[i + 1];
        phoneNum.data()[i + 1] = temp;
    }
    return phoneNum;
}

// 打开回显模式
void Dial3G::initATE()
{
    m_sendBuf[0] = 'A';
    m_sendBuf[1] = 'T';
    m_sendBuf[2] = 'E';
    m_sendBuf[3] = '0';
    m_sendBuf[4] = '\r';
    m_sendBuf[5] = '\n';

    int sendLen = 6;
    int recvLen = 6;

    bool flag = (m_serialPort->write(m_sendBuf, sendLen, m_recvBuf, recvLen) != VirtualPort::SUCCESS);

    if(flag)
    {
        RTU_DEBUG << "3G send cmd error";
    }

    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString recvCmdStr = "[" + datetime + "] Recved: ";
    for(int i=0; i<recvLen; i++) {
        recvCmdStr += m_recvBuf[i];
        RTU_DEBUG << "I = " <<i<<", " << m_recvBuf[i];
    }

    RTU_DEBUG<<recvCmdStr;
}

// 获取当前4G模块中是否有SIM卡
bool Dial3G::refreshSysInfo()
{
    m_sendBuf[0] = 'A';
    m_sendBuf[1] = 'T';
    m_sendBuf[2] = '+';
    m_sendBuf[3] = 'C';
    m_sendBuf[4] = 'I';
    m_sendBuf[5] = 'M';
    m_sendBuf[6] = 'I';
    m_sendBuf[7] = '=';
    m_sendBuf[8] = '?';
    m_sendBuf[9] = '\r';
    m_sendBuf[10] = '\n';

    int sendLen = 11;
    int recvLen = 6;

    QString sendStr = "";
    for(int i = 0; i < sendLen; i++)
        sendStr += m_sendBuf[i];

    RTU_DEBUG << "Send Cmd = " << sendStr;
    bool flag = (m_serialPort->write(m_sendBuf, sendLen, m_recvBuf, recvLen) != VirtualPort::SUCCESS);

    if(flag)
    {
        RTU_DEBUG << "3G send cmd error";
    }

    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    //QString recvCmd = "[" + datetime + "] Recved: ";
    QString recvCmdStr = "[" + datetime + "] Recved: ";
    for(int i=0; i<recvLen; i++) {
        //recvCmd += QString::number(m_recvBuf[i] & 0xFF, 16) + " ";
        recvCmdStr += m_recvBuf[i];
        //RTU_DEBUG << "I = " <<i<<", " << m_recvBuf[i];
    }

    RTU_DEBUG<<recvCmdStr;

    if ((m_recvBuf[2] == 'O') && (m_recvBuf[3] == 'K')) {
        m_simState = true;
        m_simStateStr = "Effective USIM card";
    } else {
        m_simState = false;
        m_simStateStr = "USIM card not exist";
    }

    //RTU_DEBUG << "Sim state = " << simState;

    //int: 0, invalid simcard; 1, valid simcard; 255, no simcard

    RTU_DEBUG << "[SIM Status] = " << m_simStateStr;
    return m_simState;
}

// 获取信号质量
// 返回值：+CSQ: 27,59 OK
// 第一个参数必须大于13时，才能拨号成功
void Dial3G::refresh3GBar()
{
    m_sendBuf[0] = 'A';
    m_sendBuf[1] = 'T';
    m_sendBuf[2] = '+';
    m_sendBuf[3] = 'C';
    m_sendBuf[4] = 'S';
    m_sendBuf[5] = 'Q';
    m_sendBuf[6] = '\r';
    m_sendBuf[7] = '\n';

    int sendLen = 8;
    int recvLen = 20;

    bool flag = (m_serialPort->write(m_sendBuf, sendLen, m_recvBuf, recvLen) != VirtualPort::SUCCESS);

    if(flag)
    {
        RTU_DEBUG << "3G send cmd error";
    }

    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString recvCmdStr = "";
    for(int i=0; i<recvLen; i++) {
        //recvCmd += QString::number(m_recvBuf[i] & 0xFF, 16) + " ";
        recvCmdStr += m_recvBuf[i];
        //RTU_DEBUG << "I = " <<i<<", " << m_recvBuf[i];
    }

    //RTU_DEBUG << "Received: " << recvCmdStr;

    //Find Rxlevel Sub-str
    int first = recvCmdStr.indexOf(',');
    int last = recvCmdStr.indexOf(first, '\r');
    RTU_DEBUG << first << " " << last;
    
    //RTU_DEBUG << "First = " << first << ", last = " << last;
    //RTU_DEBUG << "Rxlevel sub-str = " << recvCmdStr.mid(first + 1, last - first - 1);

    int rxlevel = 113;
    if(first != -1 && last > first)
        rxlevel = recvCmdStr.mid(first + 1, last - first - 1).toInt();

    if(rxlevel >= 109)
        m_3gBar = 0;
    else if(rxlevel < 61)
        m_3gBar = 5;
    else
        m_3gBar = 4 - (rxlevel - 61) / 12;

    RTU_DEBUG << "Rxlevel = -" << rxlevel << ", bar = " << m_3gBar;
}

// 查询网络的注册信息
// 返回值：\r\n+CREG: 0,3\r\n\r\nOK\r\n
// 第一个参数：0---禁用网络注册非请求结果码；
//           1---启用网络注册非请求结果码+CREG
// 第二个参数：0---未注册，没有搜索到新运营商
//           1---已注册，本地网
//           2---未注册，目前还在搜索中
//           3---注册被拒绝
void Dial3G::refreshCREG()
{
    m_sendBuf[0] = 'A';
    m_sendBuf[1] = 'T';
    m_sendBuf[2] = '+';
    m_sendBuf[3] = 'C';
    m_sendBuf[4] = 'R';
    m_sendBuf[5] = 'E';
    m_sendBuf[6] = 'G';
    m_sendBuf[7] = '?';
    m_sendBuf[8] = '\r';
    m_sendBuf[9] = '\n';

    int sendLen = 10;
    int recvLen = 20;

    bool flag = (m_serialPort->write(m_sendBuf, sendLen, m_recvBuf, recvLen) != VirtualPort::SUCCESS);

    if (flag) {
        m_regStatus = kNotRegistered;
        m_creg = false;
        RTU_DEBUG << "3G send cmd error";
    } else {
        int number1 = m_recvBuf[9] - '0';
        int number2 = m_recvBuf[11] - '0';

        m_regStatus = number2;

        if(number1 == 0 && number2 == 1)
            m_creg = true;
        else
            m_creg = false;
    }

    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    //QString recvCmd = "[" + datetime + "] Recved: ";
    QString recvCmdStr = "REG CMD [" + datetime + "] Recved: ";
    for(int i=0; i<recvLen; i++) {
        //recvCmd += QString::number(m_recvBuf[i] & 0xFF, 16) + " ";
        recvCmdStr += m_recvBuf[i];
        //RTU_DEBUG << "I = " <<i<<", " << m_recvBuf[i];
    }

    RTU_DEBUG<<recvCmdStr;
}

void Dial3G::refreshDialState()
{
    m_dialState = false;
    m_dialFlag = false;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for(int i = 0; i < list.size(); i++)
    {
        RTU_DEBUG << "Interface[" << i << "] = " << list.at(i).name() << ", addr = " << list.at(i).hardwareAddress();
        if(list.at(i).name() == "ppp0")
        {
            m_dialState = true;
            m_dialFlag = true;
            m_dialFailureCnt = 0;
            break;
        }
    }
    RTU_DEBUG << "DialState: " << m_dialState;
}

// 4G模块拨号上网
void Dial3G::dial3G()
{
    /*
    //Kill exist pppd
    QProcess cmd;
    cmd.start("killall pppd");
    cmd.waitForStarted();
    cmd.waitForFinished();
    */

    //m_process->start("sudo nohup pppd call gprs");

    RTU_DEBUG << "BEGIN DIAL PROCESS";
    m_process->start("pppd call ls-gprs");

    RTU_DEBUG << "WAIT DIAL PROCESS";
    m_process->waitForStarted();
    m_process->waitForFinished(15000);
    RTU_DEBUG << "END DIAL PROCESS";

    QString valueStr = QString::fromLocal8Bit(m_process->readAllStandardOutput());

    RTU_DEBUG << "RetStr: " << valueStr;

    // 关闭默认路由
    QProcess::execute("route del default");
    // 启动ppp0路由
    QProcess::execute("route add default dev ppp0");

//    m_process->close();
}

// 获取当前4G模块的工作模式
void Dial3G::readPhoneMode()
{
    m_sendBuf[0] = 'A';
    m_sendBuf[1] = 'T';
    m_sendBuf[2] = '+';
    m_sendBuf[3] = 'C';
    m_sendBuf[4] = 'F';
    m_sendBuf[5] = 'U';
    m_sendBuf[6] = 'N';
    m_sendBuf[7] = '?';
    m_sendBuf[8] = '\r';
    m_sendBuf[9] = '\n';

    int sendLen = 10;
    int recvLen = 18;

    bool flag = (m_serialPort->write(m_sendBuf, sendLen, m_recvBuf, recvLen) != VirtualPort::SUCCESS);

    if(flag)
    {
        RTU_DEBUG << "3G send cmd error";
    }

    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    //QString recvCmd = "[" + datetime + "] Recved: ";
    QString recvCmdStr = "[" + datetime + "] Recved: ";
    for(int i=0; i<recvLen; i++) {
        //recvCmd += QString::number(m_recvBuf[i] & 0xFF, 16) + " ";
        recvCmdStr += m_recvBuf[i];
        //RTU_DEBUG << "I = " <<i<<", " << m_recvBuf[i];
    }

    RTU_DEBUG << "FUN READ return = " << recvCmdStr;

    m_funStatus = m_recvBuf[9] - '0';

    RTU_DEBUG << "PHONE FUN CODE = " << m_funStatus;
}

// 切换4G模块的模式，
// 输入值：0----最小功能模式，射频收发处于关闭状态
//        1----重新打开收发注册网络到正常功能模式
//        4----进入飞行模式
//        6----进行重启
void Dial3G::writePhoneMode(int mode)
{
    m_sendBuf[0] = 'A';
    m_sendBuf[1] = 'T';
    m_sendBuf[2] = '+';
    m_sendBuf[3] = 'C';
    m_sendBuf[4] = 'F';
    m_sendBuf[5] = 'U';
    m_sendBuf[6] = 'N';
    m_sendBuf[7] = '=';

    m_sendBuf[8] = ('0' + mode) & 0xff;
    m_sendBuf[9] = '\r';
    m_sendBuf[10] = '\n';

    int sendLen = 11;
    int recvLen = 4;

    bool flag = (m_serialPort->write(m_sendBuf, sendLen, m_recvBuf, recvLen) != VirtualPort::SUCCESS);

    if(flag)
    {
        RTU_DEBUG << "3G send cmd error";
    }

    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString recvCmdStr = "[" + datetime + "] Recved: ";
    for(int i=0; i<recvLen; i++) {
        recvCmdStr += m_recvBuf[i];
    }

    RTU_DEBUG << "Write func set cmd = " << recvCmdStr;
}

