#include "musiclist.h"
//#include "subthread.h"
//#include "databaseoperation.h"
#include "xmlprocess.h"

#include <QScrollBar>
#include <QFrame>
#include <QMenu>
#include <QContextMenuEvent>
#include <QModelIndex>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileInfo>
#include <QStringListModel>
#include <QTimer>

MusicList::MusicList(QString programPath, QWidget *parent) :
    QTreeWidget(parent)
  ,xmlPath(programPath + "ini.xml")
  ,xml(xmlPath)
  ,toStopNum(-1)
  ,selectedIndex({-1, -1})
//  ,programDir(programPath)
//  ,volumn(40)
//  ,musicListDatabaseName(programPath + "musicList.db")
//  ,setupDatabaseName(programPath + "setUp.db")
{
    this->setHeaderHidden(true);            //隐藏表头
    this->setFrameStyle(QFrame::NoFrame);   //设置无边框线
    this->setMouseTracking(true);           //设置鼠标跟踪

    QFont font;
    font.setPointSize(10);
    this->setFont(font);
    this->setHeaderLabel(tr("播放列表"));
    DefaultList << "默认列表" << "在线试听";

    //初始化 多媒体播放器
    player = new QMediaPlayer(this);		//创建 多媒体播放器

    //创建 搜索 自动补全
    completer = new QCompleter(this);
    stringListModel = new QStringListModel();
    completer->setModel(stringListModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);     //设置大小写不敏感

    initMusicList();                                        //初始化默认播放列表
//    if (QFileInfo(musicListDatabaseName).exists())          //检测数据库是否存在
//    {
//        loadMusicList();                                    //如果数据库存在，加载歌曲列表
//    }
//    else
//    {
//        createDatebase(DefaultList);                        //如果数据库不存在，创建数据库
//    }
    if (QFileInfo(xmlPath).exists())
    {
        if (xml.isElementExist(xml.MusicListElement))
        {
            loadMusicList();
        }
        else
        {
            initDefaultMusicList();
        }
    }
    else
    {
        QMessageBox::warning(0, "Error!", "配置文件找不到，用户数据不能加载！");
    }

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));  //双击 播放歌曲

    //遍历播放列表， 为每个播放列表关联信号槽
    for (int i=0; i<playlistVector.length(); i++)
    {
        connect(playlistVector.at(i), SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentRow(int)));         //歌曲切换时，设置播放列表中当前歌曲被选中
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
    createMusiclistToplevel(listName);
    this->setCurrentItem(this->topLevelItem(this->topLevelItemCount()-1));
    setPlayMode(playMode);

    //添加到数据库
//    DatabaseOperation db(musicListDatabaseName);
//    db.createTable(listName, tr("id integer primary key, musicName text"));
    // 添加到 xml 配置文件
    xml.addElement(xml.FirstChildElement, xml.MusicListElement, xml.MusicListElementKey, listName);
}

//检测当前播放列表(有孩子的情况下，判断孩子所在的toplevel，如果没子孩子，返回-1）
int MusicList::get_current_rootDir()
{
    QTreeWidgetItem *item = this->currentItem()->parent();    //当前行所在 父节点的指针
    for (int i=0; i<this->topLevelItemCount(); i++)
    {
        if (this->topLevelItem(i) == item)                   //i 为当前项所在父节点在总节点中的索引
        {
            return i;
        }
    }
    Q_ASSERT_X(false, "MusicList::get_current_rootDir()", "wrong index");
}

//获得播放列表名列表
QStringList MusicList::getToplevelNames()
{
    QStringList names;
    for (int i=0; i<this->topLevelItemCount(); ++i)
    {
        names.append(this->topLevelItem(i)->text(0));
    }
    return names;
}

//打开播放临时文件
void MusicList::openTempFile(QString file)
{
    // 在界面上创建“临时列表”
    QTreeWidgetItem *toplevelItem = new QTreeWidgetItem(QStringList(QString("临时列表")));
    this->addTopLevelItem(toplevelItem);

    QTreeWidgetItem *musicItem = new QTreeWidgetItem(QStringList(QFileInfo(file).fileName()));
    this->topLevelItem(this->topLevelItemCount()-1)->addChild(musicItem);

    // 创建临时播放列表
    QMediaPlaylist *playlist = new QMediaPlaylist(this);
    playlistVector.append(playlist);

    // play
    playlist->addMedia(QUrl::fromLocalFile(file));
//    completerList.append(QFileInfo(query_musicName.value("musicName").toString()).fileName());
    this->topLevelItem(0)->setExpanded(false);
    this->topLevelItem(this->topLevelItemCount()-1)->setExpanded(true);
//    this->setCurrentItem(createItem);
    int toplevelIndex = this->topLevelItemCount()-1;
    int musicIndex = this->topLevelItem(toplevelIndex)->childCount()-1;
    playMusic(toplevelIndex, musicIndex);
}

