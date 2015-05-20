#ifndef GAMER_H
#define GAMER_H

#include <QLabel>

class Gamer : public QLabel
{
    Q_OBJECT
public:
    explicit Gamer(QWidget *parent = 0);
    int line;
    int column;
    int number;

signals:

public slots:

};

#endif // GAMER_H
