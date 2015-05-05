#include "videoplayer.h"

#include "videolist.h"
#include <QMessageBox>
#include <QDebug>

#include <QDesktopWidget>
#include <QToolButton>
#include <QTimer>
#include <QMouseEvent>
#include <QFileDialog>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

VideoPlayer::VideoPlayer(QString programPath, QWidget *parent) :
    QWidget(parent)
  ,programDir(programPath)
  ,WIDTH(600)
  ,HEIGHT(380)
  ,videoNameWidth(300)
  ,videoListWidth(200)
  ,hideButtonsTime(2000)
  ,forwardStep(5)
  ,retreatStep(5)
  ,volumnChangeStep(5)
  ,hideCursor(false)
  ,keepAwakeTimes(0)
{
    //设置窗口无边框
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    this->installEventFilter(this);

    //初始化窗口位置、大小
    QDesktopWidget desktop;
    setGeometry((desktop.width()-WIDTH)/2, (desktop.height()-HEIGHT)/2, WIDTH, HEIGHT);
    setObjectName("videoPlayer");

    //背景label
    lab_background = new QLabel(this);
    lab_background->setGeometry(0, 0, WIDTH, HEIGHT);
    lab_background->setObjectName("lab_background");

//    videoList = new VideoList(0, 25, videoListWidth, this->height()-50, false, this);
//    videoList->show();

    //设置鼠标跟踪，响应鼠标移动事件
    setMouseTracking(true);
    lab_background->setMouseTracking(true);

    //
    videoWidget = new VideoWidget;
    videoWidget->hide();
    videoWidget->installEventFilter(this);
    videoWidget->setMouseTracking(true);

    //
    helpKeepAwake = new HelpKeepAwake;

    //布局
    layout_top = new QGridLayout;
    layout_top->addWidget(lab_background, 0, 0, 1, 1);
    layout_top->addWidget(videoWidget, 0, 0, 1, 1);
    layout_top->setSpacing(0);
    layout_top->setMargin(0);
    this->setLayout(layout_top);

    //
    videoContral = new VideoContral(this);
    videoContral->setGeometry(this->x(), this->y(), this->width(), this->height());

    tbn_openVideoFile = new QToolButton(this);
    tbn_openVideoFile->setText("放入视频");
    tbn_openVideoFile->setGeometry((this->width()-126-32)/2+35, (this->height()-41)/2+95, 80, 30);
    tbn_openVideoFile->setObjectName("tbn_openVideoFile");

    //初始化多媒体播放器
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(player);
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    player->setPlaylist(playlist);
    player->setVideoOutput(videoWidget);
    player->setVolume(60);

    connect(tbn_openVideoFile,SIGNAL(clicked()), this, SLOT(openVideoFile()));
    connect(player, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
/*    connect(this, SIGNAL(moveWindow(QPoint)), videoList, SLOT(toMoveWindow(QPoint)));*/

    //设置样式表
    setStyleSheet(
                "#videoPlayer{"
//                    "border-radius: 3px;"
                "}"
                "#lab_background{"
//                    "border-radius: 3px;"
                    "border-image: url(:/Images/videoPlayerBg.png);"
                "}"
                "#tbn_openVideoFile{"
                    "background: rgba(0, 0, 0, 30);"
                    "color: rgba(200, 255, 255, 255);"
                "}"
                );

    //创建定时器（1.5秒无鼠标动作按钮自动隐藏）
    timerHideWidgets = new QTimer(this);
    timerHideWidgets->start(hideButtonsTime);
    connect(timerHideWidgets, SIGNAL(timeout()), this, SLOT(hideWidgets_timeout()));


    //操作控制
    connect(videoWidget, SIGNAL(clicked()), this, SLOT(videoPlayPause()));
    connect(videoWidget, SIGNAL(switchFullScreen()), this, SLOT(switchFullScreen()));
//    connect(videoWidget, SIGNAL(goForward()), this, SLOT(videoGoForward()));
//    connect(videoWidget, SIGNAL(retreat()), this, SLOT(videoRetreat()));
    connect(videoWidget, SIGNAL(playJump(int)), this, SLOT(videoPlayJump(int)));
    connect(videoWidget, SIGNAL(volumnAdd()), this, SLOT(volumnAdd()));
    connect(videoWidget, SIGNAL(volumnReduce()), this, SLOT(volumnReduce()));

    connect(videoContral, SIGNAL(isHideWidget(bool)), this, SLOT(isHideWidget(bool)));
    connect(videoContral, SIGNAL(playerStop()), player, SLOT(stop()));
    connect(videoContral, SIGNAL(playPause()), this, SLOT(videoPlayPause()));
    connect(videoContral, SIGNAL(playNext()), playlist, SLOT(next()));
    connect(videoContral, SIGNAL(currentMediaEnd()), this, SLOT(currentMediaEnd()));
//    connect(videoContral, SIGNAL(goForward()), this, SLOT(videoGoForward()));
//    connect(videoContral, SIGNAL(retreat()), this, SLOT(videoRetreat()));
    connect(videoContral, SIGNAL(playJump(int)), this, SLOT(videoPlayJump(int)));
    connect(videoContral, SIGNAL(switchFullScreen()), this, SLOT(switchFullScreen()));

    connect(player, SIGNAL(durationChanged(qint64)), videoContral, SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(setVideoTitle()));
    connect(player, SIGNAL(positionChanged(qint64)), videoContral, SLOT(positionChanged(qint64)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(playStateChanged(QMediaPlayer::State)));


//    videoWidget->setFocus();
    timerKeepAwake = new QTimer(this);
    connect(timerKeepAwake, SIGNAL(timeout()), this, SLOT(timeoutKeepAwake()));
}

//鼠标按下事件
void VideoPlayer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        offset = event->globalPos() - pos();
    }
}

