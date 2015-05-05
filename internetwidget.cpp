#include "internetwidget.h"

InternetWidget::InternetWidget(QWidget *parent) :
    QGroupBox(parent)
{
    setGeometry(276, 113, 731, 476);
    setStyleSheet("border-image: url(:/Images/internet.png)");
}
