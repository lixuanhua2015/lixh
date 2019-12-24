#include <QCoreApplication>
#include "databasemanager.h"
#include <cstdlib>
#include <ctime>
#include <QFile>
#include <time.h>
#include <QTextCodec>

/*插入排序
 * 选择一个比较的基准数temp，从数组角标1一直遍历到结束；
 * 然后用temp与temp前面的元素比较，从temp前面第一个元素遍历到数组第一个元素；
 * 如果比较的元素大于temp，那么比较的元素在数组中就向后移一位，这时比较的元素的位置上就空出来一个位置
 * 直到找到小于temp的元素，然后将空出来的位置赋值为temp；
 * 或者一直遍历到第一个元素也没有找到比temp小的元素时，将第一个元素的位置上赋值为temp；
 * 一直遍历到结束及完成了插入排序
 */
/**
 * @brief insertSort 插入排序
 * @param arr 数组
 * @param len 长度
 */
void insertSort(int *arr, const size_t &len)
{
    for (size_t i = 1; i < len; ++i) {
        int temp = arr[i];
        size_t j = 0;
        for (j = i - 1; j >= 0; --j) {
            if (arr[j] > temp) {
                arr[j + 1] = arr[j];
            } else {

                break;
            }
        }
        arr[j + 1] = temp;
    }
}
/**
 * @brief bubbleSort 冒泡排序，原理：每次从左到右两两比较，把大的交换到后面，每次可以
 *                   确保将前M个元素的最大值移动到最右边；
 * @param arr 数组指针
 * @param len 数组长度
 */
void bubbleSort(int *arr, const size_t &len)
{
    for (size_t i = len - 1; i > 0; --i) {
        for (size_t j = 0; j < i; ++j) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
/**
 * @brief selectionSort 选择排序：每次都从乱序数组中找到最大（最小）值，放到当前乱序数组头部，最终使数组有序
 * @param arr 数组指针
 * @param len 数组长度
 */
void selectionSort(int *arr, const size_t &len)
{
    for (size_t i = 0; i < len - 1; ++i) {
        size_t min = i;
        // 找到角标i后面数组中最小值的角标min
        for (size_t j = i + 1; j < len; ++j) {
            if (arr[j] < arr[min]) {
                min = j;
            }
        }
        // 将角标min上的值与角标i上的值交换位置
        int temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    }
}

/**
 * @brief printArr 输出数组中的数据
 * @param arr   数组指针
 * @param len   数组长度
 */
void printArr(int *arr, const size_t &len)
{
    QString str = "arr:[" + QString::number(arr[0]);
    for (size_t i = 1; i < len; ++i) {
        str += ", ";
        str += QString::number(arr[i]);
        if (i == len - 1) {
            str += "]";
        }
    }
    qDebug()<<str;
}

/**
 * @brief loggingHandler 日志QDebug输出至文件，联合qInstallMessageHandler(loggingHandler)使用
 * @param type
 * @param str
 */
void loggingHandler(QtMsgType type, const QMessageLogContext &, const QString &str)
{
    QFile logFile("./log");
    // 打开方式：只写并且是追加
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QByteArray localMsg = str.toLocal8Bit();
    const char *msg = str.toStdString().c_str();
    QTextStream textStream(&logFile);
    switch (type) {
    case QtDebugMsg:
        textStream << " Debug: " << localMsg.data() << endl;
        //NOP
        break;
    case QtWarningMsg:
        textStream << QTime::currentTime().toString().toLatin1().data() << " Warning: " << msg << endl;
        break;
    case QtCriticalMsg:
        textStream << QTime::currentTime().toString().toLatin1().data() << " Critical: " << msg << endl;
        break;
    case QtFatalMsg:
        textStream << QTime::currentTime().toString().toLatin1().data() << " Fatal: " << msg << endl;
        break;
    default:
        break;
    }
    logFile.flush();
    logFile.close();
    // 如果文件大小大于某个数值时，将会把该文件保存为另一个文件，然后继续输出
    if(logFile.size() >= 1024 * 30)
    {
        QFile tempFile("./log1");
        // 另存为一个文件时，如果这个文件存在就删除
        if (tempFile.exists()) {
            tempFile.remove();
        }
        logFile.rename("log1");
    }
}
/**
 * @brief strToUnicode 字符串转Unicode码
 * @param str 输入的字符串
 * @return
 */
QString strToUnicode(const QString &str)
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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DatabaseManager myDb("mydatabase.db");
    QString str = strToUnicode("你好125");
    qDebug() << str << str.size();
    return a.exec();
}
