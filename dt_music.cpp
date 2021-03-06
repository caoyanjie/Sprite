/***
 *  database: "create table setUp(theme, volumn, playMode)"
 *
 *
 *
 *
 *
 ***/

#include "dt_music.h"
#include "ui_dt_music.h"
#include <QDebug>

//自定义类
#include "titlegroupbox.h"
#include "toolglobal.h"
#include "toolinternet.h"
#include "lrcview.h"
#include "bottomgroupbox.h"
#include "playmodle.h"
#include "desktoplrc.h"
#include "create_musiclist.h"
#include "timing.h"
#include "currentlrc.h"
#include "videoplayer.h"        //视频播放器
#include "game.h"
//#include "databaseoperation.h"
#include "xmlprocess.h"
#include "networkwidget.h"

//部件类
#include <QMouseEvent>          //鼠标事件
#include <QCloseEvent>          //关闭事件
#include <QScrollBar>
#include <QComboBox>
#include <QListView>
#include <QSlider>
#include <QLineEdit>
#include <QFont>                //定义字体
#include <QToolButton>
#include <QStringList>

//文件操作
#include <QFileDialog>          //文件对话框
#include <QFile>                //文件操作
#include <QDir>                 //目录操作
#include <QMessageBox>          //警告框
#include <QTextStream>          //输入输出流
#include <QFileInfo>            //文件信息
//#include <stdlib.h>             //调用系统命令
//#include <windows.h>            //调用系统命令

//多媒体模块
#include <QMediaPlayer>         //多媒体播放器
#include <QMediaPlaylist>       //播放列表

//辅助功能
#include <QEvent>               //事件
#include <QTime>                //定时器
#include <QMenu>                //自定义 右键菜单
#include <QCompleter>           //搜索框 自动补全
#include <QDesktopWidget>       //计算窗口值
#include <QTimer>

//布局管理器
#include <QVBoxLayout>

//数据库
#include <QSqlQuery>
#include <QSqlError>

//动画
#include <QPropertyAnimation>

DT_Music::DT_Music(QString programPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DT_Music)
  ,programDir(programPath)
//  ,musicListDatabaseName(programPath + "musicList.db")
//  ,setUpDatabaseName(programPath + "setUp.db")
  ,xmlPath(programPath + "ini.xml")
  ,xml(xmlPath)
  ,ThemeDefultValue(1)
  ,VolumnDefaultValue(40)
  ,PlayModeDefaultValue(4)
{
    ui->setupUi(this);

    //重写窗口框架， 设置 无边框 和 允许最小化和还原
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

    //设置窗口出现位置
    QDesktopWidget desktopWidget;
    this->setGeometry((desktopWidget.width()-this->width())/2, (desktopWidget.height()-this->height())/2, 969, 639);

    //渐变出现
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    // 检测配置文件
    if (!QFileInfo(xmlPath).exists())
    {
        xml.initXmlFile();
    }

    //初始化 界面 和 变量
    init();
    network = new NetWork;
    connect(network, SIGNAL(lrcDownloadFinished()), this, SLOT(durationChanged()));

    //为部件安装事件过滤器
    cob_addMusic ->installEventFilter(this);
    tbn_createList ->installEventFilter(this);
    tbn_search ->installEventFilter(this);
    lab_volumnFrame->installEventFilter(this);
    playModle->installEventFilter(this);

    //多媒体信息改变
    connect(musicList->player, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged()));//多媒体时长信息出现，处理歌词
    connect(musicList->player, SIGNAL(positionChanged(qint64)), this, SLOT(updateLrc(qint64)));//播放进度 改变 更新歌词

    //播放控制按钮操作(bottomGroupbox)
    connect(bottomGroupbox, SIGNAL(lrc_click(bool)), this, SLOT(setLrcShown(bool)));            //显示/隐藏 桌面歌词
    connect(bottomGroupbox, SIGNAL(showVolumn(QPoint)), this, SLOT(showVolumn(QPoint)));        //音量     处理
    connect(bottomGroupbox, SIGNAL(showPlayModle(QPoint)), this, SLOT(showPlayModle(QPoint)));  //显示/隐藏 “播放模式”窗口
    connect(bottomGroupbox, SIGNAL(musicTitleAndAuthor(QString)),
            titleGroupBox, SIGNAL(musicTitleAndAuthor(QString)));

    //播放模式控件窗口操作
    connect(playModle, SIGNAL(playModel_choose(PlayModle::PlayMode)),                           //选择播放模式，为bottom设置相应图标
            bottomGroupbox, SLOT(playModle_choosed(PlayModle::PlayMode)));

    //搜索按钮
    connect(tbn_search, SIGNAL(clicked()), this, SLOT(searchMusic()));                          //单击 搜索按钮

    connect(toolGlobal, SIGNAL(magicStateChange(bool)), this, SLOT(maigcStateChanged(bool)));   //魔音状态改变
    connect(toolGlobal,SIGNAL(gameStateChange(bool)), this, SLOT(gameStateChange(bool)));
    connect(titleGroupBox, SIGNAL(themeNumClick(int)), this, SLOT(setTheme(int)));              //设定主题

    //倒计时 处理
    connect(toolGlobal, SIGNAL(timeout_playStop()), this, SLOT(play_stop()));

    //设置改变，设置信息写入配置文件
    connect(titleGroupBox, SIGNAL(settingDataChanged(QString,QString)),                       //设置皮肤，信息写入配置文件
            this, SLOT(writeSettingdateToIni(QString,QString)));
			
    connect(titleGroupBox, SIGNAL(ShowVideoPlayer()), this, SLOT(ShowVideoPlayer()));                   //切换为 视频播放 模式
    connect(titleGroupBox, SIGNAL(calldMusicTitleAndAuthor()),
            bottomGroupbox, SLOT(sendMusicTitleAndAuthor()));                                           //MiNiPlayer 请求发送歌曲信息
    connect(titleGroupBox, SIGNAL(callPlayPause()),    bottomGroupbox, SLOT(play_pause_clicked()));     //MiNiPlayer 播放/暂停
    connect(titleGroupBox, SIGNAL(callPlayPrevious()), bottomGroupbox, SLOT(play_previous_clicked()));  //MiNiPlayer 上一曲
    connect(titleGroupBox, SIGNAL(callPlayNext()),     bottomGroupbox, SLOT(play_next_clicked()));      //MiNiPlayer 下一曲
    connect(titleGroupBox, SIGNAL(callVolumnPlus()),   musicList,      SLOT(setVolumnPlus()));          //MiNiPlayer 增加音量
    connect(titleGroupBox, SIGNAL(callVolumnLess()),   musicList,      SLOT(setVolumnLess()));          //MiNiPlayer 减小音量

    //搜索在线音乐
    connect(titleGroupBox, SIGNAL(searchMusicClicked(QString)), this, SLOT(searchMusicClicked(QString)));

    //边缘缩放
    isLeftPressDown = false;
    this->direction = NONE;
    this->centralWidget()->setMouseTracking(true);
    titleGroupBox->setMouseTracking(true);
    toolGlobal->setMouseTracking(true);