//添加音乐
void MusicList::addMusicToList(int topLevelIndex, QStringList musicNames)
{
    for (int i=0; i<musicNames.length(); ++i)
    {
        QString fileName = QFileInfo(musicNames.at(i)).fileName();
        QTreeWidgetItem *createItem = new QTreeWidgetItem(QStringList(fileName));
        this->topLevelItem(topLevelIndex)->addChild(createItem);
        this->playlistVector[topLevelIndex]->addMedia(QUrl::fromLocalFile(musicNames.at(i)));
        createItem->setToolTip(topLevelIndex, fileName);
        completerList.append(QFileInfo(musicNames.at(i)).fileName());	//添加到自动补全列表
    }

    //设置自动补全
    stringListModel->setStringList(completerList);
}

//添加音乐
void MusicList::addMusicToList(QString topLevelName, QList<QMap<QString, QString> > musicUrlsAndNames)
{
    int topLevelIndex = -1;
    for (int i=0; i<this->topLevelItemCount(); ++i)
    {
        if (this->topLevelItem(i)->text(0) == topLevelName)
        {
            topLevelIndex = i;
            break;
        }
    }
    Q_ASSERT_X(topLevelIndex > -1, "adMusicToList()", "topLevel is not exists");

    for (int i=0; i<musicUrlsAndNames.length(); ++i)
    {
        QString musicUrl = musicUrlsAndNames.at(i).firstKey();
        QString musicName = musicUrlsAndNames.at(i).first();
        QTreeWidgetItem *createItem = new QTreeWidgetItem(QStringList(musicName));
        this->topLevelItem(topLevelIndex)->addChild(createItem);
        this->playlistVector[topLevelIndex]->addMedia(QUrl(musicUrl));
        createItem->setToolTip(topLevelIndex, musicName);
        completerList.append(musicName);	//添加到自动补全列表
    }

    //设置自动补全
    stringListModel->setStringList(completerList);
}

//设置播放模式
void MusicList::setPlayMode(PlayModle::PlayMode playModeValue)
{
    playMode = playModeValue;
    switch(playModeValue)
    {
    case PlayModle::PlayRandom:         // 随机播放
        for (int i=0; i<playlistVector.length(); ++i)
        {
            playlistVector.at(i)->setPlaybackMode(QMediaPlaylist::Random);
        }
        break;
    case PlayModle::PlayOnce:           // 单次播放
        for (int i=0; i<playlistVector.length(); ++i)
        {
            playlistVector.at(i)->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        }
        break;
    case PlayModle::PlaySingle:         // 单曲循环
        for (int i=0; i<playlistVector.length(); ++i)
        {
            playlistVector.at(i)->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        }
        break;
    case PlayModle::PlaySequence:       // 顺序列表
        for (int i=0; i<playlistVector.length(); ++i)
        {
            playlistVector.at(i)->setPlaybackMode(QMediaPlaylist::Sequential);
        }
        break;
    case PlayModle::PlayLoop:           // 列表循环
        for (int i=0; i<playlistVector.length(); ++i)
        {
            playlistVector.at(i)->setPlaybackMode(QMediaPlaylist::Loop);
        }
        break;
    case PlayModle::PlayCustom:         // 自定义播放
        break;
    default:
        break;
    }

    //保存到数据库（子线程）
//    QMap<QString, QString> playMode;
//    playMode.insert("playMode", tr("%1").arg(playModeValue));
//    subThread.updateDatabase(SubThread::UpdateDatabase, setupDatabaseName, "setUp", playMode);
//    subThread.start();

    // 更新配置文件
    xml.alterElementText(xml.SettingElement, xml.PlayModeElement, tr("%1").arg(playModeValue));
}

