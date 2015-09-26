#include "musiclist.h"
#include "subthread.h"
#include "databaseoperation.h"

#include <QScrollBar>
#include <QFrame>
#include <QMenu>
#include <QContextMenuEvent>
#include <QModelIndex>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileInfo>

MusicList::MusicList(QString programPath, QWidget *parent) :
    QTreeWidget(parent)
//  ,programDir(programPath)
//  ,volumn(40)
  ,toStopNum(-1)
  ,selectedIndex(-1)
  ,musicListDatabaseName(programPath + "musicList.db")
{
    this->setHeaderHidden(true);            //隐藏表头
    this->setFrameStyle(QFrame::NoFrame);   //设置无边框线
    this->setMouseTracking(true);           //设置鼠标跟踪

    QFont font;
    font.setPointSize(10);
    this->setFont(font);
    this->setHeaderLabel(tr("播放列表"));

    //初始化 多媒体播放器
    player = new QMediaPlayer(this);		//创建 多媒体播放器
    playlist = new QMediaPlaylist(this);	//创建 多媒体播放列表
    playlistVector.append(playlist);		//把创建的播放列表添加到 playlistVector 容器中（每个列表一个playlist）

    initMusicList();                        //初始化一个空的播放列表
    if (QFileInfo(musicListDatabaseName).exists()) //检测数据库是否存在
    {
        loadMusicList();                    //如果数据库存在，加载歌曲列表
    }
    else
    {
        createDatebase(musicListDatabaseName, "默认列表");                   //如果数据库不存在，创建数据库
    }
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(playMusic()));    //双击 播放歌曲
    connect(this, SIGNAL(itemPlay()), this, SLOT(playMusic()));

    //遍历播放列表， 为每个播放列表关联信号槽
    for (int i=0; i<playlistVector.length(); i++)
    {
        connect(playlistVector.at(i), SIGNAL(currentIndexChanged(int)),     //歌曲切换时，设置播放列表中当前歌曲被选中
                this, SLOT(setCurrentRow(int)));
    }

    //设置样式
    this ->setObjectName(tr("musicList"));
    setStyleSheet(
                "#musicList{background-color: rgba(0, 0, 0, 30);}"
                "#musicList::item{height: 33px;}"
                );

    this ->verticalScrollBar()->setStyleSheet(
                "QScrollBar{"                                           //自定义 播放进度条
                    "background-color: rgba(63, 63, 63, 125);"
                    "width: 10px;}"
                    "QScrollBar::handle{"
                    "background-color: rgba(203, 203, 203, 40);"
                    "border: 2px solid transparent;"
                    "border-radius: 5px;"
                    "margin: 10 0;"
                    "float: left;"
                "}"
                "QScrollBar::handle:hover{"                             //滚动条伪状态
                    "background: rgba(203, 203, 203, 95);"
                    "margin: 10 0;"
                "}"
                "QScrollBar::sub-line{"                                 //已
                    "background: transparent;"
                "}"
                "QScrollBar::add-line{"                                 //未
                    "background: transparent;"
                "}"
                "QScrollBar::up-arrow:vertical{"                        //上箭头
                    "border-image: url(:/Images/progressTop.png);"
                "}"
                "QScrollBar::down-arrow:vertical{"                      //下箭头
                    "background-image: url(:/Images/progressBottom.png);"
                "}"
                "QScrollBar::sub-page{"                                 //已
                    "background: transparent;"
                "}"
                "QScrollBar::add-page{"                                 //未
                    "background: transparent;"
                "}"
                );
}

//新建 播放列表
void MusicList::create_musicList(QString listName)
{
    if (listName.isEmpty())
    {
        return;
    }
    //创建播放列表
    createItem = new QTreeWidgetItem(QStringList(QString(listName)));
    this ->addTopLevelItem(createItem);
    this ->setCurrentItem(createItem);

//    musicListName.append(listName);

    //添加到数据库
    if (!openDatebase(musicListDatabaseName))
    {
        QMessageBox::warning(0, tr("意外错误！"), tr("数据库打开失败！\n此列表将成为临时列表！"));
        return;
    }
    QSqlQuery query(db);
    if (!query.exec(tr("create table %1(id integer primary key, %2 text)").arg(listName).arg("musicName")))
    {
        QMessageBox::warning(0, tr("意外错误！"), tr("数据库更新失败！\n此列表将成为临时列表！"));
        db.close();
        return;
    }
    db.close();
}

