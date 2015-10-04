#ifndef BOTTOMGROUPBOX_H
#define BOTTOMGROUPBOX_H

#include "musiclist.h"
#include "playmodle.h"
#include <QGroupBox>
class QToolButton;
class QSlider;
class QLabel;
class QCheckBox;
class QHBoxLayout;
class QGridLayout;

class BottomGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit BottomGroupBox(MusicList *object, QString programPath, QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);   //重写 鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);    //重写 鼠标移动事件
    bool eventFilter(QObject *, QEvent *);      //事件过滤器

private:
    void setChildrenWidgets();                  //初始化 附属 部件

    const QString setupDatabaseName;

    MusicList *musicList;

    QToolButton *tbn_play_previous;             //定义 ”上一曲“   按钮
    QToolButton *tbn_play_pause;                //定义 ”播放/暂停“ 按钮
    QToolButton *tbn_play_next;                 //定义 ”下一曲“    按钮
    QLabel *lab_fram;                           //定义 工具框 区域
    QLabel *lab_playLogo;                       //定义 播放图标
    QSlider *slider_progress;                   //定义 进度条
    QLabel *lab_musicMessage;                   //定义 歌曲信息
    QLabel *lab_currentTime;                    //定义 播放时间
    QLabel *lab_totalTime;                      //定义 歌曲总时长
    QCheckBox *cbx_lrc;                         //定义 ”桌面歌词“ 按钮
    QToolButton *tbn_volumn;                    //定义 ”音量“    按钮
    QToolButton *tbn_playModle;                 //定义 ”播放模式“ 按钮

    //布局
    QHBoxLayout *layout_H;
    QGridLayout *layout_G;
    QHBoxLayout *layout_top;

signals:
    void lrc_click(bool);                               //单击 “桌面歌词”  按钮
    void showVolumn(QPoint);                            //单击 “音量”     按钮
    void showPlayModle(QPoint);                         //播放模式 改变
    void musicTitleAndAuthor(QString);
//    void slider_progress_valueChange(qint64);         //拖动 ”播放进度“  滑竿

public slots:
    void playModle_choosed(PlayModle::PlayMode);        //播放模式改变 处理     [需要判断情况]
    void sendMusicTitleAndAuthor();

protected slots:
//    void slider_progress_moved(int);                  //拖动 ”播放进度“  滑竿 [需要数据转换]
    void durationChanged(qint64);                       //歌曲 时长信息 出现

private slots:
    void getMetaData(bool);                             //提取 歌曲信息

    void positionChanged(qint64);                       //播放进度改变，更新进度条
    void slider_progress_moved(int);
    void play_pause_clicked();                          //播放/暂停 处理
    void play_previous_clicked();                       //上一曲 处理
    void play_next_clicked();                           //下一曲 处理
    void playStatusChanged(QMediaPlayer::State state);  //播放状态 改变
    void seek(int current);                             //定位播放进度

};

#endif // BOTTOMGROUPBOX_H