//
void MusicList::setCurrentRow(int currentIndex, int topLevel /* =-1 */)
{
    if (currentIndex == -1)
    {
        return;
    }

    if (topLevel == -1)
    {
        QMediaPlaylist *currentPlaylist = player->playlist();
        for (int i=0; i<playlistVector.length(); ++i)
        {
            if (currentPlaylist == playlistVector.at(i))
            {
                topLevel = i;
                break;
            }
        }
    }

    Q_ASSERT_X(currentIndex > -1 && topLevel > -1, "setCurrentRow", "index out of range!");

    //如果当前索引有效
    if (currentIndex < this->topLevelItem(topLevel)->childCount())
    {
        //取消上次的高亮行
        if (selectedIndex[0]>=0 && selectedIndex[1]>=0)
        {
            if (this->topLevelItem(selectedIndex[0])->childCount() > 0)
            {
                this->topLevelItem(selectedIndex[0])->child(selectedIndex[1])->setSelected(false);
            }
        }

        //设置当前行
        this->topLevelItem(topLevel)->child(currentIndex)->setSelected(true);

        //标记本次选择的行
        selectedIndex[0] = topLevel;
        selectedIndex[1] = currentIndex;
    }
}

// 单击检索按钮，定位当前行
void MusicList::setCurrentRow(QString text)
{
    Q_ASSERT_X(!text.isEmpty(), "setCurrentRow by text", "text is empty!");

    for (int topLevel=0; topLevel<this->topLevelItemCount()-1; ++topLevel)
    {
        for (int item=0; item<this->topLevelItem(topLevel)->childCount()-1; ++item)
        {
            if (text == this->topLevelItem(topLevel)->child(item)->text(0))
            {
                if (!this->topLevelItem(topLevel)->isExpanded())
                {
                    this->topLevelItem(topLevel)->setExpanded(true);
                }
                this->setCurrentItem(this->topLevelItem(topLevel)->child(item));
                return;
            }
        }
    }
}

//自定义 右键菜单
void MusicList::contextMenuEvent(QContextMenuEvent *event)
{
    //播放列表右键菜单
    if (this->currentItem() == this->topLevelItem(0))
    {
        QMenu menu_rootDir;
        menu_rootDir.addAction(tr("清空所有列表"), this, SLOT(clearAll()));
        menu_rootDir.exec(event ->globalPos());
        return;
    }
    for (int i=0; i<this->topLevelItemCount(); i++)
    {
        if (this->currentItem() == this->topLevelItem(i))
        {
            //创建节点菜单
            QMenu menu_rootDir;
            menu_rootDir.addAction(tr("删除"), this, SLOT(remove_rootDir()));
            menu_rootDir.addAction(tr("重命名"), this, SLOT(renameToplevel(QTreeWidgetItem *item)));
            menu_rootDir.addAction(tr("清空所有列表"), this, SLOT(clearAll()));
            menu_rootDir.exec(event ->globalPos());
            return;
        }
    }

    //音乐右键菜单
    QStringList otherToplevel;                                                      // 选中列表外的其他列表（用于添加到其他列表）
    QString activedToplevel = this->topLevelItem(get_current_rootDir())->text(0);   // 被右键选中的列表的名字
    for(int i=0; i<this->topLevelItemCount(); i++)                                  // 遍历选中之外的其他列表的名字
    {
        QString toplevelText = this->topLevelItem(i)->text(0);
        if (toplevelText == activedToplevel)
        {
            continue;
        }
        otherToplevel.append(toplevelText);
    }

    //创建二级菜单
    QMenu menu_child;
    menu_child.setIcon(QIcon(":/Images/menu_add.png"));
    menu_child.setTitle(tr("添加到..."));
    for (int i=0; i<this->topLevelItemCount(); ++i)
    {
        if (this->topLevelItem(i) != this->topLevelItem(get_current_rootDir()))
        {
            menu_child.addAction(musicMenuActionList[i]);
        }
    }
    connect(&menu_child, SIGNAL(triggered(QAction*)), this, SLOT(add_otherMusicList(QAction*)));
//    for (int i=0; i<otherToplevel.count(); i++)
//    {
//        menu_child.addAction(otherToplevel.at(i), this, SLOT(add_otherMusicList()));
//    }

    //创建项目菜单
    QMenu menu_item;
    menu_item.addAction(QIcon(":/Images/menu_play.png"), tr("立即播放"), this, SLOT(rightBtnPlay()));
    menu_item.addMenu(&menu_child);
    menu_item.addAction(QIcon(":/Images/miniWindow_hover.png"), tr("移出列表"), this, SLOT(removeSelection()));
    menu_item.addAction(QIcon(":/Images/closeWindow_hover.png"), tr("删除歌曲及文件"), this, SLOT(deleteSelection()));
    menu_item.addAction(QIcon(":/Images/menu_clear.png"), tr("清空本列表"), this, SLOT(clearSelf()));
    menu_item.exec(event ->globalPos());
    return;
}

