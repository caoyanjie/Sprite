#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include <QWidget>
#include <QMap>
class NetWork;
class QLabel;
class QTableWidget;
class QVBoxLayout;
class NetWorkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NetWorkWidget(QWidget *parent = 0);

    void searchMusic(QString musicName);

private:
    NetWork *netWork;
    QStringList musicUrls;
    QLabel *lab_internet;
    QLabel *labTitle;
    QTableWidget *tableWidget;
    QVBoxLayout *layout;

signals:
    void playInternetMusic(QString);        // 双击试听在线音乐

private slots:
    void showSearchResult(QMap<QString, QStringList> musicList);
    void playMusic(int row, int column);

public slots:

};

#endif // NETWORKWIDGET_H
