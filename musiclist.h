#ifndef MUSICLIST_H
#define MUSICLIST_H

#include "playmodle.h"
#include "subthread.h"
#include <QTreeWidget>

#include <QSqlDatabase>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTreeWidgetItem>
#include <QCompleter>
#include <QGridLayout>
#include <QVBoxLayout>
//class SubThread;
class QStringListModel;
class QAction;

class MusicList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MusicList(QString programPath, QWidget *parent = 0);
/*
    //枚举播放状态
    enum PlayMode{
        PlayRandom,            //随机播放
        PlayOnce,              //单首播放
        PlaySingle,            //单曲循环
        PlaySequence,          //单次列表
        PlayLoop,              //泪飙循环
        PlayCustom             //自定义
    };
*/
    //枚举切换方式
    enum method{
        auto_next,              //自动切换
        click_next              //手动切换
    };

    friend class DT_Music;          //设置友元类
    friend class BottomGroupBox;    //设置友元类
//    friend class ToolGlobal;        //设置友元类

    void create_musicList(QString);                     //创建播放列表
    int get_current_rootDir();                          //检测当前播放列表
    void openTempFile(QString file);
    void addMusicToList(int topLevelIndex, QStringList musicNames);
    void setPlayMode(PlayModle::PlayMode playModeValue);

    QList<QTreeWidgetItem*> rootDirVector;              //定义 播放列表 容器                    ########有什么用#########
    int currentPlayingIndex[2];                         //标记 当前播放二维索引（播放列表， 播放索引）############ 尝试去掉 ##############
    QStringList musicListName;                          //保存播放列表名字

protected:
    void contextMenuEvent(QContextMenuEvent *event);    //上下文菜单事件

private:
    void initMusicList();                               //初始化播放列表
    void loadMusicList();                               //加载歌曲列表
    void createMusiclistToplevel(QString toplevelName);         //创建播放列表

//    QString programDir;
    QTreeWidgetItem *createItem;                        //
    QSqlDatabase db;
    QMediaPlayer *player;                           //定义 多媒体播放器
    QMediaPlaylist *playlist;                       //定义 多媒体播放列表
    QList<QMediaPlaylist*> playlistVector;          //定义 播放列表 容器

    //定义自动补全功能
    QCompleter *completer;                          //定义 自动补全功能
    QStringListModel *stringListModel;
    QStringList completerList;

    //右键菜单事件
    QList<QAction*> musicMenuActionList;
    QAction *musicMenuAction;

//    SubThread *subThread;
    SubThread subThread;
    int volumn;
    int toStopNum;                                  //自定义播放模式（N首后停止）
    int selectedIndex;                              //播放列表中被选中的歌曲

//    PlayMode playModel_currentValue;
    method thisMethod;
    const QString musicListDatabaseName;
    const QString setupDatabaseName;

signals:
    void itemPlay();                                    //播放歌曲

public slots:
    void setCurrentRow(int currentIndex=-1, int topLevel=-1);
    void setCurrentRow(QString text);

private slots:
    //数据库操作
    void createDatebase(QString databaseName, QString tableName);                                                                                              //创建数据库
    bool openDatebase(QString datebaseName, QString hostName="localhost", QString userName="", QString password="");    //打开数据库
    bool insertDatebase(QString datebaseName, QString tableName, QString columnName, QStringList content, int indext=-1);                   //数据库插入数据
    bool deleteDatebase(QString datebaseName, QString tableName, int id_deleteDate);                                   //数据库删除数据

    //
    void playMusic();
    void itemDoubleClicked(QTreeWidgetItem *item, int index);

    void playList_currentIndexChanged(int);                         //歌曲切换 处理
//    void searchedMusic(int, int);                                   //搜索歌曲
    void removeSelection(bool delete_file=false);                   //把歌曲移除列表
    void deleteSelection();                                         //彻底删除
    void remove_rootDir();                                          //删除播放列表
    void closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint);  //重写关闭事件

    //清空列表
    void clearAll();    //清空所有
    void clearSelf();   //清空本列表

    //添加到列表
    void add_otherMusicList(QAction *);

    //释放子线程
    void releaseThread();

    //定义子线程
//    SubThread subThread;
};

#endif // MUSICLIST_H