//初始化播放列表（创建[默认列表]）
void MusicList::initMusicList()
{
    for (int i=0; i<DefaultList.count(); ++i)
    {
        createMusiclistToplevel(DefaultList[i]);
    }
    this->topLevelItem(0) ->setExpanded(true);          //展开默认列表
}

//从配置文件加载用户歌曲到播放器
void MusicList::loadMusicList()
{
    /*
    DatabaseOperation db(musicListDatabaseName);
    QList<QMap<QString, QStringList> > tableNamesAndTableData;
    tableNamesAndTableData = db.getTableNamesAndTableData();

    for (int i=0; i<tableNamesAndTableData.length(); ++i)
    {
        QString musicListName = tableNamesAndTableData.at(i).firstKey();
        QStringList musicNames = tableNamesAndTableData.at(i).first();

        if ( !DefaultList.contains(musicListName) )
        {
            createMusiclistToplevel(musicListName);
        }

        addMusicToList(musicListName, musicNames);
    }
    */
    QList<QMap<QString, QList<QMap<QString, QString> > > > musicListAndMusics;
    QStringList urlAndName;
    urlAndName << "url" << "name";
    musicListAndMusics = xml.getElementAttributeValueAndChildrenText(xml.MusicListElement, urlAndName);

    for (int i=0; i<musicListAndMusics.length(); ++i)
    {
        QString musicListName = musicListAndMusics.at(i).firstKey();
        QList<QMap<QString, QString> > musics = musicListAndMusics.at(i).first();

        if ( !DefaultList.contains(musicListName) )
        {
            createMusiclistToplevel(musicListName);
        }

        addMusicToList(musicListName, musics);
    }
}

// 创建播放列表
void MusicList::createMusiclistToplevel(QString toplevelName)
{
    //界面添加 toplevel
    QTreeWidgetItem *createItem = new QTreeWidgetItem(QStringList(QString(toplevelName)));
    this->addTopLevelItem(createItem);

    //为右键菜单事件绑定 action
    QAction *musicMenuAction = new QAction(toplevelName, this);
    musicMenuActionList.append(musicMenuAction);

    //创建对应的播放列表
    QMediaPlaylist *playlist = new QMediaPlaylist(this);
    playlistVector.append(playlist);
}

// 初始化配置文件（ini.xml）
void MusicList::initDefaultMusicList()
{
    QList<QMap<QString, QMap<QString, QString> > > musicListElements;
    for (int i=0; i<DefaultList.length(); ++i)
    {
        QMap<QString, QMap<QString, QString> > musicList;
        QMap<QString, QString> attribute;
        attribute.insert(xml.MusicListElementKey, DefaultList.at(i));
        musicList.insert(xml.MusicListElement, attribute);
        musicListElements.append(musicList);
    }
    xml.addElements(xml.FirstChildElement, musicListElements);
}

/*
// 初始化数据库
void MusicList::createDatebase(QStringList tableNames)
{
    DatabaseOperation db(musicListDatabaseName);
    QStringList columnMessages;
    for (int i=0; i<tableNames.length(); ++i)
    {
       columnMessages << "id integer primary key, musicName text";
    }
    db.createTables(tableNames, columnMessages);
}
*/

/*
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
*/

//播放歌曲
void MusicList::playMusic(int musicListIndex, int musicIndex)
{
    Q_ASSERT_X(musicListIndex > -1 && musicIndex > -1, "playMusic", "Wrong index!");
    Q_ASSERT_X(musicListIndex < playlistVector.length(), "playMusic", "index out of playlistVector range!");
    Q_ASSERT_X(musicIndex < playlistVector.at(musicListIndex)->mediaCount(), "playMusic", "index out of playlist range!");

    player->setPlaylist(playlistVector.at(musicListIndex));
    playlistVector.at(musicListIndex)->setCurrentIndex(musicIndex);
    player->play();
}

//双击处理
void MusicList::doubleClicked(QModelIndex index)
{
    int parentIndex = index.parent().row();
    int selfIndex = index.row();

    if (parentIndex == -1)
    {
        return;
    }

    playMusic(parentIndex, selfIndex);
}

void MusicList::rightBtnPlay()
{
    int toplevelIndex = get_current_rootDir();
    int musicIndex = this->topLevelItem(toplevelIndex)->indexOfChild(this->currentItem());
    playMusic(toplevelIndex, musicIndex);
}

