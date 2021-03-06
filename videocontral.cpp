#include "videocontral.h"
#include <QMessageBox>
#include <QListView>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>

VideoContral::VideoContral(QWidget *parent) : QWidget(parent)
  ,tbnHeaderSize(12)
  ,tbnContralSize(22)
  ,playlistMaxWidth(300)
  ,forwardStep(5)
  ,retreatStep(5)
{
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::SubWindow);
    this->setAttribute(Qt::WA_TranslucentBackground);

    //顶部工具栏按钮
    fram_header = new QWidget;
    lab_windowTitle = new QLabel("兔兔播放器");
    lab_videoFileName = new QLabel;
    tbn_windowClose = new QToolButton;
    tbn_windowMini = new QToolButton;
    tbn_setting = new QToolButton;
    tbn_switchMusicPlayer = new QToolButton;
    tbn_windowClose->setFixedSize(tbnHeaderSize, tbnHeaderSize);
    tbn_windowMini->setFixedSize(tbnHeaderSize, tbnHeaderSize);
    tbn_setting->setFixedSize(tbnHeaderSize, tbnHeaderSize);
    tbn_switchMusicPlayer->setFixedSize(tbnHeaderSize, tbnHeaderSize);
    //布局
    layout_tbnHeader = new QHBoxLayout;
    layout_tbnHeader->addWidget(lab_windowTitle);
    layout_tbnHeader->addStretch();
    layout_tbnHeader->addWidget(lab_videoFileName);
    layout_tbnHeader->addStretch();
    layout_tbnHeader->addWidget(tbn_switchMusicPlayer);
    layout_tbnHeader->addWidget(tbn_setting);
    layout_tbnHeader->addWidget(tbn_windowMini);
    layout_tbnHeader->addWidget(tbn_windowClose);
    layout_tbnHeader->setSpacing(10);
    layout_tbnHeader->setMargin(8);
    fram_header->setLayout(layout_tbnHeader);

    //播放时间
    lab_totalTime = new QLabel;
    lab_currentTime = new QLabel;
    layout_playTime = new QHBoxLayout;
    layout_playTime->addStretch();
    layout_playTime->addWidget(lab_currentTime);
    layout_playTime->addWidget(lab_totalTime);
    layout_playTime->setSpacing(0);
    layout_playTime->setMargin(0);

    //底部播放控制按钮
    lab_playContralFram = new QLabel;

    slider_position = new QSlider(Qt::Horizontal);  //播放进度条
    tbn_playStop = new QToolButton;
    tbn_playPrevious = new QToolButton;
    tbn_playPause = new QToolButton;
    tbn_playPause->setShortcut(Qt::Key_Space);
    tbn_playNext = new QToolButton;
    tbn_volumn = new QToolButton;
    tbn_playStop->setFixedSize(tbnContralSize, tbnContralSize);
    tbn_playPrevious->setFixedSize(tbnContralSize, tbnContralSize);
    tbn_playPause->setFixedSize(tbnContralSize, tbnContralSize);
    tbn_playNext->setFixedSize(tbnContralSize, tbnContralSize);
    tbn_volumn->setFixedSize(tbnContralSize, tbnContralSize);
    layout_tbnPlayContral = new QHBoxLayout;
    layout_tbnPlayContral->addStretch(5);
    layout_tbnPlayContral->addWidget(tbn_playStop, 1);
    layout_tbnPlayContral->addWidget(tbn_playPrevious, 1);
    layout_tbnPlayContral->addWidget(tbn_playPause, 1);
    layout_tbnPlayContral->addWidget(tbn_playNext, 1);
    layout_tbnPlayContral->addWidget(tbn_volumn, 1);
//    layout_tbnPlayContral->addStretch();
//    layout_tbnPlayContral->addWidget(lab_currentTime);
//    layout_tbnPlayContral->addWidget(lab_totalTime);
    layout_tbnPlayContral->addLayout(layout_playTime, 4);
    layout_tbnPlayContral->setSpacing(20);
    layout_tbnPlayContral->setContentsMargins(5, 0, 20, 5);

    layout_bottom = new QVBoxLayout;
    layout_bottom->addWidget(slider_position);
    layout_bottom->addLayout(layout_tbnPlayContral);
    layout_bottom->setSpacing(0);
    layout_bottom->setMargin(0);
    lab_playContralFram->setLayout(layout_bottom);
    lab_playContralFram->setFixedHeight(tbnContralSize+30);

    //播放列表
    lab_playlistFram = new QLabel;
    lab_playlistFram->setMaximumWidth(playlistMaxWidth);

    lab_playlist = new QLabel("播放列表");
    cob_addFile = new QComboBox;
    cob_addFile->addItem("添加视频");
    cob_addFile->addItem("添加目录");
    cob_addFile->setFixedSize(80, 18);
    layout_playlistHeader = new QHBoxLayout;
    layout_playlistHeader->addWidget(lab_playlist);
    layout_playlistHeader->addStretch();
    layout_playlistHeader->addWidget(cob_addFile);
    layout_playlistHeader->setSpacing(0);
    layout_playlistHeader->setMargin(5);

