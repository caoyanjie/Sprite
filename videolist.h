#ifndef VIDEOLIST_H
#define VIDEOLIST_H

#include <QWidget>
class QLabel;
class QComboBox;

class VideoList : public QWidget
{
    Q_OBJECT
public:
    explicit VideoList(int x, int y, int width, int height, bool independ, QWidget *parent = 0);

private:
    QLabel *lab_background;
    QLabel *lab_title;
    QComboBox *cob_addVideo;

    int listWidth;
    int listHeight;

protected:
    bool eventFilter(QObject *, QEvent *);

signals:
    void isMouseEnter(bool);

public slots:
    void gotVideoSize(QSize);
    void toMoveWindow(QPoint);

};

#endif // VIDEOLIST_H
