#include "lrcview.h"

#include "currentlrc.h"

//#include <QPainter>
#include <QTimer>
//#include <QLinearGradient>
//#include <QPaintEvent>
/*
#include <QAxWidget>
#include <QtGui>
#include <QGridLayout>
*/
#include <QPropertyAnimation>

LrcView::LrcView(QWidget *parent) :
    QLabel(parent)
{
//////////////    setGeometry(276, 113, 731, 476);

    //创建 动画
    lab_left_top = new QLabel(this);
    lab_left_bottom = new QLabel(this);

    lab_left_top ->setGeometry(20, 40, 100, 100);
    lab_left_bottom  ->setGeometry(20, 100, 240, 320);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updateGif()));
    timer ->start(100);

    //创建 后续歌词
    textNext = new QLabel(this);
    textNext ->setGeometry(260, 260, 450, 200);
    textNext ->setAlignment(Qt::AlignTop);
    textNext ->setStyleSheet(
                "QLabel{"
//                    "color: rgba(40, 16, 144, 255);"
//                    "color: rgba(92, 133, 20, 255);"
//                    "color: rgba(95, 110, 13, 255);"       //黄绿色
                    "color: rgba(38, 79, 16, 255);"        //墨绿色
//                    "color: rgba(13, 72, 110, 255);"        //蓝绿色
                    "font-family: 楷体;"
                    "font-size: 20px;"
                "}"
                );

    //创建 当前播放歌词
    currentLrc = new CurrentLrc(this);
    currentLrc ->setGeometry(260, 232, 450, 26);

    //创建 播放过的歌词
    textPrevious = new QLabel(this);
    textPrevious ->setGeometry(260, 6, 450, 225);
    textPrevious ->setAlignment(Qt::AlignBottom);
    textPrevious ->setStyleSheet(
                "QLabel{"
                    "color: rgba(38, 79, 16, 255);"
                    "font-family: 楷体;"
                    "font-size: 20px;"
                "}"
                );
    setMouseTracking(true);
    textNext->setMouseTracking(true);
    currentLrc->setMouseTracking(true);
    textPrevious->setMouseTracking(true);
    lab_left_top->setMouseTracking(true);
    lab_left_bottom->setMouseTracking(true);


/*
    QAxWidget *flash = new QAxWidget(this);
    flash->setControl(QString::fromUtf8("{d27cdb6e-ae6d-11cf-96b8-444553540000}"));
    QString name=qApp->applicationDirPath();
    flash->dynamicCall("LoadMovie(long,string)",0,qApp->applicationDirPath()+"/top.swf");
    flash->show();
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(flash);
    layout->setMargin(0);
    setLayout(layout);
*/
/*
    QPropertyAnimation *animation = new QPropertyAnimation(lab_left_bottom, "pos");
    animation->setKeyValueAt(0.0, QPoint(lab_left_bottom->x(), lab_left_bottom->y()));
    animation->setKeyValueAt(0.5, QPoint(this->x()-lab_left_bottom->width(), lab_left_bottom->y()));
    animation->setKeyValueAt(1.0, QPoint(lab_left_bottom->x(), lab_left_bottom->y()));
    animation->setDuration(20000);
    animation->setLoopCount(-1);
    animation->start();
*/
    QPropertyAnimation *animation = new QPropertyAnimation(lab_left_bottom, "geometry");
    animation->setKeyValueAt(0.0, QRectF(lab_left_bottom->pos(), lab_left_bottom->size()));
    animation->setKeyValueAt(0.5, QRectF(lab_left_bottom->x()+50, lab_left_bottom->y()+50, lab_left_bottom->width()-100, lab_left_bottom->height()-133));
    animation->setKeyValueAt(1.0, QRectF(lab_left_bottom->pos(), lab_left_bottom->size()));
    animation->setDuration(20000);
    animation->setLoopCount(-1);
    animation->start();
}

//更新 Gif 动画
void LrcView::updateGif()
{
    lab_left_bottom ->setStyleSheet(
                tr(
                    "QLabel{"
                        "border-image: url(:/Images/gif%1.png);"
                    "}"
                    ).arg(gifNum++)
                );
    if (gifNum > 30)
    {
        gifNum = 1;
    }
}
