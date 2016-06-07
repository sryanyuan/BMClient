#ifndef _INC_SIBDEFINE_
#define _INC_SIBDEFINE_
//////////////////////////////////////////////////////////////////////////
//	This file is a header file of sryan's image box module
//	sryan all rights reserved
//////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <list>
#include <vector>
#include <string>
//////////////////////////////////////////////////////////////////////////
using std::string;
typedef std::vector<string> FILENAMES;
typedef std::vector<int> FILENUMBERS;
//////////////////////////////////////////////////////////////////////////
#define PACK_OK				0
#define PACK_DRTNOTFOUND	1
#define PACK_INVALID_FILE	2
#define PACK_NOFILE			3
#define PACK_NEWFILE		4
#define PACK_NOTNUMBER		5
#define PACK_OUTFILE		6
#define PACK_NODATA			7

#define BITMAP8_DATAPOS		0x436
#define BITMAP_16BITMASK	0x80000000
//////////////////////////////////////////////////////////////////////////
struct SIBPackHeader
{
	//	size:30+2+2=34
	char szInfo[30];
	unsigned short uImgCount;
	//	The colours in palette, if zero stands for no palette, for 8 bits
	//	Bitmap, it is always 256, if 16bits bitmaps, it is 3,if the pack
	//	have all above colors, it is 256+3
	unsigned short uPalette;
};

struct SIBImgInfo
{
	//	size:4+2+2+2+2=12
	//	If uImgPointer is zero, the destinate image doesn't exist
	//	So the other 4 varibles doesn't exist
	//	The highest bit is a bits mask,0 for 8bits, 1 for 16bits
	unsigned int uImgPointer;
	unsigned short uImgWidth;
	unsigned short uImgHeight;
	short sImgOftX;
	short sImgOftY;
};
//////////////////////////////////////////////////////////////////////////
#endif