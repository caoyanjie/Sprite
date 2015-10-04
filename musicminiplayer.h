#ifndef MUSICMINIPLAYER_H
#define MUSICMINIPLAYER_H

#include <QWidget>
class QToolButton;
class QLabel;
class QHBoxLayout;
class QPropertyAnimation;
class QParallelAnimationGroup;
class QHBoxLayout;
class QGridLayout;
class QVBoxLayout;

class MusicMiniPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit MusicMiniPlayer(QWidget *parent = 0);

private:
    QLabel *lab_bg;
    QLabel *lab_logo;
    QLabel *lab_musicMsg;
    QLabel *lab_buttonsParent;
    QToolButton *tbn_play_previous;
    QToolButton *tbn_play;
    QToolButton *tbn_play_next;
    QToolButton *tbn_volumn_plus;
    QToolButton *tbn_volumn_less;
    QToolButton *tbn_closeWindow;
    QToolButton *tbn_hideWindow;
    QToolButton *tbn_miniWindow;
//    QToolButton *tbn_simple;
    QToolButton *tbn_normal;
    QHBoxLayout *layout_topright;
    QGridLayout *layout_middle_left;
    QHBoxLayout *layout_middle;
    QVBoxLayout *layout_right;
//    QVBoxLayout *layout_top;
    QHBoxLayout *layout_top;

    QPropertyAnimation *animation_previous;
    QPropertyAnimation *animation_play;
    QPropertyAnimation *animation_next;
    QParallelAnimationGroup *animationG;

    QPoint offest;

    //logo 旋转
    QTimer *timer_logoRotate;
    int rotateAngle = 0;
    const int angleOffset = 10;
    const int timer_rotateStart = 100;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *, QEvent *);
    void paintEvent(QPaintEvent *);

private:
    void paintOnWidget();

signals:

public slots:
    void showNormalWindow();
    //logo 旋转
    void timeout_logoRotate();
    void setMusicMessage(QString musicMsg);
};

#endif // MUSICMINIPLAYER_H
