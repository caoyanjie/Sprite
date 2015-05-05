#ifndef PLAYMODLE_H
#define PLAYMODLE_H

#include <QGroupBox>
#include <QLabel>
#include <QToolButton>

class PlayModle : public QGroupBox
{
    Q_OBJECT
public:
    explicit PlayModle(QWidget *parent = 0);

    enum playModelValue{
        play_random,
        play_once,
        play_single,
        play_sequence,
        play_loop,
        play_custom
    };

    //当前音量值
//    int currentVolumeValue;

private:
    //定义6种 播放模式
    QLabel *lab_fram;
    QToolButton *tbn_play_random;
    QToolButton *tbn_play_once;
    QToolButton *tbn_play_single;
    QToolButton *tbn_play_sequence;
    QToolButton *tbn_play_loop;
    QToolButton *tbn_play_custom;



    //将当前播放模式写人配置文件
    void write_currentPlayModelValue_toIni(playModelValue);

signals:
    void playModel_choose(playModelValue);       //改变播放模式
    void settingDataChanged(QString, QString);   //设置信息改变，写入配置文件

public slots:
//    void receiveCurrentVolumeValue(int);        //接收当前音量，以更新配置文件
    void play_random_clicked();
    void play_once_clicked();
    void play_single_clicked();
    void play_sequence_clicked();
    void play_loop_clicked();
    void play_custom_clicked();
};

#endif // PLAYMODLE_H
