﻿#ifndef MYARITHMETIC_H
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
#include <stack>
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
QVector<int> singleNumberThree(int arr[], int len);
QVector<int> twoSum(int arr[], int len ,int target);
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2);
int lengthOfLongestSubstring(QString str);
double findMedianSortedArrays(QVector<int>& nums1, QVector<int>& nums2);
string longestPalindrome(string s);
int reverse(int x);
int maxArea(vector<int>& height);
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
/**
 * @brief singleNumberThree 给定一个整数数组 nums，其中恰好有两个元素只出现一次，其余所有元素均出现两次。 找出只出现一次的那两个元素。
 * @param arr 数组
 * @param len 长度
 * @return 返回只出现一次的两个元素
 */
QVector<int> singleNumberThree(int arr[], int len)
{
    int mask = 0;
    // 得到只出现一次的两个元素的异或结果
    for (int i = 0; i < len; ++i) {
        mask ^= arr[i];
    }
    // 解析异或结果，得到两个元素转换为二进制时，从低位到高位计算，第一次出现bit位上数字不一样的位置，若bit位0不一样，结果为1；
    // bit位1不一样，结果为2；bit为2不一样，结果为4...
    // 也可以写成：mask &= (~mask) + 1
    mask &= -mask;
    QVector<int> result(2, 0);
    // 最新mask值的意义：mask为1的bit位上，两个元素在相同bit位上数字不一样；遍历每个元素&mask，肯定能找到该bit位上为1的元素，
    // 其他元素都是出现两次，而且这两个元素在该bit为不一样，那么result[0]和result[1]的元素个数都为单数，而且最终结果就是两个元素。
    // 计算出最新mask，主要是区分两个元素，找到两个元素从低到高位第一个bit位上数字不一样的bit位;
    for (int i = 0; i < len; ++i) {
        if ((mask & arr[i]) == mask) {
            result[0] ^= arr[i];
        } else {
            result[1] ^= arr[i];
        }
    }
    return result;
}
/**
 * @brief twoSum 给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的那两个整数，并返回他们的数组下标。
 * @param arr 数组
 * @param len 长度
 * @param target 和
 * @return 两个元素的下标
 */
QVector<int> twoSum(int arr[], int len ,int target)
{
    QHash<int, int> temp;
    QVector<int> result(2, 0);
    for (int i = 0; i < len; ++i) {
        int value = target - arr[i];
        if (temp.contains(value)) {
            result[1] = i;
            result[0] = temp[value];
            break;
        }
        temp[arr[i]] = i;
    }
    return result;
}
/**
 * @brief addTwoNumbers 给出两个 非空 的链表用来表示两个非负的整数。其中，它们各自的位数是按照 逆序 的方式存储的，并且它们的每个节点只能存储 一位 数字。
                        如果，我们将这两个数相加起来，则会返回一个新的链表来表示它们的和。
 * @param l1 链表1
 * @param l2 链表2
 * @return 返回和的链表
 */
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode *temp1 = l1;
    ListNode *temp2 = l2;
    int a = 0;
    ListNode *lastTemp = NULL;
    ListNode *result = NULL;
    // 延用链表1和链表2，减少内存的开销。
    while(temp1 != NULL ||  temp2 != NULL) {
        int b = temp1 != NULL ? temp1->val : 0;
        int c = temp2 != NULL ? temp2->val : 0;
        int sum = a + b + c;
        a = sum / 10;
        int d = sum % 10;
        if (temp1 != NULL) {
            temp1->val = d;
            lastTemp = temp1;
            result = l1;
            temp1 = temp1->next;
        }
        if (temp2 != NULL) {
            temp2->val = d;
            lastTemp = temp2;
            result = l2;
            temp2 = temp2->next;
        }
    }
    if (a == 1) {
        lastTemp->next = new ListNode(a);
    }
    return result;
}
/**
 * @brief lengthOfLongestSubstring 无重复字符的最长子串
 * @param str 字符串
 * @return  最长子串的长度
 */
int lengthOfLongestSubstring(QString str)
{
    int len = str.size();
    int start = 0;
    int length = 0;
    int result = 0;
    QMap<int, int> arr;
    for (int i = 0; i < len; ++i) {
        int num = QString(str[i]).toLatin1().data()[0] & 0xff;
        if (arr.find(num) != arr.end() && start <= arr[num]) {
            start = arr[num] + 1;
            length = i - start;
        }
        arr[num] = i;
        length++;
        result = result > length ? result : length;
    }
    return result;
}
/**
 * @brief findMedianSortedArrays 寻找两个有序数组中的中位数
 * @param nums1 有序数组1
 * @param nums2 有序数组2
 * @return 返回的中位数
 */
double findMedianSortedArrays(QVector<int>& nums1, QVector<int>& nums2)
{
    int len1 = nums1.size() - 1; // 数组1从后往前遍历的变量
    int len2 = nums2.size() - 1; // 数组2从后向前遍历的变量
    int i = 0; // 数组1从前向后遍历的变量
    int j = 0; // 数组2从前向后遍历的变量
    int minA = 0; // 记录计算中位数时两个值中的最小值
    int maxA = 0; // 记录计算中位数时两个值中的最大值
    // 思路：用minA记录两个数组中最小值，maxA记录两个数组中最大值；然后继续找第二最小值和第二最大值。。。
    // 直到（i > len1 && j > len2），表示两个数组都遍历完了；当minA为数组1中的元素时i++，为数组2中的元素时j++；
    // 当maxA为数组1中的元素时len1--；为数组2中的元素时len2--；最终中位数为minA和maxA的平均值。
    while (i <= len1 || j <= len2) {
        if (i <= len1 && j <= len2) {
            minA = nums1[i] < nums2[j] ? nums1[i++] : nums2[j++];
            maxA = nums1[len1] > nums2[len2] ? nums1[len1--] : nums2[len2--];
        } else {
            if (i <= len1) {
                minA = nums1[i++];
                maxA = nums1[len1--];
            }
            if (j <= len2) {
                minA = nums2[j++];
                maxA = nums2[len2--];
            }
        }
    }
    double res = (double)(minA + maxA) / 2;
    return res;
}
/**
 * @brief getLen 获取回文子串的长度
 * @param s 字符串
 * @param l 左起点
 * @param r 右起点
 * @return 回文子串的长度
 */