//检测当前播放列表
int MusicList::get_current_rootDir()
{
    QTreeWidgetItem *item = this ->currentItem() ->parent();    //当前行所在 父节点的指针
    int forCount = this->topLevelItemCount();                   //总节点数
    for (int i=0; i<forCount; i++)
    {
        if (this->topLevelItem(i) == item)                   //i 为当前项所在父节点在总节点中的索引
        {
            return i;
        }
    }
    return -1;
}

//打开播放临时文件
void MusicList::openTempFile(QString file)
{
    createItem = new QTreeWidgetItem(QStringList(QString("临时列表")));
    this->addTopLevelItem(createItem);
    playlist = new QMediaPlaylist(this);
    playlistVector.append(playlist);
    this->rootDirVector.append(createItem);

    createItem = new QTreeWidgetItem(QStringList(QFileInfo(file).fileName()));
    this->topLevelItem(this->topLevelItemCount()-1) ->addChild(createItem);
    playlist ->addMedia(QUrl::fromLocalFile(file));                              ///////////////////////或者这个 musicNameList.at(musicNameList.length - 1).append(read_musicList);
//    completerList.append(QFileInfo(query_musicName.value("musicName").toString()).fileName());
    this->topLevelItem(0)->setExpanded(false);
    this->topLevelItem(this->topLevelItemCount()-1)->setExpanded(true);
    this->setCurrentItem(createItem);
    itemPlay();
    //    player->setPlaylist(playlist);
    //    player->play();
}

//
void MusicList::setCurrentRow(int currentIndex, int topLevel)
{
    //调用函数， 获得当前播放的列表
    if (topLevel == -1)
    {
        topLevel = get_current_rootDir();
    }

    //如果当前索引有效
    if ((currentIndex <= (this ->topLevelItem(topLevel) ->childCount() - 1)) && currentIndex > -1)
    {
        if (selectedIndex != -1)
        {
            this ->topLevelItem(topLevel) ->child(selectedIndex) ->setSelected(false);
        }
        this ->topLevelItem(topLevel) ->child(currentIndex) ->setSelected(true);
        selectedIndex = currentIndex;
    }
}

void MusicList::setCurrentRow(QString text)
{
    if (text.isEmpty())
    {
        return;
    }

    for (int topLevel=0; topLevel<this->topLevelItemCount(); ++topLevel)
    {
        for (int item=0; item<this->topLevelItem(topLevel)->childCount(); ++item)
        {
            if (text == this->topLevelItem(topLevel)->child(item)->text(0))
            {
                this->topLevelItem(topLevel)->child(item)->setSelected(true);
                return;
            }
        }
    }
}

//自定义 右键菜单
void MusicList::contextMenuEvent(QContextMenuEvent *event)
{
    //播放列表右键菜单
    for (int i=0; i<this->topLevelItemCount(); i++)
    {
        if (this->currentItem() == this->topLevelItem(i))
        {
            //创建节点菜单
            QMenu menu_rootDir;
            menu_rootDir.addAction(tr("删除"), this, SLOT(remove_rootDir()));
            menu_rootDir.addAction(tr("重命名"), this, SLOT(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)));
            menu_rootDir.addAction(tr("清空所有列表"), this, SLOT(clearAll()));
            menu_rootDir.exec(event ->globalPos());
            return;
        }
    }

    //音乐右键菜单
    QStringList otherToplevel;
    QString activedToplevel = this->topLevelItem(get_current_rootDir())->text(0);
    QString toplevelText;
    for(int i=0; i<this->topLevelItemCount(); i++)
    {
        toplevelText = this->topLevelItem(i)->text(0);
        if (toplevelText == activedToplevel)
        {
            continue;
        }
        otherToplevel.append(toplevelText);
    }
    //创建二级菜单
    QMenu menu_child;
    menu_child.setIcon(QIcon(":/Images/add.png"));
    menu_child.setTitle(tr("添加到..."));
    for (int i=0; i<otherToplevel.count(); i++)
    {
        menu_child.addAction(otherToplevel.at(i), this, SLOT(add_otherMusicList()));
    }
    //创建项目菜单
    QMenu menu_item;
    menu_item.addAction(QIcon(":/Images/play_play_hover.png"), tr("立即播放"), this, SIGNAL(itemPlay()));
    menu_item.addMenu(&menu_child);
    menu_item.addAction(QIcon(":/Images/delete2.png"), tr("删除"), this, SLOT(removeSelection()));
    menu_item.addAction(tr("清空本列表"), this, SLOT(clearSelf()));
    menu_item.exec(event ->globalPos());
    return;
}

