#include "micon.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <QPixmap>
#include <QImage>
#include <QBitmap>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QBitArray>
#include <QDateTime>

QString getCurrentTime()
{
    return QDateTime::currentDateTime().toString(QString("[yyyy-MM-dd-hh-mm-ss]"));
}

mIcon::mIcon()
{
    m_pixmap = NULL;
    m_imgOrig = NULL;
    m_imgMask = NULL;
    m_sizeList = NULL;
    m_bppList = NULL;
    m_iconImageMapList = NULL;

    m_fileName = NULL;
}

mIcon::mIcon(const QString &filename)
{
    m_pixmap = NULL;
    m_imgOrig = NULL;
    m_imgMask = NULL;
    m_sizeList = NULL;
    m_bppList = NULL;
    m_iconImageMapList = NULL;

    m_fileName = NULL;

    m_iconDir = NULL;

    m_iconDirEn = NULL;
    m_bmpHeader = NULL;
    m_bmpRGBQuad = NULL;
    m_bmpXorMask = NULL;
    m_bmpAndMask = NULL;
    m_width = 0;
    m_height = 0;

    m_sizeList = new QList<int>;
    m_bppList = new QList<int>;
    m_bppList->append(32);
    m_bppList->append(24);

    this->loadFile(filename);
}

mIcon::~mIcon()
{
    if(NULL != m_sizeList){m_sizeList->clear(); delete m_sizeList;/* m_sizeList = NULL;*/}
    if(NULL != m_bppList){m_bppList->clear(); delete m_bppList;/* m_bppList = NULL;*/}
    this->clearMemory();
}

void mIcon::initIconFormat()
{
//    int i,j,k;
//    m_icondir = new ICONDIR{0, 1, 0};
//    m_icondir->idReserved = 0;
//    m_icondirentry = new ICONDIRENTRY{0};
//    m_iconimage = new ICONIMAGEMAP;

}

void mIcon::setSizeList(const QList <int> &list)
{
    m_sizeList->clear();
    m_sizeList->append(list);
//    MMASSGE(m_sizeList->append(list););
}

void mIcon::setIconFileName(const QString &filename)
{
    if(NULL != m_fileName){ delete m_fileName;}
    m_fileName = new QString;

    *m_fileName = filename;
}

void mIcon::loadFile(const QString &filename)
{
    this->clearMemory();
    m_pixmap = new QPixmap;
    m_imgOrig = new QImage;
    m_imgMask = new QImage;
    m_iconImageMapList = new QList<ICONIMAGEMAP>;

//    MDEBUG(m_pixmap);
//    MDEBUG(m_imgOrig);
//    MDEBUG(m_imgMask);
//    MDEBUG(m_sizeList);
//    MDEBUG(m_bppList);
//    MDEBUG(m_iconImageMapList);

    *m_pixmap = QPixmap(filename);
//    MDEBUG(m_pixmap->width());
//    MDEBUG(m_pixmap->height());

    m_fileName = new QString;
    QFileInfo fileinfo_t(filename);
    *m_fileName = fileinfo_t.baseName();
}

bool mIcon::initSubBMPImg(const QPixmap &pixmap_l)
{
    if(NULL == m_imgOrig || NULL == m_imgMask/* || NULL == m_imgAlpha*/){
        MMASSGE(initSubBMPImg error!!!);
        return false;
    }
    *m_imgOrig = pixmap_l.toImage();
    *m_imgMask = pixmap_l.mask().toImage();

    m_width = pixmap_l.width();
    m_height = pixmap_l.height();

//    MDEBUG(m_width);
//    MDEBUG(m_height);
//    MDEBUG(m_imgMask->width());
//    MDEBUG(m_imgMask->height());
    return true;
}

