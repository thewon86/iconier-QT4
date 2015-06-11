#ifndef MBITMAP_H
#define MBITMAP_H

#include "bmptypes.h"
QT_BEGIN_NAMESPACE
class QPixmap;
class QBitmap;
class QByteArray;
QT_END_NAMESPACE

class mBitMap
{
public:
    mBitMap(const QPixmap &pixmap);
    mBitMap(const QBitmap &bitmap);
    int parserImg();
    QByteArray getBMPFileHeader();
    QByteArray getBMPInfoHeader();
    QByteArray getBMPRGBQuad();
    QByteArray getBMPData();
    QByteArray getBMPMaskData();
    BITMAPINFOHEADER getBMPInfoHeader_struct();
    int getRGBQuadNumb();

private:
    QPixmap *m_pixmap;
    BITMAPFILEHEADER m_bmpFileHeader;
    BITMAPINFOHEADER m_bmpInfoHeader;
    RGBQUAD *m_bmpRGBQuad;
    int m_rgbQuadNumb;
    QByteArray *m_bmpRGBQuad_arr;
    QByteArray *m_bmpData_arr;
};

#endif // MBITMAP_H
