#include "game.h"
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QMessageBox>

Game::Game(QWidget *parent, int width, int height) :
    QWidget(parent)
{
    this->resize(width, height);


    layout = new QGridLayout;
    layout->setSpacing(5);
    layout->setMargin(5);
    this->setLayout(layout);

    for (int i=0; i<LINES; ++i)
    {
        for (int j=0; j<LINES; ++j)
        {
            chessboardGrid[i][j] = nullptr;
            labBg[i][j] = new QLabel;
            layout->addWidget(labBg[i][j], i, j, 1, 1);
        }
    }

    this->setStyleSheet(
                "QLabel, Gamer{"
                    "background: rgba(0, 0, 0, 80);"
                    "border-radius: 5px;"
//                    "font-size: 40pt;"
                "}"
                );

    //初始化随机数
    QTime t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);

    //产生第一个初始化后的子
    int line = qrand() % LINES;
    int column = qrand() % LINES;qDebug() << tr("生成棋子在第%1行第%2列").arg(line).arg(column);
    chessboardGrid[line][column] = new GamePiece(this);
    chessboardGrid[line][column]->move(column*(68+5)+5, line*(68+5)+5);
}

//键盘事件
void Game::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_A:
    case Qt::Key_Left:
        moveGamer(LEFT);
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        moveGamer(RIGHT);
        break;
    case Qt::Key_W:
    case Qt::Key_Up:
        moveGamer(UP);
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        moveGamer(DOWN);
        break;
    }
}

// 产生新子
void Game::produceChessPiece()
{
    bool isEmptyGrid = false;

    // 标记所有没有子的空格
    QVector<unsigned int> lineEmpty[LINES];
    for (int line=0; line<LINES; ++line)
    {
        for (int column=0; column<LINES; ++column)
        {
            if (!chessboardGrid[line][column])
            {
                lineEmpty[line].push_back(column);
                isEmptyGrid = true;
            }
        }
    }

    // 判断是否全部填满
    if (!isEmptyGrid)
    {
        QMessageBox::warning(0, tr("真遗憾！"), tr("游戏结束！"), QMessageBox::Ok);
        return;
    }

    // 随机选取一个空着的格子
    int line, column;
    while (true)
    {
        line = qrand() % LINES;
        if (lineEmpty[line].isEmpty())
        {
            continue;
        }
        column = lineEmpty[line].at( qrand() % lineEmpty[line].size() );
        break;
    }

    // 创建新子
    chessboardGrid[line][column] = new GamePiece(this);
    chessboardGrid[line][column]->move(labBg[line][column]->pos());
    chessboardGrid[line][column]->show();
    if (chessboardGrid[line][column])
    {
        chessboardGrid[line][column]->setHavePlused(false);
    }
}

//移动
void Game::moveGamer(Forward moveForwad)
{
    switch(moveForwad)
    {
    case LEFT:
        moveLeft();
        break;
    case RIGHT:
        moveChessPiece(RIGHT);
        break;
    case UP:
        moveUp();
        break;
    case DOWN:
        moveDown();
        break;
    }
    produceChessPiece();
}

