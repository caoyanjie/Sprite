#include "musicminiplayer.h"
#include <QDebug>

#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QMovie>
#include <QTimer>
#include <QPainter>

MusicMiniPlayer::MusicMiniPlayer(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowOpacity(0.9);
    resize(230, 95);

    lab_bg = new QLabel(this);
    lab_bg->resize(this->width(), this->height());
    lab_bg->installEventFilter(this);

    lab_logo = new QLabel(lab_bg);
    lab_musicName = new QLabel(lab_bg);
    lab_musicMsg = new QLabel(lab_bg);
    lab_logo->setFixedSize(66, 66);
    lab_logo->move(10, 14);
    lab_musicName->move(85, 16);
    lab_musicMsg->move(85, 36);
    lab_musicName->setText("歌曲：");
    lab_musicMsg->setText("歌手：");
    lab_logo->setAlignment(Qt::AlignCenter);
    
    lab_buttonsParent = new QLabel(lab_bg);
    lab_buttonsParent->resize(this->width(), this->height());
    lab_buttonsParent->installEventFilter(this);
    lab_buttonsParent->setMouseTracking(true);

    tbn_play_previous = new QToolButton(lab_buttonsParent);
    tbn_play = new QToolButton(lab_buttonsParent);
    tbn_play_next = new QToolButton(lab_buttonsParent);
    tbn_volumn_plus = new QToolButton(lab_buttonsParent);
    tbn_volumn_less = new QToolButton(lab_buttonsParent);
    tbn_closeWindow = new QToolButton(lab_buttonsParent);
    tbn_hideWindow = new QToolButton(lab_buttonsParent);
    tbn_miniWindow = new QToolButton(lab_buttonsParent);
    tbn_simple = new QToolButton(lab_buttonsParent);
    tbn_normal = new QToolButton(lab_buttonsParent);
    tbn_simple->setText("精简");
    tbn_normal->setText("标准");

    //为按钮安装事件过滤器
    tbn_play_previous->installEventFilter(this);
    tbn_play->installEventFilter(this);
    tbn_play_next->installEventFilter(this);
    tbn_volumn_plus->installEventFilter(this);
    tbn_volumn_less->installEventFilter(this);
    tbn_closeWindow->installEventFilter(this);
    tbn_hideWindow->installEventFilter(this);
    tbn_miniWindow->installEventFilter(this);
    tbn_simple->installEventFilter(this);
    tbn_normal->installEventFilter(this);

    connect(tbn_closeWindow, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(tbn_miniWindow, SIGNAL(clicked()),
            this, SLOT(showMinimized()));
    connect(tbn_normal, SIGNAL(clicked()),
            this, SLOT(showNormalWindow()));

/*
    tbn_play_previous->setGeometry(20, (tbn_play_previous->parentWidget()->height()-tbn_play_previous->height())/2, 20, 20);
    tbn_play->setGeometry(45, (tbn_play->parentWidget()->height()-tbn_play->height())/2, 20, 20);
    tbn_play_next->setGeometry(70, (tbn_play_next->parentWidget()->height()-tbn_play_next->height())/2, 20, 20);
*/
    tbn_play_previous->setFixedSize(20, 20);
    tbn_play->setFixedSize(20, 20);
    tbn_play_next->setFixedSize(20, 20);
    tbn_volumn_plus->setFixedSize(20, 20);
    tbn_volumn_less->setFixedSize(20, 20);
    tbn_closeWindow->setFixedSize(11, 11);
    tbn_hideWindow->setFixedSize(11, 11);
    tbn_simple->setFixedSize(25, 14);
    tbn_normal->setFixedSize(25, 14);
    tbn_miniWindow->setFixedSize(11, 11);

    //布局
    layout_topright = new QHBoxLayout;
//    layout_topright->addStretch();
    layout_topright->addWidget(tbn_normal);
    layout_topright->addWidget(tbn_simple);
    layout_topright->addWidget(tbn_miniWindow);
    layout_topright->addWidget(tbn_hideWindow);
    layout_topright->addWidget(tbn_closeWindow);
    layout_topright->setSpacing(5);
    layout_topright->setMargin(0);

    layout_right = new QVBoxLayout;
    layout_right->addLayout(layout_topright);
    layout_right->addStretch();
    layout_right->setSpacing(0);
    layout_right->setMargin(0);

    layout_middle_left = new QGridLayout;
    layout_middle_left->addWidget(tbn_volumn_plus, 0, 1, 1, 1);
    layout_middle_left->addWidget(tbn_play_previous, 1, 0, 1, 1);
    layout_middle_left->addWidget(tbn_play, 1, 1, 1, 1);
    layout_middle_left->addWidget(tbn_play_next, 1, 2, 1, 1);
    layout_middle_left->addWidget(tbn_volumn_less, 2, 1, 1, 1);
    layout_middle_left->setSpacing(5);
    layout_middle_left->setMargin(0);

    layout_middle = new QHBoxLayout;
    layout_middle->addLayout(layout_middle_left);
    layout_middle->addStretch();
    layout_middle->setSpacing(0);
    layout_middle->setMargin(0);

//    layout_top = new QVBoxLayout;
    layout_top = new QHBoxLayout;
//    layout_top->addLayout(layout_topright);
    layout_top->addLayout(layout_middle);
    layout_top->addStretch();
//    layout_top->addLayout(layout_topright);
    layout_top->addLayout(layout_right);
//    layout_top->addStretch();
    layout_top->setSpacing(0);
    layout_top->setContentsMargins(10, 5, 5, 5);
    lab_buttonsParent->setLayout(layout_top);

    lab_buttonsParent->hide();

    //动画
    animation_previous = new QPropertyAnimation(lab_buttonsParent, "pos");
    animation_previous->setStartValue(QPoint(0, -lab_buttonsParent->height()));
    animation_previous->setEndValue(QPoint(0, 0));
//    connect(animation_previous, SIGNAL(finished()),
//            this, SLOT(TEST()));
/*
    animation_play = new QPropertyAnimation(tbn_play, "pos");
    animation_play->setStartValue(QPoint(tbn_play_previous->x()+tbn_play_previous->width()+5, -tbn_play->height()));
    animation_play->setEndValue(QPoint(tbn_play_previous->x()+tbn_play_previous->width()+5, (lab_bg->height()-tbn_play->height())/2));

    animation_next = new QPropertyAnimation(tbn_play_next, "pos");
    animation_next->setStartValue(QPoint(tbn_play->x()+tbn_play->width()+5, -tbn_play_next->height()));
    animation_next->setEndValue(QPoint(tbn_play->x()+tbn_play->width()+5, (lab_bg->height()-tbn_play_next->height())/2));

    animationG = new QParallelAnimationGroup(lab_bg);
    animationG->addAnimation(animation_previous);
    animationG->addAnimation(animation_play);
    animationG->addAnimation(animation_next);
*/
    setObjectName("this");
    lab_bg->setObjectName("lab_bg");
    lab_buttonsParent->setObjectName("lab_buttonsParent");
    tbn_play->setObjectName("tbn_play");
    tbn_play_previous->setObjectName("tbn_play_previous");
    tbn_play_next->setObjectName("tbn_play_next");
    tbn_volumn_plus->setObjectName("tbn_volumn_plus");
    tbn_volumn_less->setObjectName("tbn_volumn_less");
    tbn_closeWindow->setObjectName("tbn_closeWindow");
    tbn_hideWindow->setObjectName("tbn_hideWindow");
    tbn_miniWindow->setObjectName("tbn_miniWindow");
    tbn_simple->setObjectName("tbn_simple");
    tbn_normal->setObjectName("tbn_normal");
    lab_logo->setObjectName("lab_miniLogo");
    lab_musicName->setObjectName("lab_musicName");
    lab_musicMsg->setObjectName("lab_musicMsg");

/*
    layout_H = new QHBoxLayout;
    layout_H->addWidget(tbn_play_previous);
    layout_H->addWidget(tbn_play);
    layout_H->addWidget(tbn_play_next);
    layout_H->addStretch();
    layout_H->setSpacing(5);
    lab_bg->setLayout(layout_H);
*/
    QMatrix logoMatrix;
    logoMatrix.rotate(rotateAngle -= angleOffset);
    lab_logo->setPixmap(QPixmap(":/Images/miniLogo_rotate.png").transformed(logoMatrix,Qt::SmoothTransformation));
    setStyleSheet(
                "#this{"
                    "border-radius: 8px;"
                "}"
                "#lab_miniLogo{"
//                    "image: url(:/Images/miniLogo);"
                    "border: 1px solid rgb(94, 31, 172);"
                    "border-radius: 8px;"
                    "background: rgba(0, 0, 0, 80);"
                "}"
                "#lab_musicName, #lab_musicMsg{"
//                    "font-weight: bold;"
                    "color: rgb(23, 215, 255);"
                "}"
                "#lab_bg{"
//                    "background: rgba(53, 78, 74, 220);"
                    "background: rgba(0, 0, 0, 120);"
                    "border: 1px solid rgba(255, 255, 255, 80);"
                    "border-radius: 8px;"
//                    "border-image: url(:/Images/mini_bg1.png);"
                "}"
                "#lab_buttonsParent{"
                    "background: rgba(0, 0, 0, 80);"
                    "border-radius: 8px;"
                //    "border-image: url(:/Images/mini_bg3.png);"
                "}"
                "#tbn_play_previous{"
                    "border-image: url(:/Images/play_previous.png);"
                "}"
                "#tbn_play{"
                    "border-image: url(:/Images/play_pause.png);"
                "}"
                "#tbn_play_next{"
                    "border-image: url(:/Images/play_next.png);"
                "}"
                "#tbn_volumn_plus{"
                    "border-image: url(:/Images/volumn_plus.png);"
                "}"
                "#tbn_volumn_less{"
                    "border-image: url(:/Images/volumn_less.png);"
                "}"
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
                "#tbn_miniWindow{"
                    "border-image: url(:/Images/miniWindow.png);"
                "}"
                "#tbn_miniWindow:hover{"
                    "border-image: url(:/Images/miniWindow_hover.png);"
                "}"
                "#tbn_simple{"
                    "background: rgba(0, 0, 0, 0);"
//                    "color: rgba(230, 230, 230, 255);"
//                    "color: gray;"
                    "color: rgba(200, 200, 200, 255);"
                "}"
                "#tbn_normal{"
                    "background: rgba(0, 0, 0, 0);"
                    "color: rgb(23, 215, 255);"
                "}"
                );
//    QMovie *movie = new QMovie(":/Images/playing.gif", QByteArray(), lab_logo);
//    lab_logo->setMovie(movie);
//    movie->start();

    timer_logoRotate = new QTimer(this);
    connect(timer_logoRotate, SIGNAL(timeout()),
            this, SLOT(timeout_logoRotate()));
    timer_logoRotate->start(timer_rotateStart);
}
//鼠标按下事件
void MusicMiniPlayer::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        offest = event->globalPos()- pos();
    }
}
//鼠标拖动事件
void MusicMiniPlayer::mouseMoveEvent(QMouseEvent *event)
{
    if (event ->buttons() & Qt::LeftButton)
    {
        QPoint temp;
        temp = event ->globalPos() - offest;
        move(temp);
    }
}

