#include "mytypedef.h"
#include "mdefinedwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QList>
#include <qmath.h>
#include <QApplication>
#include <QDir>
#include <QSettings>

#define TOP_CORNER_WIDTH        22
#define TOP_CORNER_HEIGHT       30
#define TOP_BORDER_HEIGHT       30
#define MIDDLE_BORDER_WIDTH     22
#define BOTTOM_CORNER_WIDTH     22
#define BOTTOM_CORNER_HEIGHT    33
#define BOTTOM_BORDER_HEIGHT    33

//#define TOP_BORDER_WIDTH 30
//#define BORDER_WIDTH 22
//#define BOTTOM_BORDER_WIDTH 33

#define     LEFT_TOP        0
#define     MIDDLE_TOP      1
#define     RIGHT_TOP       2
#define     LEFT_MIDDLE     3
#define     CENTER          4
#define     RIGHT_MIDDLE    5
#define     LEFT_BOTTOM     6
#define     MIDDLE_BOTTOM   7
#define     RIGHT_BOTTOM    8

mDefinedWidget::mDefinedWidget(QWidget *parent) :
    QDialog(parent),
    m_mouseLeftPress(false),
    m_winactive(true),
    m_skinName(new QString)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    this->installEventFilter(this);
//    this->setWindowIcon();
//    this->changeSkin(QString("Brushed"));
    this->initSkinLayout();
}

mDefinedWidget::~mDefinedWidget()
{
    delete m_skinName;
//    MMASSGE(~mDefinedWidget());
}

void mDefinedWidget::changeSkin(const QString &skinName)
{
    *m_skinName = skinName;
    QDir iconsDir_t(QString("./"));
    if(!iconsDir_t.exists(QString("./skins"))){
        this->initSkinLayout();
    }
    else {
        this->readSettings();
    }
//    MDEBUG(*m_skinName);
    this->update();
}

void mDefinedWidget::initSkinLayout()
{
    *m_skinName = QString(":/default");//defalutSkinName;
    m_TopCornerW = TOP_CORNER_WIDTH;
    m_TopCornerH = TOP_CORNER_HEIGHT;
    m_TopBorderH = TOP_BORDER_HEIGHT;          // may be always equale to m_topCornerHeight
    m_MiddleBorderW = MIDDLE_BORDER_WIDTH;
    m_BottomCornerW = BOTTOM_CORNER_WIDTH;
    m_BottomCornerH = BOTTOM_CORNER_HEIGHT;
    m_BottomBorderH = BOTTOM_BORDER_HEIGHT;

    m_InActTopCornerW = TOP_CORNER_WIDTH;
    m_InActTopCornerH = TOP_CORNER_HEIGHT;
    m_InActTopBorderH = TOP_BORDER_HEIGHT;          // may be always equale to m_topCornerHeight
    m_InActMiddleBorderW = MIDDLE_BORDER_WIDTH;
    m_InActBottomCornerW = BOTTOM_CORNER_WIDTH;
    m_InActBottomCornerH = BOTTOM_CORNER_HEIGHT;
    m_InActBottomBorderH = BOTTOM_BORDER_HEIGHT;
}

int mDefinedWidget::readSettingsValue(QSettings *settings_l, const QString &group_l, const QString &key_l)
{
    QStringList grouplist_t;
    int val = 0;
    grouplist_t = settings_l->childGroups();
    if(grouplist_t.contains(group_l)){
        settings_l->beginGroup(group_l);
        if(settings_l->contains(key_l)){
            val = settings_l->value(key_l).toInt();
        }
        settings_l->endGroup();
    }
    return val;
}