void Game::moveChessPiece(Forward forward)
{
    for (int i=0; i<LINES; ++i)                 // 每一行都移动
    {
        for (int j=1;  j<LINES; ++j)            // 每一行中的第 j 列
        {
            int line, column, offset, tmpLine, tmpColumn, bgTmpLine, bgTmpColumn;
            switch (forward)
            {
            case LEFT:                                      // 左移
            case RIGHT:                                     // 右移
                line = i;                                   // 外层循环(行) 0, 1, 2, 3
                column = forward == LEFT ? j : LINES-1-j;   // 内层循环(列) 1, 2, 3 (2, 1, 0)
                offset = forward == LEFT ? 1 : -1;
                tmpLine = line;
                tmpColumn = column - offset;
                bgTmpLine = line;
       //         bgTmpColumn = tmpColumn + offset;
                break;
            case UP:                                        // 上移
            case DOWN:                                      // 下移
                line = forward == UP ? j : LINES-1-j;       // 内层循环(行) 2, 1, 0 (1, 2, 3)
                column = i;                                 // 外层循环(列) 0, 1, 2, 3
                offset = forward == UP ? 1 : -1;
                tmpLine = line - offset;
                tmpColumn = column;
       //         bgTmpLine = tmpLine + offset;
                bgTmpColumn = column;
                break;
            }qDebug() << tr("操作第%1行 第%2列").arg(line).arg(column);

            // 空格子跳过
            if (!chessboardGrid[line][column])
            {qDebug() << tr("空格子跳过");
                continue;
            }

            // 找前边第一个非空格子
            qDebug() << tr("遇到棋子");
            while (!chessboardGrid[tmpLine][tmpColumn])
            {qDebug() << tr("寻找第%1行第%2列").arg(line).arg(tmpColumn);
                qDebug() << "line: " << line;
                switch (forward)
                {
                case LEFT:
                case RIGHT:
                    if (!(tmpColumn>=0 && tmpColumn<=LINES-1))
                        goto ai;
                    break;
                case UP:
                case DOWN:
                    if (!(tmpLine>=0 && tmpLine<LINES-1))
                        goto ai;
                    break;
                }
                if ((forward==LEFT || forward==RIGHT) && (tmpColumn == 0 || tmpColumn == LINES-1) || (forward==UP || forward==DOWN) && (tmpLine == 0 || tmpLine == LINES-1)) // 前边找不到非空格子
                {   qDebug() << "前边没找到棋子";
                    qDebug() << tr("将要第%1行第%2列移动到了第%3行第%4列").arg(line).arg(column).arg(tmpLine).arg(tmpColumn);
                    chessboardGrid[line][column]->move(labBg[tmpLine][tmpColumn]->pos());
                    chessboardGrid[tmpLine][tmpColumn] = chessboardGrid[line][column];
                    chessboardGrid[line][column] = nullptr;
                }
                forward == LEFT || forward == RIGHT ? tmpColumn -= offset : tmpLine -= offset;
            }

            // 前边找到非空格子
ai:            if ((forward==LEFT || forward==RIGHT) &&(tmpColumn>-1 && tmpColumn<LINES) || (forward==UP || forward==DOWN) &&(tmpLine>-1 && tmpLine<LINES))
            {qDebug() << tr("第%1行第%2列处找到棋子").arg(tmpLine).arg(tmpColumn);
                int frontNum = chessboardGrid[tmpLine][tmpColumn]->text().toInt();
                int currentNum = chessboardGrid[line][column]->text().toInt();
                if (frontNum == currentNum)
                {qDebug() << "数字相等";
                    chessboardGrid[tmpLine][tmpColumn]->setTextNum(tr("%1").arg(frontNum+currentNum));
                    chessboardGrid[tmpLine][tmpColumn]->setHavePlused(true);
                    chessboardGrid[line][column]->deleteLater();
                    chessboardGrid[line][column] = nullptr;
                }
                else if ((forward==LEFT || forward==RIGHT) && (column != tmpColumn+offset) || (forward==UP || forward==DOWN) && (bgTmpLine = tmpLine + offset))
                {qDebug() << "数字不等";
                    forward == LEFT || forward == RIGHT ? bgTmpColumn = tmpColumn + offset : bgTmpLine = tmpLine + offset;
                    chessboardGrid[line][column]->move(labBg[bgTmpLine][bgTmpColumn]->pos());qDebug() << tr("第%1行第%2列移动到了第%3行第%4列").arg(line).arg(column).arg(bgTmpLine).arg(bgTmpColumn);
                    chessboardGrid[bgTmpLine][bgTmpColumn] = chessboardGrid[line][column];
                    chessboardGrid[line][column] = nullptr;
                }
            }//if
        }//for
        for (int j=0; j<LINES; ++j)
        {
            if (chessboardGrid[i][j])
            {qDebug() << tr("第%1行第%2列有棋子").arg(i).arg(j);
                chessboardGrid[i][j]->setHavePlused(false);
            }
        }
        qDebug() << "\n";
    }//for
}

