#ifndef GAMEPIECE_H
#define GAMEPIECE_H

#include <QLabel>
#include <QTime>

class GamePiece : public QLabel
{
    Q_OBJECT
public:
    explicit GamePiece(QWidget *parent = 0);

    bool havePlused();
    void setHavePlused(bool val);
    void setTextNum(const QString &val);

    int line;
    int column;

private:
    bool leftIsEmpty;
    bool rightIsEmpty;
    bool upIsEmpty;
    bool downIsEmpty;
    bool plusedDone;

signals:

public slots:

};

#endif // GAMEPIECE_H