//初始化播放列表（创建[默认列表]）
void MusicList::initMusicList()
{
    QStringList itemsRootNames;
//    itemsRootNames << "默认列表" << "我的最爱";
    itemsRootNames << "默认列表";
    for (int i=0; i<itemsRootNames.length(); i++)
    {
        createItem = new QTreeWidgetItem(QStringList(QString(itemsRootNames[i])));
        this->addTopLevelItem(createItem);
        rootDirVector.append(createItem);
    }
    this->topLevelItem(0) ->setExpanded(true);          //展开默认列表

    //初始化成员函数 和 初始状态
//    currentPlayingIndex[0] = -1;   //当前播放 节点索引
//    currentPlayingIndex[1] = -1;   //当前播放 列表索引
}

//从数据库加载用户歌曲到播放器
void MusicList::loadMusicList()
{
    //定义自动补全功能
    QStringList completerList;

    //打开 数据库 加载音乐列表
    if (!openDatebase(musicListDatabaseName))
    {
        QMessageBox::warning(0, "错误！", "数据库读取失败！", QMessageBox::Ok);
        return;
    }
    QSqlQuery query_musicListName(db);                        //查询表名（即播放列表名）
    QSqlQuery query_musicName(db);                            //查询音乐名（即每音乐的路径）
    if (!query_musicListName.exec("SELECT NAME FROM sqlite_master"))    //如果数据库中没有表，则返回
    {
        return;
    }
    while(query_musicListName.next())
    {
        //查询播放列表名称
        if (query_musicListName.value("name").toString() != "默认列表")      //如果不是默认播放列表，就创建这个播放列表
        {
            //创建用户自定义的播放列表
            createItem = new QTreeWidgetItem(QStringList(QString(query_musicListName.value("name").toString())));
            this->addTopLevelItem(createItem);
            playlist = new QMediaPlaylist(this);
            playlistVector.append(playlist);
            this->rootDirVector.append(createItem);
//            this->musicListName.append(query_musicListName.value(0).toString());
        }
        //查询歌曲路径
        if(!query_musicName.exec(tr("SELECT * FROM %1").arg(query_musicListName.value("name").toString())))
        {
            continue;                   //如果列表为空，加载下一个列表
        }
        while(query_musicName.next())   //加载音乐名到播放列表中
        {
            createItem = new QTreeWidgetItem(QStringList(QFileInfo(query_musicName.value("musicName").toString()).fileName()));
            this->topLevelItem(this->topLevelItemCount()-1) ->addChild(createItem);
            playlist ->addMedia(QUrl::fromLocalFile(query_musicName.value("musicName").toString()));                              ///////////////////////或者这个 musicNameList.at(musicNameList.length - 1).append(read_musicList);
            completerList.append(QFileInfo(query_musicName.value("musicName").toString()).fileName());	//添加到自动补全列表
        }
    }
    //关闭数据库
    db.close();

    //创建 搜索 自动补全
    completer = new QCompleter(completerList);
    completer ->setCaseSensitivity(Qt::CaseInsensitive);    //设置大小写不敏感
}

//创建数据库
void MusicList::createDatebase(QString databaseName, QString tableName)
{
    if (!openDatebase(databaseName))
    {
        QMessageBox::warning(0, "错误！", "数据创建失败！", QMessageBox::Ok);
        return;
    }
    QSqlQuery query(db);
    if (!query.exec(tr("CREATE TABLE %1(id integer primary key, musicName text)").arg(tableName)))
    {
        QMessageBox::warning(0, "错误！", "数据建表失败！", QMessageBox::Ok);
        db.close();
        return;
    }
    db.close();
}

