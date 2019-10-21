#include <QCoreApplication>
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DatabaseManager myDb("mydatabase.db");
    myDb.deleteTable("t_ssss");
    return a.exec();
}