void mIcon::creatIconFile()
{
//    MMASSGE();
    if(m_sizeList->isEmpty()){
        m_sizeList->append(128);
        m_sizeList->append(96);
        m_sizeList->append(72);
        m_sizeList->append(48);
        m_sizeList->append(24);
        m_sizeList->append(16);
    }
    if(m_bppList->isEmpty()){
        m_bppList->append(32);
        m_bppList->append(24);
    }
    if(NULL == m_pixmap || m_pixmap->isNull()){
        MMASSGE(m_pixmap->isNull());
        return;
    }
    QList <QPixmap> pixmapList;
    QList <int>::iterator i;
    for(i=this->m_sizeList->begin();i!=this->m_sizeList->end();i++){
        QPixmap pixmap_t0 = m_pixmap->scaled(*i, *i);
        if(pixmap_t0.isNull()){
            MMASSGE(pixmap_t0.isNull());
            continue;
        }
        pixmapList.append(pixmap_t0);
    }
//    MMASSGE();
    if(pixmapList.isEmpty()){
        MMASSGE(pixmapList.isEmpty());
        return;
    }
    QList <QPixmap>::iterator ii;
    for(ii=pixmapList.begin(); ii!=pixmapList.end(); ii++){
//        MMASSGE();
        if((*ii).isNull()){
            MMASSGE(pixmap isNull);
            continue;
        }
        if(false == this->initSubBMPImg((*ii))){
            continue;
        }
//        MMASSGE(initSubBMPImg());
        QList <int>::iterator ibpp;
        for(ibpp=this->m_bppList->begin();ibpp!=this->m_bppList->end();ibpp++){
            this->m_bmpBitCount = (*ibpp);
//            MDEBUG(m_bmpBitCount);
            if(!(m_bmpBitCount == 32 || m_bmpBitCount == 24 || m_bmpBitCount == 16 || m_bmpBitCount == 8 || m_bmpBitCount == 4)){
                MMASSGE(unkown bpp!!!);
                m_bmpBitCount = 24;
            }
            if(false == this->makeBMPRGBQuad()){
                continue;
            }
//            MMASSGE(makeBMPRGBQuad());
            if(false == this->makeBMPXorMask()){
                continue;
            }
//            MMASSGE(makeBMPXorMask());
            if(false == this->makeBMPAndMask()){
                continue;
            }
//            MMASSGE(makeBMPAndMask());
            if(false == this->makeBMPInfoHeader()){
                continue;
            }
//            MMASSGE(makeBMPInfoHeader());
            if(false == this->makeIconDirEntry()){
                continue;
            }
//            MMASSGE(makeIconDirEntry());
            if(false == this->makeIconMap()){
                continue;
            }
//            MMASSGE(makeIconMap());

        }
    }

    if(this->makeIconFileHeader()){
//        MMASSGE(makeIconFileHeader());
        if(this->makeIconData()){
//            MMASSGE(makeIconData());
            if(this->save()){
//                MMASSGE(save());
            }
            else{
                MMASSGE(save file error!!!);
            }
        }
     }
    this->clearMemory();
//    MMASSGE(clearMemory());
}

bool mIcon::makeBMPRGBQuad()
{
    m_bmpRGBQuad = NULL;
    return true;
}

bool mIcon::makeBMPXorMask()
{
    QRgb rgb_orig;
    QRgb rgb_new;
    m_bmpXorMask = new QByteArray ;

    if(m_imgOrig->isNull()){
        MMASSGE(makeBMPXorMask error!!!);
        return false;
    }

    int w = m_width;
    int h = m_height;
    //    MDEBUG(w);
    //    MDEBUG(h);
    if(w <=0 || h <=0){
        MMASSGE(makeBMPXorMask size error!!!);
        return false;
    }

//    int biBitCount = 24;
    int bytePerLine_l = (((w * m_bmpBitCount) + 7) >> 3);          //
    int bytePerLine = (((w * m_bmpBitCount) + 31) >> 5 ) << 2;     // bytePerLine >= bytePerLine_l
    int i = 0;
    int j = 0;
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
//            qDebug() << "i=" << i << "j=" << j << "h-1-i=" << (h-1-i) << "j=" << j;
            rgb_orig = m_imgOrig->pixel(j, h - 1 -i);           //
            rgb_new = rgb_orig;
            switch (m_bmpBitCount) {
                case 24:
                    if(::qAlpha(rgb_orig) == 0x00){
                        rgb_new = 0x0;
                    }
                    else if(::qAlpha(rgb_orig) == 0xFF){
                        rgb_new = rgb_orig;
                    }
                    else{
                        int alpha = ::qAlpha(rgb_orig);
                        int red = ::qRed(rgb_orig);
                        int green = ::qGreen(rgb_orig);
                        int blue = ::qBlue(rgb_orig);
                        red = 255 - alpha + (alpha * red) / 255;
                        green = 255 - alpha + (alpha * green) / 255;
                        blue = 255 - alpha + (alpha * blue) / 255;
                        rgb_new = ::qRgba(red, green, blue, 0xFF);
                    }
                    m_bmpXorMask->append((char*)(&rgb_new), 3);
                    break;
                case 32:
                    m_bmpXorMask->append((char*)(&rgb_new), 4);
                    break;
            }
        }
        for(j = bytePerLine_l; j < bytePerLine; j++){           // padding bytes per line
            char padding = 0x00;
            m_bmpXorMask->append(&padding);
        }
    }

    return true;
}