// 左移
void Game::moveLeft()
{
    for (int i=0; i<LINES; ++i)                 // 每一行都移动
    {
        for (int j=1;  j<LINES; ++j)            // 每一行中的第 j 列
        {
            int line = i;
            int column = j;
            int offset = 1;
            int tmpLine = line;
            int tmpColumn = column - offset;//, bgTmpLine, bgTmpColumn;

            // 空格子跳过
            if (!chessboardGrid[line][column])
            {
                continue;
            }

            // 找前边第一个非空格子
            for (; column>=0 && column<=LINES-1 && !chessboardGrid[tmpLine][tmpColumn]; tmpColumn-=offset)
            {
                if (tmpColumn == 0) // 前边找不到非空格子
                {
                    chessboardGrid[line][column]->move(labBg[tmpLine][tmpColumn]->pos());
                    chessboardGrid[tmpLine][tmpColumn] = chessboardGrid[line][column];
                    chessboardGrid[line][column] = nullptr;
                }
            }

            // 前边找到非空格子
            if (tmpColumn > -1)
            {
                int frontNum = chessboardGrid[tmpLine][tmpColumn]->text().toInt();
                int currentNum = chessboardGrid[line][column]->text().toInt();
                if (frontNum == currentNum)
                {
                    chessboardGrid[tmpLine][tmpColumn]->setTextNum(tr("%1").arg(frontNum+currentNum));
                    chessboardGrid[tmpLine][tmpColumn]->setHavePlused(true);
                    chessboardGrid[line][column]->deleteLater();
                    chessboardGrid[line][column] = nullptr;
                }
                else if (column != tmpColumn+offset)
                {
                    chessboardGrid[line][column]->move(labBg[line][tmpColumn+offset]->pos());
                    chessboardGrid[line][tmpColumn+offset] = chessboardGrid[line][column];
                    chessboardGrid[line][column] = nullptr;
                }
            }//if
        }//for
        for (int j=0; j<LINES; ++j)
        {
            if (chessboardGrid[i][j])
            {
                chessboardGrid[i][j]->setHavePlused(false);
            }
        }
    }//for
}

// 右移
void Game::moveRight()
{
    for (int i=0; i<LINES; ++i)                 // 每一行都移动
    {
        for (int j=1;  j<LINES; ++j)            // 每一行中的第 j 列
        {
            int line = i;
            int column = j;
            int offset = -1;
            int tmpLine = line;
            int tmpColumn = column - offset;//, bgTmpLine, bgTmpColumn;

            // 空格子跳过
            if (!chessboardGrid[line][column])
            {
                continue;
            }

            // 找前边第一个非空格子
            for (; column>=0 && column<=LINES-1 && !chessboardGrid[tmpLine][tmpColumn]; tmpColumn-=offset)
            {
                if (tmpColumn == LINES-1) // 前边找不到非空格子
                {
                    chessboardGrid[line][column]->move(labBg[tmpLine][tmpColumn]->pos());
                    chessboardGrid[tmpLine][tmpColumn] = chessboardGrid[line][column];
                    chessboardGrid[line][column] = nullptr;
                }
            }

            // 前边找到非空格子
            if (tmpColumn > -1 && tmpColumn <LINES)
            {
                int frontNum = chessboardGrid[tmpLine][tmpColumn]->text().toInt();
                int currentNum = chessboardGrid[line][column]->text().toInt();
                if (frontNum == currentNum)
                {
                    chessboardGrid[tmpLine][tmpColumn]->setTextNum(tr("%1").arg(frontNum+currentNum));
                    chessboardGrid[tmpLine][tmpColumn]->setHavePlused(true);
                    chessboardGrid[line][column]->deleteLater();
                    chessboardGrid[line][column] = nullptr;
                }
                else if (column != tmpColumn+offset)
                {
                    chessboardGrid[line][column]->move(labBg[line][tmpColumn+offset]->pos());
                    chessboardGrid[line][tmpColumn+offset] = chessboardGrid[line][column];
                    chessboardGrid[line][column] = nullptr;
                }
            }//if
        }//for
        for (int j=0; j<LINES; ++j)
        {
            if (chessboardGrid[i][j])
            {
                chessboardGrid[i][j]->setHavePlused(false);
            }
        }
    }//for
}

