#ifndef VIDEOCONTRAL_H
#define VIDEOCONTRAL_H

#include <QWidget>

#include <QToolButton>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
//#include <QListWidget>
//#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
#include <QKeyEvent>

class VideoContral : public QWidget
{
    Q_OBJECT
public:
    explicit VideoContral(QWidget *parent = 0);
    ~VideoContral();

    void setVideoTitle(QString videoFileName);
    void addVideoList(QStringList videoPaths);
    void playStop();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
//    QWidget *contralWidget;

    QToolButton *tbn_windowClose;
    QToolButton *tbn_windowMini;
    QToolButton *tbn_setting;
    QToolButton *tbn_switchMusicPlayer;
    QToolButton *tbn_playStop;
    QToolButton *tbn_playPrevious;
    QToolButton *tbn_playPause;
    QToolButton *tbn_playNext;
    QToolButton *tbn_volumn;
    QToolButton *tbn_openFile;

    QSlider *slider_position;
    QSlider *slider_volumn;

    QLabel *lab_windowTitle;
    QLabel *lab_videoFileName;
    QLabel *lab_playlistFram;
    QLabel *lab_playlist;
    QWidget *fram_header;
    QLabel *lab_playContralFram;
    QLabel *lab_totalTime;
    QLabel *lab_currentTime;

//    QListWidget *list_playlist;
    QTreeWidget *list_playlist;
    QComboBox *cob_addFile;

    QHBoxLayout *layout_tbnHeader;
    QHBoxLayout *layout_tbnPlayContral;
    QHBoxLayout *layout_playlistHeader;
    QHBoxLayout *layout_middle;
    QHBoxLayout *layout_playTime;
    QVBoxLayout *layout_playlist;
    QVBoxLayout *layout_bottom;
    QVBoxLayout *layout_top;

//    QListWidgetItem *newVideoItem;
    QTreeWidgetItem *newVideoItem;

    const int tbnHeaderSize;
    const int tbnContralSize;
    const int playlistMaxWidth;
    const int forwardStep;
    const int retreatStep;
//    const int jumpStep;

signals:
    void playerStop();
    void playPause();
    void playNext();
    void currentMediaEnd();
//    void goForward();           //快进
//    void retreat();             //快退
    void playJump(int);
//    void valueChanged(int);
    void switchFullScreen();
    void isHideWidget(bool);
    void addedVideoFiles(QStringList);

public slots:
    void addVidwoFile(int);
    void durationChanged(qint64 totalTime);
    void positionChanged(qint64 currentPosition);
};

#endif // VIDEOCONTRAL_H
