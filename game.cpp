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

    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<4; ++j)
        {
            flagGamer[i][j] = 0;
            labBg[i][j] = new QLabel;
            layout->addWidget(labBg[i][j], i, j, 1, 1);
        }
    }

    this->setStyleSheet(
                "QLabel, Gamer{"
                    "background: rgba(0, 0, 0, 80);"
                    "border-radius: 5px;"
                    "font-size: 40pt;"
                "}"
                );

    //初始化随机数
    QTime t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);

    //产生第一个初始化后的子
    int line = qrand() % 4;
    int column = qrand() % 4;
    newGamer = new Gamer(this);
    newGamer->move(column*(68+5)+5, line*(68+5)+5);
    flagGamer[line][column] = newGamer;
}

//键盘事件
void Game::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_A:
        qDebug() << "left";
        moveGamer(LEFT);
        break;
    case Qt::Key_D:
        moveGamer(RIGHT);
        break;
    case Qt::Key_W:
        moveGamer(UP);
        break;
    case Qt::Key_S:
        moveGamer(DOWN);
        break;
    }
}

//产生新子
void Game::produce()
{
    QVector<int> empty[4];
    int line;
    int column;
    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<4; ++j)
        {
            if (flagGamer[i][j] == 0)
            {
                empty[i].append(j);
            }
        }
    }
    if (empty[0].isEmpty() && empty[1].isEmpty() && empty[2].isEmpty() && empty[3].isEmpty())
    {
        QMessageBox::warning(0, tr("真遗憾！"), tr("游戏结束！"), QMessageBox::Ok);
        return;
    }
    while(1)
    {
        line = qrand() % 4;
        if (!empty[line].isEmpty())
        {
            break;
        }
    }
    column = empty[line].at(qrand() % empty[line].count());
    newGamer = new Gamer(this);
    newGamer->move(labBg[line][column]->pos());
    newGamer->show();
    flagGamer[line][column] = newGamer;
}

