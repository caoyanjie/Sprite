#include "network.h"

#include <QFile>
#include <QFileInfo>

NetWork::NetWork()
{
    manager = new QNetworkAccessManager;
}

NetWork::~NetWork()
{

}

void NetWork::downLoadLrc(QString lrcUrl)
{
    manager->get(QNetworkRequest(QUrl(lrcUrl)));
    lrcFileName = QFileInfo(lrcUrl).fileName();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replayFinished(QNetworkReply*)));
}

void NetWork::replayFinished(QNetworkReply *replay)
{
    QFile file(tr("%1").arg(lrcFileName));
    file.open(QIODevice::WriteOnly);
    file.write(replay->readAll());
    file.close();
    replay->deleteLater();
}

