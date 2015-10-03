#include "create_musiclist.h"
#include <QDebug>

#include <QLineEdit>
#include <QPushButton>

Create_MusicList::Create_MusicList(QWidget *parent) :
    QLabel(parent)
{

//    createMusiclist_name = tr("新建列表");
//    QLabel *lab_create_MusicList = new QLabel(this);
    this->setGeometry(50, 200, 190, 120);

    lab_title = new QLabel("输入列表名", this);
    ln_name = new QLineEdit(this);
    pbn_ok = new QPushButton("确认", this);
    pbn_cancle = new QPushButton("取消", this);

    lab_title->setGeometry(50, 13, 90, 20);
    ln_name ->setGeometry(35, 45, 115, 20);
    pbn_ok ->setGeometry(25, 85, 50, 17);
    pbn_cancle ->setGeometry(105, 85, 50, 17);

    lab_title->setAlignment(Qt::AlignCenter);
    ln_name ->setText(tr("新建列表"));
    ln_name ->setSelection(0, 100);
    ln_name ->setFocus();

//    lab_create_MusicList ->show();

//    connect(tbn_ok, SIGNAL(clicked()), this, SLOT(create_musicList_ok()));        //确定 创建播放列表
    connect(pbn_cancle, SIGNAL(clicked()), this, SLOT(close()));                    //取消 创建播列表 关闭窗口
    connect(pbn_ok, SIGNAL(clicked()), this, SLOT(pbn_ok_clicked()));
    connect(ln_name, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
//    connect(ln_create_MusicList, SIGNAL(textChanged(QString)), this, SLOT(update_createMusiclist_name(QString)));
//    connect(this, SLOT(create_musicList_ok()), ln_create_MusicList, SLOT(close()));


    lab_title->setObjectName("title");
    this->setStyleSheet(
                "QLabel{"
                    "background: rgba(0, 0, 0, 150);"
                    "border-radius: 3px;"
                "}"
                "QPushButton{"
                    "border-radius: 8px;"
                    "background: rgba(246, 237, 144, 240);"
                "}"
                "QLineEdit{"
                    "background: rgba(255, 255, 255, 130);"
                    "border-radius: 2px;"
                "}"
                "#title{"
                    "background: rgba(0, 0, 0, 0);"
                    "color: white;"
                "}"
                  );
}

//命名错误警告
void Create_MusicList::setNameError()
{
    lab_title->setText("<font color=red>列表名已存在!</font>");
    ln_name ->setSelection(0, 100);
}

//单击确定按钮
void Create_MusicList::pbn_ok_clicked()
{
    emit pbn_ok_click(ln_name ->text());
}

//
void Create_MusicList::textChanged()
{
    lab_title->setText("输入列表名");
}
