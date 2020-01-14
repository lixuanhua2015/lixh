#include <QCoreApplication>
#include "databasemanager.h"
#include "myArithmetic.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DatabaseManager myDb("mydatabase.db");
    int arr[25] = {0};
    randomArr(arr, 100, 25);
    printArr(arr, 25);
    RTU_DEBUG << getMaxDistanceInArr(arr, 25);
    insertSort(arr, 25);
    printArr(arr, 25);

    return a.exec();
}
