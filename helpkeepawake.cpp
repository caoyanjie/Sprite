#include "helpkeepawake.h"

HelpKeepAwake::HelpKeepAwake(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
}

HelpKeepAwake::~HelpKeepAwake()
{

}

