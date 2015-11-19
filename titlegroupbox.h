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
    QLabel *lab_logo;				//定义 Logo 图标
    QLabel *lab_logoWords;			//定义 Logo 文字
    QLabel *lab_bearHead;			//定义 小熊头 图标
    QLabel *lab_bearFoot;			//定义 小熊脚 图标
    QToolButton *tbn_closeWindow;	//定义 退出 按钮
    QToolButton *tbn_hideWindow;	//定义 关闭 按钮
    QToolButton *tbn_miniWindow;	//定义 最小化 按钮
    QToolButton *tbn_setting;		//定义 设置 按钮
    QToolButton *tbn_theme;         //定义 主题 按钮
    QToolButton *tbn_simple;		//定义 MiNi窗口 按钮
//    QToolButton *tbn_normal;		//定义 正常窗口 按钮
    QToolButton *tbn_switch_music;	//定义 切换音乐模式 按钮
    QToolButton *tbn_switch_video;	//定义 切换视频模式 按钮
    QLabel *lab_searchFrame;
    QLabel *lab_searchBg;
    QLineEdit *ln_search;           //定义 搜索框
    QToolButton *tbn_search;        //定义 搜索按钮

    Setting *setting;
    Theme *theme;

    QTimer *timer;

    void setTitleUi();

    //布局
    QHBoxLayout *layout_H;
    QHBoxLayout *layout_search;
    QGridLayout *layout_searchFrame;
    QGridLayout *layout_top;
	
	//logo 旋转
//    QMatrix logoMatrix;
    QTimer *timer_logoRotate;
    QTimer *timer_logoRotatePause;
    int rotateAngle;
    const int angleOffset;
    const int timer_rotateStart;
    const int timer_rotatePause;

private slots:
    void settingClicked();
    void themeClicked();
    void searchMusic();             // 搜索在线音乐

signals:
    void clickGlobalMusic();
    void clickInternetMusic();
    void themeNumClick(int);
    void settingDataChanged(QString, QString);   //设置信息改变，写入配置文件
    void ShowVideoPlayer();
    void musicTitleAndAuthor(QString);
    void calldMusicTitleAndAuthor();
    void callPlayPause();
    void callPlayPrevious();
    void callPlayNext();
    void callVolumnPlus();
    void callVolumnLess();
    void searchMusicClicked(QString);           //搜索在线音乐

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
