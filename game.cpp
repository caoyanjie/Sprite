#include "game.h"
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>

Game::Game(QWidget *parent, int width, int height) :
    QWidget(parent)
{
    this->resize(width, height);

    layout = new QGridLayout;
    layout->setSpacing(5);
    layout->setMargin(5);
    this->setLayout(layout);

    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<4; ++j)
        {
            labBg[i][j] = new QLabel;
            labGame[i][j] = new QLabel;
            layout->addWidget(labBg[i][j], i, j, 1, 1);
        }
    }
    newGamer = new Gamer(this);
    newGamer->setText("2");
    newGamer->resize(68, 68);
    newGamer->move(5, 5);
    newGamer->setAlignment(Qt::AlignCenter);


    setStyleSheet(
                "QLabel{"
                    "background: rgba(0, 0, 0, 80);"
                    "border-radius: 5px;"
                    "font-size: 40pt;"
                "}"
                );
}

//键盘事件
void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A)
    {qDebug() << "left";
        if (newGamer->x() == 5)
        {
            return;
        }
        newGamer->move(newGamer->x()-newGamer->width()-5, newGamer->y());
    }
    else if (event->key() == Qt::Key_D)
    {
        if (newGamer->x()+newGamer->width()+5 == this->width())
        {
            return;
        }
        newGamer->move(newGamer->x()+newGamer->width()+5, newGamer->y());
    }
    else if (event->key() == Qt::Key_W)
    {
        if (newGamer->y()-5 == 0)
        {
            return;
        }
        newGamer->move(newGamer->x(), newGamer->y()-newGamer->height()-5);
    }
    else if (event->key() == Qt::Key_S)
    {
        if (newGamer->y()+newGamer->height()+5 == this->height())
        {
            return;
        }
        newGamer->move(newGamer->x(), newGamer->y()+newGamer->height()+5);
    }
}
