#include "MapSurface.h"
#include "../GameScene/GameMapManager.h"
#include "../GameScene/GameResourceManager.h"

//////////////////////////////////////////////////////////////////////////
GameMapManager* MapSurface::s_pMapMgr = NULL;

MapSurface::MapSurface()
{
	if(NULL == s_pMapMgr)
	{
		s_pMapMgr = GameMapManager::GetInstance();
	}

	InitBuffer(VIEW_WIDTH + (LEFT_OFFSET_COL + RIGHT_OFFSET_COL) * UNIT_WIDTH,
		VIEW_HEIGHT + (TOP_OFFSET_ROW + BOTTOM_OFFSET_ROW) * UNIT_HEIGHT);
}

MapSurface::~MapSurface()
{

}


//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 第一次绘制 需要全部绘制
/************************************************************************/
bool MapSurface::DrawEntire()
{
	for(int i = 0; i < m_stRect.y / UNIT_HEIGHT; ++i)
	{
		DrawOneRow(i);
	}

	return true;
}

/************************************************************************/
/* 滚动地图
/************************************************************************/
bool MapSurface::ScrollMap(int _nRowOft, int _nColOft)
{
	Offset(_nColOft * UNIT_WIDTH, _nRowOft * UNIT_HEIGHT);

	int nReDrawRow = _nRowOft;
	int nReDrawCol = _nColOft;

	if(nReDrawCol != 0)
	{
		if(nReDrawCol > 0)
		{
			for(int i = 0; i < nReDrawCol; ++i)
			{
				DrawOneCol(i);
			}
		}
		else if(nReDrawCol < 0)
		{
			nReDrawCol += m_stRect.x / UNIT_WIDTH;
			for(int i = m_stRect.x / UNIT_WIDTH - 1; i >= nReDrawCol; --i)
			{
				DrawOneCol(i);
			}
		}
	}

	if(nReDrawRow != 0)
	{
		if(nReDrawRow > 0)
		{
			for(int i = 0; i < nReDrawRow; ++i)
			{
				DrawOneRow(i);
			}
		}
		else if(nReDrawRow < 0)
		{
			nReDrawRow += m_stRect.y / UNIT_HEIGHT;
			for(int i = m_stRect.y / UNIT_HEIGHT - 1; i >= nReDrawRow; --i)
			{
				DrawOneRow(i);
			}
		}
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 绘制一行(缓存的行)
/************************************************************************/
void MapSurface::DrawOneCol(int _nCol)
{
	int nRealCol = s_pMapMgr->GetRect().left + _nCol - LEFT_OFFSET_COL;
	int nRealRow = 0;
	short sTile = 0;
	GameTextureManager* pTexBkMgr = GameResourceManager::GetInstance()->GetTexs(RES_TILES);
	GameTextureManager* pTexMdMgr = GameResourceManager::GetInstance()->GetTexs(RES_SMTILES);
	HTEXTURE tex = 0;

	MapInfo stInfo;
	TileMap* pMapData = s_pMapMgr->GetMapData();
	pMapData->GetMapInfo(stInfo);

	if(nRealCol < 0 ||
		nRealCol >= stInfo.nCol)
	{
		return;
	}

	for(int i = 0; i < m_stRect.y / UNIT_HEIGHT; ++i)
	{
		nRealRow = s_pMapMgr->GetRect().top + i - TOP_OFFSET_ROW;
		if(nRealRow < 0 ||
			nRealRow >= stInfo.nRow)
		{
			continue;
		}

		if(nRealRow % 2 == 0 &&
			nRealCol % 2 == 0)
		{
			//	背景
			if(pTexBkMgr)
			{
				sTile = pMapData->GetCellData(nRealRow, nRealCol)->wBkImg;
				sTile &= (~0x8000);
				sTile -= 1;

				if(sTile >= 0)
				{
					tex = pTexBkMgr->GetTexture(sTile);
					if(tex)
					{
						DrawTextureDirect(tex, _nCol * UNIT_WIDTH, i * UNIT_HEIGHT, UNIT_WIDTH * 2, UNIT_HEIGHT * 2);
					}
				}
			}
		}

		//	中景
		if(pTexMdMgr)
		{
			sTile = pMapData->GetCellData(nRealRow, nRealCol)->wMidImg;
			sTile &= (~0x8000);
			sTile -= 1;

			if(sTile >= 0)
			{
				tex = pTexMdMgr->GetTexture(sTile);
				if(tex)
				{
					DrawTextureDirect(tex, _nCol * UNIT_WIDTH, i * UNIT_HEIGHT, UNIT_WIDTH, UNIT_HEIGHT);
				}
			}
		}
	}
}

/************************************************************************/
/* 绘制一列
/************************************************************************/
void MapSurface::DrawOneRow(int _nRow)
{
	int nRealRow = s_pMapMgr->GetRect().top + _nRow - TOP_OFFSET_ROW;
	int nRealCol = 0;
	short sTile = 0;
	GameTextureManager* pTexBkMgr = GameResourceManager::GetInstance()->GetTexs(RES_TILES);
	GameTextureManager* pTexMdMgr = GameResourceManager::GetInstance()->GetTexs(RES_SMTILES);
	HTEXTURE tex = 0;

	MapInfo stInfo;
	TileMap* pMapData = s_pMapMgr->GetMapData();
	pMapData->GetMapInfo(stInfo);

	if(nRealRow < 0 ||
		nRealRow >= stInfo.nRow)
	{
		return;
	}

	for(int i = 0; i < m_stRect.x / UNIT_WIDTH; ++i)
	{
		nRealCol = s_pMapMgr->GetRect().left - LEFT_OFFSET_COL + i;
		if(nRealCol < 0 ||
			nRealCol >= stInfo.nCol)
		{
			continue;
		}

		if(nRealRow % 2 == 0 &&
			nRealCol % 2 == 0)
		{
			//	背景
			if(pTexBkMgr)
			{
				sTile = pMapData->GetCellData(nRealRow, nRealCol)->wBkImg;
				sTile &= (~0x8000);
				sTile -= 1;

				if(sTile >= 0)
				{
					tex = pTexBkMgr->GetTexture(sTile);
					if(tex)
					{
						DrawTextureDirect(tex, i * UNIT_WIDTH, _nRow * UNIT_HEIGHT, UNIT_WIDTH * 2, UNIT_HEIGHT * 2);
					}
				}
			}
		}

		//	中景
		if(pTexMdMgr)
		{
			sTile = pMapData->GetCellData(nRealRow, nRealCol)->wMidImg;
			sTile &= (~0x8000);
			sTile -= 1;

			if(sTile >= 0)
			{
				tex = pTexMdMgr->GetTexture(sTile);
				if(tex)
				{
					DrawTextureDirect(tex, i * UNIT_WIDTH, _nRow * UNIT_HEIGHT, UNIT_WIDTH, UNIT_HEIGHT);
				}
			}
		}
	}
}