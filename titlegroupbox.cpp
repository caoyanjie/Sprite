#include "titlegroupbox.h"
#include <QDebug>

#include "setting.h"
#include "theme.h"
#include "musicminiplayer.h"

#include <QMouseEvent>
//#include <QPaintEvent>
#include <QPainter>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QToolButton>
#include <QMouseEvent>
#include <QTimer>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDesktopWidget>
const int tbn_switchMusicVideo_width = 70;
const int tbn_switchMusicVideo_height = 38;

TitleGroupBox::TitleGroupBox(QWidget *parent) :
    QGroupBox(parent)
  ,rotateAngle(0)
  ,angleOffset(30)
  ,timer_rotateStart(60)
  ,timer_rotatePause(2000)
{
    setTitleUi();   //设计头部
    setting = new Setting(this);                        //创建 “设置” 对话框
    theme = new Theme(this);                            //创建 “主题” 对话框

//    tbn_globalMusic_icon ->installEventFilter(this);    //安装事件过滤器
//    tbn_globalMusic ->installEventFilter(this);         //安装事件过滤器
//    tbn_internet_Music_icon ->installEventFilter(this); //安装事件过滤器
//    tbn_internet_Music ->installEventFilter(this);      //安装事件过滤器
    tbn_closeWindow ->installEventFilter(this);         //安装事件过滤器
    tbn_miniWindow ->installEventFilter(this);          //安装事件过滤器
    tbn_setting ->installEventFilter(this);             //安装事件过滤器
    tbn_theme ->installEventFilter(this);               //安装事件过滤器

    connect(tbn_miniWindow, SIGNAL(clicked()), parent, SLOT(showMinimized()));  //最小化窗口
    connect(tbn_closeWindow, SIGNAL(clicked()), parent, SLOT(close()));         //关闭窗口
    connect(tbn_setting, SIGNAL(clicked()), this, SLOT(settingClicked()));
    connect(tbn_theme, SIGNAL(clicked()), this, SLOT(themeClicked()));
    connect(tbn_simple, SIGNAL(clicked()), this, SLOT(showMiniWindow()));
    //Why????
    connect(theme, SIGNAL(themeClick(int)), this, SIGNAL(themeNumClick(int)));

    //
    connect(theme, SIGNAL(settingDataChanged(QString,QString)), this, SIGNAL(settingDataChanged(QString,QString)));
    setMouseTracking(true);
}

//头部忽略鼠标按下事件
void TitleGroupBox::mousePressEvent(QMouseEvent *event)
{
    event ->ignore();
}

//头部忽略鼠标移动事件
void TitleGroupBox::mouseMoveEvent(QMouseEvent *event)
{
    event ->ignore();
}

//重绘事件
void TitleGroupBox::paintEvent(QPaintEvent *event)
{
    QPixmap pix;
    pix.load(":/Images/logo_rotate.png");

    QPainter painter(this);
    painter.translate(lab_logo->x()+lab_logo->width()/2, lab_logo->y()+lab_logo->height()/2);
    painter.rotate(rotateAngle -= angleOffset);
//    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(-lab_logo->width()/2, -lab_logo->width()/2, lab_logo->width(), lab_logo->height(), pix);
}

//事件过滤器
bool TitleGroupBox::eventFilter(QObject *target, QEvent *event)
{
    if(target == tbn_closeWindow
            || target == tbn_miniWindow
            || target == tbn_setting
            || target == tbn_theme
            )
    {
        if (event ->type() == QEvent::MouseMove)
        {
//            event ->accept();
        }
        else
        return QGroupBox::eventFilter(target, event);
    }
}

