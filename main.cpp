#include "dt_music.h"
#include "game.h"
#include <QApplication>

#include <QTextCodec>

#include <QSplashScreen>
#include <QLabel>
#include <QMovie>
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>

int main(int argc, char *argv[])
{
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    QFont font;
    font.setPointSize(10);
    qApp ->setFont(font);

/*
    QSplashScreen splash(QPixmap(":/Images/internet.png"));
    QLabel lbl(&splash);
    QMovie mv(":/images/main.gif");
    lbl.setMovie (&mv);
    mv.start ();
    splash.show ();
    splash.setCursor(Qt::BlankCursor);
    a.processEvents();
    lbl.close();
*/
    //获取执行文件绝对路径
    QString programPath;
    programPath = QFileInfo(QTextStream(argv[0]).readAll()).absolutePath() + "/";
    if (programPath.isEmpty())
    {
        exit(-1);
    }

    //创建主程序对象
    DT_Music w(programPath);
    w.hide();
    w.setMouseTracking(true);
//    w.show();

//    Game g;
//    g.show();
//    splash.finish(&w);

    //定义 音频/视频 格式
    QStringList audioFormat;
    QStringList videoFormat;
    audioFormat << "mp3" << "wma" << "wav" << "asf" << "aac" << "mp3pro" << "vqf" << "flac" << "ape" << "mid" << "ogg"
                << "MP3" << "WMA" << "WAV" << "ASF" << "AAC" << "MP3PRO" << "VQF" << "FLAC" << "APE" << "MID" << "OGG";
    videoFormat << "rmvb" << "avi" << "mp4" << "rm" << "wmv" << "3gp" << "flv" << "mkv" << "ts" << "mpeg1" << "mpeg2" << "mpeg3" << "mpeg4" << "mov" << "vob" << "mtv" << "dat" << "amv" << "dmv"
                << "RMVB" << "AVI" << "MP4" << "RM" << "WMV" << "3GP" << "FLV" << "MKV" << "TS" << "MP3G1" << "MP3G2" << "MPEG3" << "MPEG4" << "MOV" << "VOB" << "MTV" << "DAT" << "AMV" << "DMV";
    if (argc == 2)
    {
        //通过 QTextStream 编码中间变量转换
        QString fileOpen = QTextStream(argv[1]).readAll();
        fileOpen = QFileInfo(fileOpen).absoluteFilePath();      //将 Window 下路径中的 "\" 装换成 "/"
        QString fileOpenSuffix = QFileInfo(fileOpen).suffix();

        for (int i=0; i<audioFormat.length(); i++)
        {
            if (fileOpenSuffix == audioFormat.at(i))
            {
                w.show();
                w.openTempMusicFile(fileOpen);
                goto openFileFinished;
            }
        }
        for (int i=0; i<videoFormat.length(); i++)
        {
            if (fileOpenSuffix == videoFormat.at(i))
            {
                w.openTempVideoFile(fileOpen);
            }
        }
    }
    else
    {
        w.show();
    }

openFileFinished:
    return a.exec();
}