bool mIcon::makeBMPAndMask()
{
    QRgb rgb_orig;
//    QRgb rgb_new;
    m_bmpAndMask = new QByteArray ;


    int w = m_width;
    int h = m_height;
    if(w <=0 || h <=0){
        MMASSGE(makeBMPAndMask size error!!!);
        return false;
    }

    int biBitCount = 1;
//    int bytePerLine_l = (((w * biBitCount) + 7) >> 3);          //
    int bytePerLine = (((w * biBitCount) + 31) >> 5 ) << 2;     // bytePerLine >= bytePerLine_l

//    MDEBUG(bytePerLine);
    m_bmpAndMask->resize(bytePerLine * h);
    m_bmpAndMask->fill('\0');
//    MDEBUG(m_imgMask->size());

    if(m_imgMask->isNull()){                            // no mask image fill 0 with And-mask;
        MMASSGE(makeBMPAndMask no mask warning!!!);
        return true;
    }
    QString tmp;
    int i = 0;
    int j = 0;
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            int index = j >> 3;                         // byte position
            int bits = 7 - j % 8;                       // bit position
            rgb_orig = m_imgMask->pixel(j, h - 1 -i);   //
            if(::qBlue(rgb_orig) == 0xFF){
//                qDebug() << "i=" << i << "j=" << j;
                char *data = m_bmpAndMask->data();
                data[i * bytePerLine + index] = data[i * bytePerLine + index] | (0x1 << bits);
            }
        }
    }
    return true;
}

bool mIcon::makeBMPInfoHeader()
{
    if(m_bmpXorMask->isNull() || m_bmpAndMask->isNull()){
        MMASSGE(makeBMPInfoHeader error!!!);
        return false;
    }

    BITMAPINFOHEADER bmpInfoHeader_t;
    bmpInfoHeader_t.biSize = 0x28;
    bmpInfoHeader_t.biWidth = m_width;
    bmpInfoHeader_t.biHeight = m_height * 2;
    bmpInfoHeader_t.biPlanes = 0x01;
    bmpInfoHeader_t.biBitCount = m_bmpBitCount;
    bmpInfoHeader_t.biCompression = 0x0;
    bmpInfoHeader_t.biSizeImage = m_bmpXorMask->size() + m_bmpAndMask->size();
    bmpInfoHeader_t.biXPelsPerMeter = 0x0;
    bmpInfoHeader_t.biYPelsPerMeter = 0x0;
    bmpInfoHeader_t.biClrUsed = 0x0;
    bmpInfoHeader_t.biClrImportant = 0x0;

    m_bmpHeader = new QByteArray;
    m_bmpHeader->append((char*)&bmpInfoHeader_t, sizeof(BITMAPINFOHEADER));
    return true;
}

bool mIcon::makeIconDirEntry()
{
    ICONDIRENTRY icondirety_t;
    icondirety_t.ibWidth = m_width;
    icondirety_t.ibHeight = m_height;
    icondirety_t.ibColorCount = 0;      // number of colors in image (0 if >=8bpp)
    icondirety_t.ibReserved = 0;
    icondirety_t.iwPlanes = 1;
    icondirety_t.iwBitCount = m_bmpBitCount;
    icondirety_t.idwBytesInRes = sizeof(BITMAPINFOHEADER) + m_bmpXorMask->size() + m_bmpAndMask->size();
    icondirety_t.idwImageOffset = 0;

    m_iconDirEn = new ICONDIRENTRY;
    *m_iconDirEn = icondirety_t;
//    m_iconDirEnList->append(icondirety_t);
    return true;
}

bool mIcon::makeIconMap()
{
    ICONIMAGEMAP iconImageMap_t;
//    m_iconimage = new ICONIMAGEMAP;
    iconImageMap_t.ibIconDirEn = m_iconDirEn;
    iconImageMap_t.ibBMPHeader = m_bmpHeader;
    iconImageMap_t.ibColors = m_bmpRGBQuad;
    iconImageMap_t.ibXor = m_bmpXorMask;
    iconImageMap_t.ibAnd = m_bmpAndMask;

    m_iconDirEn = NULL;
    m_bmpHeader = NULL;
    m_bmpRGBQuad = NULL;
    m_bmpXorMask = NULL;
    m_bmpAndMask = NULL;

    m_iconImageMapList->append(iconImageMap_t);
    return true;
}

bool mIcon::makeIconFileHeader()
{
    ICONDIR icondir_t;
    icondir_t.idReserved = 0;
    icondir_t.idType = 0x01;
    icondir_t.idCount = m_iconImageMapList->size();

    m_iconDir = new QByteArray;
    if(NULL == m_iconDir){
        MMASSGE(m_iconDir new QByteArray error!!!);
        return false;
    }
    m_iconDir->append((char*)&icondir_t, sizeof(ICONDIR));
    return true;
}

