#ifndef TITLEGROUPBOX_H
#define TITLEGROUPBOX_H

#include <QGroupBox>
class QLabel;
class QComboBox;
class QLineEdit;
class QToolButton;

class Setting;
class Theme;

class QHBoxLayout;
class QGridLayout;

class TitleGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit TitleGroupBox(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *, QEvent *);

private:
    QLabel *lab_logo;
    QLabel *lab_logoWords;
    QLabel *lab_bearHead;
    QLabel *lab_bearFoot;
//    QLabel *xiaowanzi;
//    QToolButton *tbn_globalMusic_icon;
//    QToolButton *tbn_globalMusic;
//    QToolButton *tbn_internet_Music_icon;
//    QToolButton *tbn_internet_Music;
    QToolButton *tbn_closeWindow;
    QToolButton *tbn_hideWindow;
    QToolButton *tbn_miniWindow;
    QToolButton *tbn_setting;
    QToolButton *tbn_theme;
    QToolButton *tbn_simple;
    QToolButton *tbn_normal;
    QToolButton *tbn_switch_music;
    QToolButton *tbn_switch_video;
//    QToolButton *tbn_switch_music_video;

    Setting *setting;
    Theme *theme;

    QTimer *timer;

    void setTitleUi();

    //布局
    QHBoxLayout *layout_H;
    QGridLayout *layout_top;
	
	//logo 旋转
//    QMatrix logoMatrix;
    QTimer *timer_logoRotate;
    QTimer *timer_logoRotatePause;
    int rotateAngle = 0;
//    const int angleOffset = 20;
//    const int timer_rotateStart = 30;
    const int angleOffset = 30;
//    const int timer_rotateStart = 60;
    const int timer_rotateStart = 60;
    const int timer_rotatePause = 2000;

protected slots:
    void settingClicked();
    void themeClicked();

signals:
    void clickGlobalMusic();
    void clickInternetMusic();
    void themeNumClick(int);
    void settingDataChanged(QString, QString);   //设置信息改变，写入配置文件
    void ShowVideoPlayer();

public slots:
    void showMiniWindow();
    void clickedGlobalMusic();
    void clickedInternetMusic();
    void ShowVideoPlayerSlot();
    void timeout();
    void timeoutBear();
	
	//logo 旋转
    void timeout_logoRotate();
    void timeout_logoRotetaRedo();
};

#endif // TITLEGROUPBOX_H
