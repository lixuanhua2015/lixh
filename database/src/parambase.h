#ifndef PARAMBASE_H
#define PARAMBASE_H
#include <QDebug>
#include <QThread>
#include <QDateTime>
#define RTU_DEBUG qDebug()<<QThread::currentThreadId()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")<<__FILE__<<":"<<__FUNCTION__<<__LINE__<<":"
#endif // PARAMBASE_H
