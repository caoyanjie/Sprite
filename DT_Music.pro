#-------------------------------------------------
#
# Project created by QtCreator 2014-03-01T14:49:14
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += multimediawidgets
QT       += sql
QT       += network
CONFIG+=qaxcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DT_Music
TEMPLATE = app


SOURCES += main.cpp\
        dt_music.cpp \
    titlegroupbox.cpp \
    bottomgroupbox.cpp \
    musiclist.cpp \
    lrcview.cpp \
    setting.cpp \
    theme.cpp \
    desktoplrc.cpp \
    internetwidget.cpp \
    toolglobal.cpp \
    toolinternet.cpp \
    playmodle.cpp \
    create_musiclist.cpp \
    timing.cpp \
    currentlrc.cpp \
    videowidget.cpp \
    videolist.cpp \
    videoplayer.cpp \
    musicminiplayer.cpp \
    game.cpp \
    videocontral.cpp \
    databaseoperation.cpp \
    helpkeepawake.cpp \
    network.cpp \
    gamer.cpp \
    subthread.cpp

HEADERS  += dt_music.h \
    titlegroupbox.h \
    bottomgroupbox.h \
    musiclist.h \
    lrcview.h \
    setting.h \
    theme.h \
    desktoplrc.h \
    internetwidget.h \
    toolglobal.h \
    toolinternet.h \
    playmodle.h \
    create_musiclist.h \
    timing.h \
    currentlrc.h \
    videowidget.h \
    videolist.h \
    videoplayer.h \
    musicminiplayer.h \
    game.h \
    videocontral.h \
    databaseoperation.h \
    helpkeepawake.h \
    network.h \
    gamer.h \
    subthread.h

FORMS    += dt_music.ui

RESOURCES += \
    tubiao.qrc

OTHER_FILES += \
    logo.rc
RC_FILE    += \
    logo.rc
