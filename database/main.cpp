#include <QCoreApplication>
#include "databasemanager.h"
#include "myArithmetic.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DatabaseManager myDb("mydatabase.db");
    QString arr("pwwkew");
    RTU_DEBUG << lengthOfLongestSubstring(arr);

    return a.exec();
}
