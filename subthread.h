#ifndef SUBTHREAD_H
#define SUBTHREAD_H

#include <QThread>
#include <QStringList>

class SubThread : public QThread
{
    Q_OBJECT
public:
    explicit SubThread(QObject *parent = 0);

    void insertDatabase(QString databaseName, QString tableName, QString columnNames, QStringList lineValues);

protected:
    void run();

private:
    QString databaseName;
    QString tableName;
    QString columnNames;
    QStringList lineValues;

signals:

public slots:

};

#endif // SUBTHREAD_H
