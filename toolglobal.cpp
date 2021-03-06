#include "toolglobal.h"

#include "timing.h"
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLCDNumber>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QTime>

ToolGlobal::ToolGlobal(QWidget *parent) :
    QGroupBox(parent),
    lcdNumber(0),
    magicState(false),
    clockState(false),
    gameState(false)
{
//////////////////////    setGeometry(0, 81, 1001, 32);

    //创建 魔音按钮
    gbx_magic = new QGroupBox(this);                            //背景阴影
    pbn_magic_state = new QPushButton("ON", this);              //状态标签
    pbn_magic = new QPushButton("魔音", this);                   //“魔音” 按钮
    gbx_magic->setFixedSize(110, 20);
    pbn_magic_state->setFixedSize(25, 15);
    pbn_magic->setFixedSize(60, 20);

    pbn_magic_state ->setToolTip(tr("开启魔音"));                 //设置 按钮提示

    pbn_magic ->setToolTip(tr("开启魔音"));                       //...........
    gbx_magic->setObjectName("gbx_magic");
    pbn_magic_state ->setObjectName(tr("pbn_magic_state"));
    pbn_magic->setObjectName("pbn_magic");

    //布局
    layout_magic = new QGridLayout;
    layout_magic->addWidget(gbx_magic, 0, 0, 20, 110);
    layout_magic->addWidget(pbn_magic_state, 3, 18, 15, 25);
    layout_magic->addWidget(pbn_magic, 0, 50, 20, 60);
    layout_magic->setSpacing(0);
    layout_magic->setMargin(0);

    //创建 定时按钮
    gbx_clock = new QGroupBox(this);                            //背景阴影
    pbn_clock_state = new QPushButton("ON", this);              //状态标签
    pbn_clock = new QPushButton("定时", this);                   //“定时” 按钮
    gbx_clock->setFixedSize(110, 20);
    pbn_clock_state->setFixedSize(25, 15);
    pbn_clock->setFixedSize(65, 20);

    gbx_clock->setObjectName("gbx_clock");
    pbn_clock_state ->setObjectName(tr("pbn_clock_state"));
    pbn_clock->setObjectName("pbn_clock");
    pbn_clock_state ->setToolTip(tr("定时"));                    //设置 按钮提示
    pbn_clock ->setToolTip(tr("定时"));                          //...........

    //布局
    layout_timer = new QGridLayout;
    layout_timer->addWidget(gbx_clock, 0, 0, 20, 110);
    layout_timer->addWidget(pbn_clock_state, 3, 13, 15, 25);
    layout_timer->addWidget(pbn_clock, 0, 46, 20, 65);
    layout_timer->setSpacing(0);
    layout_timer->setMargin(0);

    //LCD灯
    widget_timingLCD = new QWidget;
    widget_timingLCD->setFixedSize(80, 18);

    //创建 歌词/游戏 按钮
    gbx_lrc_game = new QGroupBox(this);                         //背景阴影
    pbn_lrc_game_state = new QPushButton("小游戏", this);        //状态标签
    pbn_lrc_game = new QPushButton("歌词", this);                //“定时” 按钮
    gbx_lrc_game->setFixedSize(110, 20);
    pbn_lrc_game_state->setFixedSize(40, 15);
    pbn_lrc_game->setFixedSize(65, 20);

    gbx_lrc_game->setObjectName("gbx_lrc_game");
    pbn_lrc_game_state ->setObjectName(tr("pbn_lrc_game_state"));
    pbn_lrc_game->setObjectName("pbn_lrc_game");

    pbn_lrc_game_state ->setToolTip(tr("本地小游戏"));            //设置 按钮提示
    pbn_lrc_game ->setToolTip(tr("本地小游戏"));                  //...........

    //布局
    layout_lrc = new QGridLayout;
    layout_lrc->addWidget(gbx_lrc_game, 0, 0, 20, 110);
    layout_lrc->addWidget(pbn_lrc_game_state, 3, 3, 15, 40);
    layout_lrc->addWidget(pbn_lrc_game, 0, 45, 20, 65);
    layout_lrc->setSpacing(0);
    layout_lrc->setMargin(0);

    //顶级布局
    layout_top = new QHBoxLayout;
    layout_top->addStretch(382);
    layout_top->addLayout(layout_magic);
    layout_top->addStretch(widget_timingLCD->width()+10);
    layout_top->addLayout(layout_timer);
    layout_top->addWidget(widget_timingLCD);
    layout_top->addLayout(layout_lrc);
    layout_top->addStretch(109);
    layout_top->setSpacing(8);
    layout_top->setContentsMargins(12, 7, 12, 5);
    setLayout(layout_top);

    setObjectName(tr("toolGlobal"));
    setStyleSheet(
                "#toolGlobal{"
//                    "border-image: url(:/Images/bgTool1.png);"
                    "border-radius: 0px;"
//                    "background: rgba(0, 0, 0, 120);"
                    "background: rgba(40, 62, 83, 195);"
                "}"
                "QGroupBox{"                                                //按钮背景阴影框架
                    "border-radius: 10px;"
                    "background: rgba(0, 0, 0, 80);"
                "}"
                "#pbn_magic_state, #pbn_clock_state, #pbn_lrc_game_state{"  //三个 “按钮状态” 样式
                    "background: rgba(0, 0, 0, 0);"
                    "color: rgba(0, 255, 0, 180);"
                "}"
                "QPushButton{"                                              //“按钮” 样式
                    "border-radius: 10px;"
                    "background: rgba(0, 0, 0, 80);"
                    "color: rgba(255, 255, 255, 200);"
                "}"
                );

    //创建定时器
    timer_default = new QTimer(this);
    timer_target = new QTimer(this);

    //按钮 信号/槽关联
    connect(pbn_magic, SIGNAL(clicked()), this, SLOT(pbn_magic_clicked()));             //单击 “魔音状态” 按钮
    connect(pbn_magic_state, SIGNAL(clicked()), this, SLOT(pbn_magic_clicked()));       //单击 “魔音开关” 按钮
    connect(pbn_clock, SIGNAL(clicked()), this, SLOT(pbn_clock_clicked()));             //单击 “定时状态” 按钮
    connect(pbn_clock_state, SIGNAL(clicked()), this, SLOT(pbn_clock_clicked()));       //单击 “定时开关” 按钮
    connect(pbn_lrc_game, SIGNAL(clicked()), this, SLOT(pbn_lrc_game_clicked()));       //单击 “小游戏” 按钮
    connect(pbn_lrc_game_state, SIGNAL(clicked()), this, SLOT(pbn_lrc_game_clicked())); //单击 “歌词” 按钮

    //定时器 信号/槽关联
    connect(timer_default, SIGNAL(timeout()), this, SLOT(timer_default_timeout()));     //1秒倒计时
    connect(timer_target, SIGNAL(timeout()), this, SLOT(timer_target_timeout()));       //自定义倒计时

    this->setMouseTracking(true);
}

