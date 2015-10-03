#include "network.h"

#include <QFile>
#include <QFileInfo>
#include <QRegExp>

NetWork::NetWork()
{
    manager = new QNetworkAccessManager;
    managerDownloadLrc = new QNetworkAccessManager;
}

NetWork::~NetWork()
{
}

void NetWork::downLoadLrc(QString musicName)
{
    QString lrcName = QFileInfo(musicName).baseName();
    QString lrcUrl = "http://music.baidu.com/search/lrc?key=" + lrcName;
    manager->get(QNetworkRequest(QUrl(lrcUrl)));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replayFinished(QNetworkReply*)));

    lrcFileName = musicName + ".lrc";
}

void NetWork::replayFinished(QNetworkReply *replay)
{
    QString htmlContent = replay->readAll();
    replay->deleteLater();
//    QRegExp regexp("(<a class=\"down-lrc-btn \\{ 'href':'/data2/lrc/\\d+/\\d+\\.lrc' \\}\" href=\"#\">下载LRC歌词</a>')");
    QRegExp regexp("(/data2/lrc/\\d+/\\d+\\.lrc)");
    QStringList list;
    int pos = 0;

    while ((pos = regexp.indexIn(htmlContent, pos)) != -1) {

        list << regexp.cap(1);
        pos += regexp.matchedLength();
    }
    if (!list.isEmpty())
    {
        QString urlDownLrc = "http://music.baidu.com" + list[0];

        managerDownloadLrc->get(QNetworkRequest(QUrl(urlDownLrc)));
        connect(managerDownloadLrc, SIGNAL(finished(QNetworkReply*)), this, SLOT(replayLrcFile(QNetworkReply*)));
    }
}

//下载 lrc 文件
void NetWork::replayLrcFile(QNetworkReply *replay)
{
    QFile file(tr("%1").arg(lrcFileName));
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << replay->readAll();
    file.close();
    replay->deleteLater();
    emit lrcDownloadFinished();
}


