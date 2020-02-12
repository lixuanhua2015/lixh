#ifndef PARAMBASE_H
#define PARAMBASE_H
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <iostream>
using namespace std;
#define RTU_DEBUG qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")<<__FILE__<<":"<<__FUNCTION__<<__LINE__<<":"
#endif // PARAMBASE_H
