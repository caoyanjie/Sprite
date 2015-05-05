#include "videolist.h"
#include <QDesktopWidget>

#include <QLabel>
#include <QComboBox>
#include <QEvent>
#include <musiclist.h>

VideoList::VideoList(int x, int y, int width, int height, bool independ, QWidget *parent) :
    QWidget(parent),
    listWidth(width),
    listHeight(height)

{
    //窗口去边框
    if (independ)
    {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    }
    else
    {
        setWindowFlags(Qt::FramelessWindowHint);
    }

    //窗口透明
    setAttribute(Qt::WA_TranslucentBackground);

    //初始化窗口位置，大小
    setGeometry(x, y, width, height);
    setObjectName("videoList");

    //设置半透明背景
    lab_background = new QLabel(this);
    lab_background->setGeometry(0, 0, this->width(), this->height());
    lab_background->setObjectName("lab_background1");

    //创建部件
    lab_title = new QLabel("播放列表", lab_background);
    cob_addVideo = new QComboBox(lab_background);
    cob_addVideo->addItem("添加视频");
    cob_addVideo->addItem("添加目录");

    lab_title->setGeometry(5, 5, 80, 20);
    cob_addVideo->setGeometry(this->width()-85, 5, 80, 20);

    lab_title->setObjectName("lab_title");
    cob_addVideo->setObjectName("cob_addVideo");

    lab_background->installEventFilter(this);
    cob_addVideo->installEventFilter(this);
//    lab_background->setMouseTracking(true);
    //MusicList *lis = new MusicList(lab_background);

    //设置样式
    setStyleSheet(
                "#videoList{"
//                    "background: rgb(43, 45, 163);"
//                    "color: rgba(255, 255, 255, 255);"
                "}"

                "#lab_background1{"
//                    "background: rgba(0, 0, 0, 120);"
//                    "background: rgba(255, 255, 255, 80);"
//                    "background: rgba(255, 0, 0, 100);"
//                    "background: rgba(40, 62, 83, 195);"
                    "background: rgba(10, 0, 0, 120);"
                    "border: 1px solid rgba(255, 255, 255, 15);"
                "}"

                "#lab_title{"
                    "color: rgba(200, 255, 255, 255);"
                    "font-family: 宋体;"
                    "font-size: 11pt;"
 //                   "font-weight: bold;"
                "}"

                "#cob_addVideo{"
                    "background: rgba(255, 255, 255, 70);"
                    "border-radius: 3px;"
                    "color: rgba(200, 255, 255, 255);"
                    "border-radius: 3px;"
//                    "min-width: 6em;"
                "}"
                "#cob_addVideo:drop-down{"
                    "background: rgba(0, 0, 0, 0);"
                    "border-image: url(:/Images/cobIcon.png);"
                "}"
                "#cob_addVideo QListView{"
                    "color: red;"
                "}"
                "#cob_addVideo QAbstractItemView{"
                    "padding: 5pt;"
                    "color: blue;"
                "}"
                );
}

