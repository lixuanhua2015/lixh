#ifndef MYARITHMETIC_H
#define MYARITHMETIC_H
#include <string.h>
#include <QSet>
#include <QByteArray>
#include <cstdlib>
#include <ctime>
#include <QFile>
#include <time.h>
#include <time.h>
#include <QTextCodec>
#include <QList>
#include <QVector>
#include "parambase.h"
void swapArr(char *srcArr, char *destArr);
template<class T>
void randomArr(T arr[], T maxValue, int len);
void insertSort(int *arr, const size_t &len);
void bubbleSort(int *arr, const size_t &len);
void selectionSort(int *arr, const size_t &len);
template<class T>
void printArr(T *arr, const size_t &len);
void loggingHandler(QtMsgType type, const QMessageLogContext &, const QString &str);
QString strToUnicode(const QString &str);
void makePartMatchValue(const char matchTable[], int next[]);
void makeNext(const char matchTable[], int next[]);
QList<int> kmp(const char bigTable[], const char matchTable[]);
int getMinChangeCount(char arr[]);
void changeAndCopyChar(const char arr[]);
int changeArr(char arr[], const char srcChar[], const char destArr[]);
void changeStarSite(char arr[], int algType);
void getChildArr(char arr[]);
bool isChildAnagram(const char baseArr[], const char childArr[]);
void reverseWords(char oldArr[]);
void reverseArr(char arr[], int n);
QList<int> getPartMin(int arr[], int len);
int firstMissPositive(int arr[], int len);
int getMaxDistanceInArr(int arr[], int len);
int singleNumberOne(int arr[], int len);
int singleNumberTwo(int arr[], int len);
/**
 * @brief swapArr 字符交换位置
 * @param srcArr 字符位置1
 * @param destArr 字符位置2
 */
void swapArr(char *srcArr, char *destArr)
{
    char temp = srcArr[0];
    srcArr[0] = destArr[0];
    destArr[0] = temp;
}
template<class T>
/**
 * @brief randomArr 随机数填充数组
 * @param arr 数组
 * @param maxValue 数组中最大值
 * @param len 长度
 */
