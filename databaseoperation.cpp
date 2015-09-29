#include "databaseoperation.h"

#include <QObject>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>

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

    if (!db.open())
    {
        QMessageBox::warning(0, "错误！", "数据库创建失败！\n错误代码：'Error: DatabaseOperation::createDatabase::000'", QMessageBox::Ok);
        return false;
    }

    return true;
}

//创建数据库
void DatabaseOperation::createDatabase(QString defaultTableName)
{
    if (!openDatabase())
    {
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

bool DatabaseOperation::createTable(QString tableName, QString columnMessage)
{
    //打开数据库
    if (!openDatabase())
    {
        QMessageBox::warning(0, QObject::tr("严重错误！"), QObject::tr("配置文件保存失败！"), QMessageBox::Ok);
        return false;
    }

    QSqlQuery query(db);
    //创建表
    if (!query.exec(QObject::tr("create table %1(%2)").arg(tableName).arg(columnMessage)))
    {
        db.close();
        QMessageBox::warning(0, QObject::tr("错误！"), QObject::tr("数据库创建表失败！%1").arg(query.lastError().text()), QMessageBox::Ok);
        return false;
    }
    db.close();
    return true;
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
    if (query.value(0).toString().isEmpty())    //用到 <QVariant>
    {
        db.close();
        return QStringList("ERROR: DatabaseOperation::getData()::002");
    }
    db.close();
    return query.value(0).toStringList();       //用到 <QVariant>
}

//数据库插入数据
bool DatabaseOperation::insertDatabase(QString tableName, QString columnName, QStringList content, int index)
{
    //打开数据库
    if (!openDatabase())
    {
        QMessageBox::warning(0, QObject::tr("严重错误！"), QObject::tr("配置文件保存失败！"), QMessageBox::Ok);
        return false;
    }

    QSqlQuery query(db);
    for(int i=0; i<content.length(); i++)
    {
        //获得要插入的一条记录
        QString line = content.at(i);

        //给单引号转义
        if (line.indexOf('\'') != -1)    //如果字符串中有单引号（'），转换成（''）
        {
            line.replace("'", "''");
        }

        //插入
        if (!query.exec(QObject::tr("INSERT INTO %1(%2) VALUES('%3')").arg(tableName).arg(columnName).arg(line)))
        {
            db.close();
            QMessageBox::warning(0, QObject::tr("错误！"), QObject::tr("数据库写入数据失败！本次添加的列表%1将成为临时列表！").arg(line), QMessageBox::Ok);
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

//删除表中全部数据
bool DatabaseOperation::deleteAll(QString tableName)
{
    if (!openDatabase())
    {
        return false;
    }
    QSqlQuery query(db);
    if (!query.exec(QObject::tr("DELETE FROM %1").arg(tableName)))
    {
        db.close();
        return false;
    }
    db.close();
    return true;
}

