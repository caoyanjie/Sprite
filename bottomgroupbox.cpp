#include "bottomgroupbox.h"
#include <QMessageBox>

#include "databaseoperation.h"

#include <QMouseEvent>
#include <QToolButton>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QFontMetrics>

#include <QMediaMetaData>   //多媒体数据

//布局
#include <QHBoxLayout>
#include <QGridLayout>

BottomGroupBox::BottomGroupBox(MusicList *object, QString programPath, QWidget *parent) :
    QGroupBox(parent)
  ,musicList(object)
  ,setupDatabaseName(programPath + "setUp.db")
{
    //设置样式
    this ->setObjectName(tr("bottomGroupBox"));                         //显示 设置 对象名
    setStyleSheet(
                  "#bottomGroupBox{"                                    //设置 自身样式
                        "border: 1px;"
                        "background: rgba(65, 65, 65, 200);"
                  "}");

    //初始化 附属 部件
    setChildrenWidgets();

    //为部件安装 事件过滤器
//    tbn_play_pause ->installEventFilter(this);
//    tbn_play_previous ->installEventFilter(this);
//    tbn_play_next ->installEventFilter(this);
    tbn_volumn ->installEventFilter(this);
//    cbx_lrc ->installEventFilter(this);
    tbn_playModle ->installEventFilter(this);
//    slider_progress->installEventFilter(this);

    //信号/槽 关联
    connect(musicList->player, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(getMetaData(bool)));  //多媒体 可用数据信息 改变
    connect(musicList->player, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));   //多媒体 时长信息 出现
    connect(tbn_play_pause, SIGNAL(clicked()), this, SLOT(play_pause_clicked()));                       //单击 播放/暂停 按钮
    connect(tbn_play_previous, SIGNAL(clicked()), this, SLOT(play_previous_clicked()));                 //单击 上一曲    按钮
    connect(tbn_play_next, SIGNAL(clicked()), this, SLOT(play_next_clicked()));                         //单击 下一曲    按钮
    connect(musicList->player, SIGNAL(stateChanged(QMediaPlayer::State)),                               //播放状态 改变
            this, SLOT(playStatusChanged(QMediaPlayer::State)));
    connect(cbx_lrc, SIGNAL(clicked(bool)), this, SIGNAL(lrc_click(bool)));                             //单击 桌面歌词 　按钮
    connect(slider_progress, SIGNAL(sliderMoved(int)), this, SLOT(slider_progress_moved(int)));         //拖动 播放 进度条
//    connect(bottomGroupbox, SIGNAL(slider_progress_valueChange(qint64)), this, SLOT(seek(qint64)));   //更新 进度条
    connect(musicList->player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));   //更新进度条
    connect(slider_progress, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));                          //定位播放进度
}

//忽略 鼠标按下 事件
void BottomGroupBox::mousePressEvent(QMouseEvent *event)
{
    event ->ignore();
}

//忽略 鼠标移动 事件
void BottomGroupBox::mouseMoveEvent(QMouseEvent *event)
{
    event ->ignore();
}

