#include "timing.h"

#include <QLabel>
#include <QSpinBox>
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>


Timing::Timing(int x, int y, int w, int h, QWidget *parent) :
    QGroupBox(parent)
{
    //设置窗口样式
    this->setGeometry(x, y, w, h);
    this->setObjectName(tr("timing"));

    //初始化小部件
    lab_bg1 = new QLabel(this);
    lab_bg1 ->setObjectName(tr("lab_bg1"));
    lab_bg2 = new QLabel(this);
    lab_bg2 ->setObjectName(tr("lab_bg2"));
    lab_bg3 = new QLabel(this);
    lab_bg3 ->setObjectName(tr("lab_bg3"));

    rbn_countDown = new QRadioButton("倒计时", this);
    rbn_timing = new QRadioButton("定时", this);
    spb_hour = new QSpinBox(this);
    lab_hour = new QLabel("时", this);
    spb_minite = new QSpinBox(this);
    lab_minite = new QLabel("分", this);
    spb_second = new QSpinBox(this);
    lab_second = new QLabel("秒", this);
    rbn_playStop = new QRadioButton("停止播放", this);
    rbn_windowClose = new QRadioButton("退出", this);
    rbn_shutdown = new QRadioButton("关机", this);
    pbn_ok = new QPushButton("确认", this);
    pbn_cancle = new QPushButton("取消", this);

    //单选按钮 分组
    groupButton1 = new QButtonGroup(this);
    groupButton2 = new QButtonGroup(this);
    groupButton1 ->addButton(rbn_countDown);
    groupButton1 ->addButton(rbn_timing);
    groupButton2 ->addButton(rbn_playStop);
    groupButton2 ->addButton(rbn_windowClose);
    groupButton2 ->addButton(rbn_shutdown);

    //设置部件位置大小
    rbn_countDown ->move(60, 10);
    rbn_timing ->move(130, 10);
    spb_hour ->move(20, 40);
    lab_hour ->setGeometry(65, 41, 15, 15);
    spb_minite ->move(85, 40);
    lab_minite ->setGeometry(130, 41, 15, 15);
    spb_second ->move(148, 40);
    lab_second ->setGeometry(194, 41, 15, 15);
    rbn_playStop ->move(20, 75);
    rbn_windowClose ->move(100, 75);
    rbn_shutdown ->move(160, 75);
    pbn_ok ->setGeometry(40, 105, 50, 20);
    pbn_cancle ->setGeometry(130, 105, 50, 20);

    lab_bg1 ->setGeometry(0, 5, 230, 25);
    lab_bg2 ->setGeometry(0, 36, 230, 25);
    lab_bg3 ->setGeometry(0, 70, 230, 25);

    //设置部件 初始状态
    rbn_countDown ->setChecked(true);
    rbn_windowClose ->setChecked(true);

    this->setStyleSheet(
        "#timing {"
//                "background: rgb(43, 45, 163);"
//                "background: rgb(60, 21, 124);"
            "background: rgba(0, 0, 0, 100);"
            "border: 0;"
        "}"
        "QPushButton{"
            "background: rgb(246, 237, 144);"
            "border-radius: 10px;"
        "}"
        "QRadioButton{"
            "color: white;"
        "}"
        "QLabel{"
            "color: white;"
        "}"
        "#lab_bg1, #lab_bg2, #lab_bg3{"
            "background: rgba(0, 0, 0, 50);"
        "}"
        );

    //信号槽关联
    connect(pbn_ok, SIGNAL(clicked()), this, SIGNAL(timing_pbnOk_click()));
    connect(pbn_cancle, SIGNAL(clicked()), this, SIGNAL(timing_pbnCancle_click()));
}

//获得spb_hour部件的值
int Timing::get_spbHour_value()
{
    return spb_hour ->value();
}

//获得spb_minite部件的值
int Timing::get_spbMinite_value()
{
    return spb_minite ->value();
}

//获得spb_second部件的值
int Timing::get_spbSecond_value()
{
    return spb_second ->value();
}

//获得定时模式
int Timing::get_timingMode()
{
    return groupButton1->checkedId();
}

//获得处理方式
int Timing::get_timingTarget()
{
    return groupButton2->checkedId();
}
