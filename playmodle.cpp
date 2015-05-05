#include "playmodle.h"
#include <QDebug>

#include <QFile>

PlayModle::PlayModle(QWidget *parent) :
    QGroupBox(parent)
{
    resize(38, 180);

    //创建6种 播放模式 按钮
    lab_fram = new QLabel(this);
    tbn_play_random = new QToolButton(this);
    tbn_play_once = new QToolButton(this);
    tbn_play_single = new QToolButton(this);
    tbn_play_sequence = new QToolButton(this);
    tbn_play_loop = new QToolButton(this);
    tbn_play_custom = new QToolButton(this);

    //设置6种 播放模式 位置大小
    lab_fram->setGeometry(0, 0, 38, 147);
    tbn_play_random ->setGeometry(8, 5, 22, 22);
    tbn_play_once ->setGeometry(8, 28, 22, 22);
    tbn_play_single ->setGeometry(8, 51, 22, 22);
    tbn_play_sequence ->setGeometry(8, 74, 22, 22);
    tbn_play_loop ->setGeometry(8, 97, 22, 22);
    tbn_play_custom ->setGeometry(8, 120, 22, 22);

    setObjectName("thisWidget");
    lab_fram->setObjectName("lab_fram");
    tbn_play_random->setObjectName("tbn_play_random");
    tbn_play_once->setObjectName("tbn_play_once");
    tbn_play_single->setObjectName("tbn_play_single");
    tbn_play_sequence->setObjectName("tbn_play_sequence");
    tbn_play_loop->setObjectName("tbn_play_loop");
    tbn_play_custom->setObjectName("tbn_play_custom");

    //设置6种播放模式 样式
    setStyleSheet(
                "#thisWidget{"
                    "border: 0 solid white;"
                "}"
                "#lab_fram{"
                    "background: rgba(0, 0, 0, 100);"
                    "border: 1px solid white;"
                "}"
                "#tbn_play_random{"
                    "border-image: url(:/Images/playModel_random.png);"
                "}"
                "#tbn_play_random:hover{"
                    "border-image: url(:/Images/playModel_random_hover.png);"
                "}"
                "#tbn_play_once{"
                    "border-image: url(:/Images/playModel_once.png);"
                "}"
                "#tbn_play_once:hover{"
                    "border-image: url(:/Images/playModel_once_hover.png);"
                "}"
                "#tbn_play_single{"
                    "border-image: url(:/Images/playModel_single.png);"
                "}"
                "#tbn_play_single:hover{"
                    "border-image: url(:/Images/playModel_single_hover.png);"
                "}"
                "#tbn_play_sequence{"
                    "border-image: url(:/Images/playModel_sequence.png);"
                "}"
                "#tbn_play_sequence:hover{"
                    "border-image: url(:/Images/playModel_sequence_hover.png);"
                "}"
                "#tbn_play_loop{"
                    "border-image: url(:/Images/playModel_loop.png);"
                "}"
                "#tbn_play_loop:hover{"
                    "border-image: url(:/Images/playModel_loop_hover.png);"
                "}"
                "#tbn_play_custom{"
                    "border-image: url(:/Images/playModel_custom.png);"
                "}"
                "#tbn_play_custom:hover{"
                    "border-image: url(:/Images/playModel_custom_hover.png);"
                "}"
                );

    //设置按钮提示
    tbn_play_random ->setToolTip(tr("随机播放"));
    tbn_play_once ->setToolTip(tr("单首播放"));
    tbn_play_single ->setToolTip(tr("单曲循环"));
    tbn_play_sequence ->setToolTip(tr("单次列表"));
    tbn_play_loop ->setToolTip(tr("列表循环"));
    tbn_play_custom ->setToolTip(tr("N首后停止"));

    //6种按钮被单击， 信号/槽 关联
    connect(tbn_play_random, SIGNAL(clicked()),
            this, SLOT(play_random_clicked()));
    connect(tbn_play_once, SIGNAL(clicked()),
            this, SLOT(play_once_clicked()));
    connect(tbn_play_single, SIGNAL(clicked()),
            this, SLOT(play_single_clicked()));
    connect(tbn_play_sequence, SIGNAL(clicked()),
            this, SLOT(play_sequence_clicked()));
    connect(tbn_play_loop, SIGNAL(clicked()),
            this, SLOT(play_loop_clicked()));
    connect(tbn_play_custom, SIGNAL(clicked()),
            this, SLOT(play_custom_clicked()));
}

//把当前播放模式值写入配置文件
void PlayModle::write_currentPlayModelValue_toIni(playModelValue playModel_currentValue)
{
    //将枚举变量转换成字符串
    QString playModel_currentValue_name;
    switch(playModel_currentValue)
    {
    case play_random:
        playModel_currentValue_name = tr("playModel_random");
        break;
    case play_once:
        playModel_currentValue_name = tr("playModel_once");
        break;
    case play_single:
        playModel_currentValue_name = tr("playModel_single");
        break;
    case play_sequence:
        playModel_currentValue_name = tr("playModel_sequence");
        break;
    case play_loop:
        playModel_currentValue_name = tr("playModel_loop");
        break;
    case play_custom:
        playModel_currentValue_name = tr("playModel_custom");
        break;
    }

    //打开配置文件 写入数据
/*    QFile write_toIni(".data.ini");
    if (write_toIni.open(QIODevice::ReadWrite))
    {
        write_toIni.write(tr("playModel:%1\nvolume:%2\n")
                          .arg(playModel_currentValue_name)
                          .arg(currentVolumeValue)
                          .toUtf8());
        write_toIni.close();
    }*/
    emit settingDataChanged("playModel", tr("%1").arg(playModel_currentValue_name));
}
/*
//接收当前音量值
void PlayModle::receiveCurrentVolumeValue(int currentVolume)
{
    qDebug() << "playModelWindow receive the currentVolume: " << currentVolume;
    currentVolumeValue = currentVolume;
}
*/
//选中 随机播放
void PlayModle::play_random_clicked()
{
    hide();
    emit playModel_choose(play_random);
    write_currentPlayModelValue_toIni(play_random);
}

//选中 单首播放
void PlayModle::play_once_clicked()
{
    hide();
    emit playModel_choose(play_once);
    write_currentPlayModelValue_toIni(play_once);
}

//选中 单机循环
void PlayModle::play_single_clicked()
{
    hide();
    emit playModel_choose(play_single);
    write_currentPlayModelValue_toIni(play_single);
}

//选中 单次列表
void PlayModle::play_sequence_clicked()
{
    hide();
    emit playModel_choose(play_sequence);
    write_currentPlayModelValue_toIni(play_sequence);
}

//选中 列表循环
void PlayModle::play_loop_clicked()
{
    hide();
    emit playModel_choose(play_loop);
    write_currentPlayModelValue_toIni(play_loop);
}

//选中 自定义播放
void PlayModle::play_custom_clicked()
{
    hide();
    emit playModel_choose(play_custom);
    write_currentPlayModelValue_toIni(play_custom);
}