//事件过滤器
bool VideoList::eventFilter(QObject *target, QEvent *event)
{
    if (target == lab_background)
    {
        switch(event->type())
        {
        case QEvent::Enter:
            emit isMouseEnter(true);
            break;
        case QEvent::Leave:
            emit isMouseEnter(false);
            break;
        }
    }
    else if (target == cob_addVideo)
    {
        switch(event->type())
        {
        case QEvent::Enter:
            emit isMouseEnter(true);
            break;
        case QEvent::Leave:
            emit isMouseEnter(false);
            break;
        default:
            emit isMouseEnter(true);
        }
    }
/*
    switch(event->type())
    {
    case QEvent::None: qDebug() << "None"; break;
    case QEvent::ActionAdded: qDebug() << "ActionAdded"; break;
    case QEvent::ActionChanged: qDebug() << "ActionChanged"; break;
    case QEvent::ActionRemoved: qDebug() << "ActionRemoved"; break;
    case QEvent::ActivationChange: qDebug() << "ActivationChange"; break;
    case QEvent::ApplicationDeactivate: qDebug() << "ApplicationDeactivate"; break;
    case QEvent::ApplicationFontChange: qDebug() << "ApplicationFontChange"; break;
    case QEvent::ApplicationLayoutDirectionChange: qDebug() << "ApplicationLayoutDirectionChange"; break;
    case QEvent::ApplicationPaletteChange: qDebug() << "ApplicationPaletteChange"; break;
    case QEvent::ApplicationStateChange: qDebug() << "ApplicationStateChange"; break;
    case QEvent::ApplicationWindowIconChange: qDebug() << "ApplicationWindowIconChange"; break;
    case QEvent::ChildAdded: qDebug() << "ChildAdded"; break;
    case QEvent::ChildPolished: qDebug() << "ChildPolished"; break;
    case QEvent::ChildRemoved: qDebug() << "ChildRemoved"; break;
    case QEvent::Clipboard: qDebug() << "Clipboard"; break;
    case QEvent::Close: qDebug() << "Close"; break;
    case QEvent::CloseSoftwareInputPanel: qDebug() << "CloseSoftwareInputPanel"; break;
    case QEvent::ContentsRectChange: qDebug() << "ContentsRectChange"; break;
    case QEvent::ContextMenu: qDebug() << "ContextMenu"; break;
    case QEvent::CursorChange: qDebug() << "CursorChange"; break;
    case QEvent::DeferredDelete: qDebug() << "DeferredDelete"; break;
    case QEvent::DragEnter: qDebug() << "DragEnter"; break;
    case QEvent::DragLeave: qDebug() << "DragLeave"; break;
    case QEvent::DragMove: qDebug() << "DragMove"; break;
    case QEvent::Drop: qDebug() << "Drop"; break;
    case QEvent::DynamicPropertyChange: qDebug() << "DynamicPropertyChange"; break;
    case QEvent::EnabledChange: qDebug() << "EnabledChange"; break;
    case QEvent::Enter: qDebug() << "Enter"; break;
    case QEvent::EnterWhatsThisMode: qDebug() << "EnterWhatsThisMode"; break;
    case QEvent::Expose: qDebug() << "Expose"; break;
    case QEvent::FileOpen: qDebug() << "FileOpen"; break;
    case QEvent::FocusIn: qDebug() << "FocusIn"; break;
    case QEvent::FocusOut: qDebug() << "FocusOut"; break;
    case QEvent::FocusAboutToChange: qDebug() << "FocusAboutToChange"; break;
    case QEvent::FontChange: qDebug() << "FontChange"; break;
    case QEvent::Gesture: qDebug() << "Gesture"; break;
    case QEvent::GestureOverride: qDebug() << "GestureOverride"; break;
    case QEvent::GrabKeyboard: qDebug() << "GrabKeyboard"; break;
    case QEvent::GrabMouse: qDebug() << "GrabMouse"; break;
    case QEvent::GraphicsSceneContextMenu: qDebug() << "GraphicsSceneContextMenu"; break;
    case QEvent::GraphicsSceneDragEnter: qDebug() << "GraphicsSceneDragEnter"; break;
    case QEvent::GraphicsSceneDragLeave: qDebug() << "GraphicsSceneDragLeave"; break;
    case QEvent::GraphicsSceneDragMove: qDebug() << "GraphicsSceneDragMove"; break;
    case QEvent::GraphicsSceneDrop: qDebug() << "GraphicsSceneDrop"; break;
    case QEvent::GraphicsSceneHelp: qDebug() << "GraphicsSceneHelp"; break;
    case QEvent::GraphicsSceneHoverEnter: qDebug() << "GraphicsSceneHoverEnter"; break;
    case QEvent::GraphicsSceneHoverLeave: qDebug() << "GraphicsSceneHoverLeave"; break;
    case QEvent::GraphicsSceneHoverMove: qDebug() << "GraphicsSceneHoverMove"; break;
    case QEvent::GraphicsSceneMouseDoubleClick: qDebug() << "GraphicsSceneMouseDoubleClick"; break;
    case QEvent::GraphicsSceneMouseMove: qDebug() << "GraphicsSceneMouseMove"; break;
    case QEvent::GraphicsSceneMousePress: qDebug() << "GraphicsSceneMousePress"; break;
    case QEvent::GraphicsSceneMouseRelease: qDebug() << "GraphicsSceneMouseRelease"; break;
    case QEvent::GraphicsSceneMove: qDebug() << "GraphicsSceneMove"; break;
    case QEvent::GraphicsSceneResize: qDebug() << "GraphicsSceneResize"; break;
    case QEvent::GraphicsSceneWheel: qDebug() << "GraphicsSceneWheel"; break;
    case QEvent::Hide: qDebug() << "Hide"; break;
    case QEvent::HideToParent: qDebug() << "HideToParent"; break;
    case QEvent::HoverEnter: qDebug() << "HoverEnter"; break;
    case QEvent::HoverLeave: qDebug() << "HoverLeave"; break;
    case QEvent::HoverMove: qDebug() << "HoverMove"; break;
    case QEvent::IconDrag: qDebug() << "IconDrag"; break;
    case QEvent::IconTextChange: qDebug() << "IconTextChange"; break;
    case QEvent::InputMethod: qDebug() << "InputMethod"; break;
    case QEvent::InputMethodQuery: qDebug() << "InputMethodQuery"; break;
    case QEvent::KeyboardLayoutChange: qDebug() << "KeyboardLayoutChange"; break;
    case QEvent::KeyPress: qDebug() << "KeyPress"; break;
    case QEvent::KeyRelease: qDebug() << "KeyRelease"; break;
    case QEvent::LanguageChange: qDebug() << "LanguageChange"; break;
    case QEvent::LayoutDirectionChange: qDebug() << "LayoutDirectionChange"; break;
    case QEvent::LayoutRequest: qDebug() << "LayoutRequest"; break;
    case QEvent::Leave: qDebug() << "Leave"; break;
    case QEvent::LeaveWhatsThisMode: qDebug() << "LeaveWhatsThisMode"; break;
    case QEvent::LocaleChange: qDebug() << "LocaleChange"; break;
    case QEvent::NonClientAreaMouseButtonDblClick: qDebug() << "NonClientAreaMouseButtonDblClick"; break;
    case QEvent::NonClientAreaMouseButtonPress: qDebug() << "NonClientAreaMouseButtonPress"; break;
    case QEvent::NonClientAreaMouseButtonRelease: qDebug() << "NonClientAreaMouseButtonRelease"; break;
    case QEvent::NonClientAreaMouseMove: qDebug() << "NonClientAreaMouseMove"; break;
    case QEvent::MacSizeChange: qDebug() << "MacSizeChange"; break;
    case QEvent::MetaCall: qDebug() << "MetaCall"; break;
    case QEvent::ModifiedChange: qDebug() << "ModifiedChange"; break;
    case QEvent::MouseButtonDblClick: qDebug() << "MouseButtonDblClick"; break;
    case QEvent::MouseButtonPress: qDebug() << "MouseButtonPress"; break;
    case QEvent::MouseButtonRelease: qDebug() << "MouseButtonRelease"; break;
    case QEvent::MouseMove: qDebug() << "MouseMove"; break;
    case QEvent::MouseTrackingChange: qDebug() << "MouseTrackingChange"; break;
    case QEvent::Move: qDebug() << "Move"; break;
    case QEvent::NativeGesture: qDebug() << "NativeGesture"; break;
    case QEvent::OrientationChange: qDebug() << "OrientationChange"; break;
    case QEvent::Paint: qDebug() << "Paint"; break;
    case QEvent::PaletteChange: qDebug() << "PaletteChange"; break;
    case QEvent::ParentAboutToChange: qDebug() << "ParentAboutToChange"; break;
    case QEvent::ParentChange: qDebug() << "ParentChange"; break;
    case QEvent::PlatformPanel: qDebug() << "PlatformPanel"; break;
    case QEvent::Polish: qDebug() << "Polish"; break;
    case QEvent::PolishRequest: qDebug() << "PolishRequest"; break;
    case QEvent::QueryWhatsThis: qDebug() << "QueryWhatsThis"; break;
    case QEvent::ReadOnlyChange: qDebug() << "ReadOnlyChange"; break;
    case QEvent::RequestSoftwareInputPanel: qDebug() << "RequestSoftwareInputPanel"; break;
    case QEvent::Resize: qDebug() << "Resize"; break;
    case QEvent::ScrollPrepare: qDebug() << "ScrollPrepare"; break;
    case QEvent::Scroll: qDebug() << "Scroll"; break;
    case QEvent::Shortcut: qDebug() << "Shortcut"; break;
    case QEvent::ShortcutOverride: qDebug() << "ShortcutOverride"; break;
    case QEvent::Show: qDebug() << "Show"; break;
    case QEvent::ShowToParent: qDebug() << "ShowToParent"; break;
    case QEvent::SockAct: qDebug() << "SockAct"; break;
    case QEvent::StateMachineSignal: qDebug() << "StateMachineSignal"; break;
    case QEvent::StateMachineWrapped: qDebug() << "StateMachineWrapped"; break;
    case QEvent::StatusTip: qDebug() << "StatusTip"; break;
    case QEvent::StyleChange: qDebug() << "StyleChange"; break;
    case QEvent::TabletMove: qDebug() << "TabletMove"; break;
    case QEvent::TabletPress: qDebug() << "TabletPress"; break;
    case QEvent::TabletRelease: qDebug() << "TabletRelease"; break;
    case QEvent::OkRequest: qDebug() << "OkRequest"; break;
    case QEvent::TabletEnterProximity: qDebug() << "TabletEnterProximity"; break;
    case QEvent::TabletLeaveProximity: qDebug() << "TabletLeaveProximity"; break;
    case QEvent::ThreadChange: qDebug() << "ThreadChange"; break;
    case QEvent::Timer: qDebug() << "Timer"; break;
    case QEvent::ToolBarChange: qDebug() << "ToolBarChange"; break;
    case QEvent::ToolTip: qDebug() << "ToolTip"; break;
    case QEvent::ToolTipChange: qDebug() << "ToolTipChange"; break;
    case QEvent::TouchBegin: qDebug() << "TouchBegin"; break;
    case QEvent::TouchCancel: qDebug() << "TouchCancel"; break;
    case QEvent::TouchEnd: qDebug() << "TouchEnd"; break;
    case QEvent::TouchUpdate: qDebug() << "TouchUpdate"; break;
    case QEvent::UngrabKeyboard: qDebug() << "UngrabKeyboard"; break;
    case QEvent::UngrabMouse: qDebug() << "UngrabMouse"; break;
    case QEvent::UpdateLater: qDebug() << "UpdateLater"; break;
    case QEvent::UpdateRequest: qDebug() << "UpdateRequest"; break;
    case QEvent::WhatsThis: qDebug() << "WhatsThis"; break;
    case QEvent::WhatsThisClicked: qDebug() << "WhatsThisClicked"; break;
    case QEvent::Wheel: qDebug() << "Wheel"; break;
    case QEvent::WinEventAct: qDebug() << "WinEventAct"; break;
    case QEvent::WindowActivate: qDebug() << "WindowActivate"; break;
    case QEvent::WindowBlocked: qDebug() << "WindowBlocked"; break;
    case QEvent::WindowDeactivate: qDebug() << "WindowDeactivate"; break;
    case QEvent::WindowIconChange: qDebug() << "WindowIconChange"; break;
    case QEvent::WindowStateChange: qDebug() << "WindowStateChange"; break;
    case QEvent::WindowTitleChange: qDebug() << "WindowTitleChange"; break;
    case QEvent::WindowUnblocked: qDebug() << "WindowUnblocked"; break;
    case QEvent::WinIdChange: qDebug() << "WinIdChange"; break;
    case QEvent::ZOrderChange: qDebug() << "ZOrderChange"; break;
    }
*/
    return QWidget::eventFilter(target, event);
}

//获得视频的分辨率，使窗口适应视频的大小
void VideoList::gotVideoSize(QSize size)
{
    resize(listWidth, size.height());
    lab_background->resize(listWidth, size.height());
}

//同步移动窗口
void VideoList::toMoveWindow(QPoint point)
{
    move(point.x(), point.y()+25);
}