//    lrcView->setMouseTracking(true);
    musicList->setMouseTracking(true);
    bottomGroupbox->setMouseTracking(true);
}

//打开临时音乐文件
void DT_Music::openTempMusicFile(QString file)
{
    musicList->openTempFile(file);
}

//打开临时视频文件
void DT_Music::openTempVideoFile(QString file)
{
    ShowVideoPlayer();
    videoPlayer->openVideoFile(file);
}

DT_Music::~DT_Music()
{
    delete ui;
}

//重写 鼠标按下事件
void DT_Music::mousePressEvent(QMouseEvent *event)
{
//    if (event ->button() == Qt::LeftButton) offset = event->globalPos() - pos();

    if (event->button() == Qt::LeftButton)
    {
        isLeftPressDown = true;
        if(direction != NONE)
        {
            this->mouseGrabber();
        }
        else
        {
            dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        }
    }
    else
    {
        QMainWindow::mousePressEvent(event);
    }
}

//重写 鼠标移动事件
void DT_Music::mouseMoveEvent(QMouseEvent *event)
{
//    if (event ->buttons() & Qt::LeftButton) QPoint temp; temp = event ->globalPos() - offset; move(temp);

    QPoint gloPoint = event->globalPos();           //鼠标在桌面的坐标
    QRect rect = this->rect();                      //窗口尺寸
    QPoint tl = mapToGlobal(rect.topLeft());        //窗口左上顶点在桌面的坐标
    QPoint rb = mapToGlobal(rect.bottomRight());    //窗口右下顶点在桌面的坐标

    if(!isLeftPressDown)
    {
        region(gloPoint);                           //判断鼠标位置，现实鼠标样式
    }
    else
    {
        if(direction != NONE)                       //如果左键按下，且在窗口边缘
        {
            QRect rMove(tl, rb);

            switch(direction)
            {
            case LEFT:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                break;
            case RIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                break;
            case UP:
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case DOWN:
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case LEFTTOP:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case RIGHTTOP:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setY(gloPoint.y());
                break;
            case LEFTBOTTOM:
                rMove.setX(gloPoint.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case RIGHTBOTTOM:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            default:

                break;
            }
            this->setGeometry(rMove);
        }
        else
        {
            move(event->globalPos() - dragPosition);
            event->accept();
        }
    }
    QMainWindow::mouseMoveEvent(event);
}

//鼠标释放事件
void DT_Music::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        isLeftPressDown = false;
        if(direction != NONE)
        {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

//重写 关闭事件
void DT_Music::closeEvent(QCloseEvent *event)
{/*
    if (isVisible())
    {
        hide();
//        trayIcon->showMessage(tr("MyPlayer音乐播放器"), tr("点我重新显示主界面"));
        event->ignore();
    }*/

//    this->hide();
}

//事件过滤器
bool DT_Music::eventFilter(QObject *target, QEvent *event)
{
    if (target==cob_addMusic || target==tbn_createList || target==tbn_search)
    {
        if (event ->type() == QEvent::MouseMove)
        {
            ;
        }
        else
        {
            return QWidget::eventFilter(target, event);
        }
    }
    else if (target == playModle)                           //播放模式设置区
    {
        if (event->type() == QEvent::Leave)
        {
            playModle->hide();
        }
        else
        {
            return QWidget::eventFilter(target, event);
        }
    }
    else if (target == lab_volumnFrame)                     //音量调节区
    {
        if (event->type() == QEvent::Leave)
        {
            lab_volumnFrame->hide();
        }
        else if (event->type() == QEvent::Wheel)
        {
            QWheelEvent *eventw = static_cast<QWheelEvent*>(event);
            if (eventw->delta()>0)
            {
                slider_volumn->setValue(slider_volumn->value()+5);
            }
            else if (eventw->delta()<0)
            {
                slider_volumn->setValue(slider_volumn->value()-5);
            }
        }
        else if (event->type() == QEvent::Show)
        {
            oldVolumnValue = slider_volumn->value();
        }
        else if (event->type() == QEvent::Hide)
        {
            if (slider_volumn->value() != oldVolumnValue)
            {
                update_volumn_value_of_database();
            }
        }
        else
        {
            return QWidget::eventFilter(target, event);
        }
    }
    else if (target == game)
    {
        if (event->type() == QEvent::Enter)
        {
            game->setFocus();
        }
    }
    return QWidget::eventFilter(target, event);
}

//初始化 界面 和 变量
void DT_Music::init()
{
    //创建 顶部部件
    titleGroupBox = new TitleGroupBox(this);
    titleGroupBox->setFixedHeight(81);

    //创建 工具栏部件
    toolGlobal = new ToolGlobal(this);              //本地音乐 工具栏
    toolGlobal->setFixedHeight(32);
//    toolInternet = new ToolInternet(this);          //网络歌曲 工具栏 并隐藏
//    toolInternet ->setHidden(true);                 //...................

    //初始化 播放列表
    QFont font;                                         //定义 字体
    font.setPointSize(10);                              //设置 字体大小
    lab_musicListTitle = new QLabel(" 播放列表", this);              //创建 播放列表头部
    musicList = new MusicList(programDir, this);                    //创建 播放列表
    lab_musicListTitle->setFont(font);                 //应用 字体

    //初始化 “添加音乐” 和 “搜索框”
    addMusic_and_searchMusic();

    //创建 歌词界面
    lrcView = new LrcView(this);
//    internetWidget = new InternetWidget(this);      //初始化 网络歌曲 界面 并隐藏
//    internetWidget ->setHidden(true);

    //创建 底部部件
    bottomGroupbox = new BottomGroupBox(musicList, programDir, this);
    bottomGroupbox->setFixedHeight(71);

    //初始化 音量 控件
    volumn_widget();

    //初始化 播放模式 控件 并隐藏
    playModle = new PlayModle(this);
    playModle ->hide();

    //初始化 桌面歌词 窗口 并隐藏
    desktopLrc = new DesktopLrc;
    desktopLrc ->setHidden(true);

    //初始化 工具栏 图标
    create_trayIcon();

    //读取数据库，初始化设置
    readDatabaseToSetup();

    ln_search->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    ui->line_right->setFixedWidth(1);
    ui->line_bottom->setFixedHeight(1);

    //布局窗口部件
    layout_musicListHead = new QHBoxLayout;
    layout_musicListHead->addStretch(86);
    layout_musicListHead->addWidget(cob_addMusic);
    layout_musicListHead->addWidget(tbn_createList);
    layout_musicListHead->setSpacing(26);
    layout_musicListHead->setContentsMargins(46, 0, 10, 0);

    layout_middleLeft = new QGridLayout;
    layout_middleLeft->addWidget(ui->label_musicListTitle, 0, 0, 24, 276);
    layout_middleLeft->addWidget(lab_musicListTitle, 0, 0, 24, 276);
    layout_middleLeft->addLayout(layout_musicListHead, 2, 86, 20, 192);
    layout_middleLeft->addWidget(musicList, 24, 0, 432, 276);
    layout_middleLeft->addWidget(ln_search, 455, 0, 22, 276);
    layout_middleLeft->addWidget(tbn_search, 459, 255, 15, 15);
    layout_middleLeft->addWidget(ui->line_right, 0, 276, 477, 1);
    layout_middleLeft->addWidget(ui->line_bottom, 454, 0, 1, 276);
    layout_middleLeft->setSpacing(0);
    layout_middleLeft->setMargin(0);

    layout_middle = new QHBoxLayout;
    layout_middle->addLayout(layout_middleLeft, 276);
    layout_middle->addWidget(lrcView, 731);
    layout_middle->setSpacing(0);
    layout_middle->setMargin(0);

    //顶级布局
    layout_top = new QVBoxLayout;
    layout_top->addWidget(titleGroupBox, 81);
    layout_top->addWidget(toolGlobal, 32);
    layout_top->addLayout(layout_middle, 476);
    layout_top->addWidget(bottomGroupbox, 71);
    layout_top->setSpacing(0);
    layout_top->setMargin(0);
    this->centralWidget()->setLayout(layout_top);
}

//创建 ”添加音乐“ 和 ”搜索框“
void DT_Music::addMusic_and_searchMusic()
{
    cob_addMusic = new QComboBox(this);                         //创建 添加音乐 按钮
    tbn_createList = new QToolButton(this);                     //创建 新建列表 按钮
    ln_search = new QLineEdit(this);                            //创建 搜索框
    tbn_search = new QToolButton(this);                         //创建 搜索按钮
    tbn_search->setFixedSize(15, 15);
    cob_addMusic->setFixedSize(80, 20);
    tbn_createList->setFixedSize(76, 20);

    ln_search ->setTextMargins(0, 0, 18, 0);
    ln_search ->setPlaceholderText(tr("查找......"));
    cob_addMusic ->addItem(tr("添加音乐"));                     //为 添加音乐按钮 添加项目
    cob_addMusic ->addItem(tr("添加目录"));                     //为 添加音乐按钮 添加项目
    tbn_createList ->setText(tr("  新建列表"));
    tbn_search ->setToolTip("搜索");
    ln_search->setCompleter(musicList->completer);             //将 自动补全 应用到搜索框

    cob_addMusic ->setObjectName(tr("cob_addMusic"));           //显示设置 添加文件 对象名
    tbn_createList ->setObjectName(tr("tbn_createList"));
    ln_search ->setObjectName(tr("ln_search"));                 //显示设置 搜索    对象名

    cob_addMusic ->setStyleSheet(
                "#cob_addMusic{"                                   //设置 添加音乐 样式表
                    "background: rgba(0, 0, 0, 100);"
                    "margin: 0;"
                    "padding: 0 0 0 8px;"
                    "border-radius: 3px;"
                    "color: rgb(200, 255, 255);"
                    "width: 80px;"
                "}"
                "#cob_addMusic QAbstractItemView::item{height: 20px;}"  //下拉选项高度
                "#cob_addMusic::drop-down{"                             //jian tou xiao fang kuai
                    "background: rgba(255, 255, 255, 0);"
                    "border-image: url(:/Images/cobIcon.png);"
                "}"

                //////////////////////////////////////////////////////////////////
                //                                                              //
                //    WindowsOs is no                                           //
                //    LinuxOs   is this                                         //
                //                                                              //
                //////////////////////////////////////////////////////////////////

//                "#cob_addMusic QAbstractItemView{"
//                      "background: rgba(0, 0, 0, 100);"
//                "}"
               );
    cob_addMusic->setView(new QListView());                             //如果没有这句，下拉选项高度设置不起作用

    tbn_createList ->setStyleSheet(
                "#tbn_createList{"
                    "background: rgba(0, 0, 0, 100);"
                    "border-image: url(:/Images/createMusicList.png);"
                    "color: rgb(200, 255, 255);"
                    "border-radius: 3px;"
                "}"
                );
    ln_search->setStyleSheet(
                "#ln_search{"                                          //设置 搜索框 样式表
                    "border-radius:10px;"
                    "padding:2px 4px;"
                    "background: rgba(0, 0, 0, 100);"
                    "color: rgb(200, 255, 255);"
                "}"
                );
    tbn_search ->setStyleSheet("QToolButton{border-image: url(:/Images/search.png)}");

    //信号槽关联
    connect(cob_addMusic, SIGNAL(activated(int)), this, SLOT(addMusicFile(int)));
    connect(tbn_createList, SIGNAL(clicked()), this, SLOT(create_MusicList()));
    connect(tbn_search, SIGNAL(clicked()), this, SLOT(tbn_search_clicked()));
}

//初始化 音量控件
void DT_Music::volumn_widget()
{
    //创建音量控件，并隐藏
    lab_volumnFrame = new QLabel(this);
    lab_volumnFrame->hide();
    lab_volumnFrame->installEventFilter(this);
    slider_volumn = new QSlider(Qt::Vertical, lab_volumnFrame);     //初始化 音量控件 并隐藏
    lab_volumnValue = new QLabel(lab_volumnFrame);                  //初始化 音量值 标签 并隐藏
    lab_volumnFrame->installEventFilter(this);
    lab_volumnFrame->setToolTip("滚动鼠标滚轮调节音量");

    //设置音量部件的最大值为100
    slider_volumn ->setMaximum(100);

//    //初始化音量标签值
//    lab_volumnValue ->setText(
//                tr("%1%").arg(slider_volumn ->value()));

    //设置 音量控件 位置 大小
    lab_volumnFrame->resize(34, 130);
    lab_volumnValue->setGeometry(1, 2, 33, 10);
    slider_volumn->setGeometry(9, 17, 8, 80);

    //设置 样式
    slider_volumn ->setStyleSheet(
                "QSlider::groove:vertical{"                         //设置 进度条 样式
                    "border: 1px solid #000000;"
                    "width: 2px;"
                    "margin: 0px 0;"
                    "top: 4px;"
                    "bottom: 4px;"
                "}"
                "QSlider::handle:vertical{"
                    "border: 1px solid #5c5c5c;"
                    "border-image: url(:/Images/sliderHandle.png);"
                    "height: 8px;"
                    "margin: -4px -4px -4px -4px;"
                "}"
                "QSlider::sub-page:vertical{"
                    "background: rgb(83, 83, 83);"
                "}"
                "QSlider::add-page:vertical{"
                    "background: rgb(115, 153, 1);"
                "}"
                "QSlider::handle::hover:vertical{"
                    "border-image: url(:/Images/sliderHandle_hover.png);"
                "}"
                );

    //关联音量控件和音量值标签的值同步
    connect(slider_volumn, SIGNAL(valueChanged(int)), this, SLOT(volumValue_changed()));
    connect(slider_volumn, SIGNAL(valueChanged(int)), musicList->player, SLOT(setVolume(int)));         //音量控件值改变，更新多媒体播放音量
    connect(slider_volumn, SIGNAL(sliderReleased()), lab_volumnFrame, SLOT(hide()));                    //释放音量控件，音量控件隐藏
//    connect(slider_volumn, SIGNAL(sliderReleased()), this, SLOT(update_volumn_value_of_database()));  //释放音量控件 更新数据库

//    slider_volumn->setValue(volumn_defaultValue);
}

//初始化 工具栏图标
void DT_Music::create_trayIcon()
{
    //初始化 工具栏 图标
    trayIcon = new QSystemTrayIcon(QIcon(":/Images/logo.png"), this);
    trayIcon->setToolTip(tr("兔兔音乐播放器"));

    // 创建菜单
    QMenu *menu = new QMenu;
    QList<QAction *> actions;
//////////////////////////////////////////////////////        actions << playAction << stopAction << skipBackwardAction << skipForwardAction;
        menu->addActions(actions);
        menu->addSeparator();
//////////////////////////////////////////////////////        menu->addAction(PLAction);
//////////////////////////////////////////////////////        menu->addAction(LRCAction);

    menu->addSeparator();
    menu ->addAction(QIcon(":/Images/logo.png"), tr("显示主界面"), this, SLOT(showWindow()));
    menu ->addAction(QIcon(":/Images/play_play_hover.png"), tr("播放/暂停"), bottomGroupbox, SLOT(play_pause_clicked()));
    menu ->addAction(QIcon(":/Images/play_previous_hover.png"), tr("上一曲"), bottomGroupbox, SLOT(play_previous_clicked()));
    menu ->addAction(QIcon(":/Images/play_next_hover.png"), tr("下一曲"), bottomGroupbox, SLOT(play_next_clicked()));
    menu ->addAction(tr("桌面歌词"), this, SLOT(lrc_clicked()));
    menu->addAction(tr("退出"), qApp, SLOT(quit()));

    trayIcon->setContextMenu(menu);
    // 托盘图标被激活后进行处理
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIcon_clicked(QSystemTrayIcon::ActivationReason)));
    // 显示托盘图标
    trayIcon->show();
}

