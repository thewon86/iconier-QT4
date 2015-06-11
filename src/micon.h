#ifndef MICON_H
#define MICON_H

#include "icontypes.h"

QT_BEGIN_NAMESPACE
class QImage;
class QPixmap;
class QString;
class QByteArray;
QT_END_NAMESPACE

typedef struct tagICONIMAGE{
    ICONDIRENTRY  *ibIconDirEn;   // dib header
    QByteArray  *ibBMPHeader;   // dib header
    QByteArray  *ibColors;   // color table
    QByteArray  *ibXor;      // dib bits for xor mask
    QByteArray  *ibAnd;      // dib bits for and mask
} ICONIMAGEMAP, *pICONIMAGEMAP;

class mIcon
{
public:
    mIcon();
    mIcon(const QString &filename);
//    mIcon(QPixmap &pixmap_l);
    ~mIcon();
    void setSizeList(const QList <int> &list);
    void setIconFileName(const QString &filename);
    void loadFile(const QString &filename);
    void creatIconFile();

private:
    void initIconFormat();
    bool initSubBMPImg(const QPixmap &pixmap_l);
    bool makeBMPRGBQuad();
    bool makeBMPXorMask();
    bool makeBMPAndMask();
    bool makeBMPInfoHeader();
    bool makeIconDirEntry();
    bool makeIconMap();
    bool makeIconFileHeader();
    bool makeIconData();
    bool save();
    void clearMemory();

private:
    QPixmap *m_pixmap;
    QImage *m_imgOrig;
    QImage *m_imgMask;
    QImage *m_imgAlpha;
    QList <int> *m_sizeList;
    QList <int> *m_bppList;
    QList <ICONIMAGEMAP> *m_iconImageMapList;
    QString *m_fileName;

    QByteArray *m_iconDir;

    ICONDIRENTRY *m_iconDirEn;
    QByteArray *m_bmpHeader;
    QByteArray *m_bmpRGBQuad;
    QByteArray *m_bmpXorMask;
    QByteArray *m_bmpAndMask;
    int m_width;
    int m_height;
    int m_bmpBitCount;

    QByteArray *m_iconData;
//    ICONDIRENTRY *m_icondirentry;
//    ICONIMAGEMAP *m_iconimage;
};

#endif // MICON_H