// 上移
void Game::moveUp()
{
    for (int i=0; i<LINES; ++i)                 // 每一行都移动
    {
        for (int j=1;  j<LINES; ++j)            // 每一行中的第 j 列
        {
            int line = j;
            int column = i;
            int offset = 1;
            int tmpLine = line - offset;
            int tmpColumn = column;
//            int bgTmpLine = line;
//            int bgTmpColumn = tmpLine + offset;

            // 空格子跳过
            if (!chessboardGrid[line][column])
            {
                continue;
            }

            // 找前边第一个非空格子
            for (; tmpLine>=0 && tmpLine<=LINES-1 && !chessboardGrid[tmpLine][tmpColumn]; tmpLine-=offset)
            {
                if (tmpLine == 0) // 前边找不到非空格子
                {
                    chessboardGrid[line][column]->move(labBg[tmpLine][tmpColumn]->pos());
                    chessboardGrid[tmpLine][tmpColumn] = chessboardGrid[line][column];
                    chessboardGrid[line][column] = nullptr;
                }
            }

            // 前边找到非空格子
            if (tmpLine > -1)
            {
                int frontNum = chessboardGrid[tmpLine][tmpColumn]->text().toInt();
                int currentNum = chessboardGrid[line][column]->text().toInt();
                if (frontNum == currentNum)
                {
                    chessboardGrid[tmpLine][tmpColumn]->setTextNum(tr("%1").arg(frontNum+currentNum));
                    chessboardGrid[tmpLine][tmpColumn]->setHavePlused(true);
                    chessboardGrid[line][column]->deleteLater();
                    chessboardGrid[line][column] = nullptr;
                }
                else if (line != tmpLine+offset)
                {
                    int bgTmpLine = tmpLine + offset;
                    int bgTmpColumn = column;
                    chessboardGrid[line][column]->move(labBg[bgTmpLine][bgTmpColumn]->pos());
                    chessboardGrid[bgTmpLine][bgTmpColumn] = chessboardGrid[line][column];
                    chessboardGrid[line][column] = nullptr;
                }
            }//if
        }//for
        for (int j=0; j<LINES; ++j)
        {
            if (chessboardGrid[i][j])
            {
                chessboardGrid[i][j]->setHavePlused(false);
            }
        }
    }//for
}

// 下移
void Game::moveDown()
{
    for (int i=0; i<LINES; ++i)                 // 每一行都移动
    {
        for (int j=1;  j<LINES; ++j)            // 每一行中的第 j 列
        {
            int line = LINES -1 - j;
            int column = i;
            int offset = -1;
            int tmpLine = line - offset;
            int tmpColumn = column;
//            int bgTmpLine = line;
//            int bgTmpColumn = tmpLine + offset;

            // 空格子跳过
            if (!chessboardGrid[line][column])
            {
                continue;
            }

            // 找前边第一个非空格子
            for (; tmpLine>=0 && tmpLine<=LINES-1 && !chessboardGrid[tmpLine][tmpColumn]; tmpLine-=offset)
            {
                if (tmpLine == LINES-1) // 前边找不到非空格子
                {
                    chessboardGrid[line][column]->move(labBg[tmpLine][tmpColumn]->pos());
                    chessboardGrid[tmpLine][tmpColumn] = chessboardGrid[line][column];
                    chessboardGrid[line][column] = nullptr;
                }
            }

            // 前边找到非空格子
            if (tmpLine > -1 && tmpLine<LINES)
            {
                int frontNum = chessboardGrid[tmpLine][tmpColumn]->text().toInt();
                int currentNum = chessboardGrid[line][column]->text().toInt();
                if (frontNum == currentNum)
                {
                    chessboardGrid[tmpLine][tmpColumn]->setTextNum(tr("%1").arg(frontNum+currentNum));
                    chessboardGrid[tmpLine][tmpColumn]->setHavePlused(true);
                    chessboardGrid[line][column]->deleteLater();
                    chessboardGrid[line][column] = nullptr;
                }
                else if (line != tmpLine+offset)
                {
                    int bgTmpLine = tmpLine + offset;
                    int bgTmpColumn = column;
                    chessboardGrid[line][column]->move(labBg[bgTmpLine][bgTmpColumn]->pos());
                    chessboardGrid[bgTmpLine][bgTmpColumn] = chessboardGrid[line][column];
                    chessboardGrid[line][column] = nullptr;
                }
            }//if
        }//for
        for (int j=0; j<LINES; ++j)
        {
            if (chessboardGrid[i][j])
            {
                chessboardGrid[i][j]->setHavePlused(false);
            }
        }
    }//for
}
