#include <QCoreApplication>
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DatabaseManager myDb("mydatabase.db");
    return a.exec();
}
