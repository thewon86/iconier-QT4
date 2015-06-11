#include "mbitmap.h"
#include <QBuffer>
#include <QByteArray>
#include <QPixmap>
#include <QBitmap>
#include <QFile>

//static int index = 0;

mBitMap::mBitMap(const QPixmap &pixmap)
{
    m_pixmap = new QPixmap(pixmap);
//    MDEBUG(m_pixmap);
//    MDEBUG(m_pixmap->width());
//    MDEBUG(m_pixmap->height());
    m_bmpRGBQuad_arr = new QByteArray;
    m_bmpData_arr = new QByteArray;
    m_bmpRGBQuad = new RGBQUAD;
}

mBitMap::mBitMap(const QBitmap &bitmap)
{
    QBitmap bitmap_t = bitmap;
//    MDEBUG(&bitmap_t);
//    MDEBUG(bitmap_t.width());
//    MDEBUG(bitmap_t.colorCount());
//    MDEBUG(m_pixmap);
    m_pixmap = new QPixmap;
//    MDEBUG(m_pixmap);
    *m_pixmap = *dynamic_cast<QPixmap*>(&bitmap_t);
//    MDEBUG(m_pixmap);
//    MDEBUG(m_pixmap->width());
//    MDEBUG(m_pixmap->height());
    m_bmpRGBQuad_arr = new QByteArray;
    m_bmpData_arr = new QByteArray;
    m_bmpRGBQuad = new RGBQUAD;
}

int mBitMap::parserImg()
{
    QBuffer bmpBuff_t;
    bmpBuff_t.open(QBuffer::ReadWrite);
//    MDEBUG(m_pixmap->width());
//    MDEBUG(m_pixmap->height());

    m_pixmap->save(QString("xxxbmp%1.bmp").arg(m_pixmap->size().height()), "bmp");

    bool save = m_pixmap->save(static_cast<QIODevice *>(&bmpBuff_t), "bmp");
    MDEBUG(save);
    if(!save){
        return -1;
    }

//    MDEBUG(bmpBuff_t.size());
    bmpBuff_t.seek(0);
    int size_t = bmpBuff_t.read((char *)&m_bmpFileHeader, sizeof(m_bmpFileHeader));
//    MDEBUG(size_t);
    MDEBUG(m_bmpFileHeader.bfType);
    MDEBUG(m_bmpFileHeader.bfSize);
    MDEBUG(m_bmpFileHeader.bfOffBits);
    if(size_t != sizeof(m_bmpFileHeader)){
        MMASSGE(read bmp file header error!!!);
        return -1;
    }
    if(m_bmpFileHeader.bfType != 0x4D42){
        MMASSGE(read bmp bfType error!!!);
        return -1;
    }
    if(m_bmpFileHeader.bfSize != bmpBuff_t.size()){
        MMASSGE(read bmp bfSize error!!!);
        return -1;
    }

//        bmpBuff_t.seek(BITMAPFILEHEADER);   // must be bmp header struct size
//    BITMAPINFOHEADER m_bmpInfoHeader;
    size_t = bmpBuff_t.read((char *)&m_bmpInfoHeader, sizeof(m_bmpInfoHeader));
//    MDEBUG(size_t);
    MDEBUG(m_bmpInfoHeader.biSize);
    MDEBUG(m_bmpInfoHeader.biWidth);
    MDEBUG(m_bmpInfoHeader.biHeight);
    MDEBUG(m_bmpInfoHeader.biPlanes);
    MDEBUG(m_bmpInfoHeader.biBitCount);
    MDEBUG(m_bmpInfoHeader.biSizeImage);
    if(size_t != sizeof(m_bmpInfoHeader)){
        MMASSGE(read bmp info header error!!!);
        return -1;
    }
    if(m_bmpInfoHeader.biSize != 0x28){
        MMASSGE(read bmp info header biSize error!!!);
        return -1;
    }
//    m_bmpInfoHeader.biHeight *= 2;

//    MDEBUG(m_bmpInfoHeader.biBitCount);
//    QByteArray bmpHeader;
//    bmpHeader.append((char *)&m_bmpInfoHeader, sizeof(m_bmpInfoHeader));

    m_rgbQuadNumb = 0;
    switch (m_bmpInfoHeader.biBitCount) {
        case 1:
            m_rgbQuadNumb = 2;              //2^1;
        break;
        case 4:

        break;
        case 8:
            m_rgbQuadNumb = 256;            //2^8;
        break;
        case 16:

        break;
        case 24:

        break;
        case 32:

        break;
        default:
        break;
    }
    if(0 != m_rgbQuadNumb){
        MDEBUG(m_rgbQuadNumb);
        delete m_bmpRGBQuad;
        m_bmpRGBQuad = new RGBQUAD[m_rgbQuadNumb];
        size_t = bmpBuff_t.read((char *)m_bmpRGBQuad, 4*m_rgbQuadNumb);
//        MDEBUG(size_t);
        if(size_t != 4*m_rgbQuadNumb){
            MMASSGE(read bmp rgbquad error!!!);
            return -1;
        }
        m_bmpRGBQuad_arr->append((char *)&m_bmpRGBQuad, 4*m_rgbQuadNumb);
    }

    if((m_bmpInfoHeader.biSize + sizeof(BITMAPFILEHEADER) + 4*m_rgbQuadNumb) != m_bmpFileHeader.bfOffBits){
        MDEBUG(m_bmpInfoHeader.biSize);
        MDEBUG(m_bmpFileHeader.bfOffBits);
        MMASSGE(read bmp file header bfOffBits error!!!);
        return -1;
    }

    size_t = m_bmpFileHeader.bfSize - (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 4*m_rgbQuadNumb);
    *m_bmpData_arr = bmpBuff_t.read(size_t);
    if(m_bmpData_arr->size() != size_t){
//        MDEBUG(bmpBuff_t.size());
//        MDEBUG(m_bmpData_arr->size());
        MMASSGE(read bmp file context error!!!);
        return -1;
    }

    bmpBuff_t.close();

//    m_pixmap->save(QString("xxxbmp%1.bmp").arg(m_pixmap->size().height()));
    return 0;
}

