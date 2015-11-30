#ifndef MUSICLIST_H
#define MUSICLIST_H

#include "playmodle.h"
//#include "subthread.h"
#include <xmlprocess.h>
#include <QTreeWidget>

//#include <QSqlDatabase>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTreeWidgetItem>
#include <QCompleter>
#include <QGridLayout>
#include <QVBoxLayout>
class QStringListModel;
class QAction;

class MusicList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MusicList(QString programPath, QWidget *parent = 0);

    friend class DT_Music;          // 设置友元类
    friend class BottomGroupBox;    // 设置友元类

    void create_musicList(QString);                     // 创建播放列表
    int get_current_rootDir();                          // 检测当前播放列表
    QStringList getToplevelNames();
    void openTempFile(QString file);
    void addMusicToList(int topLevelIndex, QStringList musicNames);
    void addMusicToList(QString topLevelName, QList<QMap<QString, QString> > musicUrlsAndNames);
    void setPlayMode(PlayModle::PlayMode playModeValue);

protected:
    void contextMenuEvent(QContextMenuEvent *event);    // 上下文菜单事件

private:
    void initMusicList();                               // 初始化播放列表
    void loadMusicList();                               // 加载歌曲列表
    void createMusiclistToplevel(QString toplevelName); // 创建播放列表
    void initDefaultMusicList();

//    QSqlDatabase db;
    const QString xmlPath;
    XMLProcess xml;
    QMediaPlayer *player;                               // 定义 多媒体播放器
    QList<QMediaPlaylist*> playlistVector;              // 定义 播放列表 容器

    //定义自动补全功能
    QCompleter *completer;                              // 定义 自动补全功能
    QStringListModel *stringListModel;
    QStringList completerList;

    //右键菜单事件
    QList<QAction*> musicMenuActionList;

    //定义子线程
//    SubThread subThread;

    int volumn;
    int toStopNum;                                      // 自定义播放模式（N首后停止）
    int selectedIndex[2];                               // 播放列表中被选中的歌曲
    PlayModle::PlayMode playMode;

    QStringList DefaultList;
    const QString musicListDatabaseName;
    const QString setupDatabaseName;

public slots:
    void setCurrentRow(int currentIndex, int topLevel=-1);
    void setCurrentRow(QString text);                       // 单击检索按钮，定位当前行

private slots:
    //数据库操作
//    void createDatebase(QStringList tableNames);                                                                                              //创建数据库
//    bool openDatebase(QString datebaseName, QString hostName="localhost", QString userName="", QString password="");    //打开数据库

    //
    void playMusic(int musicListIndex, int musicIndex);
    void doubleClicked(QModelIndex index);
    void rightBtnPlay();

//    void searchedMusic(int, int);                       // 搜索歌曲
    void removeSelection(bool delete_file=false);       // 把歌曲移除列表
    void deleteSelection();                             // 彻底删除
    void remove_rootDir();                              // 删除播放列表
    void renameToplevel(QTreeWidgetItem *item);         // 重命名
    void playInternetMusic(QString musicUrl, QString musicName);           // 试听在线音乐

    //清空列表
    void clearAll();    // 清空所有
    void clearSelf();   // 清空本列表

    //添加到列表
    void add_otherMusicList(QAction *);

    //调节音量
    void setVolumnPlus();
    void setVolumnLess();

    //释放子线程
//    void releaseThread();

    //test

};

#endif // MUSICLIST_H