//    list_playlist = new QListWidget;
    list_playlist = new QTreeWidget;
    list_playlist->setHeaderHidden(true);
    newVideoItem = new QTreeWidgetItem(QStringList(QString("播放历史")));
    list_playlist->addTopLevelItem(newVideoItem);
    newVideoItem = new QTreeWidgetItem(QStringList(QString("当前列表")));
    list_playlist->addTopLevelItem(newVideoItem);
    list_playlist->topLevelItem(1)->setExpanded(true);

    layout_playlist = new QVBoxLayout;
    layout_playlist->addLayout(layout_playlistHeader);
    layout_playlist->addWidget(list_playlist);
    layout_playlist->setSpacing(0);
    layout_playlist->setMargin(0);

    lab_playlistFram->setLayout(layout_playlist);

    //
    tbn_openFile = new QToolButton;
    slider_volumn = new QSlider;

    //子布局
    layout_middle = new QHBoxLayout;
    layout_middle->addWidget(lab_playlistFram, 4);
    layout_middle->addStretch(9);
    layout_middle->setSpacing(0);
    layout_middle->setMargin(0);
//lab_playContralFram->setFixedHeight(50);
    //顶级布局
    layout_top = new QVBoxLayout;
//    layout_top->addLayout(layout_tbnHeader);
    layout_top->addWidget(fram_header);
    layout_top->addLayout(layout_middle);
