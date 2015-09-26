#include "videowidget.h"

#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>

VideoWidget::VideoWidget(QWidget *parent) :
    QVideoWidget(parent)
  ,forwardStep(5)
  ,retreatStep(5)
{
    setWindowFlags(Qt::FramelessWindowHint);

    //消除视频白边
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::black);
    this->setPalette(p);
}

//鼠标按下事件
void VideoWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mouseClickFlag = true;
    }
    event->ignore();
}

//鼠标移动事件
void VideoWidget::mouseMoveEvent(QMouseEvent *event)
{
    mouseClickFlag = false;
    event->ignore();
}

//鼠标双击事件
void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit switchFullScreen();
}

//鼠标释放事件
void VideoWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (mouseClickFlag)
        {
            emit clicked();
        }
    }
    event->ignore();
}

//键盘按下事件
void VideoWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Space:
        emit clicked();
        break;
    case Qt::Key_Return:
        emit switchFullScreen();
        break;
    case Qt::Key_Right:
//        emit goForward();
        emit playJump(forwardStep*1000);
        break;
    case Qt::Key_Left:
//        emit retreat();
        emit playJump(-retreatStep*1000);
        break;
    case Qt::Key_Up:
        emit volumnAdd();
        break;
    case Qt::Key_Down:
        emit volumnReduce();
        break;
    case Qt::Key_PageUp:

        break;
    case Qt::Key_PageDown:

        break;
    case Qt::Key_Escape:
        if (this->parentWidget()->isFullScreen())
        {
            this->parentWidget()->showNormal();
        }
        break;
    }
}
//获得视频分辨率
void VideoWidget::gotVideoSize(QSize size)
{
    videoWidth = size.width();
    videoHeight = size.height();
}
