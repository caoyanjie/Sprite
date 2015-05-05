#include "desktoplrc.h"
#include <QDebug>

#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QDesktopWidget>

DesktopLrc::DesktopLrc(QWidget *parent) :
    QLabel(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    // 设置背景透明
    setAttribute(Qt::WA_TranslucentBackground);

    //设置窗口出现位置
    QDesktopWidget desktopWidget;
    setGeometry(desktopWidget.width() / 2 - 300, desktopWidget.height() - 80, 800, 600);

    // 固定部件大小
    setMaximumSize(800, 60);
    setMinimumSize(800, 60);

    setText(tr("兔兔音乐播放器----祝您听歌愉快"));

    // 歌词的线性渐变填充
    linearGradient.setStart(0, 10);
    linearGradient.setFinalStop(0, 40);
    linearGradient.setColorAt(0.1, QColor(86, 22, 229));
    linearGradient.setColorAt(0.5, QColor(35, 8, 124));
    linearGradient.setColorAt(0.9, QColor(58, 126, 20));

    // 遮罩的线性渐变填充
    maskLinearGradient.setStart(0, 10);
    maskLinearGradient.setFinalStop(0, 40);
    maskLinearGradient.setColorAt(0.1, QColor(58, 126, 20));
    maskLinearGradient.setColorAt(0.5, QColor(164, 198, 31));
    maskLinearGradient.setColorAt(0.9, QColor(86, 22, 229));

    // 设置字体
    font.setFamily("Times New Roman");
    font.setBold(true);
    font.setPointSize(30);

    // 设置定时器
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    lrcMaskWidth = 0;
    lrcMaskWidthInterval = 0;
}

void DesktopLrc::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setFont(font);

    // 先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(1, 1, 800, 60, Qt::AlignLeft, text());

    // 再在上面绘制渐变文字
    painter.setPen(QPen(linearGradient, 0));
    painter.drawText(0, 0, 800, 60, Qt::AlignLeft, text());

    // 设置歌词遮罩
    painter.setPen(QPen(maskLinearGradient, 0));
    painter.drawText(0, 0, lrcMaskWidth, 60, Qt::AlignLeft, text());
}

// 开启遮罩，需要指定当前歌词开始与结束之间的时间间隔
void DesktopLrc::startLrcMask(qint64 intervalTime)
{
    // 这里设置每隔30毫秒更新一次遮罩的宽度
    qreal count = intervalTime / 30;

    // 获取遮罩每次需要增加的宽度，这里的800是部件的固定宽度
    lrcMaskWidthInterval = 800 / count;
    lrcMaskWidth = 0;
    timer->start(30);
}

// 停止遮罩
void DesktopLrc::stopLrcMask()
{
    timer->stop();
    lrcMaskWidth = 0;
    update();
}

// 两个鼠标事件处理函数实现了部件的拖动
void DesktopLrc::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        offset = event->globalPos() - frameGeometry().topLeft();
    }
}
void DesktopLrc::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        setCursor(Qt::PointingHandCursor);
        move(event->globalPos() - offset);
    }
}

// 实现右键菜单来隐藏部件
void DesktopLrc::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(tr("隐藏"), this, SLOT(hide()));
    menu.exec(event->globalPos());
}

// 定时器溢出时增加遮罩的宽度，并更新显示
void DesktopLrc::timeout()
{
    lrcMaskWidth += lrcMaskWidthInterval;
    update();
}
