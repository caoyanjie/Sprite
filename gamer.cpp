#include "gamer.h"

#include <QDebug>

Gamer::Gamer(QWidget *parent) :
    QLabel(parent)
{
    this->resize(68, 68);
    QTime t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    int number;
    while(1)
    {
        number = qrand()%(5-2)+2;
        if (number % 2 == 0)
            break;
    }
    this->setText(tr("%1").arg(number));
    this->setAlignment(Qt::AlignCenter);

    connect(this, SIGNAL(destroyed(QObject*)), SLOT(destroyed()));
}