//设计头部
void TitleGroupBox::setTitleUi()
{
    this ->setObjectName(tr("titleGroupBox"));

    //创建按钮
    lab_logo = new QLabel(this);                    //创建 Logo 图标
    lab_logoWords = new QLabel(this);               //创建 Logo 文字
    tbn_closeWindow = new QToolButton(this);        //创建 退出 按钮
    tbn_hideWindow = new QToolButton(this);         //创建 关闭 按钮
    tbn_miniWindow = new QToolButton(this);         //创建 最小化 按钮
    tbn_setting = new QToolButton(this);            //创建 设置 按钮
    tbn_theme = new QToolButton(this);              //创建 主题 按钮
    tbn_simple = new QToolButton(this);             //创建 MiNi窗口 按钮
    tbn_simple->setText("精简");                     //................
    tbn_normal = new QToolButton(this);             //创建 正常窗口 按钮
    tbn_normal->setText("标准");                     //...............
    tbn_switch_music = new QToolButton(this);       //创建 切换音乐模式 按钮
    tbn_switch_music->setText("音 乐");              //..................
    tbn_switch_video = new QToolButton(this);       //创建 切换视频模式 按钮
    tbn_switch_video->setText("视 频");              //..................
    lab_bearHead = new QLabel(this);                //创建 小熊头 图标
    lab_bearFoot = new QLabel(this);                //创建 小熊脚 图标
    lab_logo->setAlignment(Qt::AlignCenter);

    //设置按钮固定大小
    tbn_closeWindow->setFixedSize(11, 11);
    tbn_hideWindow->setFixedSize(11, 11);
    tbn_miniWindow->setFixedSize(11, 11);
    tbn_setting->setFixedSize(13, 13);
    tbn_theme->setFixedSize(15, 15);
    tbn_simple->setFixedSize(30, 14);
    tbn_normal->setFixedSize(30, 14);
    tbn_switch_music->setFixedSize(tbn_switchMusicVideo_width, tbn_switchMusicVideo_height);
    tbn_switch_video->setFixedSize(tbn_switchMusicVideo_width, tbn_switchMusicVideo_height);
    lab_logo->setFixedSize(50, 50);
    lab_logoWords->setFixedSize(51, 31);
    lab_bearHead->setFixedSize(41, 29);
    lab_bearFoot->setFixedSize(18, 6);

    //设置按钮提示
    tbn_closeWindow->setToolTip("退出程序");
    tbn_hideWindow->setToolTip("最小化到系统托盘");
    tbn_miniWindow->setToolTip("最小化");
    tbn_setting->setToolTip("设置");
    tbn_theme->setToolTip("更换主题");

    //布局
    layout_H = new QHBoxLayout;
    layout_H->addStretch();
    layout_H->addWidget(tbn_simple);
    layout_H->addWidget(tbn_normal);
    layout_H->addWidget(tbn_theme);
    layout_H->addWidget(tbn_setting);
    layout_H->addWidget(tbn_miniWindow);
    layout_H->addWidget(tbn_hideWindow);
    layout_H->addWidget(tbn_closeWindow);
    layout_H->setSpacing(9);
    layout_H->setContentsMargins(0, 0, 11, 0);

    //顶级布局
    layout_top = new QGridLayout;
    layout_top->addWidget(lab_logo, 8, 10, 50, 45);
    layout_top->addWidget(lab_logoWords, 53, 10, 31, 51);
    layout_top->addWidget(tbn_switch_music, 20, 500, tbn_switchMusicVideo_height, tbn_switchMusicVideo_width);
    layout_top->addWidget(lab_bearHead, 4, 500+(tbn_switchMusicVideo_height-29)/2, 29, 41);
    layout_top->addWidget(lab_bearFoot, 20+tbn_switchMusicVideo_height, 400+(tbn_switchMusicVideo_width-18)/2, 6, 18);
    layout_top->addWidget(tbn_switch_video, 20, 600, tbn_switchMusicVideo_height, tbn_switchMusicVideo_width);
    layout_top->addLayout(layout_H, 6, 650, 15, 50);
    layout_top->setSpacing(0);
    layout_top->setMargin(0);
    layout_top->setContentsMargins(10, 8, 0, 0);
    setLayout(layout_top);

    tbn_switch_video->setToolTip("切换到视频播放");
//    lab_bearHead->setGeometry(tbn_switch_music->x()+(tbn_switch_music->width()-41)/2, tbn_switch_music->y()-16, 41, 29);
//    lab_bearFoot->setGeometry(tbn_switch_music->x()+(tbn_switch_music->width()-18)/2, tbn_switch_video->y()+tbn_switchMusicVideo_height, 18, 6);

    //显示设置按钮对象名称
    lab_logo->setObjectName("lab_logo");
    lab_logoWords->setObjectName("lab_logoWords");
    tbn_closeWindow->setObjectName("tbn_closeWindow");
    tbn_hideWindow->setObjectName("tbn_hideWindow");
    tbn_miniWindow->setObjectName("tbn_miniWindow");
    tbn_setting->setObjectName("tbn_setting");
    tbn_theme->setObjectName("tbn_theme");
    tbn_simple->setObjectName("tbn_simple_head");
    tbn_normal->setObjectName("tbn_normal_head");
    tbn_switch_music->setObjectName("tbn_switch_music");
    tbn_switch_video->setObjectName("tbn_switch_video");
    lab_bearHead->setObjectName("lab_bearHead");
    lab_bearFoot->setObjectName("lab_bearFoot");
/*
    QMatrix logoMatrix;
    logoMatrix.rotate(rotateAngle -= angleOffset);
    lab_logo->setPixmap(QPixmap(":/Images/logo_rotate.png").transformed(logoMatrix,Qt::SmoothTransformation));
*/
    //设置样式表
    setStyleSheet(
                "#titleGroupBox{"
//                    "border-image: url(:/Images/bgTool1.png);"
//                    "background: rgba(0, 0, 0, 100);"
                    "border: 0 solid white;"
                    "background: rgba(51, 73, 100, 180);"
                "}"

                //设置 Logo 样式表
                "#lab_logo{"
//                    "border-image: url(:/Images/logo.png);"
                "}"
                "#lab_logoWords{"
                    "border-image: url();"
                "}"
                //设置 关闭窗口 样式表
                "#tbn_closeWindow{"
                    "border-image: url(:/Images/closeWindow.png);"
                "}"
                "#tbn_closeWindow:hover{"
                    "border-image: url(:/Images/closeWindow_hover.png);"
                "}"

                "#tbn_hideWindow{"
                    "border-image: url(:/Images/hideWindow.png);"
                "}"
                "#tbn_hideWindow:hover{"
                    "border-image: url(:/Images/hideWindow_hover.png);"
                "}"

                "#tbn_miniWindow{"                                               //设置 最小化按钮 样式表
                    "border-image: url(:/Images/miniWindow.png);"
                "}"
                "#tbn_miniWindow::hover{"
                    "border-image: url(:/Images/miniWindow_hover.png);"
                "}"

                "#tbn_setting{"                                                  //设置 设置按钮 样式表
                    "border-image: url(:/Images/setting.png);"
                "}"
                 "#tbn_setting:hover{"
                    "border-image: url(:/Images/setting_hover.png);"
                "}"

                "#tbn_theme{"
                    "border-image: url(:/Images/theme.png);"
                "}"
                "#tbn_theme::hover{"
                    "border-image: url(:/Images/theme_hover.png);"
                "}"

                "#tbn_simple_head{"
                    "background: rgba(0, 0, 0, 0);"
                    "color: rgb(23, 215, 255);"
                "}"

                "#tbn_normal_head{"
                    "background: rgba(0, 0, 0, 0);"
                    "color: gray;"
                "}"

                "#tbn_switch_music, #tbn_switch_video{"
//                    "background: rgba(0, 0, 0, 100);"
                    "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(255, 255, 255, 230), stop:0.4 rgba(50, 50, 50, 100), stop:0.5 rgba(50, 50, 50, 100), stop:0.6 rgba(50, 50, 50, 100), stop:1 rgba(255, 255, 255, 230));"
                    "border: 1px solid rgba(255, 255, 255, 60);"
                    "border-radius: 2px;"
                    "font-size: 11pt;"
                    "color: rgba(200, 255, 255, 255);"
                "}"

                "#tbn_switch_music:hover, #tbn_switch_video:hover{"
                    "background: rgba(0, 0, 0, 130);"
                "}"

                "#lab_bearHead{"
                    "border-image: url(:/Images/bear_head.png);"
                "}"
                "#lab_bearFoot{"
                    "border-image: url(:/Images/bear_foot.png);"
                "}"

//              "#tbn_switch_music_video{"
//                    "border-image: url(:/Images/switch_video.png);"
//              "}"
                );


    lab_logoWords ->setText("<html><body><p><span style='color:#ffffff;'>兔兔音乐</span></p></body></html>");

    connect(tbn_switch_video, SIGNAL(clicked()),                      //播放视频
            this, SLOT(ShowVideoPlayerSlot()));

    timer_logoRotate = new QTimer(this);
    timer_logoRotatePause = new QTimer(this);
    connect(timer_logoRotate, SIGNAL(timeout()),
            this, SLOT(timeout_logoRotate()));
    connect(timer_logoRotatePause, SIGNAL(timeout()),
            this, SLOT(timeout_logoRotetaRedo()));
    timer_logoRotate->start(timer_rotateStart);

}

