#include "currentlrc.h"

#include <QPainter>
#include <QTimer>

CurrentLrc::CurrentLrc(QWidget *parent) :
    QLabel(parent)
{
    // 歌词的线性渐变填充
    linearGradient.setStart(0, 5);
    linearGradient.setFinalStop(0, 16);
//    linearGradient.setColorAt(0.1, QColor(14, 179, 255));
//    linearGradient.setColorAt(0.5, QColor(114, 232, 255));
//    linearGradient.setColorAt(0.9, QColor(14, 179, 255));
//    linearGradient.setColorAt(0.1, QColor(13, 72, 110));
//    linearGradient.setColorAt(0.5, QColor(13, 72, 110));
//    linearGradient.setColorAt(0.9, QColor(13, 72, 110));
//    linearGradient.setColorAt(0.1, QColor(58, 126, 20));
    linearGradient.setColorAt(0.1, QColor(200, 200, 0));
//    linearGradient.setColorAt(0.5, QColor(255, 110, 2));
    linearGradient.setColorAt(0.9, QColor(204, 255, 0));

    // 遮罩的线性渐变填充
    maskLinearGradient.setStart(0, 10);
    maskLinearGradient.setFinalStop(0, 40);
//    maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
//    maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
//    maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));
    maskLinearGradient.setColorAt(0.1, QColor(255, 110, 2));
    maskLinearGradient.setColorAt(0.5, QColor(255, 255, 0));
    maskLinearGradient.setColorAt(0.9, QColor(255, 109, 0));

    // 设置字体
    font.setFamily("楷体");
    font.setBold(true);
    font.setPointSize(16);

    // 设置定时器
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    lrcMaskWidth = 0;
    lrcMaskWidthInterval = 0;
}

void CurrentLrc::startLrcMask(qint64 intervalTime)
{
    // 这里设置每隔30毫秒更新一次遮罩的宽度
    qreal count = intervalTime / 30;

    // 获取遮罩每次需要增加的宽度，这里的800是部件的固定宽度
    lrcMaskWidthInterval = 450 / count;
    lrcMaskWidth = 0;
    timer->start(30);
}

void CurrentLrc::stopLrcMask()
{
    timer->stop();
    lrcMaskWidth = 0;
    update();
}

void CurrentLrc::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setFont(font);

    // 先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
    painter.setPen(QColor(0, 0, 0, 200));
//    painter.drawText(1, 1, 800, 60, Qt::AlignLeft, text());

    // 再在上面绘制渐变文字
    painter.setPen(QPen(linearGradient, 0));
    painter.drawText(0, 0, 450, 26, Qt::AlignLeft, text());

    // 设置歌词遮罩
    painter.setPen(QPen(maskLinearGradient, 0));
    painter.drawText(0, 0, lrcMaskWidth, 26, Qt::AlignLeft, text());
}

void CurrentLrc::timeout()
{
    lrcMaskWidth += lrcMaskWidthInterval;
    update();
}