//    layout_top->addWidget(slider_position);
//    layout_top->addLayout(layout_tbnPlayContral);
//    layout_top->addLayout(layout_bottomG);
    layout_top->addWidget(lab_playContralFram);
    layout_top->setSpacing(0);
    layout_top->setMargin(0);
    this->setLayout(layout_top);

    //安装事件过滤器
    fram_header->installEventFilter(this);
    lab_playContralFram->installEventFilter(this);
    lab_playlistFram->installEventFilter(this);
    lab_playlistFram->installEventFilter(this);
    //    layout_tbnHeader->installEventFilter(this);

    //关联 信号/槽
    connect(tbn_windowClose, SIGNAL(clicked()), parent, SLOT(close()));
    connect(tbn_windowMini, SIGNAL(clicked()), parent, SLOT(showMinimized()));
    connect(tbn_switchMusicPlayer, SIGNAL(clicked()), parent, SIGNAL(switchMusicPlayer()));
    connect(slider_position, SIGNAL(sliderMoved(int)), parent, SLOT(valueChanged(int)));
    connect(this, SIGNAL(valueChanged(int)), parent, SLOT(valueChanged(int)));
    connect(tbn_playStop, SIGNAL(clicked()), this, SIGNAL(playerStop()));
    connect(tbn_playPause, SIGNAL(clicked()), this, SIGNAL(playPause()));
    connect(tbn_playNext, SIGNAL(clicked()), this, SIGNAL(playNext()));
    connect(cob_addFile, SIGNAL(activated(int)), this, SLOT(addVidwoFile(int)));

    //双击播放列表
    connect(list_playlist, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));

    //显式设置对象名
    fram_header->setObjectName("fram_header");
    tbn_windowClose->setObjectName("tbn_windowClose");
    tbn_windowMini->setObjectName("tbn_windowMini");
    tbn_setting->setObjectName("tbn_setting");
    tbn_switchMusicPlayer->setObjectName("tbn_switchMusicPlayer");
    tbn_playStop->setObjectName("tbn_playStop");
    tbn_playPrevious->setObjectName("tbn_playPrevious");
    tbn_playPause->setObjectName("tbn_playPause");
    tbn_playNext->setObjectName("tbn_playNext");
    tbn_volumn->setObjectName("tbn_volumn");
    tbn_openFile->setObjectName("tbn_openFile");
    slider_position->setObjectName("slider_position");
    slider_volumn->setObjectName("slider_volumn");
    lab_windowTitle->setObjectName("lab_windowTitle");
    lab_videoFileName->setObjectName("lab_videoFileName");
    lab_playlistFram->setObjectName("lab_playlistFram");
    lab_playlist->setObjectName("lab_playlist");
    lab_playContralFram->setObjectName("lab_playContralFram");
    lab_totalTime->setObjectName("lab_totalTime");
    lab_currentTime->setObjectName("lab_currentTime");
    list_playlist->setObjectName("list_playlist");
    cob_addFile->setObjectName("cob_addFile");

    //设置样式表
    this->setStyleSheet(
                "#lab_windowTitle, #lab_videoFileName, #lab_totalTime, #lab_currentTime, #list_playlist{"
                    "color: rgba(200, 255, 255, 255);"
                "}"
                "#tbn_switchMusicPlayer{"
                    "border-image: url(:/Images/tbn_switchMusicPlayer3.png);"
                "}"
                "#tbn_switchMusicPlayer:hover{"
                    ""
                "}"
                "#tbn_setting{"
                    "border-image: url(:Images/setting.png);"
                "}"
                "#tbn_setting:hover{"
                    "border-image: url(:Images/setting_hover.png);"
                "}"

                "#tbn_windowMini{"
                    "border-image: url(:Images/miniWindow.png);"
                "}"
                "#tbn_windowMini:hover{"
                    "border-image: url(:Images/miniWindow_hover.png);"
                "}"
                "#tbn_windowClose{"
                    "border-image: url(:Images/closeWindow.png);"
                "}"
                "#tbn_windowClose:hover{"
                    "border-image: url(:Images/closeWindow_hover.png);"
                "}"

                //底部播放控制按钮
                "#tbn_playStop{"
                    "border-image: url(:Images/play_stop.png);"
                "}"
                "#tbn_playStop:hover{"
                    "border-image: url(:Images/play_stop_hover.png);"
                "}"

                "#tbn_playPrevious{"
                    "border-image: url(:Images/play_previous.png);"
                "}"
                "#tbn_playPrevious:hover{"
                    "border-image: url(:Images/play_previous_hover.png);"
                "}"

                "#tbn_playPause{"
                    "border-image: url(:Images/play_pause.png);"
                "}"
                "#tbn_playPause:hover{"
                    "border-image: url(:Images/play_pause_hover.png);"
                "}"

                "#tbn_playNext{"
                    "border-image: url(:Images/play_next.png);"
                "}"
                "#tbn_playNext:hover{"
                    "border-image: url(:Images/play_next_hover.png);"
                "}"
                "#tbn_volumn{"
                    "border-image: url(:/Images/volumn.png);"
                "}"
                "#tbn_volumn:hover{"
                    "border-image: url(:/Images/volumn_hover.png);"
                "}"

                //进度条
                "#slider_position::groove:horizontal{"                                       //设置 进度条        样式
                    "border: 1px solid #000000;"
                    "height: 2px;"
                    "margin: 0px 0;"
                    "left: 12px;"
                    "right: 12px;"
                "}"
                "#slider_position::handle:horizontal{"
                    "border: 1px solid #5c5c5c;"
                    "border-image: url(:/Images/sliderHandle.png);"
                    "width: 10px;"
//                                        "height: 15px;"
                    "margin: -5px -5px -5px -5px;"
                "}"
                "#slider_position::sub-page:horizontal{"
                    "background: rgb(115, 153, 1);"
                "}"
                "#slider_position::add-page:horizontal{"
                    "background: rgb(83, 83, 83);"
                "}"
                "#slider_position::handle::hover:horizontal{"
                    "border-image: url(:/Images/sliderHandle_hover.png);"
                "}"

                //框架背景
                "#fram_header, #lab_playlistFram, #lab_playContralFram{"
//                    "background: rgba(0, 0, 0, 120);"
//                    "background: rgba(255, 255, 255, 80);"
//                    "background: rgba(255, 0, 0, 100);"
//                    "background: rgba(40, 62, 83, 195);"
                    "background: rgba(10, 0, 0, 120);"
                    "border: 1px solid rgba(255, 255, 255, 15);"
                "}"
                "#lab_playlist{"
                    "color: rgba(200, 255, 255, 255);"
                    "font-family: 宋体;"
                    "font-size: 10pt;"
                "}"
                "#cob_addFile{"                                   //设置 添加音乐 样式表
                    "background: rgba(255, 0, 0, 100);"
                    "margin: 0;"
                    "padding: 0 0 0 8px;"
                    "border-radius: 2px;"
                    "color: rgb(200, 255, 255);"
                "}"
                "#cob_addFile QAbstractItemView::item{height: 20px;}"  //下拉选项高度
                "#cob_addFile::drop-down{"                             //jian tou xiao fang kuai
                    "background: rgba(255, 255, 255, 0);"
                    "border-image: url(:/Images/cobIcon.png);"
                "}"
                "#list_playlist{"
                    "background-color: rgba(0, 0, 0, 30);"
//                    "border: 1px solid rgba(255, 255, 255, 15);"
                "}"
                "#list_playlist::item{"
                    "height: 33px;"
                "}"
                );
    cob_addFile->setView(new QListView());                             //如果没有这句，下拉选项高度设置不起作用
}

