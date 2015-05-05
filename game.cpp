#include "game.h"
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>

Game::Game(QWidget *parent, int width, int height) :
    QWidget(parent)
{
    lab_11 = new QLabel(this);
    lab_12 = new QLabel(this);
    lab_13 = new QLabel(this);
    lab_14 = new QLabel(this);
    lab_21 = new QLabel(this);
    lab_22 = new QLabel(this);
    lab_23 = new QLabel(this);
    lab_24 = new QLabel(this);
    lab_31 = new QLabel(this);
    lab_32 = new QLabel(this);
    lab_33 = new QLabel(this);
    lab_34 = new QLabel(this);
    lab_41 = new QLabel(this);
    lab_42 = new QLabel(this);
    lab_43 = new QLabel(this);
    lab_44 = new QLabel(this);
    lab_new = new QLabel("2", this);

    layout = new QGridLayout;
    layout->addWidget(lab_11, 0, 0, 1, 1);
    layout->addWidget(lab_12, 0, 1, 1, 1);
    layout->addWidget(lab_13, 0, 2, 1, 1);
    layout->addWidget(lab_14, 0, 3, 1, 1);
    layout->addWidget(lab_21, 1, 0, 1, 1);
    layout->addWidget(lab_22, 1, 1, 1, 1);
    layout->addWidget(lab_23, 1, 2, 1, 1);
    layout->addWidget(lab_24, 1, 3, 1, 1);
    layout->addWidget(lab_31, 2, 0, 1, 1);
    layout->addWidget(lab_32, 2, 1, 1, 1);
    layout->addWidget(lab_33, 2, 2, 1, 1);
    layout->addWidget(lab_34, 2, 3, 1, 1);
    layout->addWidget(lab_41, 3, 0, 1, 1);
    layout->addWidget(lab_42, 3, 1, 1, 1);
    layout->addWidget(lab_43, 3, 2, 1, 1);
    layout->addWidget(lab_44, 3, 3, 1, 1);
    layout->setSpacing(5);
    layout->setMargin(5);
    setLayout(layout);

    lab_new->resize(68, 68);
    lab_new->move(5, 5);
    lab_new->setAlignment(Qt::AlignCenter);

    resize(width, height);

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
        if (lab_new->x() == 5)
        {
            return;
        }
        lab_new->move(lab_new->x()-lab_new->width()-5, lab_new->y());
    }
    else if (event->key() == Qt::Key_D)
    {
        if (lab_new->x()+lab_new->width()+5 == this->width())
        {
            return;
        }
        lab_new->move(lab_new->x()+lab_new->width()+5, lab_new->y());
    }
    else if (event->key() == Qt::Key_W)
    {
        if (lab_new->y()-5 == 0)
        {
            return;
        }
        lab_new->move(lab_new->x(), lab_new->y()-lab_new->height()-5);
    }
    else if (event->key() == Qt::Key_S)
    {
        if (lab_new->y()+lab_new->height()+5 == this->height())
        {
            return;
        }
        lab_new->move(lab_new->x(), lab_new->y()+lab_new->height()+5);
    }
}
