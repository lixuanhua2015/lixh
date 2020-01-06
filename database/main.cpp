#include <QCoreApplication>
#include "databasemanager.h"
#include "myArithmetic.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DatabaseManager myDb("mydatabase.db");
    char arr[] = "1321**321*1321**654*465468**";
    printArr(arr, strlen(arr));
    changeStarSite(arr,2);
    printArr(arr, strlen(arr));
    char arr1[] = "1321**321*1321**654*465468**";
    printArr(arr1, strlen(arr1));
    changeStarSite(arr1,1);
    printArr(arr1, strlen(arr1));
    char arr2[] = "1321**321*1321**654*465468**";
    printArr(arr2, strlen(arr2));
    changeStarSite(arr2,0);
    printArr(arr2, strlen(arr2));
    return a.exec();
}
