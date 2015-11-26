#include "networkwidget.h"
#include <QDebug>
#include "network.h"
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QMediaPlayer>

NetWorkWidget::NetWorkWidget(QWidget *parent) :
    QWidget(parent)
{

    const int WidgetWidth = 500, WidgetHeight = 400;
    const int WidgetX = parentWidget()->width()-WidgetWidth-20, WidgetY = 150;
    this->resize(WidgetWidth, WidgetHeight);

    lab_internet = new QLabel(this);
    lab_internet->setStyleSheet("background: rgba(0, 0, 0, 120);");
    lab_internet->resize(WidgetWidth, WidgetHeight);

    labTitle = new QLabel("在线音乐");
    labTitle->setAlignment(Qt::AlignCenter);
    tableWidget = new QTableWidget;
    tableWidget->setColumnCount(3);
    tableWidget->setColumnWidth(0, this->width()-120);
    tableWidget->setColumnWidth(1, 60);
    tableWidget->setColumnWidth(2, 60);
    tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->horizontalHeader()->hide();
    tableWidget->verticalHeader()->hide();
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(playMusic(int, int)));

    //布局
    layout = new QVBoxLayout;
    layout->addWidget(labTitle);
    layout->addWidget(tableWidget);
    this->setLayout(layout);

    netWork = new NetWork;

    //动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(300);
    animation->setStartValue(QPoint(WidgetX, -WidgetHeight));
    animation->setEndValue(QPoint(WidgetX, WidgetY));
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    labTitle->setObjectName("labTitle");
    tableWidget->setObjectName("tableWidget");
    this->setStyleSheet(
                "#labTitle{"
                    "color: white;"
                    "font-size: 20;"
                "}"
                "#tableWidget{"
                    "background: rgba(0, 0, 0, 10);"
                    "color: white;"
                "}"
                );
}

void NetWorkWidget::searchMusic(QString musicName)
{
    connect(netWork, SIGNAL(getMusicUrlsFinished(QMap<QString, QStringList>)),
            this, SLOT(showSearchResult(QMap<QString, QStringList>)));
    netWork->searchMusic(musicName);
}

// 显示在线音乐搜索结果
void NetWorkWidget::showSearchResult(QMap<QString, QStringList> musicList)
{
    musicUrls = musicList.first();
    QString musicName = musicList.firstKey();

    while(tableWidget->rowCount())
    {
        tableWidget->removeRow(tableWidget->rowCount()-1);
    }

    for (int i=0; i<musicUrls.count(); ++i)
    {
        tableWidget->insertRow(tableWidget->rowCount());
        tableWidget->setItem(tableWidget->rowCount()-1, 0, new QTableWidgetItem(musicName));
        tableWidget->setItem(tableWidget->rowCount()-1, 1, new QTableWidgetItem("试听"));
        tableWidget->setItem(tableWidget->rowCount()-1, 2, new QTableWidgetItem("下载"));
    }
}

// 双击试听
void NetWorkWidget::playMusic(int row, int column)
{
    emit playInternetMusic(musicUrls.at(row), tableWidget->item(row, 0)->text());
}
