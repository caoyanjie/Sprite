#ifndef LRCVIEW_H
#define LRCVIEW_H

#include <QLabel>
class QTimer;
class CurrentLrc;
//class QLinearGradient;

class LrcView : public QLabel
{
    Q_OBJECT
public:
    explicit LrcView(QWidget *parent = 0);

    void setLrcTextView(bool show);
    QLabel *textNext;                           //后续歌词
    CurrentLrc *currentLrc;                     //当前播放歌词
    QLabel *textPrevious;                       //播放过的歌词

    QStringList previousLines;
    QString previousLrc;

//    void startLrcMask(qint64 intervalTime);
//    void stopLrcMask();
//    QString currentLrc;

//protected:
//    void paintEvent(QPaintEvent *);

private:
    //动画
    QLabel *lab_left_top;
    QLabel *lab_left_bottom;
    QTimer *timer;

    //更新 Gif 图
    int gifNum = 1;             //待初始化

//    QLinearGradient linearGradient;
//    QLinearGradient maskLinearGradient;
//    QFont font;
//    qreal lrcMaskWidth;
//    QLabel *lab_temp;
//    qreal lrcMaskWidthInterval;
//    QPoint offset;

signals:

public slots:

private slots:
    //更新 Gif 动画
    void updateGif();

//    void timeout();
};

#endif // LRCVIEW_H
