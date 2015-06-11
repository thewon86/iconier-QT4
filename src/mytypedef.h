#ifndef MYTYPEDEF_H
#define MYTYPEDEF_H

#include <QDebug>

#ifdef _MAPPDEBUG
#define STR_X(x)    #x
#define MDEBUG(x)   qDebug() << "LINE:" << __LINE__ << ">>> " << STR_X(x) << "=" << x
#define MMASSGE(x)    qDebug() << "LINE:" << __LINE__ << ">>> " << STR_X(x)
#endif

typedef unsigned char 	BYTE;
typedef unsigned char*	PBYTE;
typedef unsigned short 	WORD;
typedef unsigned int 	DWORD;
typedef int 			LONG;

#endif // MYTYPEDEF_H
