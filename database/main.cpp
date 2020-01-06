#include <QCoreApplication>
#include "databasemanager.h"
#include "myArithmetic.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DatabaseManager myDb("mydatabase.db");
    char baseArr[] = "abcdefghijklmnopqrstuvwxyz";
    char arr[] = "fgohi";
    printArr(baseArr, strlen(baseArr));
    printArr(arr, strlen(arr));
    RTU_DEBUG << isChildAnagram(baseArr, arr);

    return a.exec();
}
