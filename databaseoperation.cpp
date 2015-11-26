#include "databaseoperation.h"
#include <QDebug>
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
    if (db.isOpen())
    {
        db.close();
    }
}

//打开数据库
bool DatabaseOperation::openDatabase(QString hostName, QString userName, QString password)
{
//    if(QSqlDatabase::contains("qt_sql_default_connection"))
//      db = QSqlDatabase::database("qt_sql_default_connection");
//    else
//      db = QSqlDatabase::addDatabase("QSQLITE");
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

//创建表
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
        db.removeDatabase("qt_sql_default_connection");
        db.close();
        QMessageBox::warning(0, QObject::tr("错误！"), QObject::tr("数据库创建表失败！%1").arg(query.lastError().text()), QMessageBox::Ok);
        return false;
    }
    db.removeDatabase("qt_sql_default_connection");
    db.close();
    return true;
}

bool DatabaseOperation::createTables(QStringList tableNames, QStringList columnMessages)
{
    Q_ASSERT_X(tableNames.length() == columnMessages.length(), "createTables", "arguments error");

    //打开数据库
    if (!openDatabase())
    {
        QMessageBox::warning(0, QObject::tr("严重错误！"), QObject::tr("配置文件保存失败！"), QMessageBox::Ok);
        return false;
    }

    //创建表
    QSqlQuery query(db);
    for (int i=0; i<tableNames.length(); ++i)
    {
        if (!query.exec(QObject::tr("create table %1(%2)").
                         arg(tableNames.at(i)).
                         arg(columnMessages.at(i))
                         ))
        {
            db.removeDatabase("qt_sql_default_connection");
            db.close();
            QMessageBox::warning(0, QObject::tr("错误！"), QObject::tr("数据库创建表失败！%1").arg(query.lastError().text()), QMessageBox::Ok);
            return false;
        }
    }
    db.removeDatabase("qt_sql_default_connection");
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
        db.removeDatabase("qt_sql_default_connection");
        db.close();
        return QStringList("ERROR: DatabaseOperation::getData()::001");
    }
    query.next();
    if (query.value(0).toString().isEmpty())    //用到 <QVariant>
    {
        db.removeDatabase("qt_sql_default_connection");
        db.close();
        return QStringList("ERROR: DatabaseOperation::getData()::002");
    }
    db.removeDatabase("qt_sql_default_connection");
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
            db.removeDatabase("qt_sql_default_connection");
            db.close();
            QMessageBox::warning(0, QObject::tr("错误！"), QObject::tr("数据库写入数据失败！本次添加的列表%1将成为临时列表！").arg(line), QMessageBox::Ok);
            return false;
        }
    }
    db.removeDatabase("qt_sql_default_connection");
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
            db.removeDatabase("qt_sql_default_connection");
            db.close();
            return false;
        }
    }
    db.removeDatabase("qt_sql_default_connection");
    db.close();
    return true;
}

//数据库修改数据
bool DatabaseOperation::alterDatabase(QString tableName, QString oldDate, QString newDate)
{

}

// 查询所有表及每张表对应的数据
QList<QMap<QString, QStringList> > DatabaseOperation::getTableNamesAndTableData()
{
    QList<QMap<QString, QStringList> > result;                   //

    //打开数据库
    if (!openDatabase())
    {
        return result;
    }

    QSqlQuery queryTableName(db);                               // 查询表名
    QSqlQuery queryLineData(db);                                // 查询表中数据

    queryTableName.exec("SELECT NAME FROM sqlite_master");
    while(queryTableName.next())
    {
        QString tableName;
        QStringList lineDataList;
        QMap<QString, QStringList> tableData;       // key:表名  value:表中数据

        tableName = queryTableName.value("name").toString();

        queryLineData.exec(QObject::tr("SELECT * FROM %1").arg(tableName));
        while(queryLineData.next())
        {
            lineDataList.append(queryLineData.value("musicName").toString());
        }

        tableData.insert(tableName, lineDataList);
        result.append(tableData);
    }

    db.removeDatabase("qt_sql_default_connection");
    db.close();
    return result;
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
        db.removeDatabase("qt_sql_default_connection");
        db.close();
        QMessageBox::warning(0, QObject::tr("错误！"), QObject::tr("数据库删除数据失败！"), QMessageBox::Ok);
        return false;
    }
    if (!query.exec(QObject::tr("UPDATE %1 SET id=id-1 WHERE id>%2").arg(tableName).arg(id_deleteDate)))
    {
        QMessageBox::warning(0, QObject::tr("错误！"), QObject::tr("数据库id排序失败！"), QMessageBox::Ok);
        db.removeDatabase("qt_sql_default_connection");
        db.close();
        return false;
    }
    db.removeDatabase("qt_sql_default_connection");
    db.close();
    return true;
}

//删除表中全部数据
bool DatabaseOperation::deleteAllOfTable(QString tableName)
{
    if (!openDatabase())
    {
        return false;
    }
    QSqlQuery query(db);
    if (!query.exec(QObject::tr("DELETE FROM %1").arg(tableName)))
    {
        db.removeDatabase("qt_sql_default_connection");
        db.close();
        return false;
    }
    db.removeDatabase("qt_sql_default_connection");
    db.close();
    return true;
}

//删除表
bool DatabaseOperation::deleteTable(QString tableName)
{
    if (!openDatabase())
    {
        return false;
    }
    QSqlQuery query(db);
    if (!query.exec(QObject::tr("DROP TABLE %1").arg(tableName)))
    {
        db.removeDatabase("qt_sql_default_connection");
        db.close();
        QMessageBox::warning(0, QObject::tr("错误！"), QObject::tr("数据库删除表失败！"), QMessageBox::Ok);
        return false;
    }
    db.removeDatabase("qt_sql_default_connection");
    db.close();
    return true;
}