//鼠标移动事件
void VideoPlayer::mouseMoveEvent(QMouseEvent *event)
{
    //晃动鼠标 功能按钮出现
    buttonsVisible(true);
    timerHideWidgets->start(1000);

    //拖动界面
    if (event->buttons() & Qt::LeftButton)
    {
        if (!this->isFullScreen())
        {
            this->move(event->globalPos() - offset);
        }
/*        emit moveWindow(temp);*/
    }
}

//事件过滤器
bool VideoPlayer::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == videoWidget)
    {
        if (event->type() == QEvent::MouseMove)
        {
            videoContral->show();
        }
    }
    else if (obj == this)
    {
        if (event->type() == QEvent::Resize)
        {
            videoContral->resize(this->size());
        }
        else if (event->type() == QEvent::Move)
        {
            videoContral->move(this->pos());
        }
    }
    return QWidget::eventFilter(obj, event);
}

//显示/隐藏 按钮
void VideoPlayer::buttonsVisible(bool visible)
{
    if(visible)
    {
        videoContral->setVisible(true);
        this->setCursor(Qt::ArrowCursor);
    }
    else
    {
        videoContral->setVisible(false);
        if (hideCursor)
        {
            this->setCursor(Qt::BlankCursor);
        }
    }
}

//打开视频文件
void VideoPlayer::openVideoFile(QString file)       //默认参数 file=""
{
    QStringList openVideoFileNames;
    QString absolutePath = QFileInfo(file).absolutePath();
    if (file.isEmpty())
    {
        openVideoFileNames.append(QFileDialog::getOpenFileName(this));
    }
    else
    {
        QDir videoDir(absolutePath);
        QStringList tempFiles = videoDir.entryList(QDir::Files);    //获得文件名
        for (int indext=0; indext<tempFiles.length(); indext++)
        {
            openVideoFileNames.append(QFileInfo(tempFiles.at(indext)).absoluteFilePath());  //将文件名转换成绝对路径
        }

    }
    if (openVideoFileNames.isEmpty())
    {
        return;
    }

    //添加到 playlist 中
    for(int indext=0; indext<openVideoFileNames.length(); indext++)
    {
        playlist->addMedia(QUrl::fromLocalFile(tr("%1").arg(openVideoFileNames.at(indext))));

    }
    for(int indext=0; indext<openVideoFileNames.length(); indext++)
    {
        if(openVideoFileNames.at(indext) == file)
        {
            playlist->setCurrentIndex(indext);
            break;
        }
    }
    player->play();
    QStringList fileBaseNames;
    for(int indext=0; indext<playlist->mediaCount(); indext++)
    {
        fileBaseNames.append(playlist->media(indext).canonicalUrl().fileName());
    }
    videoContral->addVideoList(fileBaseNames);
    lab_background->setStyleSheet("border-image: url(:/Images/black.png);");
    tbn_openVideoFile->hide();
}

//处理定时器，隐藏按钮
void VideoPlayer::hideWidgets_timeout()
{
    buttonsVisible(false);
    timerHideWidgets->stop();
}