bool mIcon::makeIconData(){
    m_iconData = new QByteArray;
    if(NULL == m_iconData){
        MMASSGE(m_iconData is null!!!);
        return false;
    }
    //    MMASSGE();
    if(!m_iconDir->isEmpty()){
        m_iconData->append(*m_iconDir);                      // icon file dir header
    }

    if(m_iconImageMapList->isEmpty()){
        MMASSGE(m_iconImageMapList->isEmpty());
        return false;
    }

    int numb_t = m_iconImageMapList->size();        // icon file dir entry
    m_iconImageMapList->at(0).ibIconDirEn->idwImageOffset = sizeof(ICONDIR) + sizeof(ICONDIRENTRY)*numb_t;
//    MMASSGE();
    for(int i = 1; i < numb_t; i++){
        int lastOffset = m_iconImageMapList->at(i-1).ibIconDirEn->idwImageOffset;
        int lastInRes = m_iconImageMapList->at(i-1).ibIconDirEn->idwBytesInRes;
        m_iconImageMapList->at(i).ibIconDirEn->idwImageOffset = lastInRes + lastOffset;
//        MDEBUG(i);
    }
//    MMASSGE();
    QList <ICONIMAGEMAP>::iterator imap;
    for(imap=m_iconImageMapList->begin();imap!=m_iconImageMapList->end();imap++){
        QByteArray direntry_t;
        direntry_t.append((char*)(*imap).ibIconDirEn, sizeof(ICONDIRENTRY));
        if(!direntry_t.isEmpty()){
            m_iconData->append(direntry_t);
        }
//        MDEBUG(imap);
    }
//    MMASSGE();
    for(imap=m_iconImageMapList->begin();imap!=m_iconImageMapList->end();imap++){
        if(!(*imap).ibBMPHeader->isEmpty()){
            m_iconData->append(*(*imap).ibBMPHeader);
        }
//        MDEBUG((*imap).ibBMPHeader);
        if(!(*imap).ibXor->isEmpty()){
            m_iconData->append(*(*imap).ibXor);
        }
//        MDEBUG((*imap).ibXor);
        if(!(*imap).ibAnd->isEmpty()){
            m_iconData->append(*(*imap).ibAnd);
        }
//        MDEBUG((*imap).ibAnd);
    }
    return true;
}

bool mIcon::save()
{
    // save icon file
    QString currTime_t = getCurrentTime();
    QDir iconsDir_t(QString("./"));
    if(!iconsDir_t.exists(QString("./icons"))){
        MMASSGE(icons dir is not exist);
        if(iconsDir_t.mkdir(QString("./icons"))){
            MMASSGE(mkdir ok!!!);
        }
        else {
            MMASSGE(mkdir error!!!);
        }
    }
    *m_fileName = QString("./icons/icon_") + currTime_t + QString("_") + *m_fileName + QString(".ico");
//    MDEBUG(*m_fileName);
    QFile icoFile(*m_fileName);
    if(!icoFile.open(QFile::WriteOnly)){
        MMASSGE(open icon file error!!!);
        return false;
    }
    if(m_iconData->size() != icoFile.write(*m_iconData)){
        MMASSGE(icoFile.write to file error!!!);
        return false;
    }
    icoFile.close();
    MMASSGE(create icon file ok!!!);
    return true;
}

void mIcon::clearMemory()
{
//    MMASSGE(begain clearMemory);
    if(NULL != m_pixmap){ delete m_pixmap; m_pixmap = NULL;}
    if(NULL != m_imgOrig){ delete m_imgOrig; m_imgOrig = NULL;}
    if(NULL != m_imgMask){ delete m_imgMask; m_imgMask = NULL;}
    if(NULL != m_fileName){ delete m_fileName; m_fileName = NULL;}
    if(NULL != m_iconDir){ delete m_iconDir; m_iconDir = NULL;}

    if(NULL != m_iconImageMapList){
        QList <ICONIMAGEMAP>::iterator imap;
        for(imap=m_iconImageMapList->begin();imap!=m_iconImageMapList->end();imap++){
            if(NULL != m_iconDir){ delete (*imap).ibIconDirEn;}
            if(NULL != m_iconDir){ delete (*imap).ibBMPHeader;}
//            if(NULL != m_iconDir){ delete (*imap).ibColors;}
            if(NULL != m_iconDir){ delete (*imap).ibXor;}
            if(NULL != m_iconDir){ delete (*imap).ibAnd;}
        }
        m_iconImageMapList->clear();
        delete m_iconImageMapList;
        m_iconImageMapList = NULL;
    }

//    MMASSGE(end clearMemory);
}
