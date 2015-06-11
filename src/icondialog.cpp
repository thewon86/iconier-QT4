#include "mytypedef.h"
#include "icondialog.h"
#include "ui_icondialog.h"
#include "fontaws.h"
#include <QFile>
#include <QStyle>
#include <QDir>
#include <QDesktopWidget>
#include <QSettings>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QCursor>
#include <QList>
#include <QListIterator>
#include <QInputDialog>

#ifdef CONF_XML
#include <QtXml>
#include <QDomDocument>
#include <QXmlStreamWriter>
#endif

#ifdef CONF_INI
#ifdef CONF_XML
#error "defined both CONF_INI and CONF_XML!"
#endif
#pragma message ("USE INI CONF FILE")
#else
#ifndef CONF_XML
#error "NOT define either CONF_INI CONF_XML"
#endif
#pragma message "USE XML CONF FILE"
#endif

#ifdef CONF_INI
#define CONFIGFILENAME "./config.ini"
#else
#ifdef CONF_XML
#define CONFIGFILENAME "./config.xml"
#endif
#endif


IconDialog::IconDialog(QWidget *parent) :
    mDefinedWidget(parent),
    ui(new Ui::IconDialog),
    m_contextMenu(new mMenu(this)),
    m_pos(new QPoint),
    m_skin(new QString),
    m_sizeList(new QList<SIZELIST>),
    m_customSizeList(new QList<SIZELIST>),
    m_sizeActs(new QList<QAction*>),
    m_customSizeActs(new QList<QAction*>),
    m_customAct(new QAction(this))
{
    ui->setupUi(this);
    FontAws::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 18);
    FontAws::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    FontAws::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    FontAws::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    FontAws::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);

    initGuiStyle();

    loadConfig();

    createContextMenu();
//    connect(menuAct, SIGNAL(triggered()), this, SLOT(testslot()));
    connect(m_contextMenu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuTriggered(QAction*)));
    connect(m_contextMenu, SIGNAL(removeItem(QAction *)), this, SLOT(removeCurrItem(QAction *)));
}

IconDialog::~IconDialog()
{
    MMASSGE(quit);
    this->updateSizeList();
    this->saveConfig();
    delete m_customAct;
    delete m_customSizeActs;
    delete m_sizeActs;
    delete m_customSizeList;
    delete m_sizeList;
    delete m_skin;
    delete m_pos;
    delete m_contextMenu;
    delete ui;
}

void IconDialog::initGuiStyle()
{
    QFile qss(":/default.css");
    if(!qss.open(QFile::ReadOnly | QFile::Text)){
        MMASSGE(qss file open error!!!);
    }
    else{
        this->setStyleSheet(qss.readAll());
        qss.close();
    }
}

void IconDialog::updateSizeList()
{
    QList<SIZELIST>::iterator i;
    for(i=this->m_sizeList->begin();i!=this->m_sizeList->end();i++)
    {
        QList<QAction *>::iterator ii;
        for(ii=this->m_sizeActs->begin();ii!=this->m_sizeActs->end();ii++){
            if((*i).size == (*ii)->objectName().toInt()){
                (*i).enable = (*ii)->isChecked();
                break;
            }
        }
    }

    this->m_customSizeList->clear();
    QList<QAction *>::iterator ii;
    for(ii=this->m_customSizeActs->begin();ii!=this->m_customSizeActs->end();ii++){
        SIZELIST t;
        t.size = (*ii)->objectName().toInt();
        t.enable = (*ii)->isChecked();
        this->m_customSizeList->append(t);
    }
}

void IconDialog::contextMenuTriggered(QAction *act)
{
    if(act == m_customAct){
        // show input dialog
        bool ok;
        int sizenumb = QInputDialog::getInt(this,
                                            QString("Input a Number"),
                                            QString("1 - 256"),
                                            256,
                                            1,
                                            256,
                                            1,
                                            &ok);
        if(ok){
            SIZELIST t;
            t.size = sizenumb;
            t.enable = true;
            this->m_customSizeList->append(t);
        }
    }
    else{
        this->updateSizeList();
    }
    this->createContextMenu();
}

void IconDialog::removeCurrItem(QAction *act)
{
//    MDEBUG(act);
//    QList<QAction *>::iterator ii;
//    for(ii=this->m_customSizeActs->begin();ii!=this->m_customSizeActs->end();ii++){
//        MDEBUG((*ii)->objectName());
//        MDEBUG((*ii));
//    }
    bool removeone = m_customSizeActs->removeOne(act);
    MDEBUG(removeone);
//    for(ii=this->m_customSizeActs->begin();ii!=this->m_customSizeActs->end();ii++){
//        MDEBUG((*ii)->objectName());
//        MDEBUG((*ii));
//    }
    this->updateSizeList();
    this->createContextMenu();
}

