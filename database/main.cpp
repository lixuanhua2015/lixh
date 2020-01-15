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
    for (int i = 0; i < 25; ++i) {
        arr[i] &= -arr[i];
    }
    printArr(arr, 25);

    return a.exec();
}