QByteArray mBitMap::getBMPFileHeader()
{
    QByteArray bmpFileHeader;
    bmpFileHeader.append((char *)&m_bmpFileHeader, sizeof(m_bmpFileHeader));
    return bmpFileHeader;
}

QByteArray mBitMap::getBMPInfoHeader()
{
    QByteArray bmpInfoHeader;
    BITMAPINFOHEADER bmpInfoHeader_t = m_bmpInfoHeader;
    bmpInfoHeader_t.biHeight *= 2;
    bmpInfoHeader.append((char *)&bmpInfoHeader_t, sizeof(bmpInfoHeader_t));
    return bmpInfoHeader;
}

BITMAPINFOHEADER mBitMap::getBMPInfoHeader_struct()
{
    return m_bmpInfoHeader;
}

QByteArray mBitMap::getBMPRGBQuad()
{
    return *m_bmpRGBQuad_arr;
}

QByteArray mBitMap::getBMPData()
{
    return *m_bmpData_arr;
}

QByteArray mBitMap::getBMPMaskData()
{
    QByteArray m_bmpMaskData_arr;
//    m_bmpMaskData_arr.resize(m_pixmap->size().height() * m_pixmap->size().width());
//    m_bmpMaskData_arr.fill(100);
//    return m_bmpMaskData_arr;

    if(!m_pixmap->hasAlpha()){
        m_pixmap->setMask(m_pixmap->createHeuristicMask());
//        m_pixmap->setMask(m_pixmap->createMaskFromColor(QColor(255, 255, 255)));
    }
//    m_pixmap->setMask(m_pixmap->createMaskFromColor(QColor(255, 255, 255)));
//    QBitmap bitmap_t = m_pixmap->mask();
//    MDEBUG(m_pixmap->width());
//    MDEBUG(m_pixmap->height());
    QBitmap bitmap_t = m_pixmap->alphaChannel();
//    bitmap_t.fill(Qt::white);
    if(!bitmap_t.isNull()){
//        MDEBUG(bitmap_t.width());
//        MDEBUG(bitmap_t.colorCount());
        mBitMap bmpMask_t(bitmap_t);
        bmpMask_t.parserImg();
        m_bmpMaskData_arr = bmpMask_t.getBMPData();
        MDEBUG(m_bmpMaskData_arr.size());

//        QBuffer bmpBuff_t;
//        bmpBuff_t.open(QBuffer::ReadWrite);
//        bool save = bitmap_t.save(static_cast<QIODevice *>(&bmpBuff_t), "bmp");
////        MDEBUG(save);
//        if(save){
////            MDEBUG(bmpBuff_t.size());
//            static int ind = 0;
//            QFile maskFile(QString("xxxx%1.bmp").arg(ind));
//            maskFile.open(QFile::WriteOnly);
//            maskFile.write(bmpBuff_t.data());
//            maskFile.flush();
//            maskFile.close();
//            ind ++;
//        }
    }
    return m_bmpMaskData_arr;
}

int mBitMap::getRGBQuadNumb()
{
    return m_rgbQuadNumb;
}