void IconDialog::createContextMenu()
{
//    QList<QAction *> acts;
    QAction *menuAct;

    m_sizeActs->clear();
    m_customSizeActs->clear();
    m_contextMenu->clear();
    m_contextMenu->clearSubMenuActsFilter();

    QList<SIZELIST>::iterator i;
    for(i=this->m_sizeList->begin();i!=this->m_sizeList->end();i++)
    {
        QString key = QString::number((*i).size);
        menuAct = new QAction(QString("%1 X %1").arg(key), this);
        menuAct->setObjectName(key);
        menuAct->setCheckable(1);
        if((*i).enable){
            menuAct->setChecked(1);
        }
        m_sizeActs->append(menuAct);
    }
    m_contextMenu->addActions(*m_sizeActs);
    m_contextMenu->addSeparator();

//    acts.clear();
//    QList<SIZELIST>::iterator ii;
    for(i=this->m_customSizeList->begin();i!=this->m_customSizeList->end();i++)
    {
        QString key = QString::number((*i).size);
        menuAct = new QAction(QString("%1 X %1").arg(key), this);
        menuAct->setObjectName(key);
        menuAct->setCheckable(1);
        if((*i).enable){
            menuAct->setChecked(1);
        }

//        QAction *delCurrAct = new QAction(QString("Del %1").arg(menuAct->text()), this);
//        QMenu *delActMenu = new QMenu(this);
//        delActMenu->addAction(delCurrAct);
//        menuAct->setMenu(delActMenu);

        m_customSizeActs->append(menuAct);
    }
    m_contextMenu->addSubMenuActsFilter(m_customSizeActs);
    m_contextMenu->addActions(*m_customSizeActs);
    m_contextMenu->addSeparator();

//    menuAct = new QAction(QString("Custom Size"), this);
    m_customAct->setText(QString("Customed Size"));
    m_contextMenu->addAction(m_customAct);

    enabledSizeList();
}

void IconDialog::enabledSizeList()
{
    QList <int> sizeList_t;

    QList<SIZELIST>::iterator i;
    for(i=this->m_sizeList->begin();i!=this->m_sizeList->end();i++){
        if((*i).enable){
            sizeList_t.append((*i).size);
        }
    }
    for(i=this->m_customSizeList->begin();i!=this->m_customSizeList->end();i++){
        if((*i).enable){
            sizeList_t.append((*i).size);
        }
    }

    QSet <int> sizeSet_t;
    sizeSet_t = sizeList_t.toSet();
    sizeList_t = sizeSet_t.values();

    this->ui->widget_img->clearSizeList();
    this->ui->widget_img->addSize(sizeList_t);
}

void IconDialog::loadConfig()
{
    const QString filename(CONFIGFILENAME);

    loadDefaultConfig();
    if(QFile(filename).exists()){
        if(readConfig()){
            return;
        }
    }
    saveConfig();
}

void IconDialog::loadDefaultConfig()
{
    int w = this->width();
    int h = this->height();
    QDesktopWidget scr;
    int dw = scr.width();
    int dh = scr.height();

    *this->m_pos = QPoint(dw / 2 - w / 2, dh / 2 - h / 2);
    *this->m_skin = QString("Brushed");
    *this->m_sizeList = QList<SIZELIST>()
                        << SIZELIST{256, true}
                        << SIZELIST{128, true}
                        << SIZELIST{96, false}
                        << SIZELIST{64, true}
                        << SIZELIST{48, true}
                        << SIZELIST{32, false}
                        << SIZELIST{24, true}
                        << SIZELIST{16, true}
                        << SIZELIST{8, true}
                        << SIZELIST{4, true};

//    *this->m_customSizeList = QList<int>();
//    MDEBUG(*m_pos);
}

bool IconDialog::readConfig()
{
    const QString filename(CONFIGFILENAME);
    int posX = 0;
    int posY = 0;
    QString skin_path = QString("Brushed");
    QStringList grouplist;
    QString Placement = "Placement";
    QString Skin = "Skin";
    QString Sizes = "Sizes";
    QString Customized = "CustomizedSizes";
    bool saveFlag = true;

#ifdef CONF_INI
    QSettings *settings;
#else

#endif

#ifdef CONF_INI
    settings = new QSettings(filename, QSettings::IniFormat);
    grouplist = settings->childGroups();
    if(grouplist.contains(Placement)){
        settings->beginGroup(Placement);
        if(settings->contains(QString("X"))){
            posX = settings->value("X").toInt();
        }
        else{
            saveFlag = false;
        }

        if(settings->contains(QString("Y"))){
            posY = settings->value("Y").toInt();
        }
        else{
            saveFlag = false;
        }
        settings->endGroup();
        *m_pos = QPoint(posX, posY);
        this->move(posX, posY);
    }
    else{
        saveFlag = false;
    }

    if(grouplist.contains(Skin)){
        settings->beginGroup(Skin);
        if(settings->contains(QString("Name"))){
            skin_path = settings->value("Name").toString();
//            MDEBUG(skin_path);
            *m_skin = skin_path;
            this->setSkin(*m_skin);
        }
        else{
            saveFlag = false;
        }
        settings->endGroup();
//        MDEBUG(*skin_path);
    }
    else{
        saveFlag = false;
    }

    if(grouplist.contains(Sizes)){
        settings->beginGroup(Sizes);
//        foreach (QString key, settings->childKeys()){
//            MDEBUG(key);
//        }

        QList<SIZELIST>::iterator i;
        for(i=this->m_sizeList->begin();i!=this->m_sizeList->end();i++)
        {
            QString key = QString::number((*i).size);
            if(settings->contains(key)){
                (*i).enable = settings->value(key).toBool();
            }
        }
        settings->endGroup();
    }
    else{
        saveFlag = false;
    }

    if(grouplist.contains(Customized)){
        settings->beginGroup(Customized);
        foreach (QString custom_key, settings->childKeys()){
//            MDEBUG(custom_key);
            SIZELIST t;
            t.size = custom_key.toInt();
            t.enable = settings->value(custom_key).toBool();
            this->m_customSizeList->append(t);
        }
        settings->endGroup();
    }

#endif

    return saveFlag;
}