//读取数据库，加载用户设置，如果数据库不存在，创建数据库，设置默认值
void DT_Music::readDatabaseToSetup()
{
    // set theme
    theme = ThemeDefultValue;
    if (!xml.isElementExist(xml.ThemeElement))
    {
        xml.addElementWithText(xml.SettingElement, xml.ThemeElement, tr("%1").arg(theme));
    }
    else
    {
        theme = xml.getElementText(xml.ThemeElement).toInt();
    }
    setTheme(theme);

    // set volumn
    int volumn = VolumnDefaultValue;
    if (!xml.isElementExist(xml.VolumnElement))
    {
        xml.addElementWithText(xml.SettingElement, xml.VolumnElement, tr("%1").arg(volumn));
    }
    else
    {
        volumn = xml.getElementText(xml.VolumnElement).toInt();
    }
    slider_volumn->setValue(volumn);

    // set playMode
    int playMode = PlayModeDefaultValue;
    if (!xml.isElementExist(xml.PlayModeElement))
    {
        xml.addElementWithText(xml.SettingElement, xml.PlayModeElement, tr("%1").arg(playMode));
    }
    else
    {
        playMode = xml.getElementText(xml.PlayModeElement).toInt();
    }
    switch(playMode)
    {
    case PlayModle::PlayRandom:
        bottomGroupbox->playModle_choosed(PlayModle::PlayRandom);
        break;
    case PlayModle::PlayOnce:
        bottomGroupbox->playModle_choosed(PlayModle::PlayOnce);
        break;
    case PlayModle::PlaySingle:
        bottomGroupbox->playModle_choosed(PlayModle::PlaySingle);
        break;
    case PlayModle::PlaySequence:
        bottomGroupbox->playModle_choosed(PlayModle::PlaySequence);
        break;
    case PlayModle::PlayLoop:
        bottomGroupbox->playModle_choosed(PlayModle::PlayLoop);
        break;
    case PlayModle::PlayCustom:
        bottomGroupbox->playModle_choosed(PlayModle::PlayCustom);
        break;
    default:
        QMessageBox::warning(0, "错误！", "初始化读取音量值错误！", QMessageBox::Ok);
        break;
    }
}

