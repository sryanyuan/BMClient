//#include "stdafx.h"
#include "MirMap.h"
#include <assert.h>
#include "SKnl3Helper.h"
#include <io.h>

TileMap::TileMap()
{
	m_nHeight = m_nWidth = 0;
	m_pTileData = NULL;
	memset(&m_stHeader, sizeof(MapHeader), 0);
	ZeroMemory(m_szMapName, sizeof(m_szMapName));
}

TileMap::TileMap(int _row, int _col, int _width, int _height)
{
	m_stHeader.bRow = _row;
	m_stHeader.bCol = _col;
	m_nWidth = _width;
	m_nHeight = _height;
	m_pTileData = NULL;
	memset(&m_stHeader, sizeof(MapHeader), 0);
	ZeroMemory(m_szMapName, sizeof(m_szMapName));
}

TileMap::~TileMap()
{

}





void TileMap::LoadMapName(const char* lpszFilname)
{
	char szbuf[MAX_PATH];
	SKNL3GetCurrentAppPath(szbuf, MAX_PATH);
	strcat(szbuf, "Config\\map.ini");
	int nIndex = 0;
	ZeroMemory(m_szMapName, sizeof(m_szMapName));
	bool bFindPoint = false;

	for(int i = strlen(lpszFilname) - 1; i > 0; --i)
	{
		if(lpszFilname[i] == '\\')
		{
			break;
		}

		//if(lpszFilname[i] > '9' ||
		//	lpszFilname[i] < '0')
		//{
		//	//	不是数字
		//	continue;
		//}
		if(lpszFilname[i] == '.')
		{
			bFindPoint = true;
			continue;
		}
		
		if(bFindPoint)
		{
			m_szMapName[nIndex++] = lpszFilname[i];
		}
	}

	char cTmp = 0;
	//	交换顺序
	for(int i = 0; i < (strlen(m_szMapName)) / 2; ++i)
	{
		cTmp = m_szMapName[i];
		m_szMapName[i] = m_szMapName[strlen(m_szMapName) - 1 - i];
		m_szMapName[strlen(m_szMapName) - 1 - i] = cTmp;
	}
	char szKey[20];
	strcpy(szKey, m_szMapName);

	::GetPrivateProfileString("MapNameInfo", szKey, "0", m_szMapName, sizeof(m_szMapName), szbuf);
	if(strcmp(m_szMapName, "0") == 0)
	{
		strcpy(m_szMapName, "未知地图");
	}
}

void TileMap::CreateMap(int _row, int _col, int _width, int _height)
{
	DestroyMap();

	m_nWidth = _width;
	m_nHeight = _height;
	m_stHeader.bRow = _row;
	m_stHeader.bCol = _col;

	m_pTileData = new MapTile*[m_stHeader.bRow];

	for(int i = 0; i < m_stHeader.bRow; ++i)
	{
		m_pTileData[i] = new MapTile[m_stHeader.bCol];
	}
	for(int i = 0; i < m_stHeader.bRow; ++i)
	{
		for(int j = 0; j < m_stHeader.bCol; ++j)
		{
			//memset((void*)GetCellData(i, j), sizeof(MapTile), 0);
			ZeroMemory(GetCellData(i, j), sizeof(MapTile));
		}
	}
}

void TileMap::DestroyMap()
{
	if(m_pTileData)
	{
		for(int i = 0; i < m_stHeader.bRow; ++i)
		{
			delete [] m_pTileData[i];
		}
		delete [] m_pTileData;
	}
	m_pTileData = NULL;
	m_stHeader.bRow = m_stHeader.bCol = 0;
}

void TileMap::GetMapInfo(MapInfo& _info)
{
	_info.nCol = m_stHeader.bCol;
	_info.nRow = m_stHeader.bRow;
	_info.nWidth = m_nWidth;
	_info.nHeight = m_nHeight;
}

MapTile* TileMap::GetCellData(int _row, int _col)
{
	if(_row >= m_stHeader.bRow ||
		_col >= m_stHeader.bCol)
	{
		return NULL;
	}

	return &m_pTileData[_row][_col];
}

