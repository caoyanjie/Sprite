#ifndef NETWORK_H
#define NETWORK_H
//#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NetWork : QObject
{
    Q_OBJECT

public:
    NetWork();
    ~NetWork();

    void downLoadLrc(QString lrcUrl);

private:
    QNetworkAccessManager *manager;
    QString lrcFileName;

private slots:
    void replayFinished(QNetworkReply *replay);
};

#endif // NETWORK_H
