#ifndef CURRENTLRC_H
#define CURRENTLRC_H

#include <QLabel>
class QTimer;

class CurrentLrc : public QLabel
{
    Q_OBJECT
public:
    explicit CurrentLrc(QWidget *parent = 0);

    void startLrcMask(qint64 intervalTime);
    void stopLrcMask();

protected:
    void paintEvent(QPaintEvent *);

private:
    QLinearGradient linearGradient;
    QLinearGradient maskLinearGradient;
    QFont font;
    QTimer *timer;
    qreal lrcMaskWidth;

    // 每次歌词遮罩增加的宽度
    qreal lrcMaskWidthInterval;

private slots:
    void timeout();

signals:

public slots:

};

#endif // CURRENTLRC_H
