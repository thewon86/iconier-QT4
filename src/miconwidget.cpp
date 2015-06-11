#include "mytypedef.h"
#include "miconwidget.h"
#include "bmptypes.h"
#include "icontypes.h"
#include "micon.h"
#include <QtEvents>
#include <QString>
#include <QDrag>
#include <QDropEvent>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QList>
#include <QUrl>
#include <QDir>
#include <QFileDialog>
#include <QStringList>
#include <QPixmap>
#include <QBitmap>
#include <QBuffer>
#include <QRgb>
#include <QPalette>
#include <QCursor>

bool issuffixmatching(const QString &lsuffix, const QStringList &lsuffixlist)
{
    foreach (QString isuffix, lsuffixlist) {
        if(0 == isuffix.mid(2).compare(lsuffix)){
            return 1;
        }
    }
    return 0;
}

mIconWidget::mIconWidget(QWidget *parent) :
    QLabel(parent),
    imgFileList(new QStringList()),
    m_HistoryDir(new QString()),
    m_sizeList(new QList<int>)
{
    this->setAcceptDrops(1);
    *m_HistoryDir = QDir::currentPath();
}

mIconWidget::~mIconWidget()
{
//    MMASSGE(~mIconWidget());
}

void mIconWidget::clearSizeList()
{
    m_sizeList->clear();
}

void mIconWidget::addSize(QList <int> &list)
{
    m_sizeList->append(list);
}

void mIconWidget::mouseReleaseEvent(QMouseEvent *e)
{
    MMASSGE(mIconWidget::mouseReleaseEvent);
    if(Qt::LeftButton == e->button()){
        getOpenFiles();
        makeIcoFile();
    }
    QLabel::mouseReleaseEvent(e);
}

void mIconWidget::dropEvent(QDropEvent *e)
{
    MMASSGE(dropEvent);
    QList <QUrl>urls;
    urls = e->mimeData()->urls();
    if(!urls.isEmpty()){
        imgFileList->clear();
        foreach(QUrl url, urls) {
            QString imgFileName = url.toLocalFile();
            imgFileList->append(imgFileName);
//            MDEBUG(imgFileName);
        }
        makeIcoFile();
    }
}

void mIconWidget::dragEnterEvent(QDragEnterEvent *e)
{
    MMASSGE(dragEnterEvent);
    if(e->mimeData()->hasFormat("text/uri-list")){
        e->acceptProposedAction();
    }
}

void mIconWidget::dragLeaveEvent(QDragLeaveEvent *)
{
    MMASSGE(dragLeaveEvent);
}

void mIconWidget::dragMoveEvent(QDragMoveEvent *)
{
//    MMASSGE(dragMoveEvent);
}

void mIconWidget::getOpenFiles()
{
    QString *filesfilter = new QString("PNG Image(*.png);;BMP Image(*.bmp);;JPEG Image(*.jpeg *.jpg);;PBM Image(*.pbm);;PGM Image(*.pgm);;All Files(*.*)");

    imgFileList->clear();
    *imgFileList = QFileDialog::getOpenFileNames(this,
                                                 tr("Select Files"),
                                                 *m_HistoryDir,
                                                 *filesfilter);
    if(!imgFileList->isEmpty()){
        QFileInfo file(imgFileList->at(0));
//            qDebug() << file.absoluteDir();
//            qDebug() << file.absoluteFilePath();
//            qDebug() << file.absolutePath();
        *m_HistoryDir = file.absolutePath();
    }
}

void mIconWidget::makeIcoFile()
{
//    static int index = 0;
    QStringList suffixlist;
    suffixlist = QString("*.png;*.bmp;*.jpeg;*.jpg;*.pbm;*.pgm").split(';');

    for(int i = 0; i < imgFileList->size(); i++){
        QFileInfo tfileinfo(imgFileList->at(i));
        if(tfileinfo.isFile() && !issuffixmatching(tfileinfo.suffix(), suffixlist)){
            imgFileList->removeAt(i);
        }
    }
//    MDEBUG(imgFileList->size());
//    MDEBUG(m_sizeList->size());
    if(imgFileList->isEmpty()){
        MMASSGE(File List is Empty!!! Do nothing.);
        return;
    }

    setCursor(Qt::WaitCursor);
    foreach (QString ifile, *imgFileList) {
//        MDEBUG(ifile);
        QPixmap preview_t;
        preview_t.load(ifile);
        this->setAlignment(Qt::AlignCenter);
        this->setPixmap(preview_t);
        mIcon micon_t(ifile);
//        MDEBUG(&micon_t);
        micon_t.setSizeList(*this->m_sizeList);
//        MMASSGE(micon_t.setSizeList(););
        micon_t.creatIconFile();
//        MMASSGE(micon_t.creatIconFile(););
    }
    imgFileList->clear();
    setCursor(Qt::ArrowCursor);
//    MMASSGE(imgFileList->clear(););
}