//移动
void Game::moveGamer(forward moveForwad)
{
    QPoint targetPos[4] = {QPoint(0, 0), QPoint(0, 0), QPoint(0, 0), QPoint(0, 0)};

    switch(moveForwad)
    {
    case LEFT:
        qDebug() << "\n\n\n";
        for (int i=0; i<4; ++i)                 //第 i 行
        {qDebug() << "检测第" << i << "行";
            QVector<Gamer*> numbers;            //存放一行中的棋子
            QVector<int> columnEmpty;           //存放一行中的空列
            for (int j=0; j<4; ++j)             //第 j 列
            {qDebug() << "    检测第" << j << "列";
                if (flagGamer[i][j] == 0)
                {qDebug() << "      1个空格,已追加到columnEmpty中";
                    columnEmpty.append(j);      //把空列追加到向量中
                }
                else                            //遇到棋子
                {qDebug() << "      检测到棋子";
                    if (!numbers.isEmpty())     //如果前边有棋子
                    {qDebug() << "            前边有" << numbers.count() << "个棋子";
                        int frontNumber = numbers.at(numbers.count()-1)->text().toInt();
                        int currentNumber = flagGamer[i][j]->text().toInt();
                        if (frontNumber == currentNumber)   //数字相等，相加
                        {qDebug() << "            棋子与前边棋子相等，和为" << frontNumber + currentNumber;
                            int numberCount = frontNumber + currentNumber;
                            numbers.at(numbers.count()-1)->setText(tr("%1").arg(numberCount));
                            flagGamer[i][j]->deleteLater();
                            flagGamer[i][j] = 0;
                    //        numbers.pop_back();j=-1;
                            numbers.clear();columnEmpty.clear();j=-1;
                            continue;
                        }
                    }
                    if (!columnEmpty.isEmpty()) //如果前边有空列
                    {
                        flagGamer[i][j]->move(labBg[i][columnEmpty.at(0)]->pos());
                        flagGamer[i][columnEmpty.at(0)] = flagGamer[i][j];
                        flagGamer[i][j] = 0;
                        numbers.append(flagGamer[i][columnEmpty.at(0)]);
                        columnEmpty.pop_front();
                        columnEmpty.append(j);
                    }
                    else
                    {qDebug() << "          前边既没棋子又没空格，已追加到numbers";
                        numbers.append(flagGamer[i][j]);
                    }
                }
            }
        }
        break;
    case RIGHT:
        qDebug() << "\n\n\n";
        for (int i=0; i<4; ++i)                 //第 i 行
        {qDebug() << "检测第" << i << "行";
            QVector<Gamer*> numbers;            //存放一行中的棋子
            QVector<int> columnEmpty;           //存放一行中的空列
            for (int j=3; j>=0; --j)             //第 j 列
            {qDebug() << "    检测第" << j << "列";
                if (flagGamer[i][j] == 0)
                {qDebug() << "      1个空格,已追加到columnEmpty中";
                    columnEmpty.append(j);      //把空列追加到向量中
                }
                else                            //遇到棋子
                {qDebug() << "      检测到棋子";
                    if (!numbers.isEmpty())     //如果前边有棋子
                    {qDebug() << "            前边有" << numbers.count() << "个棋子";
                        int frontNumber = numbers.at(numbers.count()-1)->text().toInt();
                        int currentNumber = flagGamer[i][j]->text().toInt();
                        if (frontNumber == currentNumber)   //数字相等，相加
                        {qDebug() << "            棋子与前边棋子相等，和为" << frontNumber + currentNumber;
                            int numberCount = frontNumber + currentNumber;
                            numbers.at(numbers.count()-1)->setText(tr("%1").arg(numberCount));
                            flagGamer[i][j]->deleteLater();
                            flagGamer[i][j] = 0;
                         //   numbers.pop_back();++j;++j;
                            numbers.clear();columnEmpty.clear();j=4;


                            continue;
                        }
                    }
                    if (!columnEmpty.isEmpty()) //如果前边有空列
                    {
                        flagGamer[i][j]->move(labBg[i][columnEmpty.at(0)]->pos());
                        flagGamer[i][columnEmpty.at(0)] = flagGamer[i][j];
                        flagGamer[i][j] = 0;
                        numbers.append(flagGamer[i][columnEmpty.at(0)]);
                        columnEmpty.pop_front();
                        columnEmpty.append(j);
                    }
                    else
                    {qDebug() << "          前边既没棋子又没空格，已追加到numbers";
                        numbers.append(flagGamer[i][j]);
                    }
                }
            }
        }
        break;
    case UP:
        for (int column=0; column<4; ++column)                 //第 j 列
        {
            QVector<Gamer*> numbers;            //存放一行中的棋子
            QVector<int> columnEmpty;           //存放一行中的空列
            for (int line=0; line<4; ++line)             //第 i 行
            {
                if (flagGamer[line][column] == 0)
                {
                 //   columnEmpty.append(j);      //把空列追加到向量中
                    columnEmpty.append(line);      //把空列追加到向量中
                }
                else                            //遇到棋子
                {
                    if (!numbers.isEmpty())     //如果前边有棋子
                    {
                        int frontNumber = numbers.at(numbers.count()-1)->text().toInt();
                        int currentNumber = flagGamer[line][column]->text().toInt();
                        if (frontNumber == currentNumber)   //数字相等，相加
                        {
                            int numberCount = frontNumber + currentNumber;
                            numbers.at(numbers.count()-1)->setText(tr("%1").arg(numberCount));
                            flagGamer[line][column]->deleteLater();
                            flagGamer[line][column] = 0;
                         //   numbers.pop_back();++j;++j;
                            numbers.clear();columnEmpty.clear();line=-1;


                            continue;
                        }
                    }
                    if (!columnEmpty.isEmpty()) //如果前边有空列
                    {
                 //       flagGamer[line][column]->move(labBg[line][columnEmpty.at(0)]->pos());
                        flagGamer[line][column]->move(labBg[columnEmpty.at(0)][column]->pos());
                 //       flagGamer[line][columnEmpty.at(0)] = flagGamer[line][column];
                        flagGamer[columnEmpty.at(0)][column] = flagGamer[line][column];
                        flagGamer[line][column] = 0;
                //        numbers.append(flagGamer[line][columnEmpty.at(0)]);
                        numbers.append(flagGamer[columnEmpty.at(0)][column]);
                        columnEmpty.pop_front();
            //            columnEmpty.append(j);
                        columnEmpty.append(line);
                    }
                    else
                    {
                        numbers.append(flagGamer[line][column]);
                    }
                }
            }
        }
        break;
    case DOWN:
        for (int column=0; column<4; ++column)                 //第 j 列
        {
            QVector<Gamer*> numbers;            //存放一行中的棋子
            QVector<int> columnEmpty;           //存放一行中的空列
            for (int line=3; line>=0; --line)             //第 i 行
            {
                if (flagGamer[line][column] == 0)
                {
                 //   columnEmpty.append(j);      //把空列追加到向量中
                    columnEmpty.append(line);      //把空列追加到向量中
                }
                else                            //遇到棋子
                {
                    if (!numbers.isEmpty())     //如果前边有棋子
                    {
                        int frontNumber = numbers.at(numbers.count()-1)->text().toInt();
                        int currentNumber = flagGamer[line][column]->text().toInt();
                        if (frontNumber == currentNumber)   //数字相等，相加
                        {
                            int numberCount = frontNumber + currentNumber;
                            numbers.at(numbers.count()-1)->setText(tr("%1").arg(numberCount));
                            flagGamer[line][column]->deleteLater();
                            flagGamer[line][column] = 0;
                         //   numbers.pop_back();++j;++j;
                            numbers.clear();columnEmpty.clear();line=4;


                            continue;
                        }
                    }
                    if (!columnEmpty.isEmpty()) //如果前边有空列
                    {
                 //       flagGamer[line][column]->move(labBg[line][columnEmpty.at(0)]->pos());
                        flagGamer[line][column]->move(labBg[columnEmpty.at(0)][column]->pos());
                 //       flagGamer[line][columnEmpty.at(0)] = flagGamer[line][column];
                        flagGamer[columnEmpty.at(0)][column] = flagGamer[line][column];
                        flagGamer[line][column] = 0;
                //        numbers.append(flagGamer[line][columnEmpty.at(0)]);
                        numbers.append(flagGamer[columnEmpty.at(0)][column]);
                        columnEmpty.pop_front();
            //            columnEmpty.append(j);
                        columnEmpty.append(line);
                    }
                    else
                    {
                        numbers.append(flagGamer[line][column]);
                    }
                }
            }
        }
        break;
    }
    produce();
}

//销毁棋子
