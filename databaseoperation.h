#ifndef DATABASEOPERATION_H
#define DATABASEOPERATION_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>

class DatabaseOperation
{
public:
    DatabaseOperation(QString databasePath);
    ~DatabaseOperation();

    bool openDatabase(QString hostName="localhost", QString userName="", QString password="");      //打开数据库
    bool createTable(QString tableName, QString columnMessage);
    bool createTables(QStringList tableNames, QStringList columnMessages);
    QStringList getData(QString tableName, QString columnName);
    bool insertDatabase(QString tableName, QString columnName, QStringList content, int index=-1);  //数据库插入数据
    bool updateDatabase(QString tableName, QMap<QString, QString> columnContent);                   //数据库更新数据
    bool alterDatabase(QString tableName, QString oldDate, QString newDate);                        //数据库修改数据
    QList<QMap<QString, QStringList> > getTableNamesAndTableData();
    bool deleteDatabase(QString tableName, int id_deleteDate);
    bool deleteAllOfTable(QString tableName);
    bool deleteTable(QString tableName);

private:
    QString databaseName;
    QSqlDatabase db;
};

#endif // DATABASEOPERATION_H
