#include "gamepiece.h"

GamePiece::GamePiece(QWidget *parent) :
    QLabel(parent)
  ,plusedDone(false)
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
    this->setTextNum(tr("%1").arg(number));
    this->setAlignment(Qt::AlignCenter);
}

bool GamePiece::havePlused()
{
    return plusedDone;
}

void GamePiece::setHavePlused(bool val)
{
    plusedDone = val;
}

void GamePiece::setTextNum(const QString &val)
{
    int num = val.toInt();
    if (num > 999)
    {
        this->setStyleSheet("QLabel{font-size:25pt;}");
    }
    else if (num > 99)
    {
        this->setStyleSheet("QLabel{font-size:30pt;}");
    }
    else
    {
        this->setStyleSheet("QLabel{font-size:35pt;}");
    }

    this->setText(val);
}
