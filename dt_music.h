#ifndef DT_MUSIC_H
#define DT_MUSIC_H

#include "playmodle.h"
#include <QMainWindow>

#include <QMediaPlayer>

#include <QSystemTrayIcon>

#include <QPoint>
#include <QCursor>
#include <QRect>
#include <QSqlDatabase>
#include <QString>
#include "subthread.h"          //子线程
#include "network.h"
class TitleGroupBox;
class ToolGlobal;
class Timing;
class Game;
class QLabel;
class MusicList;
class LrcView;
class Create_MusicList;
class QComboBox;
class QLineEdit;
class QToolButton;
class BottomGroupBox;
class DesktopLrc;
class NetWorkWidget;
class QTreeWidgetItem;
class QMediaPlayer;
class QMediaPlaylist;
class QSlider;
class QCompleter;
class VideoPlayer;              //视频播放器
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
#define PADDING 4

namespace Ui {
class DT_Music;
}

enum Direction{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    LEFTTOP,
    LEFTBOTTOM,
    RIGHTBOTTOM,
    RIGHTTOP,
    NONE
};

class DT_Music : public QMainWindow
{
    Q_OBJECT

public:
    explicit DT_Music(QString programPath, QWidget *parent = 0);
    ~DT_Music();

    void openTempMusicFile(QString file);
    void openTempVideoFile(QString file);

    QCompleter *completer;                  //搜索时 自动补全

protected:
    void mousePressEvent(QMouseEvent *event);       //重写鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);        //重写鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);            //重写关闭事件
    bool eventFilter(QObject *, QEvent *);          //事件过滤器

private:
    Ui::DT_Music *ui;

    void init();                        //初始化 界面 和 变量
    void addMusic_and_searchMusic();    //创建 添加文件 和搜索框
    void volumn_widget();
    void create_trayIcon();
    void readDatabaseToSetup();
    void region(const QPoint &cursorGlobalPoint);

    //打开数据库
    const QString musicListDatabaseName;
    const QString setUpDatabaseName;
    bool openDatebase(QString datebaseName, QString sqlDriver="QSQLITE", QString hostName="localhost", QString userName="", QString password="");

    //程序路径
    QString programDir;
    bool isLeftPressDown;
    QPoint dragPosition;
    Direction direction;

    //子类部件
    TitleGroupBox *titleGroupBox;       //定义 头部
    ToolGlobal *toolGlobal;
    PlayModle *playModle;
    DesktopLrc *desktopLrc;             //定义 桌面歌词
    Create_MusicList *createMusicList;
    Timing *timing;
    MusicList *musicList;               //定义 播放列表
    LrcView *lrcView;                   //定义 歌词界面
    BottomGroupBox *bottomGroupbox;     //定义底部
    Game *game;
    QSystemTrayIcon *trayIcon;

    QLabel *lab_musicListTitle;
    QLabel *lab_volumnFrame;
    QLabel *lab_volumnValue;
    QSlider *slider_volumn;
    QComboBox *cob_addMusic;
    QToolButton *tbn_createList;
    QLineEdit *ln_search;
    QToolButton *tbn_search;
    QString createMusiclist_name;

    NetWorkWidget *netWorkWidget = NULL;

    //歌词
    void resolveLrc(const QString &sourceFileName);     //解析歌词
    QString previousLrcString;                          //之前一行    //没用了 找到他解决掉
//    QString initLrc;                                  //初始化5行歌词
    QMap<qint64, QString> lrcMap;

    QTreeWidgetItem *createItem;
    QPoint offset;                                      //定义窗口拖动相对坐标差

    QTimer *timer_closeGradually;

    //视频播放器
    VideoPlayer *videoPlayer;

    //布局管理器
    QHBoxLayout *layout_musicListHead;
    QGridLayout *layout_middleLeft;
    QHBoxLayout *layout_middle;
    QVBoxLayout *layout_top;

    QTimer *timerBear;

    //数据库
    QSqlDatabase db;

    NetWork *network;

    //子线程
    SubThread subThread;

    //默认值
    int volumn_defaultValue;
    int theme_defultValue;
    int playmode_defaultValue;
    int oldVolumnValue;

signals:
    void gotMetaData(QStringList);
    void sendCurrentVolumeValue(int);

private slots:
    void addMusicFile(int selected);                        //添加音乐
    void tbn_search_clicked();                              //查找歌曲
    void durationChanged();
    void searchMusic();
    void volumeChanged(int);                                //声音改变
    void setTheme(int);
    void volumValue_changed();
    void update_volumn_value_of_database();                 //更新数据库中音量值（供下次启动时加载上次音量，调节音量后调用此函数）

    void updateLrc(qint64 time);
    void setLrcShown(bool);

    void seekableChanged(bool);                             //播放进度改变
    void metaDataAvailableChanged(bool);                    //多媒体 可用数据信息 改变
    void metaDataChanged();                                 //多媒体 数据信息     改变
    void metaDataChanged(QString,QVariant);                 //多媒体 数据        改变
    void mediaStatusChanged(QMediaPlayer::MediaStatus);     //多媒体 状态        改变2
    void mediaChanged(QMediaContent);                       //多媒体 改变
    void objectNameChanged(QString);                        //对象名改变
    void playbackRateChanged(qreal);                        //播放频率 改变

    void ShowVideoPlayer();
    void switchMusicPlayer();

    void play_stop();
    void lrc_clicked();
    void showVolumn(QPoint);
    void showPlayModle(QPoint);
    void showWindow();
    void trayIcon_clicked(QSystemTrayIcon::ActivationReason);
    void maigcStateChanged(bool);
    void gameStateChange(bool);
    void create_MusicList();                                        //创建播放列表
    void create_musicList_ok(QString);
    void update_createMusiclist_name(QString);
    void timeoutBear();

    void searchMusicClicked(QString musicName);                                      // 搜索在线音乐
};

#endif // DT_MUSIC_H