//
void VideoPlayer::isHideWidget(bool enter)
{
    if(enter)
    {
        timerHideWidgets->stop();
    }
    else
    {
        timerHideWidgets->start(hideButtonsTime);
    }
}

void VideoPlayer::metaDataChanged()
{
    if (player->state() == QMediaPlayer::StoppedState)
    {
        return;
    }
    if (this->isFullScreen())
    {
        return;
    }
    QSize size = videoWidget->sizeHint();
    if (size.width()>0 && size.height()>0)
    {
        if (size.width()<WIDTH || size.height() <HEIGHT)
        {
            size = QSize(size.width()*2, size.height()*2);
        }
        int videoWidth = size.width();
        int videoHeight = size.height();
//        emit gotVideoSize(size);            ///////////////////////////////////////////何用？
        QDesktopWidget desktop;
        if (videoHeight > desktop.availableGeometry().height())
        {
            qreal bilv = qreal(videoWidth) / qreal(videoHeight);qDebug() << bilv;
            videoHeight = desktop.availableGeometry().height();
            videoWidth = videoHeight * bilv;
        }
        this->setGeometry((desktop.width()-videoWidth)/2, (desktop.availableGeometry().height()-videoHeight)/2, videoWidth, videoHeight);
    }
}

//
void VideoPlayer::setVideoTitle()
{
    videoContral->setVideoTitle(playlist->currentMedia().canonicalUrl().fileName());
}

//拖放进度
void VideoPlayer::valueChanged(int currentPosition)
{
    player->setPosition(currentPosition);
}

//切换全屏
void VideoPlayer::switchFullScreen()
{
    if (this->isFullScreen())
    {
        this->showNormal();
 //       this->setFullScreen(false);
    }
    else
    {
        this->showFullScreen();
//        this->setFullScreen(true);
    }
}

//播放/暂停
void VideoPlayer::videoPlayPause()
{
    if (player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
    }
    else
    {
        player->play();
    }
}

void VideoPlayer::currentMediaEnd()
{
//    playlist->setCurrentIndex(playlist->nextIndex());
}

// 快进/快退（默认5秒）
void VideoPlayer::videoPlayJump(int msecond)
{
    player->setPosition(player->position() + msecond);
}
/*
//快进（默认5秒）
void VideoPlayer::videoGoForward(int)
{
    player->setPosition(player->position() + forwardStep*1000);
}

//快退（默认5秒）
void VideoPlayer::videoRetreat()
{
    player->setPosition(player->position() - retreatStep*1000);
}
*/
//增加音量
void VideoPlayer::volumnAdd()
{
    player->setVolume(player->volume() + volumnChangeStep);
}

//减小音量
void VideoPlayer::volumnReduce()
{
    player->setVolume(player->volume() - volumnChangeStep);
}

//播放状态改变
void VideoPlayer::playStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::StoppedState)
    {
        QDesktopWidget desktop;
        this->setGeometry((desktop.width()-WIDTH)/2, (desktop.height()-HEIGHT)/2, WIDTH, HEIGHT);
        lab_background->setStyleSheet("border-image: url(:/Images/videoPlayerBg.png);");
        videoWidget->hide();

        videoContral->playStop();

        tbn_openVideoFile->show();

        //停止保持屏幕常亮
        timerKeepAwake->stop();
        //鼠标恢复
        hideCursor = false;
    }
    else if (state == QMediaPlayer::PlayingState)
    {
        videoWidget->show();
        videoWidget->setFocus();
        videoContral->setVideoTitle(playlist->currentMedia().canonicalUrl().fileName());
//        videoContral->setVideoTitle(tr("%1").arg(playlist->currentIndex()));
        //设置屏幕常亮
        timerKeepAwake->start(58000);
        //设置鼠标隐藏
        hideCursor = true;
    }
}

//保持屏幕常亮
void VideoPlayer::timeoutKeepAwake()
{
//    if(keepAwakeTimes % 2 == 0)
//    {
//        this->setWindowState(Qt::WindowActive);
//        this->setWindowState(Qt::WindowNoState);
    helpKeepAwake->setWindowState(Qt::WindowActive);
    helpKeepAwake->setWindowState(Qt::WindowNoState);
    videoWidget->setWindowState(Qt::WindowActive);
    videoWidget->setWindowState(Qt::WindowNoState);
//    }
//    else
//    {
//        helpKeepAwake->setWindowState(Qt::WindowActive);
//        helpKeepAwake->setWindowState(Qt::WindowNoState);
//    }
//    keepAwakeTimes++;
}
