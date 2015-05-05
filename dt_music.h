#ifndef DT_MUSIC_H
#define DT_MUSIC_H

#include <QMainWindow>

#include <QMediaPlayer>

#include <QSystemTrayIcon>

#include <QPoint>
#include <QCursor>
#include <QRect>
#include <QSqlDatabase>
#include <QString>
class TitleGroupBox;
class ToolGlobal;
//class ToolInternet;
class Timing;
class Game;
class QLabel;
class MusicList;
class LrcView;
class PlayModle;
class Create_MusicList;
class QComboBox;
class QLineEdit;
class QToolButton;
class BottomGroupBox;
class DesktopLrc;
class QTreeWidgetItem;
//class InternetWidget;
class QMediaPlayer;
class QMediaPlaylist;
class QSlider;
class QCompleter;
class VideoPlayer;                    //视频播放器
//class QGridLayout;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
#define PADDING 4

namespace Ui {
class DT_Music;
}

enum Direction{
    UP = 0,
    DOWN=1,
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

    void openTempMusicFile(QString file);
    void openTempVideoFile(QString file);

    //枚举播放状态
    enum playModelValue{
        play_random,            //随机播放
        play_once,              //单首播放
        play_single,            //单曲循环
        play_sequence,          //单次列表
        play_loop,              //列表循环
        play_custom             //自定义
    };
/*
    //枚举切换方式
    enum method{
        auto_next,              //自动切换
        click_next              //手动切换
    };
*/
//    QMediaPlayer *player;                   //定义 多媒体播放器
//    QMediaPlaylist *playlist;               //定义 播放列表
//    QList<QMediaPlaylist*> playlistVector;  //定义 播放列表 容器
    QCompleter *completer;                  //定义 自动补全功能
//    QList<QStringList> musicNameList;       //定义 全局播放列表

//    method thisMethod;
    playModelValue playModel_currentValue;

//    QStringList lrcStringList;              //歌词行

    ~DT_Music();

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
//    ToolInternet *toolInternet;
    PlayModle *playModle;
    DesktopLrc *desktopLrc;             //定义 桌面歌词
    Create_MusicList *createMusicList;
    Timing *timing;
    MusicList *musicList;               //定义 播放列表
    LrcView *lrcView;                   //定义 歌词界面
//    InternetWidget *internetWidget;
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

    //歌词
    QString previousLrcString;          //之前一行    //没用了 找到他解决掉
//    QString initLrc;                    //初始化5行歌词

//    int playModel_CurrentValue;         //定义 当前播放模式值

    QMap<qint64, QString> lrcMap;
    void resolveLrc(const QString &sourceFileName);     //解析歌词

    QTreeWidgetItem *createItem;
    QPoint offset;                      //定义窗口拖动相对坐标差

             //设置搜索时 自动补全
    QTimer *timer_closeGradually;

    //视频播放器
    VideoPlayer *videoPlayer;

    //布局管理器
//    QGridLayout *gridLayout;
    QHBoxLayout *layout_musicListHead;
    QGridLayout *layout_middleLeft;
    QHBoxLayout *layout_middle;
    QVBoxLayout *layout_top;

    QTimer *timerBear;
    //数据库
    QSqlDatabase db;
    //默认值
    int volumn_defaultValue;
    int theme_defultValue;
    int playmode_defaultValue;
    int oldVolumnValue;

signals:
    void gotMetaData(QStringList);
//    void searchedMusicIndex(int, int);
    void sendCurrentVolumeValue(int);

private slots:
    void addMusicFile(int selected);                        //添加音乐
    void tbn_search_clicked();                              //查找歌曲

    void playList_currentIndexChanged(int);                 //歌曲切换 处理
//    void clickedGlobalMusic();
//    void clickedInternetMusic();
    void durationChanged();
    void searchMusic();
    void volumeChanged(int);                                //声音改变
    void setTheme(int);
    void volumValue_changed();
    void sliderVolumn_Released();                           //释放 音量控件 隐藏 音量标签

    void updateLrc(qint64 time);
    void setLrcShown(bool);

    void seekableChanged(bool);                             //播放进度改变
    void metaDataAvailableChanged(bool);                    //多媒体 可用数据信息 改变
    void metaDataChanged();                                 //多媒体 数据信息     改变
    void metaDataChanged(QString,QVariant);                      //多媒体 数据        改变
    void mediaStatusChanged(QMediaPlayer::MediaStatus);          //多媒体 状态        改变2
    void mediaChanged(QMediaContent);                            //多媒体 改变
    void objectNameChanged(QString);                             //对象名改变
    void playbackRateChanged(qreal);                             //播放频率 改变

    void ShowVideoPlayer();
    void switchMusicPlayer();

//    void audioAvailableChanged(bool);                            //未知信号
//    void availabilityChanged(bool);                              //未知信号
//    void availabilityChanged(QMultimedia::AvailabilityStatus);   //未知信号
//    void bufferStatusChanged(int);                               //未知信号
//    void currentMediaChanged(QMediaContent);                     //未知信号
//    void destroyed();                                            //未知信号
//    void destroyed(QObject*);                                    //未知信号
//    void error(QMediaPlayer::Error);                             //未知信号
//    void mutedChanged(bool);                                     //未知信号
//    void notifyIntervalChanged(int);                             //未知信号
//    void videoAvailableChanged(bool);                            //video 有用信息改变

    void itemPlay();
    void play_stop();
    void lrc_clicked();
    void showVolumn(QPoint);
    void showPlayModle(QPoint);
    void playModelState_changed(playModelValue);
    void showWindow();
    void trayIcon_clicked(QSystemTrayIcon::ActivationReason);
    void maigcStateChanged(bool);
    void gameStateChange(bool);
    void create_MusicList();                                        //创建播放列表
    void create_musicList_ok(QString);
    void update_createMusiclist_name(QString);

    void writeSettingdateToIni(QString, QString);                   //设置改变，设置信息写入配置文件

    void timeoutBear();
};

#endif // DT_MUSIC_H
