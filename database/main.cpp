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
    QList<int> myList = getPartMin(arr, 25);
    RTU_DEBUG << myList << myList.size();

    return a.exec();
}
