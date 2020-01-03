#include <QCoreApplication>
#include "databasemanager.h"
#include "myArithmetic.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DatabaseManager myDb("mydatabase.db");
    int next[20]={0};
    char T[] = "ababxbababcadfabcdabddsss";
    char P[] = "abcdabd";
    printf("%s\n",T);
    printf("%s\n",P);
    RTU_DEBUG << kmp(T,P,next);
    return a.exec();
}