//添加音乐 槽函数
void DT_Music::addMusicFile(int selected)
{
    QStringList file_types;
    file_types << "*.mp3" << "*.wma" << "*.wav" << "*.asf" << "*.aac" << "*.mp3pro" << "*.vqf" << "*.flac" << "*.ape" << "*.mid" << "*.ogg" << "*.aac"
               << "*.MP3" << "*.WMA" << "*.WAV" << "*.ASF" << "*.AAC" << "*.MP3PRO" << "*.VQF" << "*.FLAC" << "*.APE" << "*.MID" << "*.OGG" << "*.AAC";
//    QStringList musicNameListAdd;   //音乐文件路径

    //获得要添加的音乐文件路径
    QList<QMap<QString, QString> > musics;
    QList<QMap<QString, QMap<QString, QString> > > elementsNameAttributeAndValue;
    switch(selected)
    {
    case 0:             //添加音乐文件
    {
        QString types;
        for (int i=0; i<file_types.count(); ++i)        // 组合字符串
        {
            types += file_types.at(i) + " ";
        }
        QStringList urls = QFileDialog::getOpenFileNames(this, tr("添加音乐"), "D:/", tr("音乐文件(%1)").arg(types));
        for (int i=0; i<urls.length(); ++i)
        {
            QMap<QString, QString> urlAndName;
            urlAndName.insert(urls.at(i), QFileInfo(urls.at(i)).fileName());
            musics.append(urlAndName);

            QMap<QString, QString> attributeKeyAndValue;
            attributeKeyAndValue.insert("id", tr("默认列表-%1").arg(i));

            QMap<QString, QMap<QString, QString> > nameAttributeAndValue;
            nameAttributeAndValue.insert(xml.MusicElement, attributeKeyAndValue);

            elementsNameAttributeAndValue.append(nameAttributeAndValue);
        }
        if (musics.isEmpty())
        {
            return;
        }
        break;
    }
    case 1:             //添加音乐目录
    {
        QString dirName;
        QStringList musicNames;

        //
        dirName = QFileDialog::getExistingDirectory(this, tr("选择文件夹"), "D:/");
        if (dirName.isEmpty())
        {
            return;
        }
        QDir dir(dirName);
        dir.setNameFilters(file_types);
        musicNames = dir.entryList();
        for (int i=0; i<musicNames.length(); i++)
        {
            QMap<QString, QString> urlAndName;
            urlAndName.insert(dirName + "/" + musicNames.at(i), musicNames.at(i));
            musics.append(urlAndName);

            QMap<QString, QString> attributeKeyAndValue;
            attributeKeyAndValue.insert("id", tr("默认列表-%1").arg(i));

            QMap<QString, QMap<QString, QString> > nameAttributeAndValue;
            nameAttributeAndValue.insert(xml.MusicElement, attributeKeyAndValue);

            elementsNameAttributeAndValue.append(nameAttributeAndValue);
        }
        break;
    }
    default:
        Q_ASSERT_X(false, "select combbox", "The index of combbox is not found!");
    }

    //添加到界面和播放列表中
//    musicList->addMusicToList(0, musicNameListAdd);
    musicList->addMusicToList("默认列表", musics);

    //添加到数据库中（子线程）
//    subThread.insertDatabase(SubThread::InsertDataBase, musicListDatabaseName, "默认列表", "musicName", musicNameListAdd);
//    subThread.start();

    // 更新配置文件
    QStringList childrenElementNames;
    childrenElementNames << "url" << "name";
    xml.addRecursiveElement(xml.MusicListElement, xml.MusicListElementKey, "默认列表", elementsNameAttributeAndValue, childrenElementNames, musics);
}

