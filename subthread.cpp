#include "subthread.h"
#include "databaseoperation.h"
#include <QDebug>
#include <QMessageBox>

SubThread::SubThread(QObject *parent) :
    QThread(parent)
{
}

//get arguments
void SubThread::insertDatabase(QString databaseName, QString tableName, QString columnNames, QStringList lineValues)
{
    this->databaseName = databaseName;
    this->tableName = tableName;
    this->columnNames = columnNames;
    this->lineValues = lineValues;
}

//main
void SubThread::run()
{
    DatabaseOperation db(databaseName);
    if (!db.insertDatabase(tableName, columnNames, lineValues))
    {
        QMessageBox::warning(0, tr("错误！"), tr("可能文件名中存在特殊字符，数据库写入数据失败！本次添加的列表将成为临时列表！"), QMessageBox::Ok);
    }
}