int getLen(string s, int l, int r)
{
    int len = s.size();
    while (l >= 0 && r < len && s[r] == s[l]) {
        l--;
        r++;
    }
    return r - l - 1;
}
/**
 * @brief longestPalindrome 最长回文子串
 * @param s 字符串
 * @return 返回最长回文子串
 */
string longestPalindrome(string s)
{
    // 思路：回文中心的两侧互为镜像。因此，回文可以从他的中心展开，并且只有 2n-1 个
    // 这样的中心（一个元素为中心的情况有 n 个，两个元素为中心的情况有 n-1 个）
    int len = s.size();
    string res = "";
    int index = 0;
    int first = 0;
    for (int i = 0; i < len; ++i) {
        int len1 = getLen(s, i, i);
        int len2 = getLen(s, i, i + 1);
        int maxLen = len1 > len2 ? len1 : len2;
        if (maxLen > index) {
            index = maxLen;
            first = i - (maxLen - 1) / 2;
        }
    }
    res = s.substr(first, index);
    return res;
}
/**
 * @brief reverse 整数反转
 * @param x 输入的整数
 * @return 反转后的整数
 */
int reverse(int x)
{
    int maxInt = std::numeric_limits<int>::max();
    int minInt = std::numeric_limits<int>::min();
    long long res = 0;
    while (x != 0) {
        res = res * 10 + x % 10;
        x = x / 10;
    }
    if (res > maxInt || res < minInt) {
        res = 0;
    }
    return res;
}
int myAtoi(string str)
{
    int len = str.size();
    long long res = 0;
    bool flag = false;
    int maxInt = std::numeric_limits<int>::max();
    int minInt = std::numeric_limits<int>::min();
    for (int i = 0; i < len; ++i) {
        if (str[i] != ' ') {
            while(true) {
                if (str[i] == '-') {
                    flag = true;
                } else if (str[i] >= '0' && str[i] <= '9') {
                    if (flag) {
                        res = res * 10 - (str[i] - '0');
                        if (res < minInt) {
                            res = minInt;
                            break;
                        }
                    } else {
                        res = res * 10 + (str[i] - '0');
                        if (res > maxInt) {
                            res = maxInt;
                            break;
                        }
                    }
                } else {
                    break;
                }
                i++;
            }
            break;
        }
    }
    return res;
}
/**
 * @brief maxArea leetcode 11 盛最多水的容器
 * @param height 数组
 * @return 返回最大面积
 */
int maxArea(vector<int>& height)
{
    // 思路：用两个指针从两边向中间逼近，直到两个指针相交，每次取两者指向元素小的那一个作为桶高，
    // 如果左边（k）小，计算出面积，然后向右依次遍历，直到找到比k角标上值大的，然后再继续后续操作
    int begin =0;
    int end = height.size() -1;
    int max = 0;
    int temp = 0;
    int k = 0;
    while (begin < end) {
        max = max > temp ? max : temp;
        if (height[begin] > height[end]) {
            k = end;
            temp = height[end] * (end - begin);
            // 找到比height[k]大的值，并且end必须大于begin
            do {
                end--;
            }while(height[end] <= height[k] && end > begin);
        } else {
            k = begin;
            temp = height[begin] * (end - begin);
            // 找到比height[k]大的值，并且end必须大于begin
            do {
                begin++;
            }while(height[begin] <= height[k] && begin < end);
        }
    }
    max = max > temp ? max : temp;
    return max;
}
/**
 * @brief intToRoman leetcode 12 整数（1-3999）转罗马数字
 * @param num 整数
 * @return 罗马数字
 */
string intToRoman(int num)
{
    // 直接按照每一位0~10的情况存进字符数组里，遍历四位就行了
    // 这是一种投机的做法，如果数值很大时，在创建表的时候就很麻烦。
    char* c[4][10] = {
        {"","I","II","III","IV","V","VI","VII","VIII","IX"},
        {"","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"},
        {"","C","CC","CCC","CD","D","DC","DCC","DCCC","CM"},
        {"","M","MM","MMM"}
    };
    string roman;
    roman.append(c[3][num / 1000]);
    roman.append(c[2][num / 100 % 10]);
    roman.append(c[1][num / 10 % 10]);
    roman.append(c[0][num % 10]);

    return roman;
}
/**
 * @brief studyStack 学习stack栈，先进后出，例如汽车开入死胡同，先开入的汽车最后才能出来。FILO
 */
void studyStack()
{
    // 空栈
    stack<int> intStack;
    // intStack保存0-9十个数
    for (int i = 0; i <= 10; ++i) {
        intStack.push(i);
    }
    while (!intStack.empty()) {
        // 获取栈最上面的值
        int value = intStack.top();
        RTU_DEBUG << value;
        // 弹出栈顶部的值
        intStack.pop();
    }
}

#endif // MYARITHMETIC_H