bool TileMap::GetMapSnapShot(const char* _lpszFilename, DWORD** _pOutput, MapInfo* _pInfo)
{
	if(_access(_lpszFilename, 0) == -1)
	{
		//	文件不存在
		return false;
	}

	HANDLE hFile = SKNL3CreateMemoryFileMapping(_lpszFilename);
	if(hFile == NULL)
	{
		return false;
	}

	if(m_pTileData)
	{
		DestroyMap();
	}

	//	读头部
	const char* pData = (const char*)hFile;
	m_stHeader.bCol = *((WORD*)pData);
	pData += 2;
	m_stHeader.bRow = *((WORD*)pData);
	pData += 2;

	for(int i = 0; i < sizeof(m_stHeader.szTitle); ++i)
	{
		m_stHeader.szTitle[i] = *pData;
		++pData;
	}
	for(int i = 0; i < sizeof(m_stHeader.UpdateTime); ++i)
	{
		m_stHeader.UpdateTime[i] = *pData;
		++pData;
	}
	for(int i = 0; i < sizeof(m_stHeader.Reserve); ++i)
	{
		m_stHeader.Reserve[i] = *pData;
		++pData;
	}

	//	验证数据大小
	int nFileSize = SKNL3GetFileSize(_lpszFilename);
	int nTileOffset = 0;
	if(52 + 12 * m_stHeader.bCol * m_stHeader.bRow != nFileSize)
	{
		//UnmapViewOfFile(hFile);
		//return false;
		//	每个tile的字节不是12字节了
		int nOffset = 0;
		nOffset = (nFileSize - 52) /*/ (m_stHeader.bCol * m_stHeader.bRow)*/;
		if(nOffset % (m_stHeader.bCol * m_stHeader.bRow) != 0)
		{
			UnmapViewOfFile(hFile);
			return false;
		}

		nOffset /= (m_stHeader.bCol * m_stHeader.bRow);
		nOffset -= 12;

		if(nOffset < 0)
		{
			UnmapViewOfFile(hFile);
			return false;
		}

		nTileOffset = nOffset;
	}

	//	读地图数据	传奇标准地图按列存放	这里按列读取
	//CreateMap(m_stHeader.bRow, m_stHeader.bCol, 48, 32);
	(*_pOutput) = new DWORD[m_stHeader.bRow * m_stHeader.bCol];
	MapTile stTile;
	DWORD dwValue = 0;
	//assert(m_pTileData != NULL);

	for(int i = 0; i < m_stHeader.bCol; ++i)
	{
		for(int j = 0; j < m_stHeader.bRow; ++j)
		{
			ZeroMemory(&stTile, sizeof(stTile));
			dwValue = 0;
			//	WORD
			stTile.wBkImg = *((WORD*)pData);
			pData += sizeof(WORD);
			stTile.wMidImg = *((WORD*)pData);
			pData += sizeof(WORD);
			stTile.wFrImg = *((WORD*)pData);
			pData += sizeof(WORD);

			//	BYTE
			stTile.bDoorIndex = *((BYTE*)pData);
			pData += sizeof(BYTE);
			stTile.bDoorOffset = *((BYTE*)pData);
			pData += sizeof(BYTE);
			stTile.bAniFrame = *((BYTE*)pData);
			pData += sizeof(BYTE);
			stTile.bAniTick = *((BYTE*)pData);
			pData += sizeof(BYTE);
			stTile.bArea = *((BYTE*)pData);
			pData += sizeof(BYTE);
			stTile.bLight = *((BYTE*)pData);
			pData += sizeof(BYTE);

			//	偏移offset
			if(nTileOffset > 0)
			{
				pData += nTileOffset;
			}

			//	Get type
			if(stTile.wBkImg & 0x8000)
			{
				dwValue |= BLOCK_MASK;
			}
			if(stTile.wFrImg & 0x8000)
			{
				dwValue |= BLOCK_MASK;
			}

			(*_pOutput)[j * m_stHeader.bCol + i] = dwValue;
		}
	}
	assert((int)pData == (int)hFile + nFileSize);

	//	释放
	::UnmapViewOfFile(hFile);
	GetMapInfo(*_pInfo);

	//	读地图名称
	//LoadMapName(lpszFilename);
	return true;
}