/*
//搜索到歌曲后 处理
void MusicList::searchedMusic(int rootDir, int index)
{
    setCurrentItem(topLevelItem(rootDir) ->child(index));
}
*/
//把歌曲移除列表
void MusicList::removeSelection(bool delete_file)
{
    //获得播放列表和索引值
    int rootDir = get_current_rootDir();
    int currentRow = this->currentIndex().row();
    QString file_path = playlistVector.at(rootDir)->media(currentRow).canonicalUrl().toLocalFile();

    //删除界面中的选中歌曲
    this->topLevelItem(rootDir)->removeChild(this->currentItem());

    //移除播放列表歌曲
    playlistVector.at(rootDir)->removeMedia(currentRow);

    //更新数据库
//    DatabaseOperation db(musicListDatabaseName);
//    db.deleteDatabase(this->topLevelItem(rootDir)->text(0), currentRow+1);

    // 更新配置文件
    QString musicListName = this->topLevelItem(rootDir)->text(0);
    QMap<QString, QString> attribute;
    attribute.insert("id", tr("%1-%2").arg(musicListName).arg(currentRow));
    xml.removeElementByAttribute("music", attribute);

    if (delete_file)
    {
        QFile::remove(file_path);
    }
}

//彻底删除
void MusicList::deleteSelection()
{
    removeSelection(true);
}

//删除播放列表
void MusicList::remove_rootDir()
{
    // 获得选中的列表索引
    //int currentToplevel = get_current_rootDir();          //获得
    int currentToplevel = this->currentIndex().row();
    if (currentToplevel == 0)   // 不能删除默认列表
    {
        return;
    }

    // 获取选中列表名字
    QString deleteName = this->currentItem()->text(0);

    //删除此列表中的歌曲
    while (this->currentItem()->childCount())
    {
        this->currentItem()->removeChild(this->currentItem()->child(0));
    }

    //最后删除播放列表
    //this->removeItemWidget(rootDirVector[currentToplevel], 0);
    this->takeTopLevelItem(currentToplevel);

    //更新数据库
//    DatabaseOperation db(musicListDatabaseName);
//    db.deleteTable(deleteName);

    // 更新配置文件
    QMap<QString, QString> attribute;
    attribute.insert(xml.MusicListElementKey, deleteName);
    xml.removeElementByAttribute(xml.MusicListElement, attribute);

    //删除绑定的 action
    musicMenuActionList.removeAt(currentToplevel);
}

// 重命名
void MusicList::renameToplevel(QTreeWidgetItem *item)
{

}

// 试听在线音乐
void MusicList::playInternetMusic(QString musicUrl, QString musicName)
{
    // 添加到界面播放列表
    QList<QMap<QString, QString> > musicUrlsAndNames;
    QMap<QString, QString> musicUrlAndName;
    musicUrlAndName.insert(musicUrl, musicName);
    musicUrlsAndNames.append(musicUrlAndName);
    addMusicToList(DefaultList.at(1)/*"在线试听"*/, musicUrlsAndNames);

    // 定位到播放的歌曲
    if (!this->topLevelItem(1)->isExpanded())
    {
        this->topLevelItem(1)->setExpanded(true);
    }
    this->setCurrentItem(this->topLevelItem(1)->child(this->topLevelItem(1)->childCount()-1));

    // 播放
    playMusic(1, this->topLevelItem(1)->childCount()-1);

    // 更新配置文件
    QList<QMap<QString, QMap<QString, QString> > > elementNameAttributeKeyValue;
    QMap<QString, QMap<QString, QString> > nameKeyValue;
    QMap<QString, QString> keyValue;
    keyValue.insert("id", "null");
    nameKeyValue.insert(xml.MusicElement, keyValue);
    elementNameAttributeKeyValue.append(nameKeyValue);

    QStringList urlName;
    urlName << "url" << "name";
    QList<QMap<QString, QString> > children;

    QMap<QString, QString> urlAndName;
    urlAndName.insert(musicUrl, musicName);
    children.append(urlAndName);
    xml.addRecursiveElement(xml.MusicListElement, xml.MusicListElementKey, DefaultList.at(1)/*在线试听*/, elementNameAttributeKeyValue, urlName, children);
}

