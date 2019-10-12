/**************************************************
 * 文件名 :databasemanager.h
 * 文件描述:数据库管理类
 * 创建人 ：McGrady Li
 * 创建时间：2019/10/12
***************************************************/
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>

#define RTU_DEBUG qDebug()<<QThread::currentThreadId()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")<<__FILE__<<":"<<__LINE__<<":"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);
    ~DatabaseManager();

    /**
     * @brief 连接数据库
     * @param 数据库名
     */
    void connectDB(QString dbName);
    void connectDB();
    /**
     * @brief 获取数据库
     * @param dbName
     * @return
     */
    QSqlDatabase getDB(QString dbName);
    QSqlDatabase getDB();
    /**
     * @brief 与数据库断开连接
     * @param dbName
     */
    void disconnectDB(QString dbName);
    void disconnectDB();
    /**
     * @brief 创建表
     * @param 表名
     * @param 各个列的名称
     * @param 各个列的类型   最好都是string（nvarchar）
     * @param 主键列 （-1表示没主键）
     */
    void creatTable(QSqlDatabase db, QString tableName, QVector<QString> colNames, QVector<QString> colType, int keyCol);
    /**
     * @brief 向表中添加数据
     * @param 表名
     * @param 值
     */
    void addValue2Table(QSqlDatabase db, QString tableName, QVector<QString> values);
    /**
     * @brief 查询数据库
     * @param 查询命令
     * @return
     */
    QSqlQuery getSelectValues(QSqlDatabase db, QString strQuery);
    /**
     * @brief getFilterValues
     * @param db
     * @param tableName
     * @param colName
     * @param colValue
     * @return
     */
    QSqlQuery getFilterValues(QSqlDatabase db, QString tableName, QString colName, QString colValue);
    /**
     * @brief 修改或者删除
     * @param 命令
     * @return
     */
    bool updateAndDelete(QSqlDatabase db, QString strQuery);
    /**
     * @brief 是否包含数据库
     * @param dbName
     * @return
     */
    bool isContainDB(QString dbName);
    /**
     * @brief 是否包含某表
     * @param db
     * @param tableName
     * @return
     */
    bool isContainTable(QSqlDatabase db, QString tableName);
    /**
     * @brief getTableRowCount
     * @param db
     * @param tableName
     * @return
     */
    int getTableRowCount(QSqlDatabase db, QString tableName);
    /**
     * @brief
     * @param db
     * @param tableName
     * @param colName
     * @param value
     */
    void insertValue2Table(QSqlDatabase db, QString tableName, QVector<QString> colName, QVector<QString> value);
    void insertValue2Table(QSqlDatabase db, QString tableName, QVector<QString> value);
    /**
     * @brief get record
     * @param db
     * @param tableName
     * @param index of record
     * @return
     */
    QSqlRecord getTabelRecord(QSqlDatabase db, QString tableName,int index);
    /**
     * @brief clearTableValues
     * @param db
     * @param tableName
     */
    void clearTableValues(QSqlDatabase db, QString tableName);
    /**
     * @brief getFilterRecord
     * @param db
     * @param tableName
     * @param colName
     * @param colValue
     * @return
     */
    QSqlRecord getFirstFilterRecord(QSqlDatabase db, QString tableName, QString colName, QString colValue);
    /**
     * @brief 更新数据库
     * @param tabName
     * @param entryName
     * @param entryVal
     * @param keyColName
     * @param keyVal
     * @return
     */
    bool updateEntry(QSqlDatabase db, const QString &tabName,
            const QString &entryName,
            const QString &entryVal,
            const QString &keyColName,
            const QString &keyVal
            );
    /**
     * @brief 更新数据库
     * @param tabName
     * @param entryName
     * @param entryVal
     * @param keyColName
     * @param keyVal
     * @param 主要标志物
     * @param 要修改的数目
     * @return
     */
    bool updateEntry(QSqlDatabase db, const QString &tabName,
            const QString &entryName,
            const QString &entryVal,
            const QString &keyColName,
            const QString &keyVal,
            const QString &sign,
            const int &num
            );
    /**
     * @brief 获得所有的点
     * @param tabName
     * @param colName
     * @param colValue
     * @param model
     * @return
     */
    bool getFilterEntries(QSqlDatabase db,
            QString tabName,
            QString colName,
            QString colValue,
            QSqlQueryModel &model
            );
    /**
     * @brief 获得一定数目的点
     * @param db
     * @param tabName
     * @param colName
     * @param colValue
     * @param model
     * @param 要获取的数目
     * @return
     */
    bool getFilterEntries(QSqlDatabase db,
            QString tabName,
            QString colName,
            QString colValue,
            QSqlQueryModel &model,
            int num
            );

    /**
     * @brief 获得一定数目的点
     * @param db
     * @param tabName
     * @param colName
     * @param colValue
     * @param query
     * @param 要获取的数目
     * @return
     */
    bool getFilterEntries(QSqlDatabase db,
            QString tabName,
            QString colName,
            QString colValue,
            QSqlQuery &query,
            int num
            );

    /**
     * @brief 删除数据
     * @param tabName
     * @param 命令
     * @return
     */
    bool deleteEntries(QSqlDatabase db, QString tabName,
               QString command);

    /**
     * @brief 向表中添加多个数据
     * @param tabName
     * @param values
     * @return
     */
    bool addValues2Table(QSqlDatabase db, QString tabName,
                QVector<QVector<QString> > values);

    bool addVarcharCol2Table(QSqlDatabase db,
            const QString &tabName,
            const QString &colName,
            const QString &defaultV
            );

signals:

public slots:

private:

};

#endif // DATABASEMANAGER_H