bool TileMap::LoadMap(const char* lpszFilename)
{
	if(_access(lpszFilename, 0) == -1)
	{
		//	文件不存在
		return false;
	}

	HANDLE hFile = SKNL3CreateMemoryFileMapping(lpszFilename);
	if(hFile == NULL)
	{
		return false;
	}

	if(m_pTileData)
	{
		DestroyMap();
	}

	//	读头部
	const char* pData = (const char*)hFile;
	m_stHeader.bCol = *((WORD*)pData);
	pData += 2;
	m_stHeader.bRow = *((WORD*)pData);
	pData += 2;

	for(int i = 0; i < sizeof(m_stHeader.szTitle); ++i)
	{
		m_stHeader.szTitle[i] = *pData;
		++pData;
	}
	for(int i = 0; i < sizeof(m_stHeader.UpdateTime); ++i)
	{
		m_stHeader.UpdateTime[i] = *pData;
		++pData;
	}
	for(int i = 0; i < sizeof(m_stHeader.Reserve); ++i)
	{
		m_stHeader.Reserve[i] = *pData;
		++pData;
	}

	//	验证数据大小
	int nFileSize = SKNL3GetFileSize(lpszFilename);
	int nTileOffset = 0;
	if(52 + 12 * m_stHeader.bCol * m_stHeader.bRow != nFileSize)
	{
		//UnmapViewOfFile(hFile);
		//return false;
		//	每个tile的字节不是12字节了
		int nOffset = 0;
		nOffset = (nFileSize - 52) /*/ (m_stHeader.bCol * m_stHeader.bRow)*/;
		if(nOffset % (m_stHeader.bCol * m_stHeader.bRow) != 0)
		{
			UnmapViewOfFile(hFile);
			return false;
		}

		nOffset /= (m_stHeader.bCol * m_stHeader.bRow);
		nOffset -= 12;

		if(nOffset < 0)
		{
			UnmapViewOfFile(hFile);
			return false;
		}

		nTileOffset = nOffset;
	}

	//	读地图数据	传奇标准地图按列存放	这里按列读取
	CreateMap(m_stHeader.bRow, m_stHeader.bCol, 48, 32);
	assert(m_pTileData != NULL);

	for(int i = 0; i < m_stHeader.bCol; ++i)
	{
		for(int j = 0; j < m_stHeader.bRow; ++j)
		{
			//	WORD
			m_pTileData[j][i].wBkImg = *((WORD*)pData);
			pData += sizeof(WORD);
			m_pTileData[j][i].wMidImg = *((WORD*)pData);
			pData += sizeof(WORD);
			m_pTileData[j][i].wFrImg = *((WORD*)pData);
			pData += sizeof(WORD);

			//	BYTE
			m_pTileData[j][i].bDoorIndex = *((BYTE*)pData);
			pData += sizeof(BYTE);
			m_pTileData[j][i].bDoorOffset = *((BYTE*)pData);
			pData += sizeof(BYTE);
			m_pTileData[j][i].bAniFrame = *((BYTE*)pData);
			pData += sizeof(BYTE);
			m_pTileData[j][i].bAniTick = *((BYTE*)pData);
			pData += sizeof(BYTE);
			m_pTileData[j][i].bArea = *((BYTE*)pData);
			pData += sizeof(BYTE);
			m_pTileData[j][i].bLight = *((BYTE*)pData);
			pData += sizeof(BYTE);

			//	偏移offset
			if(nTileOffset > 0)
			{
				pData += nTileOffset;
			}
		}
	}
	assert((int)pData == (int)hFile + nFileSize);

	//	释放
	::UnmapViewOfFile(hFile);

	//	读地图名称
	LoadMapName(lpszFilename);
	return true;
}

