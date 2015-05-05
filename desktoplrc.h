#ifndef DESKTOPLRC_H
#define DESKTOPLRC_H

#include <QLabel>
class QTimer;

class DesktopLrc : public QLabel
{
    Q_OBJECT
public:
    explicit DesktopLrc(QWidget *parent = 0);

    void startLrcMask(qint64 intervalTime);
    void stopLrcMask();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void timeout();

private:
    QLinearGradient linearGradient;
    QLinearGradient maskLinearGradient;
    QFont font;
    QTimer *timer;
    qreal lrcMaskWidth;
    // 每次歌词遮罩增加的宽度
    qreal lrcMaskWidthInterval;
    QPoint offset;

signals:

public slots:

};

#endif // DESKTOPLRC_H