bool mDefinedWidget::readSettings()
{
    const QString filename_t(QString("./skins/") + *m_skinName + QString("/settings.ini"));

    QString skin_name;
    QString author;
    QString Skin = "Skin";
    QString Top = "Top";
    QString Middle = "Middle";
    QString Bottom = "Bottom";
    bool retFlag = true;

    QSettings *settings_t;

    settings_t = new QSettings(filename_t, QSettings::IniFormat);
    skin_name = settings_t->value(Skin + QString("/Name")).toString();
    author = settings_t->value(Skin + QString("/Author")).toString();

    if(skin_name != *m_skinName){
        return retFlag = false;
    }
    *m_skinName = QString("./skins/") + *m_skinName;
//    MDEBUG(*m_skinName);

    m_TopCornerW = readSettingsValue(settings_t, Top, QString("TopCornerWidth"));
    m_TopCornerH = readSettingsValue(settings_t, Top, QString("TopCornerHeight"));
    m_TopBorderH = readSettingsValue(settings_t, Top, QString("TopBorderHeight"));
    m_InActTopCornerW = readSettingsValue(settings_t, Top, QString("InActTopCornerWidth"));
    m_InActTopCornerH = readSettingsValue(settings_t, Top, QString("InActTopCornerHeight"));
    m_InActTopBorderH = readSettingsValue(settings_t, Top, QString("InActTopBorderHeight"));

    m_MiddleBorderW = readSettingsValue(settings_t, Middle, QString("MiddleBorderWidth"));
    m_InActMiddleBorderW = readSettingsValue(settings_t, Middle, QString("InActMiddleBorderWidth"));

    m_BottomCornerW = readSettingsValue(settings_t, Bottom, QString("BottomCornerWidth"));
    m_BottomCornerH = readSettingsValue(settings_t, Bottom, QString("BottomCornerHeight"));
    m_BottomBorderH = readSettingsValue(settings_t, Bottom, QString("BottomBorderHeight"));
    m_InActBottomCornerW = readSettingsValue(settings_t, Bottom, QString("InActBottomCornerWidth"));
    m_InActBottomCornerH = readSettingsValue(settings_t, Bottom, QString("InActBottomCornerHeight"));
    m_InActBottomBorderH = readSettingsValue(settings_t, Bottom, QString("InActBottomBorderHeight"));

    return retFlag;
}

void mDefinedWidget::drawWindow(QPainter &painter)
{
    QList <QPixmap> pixmaps;

//    MDEBUG(*m_skinName);
    if(m_winactive){
        m_topCW = m_TopCornerW;
        m_topCH = m_TopCornerH;
        m_topBH = m_TopBorderH;          // may be always equale to m_topCornerHeight
        m_middleBW = m_MiddleBorderW;
        m_bottomCW = m_BottomCornerW;
        m_bottomCH = m_BottomCornerH;
        m_bottomBH = m_BottomBorderH;
        pixmaps.append(QPixmap(*m_skinName + QString("/left_top.png")));          // img for left top
        pixmaps.append(QPixmap(*m_skinName + QString("/top.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/right_top.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/left_middle.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/bg.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/right_middle.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/left_bottom.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/bottom.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/right_bottom.png")));
    }
    else{
        m_topCW = m_InActTopCornerW;
        m_topCH = m_InActTopCornerH;
        m_topBH = m_InActTopBorderH;          // may be always equale to m_topCornerHeight
        m_middleBW = m_InActMiddleBorderW;
        m_bottomCW = m_InActBottomCornerW;
        m_bottomCH = m_InActBottomCornerH;
        m_bottomBH = m_InActBottomBorderH;
        pixmaps.append(QPixmap(*m_skinName + QString("/inactive_left_top.png")));          // img for left top
        pixmaps.append(QPixmap(*m_skinName + QString("/inactive_top.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/inactive_right_top.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/inactive_left_middle.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/bg.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/inactive_right_middle.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/inactive_left_bottom.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/inactive_bottom.png")));
        pixmaps.append(QPixmap(*m_skinName + QString("/inactive_right_bottom.png")));
    }

//    MDEBUG(m_topCW);
//    MDEBUG(m_topCH);
//    MDEBUG(m_topBH);          // may be always equale to m_topCornerHeight
//    MDEBUG(m_middleBW);
//    MDEBUG(m_bottomCW);
//    MDEBUG(m_bottomCH);
//    MDEBUG(m_bottomBH);

    painter.drawPixmap(0, 0, m_topCW, m_topCH, pixmaps[LEFT_TOP]);
    painter.drawPixmap(m_topCW, 0, this->width() - 2*m_topCW, m_topBH, pixmaps[MIDDLE_TOP]);
    painter.drawPixmap(this->width() - m_topCW, 0, m_topCW, m_topCH, pixmaps[RIGHT_TOP]);

    painter.drawPixmap(0, m_topCH, m_middleBW, this->height() - m_topCH - m_bottomCH, pixmaps[LEFT_MIDDLE]);
    painter.drawTiledPixmap(m_middleBW, m_topCH, this->width() - 2*m_middleBW, this->height() - m_topBH - m_bottomBH, pixmaps[CENTER]);
    painter.drawPixmap(this->width() - m_middleBW, m_topCH, m_middleBW, this->height() - m_topCH - m_bottomCH, pixmaps[RIGHT_MIDDLE]);

    painter.drawPixmap(0, this->height() - m_bottomCH, m_bottomCW, m_bottomCH, pixmaps[LEFT_BOTTOM]);
    painter.drawPixmap(m_bottomCW, this->height()-m_bottomBH, this->width()-2*m_bottomCW, m_bottomBH, pixmaps[MIDDLE_BOTTOM]);
    painter.drawPixmap(this->width()-m_bottomCW, this->height()-m_bottomCH, m_bottomCW, m_bottomCH, pixmaps[RIGHT_BOTTOM]);
}

void mDefinedWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
//    MMASSGE(paintEvent);
    this->drawWindow(painter);
}

void mDefinedWidget::mousePressEvent(QMouseEvent *e)
{
//    qDebug() << "mousePressEvent";
//    m_winactive = true;
    if(Qt::LeftButton == e->button()){
        m_mouseLeftPress = true;
        m_moveOldPos = e->globalPos() - pos();      // old position
    }
//    qDebug() << "mousePressEvent2";
}

void mDefinedWidget::mouseReleaseEvent(QMouseEvent *e)
{
//    qDebug() << "mouseReleaseEvent";
    if(Qt::LeftButton == e->button()){
        m_mouseLeftPress = false;
    }

//    if(Qt::RightButton == e->button()){

//    }
}

void mDefinedWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_mouseLeftPress)
    {
        QPoint moveNewPos = e->globalPos() - m_moveOldPos;    // new position
        move(moveNewPos);
    }
}

bool mDefinedWidget::eventFilter(QObject *o, QEvent *e)
{
    if(this == o){
        if(QEvent::WindowDeactivate == e->type()){
            m_winactive = false;
        }
        else if(QEvent::WindowActivate == e->type()){
            m_winactive = true;
//            emit windowActive();
//            MDEBUG(m_winactive);
        }
    }
    return QDialog::eventFilter(o, e);
}
/*
//    qDebug() << m_winactive;
//    if(m_winactive){
//        pixmaps.append(QPixmap(":/brushed/left_top"));          // img for left top
//        pixmaps.append(QPixmap(":/brushed/top"));               // img for middle top
//        pixmaps.append(QPixmap(":/brushed/right_top"));         // img for right top
//        pixmaps.append(QPixmap(":/brushed/left_middle"));       // img for left middle
//        pixmaps.append(QPixmap(":/brushed/bg"));            // img for center
//        pixmaps.append(QPixmap(":/brushed/right_middle"));      // img for right middle
//        pixmaps.append(QPixmap(":/brushed/left_bottom"));       // img for left bottom
//        pixmaps.append(QPixmap(":/brushed/bottom"));            // img for middle bottom
//        pixmaps.append(QPixmap(":/brushed/right_bottom"));      // img for right bottom
//    }
//    else{
//        pixmaps.append(QPixmap(":/brushed/inactive_left_top"));          // img for left top
//        pixmaps.append(QPixmap(":/brushed/inactive_top"));               // img for middle top
//        pixmaps.append(QPixmap(":/brushed/inactive_right_top"));         // img for right top
//        pixmaps.append(QPixmap(":/brushed/inactive_left_middle"));       // img for left middle
//        pixmaps.append(QPixmap(":/brushed/bg"));            // img for center
//        pixmaps.append(QPixmap(":/brushed/inactive_right_middle"));      // img for right middle
//        pixmaps.append(QPixmap(":/brushed/inactive_left_bottom"));       // img for left bottom
//        pixmaps.append(QPixmap(":/brushed/inactive_bottom"));            // img for middle bottom
//        pixmaps.append(QPixmap(":/brushed/inactive_right_bottom"));      // img for right bottom
//    }
*/