bool TileMap::LoadMapReadOnly(const char* lpszFilename)
{
	if(_access(lpszFilename, 0) == -1)
	{
		//	文件不存在
		return false;
	}

	HANDLE hFile = SKNL3CreateMemoryFileMappingReadOnly(lpszFilename);
	if(hFile == NULL)
	{
		return false;
	}

	if(m_pTileData)
	{
		DestroyMap();
	}

	//	读头部
	const char* pData = (const char*)hFile;
	m_stHeader.bCol = *((WORD*)pData);
	pData += 2;
	m_stHeader.bRow = *((WORD*)pData);
	pData += 2;

	for(int i = 0; i < sizeof(m_stHeader.szTitle); ++i)
	{
		m_stHeader.szTitle[i] = *pData;
		++pData;
	}
	for(int i = 0; i < sizeof(m_stHeader.UpdateTime); ++i)
	{
		m_stHeader.UpdateTime[i] = *pData;
		++pData;
	}
	for(int i = 0; i < sizeof(m_stHeader.Reserve); ++i)
	{
		m_stHeader.Reserve[i] = *pData;
		++pData;
	}

	//	验证数据大小
	int nFileSize = SKNL3GetFileSize(lpszFilename);
	int nTileOffset = 0;
	if(52 + 12 * m_stHeader.bCol * m_stHeader.bRow != nFileSize)
	{
		//UnmapViewOfFile(hFile);
		//return false;
		//	每个tile的字节不是12字节了
		int nOffset = 0;
		nOffset = (nFileSize - 52) /*/ (m_stHeader.bCol * m_stHeader.bRow)*/;
		if(nOffset % (m_stHeader.bCol * m_stHeader.bRow) != 0)
		{
			UnmapViewOfFile(hFile);
			return false;
		}

		nOffset /= (m_stHeader.bCol * m_stHeader.bRow);
		nOffset -= 12;

		if(nOffset < 0)
		{
			UnmapViewOfFile(hFile);
			return false;
		}

		nTileOffset = nOffset;
	}

	//	读地图数据	传奇标准地图按列存放	这里按列读取
	CreateMap(m_stHeader.bRow, m_stHeader.bCol, 48, 32);
	assert(m_pTileData != NULL);

	for(int i = 0; i < m_stHeader.bCol; ++i)
	{
		for(int j = 0; j < m_stHeader.bRow; ++j)
		{
			//	WORD
			m_pTileData[j][i].wBkImg = *((WORD*)pData);
			pData += sizeof(WORD);
			m_pTileData[j][i].wMidImg = *((WORD*)pData);
			pData += sizeof(WORD);
			m_pTileData[j][i].wFrImg = *((WORD*)pData);
			pData += sizeof(WORD);

			//	BYTE
			m_pTileData[j][i].bDoorIndex = *((BYTE*)pData);
			pData += sizeof(BYTE);
			m_pTileData[j][i].bDoorOffset = *((BYTE*)pData);
			pData += sizeof(BYTE);
			m_pTileData[j][i].bAniFrame = *((BYTE*)pData);
			pData += sizeof(BYTE);
			m_pTileData[j][i].bAniTick = *((BYTE*)pData);
			pData += sizeof(BYTE);
			m_pTileData[j][i].bArea = *((BYTE*)pData);
			pData += sizeof(BYTE);
			m_pTileData[j][i].bLight = *((BYTE*)pData);
			pData += sizeof(BYTE);

			//	偏移offset
			if(nTileOffset > 0)
			{
				pData += nTileOffset;
			}
		}
	}
	assert((int)pData == (int)hFile + nFileSize);

	//	释放
	::UnmapViewOfFile(hFile);

	//	读地图名称
	LoadMapName(lpszFilename);
	return true;
}

