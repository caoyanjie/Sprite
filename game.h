#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "gamer.h"
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
    enum forward{LEFT, RIGHT, UP, DOWN};
    void produce();
    void moveGamer(forward moveForwad);

    QLabel *labBg[4][4];
    Gamer *flagGamer[4][4];
    Gamer *newGamer;

    QGridLayout *layout;

signals:

public slots:

};

#endif // GAME_H
