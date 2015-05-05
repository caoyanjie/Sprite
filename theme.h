#ifndef THEME_H
#define THEME_H

#include <QWidget>
class QLabel;
class QToolButton;

class Theme : public QWidget
{
    Q_OBJECT
public:
    explicit Theme(QWidget *parent = 0);

    int oldTheme;               //配置文件中的旧主题

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *, QEvent *);
    QPoint offset;

private:
    QLabel *lab_background;

    QToolButton *tbn_close;
    QToolButton *tbn_theme1;
    QToolButton *tbn_theme2;
    QToolButton *tbn_theme3;
    QToolButton *tbn_theme4;
    QToolButton *tbn_theme5;
    QToolButton *tbn_theme6;
    QToolButton *tbn_theme7;
    QToolButton *tbn_theme8;
    QToolButton *tbn_theme9;
    QToolButton *tbn_ok;
    QToolButton *tbn_cancle;
    QToolButton *tbn_apply;

    int newTheme;               //将新设置的主题保存

signals:
    void themeClick(int);
    void settingDataChanged(QString, QString);   //设置信息改变，写入配置文件

public slots:
//    void oldThemeState(int);

    void theme1Clicked();
    void theme2Clicked();
    void theme3Clicked();
    void theme4Clicked();
    void theme5Clicked();
    void theme6Clicked();
    void theme7Clicked();
    void theme8Clicked();
    void theme9Clicked();

    void tbn_ok_clicked();
    void tbn_cancle_clicked();
    void tbn_apply_clicked();
};

#endif // THEME_H
