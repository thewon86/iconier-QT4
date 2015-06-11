#ifndef __BMPTYPES_H
#define __BMPTYPES_H

#include "mytypedef.h"

#pragma pack(push,2)
//#pragma pack(2)
typedef struct tagBITMAPFILEHEADER{
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
}/*__attribute__((packed)) */BITMAPFILEHEADER, *pBITMAPFILEHEADER;			//__attribute__((packed)) FAR *LPBITMAPFILEHEADER,
#pragma pack(pop)
typedef struct tagBITMAPINFOHEADER{
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, *pBITMAPINFOHEADER;		// FAR *LPBITMAPINFOHEADER,

typedef struct tagRGBQUAD{
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD,*pRGBQUAD;
#endif
