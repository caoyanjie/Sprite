#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>

class VideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
//    void keyReleaseEvent(QKeyEvent *event);

private:
    int videoWidth;
    int videoHeight;
    bool mouseClickFlag;
    const int forwardStep;
    const int retreatStep;

signals:
    void clicked();
//    void goForward();
//    void retreat();
    void playJump(int);
    void switchFullScreen();
    void volumnAdd();
    void volumnReduce();

public slots:
    void gotVideoSize(QSize);
};

#endif // VIDEOWIDGET_H