//单击 检索 按钮
void DT_Music::tbn_search_clicked()
{
    QString search_content = ln_search->text();
    if (search_content.isEmpty())
    {
        return;
    }
    musicList->setCurrentRow(search_content);
}

//歌曲时长信息出现时，调用 解析歌词 函数
void DT_Music::durationChanged()
{
    //清空上一首歌词缓存
    lrcView ->currentLrc->clear();          //清空当前行歌词显示
    lrcView ->textPrevious ->clear();       //清空
    lrcView ->textNext ->clear();           //清空 后续歌词显示
    lrcView ->previousLines.clear();        //..............

    //定位当前播放歌曲
//    int rootDir = musicList->get_current_rootDir();                         //检测 当前播放列表
//    int currentIndex = musicList->playlistVector.at(rootDir)->currentIndex();  //当前行 索引值
//    lrcStringList.clear();                                        //

    //调用 解析歌词函数
    resolveLrc(musicList->player->currentMedia().canonicalUrl().toLocalFile());

    //标记当前播放歌曲的二维索引
//    musicList ->currentPlayingIndex[0] = rootDir;
//    musicList ->currentPlayingIndex[1] = currentIndex;
}
/////////////////////////////////////////////////////////////////
//查找歌曲
void DT_Music::searchMusic()
{
    QString searchStr = ln_search ->text();
    if (searchStr.isEmpty())
    {
        return;
    }
    for (int i=0; i<musicList->playlistVector.length(); i++)
    {
        for (int j=0; j<musicList->playlistVector.at(j)->mediaCount(); j++)
        {
            if (musicList->playlistVector.at(i)->media(j).canonicalUrl().fileName() == searchStr)
            {
//                emit searchedMusicIndex(i, j);
                musicList->setCurrentItem(musicList->topLevelItem(i)->child(j));
                return;
            }
        }
    }
}

