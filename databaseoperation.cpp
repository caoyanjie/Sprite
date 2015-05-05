#include "databaseoperation.h"
#include <QDebug>

//#include <QObject>
#include <QMessageBox>
#include <QVariant>

DatabaseOperation::DatabaseOperation(QString databasePath) :
    databaseName(databasePath)
{

}

DatabaseOperation::~DatabaseOperation()
{
}

//打开数据库
bool DatabaseOperation::openDatabase(QString hostName, QString userName, QString password)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);
    db.setHostName(hostName);
    db.setUserName(userName);
    db.setPassword(password);

    if (db.open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

//创建数据库
void DatabaseOperation::createDatabase(QString defaultTableName)
{
    if (!openDatabase())
    {
        QMessageBox::warning(0, "错误！", "数据库创建失败！\n错误代码：'Error: DatabaseOperation::createDatabase::000'", QMessageBox::Ok);
        return;
    }
    QSqlQuery query(db);
    if (!query.exec(QObject::tr("CREATE TABLE %1(id integer primary key, musicName text)").arg(defaultTableName)))
    {
        QMessageBox::warning(0, "错误！", "数据建表失败！\n错误代码：'Error: DatabaseOperation::createDatabase::001'", QMessageBox::Ok);
        db.close();
        return;
    }
    db.close();
}

//查询数据库数据
QStringList DatabaseOperation::getData(QString tableName, QString columnName)
{
    if (!openDatabase())
    {
        return QStringList("ERROR: DatabaseOperation::getData()::000");
    }

    QSqlQuery query(db);
    if (!query.exec(QObject::tr("SELECT %1 FROM %2").arg(columnName).arg(tableName)))
    {
        db.close();
        return QStringList("ERROR: DatabaseOperation::getData()::001");
    }
    query.next();
    if (query.value(0).toString().isEmpty())
    {
        db.close();
        return QStringList("ERROR: DatabaseOperation::getData()::002");
    }
    db.close();
    return query.value(0).toStringList();
}

//数据库插入数据
bool DatabaseOperation::insertDatabase(QString tableName, QString columnName, QStringList content, int indext)
{
    if (!openDatabase())
    {
        return false;
    }

    QSqlQuery query(db);
    for(int i=0; i<content.length(); i++)
    {
        if (!query.exec(QObject::tr("INSERT INTO %1(%2) VALUES('%3')").arg(tableName).arg(columnName).arg(content.at(i))))
        {
            db.close();
            return false;
        }
    }
    db.close();
    return true;
}

//数据库更新数据
bool DatabaseOperation::updateDatabase(QString tableName, QMap<QString, QString> columnContent)
{
    if (!openDatabase())
    {
        return false;
    }

    QSqlQuery query(db);
    foreach (const QString &key, columnContent.keys())
    {
        if (!query.exec(QObject::tr("UPDATE %1 SET %2=%3").arg(tableName).arg(key).arg(columnContent.value(key))))
        {
            db.close();
            return false;
        }
    }
    db.close();
    return true;
}

//数据库修改数据
bool DatabaseOperation::alterDatabase(QString tableName, QString oldDate, QString newDate)
{

}

//数据库删除数据
bool DatabaseOperation::deleteDatabase(QString tableName, int id_deleteDate)
{
    if (!openDatabase())
    {
        return false;
    }
    QSqlQuery query(db);
    if (!query.exec(QObject::tr("DELETE FROM %1 WHERE id=%2").arg(tableName).arg(id_deleteDate)))
    {
        db.close();
        return false;
    }
    if (!query.exec(QObject::tr("UPDATE %1 SET id=id-1 WHERE id>%2").arg(tableName).arg(id_deleteDate)))
    {
        QMessageBox::warning(0, QObject::tr("错误！"), QObject::tr("数据库id排序失败！"), QMessageBox::Ok);
        db.close();
        return false;
    }
    db.close();
    return true;
}