VideoContral::~VideoContral()
{

}

void VideoContral::setVideoTitle(QString videoFileName)
{
    lab_videoFileName->setText(videoFileName);
}

//添加到视频播放列表
void VideoContral::addVideoList(QStringList videoPaths)
{
    for(int indext=0; indext<videoPaths.length(); indext++)
    {
        newVideoItem = new QTreeWidgetItem(QStringList(QString(tr("%1").arg(videoPaths.at(indext)))));
        list_playlist->topLevelItem(1)->addChild(newVideoItem);
    }
}

void VideoContral::playStop()
{
    lab_videoFileName->clear();
    lab_currentTime->clear();
    lab_currentTime->setText("我已经结束了");
    lab_totalTime->clear();
}

//事件过滤器
bool VideoContral::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == layout_playlistHeader
            || obj == lab_playlistFram
            || obj == lab_playContralFram
            || obj == fram_header)
    {
        switch (event->type())
        {
            case QEvent::Enter:
                emit isHideWidget(true);
                break;
            case QEvent::Leave:
                emit isHideWidget(false);
                break;
        }
    }
    else if (obj == cob_addFile)
    {
        switch(event->type())
        {
        case QEvent::Enter:
            emit isHideWidget(true);
            break;
        case QEvent::Leave:
            emit isHideWidget(false);
            break;
        default:
            emit isHideWidget(true);
        }
    }
    return QWidget::eventFilter(obj, event);
}

//键盘按下事件
void VideoContral::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Right:
//        emit valueChanged(forwardStep*1000);    //快进（默认5秒）
        emit playJump(forwardStep*1000);
        break;
    case Qt::Key_Left:
//        emit valueChanged(-forwardStep*1000);   //快退（默认5秒）
        emit playJump(-retreatStep*1000);
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        emit switchFullScreen();
        break;
    }
}

//添加视频文件
void VideoContral::addVidwoFile(int index)
{
    QStringList videoFiles;
    if (index == 0)
    {
        videoFiles = QFileDialog::getOpenFileNames(0, tr("打开视频"), tr("D:/"));
    }
    else if (index == 1)
    {
        QString dirName = QFileDialog::getExistingDirectory(0, tr("选择视频文件夹"), tr("D:/"));
        QStringList getVideoFiles = QDir(dirName).entryList();
        for (int i=0; i<getVideoFiles.count(); ++i)
        {
            QString fileName = getVideoFiles.at(i);
            if (fileName != "." && fileName != "..")
            {
                videoFiles.append(fileName);
            }
        }
    }
    if (!videoFiles.isEmpty())
    {
        for (int times=0; times<videoFiles.count(); ++times)
        {
            newVideoItem = new QTreeWidgetItem(QStringList(QString(QFileInfo(videoFiles.at(times)).baseName())));
            list_playlist->topLevelItem(1)->addChild(newVideoItem);
        }
        emit addedVideoFiles(videoFiles);
    }
}

//获得视频时长
void VideoContral::durationChanged(qint64 totalTime)
{
    /*
    qDebug() << tr("%1").sprintf("%d", (qint64)1);                  // 1
    qDebug() << tr("%1").sprintf("%d", (qint64)44);                 // 44
    qDebug() << tr("%1").sprintf("%2d:%2d", (qint64)1, (qint64)44); // 1:0
    */

    lab_totalTime->setText(tr("%1").sprintf("/%02d:%02d:%02d", (int)totalTime/1000/3600, (int)totalTime/1000%3600/60, (int)totalTime/1000%60));
    lab_currentTime->setText(tr("00:00:00"));
    slider_position->setMaximum(totalTime);
}

//播放进度
void VideoContral::positionChanged(qint64 currentPositon)
{
    lab_currentTime->setText(tr("%1").sprintf("%02d:%02d:%02d", (int)currentPositon/1000/3600, (int)currentPositon/1000%3600/60, (int)currentPositon/1000%60));
    slider_position->setValue(currentPositon);
    if (slider_position->value() == slider_position->maximum())
    {
        emit currentMediaEnd();
    }
}

//双击列表中的电影
void VideoContral::itemDoubleClicked(QTreeWidgetItem *treeItem, int index)
{
    for (int toplevel=0; toplevel<list_playlist->topLevelItemCount(); ++toplevel)
    {
        if (list_playlist->topLevelItem(toplevel)->child(index) == treeItem)
        {
            emit itemDoubleClicked(toplevel, index);
            return;
        }
    }
}

