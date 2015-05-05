#include "create_musiclist.h"
#include <QDebug>

#include <QLineEdit>
#include <QPushButton>

Create_MusicList::Create_MusicList(QWidget *parent) :
    QLabel(parent)
{

//    createMusiclist_name = tr("新建列表");
//    QLabel *lab_create_MusicList = new QLabel(this);
    setGeometry(50, 200, 190, 100);
    setStyleSheet(
                "QLabel{"
                    "background: rgba(0, 0, 0, 150);"
                    "border-radius: 3px;"
                "}"
                "QPushButton{"
                    "border-radius: 8px;"
                    "background: rgba(246, 237, 144, 240);"
                "}"
                  );
    ln_name = new QLineEdit(this);
    pbn_ok = new QPushButton("确认", this);
    pbn_cancle = new QPushButton("取消", this);

    ln_name ->setGeometry(35, 25, 115, 20);
    pbn_ok ->setGeometry(25, 65, 50, 17);
    pbn_cancle ->setGeometry(105, 65, 50, 17);

    ln_name ->setStyleSheet(
                            "QLineEdit{"
                                    "background: rgba(255, 255, 255, 130);"
                                    "border-radius: 2px;"
                            "}"
                            );

    ln_name ->setText(tr("新建列表"));

    ln_name ->setSelection(0, 100);
    ln_name ->setFocus();

//    lab_create_MusicList ->show();

//    connect(tbn_ok, SIGNAL(clicked()),                  //确定 创建播放列表
//            this, SLOT(create_musicList_ok()));
    connect(pbn_cancle, SIGNAL(clicked()),              //取消 创建播列表 关闭窗口
            this, SLOT(close()));
    connect(pbn_ok, SIGNAL(clicked()),
            this, SLOT(pbn_ok_clicked()));
//    connect(ln_create_MusicList, SIGNAL(textChanged(QString)),
//            this, SLOT(update_createMusiclist_name(QString)));
//    connect(this, SLOT(create_musicList_ok()),
    //            ln_create_MusicList, SLOT(close()));
}

//单击确定按钮
void Create_MusicList::pbn_ok_clicked()
{
    emit pbn_ok_click(ln_name ->text());
}
