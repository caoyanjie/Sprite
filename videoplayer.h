#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "videowidget.h"
#include "videocontral.h"
#include "helpkeepawake.h"
#include <QWidget>
#include <QGridLayout>
#include <QMediaPlayer>
#include <QMediaPlaylist>
class QToolButton;
class VideoList;

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    friend class VideoContral;
    explicit VideoPlayer(QString programPath, QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);           //鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);            //鼠标移动事件
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void buttonsVisible(bool);

    QString programDir;
    QLabel *lab_background;
    VideoWidget *videoWidget;
    VideoContral *videoContral;
    HelpKeepAwake *helpKeepAwake;
    QMediaPlayer *player;
    QList<QMediaPlaylist*> playlist_list;

    VideoList *videoList;
    QToolButton *tbn_openVideoFile;

    QGridLayout *layout_top;

    QTimer *timerHideWidgets;
    QTimer *timerKeepAwake;

    QPoint offset;
    bool hideCursor;
    int keepAwakeTimes;

    const int WIDTH;
    const int HEIGHT;
    const int videoNameWidth;
    const int videoListWidth;
    const int hideButtonsTime;
    const int forwardStep;
    const int retreatStep;
    const int volumnChangeStep;

signals:
    void switchMusicPlayer();
    void moveWindow(QPoint);

public slots:
    void openVideoFile(QString file="");

private slots:
    void hideWidgets_timeout();
    void isHideWidget(bool);
    void metaDataChanged();
    void setVideoTitle();
    void valueChanged(int currentPosition);
    void switchFullScreen();
    void videoPlayPause();
    void currentMediaEnd();
    void videoPlayJump(int msecond);
//    void videoGoForward(int);
//    void videoRetreat();
    void volumnAdd();
    void volumnReduce();
    void playStateChanged(QMediaPlayer::State);
    void timeoutKeepAwake();

    //双击播放列表
    void itemDoubleClicked(int toplevel, int index);
};

#endif // VIDEOPLAYER_H
