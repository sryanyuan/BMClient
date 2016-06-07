#ifndef _MIRMAP_H_
#define _MIRMAP_H_

#include <Windows.h>
#include <list>

//#include "../GameTextureManager.h"

using std::list;
//////////////////////////////////////////////////////////////////////////
//#define BLOCK_MASK		0x80000000
//#define DOOR_MASK		0x40000000
#define BLOCK_MASK		0x80
#define DOOR_MASK		0x40
//////////////////////////////////////////////////////////////////////////

struct MapTile
{
	//	背景 大砖块 96*64 对应Tiles.wil 最高位是否能走动 1 不能走动
	WORD wBkImg;
	//	地表小砖块 48*32 对应SmTiles.wil
	WORD wMidImg;
	//	地面物体 对应Objectsx 最高位1不能走动
	WORD wFrImg;
	//	是否有门 最高位1代表有门 7位索引
	BYTE bDoorIndex;
	//	门的开关 最高位1开 0关
	BYTE bDoorOffset;
	//	动画效果 Draw alpha
	BYTE bAniFrame;
	//	阴影效果
	BYTE bAniTick;
	//	图标表层对应object索引
	BYTE bArea;
	//	雾的效果
	BYTE bLight;

	//	Ex	游戏用 非地图专门格式
	//	添加的 用来记录掉落装备
	DWORD wItem;
	//	怪物
	DWORD wMonster;
};

struct MapHeader
{
	//	  48*32 Col 列数 Row 行数
	WORD bCol;
	WORD bRow;
	char szTitle[16];
	BYTE UpdateTime[8];
	BYTE Reserve[24];
};

//	资源文件
struct WixHead
{
	char Title[44];
	DWORD IndexCount;
};

struct WilHead
{
	char Title[44];
	DWORD ImageCount;
	DWORD ColorCount;
	DWORD PaletteSize;
};

struct ImageInfo
{
	WORD Width;
	WORD Height;
	BYTE unknow[4];
};


class SWilFileEx
{
public:
	SWilFileEx(const char* lpszFileName);
	SWilFileEx();
	virtual ~SWilFileEx();

public:
	HBITMAP GetBitmap(HDC hDC, int _index);
	bool Load(const char* lpszFileName);
	void UnLoad();
	void ReleaseBitmaps();
	void Release(int _index);
	void SetUserManage()
	{
		m_bUserManage = true;
	}
	int GetBitmapCount() const
	{
		return m_stWilHead.ImageCount;
	}

protected:
	HBITMAP CreateDIBitmapFromData(HDC hDevice, PBITMAPINFO lpBitmapInfo, BYTE* pData);

protected:
	HBITMAP* m_bitmaps;
	WixHead m_stWixHead;
	WilHead m_stWilHead;
	char m_szWilPath[MAX_PATH];
	char m_szWixPath[MAX_PATH];
	DWORD* m_pIndexTable;
	DWORD* m_pPalette;
	list<int> m_lstGetInfo;
	HANDLE m_hWilFile;
	bool m_bUserManage;
};

class SWilFile
{
public:
	SWilFile(const char* lpszFileName);
	SWilFile();
	virtual ~SWilFile();

public:
	HBITMAP GetBitmap(HDC hDC, int _index);
	virtual bool Load(const char* lpszFileName);
	void UnLoad();
	void Release(int _index);
	int GetBitmapCount() const
	{
		return m_stWilHead.ImageCount;
	}

protected:
	HBITMAP CreateDIBitmapFromData(HDC hDevice, PBITMAPINFO lpBitmapInfo, BYTE* pData);
	HBITMAP GetBitmap8(HDC hDC, int _index);
	HBITMAP GetBitmap16(HDC hDC, int _index);
//	bool Extract(int _index);

//protected:
	bool ExtractBitmap8(const char* _pszDestFile, int _index);
//	bool ExtaactBitmap16(int _index);

protected:
	WixHead m_stWixHead;
	WilHead m_stWilHead;
	char m_szWilPath[MAX_PATH];
	char m_szWixPath[MAX_PATH];
	DWORD* m_pIndexTable;
	DWORD* m_pPalette;
	HANDLE m_hWilFile;
};

//	地图
struct MapInfo
{
	int nRow;
	int nCol;
	int nWidth;
	int nHeight;
};

class TileMap
{
public:
	TileMap();
	TileMap(int _row, int _col, int _width, int _height);
	~TileMap();

public:
	void CreateMap(int _row, int _col, int _width, int _height);
	void DestroyMap();

	bool LoadMap(const char* lpszFilename);
	bool LoadMapReadOnly(const char* lpszFilename);
	bool SaveMap(const char* lpszFilename);

	bool GetMapSnapShot(const char* _lpszFilename, DWORD** _pOutput, MapInfo* _pInfo);

	bool SetHeaderData(const char* lpszTitle, const char* lpszReserve);

	void GetMapInfo(MapInfo& _info);
	MapTile* GetCellData(int _row, int _col);

	bool IsDataInside()
	{
		return m_pTileData == NULL ? false : true;
	}

	const char* GetMapName() const
	{
		return m_szMapName;
	}

	int GetResIndex()
	{
		return *(int*)&m_stHeader.Reserve[0];
	}
	void SetResIndex(int _nResIndex)
	{
		*(int*)&m_stHeader.Reserve[0] = _nResIndex;
	}

private:
	void LoadMapName(const char* lpszFilname);

private:
	//	data
	MapTile** m_pTileData;
	//	header
	MapHeader m_stHeader;
	//	info
	int m_nWidth;
	int m_nHeight;
	char m_szMapName[20];
};


#endif