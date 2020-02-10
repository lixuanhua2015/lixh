#include <QCoreApplication>
#include "databasemanager.h"
#include "myArithmetic.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DatabaseManager myDb("mydatabase.db");
    int i[5];
    int *p = i;
    RTU_DEBUG << p  << &p;

    return a.exec();
}
