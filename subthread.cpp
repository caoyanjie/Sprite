#include "subthread.h"
#include "databaseoperation.h"
#include <QDebug>
#include <QMessageBox>

SubThread::SubThread(QObject *parent) :
    QThread(parent),
    method(None)
{
}

//get arguments
void SubThread::insertDatabase(Method method, QString databaseName, QString tableName, QString columnNames, QStringList lineValues)
{
    this->method = method;
    this->databaseName = databaseName;
    this->tableName = tableName;
    this->columnNames = columnNames;
    this->lineValues = lineValues;
}

//更新数据
void SubThread::updateDatabase(Method method, QString databaseName, QString tableName, QMap<QString, QString> columnContent)
{
    this->method = method;
    this->databaseName = databaseName;
    this->tableName = tableName;
    this->columnContent = columnContent;
}

//main
void SubThread::run()
{
    DatabaseOperation db(databaseName);

    switch(method)
    {
    case None:
        clear();
        return;
    case InsertDataBase:
        if (databaseName.isEmpty() || tableName.isEmpty() || columnNames.isEmpty() || lineValues.isEmpty())
        {
            QMessageBox::warning(0, tr("错误！"), tr("传入子线程数据不完整，子线程未执行而结束！"), QMessageBox::Ok);
            clear();
            return;
        }
        if (!db.insertDatabase(tableName, columnNames, lineValues))
        {
            QMessageBox::warning(0, tr("错误！"), tr("可能文件名中存在特殊字符，数据库写入数据失败！本次添加的列表将成为临时列表！"), QMessageBox::Ok);
        }
        break;
    case UpdateDatabase:
        if (databaseName.isEmpty() || tableName.isEmpty() || columnContent.isEmpty())
        {
            QMessageBox::warning(0, tr("错误！"), tr("传入子线程数据不完整，子线程未执行而结束！"), QMessageBox::Ok);
            clear();
            return;
        }
        if (!db.updateDatabase(tableName, columnContent))
        {
            QMessageBox::warning(0, tr("错误！"), tr("可能文件名中存在特殊字符，数据库写入数据失败！本次添加的列表将成为临时列表！"), QMessageBox::Ok);
        }
        break;
    default:
        break;
    }

    clear();
}

//还原默认值
void SubThread::clear()
{
    method = None;
    databaseName = "";
    tableName = "";
    columnNames = "";
    lineValues.clear();
    columnContent.clear();
}
