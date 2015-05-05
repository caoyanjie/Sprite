#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
class QPushButton;
class QLabel;
class QTabWidget;
class QKeySequenceEdit;
class QRadioButton;
class QLineEdit;

class Setting : public QWidget
{
    Q_OBJECT
public:
    explicit Setting(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);      //事件过滤器

private:
    QLabel *lab_background;
    QPushButton *pbn_close;
    QTabWidget *tabWidget;

    QWidget *tab_shortcutKey;               //什么东西？？？？？？
    QWidget *tab_behaviour;

    QLabel *lab_play_previous;
    QLabel *lab_play_pause;
    QLabel *lab_play_next;
    QLabel *lab_volume_reduce;
    QLabel *lab_volume_add;
    QLabel *lab_addMusic;

    QKeySequenceEdit *key_play_previous;
    QKeySequenceEdit *key_play_pause;
    QKeySequenceEdit *key_play_next;
    QKeySequenceEdit *key_volume_reduce;
    QKeySequenceEdit *key_volume_add;
    QKeySequenceEdit *key_addMusic;

    //第二个面板
    QLabel *lab_closeMethod;    //关闭窗口方式
    QRadioButton *rbn_close;
    QRadioButton *rbn_mini;

    QLabel *lab_lrc;            //歌词样式
    QLabel *lab_lrcSize;           //歌词 字体大小
    QLineEdit *ln_lrcSize;          //...........

    QLabel *lab_lrcColor;        //歌词 字体颜色
    QLineEdit *ln_lrcColor;      //...........

    QPushButton *pbn_ok;
    QPushButton *pbn_cancle;
    QPushButton *pbn_default;

    QPoint offset;

signals:

public slots:

};

#endif // SETTING_H
