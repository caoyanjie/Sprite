#include "theme.h"

#include <QToolButton>
#include <QLabel>
//#include <QEvent>
#include <QMouseEvent>
#include <QDesktopWidget>
//#include <QPoint>

Theme::Theme(QWidget *parent) :
    QWidget(parent)
{
    //窗口去边框
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //窗口透明
    setAttribute(Qt::WA_TranslucentBackground);

    //设置窗口出现位置
//    setGeometry(parent ->pos().rx(), parent ->pos().ry(), 510, 400);
    QDesktopWidget desktopWidget;
    setGeometry(
                (desktopWidget.width() - 510) / 2,
                (desktopWidget.height() - 400) / 2,
                510,
                400         //Windows OS
//                370         //Linux OS
                );

    setObjectName(tr("theme"));
    setStyleSheet(
                "#theme{"
//                    "background: rgb(49, 28, 242);"
//                    "background: rgb(56, 30, 255);"
//                    "background: rgb(33, 35, 124);"
                    "background: rgb(43, 45, 163);"
//                    "border-image: url(:/Images/bg1.jpg);"
                "}"
                );

    //初始化 部件
    lab_background = new QLabel(this);          //创建背景， 以设置窗口半透明

    tbn_close = new QToolButton(this);
    tbn_theme1 = new QToolButton(this);
    tbn_theme2 = new QToolButton(this);
    tbn_theme3 = new QToolButton(this);
    tbn_theme4 = new QToolButton(this);
    tbn_theme5 = new QToolButton(this);
    tbn_theme6 = new QToolButton(this);
    tbn_theme7 = new QToolButton(this);
    tbn_theme8 = new QToolButton(this);
    tbn_theme9 = new QToolButton(this);
    tbn_ok = new QToolButton(this);
    tbn_cancle = new QToolButton(this);
    tbn_apply = new QToolButton(this);

    //设置 部件位置大小
    lab_background ->setGeometry(0, 0, 510, 400);

    tbn_close ->setGeometry(490, 8, 11, 11);
    tbn_theme1 ->setGeometry(20, 25, 150, 100);
    tbn_theme2 ->setGeometry(180, 25, 150, 100);
    tbn_theme3 ->setGeometry(340, 25, 150, 100);
    tbn_theme4 ->setGeometry(20, 135, 150, 100);
    tbn_theme5 ->setGeometry(180, 135, 150, 100);
    tbn_theme6 ->setGeometry(340, 135, 150, 100);
    tbn_theme7 ->setGeometry(20, 245, 150, 100);
    tbn_theme8 ->setGeometry(180, 245, 150, 100);
    tbn_theme9 ->setGeometry(340, 245, 150, 100);
    tbn_ok ->setGeometry(100, 365, 50, 20);
    tbn_cancle ->setGeometry(230, 365, 50, 20);
    tbn_apply ->setGeometry(360, 365, 50, 20);

    //为部件 安装事件过滤器
    tbn_close ->installEventFilter(this);
    tbn_theme1 ->installEventFilter(this);
    tbn_theme2 ->installEventFilter(this);
    tbn_theme3 ->installEventFilter(this);
    tbn_theme4 ->installEventFilter(this);
    tbn_theme5 ->installEventFilter(this);
    tbn_theme6 ->installEventFilter(this);
    tbn_theme7 ->installEventFilter(this);
    tbn_theme8 ->installEventFilter(this);
    tbn_theme9 ->installEventFilter(this);
    tbn_ok ->installEventFilter(this);
    tbn_cancle ->installEventFilter(this);
    tbn_apply ->installEventFilter(this);

    //设置 部件样式
    lab_background ->setStyleSheet(                             //设置窗口 半透明
                "QLabel{"
//                    "background: rgba(0, 0, 0, 80);"
//                    "background: rgba(43, 45, 163, 230);"
                    "background: rgba(0, 0, 0, 120);"
                "}"
                );

    tbn_close ->setStyleSheet(
                "QToolButton{"
                    "border-image: url(:/Images/closeWindow.png);"
                "}"
                "QToolButton::hover{"
                    "border-image: url(:/Images/closeWindow_hover.png);"
                "}"
                );
    tbn_theme1 ->setStyleSheet(
                "QToolButton{"
                    "border-image: url(:/Images/bg1.jpg);"
                "}"
                "QToolButton::hover{"
//                    "border-image: url(:/Images/preview1.png);"
                    "border: 1px;"
                "}"
                );
    tbn_theme2 ->setStyleSheet(
                "QToolButton{"
                    "border-image: url(:/Images/bg2.jpg);"
                "}"
                "QToolButton::hover{"
//                  "border-image: url(:/Images/preview2.png);"
                    "border: 1px;"
                "}"
                );
    tbn_theme3 ->setStyleSheet(
                "QToolButton{"
                    "border-image: url(:/Images/bg3.jpg);"
                "}"
                "QToolButton::hover{"
//                    "border-image: url(:/Images/preview3.png);"
                    "border: 1px;"
                "}"
                );
    tbn_theme4 ->setStyleSheet(
                "QToolButton{"
                    "border-image: url(:/Images/bg4.jpg);"
                "}"
                "QToolButton::hover{"
//                    "border-image: url(:/Images/preview4.png);"
                    "border: 1px;"
                "}"
                );
    tbn_theme5 ->setStyleSheet(
                "QToolButton{"
                    "border-image: url(:/Images/bg5.jpg);"
                "}"
                "QToolButton::hover{"
//                  "border-image: url(:/Images/preview5.png);"
                    "border: 1px;"
                "}"
                );
    tbn_theme6 ->setStyleSheet(
                "QToolButton{"
                    "border-image: url(:/Images/bg6.jpg);"
                "}"
                "QToolButton::hover{"
//                  "border-image: url(:/Images/preview6.png);"
                    "border: 1px;"
                "}"
                );
    tbn_theme7 ->setStyleSheet(
                "QToolButton{"
                    "border-image: url(:/Images/bg7.jpg);"
                "}"
                "QToolButton::hover{"
//                  "border-image: url(:/Images/preview7.png);"
                    "border: 1px;"
                "}"
                );
    tbn_theme8 ->setStyleSheet(
                "QToolButton{"
                    "border-image: url(:/Images/bg8.jpg);"
                "}"
                "QToolButton::hover{"
//                    "border-image: url(:/Images/preview8.png);"
                    "border: 1px;"
                "}"
                );
    tbn_theme9 ->setStyleSheet(
                "QToolButton{"
                    "border-image: url(:/Images/bg9.jpg);"
                "}"
                "QToolButton::hover{"
//                  "border-image: url(:/Images/preview9.png);"
                    "border: 1px;"
                "}"
                );
    tbn_ok ->setStyleSheet(
                "QToolButton{"
                    "background: rgb(246, 237, 144);"
                    "border-radius: 10px;"
                "}"
                );
    tbn_cancle ->setStyleSheet(
                "QToolButton{"
                    "background: rgb(246, 237, 144);"
                    "border-radius: 10px;"
                "}"
                );
    tbn_apply ->setStyleSheet(
                "QToolButton{"
                    "background: rgb(246, 237, 144);"
                    "border-radius: 10px;"
                "}"
                );

    //设置 按钮文字
    tbn_ok ->setText(tr("确定"));
    tbn_cancle ->setText(tr("取消"));
    tbn_apply ->setText(tr("应用"));

    //设置 图标提示
    tbn_theme1 ->setToolTip(tr("点击预览"));
    tbn_theme2 ->setToolTip(tr("点击预览"));
    tbn_theme3 ->setToolTip(tr("点击预览"));
    tbn_theme4 ->setToolTip(tr("点击预览"));
    tbn_theme5 ->setToolTip(tr("点击预览"));
    tbn_theme6 ->setToolTip(tr("点击预览"));
    tbn_theme7 ->setToolTip(tr("点击预览"));
    tbn_theme8 ->setToolTip(tr("点击预览"));
    tbn_theme9 ->setToolTip(tr("点击预览"));

    //信号 槽
    connect(tbn_close, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(tbn_ok, SIGNAL(clicked()),
            this, SLOT(tbn_ok_clicked()));
    connect(tbn_cancle, SIGNAL(clicked()),
            this, SLOT(tbn_cancle_clicked()));
    connect(tbn_apply, SIGNAL(clicked()),
            this, SLOT(tbn_apply_clicked()));

    connect(tbn_theme1, SIGNAL(clicked()),
            this, SLOT(theme1Clicked()));
    connect(tbn_theme2, SIGNAL(clicked()),
            this, SLOT(theme2Clicked()));
    connect(tbn_theme3, SIGNAL(clicked()),
            this, SLOT(theme3Clicked()));
    connect(tbn_theme4, SIGNAL(clicked()),
            this, SLOT(theme4Clicked()));
    connect(tbn_theme5, SIGNAL(clicked()),
            this, SLOT(theme5Clicked()));
    connect(tbn_theme6, SIGNAL(clicked()),
            this, SLOT(theme6Clicked()));
    connect(tbn_theme7, SIGNAL(clicked()),
            this, SLOT(theme7Clicked()));
    connect(tbn_theme8, SIGNAL(clicked()),
            this, SLOT(theme8Clicked()));
    connect(tbn_theme9, SIGNAL(clicked()),
            this, SLOT(theme9Clicked()));
}

//重写鼠标按下事件
void Theme::mousePressEvent(QMouseEvent *event)
{
    offset = event ->globalPos() - pos();
}

//重写鼠标移动事件
void Theme::mouseMoveEvent(QMouseEvent *event)
{
    QPoint temp = event ->globalPos() - offset;
    move(temp);
}

//事件过滤器
bool Theme::eventFilter(QObject *target, QEvent *event)
{
    if(target == tbn_close
            || target == tbn_theme1
            || target == tbn_theme2
            || target == tbn_theme3
            || target == tbn_theme4
            || target == tbn_theme5
            || target == tbn_theme6
            || target == tbn_theme7
            || target == tbn_theme8
            || target == tbn_theme9
            || target == tbn_ok
            || target == tbn_cancle
            || target == tbn_apply)
    {
        if (event ->type() == QEvent::MouseMove)
        {
//            event ->accept();
        }
        else
        {
            return QWidget::eventFilter(target, event);
        }
    }
}

/*
//旧主题状态
void Theme::oldThemeState(int theme)
{
    switch(theme)
    {
    case 1: oldTheme = 1; break;
    case 2: oldTheme = 2; break;
    case 3: oldTheme = 3; break;
    case 4: oldTheme = 4; break;
    case 5: oldTheme = 5; break;
    case 6: oldTheme = 6; break;
    case 7: oldTheme = 7; break;
    case 8: oldTheme = 8; break;
    case 9: oldTheme = 9; break;
    default: oldTheme = 1; qDebug() << "unknown oldTheme"; break;
    }

}
*/

//主题1 被单击
void Theme::theme1Clicked()
{
    emit themeClick(1);
    newTheme = 1;
}

//主题2 被单击
void Theme::theme2Clicked()
{
    emit themeClick(2);
    newTheme = 2;
}

//主题3 被单击
void Theme::theme3Clicked()
{
    emit themeClick(3);
    newTheme = 3;
}

//主题4 被单击
void Theme::theme4Clicked()
{
    emit themeClick(4);
    newTheme = 4;
}

//主题5 被单击
void Theme::theme5Clicked()
{
    emit themeClick(5);
    newTheme = 5;
}

//主题6 被单击
void Theme::theme6Clicked()
{
    emit themeClick(6);
    newTheme = 6;
}

//主题7 被单击
void Theme::theme7Clicked()
{
    emit themeClick(7);
    newTheme = 7;
}

//主题8 被单击
void Theme::theme8Clicked()
{
    emit themeClick(8);
    newTheme = 8;
}

//主题9 被单击
void Theme::theme9Clicked()
{
    emit themeClick(9);
    newTheme = 9;
}

//单击 “确定” 关闭窗口， 将新主题写入配置文件
void Theme::tbn_ok_clicked()
{
    //关闭 设置主题窗口
    close();

    //发送信号，将新主题写入配置文件
    emit settingDataChanged("theme", tr("%1").arg(newTheme));
/*
    //写入配置文件
    QStringList oldDataes;
    bool data_exist;
//    QString
    QFile writeTheme(".data.ini");
    if (writeTheme.open(QIODevice::ReadWrite))
    {
        //读取配置文件
        QTextStream in(&writeTheme);
        while (! in.atEnd())
        {
            oldDataes.append(in.readLine());
        }

        //拆分配置文件字符串
        for (int i=0; i<oldDataes.length(); i++)
        {
            QStringList line = oldDataes[i].split(":");
            if (line[0] == tr("theme"))
            {
                data_exist = true;
                oldDataes[i] = tr("theme:%1").arg(newTheme);
                break;
            }
            if (i == oldDataes.length() - 1)
            {
                if (line[0] != tr("theme"))
                {
                    data_exist = false;             //theme数据不存在
                }
            }
        }

        //重新写入数据
        writeTheme.close();
        if (data_exist)                             //如果theme项存在，修改theme值
        {
            if (writeTheme.open(QIODevice::WriteOnly))
            {
                for (int i=0; i<oldDataes.length(); i++)
                {
                    writeTheme.write(oldDataes[i].toUtf8() + "\n");
                }
            }
        }
        else                                        //如果theme项不存在，添加
        {
            if (writeTheme.open(QIODevice::Append))
            {
                writeTheme.write(tr("theme:%1\n").arg(newTheme).toUtf8());
            }
        }

        //关闭文件
        writeTheme.close();
    }
    */
}

//单击 “取消”
void Theme::tbn_cancle_clicked()
{
    close();
    emit themeClick(oldTheme);
}

//单击 “应用”
void Theme::tbn_apply_clicked()
{

}