//事件过滤器
bool MusicMiniPlayer::eventFilter(QObject *target, QEvent *event)
{
    if (target == lab_bg)
    {
        if (event->type() == QEvent::Enter)
        {
            lab_buttonsParent->setVisible(true);
            animation_previous->start();
            lab_logo->hide();
        }
        else if (event->type() == QEvent::Leave)
        {
            lab_buttonsParent->hide();
            lab_logo->setVisible(true);
        }
        else if (event->type() == QEvent::MouseMove)
        {
            event->ignore();
        }
        else
        {
            return QWidget::eventFilter(target, event);
        }
    }
    else if (target == tbn_closeWindow
             || target == tbn_hideWindow
             || target == tbn_miniWindow
             || target == tbn_normal
             || target == tbn_play
             || target == tbn_play_next
             || target == tbn_play_previous
             || target == tbn_simple
             || target == tbn_volumn_less
             || target == tbn_volumn_plus
             )
    {
        if (event->type() == QEvent::MouseMove)
        {
//            event->accept();
        }
        else
        {
            return QWidget::eventFilter(target, event);
        }
    }
    else if (target == lab_buttonsParent)
    {
        if (event->type() == QEvent::Paint)
        {
            QPainter painter(lab_buttonsParent);
            painter.setPen(QPen(QColor(52, 148, 42), 2));

            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawLine(0, lab_buttonsParent->height()/2, 44, 0);
            painter.drawLine(44, 0, 88, lab_buttonsParent->height()/2);
            painter.drawLine(0, lab_buttonsParent->height()/2, 44, lab_buttonsParent->height());
            painter.drawLine(44, lab_buttonsParent->height(), 88, lab_buttonsParent->height()/2);

            //绘制十字线
//            painter.drawLine(44, 0, 44, 100);
//            painter.drawLine(0, lab_buttonsParent->height()/2, 88, lab_buttonsParent->height()/2);

            //绘制小圆
            painter.translate(30, lab_buttonsParent->height()/2-15);
            painter.drawEllipse(0, 0, 30, 30);

            //绘制菱形
//            painter.rotate(45);
//            painter.drawRect(0, 0, 60, 60);
        }
    }


    return QWidget::eventFilter(target, event);
}

void MusicMiniPlayer::paintEvent(QPaintEvent *)
{

}

void MusicMiniPlayer::paintOnWidget()
{
    QPainter painter;
    painter.begin(lab_buttonsParent);
    painter.drawLine(0, 10, 100, 10);
    painter.end();
}

//切换标准播放窗口
void MusicMiniPlayer::showNormalWindow()
{
    this->parentWidget()->parentWidget()->parentWidget()->show();
    this->deleteLater();
}

void MusicMiniPlayer::timeout_logoRotate()
{
    QMatrix logoMatrix;
    logoMatrix.rotate(rotateAngle += angleOffset);
    lab_logo->setPixmap(QPixmap(":/Images/miniLogo_rotate.png").transformed(logoMatrix,Qt::SmoothTransformation));
}
