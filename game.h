#ifndef GAME_H
#define GAME_H

#include <QWidget>
class QLabel;
class QGridLayout;
class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = 0, int width = 297, int height = 297);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QLabel *lab_11;
    QLabel *lab_12;
    QLabel *lab_13;
    QLabel *lab_14;
    QLabel *lab_21;
    QLabel *lab_22;
    QLabel *lab_23;
    QLabel *lab_24;
    QLabel *lab_31;
    QLabel *lab_32;
    QLabel *lab_33;
    QLabel *lab_34;
    QLabel *lab_41;
    QLabel *lab_42;
    QLabel *lab_43;
    QLabel *lab_44;
    QLabel *lab_new;

    QGridLayout *layout;

signals:

public slots:

};

#endif // GAME_H