//单击 设置 槽函数
void TitleGroupBox::settingClicked()
{
    setting ->show();
}

//单击 换肤 槽函数
void TitleGroupBox::themeClicked()
{
    //显示 设置主题窗口
    theme ->show();

    //获得当前主题， 以便选择“取消”时恢复
    QStringList getData;
    QFile iniData(".data.ini");
    if (iniData.open(QIODevice::ReadWrite))
    {
        QTextStream in(&iniData);
        while (! in.atEnd())
        {
            getData.append(in.readLine());
        }
        if (! getData.isEmpty())
        {
            for (int i=0; i<getData.length(); i++)
            {
                QStringList line = getData[i].split(":");
                if (line[0] == "theme")
                {
                    theme ->oldTheme = line[1].toInt();
                    break;
                }
                else if (i == getData.length() - 1)
                {
                    if (line[0] != "theme")
                    {
                        theme ->oldTheme = 1;
                    }
                }
            }
        }

        //关闭文件
        iniData.close();
    }
}

//切换精简播放窗口
void TitleGroupBox::showMiniWindow()
{
    QDesktopWidget desktop;
    MusicMiniPlayer *miniWindow = new MusicMiniPlayer(this);
    miniWindow->move(desktop.width()-miniWindow->width()-40, 40);
    miniWindow->show();
    parentWidget()->parentWidget()->hide();
}