//单击 “魔音” 处理
void ToolGlobal::pbn_magic_clicked()
{
    if (! magicState)
    {
        pbn_magic_state->move(gbx_magic->x()+72, pbn_magic_state->y());     //设置 ”魔音按钮“ 位置大小
        pbn_magic->move(gbx_magic->pos());
        pbn_magic_state ->setText(tr("OFF"));
        pbn_magic_state ->setToolTip(tr("关闭魔音"));
        pbn_magic ->setToolTip(tr("关闭魔音"));

        magicState = true;
        emit magicStateChange(true);
    }
    else
    {
        pbn_magic_state->move(gbx_magic->x()+18, pbn_magic_state->y());     //设置 ”定时按钮“ 位置大小
        pbn_magic->move(gbx_magic->x()+50, pbn_magic->y());
        pbn_magic_state ->setText(tr("ON"));
        pbn_magic_state ->setToolTip(tr("开启魔音"));
        pbn_magic ->setToolTip(tr("开启魔音"));
        magicState = false;
        emit magicStateChange(false);
    }
}

//单击 ”定时“ 处理
void ToolGlobal::pbn_clock_clicked()
{
    if (! clockState)       //如果 clockState 是 关闭 状态
    {
        pbn_clock_state->move(gbx_clock->x()+73, pbn_clock_state->y());                 //设置 按钮位置
        pbn_clock->move(gbx_clock->pos());                                              //...........
        pbn_clock_state ->setText(tr("OFF"));                                           //设置 按钮文字
        pbn_clock ->setText(tr("关闭"));                                                 //...........
        pbn_clock_state ->setToolTip(tr("关闭"));                                        //设置 按钮提示
        pbn_clock ->setToolTip(tr("关闭"));                                              //...........
        clockState = true;                                                              //设置为 “开启” 状态
//        timing = new Timing(parentWidget());                                          //创建 “定时器设置” 窗口 并显示
        timing = new Timing(gbx_clock->x()-42, 113, 225, 135, parentWidget());          //创建 “定时器设置” 窗口 并显示
        timing ->show();                                                                //.........................
        connect(timing, SIGNAL(timing_pbnOk_click()), this, SLOT(startTiming()));       //关联 定时器 “确定” 按钮
        connect(timing, SIGNAL(timing_pbnCancle_click()), this, SLOT(stopTiming()));    //关联 定时器 “取消” 按钮
    }
    else                                                //如果 clockState 是 开启 状态
    {
        if (timing)                                     //如果存在 “定时器设置” 窗口
        {
            timing ->close();                           //关闭 “定时器设置” 窗口
        }
        if (lcdNumber)                                  //如果存在 “LCD显示器”
        {
            delete lcdNumber;                           //销毁 “LCD显示器”
            lcdNumber = 0;                              //指针赋0值
            timer_default ->stop();                     //终止 “1秒倒计时"
            timer_target ->stop();                      //终止 "自定义倒计时”
        }
        pbn_clock_state->move(gbx_clock->x()+13, pbn_clock_state->y());     //设置 按钮位置
        pbn_clock->move(gbx_clock->x()+46, pbn_clock->y());                 //...........
        pbn_clock_state ->setText(tr("ON"));                                //设置 按钮文字
        pbn_clock ->setText(tr("定时"));                                     //...........
        pbn_clock_state ->setToolTip(tr("开启魔音"));                         //设置 按钮提示
        pbn_clock ->setToolTip(tr("开启魔音"));                               //...........
        clockState = false;                                                 //设置为 “关闭” 状态
    }
}