//更新歌词
void DT_Music::updateLrc(qint64 time)
{
    qint64 totalTimeValue = musicList->player->duration();

    if (! lrcMap.isEmpty())
    {
        qint64 previous = 0;
        qint64 later = 0;
        foreach (qint64 value, lrcMap.keys())
        {
            if (time >= value)
            {
                previous = value;
            }
            else
            {
                later = value;
                break;
            }
        }
        if (later == 0)
        {
            later = totalTimeValue;
        }
        QString currentLrc = lrcMap.value(previous);    //当前行

        //找当前时间后的5行歌词
        QString currentLrcAdd;                 //往歌词界面新增的歌词
        int i = -1;
        foreach (qint64 value, lrcMap.keys())
        {
            if (value == later)
            {
                i = 0;
            }
            if (i != -1)
            {
                i ++;
            }

            //取得当前歌词后面 9 行歌词
            if (i > 0)
            {
                currentLrcAdd += lrcMap.value(value) + "\n";
            }
            if (i > 8)
            {
                break;
            }
        }
        if (currentLrc.length() < 2)
        {
            currentLrc = tr("...");
        }

        //更新后续歌词
        if (lrcView ->textNext ->text() != currentLrcAdd)
        {
            lrcView ->textNext ->setText(currentLrcAdd);
        }
//        lrcStringList.append(lrcView ->text());
/*
        //
        foreach (qint64 value, lrcMap.keys())
        {
            if (time > previous && time <later)
            {
                if (previousLrcString != currentLrcAdd)
                {
//                    lrcStringList.append(currentLrc);
//                    previousLrcString = currentLrc;
                    lrcStringList.append(currentLrcAdd);
                    previousLrcString = currentLrcAdd;
                    if (lrcStringList.length() > 18)
                    {
                        lrcStringList.removeAt(0);
                    }
                    QString lrcString;                              //屏显歌词
//                    lrcString += initLrc;
                    for (int i=0; i<lrcStringList.length(); i++)
                    {
                        lrcString += lrcStringList[i] + "\n";
                    }
                    lrcView ->setText(lrcString);
                    break;
                }
            }

        if (currentLrc != lrcView ->text())

        {
//            lrcView ->setText(lrcString);
            qint64 intervalTime = later - previous;
            lrcView ->startLrcMask(intervalTime);
        }
*/
        //重复重复重复重复重复重复重复重复重复重复重复重复重复重复重复重复重复重复重复重复重复重复重复
        if (currentLrc != desktopLrc ->text())
        {
            if (lrcView ->previousLines.length() >7)
            {
                lrcView ->previousLines.removeAt(0);
            }
            lrcView ->previousLines.append(lrcView ->currentLrc ->text());
            lrcView ->previousLrc.clear();
            for (int i=0; i<lrcView ->previousLines.length(); i++)
            {
                lrcView ->previousLrc += "\n" + lrcView ->previousLines[i];
            }
            lrcView ->textPrevious -> setText(lrcView ->previousLrc);

            lrcView ->currentLrc ->setText(currentLrc);
            desktopLrc ->setText(currentLrc);

            qint64 intervalTime = later - previous;
            lrcView ->currentLrc ->startLrcMask(intervalTime);
            desktopLrc ->startLrcMask(intervalTime);
        }
//        lrcView ->currentLrc = currentLrc;
    }
    else
    {
        lrcView ->currentLrc ->setText("找不到歌词文件");
        desktopLrc ->setText("找不到歌词文件");
    }
}