//事件过滤器
bool BottomGroupBox::eventFilter(QObject *target, QEvent *event)
{
/*
    if (target == tbn_play_pause ||
            target == tbn_play_previous ||
            target == tbn_play_next ||
            target == cbx_lrc ||
            target == tbn_volumn ||
            target == tbn_playModle)
    {
        if (event ->type() == QEvent::MouseMove)
        {
            event ->accept();
            event->ignore();
            ;
        }
        else
        {
            return QGroupBox::eventFilter(target, event);
        }
    }
*/
    if (event->type() == QEvent::Enter)
    {
        if (target == tbn_volumn)
        {
            emit showVolumn(tbn_volumn->pos());
        }
        else if (target == tbn_playModle)
        {
            emit showPlayModle(tbn_playModle->pos());
        }
    }
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent *)event)->button() == Qt::LeftButton)
        {
            int max = ((QSlider*)target)->maximum();
            int min = ((QSlider*)target)->minimum();
            int pointPos = ((double)((QMouseEvent *)event)->x()) / ((QSlider*)target)->width()*(max - min)+min;
            if(abs(pointPos - ((QSlider*)target)->value())> (((QSlider*)target)->pageStep()) )
            {
                ((QSlider*)target)->setValue(pointPos);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return QGroupBox::eventFilter(target, event);
}

//初始化 附属 部件
void BottomGroupBox::setChildrenWidgets()
{
    //创建部件
    tbn_play_previous = new QToolButton(this);                          //创建 上一曲    按钮
    tbn_play_previous->setFixedSize(25, 25);                            //设置固定大小
    tbn_play_previous  ->setShortcut(Qt::Key_Left);                     //设置 快捷键

    tbn_play_pause = new QToolButton(this);                             //创建 播放/暂停　按钮
    tbn_play_pause->setFixedSize(25, 25);                               //设置固定大小
    tbn_play_pause ->setShortcut(Qt::Key_Space);                        //设置 快捷键

    tbn_play_next = new QToolButton(this);                              //创建 下一曲    按钮
    tbn_play_next->setFixedSize(25, 25);
    tbn_play_next ->setShortcut(Qt::Key_Right);                         //设置 快捷键

    lab_fram = new QLabel(this);                                        //创建 工具框    区域
    lab_playLogo = new QLabel(this);                                    //创建 播放图标
    slider_progress = new QSlider(Qt::Horizontal, this);                //创建 进度条    滑竿
    slider_progress->setPageStep(1);
    lab_musicMessage = new QLabel(tr("歌曲： 未知\n歌手： 未知"), this);    //创建 歌曲信息  标签
    lab_currentTime = new QLabel(tr("00:00"), this);                    //创建 播放时间  标签
    lab_totalTime = new QLabel(tr("/00:00"), this);                     //创建 总时长    标签
    cbx_lrc = new QCheckBox("桌面歌词", this);                           //创建 歌词      按钮
    tbn_volumn = new QToolButton(this);                                 //创建 音量      按钮
    tbn_playModle = new QToolButton(this);                              //创建 播放模式   按钮
    lab_fram->setFixedHeight(50);
    lab_musicMessage->setFixedHeight(31);
    lab_currentTime->setFixedSize(34, 31);
    lab_totalTime->setFixedSize(41, 31);
    slider_progress->setFixedHeight(19);
    lab_playLogo->setFixedSize(40, 40);
    cbx_lrc->setFixedSize(80, 26);
    tbn_volumn->setFixedSize(26, 26);
    tbn_playModle->setFixedSize(24, 24);

    lab_currentTime->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    lab_totalTime->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    //布局
    layout_H = new QHBoxLayout;
    layout_H->addWidget(lab_musicMessage);
    layout_H->addWidget(lab_currentTime);
    layout_H->addWidget(lab_totalTime);
    layout_H->setSpacing(0);
    layout_H->setContentsMargins(0, 0, 3, 0);

    layout_G = new QGridLayout;
    layout_G->addWidget(lab_fram, 0, 0, 50, 605);
    layout_G->addWidget(lab_playLogo, 5, 5, 40, 40);
    layout_G->addLayout(layout_H, 0, 52, 31, 545);
    layout_G->addWidget(slider_progress, 36, 41, 13, 565);
    layout_G->setSpacing(0);
    layout_G->setMargin(0);

    layout_top = new QHBoxLayout;
    layout_top->addWidget(tbn_play_previous);
    layout_top->addWidget(tbn_play_pause);
    layout_top->addWidget(tbn_play_next);
    layout_top->addLayout(layout_G);
    layout_top->addWidget(cbx_lrc);
    layout_top->addWidget(tbn_volumn);
    layout_top->addWidget(tbn_playModle);
    layout_top->setSpacing(25);
    layout_top->setContentsMargins(10, 10, 18, 20);
    setLayout(layout_top);

    //初始化 样式
    tbn_play_previous ->setStyleSheet(
                                      "QToolButton{"                                                    //设置 上一曲 按钮    样式
                                            "border-image: url(:/Images/play_previous.png);"
                                      "}"
                                      "QToolButton::hover{"
                                            "border-image: url(:/Images/play_previous_hover.png);"
                                      "}"
                                      );
    tbn_play_pause ->setStyleSheet(
                                   "QToolButton{"                                                       //设置 播放/暂停 按钮 样式
                                        "border-image: url(:/Images/play_play.png);"
                                   "}"
                                   "QToolButton::hover{"
                                        "border-image: url(:/Images/play_play_hover.png);"
                                   "}"
                                  );
    tbn_play_next ->setStyleSheet(
                                  "QToolButton{"                                                        //设置 下一曲 按钮    样式
                                        "border-image: url(:/Images/play_next.png);"
                                  "}"
                                  "QToolButton::hover{"
                                        "border-image: url(:/Images/play_next_hover.png);"
                                  "}"
                                 );
    lab_fram ->setStyleSheet(
                            "QLabel{"
                                    "border-radius: 5px;"
//                                    "background: rgba(0, 0, 0, 180);"
                                    "background: rgba(35, 35, 35, 200);"
                            "}"
                            );
    lab_playLogo ->setStyleSheet(
                                "QLabel{"
                                        "border-image: url(:/Images/playLogo.png);"
                                "}"
                                );
    slider_progress ->setStyleSheet(
                                    "QSlider::groove:horizontal{"                                       //设置 进度条        样式
                                        "border: 1px solid #000000;"
                                        "height: 2px;"
                                        "margin: 0px 0;"
                                        "left: 12px;"
                                        "right: 12px;"
                                    "}"
                                    "QSlider::handle:horizontal{"
                                        "border: 1px solid #5c5c5c;"
                                        "border-image: url(:/Images/sliderHandle.png);"
                                        "width: 10px;"
//                                        "height: 15px;"
                                        "margin: -5px -5px -5px -5px;"
                                    "}"
                                    "QSlider::sub-page:horizontal{"
                                        "background: rgb(115, 153, 1);"
                                    "}"
                                    "QSlider::add-page:horizontal{"
                                        "background: rgb(83, 83, 83);"
                                    "}"
                                    "QSlider::handle::hover:horizontal{"
                                        "border-image: url(:/Images/sliderHandle_hover.png);"
                                    "}"
                                    );

    lab_musicMessage ->setStyleSheet(
                                    "QLabel{"                                                          //设置 歌曲信息 样式
                                         "color: rgb(36, 86, 18);"
                                    "}"
                                     );
    lab_currentTime ->setStyleSheet(
                                    "QLabel{"                                                           //设置 当前时间 样式
                                         "color: rgb(36, 86, 18);"
                                    "}"
                                    );
    lab_totalTime ->setStyleSheet(
                                  "QLabel{"                                                             //设置 总时间   样式
                                        "color: rgb(36, 86, 18);"
                                  "}"
                                  );
    tbn_playModle ->setStyleSheet(tr(
                                  "QToolButton{"                                                        //设置 播放模式 样式
                                        "border-image: url(:/Images/%1.png);"
                                   "}"
                                  "QToolButton::hover{"
                                        "border-image: url(:/Images/%2_hover.png);"
                                   "}"
                                  ).arg(4).arg(4));
    cbx_lrc ->setStyleSheet(
                           "QCheckBox{"
                                "border: 0;"
                                "color: white;"
                           "}"
                            );
    tbn_volumn ->setStyleSheet(
                               "QToolButton{"
                                    "border-image: url(:/Images/volumn.png);"
                               "}"
                               "QToolButton::hover{"
                                    "border-image: url(:/Images/volumn_hover.png);"
                               "}"
                               );

    //设置 按钮提示
    tbn_play_previous ->setToolTip(tr("上一曲"));
    tbn_play_pause ->setToolTip(tr("播放/暂停"));
    tbn_play_next ->setToolTip(tr("下一曲"));
}

/*
//单击 播放/暂停 按钮
void BottomGroupBox::play_pause_clicked()
{
    emit play_pause_click();
}

//单击 上一曲 按钮
void BottomGroupBox::play_previous_clicked()
{
    emit play_previous_click();
}

//单击 下一曲 按钮
void BottomGroupBox::play_next_clicked()
{
    emit play_next_click();
}

//单击 桌面歌词 按钮
void BottomGroupBox::lrc_clicked(bool checked)
{
        emit lrc_click(checked, cbx_lrc->pos());
}
*/

//播放模式改变 处理
void BottomGroupBox::playModle_choosed(PlayModle::PlayMode currentPlayModel)
{
    switch(currentPlayModel)
    {
    case PlayModle::PlayRandom:
        tbn_playModle ->setStyleSheet(
                    "QToolButton{"
                        "border-image: url(:/Images/playModel_random.png);"
                    "}"
                    "QToolButton::hover{"
                        "border-image: url(:/Images/playModel_random_hover.png);"
                    "}"
                    );
        musicList->setPlayMode(PlayModle::PlayRandom);
        break;
    case PlayModle::PlayOnce:
        tbn_playModle ->setStyleSheet(
                    "QToolButton{"
                        "border-image: url(:/Images/playModel_once.png);"
                    "}"
                    "QToolButton::hover{"
                        "border-image: url(:/Images/playModel_once_hover.png);"
                    "}"
                    );
        musicList->setPlayMode(PlayModle::PlayOnce);
        break;
    case PlayModle::PlaySingle:
        tbn_playModle ->setStyleSheet(
                    "QToolButton{"
                        "border-image: url(:/Images/playModel_single.png);"
                    "}"
                    "QToolButton::hover{"
                        "border-image: url(:/Images/playModel_single_hover.png);"
                    "}"
                    );
        musicList->setPlayMode(PlayModle::PlaySingle);
        break;
    case PlayModle::PlaySequence:
        tbn_playModle ->setStyleSheet(
                    "QToolButton{"
                        "border-image: url(:/Images/playModel_sequence.png);"
                    "}"
                    "QToolButton::hover{"
                        "border-image: url(:/Images/playModel_sequence_hover.png);"
                    "}"
                    );
        musicList->setPlayMode(PlayModle::PlaySequence);
        break;
    case PlayModle::PlayLoop:
        tbn_playModle ->setStyleSheet(
                    "QToolButton{"
                        "border-image: url(:/Images/playModel_loop.png);"
                    "}"
                    "QToolButton::hover{"
                        "border-image: url(:/Images/playModel_loop_hover.png);"
                    "}"
                    );
        musicList->setPlayMode(PlayModle::PlayLoop);
        break;
    case PlayModle::PlayCustom:
        tbn_playModle ->setStyleSheet(
                    "QToolButton{"
                        "border-image: url(:/Images/playModel_custom.png);"
                    "}"
                    "QToolButton::hover{"
                        "border-image: url(:/Images/playModel_custom_hover.png);"
                    "}"
                    );
        musicList->setPlayMode(PlayModle::PlayCustom);
        break;
    }
}

//发送音乐标题和作者信息
void BottomGroupBox::sendMusicTitleAndAuthor()
{
    emit musicTitleAndAuthor(lab_musicMessage->text());
}

//播放 时长信息 出现
void BottomGroupBox::durationChanged(qint64 totalTime)
{
    lab_totalTime ->setText(tr("%1").sprintf("/%02d:%02d",
                                             (int)totalTime / 1000 / 60,
                                             (int)totalTime / 1000 % 60));
    slider_progress ->setMaximum(totalTime);

    QString filename = musicList->player->playlist()->currentMedia().canonicalUrl().fileName();
    filename = filename.remove(filename.right(4));

    QString musicMessage = tr("歌曲：%1").arg(filename);
    QFontMetrics metrics(this->font());
    musicMessage = metrics.elidedText(musicMessage, Qt::ElideRight, 300);

    lab_musicMessage->setText(musicMessage);
    lab_musicMessage->setToolTip(filename);
    emit musicTitleAndAuthor(tr("歌曲：%1").arg(filename));
}

//提取 歌曲信息
void BottomGroupBox::getMetaData(bool)
{
    QStringList title_author;
    title_author.append(musicList->player->metaData(QMediaMetaData::Title).toString());
    if (title_author.at(0).isEmpty())
    {
        return;
    }
    title_author.append(musicList->player->metaData(QMediaMetaData::Author).toString());
    QString musicMessage = tr("歌曲：%1\n歌手：%2").arg(title_author.at(0)).arg(title_author.at(1));
    lab_musicMessage->setText(musicMessage);
    emit musicTitleAndAuthor(musicMessage);

    //test
    QImage coverArtImage;
    QPixmap pixmap;
    coverArtImage = musicList->player->metaData("CoverArtImage").value<QImage>();
    if (coverArtImage.isNull())
    {
//        QMessageBox::warning(0, tr("image"), tr("没取到"), QMessageBox::Ok);
    }
    pixmap.convertFromImage(coverArtImage);
    lab_playLogo->setPixmap(pixmap);
}

//播放 进度改变 处理
void BottomGroupBox::positionChanged(qint64 currentPosition)
{
    lab_currentTime ->setText(tr("%1").sprintf("%02d:%02d",
                                             (int)currentPosition / 1000 / 60,
                                             (int)currentPosition / 1000 % 60));
    slider_progress ->setValue(currentPosition);
}

void BottomGroupBox::slider_progress_moved(int currentPosition)
{
    musicList->player->setPosition(currentPosition);
}

//播放/暂停 处理
void BottomGroupBox::play_pause_clicked()
{
    if (musicList->player->state() == musicList->player->PlayingState)
    {
        musicList->player->pause();
    }
    else
    {
        musicList->player->play();
    }
}

//上一曲 处理
void BottomGroupBox::play_previous_clicked()
{
    musicList->playlistVector.at(musicList->get_current_rootDir())->previous();
}

//下一曲 处理
void BottomGroupBox::play_next_clicked()
{
    musicList->playlistVector.at(musicList->get_current_rootDir())->next();
}

//播放状态改变， 设置 “播放/暂停” 按钮图标
void BottomGroupBox::playStatusChanged(QMediaPlayer::State state)
{
    switch(state)
    {
    case QMediaPlayer::PlayingState:
        tbn_play_pause ->setStyleSheet(
                    "QToolButton{"                                           //设置 播放 图标
                        "border-image: url(:/Images/play_pause.png);"
                    "}"
                    "QToolButton::hover{"                                    //设置 伪状态 图标
                        "border-image: url(:/Images/play_pause_hover.png);"
                    "}"
                    );
        break;
    case QMediaPlayer::PausedState:
    case QMediaPlayer::StoppedState:
        tbn_play_pause ->setStyleSheet(
                    "QToolButton{"                                           //设置 暂停 图标
                        "border-image: url(:/Images/play_play.png);"
                    "}"
                    "QToolButton::hover{"                                    //设置 伪状态 图标
                        "border-image: url(:/Images/play_play_hover.png);"
                    "}"
                    );
        break;
    default:
        qDebug() << "Recieved the unknown playStatus signal!!!";
    }
}

//定位播放进度
void BottomGroupBox::seek(int current)
{
    musicList->player->setPosition(current);
}
