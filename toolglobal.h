#ifndef TOOLGLOBAL_H
#define TOOLGLOBAL_H

#include <QGroupBox>
class QPushButton;
class Timing;
class QLCDNumber;
class QGridLayout;
class QHBoxLayout;

class ToolGlobal : public QGroupBox
{
    Q_OBJECT
public:
    explicit ToolGlobal(QWidget *parent = 0);

    //枚举 定时器处理方式
    enum whatTimingDo{
        playStop = -2,
        windowClose = -3,
        shutdown = -4
    }TimingTarget;


private:
    //魔音 按钮
    QGroupBox *gbx_magic;
    QPushButton *pbn_magic_state;
    QPushButton *pbn_magic;
    bool magicState;

    //定时 按钮
    QGroupBox *gbx_clock;
    QPushButton *pbn_clock_state;
    QPushButton *pbn_clock;
    bool clockState;

    //定时器
    Timing *timing;
    QTimer *timer_default;
    QTimer *timer_target;
    QLCDNumber *lcdNumber;
    int timer_totalTime;

    //定时器LCD灯
    QWidget *widget_timingLCD;

    //歌词 按钮
    QGroupBox *gbx_lrc_game;
    QPushButton *pbn_lrc_game_state;
    QPushButton *pbn_lrc_game;
    bool gameState;

    //布局
    QGridLayout *layout_magic;
    QGridLayout *layout_timer;
    QGridLayout *layout_lrc;
    QHBoxLayout *layout_top;

signals:
    void magicStateChange(bool);
    void timeout_playStop();
    void gameStateChange(bool);
//    void ShowVideoPlayer();

public slots:

protected slots:
    void pbn_magic_clicked();
    void pbn_clock_clicked();
    void pbn_lrc_game_clicked();

    void startTiming();             //开启定时器
    void stopTiming();              //结束定时器

    void timer_default_timeout();   //定时器处理
    void timer_target_timeout();    //定时器处理
};

#endif // TOOLGLOBAL_H