//单击 ”本地音乐“ 槽函数
void TitleGroupBox::clickedGlobalMusic()
{
    setStyleSheet("QGroupBox{"
//                        "border-image: url(:/Images/bgTitle1_1);"
                        "border: 1px;"
                  "}"
                  );
    emit clickGlobalMusic();
}

//单击 ”网络歌曲“ 槽函数
void TitleGroupBox::clickedInternetMusic()
{
    setStyleSheet("QGroupBox{"
                        "border-image: url(:/Images/bgTitle1_2)"
                        "border: 1px;"
                  "}"
                  );
    emit clickInternetMusic();
}

void TitleGroupBox::ShowVideoPlayerSlot()
{
    lab_bearHead->move(tbn_switch_video->x()+(tbn_switch_video->width()-41)/2, tbn_switch_video->y()-16);
    lab_bearFoot->move(tbn_switch_video->x()+(tbn_switch_video->width()-18)/2, tbn_switch_video->y()+tbn_switchMusicVideo_height);
    timer = new QTimer(this);
    timer->start(200);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(timeout()));
}

void TitleGroupBox::timeout()
{
    emit ShowVideoPlayer();
    timer->deleteLater();

}

//小熊移动到音乐按钮上
void TitleGroupBox::timeoutBear()
{
    lab_bearHead->move(tbn_switch_music->x()+(tbn_switch_music->width()-41)/2, tbn_switch_music->y()-16);
    lab_bearFoot->move(tbn_switch_music->x()+(tbn_switch_music->width()-18)/2, tbn_switch_music->y()+tbn_switchMusicVideo_height);
}

//logo 旋转
void TitleGroupBox::timeout_logoRotate()
{
    if (rotateAngle % (360*2) == 0)
    {
        timer_logoRotate->stop();
        timer_logoRotatePause->start(timer_rotatePause);
        rotateAngle = -angleOffset;
        return;
    }
/*
    QMatrix logoMatrix;
    logoMatrix.rotate(rotateAngle -= angleOffset);
    lab_logo->setPixmap(QPixmap(":/Images/logo_rotate.png").transformed(logoMatrix,Qt::SmoothTransformation));
*/
    update();
}

//logo 旋转暂停恢复
void TitleGroupBox::timeout_logoRotetaRedo()
{
    timer_logoRotate->start(timer_rotateStart);
}
