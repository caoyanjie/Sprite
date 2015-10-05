#include "videolist.h"
#include <QDesktopWidget>

#include <QLabel>
#include <QComboBox>
#include <QEvent>
#include <musiclist.h>

VideoList::VideoList(int x, int y, int width, int height, bool independ, QWidget *parent) :
    QWidget(parent),
    listWidth(width),
    listHeight(height)

{
    //窗口去边框
    if (independ)
    {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    }
    else
    {
        setWindowFlags(Qt::FramelessWindowHint);
    }

    //窗口透明
    setAttribute(Qt::WA_TranslucentBackground);

    //初始化窗口位置，大小
    setGeometry(x, y, width, height);
    setObjectName("videoList");

    //设置半透明背景
    lab_background = new QLabel(this);
    lab_background->setGeometry(0, 0, this->width(), this->height());
    lab_background->setObjectName("lab_background1");

    //创建部件
    lab_title = new QLabel("播放列表", lab_background);
    cob_addVideo = new QComboBox(lab_background);
    cob_addVideo->addItem("添加视频");
    cob_addVideo->addItem("添加目录");

    lab_title->setGeometry(5, 5, 80, 20);
    cob_addVideo->setGeometry(this->width()-85, 5, 80, 20);

    lab_title->setObjectName("lab_title");
    cob_addVideo->setObjectName("cob_addVideo");

    lab_background->installEventFilter(this);
    cob_addVideo->installEventFilter(this);
//    lab_background->setMouseTracking(true);
    //MusicList *lis = new MusicList(lab_background);

    //设置样式
    setStyleSheet(
                "#videoList{"
//                    "background: rgb(43, 45, 163);"
//                    "color: rgba(255, 255, 255, 255);"
                "}"

                "#lab_background1{"
//                    "background: rgba(0, 0, 0, 120);"
//                    "background: rgba(255, 255, 255, 80);"
//                    "background: rgba(255, 0, 0, 100);"
//                    "background: rgba(40, 62, 83, 195);"
                    "background: rgba(10, 0, 0, 120);"
                    "border: 1px solid rgba(255, 255, 255, 15);"
                "}"

                "#lab_title{"
                    "color: rgba(200, 255, 255, 255);"
                    "font-family: 宋体;"
                    "font-size: 11pt;"
 //                   "font-weight: bold;"
                "}"

                "#cob_addVideo{"
                    "background: rgba(255, 255, 255, 70);"
                    "border-radius: 3px;"
                    "color: rgba(200, 255, 255, 255);"
                    "border-radius: 3px;"
//                    "min-width: 6em;"
                "}"
                "#cob_addVideo:drop-down{"
                    "background: rgba(0, 0, 0, 0);"
                    "border-image: url(:/Images/cobIcon.png);"
                "}"
                "#cob_addVideo QListView{"
                    "color: red;"
                "}"
                "#cob_addVideo QAbstractItemView{"
                    "padding: 5pt;"
                    "color: blue;"
                "}"
                );
}

//事件过滤器
bool VideoList::eventFilter(QObject *target, QEvent *event)
{
    if (target == lab_background)
    {
        switch(event->type())
        {
        case QEvent::Enter:
            emit isMouseEnter(true);
            break;
        case QEvent::Leave:
            emit isMouseEnter(false);
            break;
        }
    }
    else if (target == cob_addVideo)
    {
        switch(event->type())
        {
        case QEvent::Enter:
            emit isMouseEnter(true);
            break;
        case QEvent::Leave:
            emit isMouseEnter(false);
            break;
        default:
            emit isMouseEnter(true);
        }
    }
    return QWidget::eventFilter(target, event);
}

//获得视频的分辨率，使窗口适应视频的大小
void VideoList::gotVideoSize(QSize size)
{
    resize(listWidth, size.height());
    lab_background->resize(listWidth, size.height());
}

//同步移动窗口
void VideoList::toMoveWindow(QPoint point)
{
    move(point.x(), point.y()+25);
}