//清空所有列表
void MusicList::clearAll()
{
    //清空所有列表
    this ->clear();

    //重新初始化播放列表
    initMusicList();

    // 更新配置文件
    xml.removeElements(xml.MusicElement);
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
//    QString databaseName = this->topLevelItem(rootDir)->text(0);
//    DatabaseOperation db_delete_all(musicListDatabaseName);
//    db_delete_all.deleteAllOfTable(databaseName);

    // 更新配置文件
    QString musicListName = this->topLevelItem(rootDir)->text(0);
    QMap<QString, QString> attribute;
    attribute.insert(xml.MusicListElementKey, musicListName);
    xml.removeAllChildrenByAttribute(xml.MusicListElement, xml.MusicListElementKey, musicListName);
}

//添加歌曲到其他列表
void MusicList::add_otherMusicList(QAction *action)
{
    int currentToplevel = get_current_rootDir();                                        // 当前列表索引
    int itemIndex = this->currentIndex().row();                                         // 当前歌曲索引
    QString musicName = this->topLevelItem(currentToplevel)->child(itemIndex)->text(0); // 当前歌曲名字
    QString musicUrl = playlistVector.at(currentToplevel)->media(itemIndex).canonicalUrl().toString();

    int targetIndex = -1;
    for (targetIndex=0; targetIndex<this->topLevelItemCount(); ++targetIndex)
    {
        if (action->text() == this->topLevelItem(targetIndex)->text(0))
        {
            break;
        }
    }
    Q_ASSERT_X(targetIndex>-1, "add_otherMusicList", "wrong index!");

    QList<QMap<QString, QString> > music;
    QMap<QString, QString> musicUrlAndName;
    musicUrlAndName.insert(musicUrl, musicName);
    music.append(musicUrlAndName);
    addMusicToList(action->text(), music);
    if (!this->topLevelItem(targetIndex)->isExpanded())
    {
        this->topLevelItem(targetIndex)->setExpanded(true);
    }

//    DatabaseOperation db(musicListDatabaseName);
//    db.insertDatabase(this->topLevelItem(targetIndex)->text(0), "musicName", QStringList(musicName));

    // 更新配置文件
    QList<QMap<QString, QString> > musics;
    QMap<QString, QString> urlAndName;
    urlAndName.insert(musicUrl, musicName);
    musics.append(urlAndName);

    QList<QMap<QString, QMap<QString, QString> > > elementsNameAttributeAndValue;
    QMap<QString, QString> attributeKeyAndValue;
    QMap<QString, QMap<QString, QString> > nameAttributeAndValue;
    attributeKeyAndValue.insert("id", tr("%1-%2").arg(action->text()).arg("null"));
    nameAttributeAndValue.insert(xml.MusicElement, attributeKeyAndValue);
    elementsNameAttributeAndValue.append(nameAttributeAndValue);

    QStringList childrenElementNames;
    childrenElementNames << "url" << "name";
    xml.addRecursiveElement(xml.MusicListElement, xml.MusicListElementKey, action->text(), elementsNameAttributeAndValue, childrenElementNames, musics);

  /*for (int i=0,act=0; i<musicMenuActionList.length(); ++i)
    {
        if (this->topLevelItem(i) == this->topLevelItem(get_current_rootDir()))
        {
//            continue;
        }
        if (action == musicMenuActionList[act])
        {
            int other_toplevel_index = act;                                                 // 点击菜单的索引
            int current_toplevel = get_current_rootDir();                                   // 当前列表索引
            int item_index = this->currentIndex().row();                                    // 当前歌曲索引
            QTreeWidgetItem *activedToplevel = this->topLevelItem(get_current_rootDir());   // 当前列表的指针
            for(int index=0, other=0; index<this->topLevelItemCount(); index++)             // 找出点击列表的索引
            {
                if (this->topLevelItem(index) == activedToplevel)
                {
                    //continue;
                }
                if (other == other_toplevel_index)
                {
                    QString musicName = playlistVector[current_toplevel]->media(item_index).canonicalUrl().toLocalFile();
                    addMusicToList(index, QStringList(musicName));
                    if (!this->topLevelItem(index)->isExpanded())       //展开
                    {
                        this->topLevelItem(index)->setExpanded(true);
                    }
                    DatabaseOperation db(musicListDatabaseName);
                    db.insertDatabase(this->topLevelItem(index)->text(0), "musicName", QStringList(musicName));
                    return;
                }
                other++;
            }
        }
        act++;
    }*/
}

//设置音量增加
void MusicList::setVolumnPlus()
{
    player->setVolume(player->volume() + 5);
}

//设置音量减小
void MusicList::setVolumnLess()
{
    player->setVolume(player->volume() - 5);
}

/*
//释放子线程
void MusicList::releaseThread()
{
    //    subThread->deleteLater();
}
*/
