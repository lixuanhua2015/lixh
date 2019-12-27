#ifndef DIAL3G_H
#define DIAL3G_H

#include <QObject>
#include <QThread>
#include <QProcess>
#include "parambase.h"
#include "serialport.h"

class Dial3G : public QObject
{
    Q_OBJECT
public:
    Dial3G();
    ~Dial3G();

    /**
     * @brief 设置拨号串口参数
     * @param portName: 串口号
     * @param timeout: 超时时间
     */
    void setPortParam(const QString &portName, int timeout);

    /**
     * @brief 打开串口
     * @return bool
     */
    bool openPort();

    /**
     * @brief 获取信号强度
     * @return int: 0~5
     */
    inline int get3GBar()
    {
        return m_3gBar;
    }

    /**
     * @brief Return currunt state byte
     * @return int:  Sim Card | Reg State | Online State
     */
    int getState();

    /**
     * @brief 获取SIM卡状态
     * @return bool
     */
    inline bool getSimState()
    {
        return m_simState;
    }

    /**
     * @brief 获取SIM卡状态信息
     * @return QString
     */
    inline QString getSimStateStr() const
    {
        return m_simStateStr;
    }

    /**
     * @brief 获取网络注册状态
     * @return bool
     */
    inline bool getCREGState()
    {
        return m_creg;
    }

    /**
     * @brief 拨号上网
     */
    void dial3G();

    /**
     * @brief 设置拨号上网标志
     * @param bool
     */
    inline void setDialFlag(bool f) 
    {
        m_dialFlag = f;
    }

    /**
     * @brief 获取拨号上网状态
     * @return bool
     */
    inline bool getDialState()
    {
        return m_dialState;
    }

public slots:
    void run();

    /**
     * @brief sendShortMessage  发短信功能
     * @param message 短信内容
     * @param phoneNum 接收短信的电话号码
     */
    void sendShortMessage(const QByteArray &message, const QByteArray &phoneNum);

    void sendMsg(const QByteArray &message, const QByteArray &phoneNum);
    /**
     * @brief sendTextMsg 发送text纯文本短信
     * @param message 短信内容
     * @param phoneNum 接收短信的电话号码
     * @return 返回是否发送短信成功
     */
    bool sendTextMsg(const QByteArray &message, const QByteArray &phoneNum);

signals:
    void refresh3GSig();

private:
    enum {
        kMinFunc = 0,
        kFullFunc = 1,
        kNotRegistered = 0,
        kRegistered = 1,
        kIsRegistering = 2
    };
    /**
    * @brief strToUnicode 字符串转Unicode码
    * @param str 输入的字符串
    * @return
    */
    QString strToUnicode(const QString &str);
    /**
     * @brief phoneNumToPdu 电话号码转换为pdu编码
     * @param phoneNum 电话号码
     * @return
     */
    QByteArray phoneNumToPdu(QByteArray phoneNum);
    bool refreshSysInfo();
    void refresh3GBar();
    void refreshCREG();
    void refreshDialState();

    void initATE();

    void readPhoneMode();
    void writePhoneMode(int mode);

    void restarting();

    serialPort* m_serialPort;

    //发送数据字符数组
    char m_sendBuf[64];

    //接收数据字符数组
    char m_recvBuf[64];

    int m_3gBar;

    QString m_simStateStr;
    bool m_simState;

    bool m_creg;

    bool m_dialFlag;
    bool m_dialState;
    QProcess *m_process;

    bool m_initFlag;

    int m_regStatus;
    int m_funStatus;

    //Count the failure count of dialing
    //If count > 5, enter fly mode
    int m_dialFailureCnt;
    bool m_sendMsgFlag;
    QByteArray m_sendMsg;
    QByteArray m_recvPhone;
};

#endif // DIAL3G_H