//单击 ”歌词/游戏“ 处理
void ToolGlobal::pbn_lrc_game_clicked()
{
    if (! gameState)
    {
        pbn_lrc_game_state->move(gbx_lrc_game->x()+65, pbn_lrc_game_state->y());
        pbn_lrc_game->move(gbx_lrc_game->pos());
        pbn_lrc_game_state ->setText(tr("歌词"));
        pbn_lrc_game->setText(tr("小游戏"));
        pbn_lrc_game_state ->setToolTip(tr("显示歌词"));
        pbn_lrc_game ->setToolTip(tr("显示歌词"));

        gameState = true;
        emit gameStateChange(true);
    }
    else
    {
        pbn_lrc_game_state->move(gbx_lrc_game->x()+5, pbn_lrc_game_state->y());
        pbn_lrc_game->move(gbx_lrc_game->x()+46, pbn_lrc_game->y());
        pbn_lrc_game_state->setText(tr("小游戏"));
        pbn_lrc_game->setText(tr("歌词"));
        pbn_lrc_game_state->setToolTip(tr("本地小游戏"));
        pbn_lrc_game->setToolTip(tr("本地小游戏"));

        gameState = false;
        emit gameStateChange(false);
    }
}

//开启定时器
void ToolGlobal::startTiming()
{
    //获得用户设定时间
    int widgetHour = timing->get_spbHour_value();
    int widgetMinute = timing->get_spbMinite_value();
    int widgetSecond = timing->get_spbSecond_value();
    long widgetTotal = widgetHour * 3600 + widgetMinute * 60 + widgetSecond;

    int timingMode = timing->get_timingMode();
    TimingTarget = (WhatTimingDo)timing->get_timingTarget();

    //获得用户处理方式
    switch(timingMode)
    {
    case -2:             //定时
        timer_totalTime = widgetTotal;
        break;
    case -3:             //倒计时
        if (true)
        {
            QTime sysTime = QTime::currentTime();
            int sysHour = sysTime.hour();
            int sysMinute = sysTime.minute();
            int sysSecond = sysTime.second();
            long sysTotal = sysHour * 3600 + sysMinute * 60 + sysSecond;
            timer_totalTime = widgetTotal - sysTotal;
            if (widgetTotal <= 0)
                return;
        }
        break;
    default:            //异常
        return;
    }

    //关闭设置窗口
    timing->close();

    //开启定时器
    timer_default ->start(1000);
    timer_target ->start(timer_totalTime * 1000);

    //创建LCD显示器
    lcdNumber = new QLCDNumber(widget_timingLCD);
    lcdNumber->resize(lcdNumber->parentWidget()->size());
    lcdNumber ->setDigitCount(8);
    int hour = timer_totalTime / 3600;
    int minite = (timer_totalTime - hour *3600) / 60;
    int second = timer_totalTime - hour *3600 - minite * 60;
    lcdNumber ->display(tr("%1").sprintf("%02d:%02d:%02d", hour, minite, second));
    lcdNumber ->show();

    //设置LCD样式
    lcdNumber ->setStyleSheet(
                "QLCDNumber{"
                    "border: 0;"
                "}"
                );
}

//结束定时器
void ToolGlobal::stopTiming()
{
    pbn_clock_clicked();
}

//1秒倒计时 处理
void ToolGlobal::timer_default_timeout()
{
    if (--timer_totalTime < 0)
    {
//        timer_totalTime = 0;
        timer_default->stop();
        lcdNumber->deleteLater();
        clockState = false;
    }
    int hour = timer_totalTime / 3600;
    int minite = (timer_totalTime - hour *3600) / 60;
    int second = timer_totalTime - hour *3600 - minite * 60;
    lcdNumber ->display(tr("%1").sprintf("%02d:%02d:%02d", hour, minite, second));
}

//任务定时器 处理
void ToolGlobal::timer_target_timeout()
{
    switch(TimingTarget)        //停止播放/退出/关机
    {
    case playStop:
        emit timeout_playStop();
        break;
    case windowClose:
        this->parentWidget()->parentWidget()->close();
        break;
    case shutdown:
        system("shutdown -s -t 0");
        break;
    }
}
