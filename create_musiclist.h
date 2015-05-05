#ifndef CREATE_MUSICLIST_H
#define CREATE_MUSICLIST_H

#include <QLabel>
class QLineEdit;
class QPushButton;

class Create_MusicList : public QLabel
{
    Q_OBJECT
public:
    explicit Create_MusicList(QWidget *parent = 0);

protected:
    QLineEdit *ln_name;
    QPushButton *pbn_ok;
    QPushButton *pbn_cancle;

signals:
    void pbn_ok_click(QString);

public slots:
    void pbn_ok_clicked();

};

#endif // CREATE_MUSICLIST_H