bool TileMap::SaveMap(const char* lpszFilename)
{
	if(m_pTileData == NULL)
	{
		return false;
	}

	HANDLE hFile = ::CreateFile(lpszFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD nCount = 0;
	DWORD nTotalCount = 0;
	//	写文件头
	if(strlen(m_stHeader.szTitle) == 0)
	{
		strcpy(m_stHeader.szTitle, "BackMir");
	}
	if(strlen((const char*)m_stHeader.Reserve) == 0)
	{
		strcpy((char*)m_stHeader.Reserve, "仅供学习");
	}

	//	先写列数再写行数
	if(!::WriteFile(hFile, &m_stHeader.bCol, sizeof(m_stHeader.bCol), &nCount, NULL))
	{
		goto _CLEANUP_;
	}
	nTotalCount += nCount;
	if(!::WriteFile(hFile, &m_stHeader.bRow, sizeof(m_stHeader.bRow), &nCount, NULL))
	{
		goto _CLEANUP_;
	}
	nTotalCount += nCount;
	//	其余信息
	if(!::WriteFile(hFile, &m_stHeader.szTitle, sizeof(m_stHeader.szTitle), &nCount, NULL))
	{
		goto _CLEANUP_;
	}
	nTotalCount += nCount;
	if(!::WriteFile(hFile, &m_stHeader.UpdateTime, sizeof(m_stHeader.UpdateTime), &nCount, NULL))
	{
		goto _CLEANUP_;
	}
	nTotalCount += nCount;
	if(!::WriteFile(hFile, &m_stHeader.Reserve, sizeof(m_stHeader.Reserve), &nCount, NULL))
	{
		goto _CLEANUP_;
	}
	nTotalCount += nCount;

	assert(nTotalCount == 52);
	nTotalCount = 0;

	//	写地图数据 一列一列写
	for(int i = 0; i < m_stHeader.bCol; ++i)
	{
		for(int j = 0; j < m_stHeader.bRow; ++j)
		{
			if(!::WriteFile(hFile, &m_pTileData[j][i].wBkImg, sizeof(WORD), &nCount, NULL))
			{
				goto _CLEANUP_;
			}
			nTotalCount += nCount;
			if(!::WriteFile(hFile, &m_pTileData[j][i].wMidImg, sizeof(WORD), &nCount, NULL))
			{
				goto _CLEANUP_;
			}
			nTotalCount += nCount;
			if(!::WriteFile(hFile, &m_pTileData[j][i].wFrImg, sizeof(WORD), &nCount, NULL))
			{
				goto _CLEANUP_;
			}
			nTotalCount += nCount;
			if(!::WriteFile(hFile, &m_pTileData[j][i].bDoorIndex, sizeof(BYTE), &nCount, NULL))
			{
				goto _CLEANUP_;
			}
			nTotalCount += nCount;
			if(!::WriteFile(hFile, &m_pTileData[j][i].bDoorOffset, sizeof(BYTE), &nCount, NULL))
			{
				goto _CLEANUP_;
			}
			nTotalCount += nCount;
			if(!::WriteFile(hFile, &m_pTileData[j][i].bAniFrame, sizeof(BYTE), &nCount, NULL))
			{
				goto _CLEANUP_;
			}
			nTotalCount += nCount;
			if(!::WriteFile(hFile, &m_pTileData[j][i].bAniTick, sizeof(BYTE), &nCount, NULL))
			{
				goto _CLEANUP_;
			}
			nTotalCount += nCount;
			if(!::WriteFile(hFile, &m_pTileData[j][i].bArea, sizeof(BYTE), &nCount, NULL))
			{
				goto _CLEANUP_;
			}
			nTotalCount += nCount;
			if(!::WriteFile(hFile, &m_pTileData[j][i].bLight, sizeof(BYTE), &nCount, NULL))
			{
				goto _CLEANUP_;
			}
			nTotalCount += nCount;
		}
	}
	assert(nTotalCount == m_stHeader.bCol * m_stHeader.bRow * 12);

	::CloseHandle(hFile);
	return true;

_CLEANUP_:
	::CloseHandle(hFile);
	return false;
}