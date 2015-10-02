#ifndef SUBTHREAD_H
#define SUBTHREAD_H

#include <QThread>
#include <QStringList>
#include <QMap>

class SubThread : public QThread
{
    Q_OBJECT
public:
    explicit SubThread(QObject *parent = 0);

    enum Method{
        None = -1,
        InsertDataBase = 0,
        UpdateDatabase
    };

    void insertDatabase(Method method, QString databaseName, QString tableName, QString columnNames, QStringList lineValues);
    void updateDatabase(Method method, QString databaseName, QString tableName, QMap<QString, QString> columnContent);

protected:
    void run();

private:
    void clear();

    Method method;
    QString databaseName;
    QString tableName;

    QString columnNames;
    QStringList lineValues;

    QMap<QString, QString> columnContent;

signals:

public slots:

};

#endif // SUBTHREAD_H