//
void DT_Music::setLrcShown(bool view)
{
    if (view)
    {
        desktopLrc ->show();
    }
    else
    {
        desktopLrc ->hide();
    }
}

//未知信号
void DT_Music::seekableChanged(bool a)
{
    qDebug() << "seekableChanged(bool)" << a;
}

//未知信号
void DT_Music::metaDataAvailableChanged(bool a)
{
    qDebug() << "metaDataAvailableChanged(bool)" << a;
}

//未知信号
void DT_Music::metaDataChanged()
{
    qDebug() << "metaDataChanged()";
}

//未知信号
void DT_Music::metaDataChanged(QString a, QVariant b)
{
    qDebug() << "metaDataChanged(QString, QVariant)" << a << "----" << b;
}

//未知信号
void DT_Music::mediaStatusChanged(QMediaPlayer::MediaStatus a)
{
    qDebug() << ":mediaStatusChanged(QMediaPlayer::MediaStatus)" << a;
}

//未知信号
void DT_Music::mediaChanged(QMediaContent)
{
    qDebug() << "mediaChanged(QMediaContent)";
}

//未知信号
void DT_Music::objectNameChanged(QString a)
{
    qDebug() << "objectNameChanged(QString)" << a;
}

//未知信号
void DT_Music::playbackRateChanged(qreal a)
{
    qDebug() << "playbackRateChanged(qreal)" << a;
}

//切换为视频播放器
void DT_Music::ShowVideoPlayer()
{
    musicList->player->pause();

    videoPlayer = new VideoPlayer(programDir);
    videoPlayer->show();
    close();
    connect(videoPlayer, SIGNAL(switchMusicPlayer()), this, SLOT(switchMusicPlayer()));
}

//切换回音乐播放器
void DT_Music::switchMusicPlayer()
{
    setVisible(true);
    videoPlayer->deleteLater();
    timerBear = new QTimer(this);
    timerBear->start(300);
    connect(timerBear, SIGNAL(timeout()), this, SLOT(timeoutBear()));
}

//声音改变
void DT_Music::volumeChanged(int a)
{
    qDebug() << "volumeChanged(int)" << a;
}

//设置 自定义主题
void DT_Music::setTheme(int theme)
{
    if (theme == -1)
    {
        theme = this->theme;
    }
    setStyleSheet(tr("#centralWidget{border-image: url(:/Images/bg%1.jpg);}").arg(theme));
}

//音量值改变
void DT_Music::volumValue_changed()
{
    lab_volumnValue ->setText(tr("%1%").arg(slider_volumn ->value()));
}

//更新数据库中音量值
void DT_Music::update_volumn_value_of_database()
{
//    QMap<QString, QString> new_volumn_value;
//    new_volumn_value.insert("volumn", tr("%1").arg(slider_volumn->value()));
//    DatabaseOperation db_update_volumn(setUpDatabaseName);
//    db_update_volumn.updateDatabase("setUp", new_volumn_value);
    QString volumn = tr("%1").arg(slider_volumn->value());
    xml.alterElementText(xml.SettingElement, xml.VolumnElement, volumn);
}

//倒计时处理，停止播放
void DT_Music::play_stop()
{
    musicList->player->stop();
}

//歌词 处理
void DT_Music::lrc_clicked()
{
    if (desktopLrc ->isHidden())
    {
        desktopLrc ->setVisible(true);
    }
    else
    {
        desktopLrc ->hide();
    }
}

//音量 处理
void DT_Music::showVolumn(QPoint point)
{
    if (lab_volumnFrame->isHidden())
    {
        lab_volumnFrame->move(point.x(), bottomGroupbox->y()-84);
        lab_volumnFrame->setVisible(true);
    }
    else
    {
        lab_volumnFrame->hide();
    }
}

//显示/隐藏 ”播放模式“ 面板
void DT_Music::showPlayModle(QPoint point)
{
    if (playModle ->isHidden())
    {
        playModle->move(point.x()-8, bottomGroupbox->y()-playModle->height()+45);
        playModle->setVisible(true);
    }
    else
    {
        playModle ->hide();
    }
}

//显示主界面
void DT_Music::showWindow()
{
    show();
}

//单击 托盘图标 处理
void DT_Music::trayIcon_clicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        if (isHidden())
        {
            this->show();
            this->activateWindow();
        }
        else
        {
            this->hide();
        }
        break;
    default:
        break;
    }
}

//魔音状态改变 处理
void DT_Music::maigcStateChanged(bool magicState)
{
    if (magicState)
    {
        musicList->player->setPlaybackRate(1.7);
    }
    else
    {
        musicList->player->setPlaybackRate(1);
    }
}

//歌词游戏切换
void DT_Music::gameStateChange(bool gameState)
{
    if (gameState)
    {
        game = new Game(this);
        game->installEventFilter(this);
        game->move(lrcView->pos().x()+250, lrcView->pos().y()+50);
        lrcView->setLrcTextView(false);
        game->show();
        game->setFocus();
    }
    else
    {
        game->deleteLater();
        lrcView->setLrcTextView(true);
    }
}

