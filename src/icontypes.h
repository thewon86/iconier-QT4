#ifndef ICONTYPES_H
#define ICONTYPES_H

#include "mytypedef.h"
#include "bmptypes.h"

#pragma pack( push, 2)
typedef struct tagICONDIR{
    WORD    idReserved;             // Reserved
    WORD    idType;                 // resource type (1 for icons)
    WORD    idCount;                // how many images?
} ICONDIR, *pICONDIR;
typedef struct tagICONDIRENTRY{
    BYTE	ibWidth;                 // Width, in pixels, of the image
    BYTE	ibHeight;                // Height, in pixels, of the image
    BYTE	ibColorCount;            // Number of colors in image (0 if >=8bpp)
    BYTE	ibReserved;              // Reserved ( must be 0)
    WORD	iwPlanes;                // Color Planes
    WORD	iwBitCount;              // Bits per pixel
    DWORD	idwBytesInRes;           // how many bytes in this resource?
    DWORD	idwImageOffset;          // where in the file is this image?
} ICONDIRENTRY, *pICONDIRENTRY;
#pragma pack( pop )

/*
typedef struct{
    BitmapInfoHeader    icheader;      // dib header
    RGBQuad             iccolors[1];   // color table
    byte                icxor[1];      // dib bits for xor mask
    byte                icand[1];      // dib bits for and mask
} iconimage, *lpiconimage;
*/
#endif // ICONTYPES_H
