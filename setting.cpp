#include "setting.h"

#include <QPushButton>
#include <qdesktopwidget.h>
#include <QMouseEvent>

#include <QTabWidget>
#include <QKeySequenceEdit>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>

Setting::Setting(QWidget *parent) :
    QWidget(parent)
{
    //窗口去边框
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //窗口透明
    setAttribute(Qt::WA_TranslucentBackground);

    //窗口居中显示
    QDesktopWidget desktopWidget;
    setGeometry(
				(desktopWidget.width() - 510) / 2,
				(desktopWidget.height() - 400) / 2,
				510,
				400         //Windows OS
//                370         //Linux OS
				);
//##    setGeometry(desktopWidget.width() / 2 - 510 / 2, desktopWidget.height() / 2 - 400 / 2, 510, 370);

    //设置样式
    setObjectName(tr("setting"));
    setStyleSheet(
                  "#setting{"
//                        "background: rgb(43, 45, 163);"
                  "}"
                  );

    //初始化 部件
    lab_background = new QLabel(this);

    tabWidget = new QTabWidget(this);

    tab_shortcutKey = new QWidget();
    tab_behaviour = new QWidget();

    lab_play_previous = new QLabel("<html><body><font color=#c8ffff>上一曲</html></body></font>", tab_shortcutKey);
    lab_play_pause = new QLabel("<html><body><font color=#c8ffff>播放/暂停</html></body></font>", tab_shortcutKey);
    lab_play_next = new QLabel("<html><body><font color=#c8ffff>下一曲</html></body></font>", tab_shortcutKey);
    lab_volume_reduce = new QLabel("<html><body><font color=#c8ffff>增加音量</html></body></font>", tab_shortcutKey);
    lab_volume_add = new QLabel("<html><body><font color=#c8ffff>减少音量</html></body></font>", tab_shortcutKey);
    lab_addMusic = new QLabel("<html><body><font color=#c8ffff>添加音乐</html></body></font>", tab_shortcutKey);

    key_play_previous = new QKeySequenceEdit(Qt::Key_Left, tab_shortcutKey);
    key_play_pause = new QKeySequenceEdit(Qt::Key_Space, tab_shortcutKey);
    key_play_next = new QKeySequenceEdit(Qt::Key_Right, tab_shortcutKey);
    key_volume_reduce = new QKeySequenceEdit(Qt::Key_Up, tab_shortcutKey);
    key_volume_add = new QKeySequenceEdit(Qt::Key_Down, tab_shortcutKey);
    key_addMusic = new QKeySequenceEdit(tr("Ctrl + O"), tab_shortcutKey);

    //第二个面板
    lab_closeMethod = new QLabel("<font size=4>关闭面板时：</font>", tab_behaviour);
    rbn_mini = new QRadioButton("退出主面板时，最小化到系统托盘，不退出程序", tab_behaviour);
    rbn_close = new QRadioButton("推出程序", tab_behaviour);

    lab_lrc = new QLabel("<font size=4>桌面歌词样式：</font>", tab_behaviour);
    lab_lrcSize = new QLabel("字体大小：", tab_behaviour);               //字体大小
    ln_lrcSize = new QLineEdit(tab_behaviour);                         //.......
    lab_lrcColor = new QLabel("字体颜色", tab_behaviour);                //字体颜色
    ln_lrcColor = new QLineEdit(tab_behaviour);                         //......

    pbn_ok = new QPushButton("确认", this);
    pbn_cancle = new QPushButton("取消", this);
    pbn_default = new QPushButton("恢复默认", this);

    pbn_close = new QPushButton(this);

    //设置 部件位置大小
    lab_background ->setGeometry(0, 0, 510, 400);       //创建 背景， 设置背景半透明

    pbn_close ->setGeometry(490, 8, 11, 11);
    tabWidget ->setGeometry(0, 0, 520, 350);

    lab_play_previous ->setGeometry(50, 50, 100, 20);
    lab_play_pause ->setGeometry(50, 90, 100, 20);
    lab_play_next ->setGeometry(50, 130, 100, 20);
    lab_volume_add ->setGeometry(50, 170, 100, 20);
    lab_volume_reduce ->setGeometry(50, 210, 100, 20);
    lab_addMusic ->setGeometry(50, 250, 100, 20);

    key_play_previous ->setGeometry(150, 50, 150, 20);
    key_play_pause ->setGeometry(150, 90, 150, 20);
    key_play_next ->setGeometry(150, 130, 150, 20);
    key_volume_reduce ->setGeometry(150, 170, 150, 20);
    key_volume_add ->setGeometry(150, 210, 150, 20);
    key_addMusic ->setGeometry(150, 250, 150, 20);

    //第二个面板
    lab_closeMethod ->move(20, 30);
    rbn_mini ->move(50, 70);            //退出时最小化
    rbn_close ->move(50, 100);          //退出时关闭
    lab_lrc ->move(20, 140);            //歌词样式
    lab_lrcSize ->move(50, 170);        //字体大小
    ln_lrcSize ->move(50, 190);         //.......
    ln_lrcSize ->setText("12");         //.......
    lab_lrcColor ->move(50, 230);       //字体颜色
    ln_lrcColor ->move(50, 250);        //.......

    rbn_mini ->setChecked(true);

    pbn_ok ->setGeometry(75, 370, 60, 20);
    pbn_cancle ->setGeometry(213, 370, 60, 20);
    pbn_default ->setGeometry(355, 370, 60, 20);

/*
    //为部件 安装事件过滤器
    tabWidget ->installEventFilter(this);
    key_play_previous ->installEventFilter(this);
    key_play_pause ->installEventFilter(this);
    key_play_next ->installEventFilter(this);
    key_volume_reduce ->installEventFilter(this);
    key_volume_add ->installEventFilter(this);
*/
    //设置 部件样式
    lab_background ->setStyleSheet(                                         //设置 背景半透明
                "QLabel{"
                    "background: rgba(0, 0, 0, 80);"
//                    "background: rgba(40, 62, 83, 195);"
                "}"
                );

    pbn_close ->setStyleSheet(                                              //关闭窗口 按钮
                "QPushButton{"
                    "border-image: url(:/Images/closeWindow.png);"
                "}"
                "QPushButton::hover{"
                    "border-image: url(:/Images/closeWindow_hover.png);"
                "}"
                );
/*    tab_shortcutKey ->setStyleSheet(
                "QWidget{"
                    "background: rgba(0, 0, 0, 100);"
                    "border: 0;"
                "}"
                );
                */

    tabWidget ->setStyleSheet(                                              //面板
                "QTabBar::tab {"
                    "border: 0;"
//                    "margin: 0 10px;"
//                    "color: white;"
                    "width: 120px;"
                    "height: 35px;"
                    "border-top-left-radius: 5px;"
                    "border-top-right-radius: 5px;"
                    "color: rgb(200, 255, 255;)"
                "}"
                "QTabBar::tab:selected{"
                    "background: rgba(25, 0, 0, 40);"
                "}"
                "QTabWidget:pane {"
                    "border: 0;"
                    "background: rgba(25, 0, 0, 40);"
                "}"
                "QRadioButton{"                         //设置第二个面板中 单选按钮样式
                    "color: #c8ffff;"
                "}"
                "QLabel{"                               //设置第二个面板中 label标签样式
                    "color: #c8ffff;"
                "}"
                );

    key_play_previous ->setStyleSheet("QToolButton{"
                                    "border-image: url(:/Images/bg10.jpg);}"
                              "QToolButton::hover{"
//                                    "border-image: url(:/Images/preview1.png);
                                    "border: 1px;"
                               "}"
                               );
    key_play_pause ->setStyleSheet("QToolButton{"
                                    "border-image: url(:/Images/bg2.jpg);}"
                              "QToolButton::hover{"
//                                    "border-image: url(:/Images/preview2.png);"
                                    "border: 1px;"
                                "}"
                               );
    key_play_next ->setStyleSheet("QToolButton{"
                                    "border-image: url(:/Images/bg3.jpg);}"
                              "QToolButton::hover{"
//                                    "border-image: url(:/Images/preview3.png);
                                    "border: 1px;"
                                "}"
                               );
    key_volume_reduce ->setStyleSheet("QToolButton{"
                                    "border-image: url(:/Images/bg4.jpg);}"
                              "QToolButton::hover{"
//                                    "border-image: url(:/Images/preview4.png);"
                                    "border: 1px;"
                                "}"
                               );
    key_volume_add ->setStyleSheet("QToolButton{"
                                    "border-image: url(:/Images/bg5.jpg);}"
                              "QToolButton::hover{"
//                                    "border-image: url(:/Images/preview5.png);"
                                    "border: 1px;"
                                "}"
                               );

    //第二个面板
    pbn_ok ->setStyleSheet(
                "QPushButton{"
                    "background: rgba(246, 237, 144);"
                    "border-radius: 10px;"
                "}"
                "QPushButton::hover{"
//                  "border-image: url(:/Images/preview5.png);"
                    "border: 1px;"
                "}"
                );
    pbn_cancle ->setStyleSheet(
                "QPushButton{"
                    "background: rgb(246, 237, 144);"
                    "border-radius: 10px;"
                "}"
                "QPushButton::hover{"
//                  "border-image: url(:/Images/preview5.png);"
                    "border: 1px;"
                "}"
                );
    pbn_default ->setStyleSheet(
                "QPushButton{"
                    "background: rgba(246, 237, 144);"
                    "border-radius: 10px;"
                "}"
                "QPushButton::hover{"
//                  "border-image: url(:/Images/preview5.png);"
                    "border: 1px;"
                "}"
                );

    //设置
    tabWidget ->addTab(tab_shortcutKey, tr("快捷键"));
    tabWidget ->addTab(tab_behaviour, tr("操作习惯"));

    //信号 槽
    connect(pbn_close, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(pbn_cancle, SIGNAL(clicked()),
            this, SLOT(close()));
}

//重写 鼠标按下事件
void Setting::mousePressEvent(QMouseEvent *event)
{
    offset = event ->globalPos() - pos();
}

//重写 鼠标移动事件
void Setting::mouseMoveEvent(QMouseEvent *event)
{
    QPoint temp = event ->globalPos() - offset;
    move(temp);
}

//事件过滤器
bool Setting::eventFilter(QObject *target, QEvent *event)
{
    /*    if(target == tbn_close ||
                target == tbn_theme1 ||
                target == tbn_theme2 ||
                target == tbn_theme3 ||
                target == tbn_theme4 ||
                target == tbn_theme5 ||
                target == tbn_theme6 ||
                target == tbn_theme7 ||
                target == tbn_theme8 ||
                target == tbn_theme9 ||
                target == tbn_ok ||
                target == tbn_cancle ||
                target == tbn_apply)
        {
            if (event ->type() == QEvent::MouseMove)
            {
                event ->accept();
            }
            else
            {
                return QWidget::eventFilter(target, event);
            }
        }*/
}
