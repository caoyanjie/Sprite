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

    void downLoadLrc(QString lrcName);

private:
    QNetworkAccessManager *manager;
    QNetworkAccessManager *managerDownloadLrc;
    QString lrcFileName;
    int a;

private slots:
    void replayFinished(QNetworkReply *replay);
    void replayLrcFile(QNetworkReply *replay);
};

#endif // NETWORK_H
