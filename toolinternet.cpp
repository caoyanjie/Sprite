#include "toolinternet.h"

ToolInternet::ToolInternet(QWidget *parent) :
    QGroupBox(parent)
{
    setGeometry(0, 81, 1001, 32);
    setStyleSheet("QGroupBox{"
                        "border-image: url(:/Images/bgTool2.png)"
                        "border: 1px;"
                  "}"
                  );
}