//打开数据库
bool MusicList::openDatebase(QString datebaseName, QString hostName, QString userName, QString password)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(datebaseName);
    db.setHostName(hostName);
    db.setUserName(userName);
    db.setPassword(password);

    if (db.open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

//数据库插入数据
bool MusicList::insertDatebase(QString datebaseName, QString tableName, QString columnName, QStringList content, int indext)
{
    if (!openDatebase(datebaseName))
    {
        return false;
    }

    QSqlQuery query(db);
    for(int i=0; i<content.length(); i++)
    {
        if (!query.exec(tr("INSERT INTO %1(%2) VALUES('%3')").arg(tableName).arg(columnName).arg(content.at(i))))
        {
            return false;
        }
    }
    return true;
}

//数据库修改数据
bool MusicList::alterDatebase(QString datebaseName, QString tableName, QString oldDate, QString newDate)
{

}

//数据库删除数据
bool MusicList::deleteDatebase(QString datebaseName, QString tableName, int id_deleteDate)
{
    if (!openDatebase(datebaseName))
    {
        return false;
    }
    QSqlQuery query(db);
    if (!query.exec(tr("delete from %1 where id=%2").arg(tableName).arg(id_deleteDate)))
    {
        db.close();
        return false;
    }
    if (!query.exec(tr("update %1 set id=id-1 where id>%2").arg(tableName).arg(id_deleteDate)))
    {
        QMessageBox::warning(0, tr("错误！"), tr("数据库id排序失败！"), QMessageBox::Ok);
        db.close();
        return false;
    }
    db.close();
    return true;
}

//播放歌曲
void MusicList::playMusic()
{
//    thisMethod = click_next;                                    //设置为发行为
    int currentIndex = this->currentIndex().row();              //取得 当前行 索引值
    int rootDir = get_current_rootDir();                        //检测当前活动项目所在的播放列表
    if (rootDir > playlistVector.length()-1)                  //如果单击的是播放列表而不是歌曲，则不处理（默认展开列表）
    {
        return;
    }
    player ->setPlaylist(playlistVector.at(rootDir));
    playlistVector.at(rootDir) ->setCurrentIndex(currentIndex);
//    player->stop();
//    player->setVolume(volumn);
    player ->play();
}

//歌曲切换 设置当前歌曲被选中
void MusicList::playList_currentIndexChanged(int currentIndex)
{
    //调用函数， 获得当前播放的列表
    int rootDir = get_current_rootDir();

    //如果当前索引有效
    if ((currentIndex <= (this ->topLevelItem(rootDir) ->childCount() - 1)) && currentIndex > -1)
    {
        if (selectedIndex != -1)
        {
            this ->topLevelItem(rootDir) ->child(selectedIndex) ->setSelected(false);
        }
        this ->topLevelItem(rootDir) ->child(currentIndex) ->setSelected(true);
        selectedIndex = currentIndex;
    }
}
/*
//搜索到歌曲后 处理
void MusicList::searchedMusic(int rootDir, int index)
{
    setCurrentItem(topLevelItem(rootDir) ->child(index));
}
*/
//移除歌曲
void MusicList::removeSelection()
{
    //获得播放列表和索引值
    int rootDir = get_current_rootDir();
    int currentRow = this->currentIndex().row();
//    QString deleteData = playlistVector[rootDir]->media(currentRow).canonicalUrl().toLocalFile();

    //删除界面中的选中歌曲
    this->topLevelItem(rootDir)->removeChild(this->currentItem());

    //移除播放列表歌曲
    playlistVector[rootDir]->removeMedia(currentRow);

    //更新数据库
    if (!deleteDatebase(musicListDatabaseName, this->topLevelItem(0)->text(0), currentRow+1))
    {
        QMessageBox::warning(0, tr("发生意外！"), tr("配置文件更新失败！"), QMessageBox::Ok);
    }
}

//删除播放列表
void MusicList::remove_rootDir()
{
    //删除此列表中的歌曲
    while (this->currentItem()->childCount())
    {
        this->currentItem()->removeChild(this->currentItem()->child(0));
    }

    //最后删除播放列表
    this->currentItem()->setHidden(true);

    //更新数据库

    if (!openDatebase(musicListDatabaseName))
    {
        QMessageBox::warning(0, tr("严重错误！"), tr("配置文件保存失败！"), QMessageBox::Ok);
        return;
    }
    QSqlQuery query(db);
    if (!query.exec(tr("DROP TABLE %1").arg(this->currentItem()->text(0))))
    {
        QMessageBox::warning(0, tr("严重错误！"), tr("配置文件保存失败！"), QMessageBox::Ok);
        db.close();
        return;
    }
    db.close();
}

//重命名
void MusicList::closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)
{
    this ->editItem(createItem);
}

//清空所有列表
void MusicList::clearAll()
{
    //清空所有列表
    this ->clear();

    //重新初始化播放列表
    initMusicList();

    QFile clearList(".mData.ini");
    if (clearList.open(QIODevice::WriteOnly))
    {
        clearList.remove();
    }
}

//清空本列表
void MusicList::clearSelf()
{
    //获得当前选中列表和列表歌曲数
    int rootDir = get_current_rootDir();

    //遍历删除每一个子项
    while (this->topLevelItem(rootDir)->childCount())
    {
        this->topLevelItem(rootDir)->removeChild(this->topLevelItem(rootDir)->child(0));
    }

    //移除播放列表歌曲
    playlistVector.at(rootDir)->clear();

    //更新数据库
    QString databaseName = this->topLevelItem(rootDir)->text(0);
    DatabaseOperation db_delete_all(musicListDatabaseName);
    db_delete_all.deleteAll(databaseName);
}

//添加到列表
void MusicList::add_otherMusicList()
{
    qDebug() << "已添加到";
}

//释放子线程
void MusicList::releaseThread()
{
    subThread->deleteLater();
}
