#include <QCoreApplication>
#include "databasemanager.h"
#include <cstdlib>
#include <ctime>

/*插入排序
 * 选择一个比较的基准数temp，从数组角标1一直遍历到结束；
 * 然后用temp与temp前面的元素比较，从temp前面第一个元素遍历到数组第一个元素；
 * 如果比较的元素大于temp，那么比较的元素在数组中就向后移一位，这时比较的元素的位置上就空出来一个位置
 * 直到找到小于temp的元素，然后将空出来的位置赋值为temp；
 * 或者一直遍历到第一个元素也没有找到比temp小的元素时，将第一个元素的位置上赋值为temp；
 * 一直遍历到结束及完成了插入排序
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

void printArr(int *arr, const size_t &len)
{
    QString str = "arr:[" + QString::number(arr[0]);
    for (int i = 1; i < len; ++i) {
        str += ", ";
        str += QString::number(arr[i]);
        if (i == len - 1) {
            str += "]";
        }
    }
    qDebug()<<str;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DatabaseManager myDb("mydatabase.db");
//    int arr[20] = {0};
//    srand((int)time(0));
//    for (int i = 0; i < 20; ++i) {
//        arr[i] = rand() % 1000;
//    }
//    printArr(arr, 20);
//    insertSort(arr, 20);
//    printArr(arr, 20);
    float floatValue = 27.000000;
    char *chValue = (char*)(&floatValue);
    for (int i = 0; i < 4; ++i) {
        RTU_DEBUG << (chValue[i] & 0xff);
    }

    return a.exec();
}