//创建播放列表
void DT_Music::create_MusicList()
{
    createMusicList = new Create_MusicList(this);
    createMusicList->show();

    connect(createMusicList, SIGNAL(pbn_ok_click(QString)), this, SLOT(create_musicList_ok(QString)));
}

//确认 创建播放列表
void DT_Music::create_musicList_ok(QString name)
{
    QStringList toplevelNames = musicList->getToplevelNames();
    for (int i=0; i<toplevelNames.length(); ++i)
    {
        if (name == toplevelNames.at(i))
        {
            createMusicList->setNameError();
            return;
        }
    }

    musicList ->create_musicList(name);         //调用 musicList 类 创建列表
    createMusicList->deleteLater();
}

/////////////////////////////////////////////更新 要创建列表的名字
void DT_Music::update_createMusiclist_name(QString name)
{
    createMusiclist_name = name;
}

//
void DT_Music::timeoutBear()
{
    titleGroupBox->timeoutBear();
    timerBear->deleteLater();
}

// 搜索在线音乐
void DT_Music::searchMusicClicked(QString musicName)
{
    if (! netWorkWidget)
    {
        netWorkWidget = new NetWorkWidget(this);
    }
    netWorkWidget->show();
    netWorkWidget->searchMusic(musicName);
    connect(netWorkWidget, SIGNAL(playInternetMusic(QString, QString)), musicList, SLOT(playInternetMusic(QString, QString)));
}

void DT_Music::writeSettingdateToIni(QString, QString value)
{
    setTheme(value.toInt());
    theme = value.toInt();
    xml.alterElementText(xml.SettingElement, xml.ThemeElement, tr("%1").arg(value));
}

//判断鼠标区域
void DT_Music::region(const QPoint &cursorGlobalPoint)
{
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x()+PADDING>=x && tl.x()<=x && tl.y()+PADDING>=y && tl.y()<=y)        // 左上角
    {
        direction = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x>=rb.x()-PADDING && x<=rb.x() && y>=rb.y()-PADDING && y<=rb.y())   // 右下角
    {
        direction = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if(x<=tl.x()+PADDING && x>=tl.x() && y>=rb.y()-PADDING && y<=rb.y())   //左下角
    {
        direction = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x<=rb.x() && x>=rb.x()-PADDING && y>=tl.y() && y<=tl.y()+PADDING)   // 右上角
    {
        direction = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if(x<=tl.x()+PADDING && x>=tl.x())                                     // 左边
    {
        direction = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if( x<=rb.x() && x>=rb.x()-PADDING)                                    // 右边
    {
        direction = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(y>=tl.y() && y<=tl.y()+PADDING)                                     // 上边
    {
        direction = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if(y<=rb.y() && y>=rb.y()-PADDING)                                     // 下边
    {
        direction = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else                                                                        // 默认
    {
        direction = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

//解析歌词
void DT_Music::resolveLrc(const QString &sourceFileName)
{
    lrcMap.clear();

    if(sourceFileName.isEmpty())
    {
        return;
    }
    QString fileName = sourceFileName;
    QString lrcFilePath = fileName.remove(fileName.right(3)) + "lrc";

    //如果歌词文件不存在，下载歌词文件
    if(!QFileInfo(lrcFilePath).exists())
    {
        lrcView ->currentLrc ->setText(tr("正在下载歌词......"));
        network->downLoadLrc(lrcFilePath.remove(lrcFilePath.right(4)));
        return;
    }

    QFile file(lrcFilePath);
    if (! file.open(QIODevice::ReadOnly))
    {
        lrcView ->currentLrc ->setText(tr("找不到歌词文件"));
        desktopLrc ->setText(tr("找不到歌词文件"));
    }
    QString allText = QString(file.readAll());
    file.close();
    QStringList lines = allText.split("\n");
    QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
    foreach(QString oneLine, lines)
    {
        QString temp = oneLine;
        temp.replace(rx, "");
        int pos = rx.indexIn(oneLine, 0);
        while (pos != -1)
        {
            QString cap = rx.cap(0);
            QRegExp regexp;
            regexp.setPattern("\\d{2}(?=:)");
            regexp.indexIn(cap);
            int minute = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\.)");
            regexp.indexIn(cap);
            int second = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\])");
            regexp.indexIn(cap);
            int millisecond = regexp.cap(0).toInt();
            qint64 totalTime = minute * 60000 + second * 1000 + millisecond * 10;

            lrcMap.insert(totalTime, temp);
            pos += rx.matchedLength();
            pos = rx.indexIn(oneLine, pos);
        }

        //解析完歌词，先显示半屏(5行)歌词
//        lrcStringList.clear();                          //清空前一首歌的歌词
        QString initLrc;
        int i = 0;
        foreach (qint64 time, lrcMap.keys())
        {
            initLrc += lrcMap.value(time) + "\n";
//            lrcStringList.append(lrcMap.value(time));
            i ++;
            if (i > 5)
            {
                break;
            }
        }
//        initLrc = tempLrc;
        lrcView ->textNext ->setText(initLrc);                    //显示 5 行后续歌词
    }
    if (lrcMap.isEmpty())
    {
//        lrcView ->setText(tr("歌词文件内容错误！"));
//        desktopLrc ->setText(tr("歌词文件内容错误！"));
        return;
    }
}

//打开数据库
bool DT_Music::openDatebase(QString datebaseName, QString sqlDriver, QString hostName, QString userName, QString password)
{
    db = QSqlDatabase::addDatabase(sqlDriver);
    db.setHostName(hostName);
    db.setDatabaseName(datebaseName);
    db.setUserName(userName);
    db.setPassword(password);
    if (db.open())
    {
        return true;
    }
    else
    {
        return false;
    }
}
