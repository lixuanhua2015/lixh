#include <QCoreApplication>
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DatabaseManager myDb;
    myDb.connectDB("AA");
    myDb.connectDB("BB");
    myDb.getDB("AA");
    myDb.getDB("BB");
    return a.exec();
}