void IconDialog::saveConfig()
{
    const QString filename(CONFIGFILENAME);
#ifdef CONF_INI
    QSettings *settings;
//    MMASSGE(save settings);
//    MDEBUG(filename);
    settings = new QSettings(filename, QSettings::IniFormat);
    settings->setValue("Placement/X", this->m_pos->x());
    settings->setValue("Placement/Y", this->m_pos->y());
    settings->setValue("Skin/Name", *this->m_skin);
    QListIterator<SIZELIST> i(*this->m_sizeList);
    for(;i.hasNext();){
        SIZELIST t = i.next();
//        MDEBUG(t.size);
        settings->setValue(QString("Sizes/%1").arg(t.size), t.enable?1:0);
    }

    QListIterator<SIZELIST> ii(*this->m_customSizeList);
    for(;ii.hasNext();){
        SIZELIST t = ii.next();
//        MDEBUG(t.size);
//        MDEBUG(t.enable);
        settings->setValue(QString("CustomizedSizes/%1").arg(t.size), t.enable?1:0);
    }
    settings->sync();

#endif
}

void IconDialog::setSkin(const QString &skinName)
{
//    MDEBUG(skinName);
    QDir skinsDir_t(QString("./"));

    if(!skinsDir_t.exists(QString("./skins/") + skinName)){
        return;
    }
    this->changeSkin(skinName);
    this->ui->logo->setAlignment(Qt::AlignHCenter);
    this->ui->logo->setPixmap(QPixmap(QString("./skins/") + skinName + QString("/logo.png")));
    this->ui->widget_img->setAlignment(Qt::AlignCenter);
    this->ui->widget_img->setPixmap(QPixmap(QString("./skins/") + skinName + QString("/question.png")));
}

void IconDialog::on_btnMenu_clicked()
{
//    QString newSkin;
//    static int index = 0;
//    index ++;
//    if(index >= 2){
//        index = 0;
//    }

    QDir skinsDir_t(QString("./"));
    QStringList dirsList;
    if(!skinsDir_t.exists(QString("./skins"))){
        return;
    }
    skinsDir_t.setPath(QString("./skins"));
    dirsList = skinsDir_t.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    if(dirsList.isEmpty()){
        return;
    }
    for(int i = 0; i < dirsList.size(); i ++){
//        MDEBUG(i);
//        MDEBUG(dirsList.at(i));
        if(0 == m_skin->compare(dirsList.at(i))){
//            MMASSGE(equale);
            if(i == dirsList.size() - 1){
//                MMASSGE(0);
                *m_skin = dirsList.at(0);
            }
            else {
//                MDEBUG(i);
                *m_skin = dirsList.at(i + 1);
            }
            break;
        }
    }

//    if(QString("Brushed") == *m_skin){
//        *m_skin = QString("Panther");
//    }
//    else if(QString("Panther") == *m_skin){
//        *m_skin = QString("Brushed");
//    }
//    MDEBUG(*m_skin);
    this->setSkin(*m_skin);
}

void IconDialog::on_btnMenu_Close_clicked()
{
    qApp->exit();
}

void IconDialog::on_btnMenu_Max_clicked()
{
//    this->showMinimized();
//    this->setGeometry(qApp->desktop()->availableGeometry());
}

void IconDialog::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void IconDialog::showContextMenu()
{
    m_contextMenu->exec(QCursor::pos());
}

void IconDialog::contextMenuEvent(QContextMenuEvent *)
{
//    MMASSGE(contextMenuEvent);
    showContextMenu();
}

void IconDialog::mouseReleaseEvent(QMouseEvent *e)
{
//    MMASSGE(IconDialog::mouseReleaseEvent);
    if(Qt::LeftButton == e->button()){
        *m_pos = this->pos();
//        MDEBUG(*m_pos);
    }
    mDefinedWidget::mouseReleaseEvent(e);
}
