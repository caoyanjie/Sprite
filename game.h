#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "gamepiece.h"
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
    enum Forward{
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    void produceChessPiece();                   // 产生新子
    void moveGamer(Forward moveForwad);         // 移动棋子
    void moveChessPiece(Forward forward);                      // 移动棋子
    void moveLeft();                            // 左移
    void moveRight();                           // 右移
    void moveUp();                              // 上移
    void moveDown();                            // 下移

    static const unsigned int LINES = 4;       // 棋盘格数

    QLabel *labBg[LINES][LINES];
    GamePiece *chessboardGrid[LINES][LINES];

    QGridLayout *layout;

signals:

public slots:

};

#endif // GAME_H
