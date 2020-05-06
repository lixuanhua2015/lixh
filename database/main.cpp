#include <QCoreApplication>
#include "databasemanager.h"
#include "myArithmetic.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DatabaseManager myDb("mydatabase.db");
    studyStack();

    return a.exec();
}