void randomArr(T arr[], T maxValue, int len)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < len; ++i) {
        arr[i] = rand()%maxValue;
    }
}
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
        int j = 0;
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
template<class T>
void printArr(T *arr, const size_t &len)
{
    QString str;
    if (typeid (arr[0]) == typeid (int)) {
        str = "arr:[" + QString::number(arr[0]) + " ";
    } else if (typeid (arr[0]) == typeid (char)) {
        str = "arr:[" + QString(arr[0]);
    }
    for (size_t i = 1; i < len; ++i) {
        if (typeid (arr[0]) == typeid (int)) {
            str += QString::number(arr[i]) + " ";
        } else if (typeid (arr[0]) == typeid (char)) {
            str += QString(arr[i]);
        }
        if (i == len - 1) {
            str += "]";
        }
    }
    qDebug()<<str << len;
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

/**
 * @brief makePartMatchValue 获取部分匹配表的前缀和后缀的共有元素的个数
 * @param matchTable 匹配表
 * @param next 保存共有元素个数的数组
 */
void makePartMatchValue(const char matchTable[], int next[])
{
    next[0] = 0;
    size_t len = strlen(matchTable);
    QByteArray tableValue = QByteArray(matchTable, len);
    QSet<QByteArray> prfix;
    QSet<QByteArray> suffix;
    for (size_t i = 1; i < len; ++i) {
        QByteArray totalValue = QByteArray(matchTable, i + 1);
        QByteArray prfixBuf = "[";
        QByteArray suffixBuf = "[";
        for (size_t j = 0; j < i; ++j) {
            QByteArray prfixValue = tableValue.mid(0,j + 1);
            QByteArray suffixValue = tableValue.mid(i - j,j + 1);
            prfixBuf += (prfixValue + " ");
            suffixBuf += (suffixValue + " ");
            prfix.insert(prfixValue);
            suffix.insert(suffixValue);
        }
        prfixBuf += "]";
        suffixBuf += "]";
         QSet<QByteArray>::iterator begin = suffix.begin();
        for (size_t k = 0; k < i; ++k) {
            if (prfix.contains(begin->data()) && strlen(begin->data()) > next[i]) {
                next[i] = strlen(begin->data());
            }
            begin++;
        }
        suffix.clear();
        prfix.clear();
        qDebug() << totalValue << prfixBuf << suffixBuf << next[i];
    }
}

/**
 * @brief makeNext 生成next配置数组
 * @param matchTable 匹配表
 * @param next 配置数组
 */
void makeNext(const char matchTable[], int next[])
{
    int q,k;
    int m = strlen(matchTable);
    next[0] = 0;
    for (q = 1,k = 0; q < m; ++q)
    {
        while(k > 0 && matchTable[q] != matchTable[k])
            k = next[k-1];
        if (matchTable[q] == matchTable[k])
        {
            k++;
        }
        next[q] = k;
    }
}
/**
 * @brief kmp KMP算法
 * @param bigTable 大表
 * @param matchTable 匹配表
 * @param next 匹配数组
 * @return
 */
QList<int> kmp(const char bigTable[], const char matchTable[])
{
    int bigLen = strlen(bigTable);
    int matchLen = strlen(matchTable);
    int *next = new int[matchLen];
    makeNext(matchTable, next);
    QList<int> result;
    for (int i = 0, q = 0; i< bigLen; ++i) {
        while (q > 0 && matchTable[q] != bigTable[i]) {
            q = next[q - 1];
        }
        if (bigTable[i] == matchTable[q]) {
            q++;
        }
        if (q == matchLen) {
            result.append(i - matchLen + 1);
            RTU_DEBUG << result;
        }
    }
    delete [] next;
    return result;
}
/**
 * @brief getMinChangeCount 获取0-1字符串进行排序时，最少的交换次数
 * @param arr 0-1字符串
 * @return 最少的交换次数
 */
int getMinChangeCount(char arr[])
{
    int len = strlen(arr);
    int count = 0;
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        for (; i < j && arr[i] == '0'; ++i);
        for (; i < j && arr[j] == '1'; --j);
        if (i < j) {
            count++;
            char temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    return count;
}
/**
 * @brief changeAndCopyChar 删除字符串中的a，并复制所有的b
 * @param arr
 */
void changeAndCopyChar(const char arr[])
{
    int len = strlen(arr);
    const int denstLen = 2 * len;
    char *buf = new char[denstLen];
    printArr(arr, len);
    int bufLen = len;
    for (int i = 0,j = 0; i < len; ++i) {
        if (arr[i] != 'a') {
            buf[j++] = arr[i];
            if (arr[i] == 'b') {
                buf[j++] = 'b';
                bufLen++;
            }
        } else {
            bufLen--;
        }
    }
    printArr(buf, bufLen);
    delete [] buf;
}
/**
 * @brief changeArr 将字符串中某个字符替换成字符串
 * @param arr 原字符串
 * @param srcChar 原字符
 * @param destArr 替换后的字符串
 * @return 返回最终数组的长度
 */
int changeArr(char arr[], const char srcChar[], const char destArr[])
{
    QList<int> srcCharNum = kmp(arr, srcChar);
    RTU_DEBUG << srcCharNum.size();
    int srcLen = strlen(srcChar);
    int destArrLen = strlen(destArr);
    int arrLen = strlen(arr);
    int resultLen = arrLen + srcCharNum.size() * (destArrLen - srcLen);
    if (resultLen > arrLen) {
        for (int i = arrLen - 1, j = resultLen - 1; i >= 0; --i) {
            if ((srcCharNum.last() + srcLen - 1) == i) {
                for (int k = destArrLen - 1; k >= 0; --k) {
                    arr[j--] = destArr[k];
                }
                i -= (srcLen - 1);
                if (srcCharNum.size() > 1) {
                    srcCharNum.pop_back();
                }
            } else {
                arr[j--] = arr[i];
            }
        }
    } else {
        for (int i = 0, j = 0; i < arrLen; ++i) {
            if (srcCharNum.first() == i) {
                for (int k = 0; k < destArrLen; ++k) {
                    arr[j++] = destArr[k];
                }
                i += (srcLen - 1);
                if (srcCharNum.size() > 1) {
                    srcCharNum.pop_front();
                }
            } else {
                arr[j++] = arr[i];
            }
        }
    }
    return resultLen;
}
/**
 * @brief changeStarSite 一个字符串中只包含*和数字，将*号都放在开头，但是这个快排算法会改变数字之间的顺序
 * @param arr 字符串
 * @param algType 算法类型，0、1算法类型数字顺序会改变，2算法类型数字顺序不会改变
 */
void changeStarSite(char arr[], int algType)
{
    int len = strlen(arr);
    switch (algType) {
    case 0:
        // 方法一： 数字顺序发生了改变，*号的顺序没有改变
        for (int i = 0, j = len - 1; i < j; ++i, --j) {
            for (; i < j && arr[i] == '*'; ++i);
            for (; i < j && arr[j] != '*'; --j);
            if (i < j) {
                swapArr(arr + i, arr + j);
            }
        }
        break;
    case 1:
        // 方法二： 数字顺序发生了改变，*号的顺序没有改变
        for (int i = 0, j = 0; i < len; ++i) {
            if (arr[i] == '*') {
                swapArr(arr + i, arr + j++);
            }
        }
        break;
    case 2:
        // 方法三： 数字顺序不会改变，'*'号是顺序其实是改变了
        for (int i = len - 1, j = len - 1; i >= 0; --i) {
            if (arr[i] != '*') {
                swapArr(arr + i, arr + j--);
            }
        }
        break;
    default:
        break;
    }
}
/**
 * @brief getChildArr 获取字符串n个字符的子串，子串的个数=n*（n-1)/2 + 1,1代表空子串
 * @param arr 字符串
 */
void getChildArr(char arr[])
{
    int len =  strlen(arr);
    for (int i = 0; i < len; ++i) {
        for (int j = 1; j <= len - i; ++j) {
            RTU_DEBUG << QByteArray(arr + i,j);
        }
    }
}
/**
 * @brief isChildAnagram 判断是否为字符串(字符串内容为英文小写字母)的子串的变位词 滑动窗口
 * @param baseArr 字符串
 * @param childArr 判断的子串
 * @return  返回值是否为字符串子串的变位词
 */
bool isChildAnagram(const char baseArr[], const char childArr[])
{
    // 用来存储子串中每个应为字母出现的次数
    QVector<int> count(26, 0);
    int nonZero = 0;
    int baseArrLen = strlen(baseArr);
    int childAddLen = strlen(childArr);
    // 计算出子串中每个小写英文字母出现的次数，当一个英文字母出现一次，nonZero就加1
    for (int i = 0; i < childAddLen; ++i) {
        if (++count[childArr[i] - 'a'] == 1) {
            nonZero++;
        }
    }
    // 字符串中前childAddLen个字符与childArr匹配，将count中对应英文字母每出现一次就减一，当count为0时，说明baseArr和childArr在这个字母上已经匹配好了
    // 当count为-1时，说明baseArr在这个字母上比childArr多；
    for (int i = 0; i < childAddLen; ++i) {
        int index = baseArr[i] - 'a';
        count[index]--;
        if (count[index] == 0) {
            nonZero--;
        } else if (count[index] == -1) {
            nonZero++;
        }
    }
    // nonZero不等于0，说明前childAddLen字符不与childArr匹配
    if (nonZero == 0) {
        return true;
    }
    // 滑动窗口，窗口向后滑动一位，那么需要消除前一位带来的count上的变化，前面前一位都是减一，那么要消除count上的变化，那么现在
    // 向后移动一位，需要在前一位字符的count上加一，这样就消除了前一位带来的影响，消除变化后在校验count的值为0，说明在这个字母上
    // 两个完全匹配了，nonZero减一，如果count为1，说明baseArr在这个字母上比childArr多一个；最后一位也向后移动一位，当前的最后
    // 一位的字母，对应的count减一，后续操作和上面一致，每向后移动一位，最后要检测nonZero是否为0，若为0就说明找到了childArr的变位词；
    for (int i = childAddLen; i < baseArrLen; ++i) {
        int index = baseArr[i - childAddLen] - 'a';
        count[index]++;
        if (count[index] == 0) {
            nonZero--;
        } else if (count[index] == 1) {
            nonZero++;
        }
        index = baseArr[i] - 'a';
        count[index]--;
        if (count[index] == 0) {
            nonZero--;
        } else if (count[index] == -1) {
            nonZero++;
        }
        if (nonZero == 0) {
            return true;
        }
    }
    return false;
}
/**
 * @brief reverseWords 翻转句子中全部的单词，单词内容不变
 * @param oldArr 需要翻转的句子
 */
void reverseWords(char oldArr[])
{
    int len = strlen(oldArr);
    int i = 0;
    int j = len - 1;
    QList<int> indexInOldArr;
    // 将首尾的空格放入list中
    indexInOldArr.push_back(i - 1);
    indexInOldArr.push_back(len);
    // 首先找到将整个句子翻转，找到翻转后空格所在的位置，放入list列表中。
    while (i < j) {
        if (oldArr[i] == ' ') {
            indexInOldArr.push_back(j);
        }
        if (oldArr[j] == ' ') {
            indexInOldArr.push_back(i);
        }
        swapArr(&oldArr[i++], &oldArr[j--]);
    }
    qSort(indexInOldArr);
    RTU_DEBUG << indexInOldArr;
    QList<int>::iterator begin = indexInOldArr.begin();
    // 根据空格，将每个单词再翻转，就恢复单词正常顺序；
    for (int k = 0; k < indexInOldArr.size() - 1; ++k, ++begin) {
        i = *begin + 1;
        j = *(begin + 1) - 1;
        while (i < j) {
            swapArr(&oldArr[i++], &oldArr[j--]);
        }
    }
}
/**
 * @brief reverseArr 字符串长度m，前n%m翻转，后m-n%m翻转，最后整体翻转,其实就是字符串向前移动n位；
 * @param arr 字符串
 * @param n 翻转的位置
 */
void reverseArr(char arr[], int n)
{
    int len = strlen(arr);
    printArr(arr, len);
    if (n > len) {
        return;
    }
    int i = 0;
    int j = n - 1;
    while (i < j) {
        swapArr(arr + i++, arr + j--);
    }
    printArr(arr, len);
    i = n;
    j = len - 1;
    while (i < j) {
        swapArr(arr + i++, arr + j--);
    }
    printArr(arr, len);
    i = 0;
    j = len - 1;
    while (i < j) {
        swapArr(arr + i++, arr + j--);
    }
    printArr(arr, len);
}
/**
 * @brief getPartMin 获取数组中所有的局部最小值
 * @param arr 数组
 * @return 返回所有的局部最小值
 */
QList<int> getPartMin(int arr[], int len)
{
    QList<int> res;
    if (len < 0) {
        return res;
    } else if (len == 1) {
        res.push_back(arr[0]);
    } else if (len == 2) {
        res.push_back(arr[0] > arr[1] ? arr[1] : arr[0]);
    } else if (len >= 3) {
        if (arr[0] < arr[1]) {
            res.push_back(arr[0]);
        }
        for (int i = 1; i < len - 1; ++i) {
            if ((arr[i] < arr[i - 1]) && (arr[i] < arr[i + 1])) {
                res.push_back(arr[i]);
            }
        }
        if (arr[len - 1] < arr[len - 2]) {
            res.push_back(arr[len - 1]);
        }
    }
    return res;
}
/**
 * @brief firstMissPositive 找到数组中缺失的第一个正整数
 * @param arr 数组
 * @param len 数组长度
 * @return  第一个缺失的正整数
 */
int firstMissPositive(int arr[], int len)
{
    if (len == 0) {
        return 1;
    }
    QVector<int> buf(len, 0);
    for (int i = 0; i < len; ++i) {
        if (arr[i] > 0 && arr[i] <= len) {
            buf[arr[i] - 1]++;
        }
    }
    for (int i = 0; i < len; i++) {
        if (buf[i] == 0) {
            return i + 1;
        }
    }
    return len + 1;
}
/**
 * @brief getMaxDistanceInArr 获取数组中元素间最大间距离
 * @param arr 数组
 * @param len 长度
 * @return 最大间距离
 */
int getMaxDistanceInArr(int arr[], int len)
{
    if (len <= 0) {
        return 0;
    }
    int maxInt = std::numeric_limits<int>::max();
    int minInt = std::numeric_limits<int>::min();
    int maxInArr = arr[0];
    int minInArr = arr[0];
    // 找到数组中的最大值和最小值
    for (int i = 1; i < len; ++i) {
        if (arr[i] < minInArr) {
            minInArr = arr[i];
        }
        if (arr[i] > maxInArr) {
            maxInArr = arr[i];
        }
    }
    if (maxInArr == minInArr) {
        return 0;
    }
    struct TempBuf{
        int maxInBuf;
        int minInBuf;
    };
    // 将[minInArr,minInArr]平均分为len + 1个区间，初始化每个区间的最大值为minInt，最小值为maxInt，
    // 方便后续数组中值插入区间是比较更新区间maxInBuf和minInBuf，len个数据插入到len + 1个区间中，
    // 至少有一个区间是空区间（没有元素在区间范围），最大间距一定在空区间的地方产生；最大间距一定
    // 会跨越一个区间；所以避免考虑计算同一个区间中的元素，减少工作量
    TempBuf *buf = new TempBuf[len + 1];
    for (int i = 0; i < len + 1; ++i) {
        buf[i].maxInBuf = minInt;
        buf[i].minInBuf = maxInt;
    }
    // 将数组中每个值插入到对应的区间中，并更新区间的maxInBuf和minInBuf
    for (int i = 0; i < len; ++i) {
        int index = (double)(arr[i] - minInArr) / (maxInArr - minInArr) * (len + 1);
        index = index == (len + 1) ? len : index;
        if (arr[i] < buf[index].minInBuf) {
            buf[index].minInBuf = arr[i];
        }
        if (arr[i] > buf[index].maxInBuf) {
            buf[index].maxInBuf = arr[i];
        }
    }
    int maxDistance = 0;
    int indexLast = 0;
    int indexTemp = 0;
    // 检查每个区间是否有元素，如果maxInBuf和minInBuf为初始值，说明为空区间；
    // 用indexLast记录上一个区间的角标；如果下一个区间为空区间，继续检查下一
    // 个，直到不为空区间时；不为空区间时用该区间的最小值减去indexLast区间的
    // 最大值得到最大间距，最大间距（maxDistance（和上一个最大间距比较，保留
    // 最大的值，并将indexLast更新为当前区间，这样找到的maxDistance为最大间距。
    for (int i = 1; i < len + 1; ++i) {
        if (buf[i].maxInBuf != minInt) {
            int tempMaxDis = buf[i].minInBuf - buf[indexLast].maxInBuf;
            if (tempMaxDis > maxDistance) {
                maxDistance = tempMaxDis;
                indexTemp = buf[indexLast].maxInBuf;
            }
            indexLast = i;
        }
    }
    RTU_DEBUG << indexTemp;
    delete [] buf;
    return maxDistance;
}
/**
 * @brief singleNumberOne 给定一个非空整数数组，除了某个元素只出现一次以外，其余每个元素均出现两次。找出那个只出现了一次的元素。
 * @param arr 数组
 * @param len 长度
 * @return 返回只出现一次的元素
 */
int singleNumberOne(int arr[], int len)
{
    int mask = 0;
    for (int i = 0; i < len; ++i) {
        mask ^= arr[i];
    }
    return mask;
}
/**
 * @brief singleNumberTwo 给定一个非空整数数组，除了某个元素只出现一次以外，其余每个元素均出现了三次。找出那个只出现了一次的元素。
 * @param arr 数组
 * @param len 长度
 * @return 返回只出现一次的元素
 */
int singleNumberTwo(int arr[], int len)
{
    int a = 0;
    int b = 0;
    for (int i = 0; i < len; ++i){
        a = (a ^ arr[i]) & ~b;
        b = (b ^ arr[i]) & ~a;
    }
    return a;
}
#endif // MYARITHMETIC_H
