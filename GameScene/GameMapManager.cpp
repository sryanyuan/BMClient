#include "../GameScene/GameMapManager.h"
#include "../Common/SKnl3Helper.h"
#include <stdio.h>
#include "../GameScene/GameMonster.h"
#include "../GameScene/GameResourceManager.h"
#include "../GameScene/GameScene.h"
#include "../GameScene/GameNPC.h"
#include "../Common/TimeCounter.h"
#include "../BackMir/BackMir.h"
#include "../DuiWnd/AssistPaneWnd.h"
#include <assert.h>
#include <io.h>
#include "../../CommonModule/HideAttribHelper.h"
#include "../Common/OutlineTextureManager.h"

const int g_nWholeMapOftX = -8;
const int g_nWholeMapOftY = 0;

//	extern const char* g_szResFile[];
//#define ENHANCE_DRAW

GameMapManager::GameMapManager()
{
	ZeroMemory(m_res, sizeof(m_res));
	m_pSpr = NULL;
	//m_pFont = NULL;
	//m_pFont = new GfxFont("宋体", 12, FALSE, FALSE, FALSE);
	ZeroMemory(&m_rcOldClient, sizeof(RECT));
	m_pxSurface = NULL;
	m_pSpr = new hgeSprite(0, 0, 0, 0, 0);
	m_pRender = NEW_EMPTY_SPRITE;
	m_pTargetRender = NEW_EMPTY_SPRITE;

#ifdef ENHANCE_DRAW
	m_TileMapSurface.SetTopLeftOffset(UNIT_WIDTH, UNIT_HEIGHT);
	m_TileMapSurface.Initialize(VIEW_WIDTH + UNIT_WIDTH * 2, VIEW_HEIGHT + VIEW_OPERATE + UNIT_HEIGHT * 2);
#endif
	m_hTileTarget = AfxGetHge()->Target_Create(UNIT_WIDTH * (VIEW_WIDTH / UNIT_WIDTH + 3),
		UNIT_HEIGHT * (VIEW_HEIGHT / UNIT_HEIGHT + 5), false);
	m_bUseTileTarget = false;
}

GameMapManager::~GameMapManager()
{
	delete m_pSpr;
	SAFE_DELETE(m_pRender);
	SAFE_DELETE(m_pTargetRender);
	//delete m_pFont;
	m_TileMapSurface.Release();
	AfxGetHge()->Target_Free(m_hTileTarget);
	m_hTileTarget = NULL;
}

bool GameMapManager::LoadMap(const char* lpszMapName)
{
	char szPath[MAX_PATH];
	GetRootPath(szPath, MAX_PATH);
	sprintf(szPath, "%sMap\\%s.map", szPath, lpszMapName);

	if(!m_mapData.LoadMap(szPath))
	{
		HGE* hge = hgeCreate(HGE_VERSION);
		hge->System_Log("读取地图%s失败", lpszMapName);
		hge->Release();
		return false;
	}

	return true;
}

bool GameMapManager::LoadMapReadOnly(const char* lpszMapName)
{
	char szPath[MAX_PATH];
	GetRootPath(szPath, MAX_PATH);
	sprintf(szPath, "%sMap\\%s.map", szPath, lpszMapName);

	if(!m_mapData.LoadMapReadOnly(szPath))
	{
		HGE* hge = hgeCreate(HGE_VERSION);
		hge->System_Log("读取地图%s失败", lpszMapName);
		hge->Release();
		return false;
	}

#ifdef _DEBUG
	AfxGetHge()->System_Log("载入地图[%s]成功，附加索引[%d]", lpszMapName, m_mapData.GetResIndex());
#endif

	return true;
}

bool GameMapManager::AddFloorItem(int _centerx, int _centery, ItemAttrib* _pitem)
{
	MapInfo info;
	m_mapData.GetMapInfo(info);
	//	遍历游戏玩家周围坐标添加物品
	for(int i = 1; i < FLOOR_ITEM_OFFSET + 1; ++i)
	{
		RECT rc;
		rc.left = _centerx - i + 1;
		rc.right = _centerx + i - 1;
		rc.top = _centery - i + 1;
		rc.bottom = _centery + i - 1;
		POINT pt;
		//	遍历左边
		for(int y = _centery - i; y <= _centery + i; ++y)
		{
			for(int x = _centerx - i; x <= _centerx + i; ++x)
			{
				pt.x = x;
				pt.y = y;
				if(/*!::PtInRect(&rc, pt)*/!(x >= rc.left &&
					x <= rc.right &&
					y >= rc.top &&
					y <= rc.bottom))
				{
					if(CanThrough(x, y))
					{
						if(!GetFloorItem(x, y))
						{
							ItemAttrib* pitem = new ItemAttrib;
							memcpy(pitem, _pitem, sizeof(ItemAttrib));
							m_mapData.GetCellData(y, x)->wItem = (DWORD)pitem;
							return true;
						}
					}
				}
			}
		}
	}
// 	for(int x = _centerx - FLOOR_ITEM_OFFSET; x <= _centerx + FLOOR_ITEM_OFFSET; ++x)
// 	{
// 		for(int y = _centery - FLOOR_ITEM_OFFSET; y <= _centery + FLOOR_ITEM_OFFSET; ++y)
// 		{
// 			if(CanThrough(x, y))
// 			{
// 				if(!GetFloorItem(x, y))
// 				{
// 					ItemAttrib* pitem = new ItemAttrib;
// 					memcpy(pitem, _pitem, sizeof(ItemAttrib));
// 					m_mapData.GetCellData(y, x)->wItem = (DWORD)pitem;
// 					return true;
// 				}
// 			}
// 		}
// 	}
	return false;
}

ItemAttrib* GameMapManager::GetFloorItem(int _x, int _y)
{
	return (ItemAttrib*)m_mapData.GetCellData(_y, _x)->wItem;
}

void GameMapManager::UpdateClientRect()
{
	RECT rcPrev = m_rcClient;

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	m_rcClient.left = (int)(pPlayer->GetObjx() / UNIT_WIDTH) - (int)((VIEW_WIDTH / UNIT_WIDTH - 1) / 2);
	//m_rcClient.right = m_rcClient.left + VIEW_WIDTH / UNIT_WIDTH - 1;
	m_rcClient.right = m_rcClient.left + VIEW_WIDTH / UNIT_WIDTH - 1;
	m_rcClient.top = (int)(pPlayer->GetObjy() / UNIT_HEIGHT) - (int)((VIEW_HEIGHT / UNIT_HEIGHT - 1) / 2);
	//m_rcClient.bottom = m_rcClient.top + (VIEW_HEIGHT + VIEW_OPERATE) / UNIT_HEIGHT /*- 1*/-2;
	m_rcClient.bottom = m_rcClient.top + (VIEW_HEIGHT + VIEW_OPERATE) / UNIT_HEIGHT /*- 1*/-2;

	/*if(m_rcClient.left != rcPrev.left ||
		m_rcClient.top != rcPrev.top)
	{
		//	begin scene
		if(m_bUseTileTarget)
		{
			AfxGetHge()->Gfx_BeginScene(m_hTileTarget);
			AfxGetHge()->Gfx_Clear(0);

			RenderBkToTileSurface();

			AfxGetHge()->Gfx_EndScene();
		}
	}*/
}

void GameMapManager::RenderBkToTileSurface()
{
	//	渲染背景
	if(!m_mapData.IsDataInside())
	{
		return;
	}
	if(!m_bUseTileTarget)
	{
		return;
	}

	GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_TILES);
	GameTextureManager* ttexSmTiles = GameResourceManager::GetInstance()->GetTexs(RES_SMTILES);
	GameTextureManager* ttexDnItems = GameResourceManager::GetInstance()->GetTexs(RES_DNITEMS);
	GameTextureManager* ttexObjects = NULL;
	if(!ttex ||
		!ttexSmTiles)
	{
		return;
	}

	static int s_nAniCount = 0;
	static DWORD dwTimeCount = 0;

	if(GetTickCount() - dwTimeCount > 50)
	{
		dwTimeCount = GetTickCount();
		++s_nAniCount;
		if(s_nAniCount > 100000)
		{
			s_nAniCount = 0;
		}
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nCenRow = 0;
	int nCenCol = 0;
	int nCenRowOffset = 0;
	int nCenColOffset = 0;
	MapInfo info;
	m_mapData.GetMapInfo(info);

	//	确定左上角坐标
	nCenRow -= (VIEW_HEIGHT / UNIT_HEIGHT - 1) / 2;
	nCenCol -= (VIEW_WIDTH / UNIT_WIDTH - 1) / 2;
	int wTile = 0;
	char bAni = 0;
	unsigned char bAniTick = 0;
	HTEXTURE tex = 0;
	int nTexWidth = 0;
	int nTexHeight = 0;

	int nDrawX = 0;
	int nDrawY = 0;

	char szWilPath[MAX_PATH];
	bool bEspRead = false;

 	for(int i = /*nCenRow*/m_rcClient.top - 2; i < /*nCenRow + VIEW_HEIGHT / UNIT_HEIGHT*/m_rcClient.bottom + 4; ++i)
 	{
 		for(int j = /*nCenCol*/m_rcClient.left/* - 2*/-1; j < /*nCenCol + VIEW_WIDTH / UNIT_WIDTH*/m_rcClient.right + 2 + 1; ++j)
 		{
 			if(i < 0 ||
 				j < 0 ||
 				i >= info.nRow ||
 				j >= info.nCol)
 			{
 				continue;
 			}

			bEspRead = false;
			ttexObjects = NULL;

 			if(i % 2 == 0 &&
 				j % 2 == 0)
 			{
				//	地表
 				wTile = m_mapData.GetCellData(i, j)->wBkImg;
				wTile &= (~0x8000);
				wTile -= 1;
 				if(wTile >= 0)
 				{
 					tex = ttex->GetTexture(wTile & (~0x8000));
 					if(tex)
 					{
						//nTexWidth = ttex->GetTextureWidth(tex);
						nTexWidth = ttex->GetTextureWidth(wTile & (~0x8000));
						//nTexHeight = ttex->GetTextureHeight(tex);
						nTexHeight = ttex->GetTextureHeight(wTile & (~0x8000));
 						if(!m_pSpr)
 						{
 							m_pSpr = new hgeSprite(tex, 0, 0, nTexWidth, nTexHeight);
 						}
 						else
 						{
 							m_pSpr->SetTexture(tex);
 							m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
 						}

 						m_pSpr->Render((j - m_rcClient.left) * UNIT_WIDTH + UNIT_WIDTH * 2,
							(i- m_rcClient.top) * UNIT_HEIGHT + UNIT_HEIGHT);
 					}
 				}
 			}

			{
				//	SmTiles
				if(i >= m_rcClient.top - 1 &&
					i <= m_rcClient.bottom + 2 &&
					j >= m_rcClient.left - 1 &&
					j <= m_rcClient.right + 2)
				{
					wTile = m_mapData.GetCellData(i, j)->wMidImg;
					wTile = wTile & (~0x8000);
					if(wTile > 0)
					{
						tex = ttexSmTiles->GetTexture(wTile - 1);
						if(tex)
						{
							//nTexWidth = ttexSmTiles->GetTextureWidth(tex);
							nTexWidth = ttexSmTiles->GetTextureWidth(wTile - 1);
							//nTexHeight = ttexSmTiles->GetTextureHeight(tex);
							nTexHeight = ttexSmTiles->GetTextureHeight(wTile - 1);
							if(!m_pSpr)
							{
								m_pSpr = new hgeSprite(tex, 0, 0, nTexWidth, nTexHeight);
							}
							else
							{
								m_pSpr->SetTexture(tex);
								m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
							}
							m_pSpr->Render((j - m_rcClient.left) * UNIT_WIDTH + 2 * UNIT_WIDTH,
								(i- m_rcClient.top) * UNIT_HEIGHT + UNIT_HEIGHT);
						}
					}
				}
			}

			{	//	新增动画
				//	画小的 48*32的FrImg
				if(i >= m_rcClient.top - 1 &&
					i <= m_rcClient.bottom + 2 &&
					j >= m_rcClient.left - 1 &&
					j <= m_rcClient.right + 2)
				{
					int nObjectsIndex = 0;
					nObjectsIndex = m_mapData.GetCellData(i, j)->bArea;
					if(nObjectsIndex >= 0 &&
						nObjectsIndex <= MAX_OBJECTS - RES_OBJECTS)
					{
						ttexObjects = GameResourceManager::GetInstance()->GetTexs(RES_OBJECTS + nObjectsIndex);
					}
					if(nObjectsIndex > MAX_OBJECTS - RES_OBJECTS &&
						nObjectsIndex < 20)
					{
						continue;
					}
					else if(nObjectsIndex >= 20)
					{
						GameResourceManager::GetInstance()->LoadFileSibMapPackage(nObjectsIndex);
						ttexObjects = GameResourceManager::GetInstance()->GetSibMapPackage(nObjectsIndex);
					}

					wTile = m_mapData.GetCellData(i, j)->wFrImg;
					//	最高位是否能行走 忽略
					wTile &= (~0x8000);
					if(wTile > 0)
					{
						/*bAni = m_mapData.GetCellData(i, j)->bAniFrame;
						if((bAni & 0x80) > 0)
						{
							bAni &= (~0x80);
						}
						if(bAni > 0)
						{
							bAniTick = m_mapData.GetCellData(i, j)->bAniTick;
							wTile = wTile + (s_nAniCount % (bAni + (bAni * bAniTick))) / (1 + bAniTick);
						}*/
						--wTile;
						/*if(bEspRead)
						{
						ttexObjects = GameResourceManager::GetInstance()->GetTexsEx(nObjectsIndex + 1);
						}
						else
						{
						ttexObjects = GameResourceManager::GetInstance()->GetTexs(RES_OBJECTS + nObjectsIndex);
						}*/

						if(ttexObjects)
						{
							if(ttexObjects->GetTextureWidth(wTile) == UNIT_WIDTH &&
								ttexObjects->GetTextureHeight(wTile) == UNIT_HEIGHT)
							{
								//	小的
								tex = ttexObjects->GetTexture(wTile);
								if(!m_pSpr)
								{
									m_pSpr = new hgeSprite(tex, 0, 0, UNIT_WIDTH, UNIT_HEIGHT);
								}
								else
								{
									m_pSpr->SetTexture(tex);
									m_pSpr->SetTextureRect(0, 0, UNIT_WIDTH, UNIT_HEIGHT);
								}
								//	人物向左走 地图向右移 故-Shift
								//if(m_rcClient.left < 0)
								{
									//nDrawX = (j - m_rcClient.left - 1) * UNIT_WIDTH - pPlayer->GetShiftX();
								}
								//else
								{
									nDrawX = (j - m_rcClient.left) * UNIT_WIDTH + 2 * UNIT_WIDTH;
								}
								//if(m_rcClient.top < 0)
								{
									//nDrawY = (i - m_rcClient.top) * UNIT_HEIGHT + UNIT_HEIGHT - pPlayer->GetShiftY();
								}
								//else
								{
									nDrawY = (i - m_rcClient.top) * UNIT_HEIGHT + UNIT_HEIGHT;
								}
								//nDrawX = (j - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX();
								//nDrawY = (i- m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY()
								m_pSpr->Render(nDrawX, nDrawY);
							}
							else
							{
								//AfxGetHge()->System_Log("not 48*32");
							}
						}
					}
				}
			}
 		}
 	}
}

void GameMapManager::RenderBkWithTileSurface()
{
	if(!m_bUseTileTarget)
	{
		return;
	}
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	HTEXTURE tex = AfxGetHge()->Target_GetTexture(m_hTileTarget);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, UNIT_WIDTH * (VIEW_WIDTH / UNIT_WIDTH + 3), UNIT_HEIGHT * (VIEW_HEIGHT / UNIT_HEIGHT + 5));
		m_pRender->Render(-2 * UNIT_WIDTH - pPlayer->GetShiftX() + g_nWholeMapOftX,
			-1 * UNIT_HEIGHT - pPlayer->GetShiftY() + g_nWholeMapOftY);
	}
}


void GameMapManager::RenderBk()
{
	//	渲染背景
	if(!m_mapData.IsDataInside())
	{
		return;
	}

	//	surface
	/*if(m_bUseTileTarget)
	{
		RenderBkWithTileSurface();
		return;
	}*/

	GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_TILES);
	GameTextureManager* ttexSmTiles = GameResourceManager::GetInstance()->GetTexs(RES_SMTILES);
	GameTextureManager* ttexDnItems = GameResourceManager::GetInstance()->GetTexs(RES_DNITEMS);
	GameTextureManager* ttexObjects = NULL;
	if(!ttex ||
		!ttexSmTiles)
	{
		return;
	}

	GameTextureManager* pFixRes = NULL;
	if(m_mapData.GetResIndex() != 0)
	{
		int nResIndex = m_mapData.GetResIndex() + 20;
		GameResourceManager::GetInstance()->LoadFileSibMapPackage(nResIndex);
		pFixRes = GameResourceManager::GetInstance()->GetSibMapPackage(nResIndex);
	}

	if(pFixRes)
	{
		ttex = pFixRes;
		ttexSmTiles = pFixRes;
	}

	static int s_nAniCount = 0;
	static DWORD dwTimeCount = 0;

	if(GetTickCount() - dwTimeCount > 50)
	{
		dwTimeCount = GetTickCount();
		++s_nAniCount;
		if(s_nAniCount > 100000)
		{
			s_nAniCount = 0;
		}
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nCenRow = 0;
	int nCenCol = 0;
	int nCenRowOffset = 0;
	int nCenColOffset = 0;
	MapInfo info;
	m_mapData.GetMapInfo(info);

	//	确定左上角坐标
	nCenRow -= (VIEW_HEIGHT / UNIT_HEIGHT - 1) / 2;
	nCenCol -= (VIEW_WIDTH / UNIT_WIDTH - 1) / 2;
	int wTile = 0;
	char bAni = 0;
	unsigned char bAniTick = 0;
	HTEXTURE tex = 0;
	int nTexWidth = 0;
	int nTexHeight = 0;

	int nDrawX = 0;
	int nDrawY = 0;

	char szWilPath[MAX_PATH];
	bool bEspRead = false;

 	for(int i = /*nCenRow*/m_rcClient.top - 2; i < /*nCenRow + VIEW_HEIGHT / UNIT_HEIGHT*/m_rcClient.bottom + 4; ++i)
 	{
 		for(int j = /*nCenCol*/m_rcClient.left/* - 2*/-1; j < /*nCenCol + VIEW_WIDTH / UNIT_WIDTH*/m_rcClient.right + 2 + 1; ++j)
 		{
 			if(i < 0 ||
 				j < 0 ||
 				i >= info.nRow ||
 				j >= info.nCol)
 			{
 				continue;
 			}

			bEspRead = false;
			ttexObjects = NULL;

 			if(i % 2 == 0 &&
 				j % 2 == 0)
 			{
				//	地表
 				wTile = m_mapData.GetCellData(i, j)->wBkImg;
				wTile &= (~0x8000);
				wTile -= 1;
 				if(wTile >= 0)
 				{
 					tex = ttex->GetTexture(wTile & (~0x8000));
 					if(tex)
 					{
						//nTexWidth = ttex->GetTextureWidth(tex);
						nTexWidth = ttex->GetTextureWidth(wTile & (~0x8000));
						//nTexHeight = ttex->GetTextureHeight(tex);
						nTexHeight = ttex->GetTextureHeight(wTile & (~0x8000));
 						if(!m_pSpr)
 						{
 							m_pSpr = new hgeSprite(tex, 0, 0, nTexWidth, nTexHeight);
 						}
 						else
 						{
 							m_pSpr->SetTexture(tex);
 							m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
 						}

 						m_pSpr->Render((j - /*(nCenCol)*/m_rcClient.left) * UNIT_WIDTH - /*nCenColOffset*/pPlayer->GetShiftX() + g_nWholeMapOftX,
							(i- /*(nCenRow)*/m_rcClient.top) * UNIT_HEIGHT - /*nCenRowOffset*/pPlayer->GetShiftY() + g_nWholeMapOftY);
 					}
 				}
 			}

			{
				//	SmTiles
				if(i >= m_rcClient.top - 1 &&
					i <= m_rcClient.bottom + 2 &&
					j >= m_rcClient.left - 1 &&
					j <= m_rcClient.right + 2)
				{
					wTile = m_mapData.GetCellData(i, j)->wMidImg;
					wTile = wTile & (~0x8000);
					if(wTile > 0)
					{
						tex = ttexSmTiles->GetTexture(wTile - 1);
						if(tex)
						{
							//nTexWidth = ttexSmTiles->GetTextureWidth(tex);
							nTexWidth = ttexSmTiles->GetTextureWidth(wTile - 1);
							//nTexHeight = ttexSmTiles->GetTextureHeight(tex);
							nTexHeight = ttexSmTiles->GetTextureHeight(wTile - 1);
							if(!m_pSpr)
							{
								m_pSpr = new hgeSprite(tex, 0, 0, nTexWidth, nTexHeight);
							}
							else
							{
								m_pSpr->SetTexture(tex);
								m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
							}
							m_pSpr->Render((j - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX() + g_nWholeMapOftX,
								(i- m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY() + g_nWholeMapOftY);
						}
					}
				}
			}

			{	//	新增动画
				//	画小的 48*32的FrImg
				if(i >= m_rcClient.top - 1 &&
					i <= m_rcClient.bottom + 2 &&
					j >= m_rcClient.left - 1 &&
					j <= m_rcClient.right + 2)
				{
					int nObjectsIndex = 0;
					nObjectsIndex = m_mapData.GetCellData(i, j)->bArea;
					if(nObjectsIndex >= 0 &&
						nObjectsIndex <= MAX_OBJECTS - RES_OBJECTS)
					{
						ttexObjects = GameResourceManager::GetInstance()->GetTexs(RES_OBJECTS + nObjectsIndex);
					}
					if(nObjectsIndex > MAX_OBJECTS - RES_OBJECTS &&
						nObjectsIndex < 20)
					{
						continue;
					}
					else if(nObjectsIndex >= 20)
					{
						GameResourceManager::GetInstance()->LoadFileSibMapPackage(nObjectsIndex);
						ttexObjects = GameResourceManager::GetInstance()->GetSibMapPackage(nObjectsIndex);
					}

					if(pFixRes)
					{
						ttexObjects = pFixRes;
					}

					wTile = m_mapData.GetCellData(i, j)->wFrImg;
					//	最高位是否能行走 忽略
					wTile &= (~0x8000);
					if(wTile > 0)
					{
						bAni = m_mapData.GetCellData(i, j)->bAniFrame;
						if((bAni & 0x80) > 0)
						{
							bAni &= (~0x80);
						}
						if(bAni > 0)
						{
							bAniTick = m_mapData.GetCellData(i, j)->bAniTick;
							wTile = wTile + (s_nAniCount % (bAni + (bAni * bAniTick))) / (1 + bAniTick);
						}
						--wTile;
						/*if(bEspRead)
						{
						ttexObjects = GameResourceManager::GetInstance()->GetTexsEx(nObjectsIndex + 1);
						}
						else
						{
						ttexObjects = GameResourceManager::GetInstance()->GetTexs(RES_OBJECTS + nObjectsIndex);
						}*/

						if(ttexObjects)
						{
							if(ttexObjects->GetTextureWidth(wTile) == UNIT_WIDTH &&
								ttexObjects->GetTextureHeight(wTile) == UNIT_HEIGHT)
							{
								//	小的
								tex = ttexObjects->GetTexture(wTile);
								if(!m_pSpr)
								{
									m_pSpr = new hgeSprite(tex, 0, 0, UNIT_WIDTH, UNIT_HEIGHT);
								}
								else
								{
									m_pSpr->SetTexture(tex);
									m_pSpr->SetTextureRect(0, 0, UNIT_WIDTH, UNIT_HEIGHT);
								}
								//	人物向左走 地图向右移 故-Shift
								//if(m_rcClient.left < 0)
								{
									//nDrawX = (j - m_rcClient.left - 1) * UNIT_WIDTH - pPlayer->GetShiftX();
								}
								//else
								{
									nDrawX = (j - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX();
								}
								//if(m_rcClient.top < 0)
								{
									//nDrawY = (i - m_rcClient.top) * UNIT_HEIGHT + UNIT_HEIGHT - pPlayer->GetShiftY();
								}
								//else
								{
									nDrawY = (i - m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY();
								}
								//nDrawX = (j - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX();
								//nDrawY = (i- m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY()
								m_pSpr->Render(nDrawX + g_nWholeMapOftX, nDrawY + g_nWholeMapOftY);
							}
							else
							{
								//AfxGetHge()->System_Log("not 48*32");
							}
						}
					}
				}
			}

			{
				//	地面表层画完
			}

			{
				//	画掉落物品
				//	画掉落物品
// 				ItemAttrib* pitem = (ItemAttrib*)m_mapData.GetCellData(i, j)->wItem;
// 				if(pitem)
// 				{
// 					tex = ttexDnItems->GetTexture(pitem->tex);
// 					if(tex)
// 					{
// 						nTexWidth = ttexDnItems->GetTextureWidthEx(pitem->tex);
// 						nTexHeight = ttexDnItems->GetTextureHeightEx(pitem->tex);
// 						m_pSpr->SetTexture(tex);
// 						m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
// 						m_pSpr->Render((j - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX() + + (UNIT_WIDTH - nTexWidth) / 2,
// 							(i- m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY() + (UNIT_HEIGHT - nTexHeight) / 2);
// 					}
// 				}
			}
 		}
 	}
}



void GameMapManager::RenderFrImg()
{
	//	渲染小背景
	if(!m_mapData.IsDataInside())
	{
		return;
	}

	//	得到地图特定资源
	GameTextureManager* pFixRes = NULL;
	if(m_mapData.GetResIndex() != 0)
	{
		int nResIndex = m_mapData.GetResIndex() + 20;
		GameResourceManager::GetInstance()->LoadFileSibMapPackage(nResIndex);
		pFixRes = GameResourceManager::GetInstance()->GetSibMapPackage(nResIndex);
	}

	GameTextureManager* ttex = NULL;
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nPreBlendMode = m_pSpr->GetBlendMode();

	int nCenRow = 0;
	int nCenCol = 0;
	int nCenRowOffset = 0;
	int nCenColOffset = 0;
	MapInfo info;
	m_mapData.GetMapInfo(info);
	bool bPlayerRendered = false;

//	char szWilPath[MAX_PATH];
	bool bEspRead = false;
	int nEspIndex = 0;
	int wTile = 0;
	HTEXTURE tex = 0;
	int nTexWidth = 0;
	int nTexHeight = 0;

	int nObjectsIndex = 0;
	int nDrawX = 0;
	int nDrawY = 0;

	TimerCounter timer;
	timer.Start();
	//PLAYERLIST tMons;
	//GROUNDITEMS tItems;

	unsigned char bAniTick = 0;
	char bAni = 0;
	bool bAlpha = 0;

	static int s_nAniCount = 0;
	static DWORD dwTimeCount = 0;

	if(GetTickCount() - dwTimeCount > 50)
	{
		dwTimeCount = GetTickCount();
		++s_nAniCount;
		if(s_nAniCount > 100000)
		{
			s_nAniCount = 0;
		}
	}
	//	画死掉的怪物
	for(int i = m_rcClient.top; i < m_rcClient.bottom; ++i)
	{
		for(int j = m_rcClient.right; j > m_rcClient.left; --j)
		{
			if(i >= m_rcClient.top &&
				i <= m_rcClient.bottom &&
				j >= m_rcClient.left &&
				j <= m_rcClient.right)
			{
				GameObject* pObj = NULL;
				const MemMapObject* pMemMapObjs = NULL;

				for(int k = 0; k < MAX_CELL_OBJECTS; ++k)
				{
					pMemMapObjs = GameScene::sThis->GetMemMapObject(j - m_rcClient.left, i - m_rcClient.top);
					if(pMemMapObjs)
					{
						pObj = GameScene::sThis->GetMemMapObject(j - m_rcClient.left, i - m_rcClient.top)->pObjects[k];
						if(pObj)
						{
							if(pObj->GetStatus() == PST_DEAD)
							{
								m_xAlphaDrawObjects.push_back(pObj);
								pObj->Render();
							}
						}
					}
				}
			}
		}
	}

	//	行的上面的偏移可以少点
	for(int i = m_rcClient.top - 2; i < m_rcClient.bottom + 18; ++i)
	{
		//for(int j = m_rcClient.right + 2; j >= m_rcClient.left - 4; --j)
		for(int j = m_rcClient.left - 4; j <= m_rcClient.right + 2; ++j)
		{
			bEspRead = false;
			bAlpha = false;
			ttex = NULL;
			//	行列非法 不画
			if(i < 0 ||
				j < 0 ||
				i >= info.nRow ||
				j >= info.nCol)
			{
				continue;
			}

			wTile = m_mapData.GetCellData(i, j)->wFrImg;
			//	最高位是否能行走 忽略
			wTile &= (~0x8000);
			if(wTile > 0)
			{
				//	计算索引
				nObjectsIndex = m_mapData.GetCellData(i, j)->bArea;
				if(nObjectsIndex >= 0 &&
					nObjectsIndex <= MAX_OBJECTS - RES_OBJECTS)
				{
					ttex = GameResourceManager::GetInstance()->GetTexs(RES_OBJECTS + nObjectsIndex);
				}
				else if(nObjectsIndex > MAX_OBJECTS - RES_OBJECTS &&
					nObjectsIndex < 20)
				{
					continue;
				}
				else if(nObjectsIndex >= 20)
				{
					GameResourceManager::GetInstance()->LoadFileSibMapPackage(nObjectsIndex);
					ttex = GameResourceManager::GetInstance()->GetSibMapPackage(nObjectsIndex);
				}

				if(pFixRes != NULL)
				{
					ttex = pFixRes;
				}

				/*if(!bEspRead)
				{
					ttex = GameResourceManager::GetInstance()->GetTexs(RES_OBJECTS + nObjectsIndex);
				}
				else
				{
					ttex = GameResourceManager::GetInstance()->GetTexsEx(nObjectsIndex + 1);
				}*/

				if(ttex)
				{
//////////////////////////////////////////////////////////////////////////
					//	Note For Animation
					bAni = m_mapData.GetCellData(i, j)->bAniFrame;
					if((bAni & 0x80) > 0)
					{
						bAni &= (~0x80);
						bAlpha = true;
					}
					if(bAni > 0)
					{
						bAniTick = m_mapData.GetCellData(i, j)->bAniTick;
						//if(bAniTick != 255)
						{
							wTile = wTile + (s_nAniCount % (bAni + (bAni * bAniTick))) / (1 + bAniTick);
						}
					}
					//	End for Animation
//////////////////////////////////////////////////////////////////////////
					--wTile;

					nTexWidth = ttex->GetTextureWidth(wTile/* - 1*/);
					nTexHeight = ttex->GetTextureHeight(wTile/* - 1*/);

					// Modify [4/23/2013 yuanxj]
					//if(nTexHeight != UNIT_HEIGHT &&
						//nTexWidth == UNIT_WIDTH)
					// Here [4/23/2013 yuanxj]
					if(nTexHeight != UNIT_HEIGHT ||
						nTexWidth != UNIT_WIDTH)
					// End [4/23/2013 yuanxj]
					{
						tex = ttex->GetTexture(wTile/* - 1*/);
						if(tex)
						{
//////////////////////////////////////////////////////////////////////////
							//	Note for animation
							if(bAlpha)
							{
								//m_pSpr->SetBlendMode(BLEND_ALPHAADD | BLEND_NOZWRITE);
							}
							if(bAlpha)
							{
								short sOftX = ttex->GetTextureOffsetX(wTile);
								short sOftY = ttex->GetTextureOffsetY(wTile);
								nDrawX = (j - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX() + sOftX + 2;
								nDrawY = (i- m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY() + sOftY - 68;

								if(nDrawY < VIEW_HEIGHT + VIEW_OPERATE)
								{
									//	画Objects
									//if(!m_pSpr)
									{
										//m_pSpr = new hgeSprite(tex, 0, 0, nTexWidth, nTexHeight);
									}
									//else
									{
										MagicElement::pEffectRender->SetTexture(tex);
										MagicElement::pEffectRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
									}
									MagicElement::pEffectRender->Render(nDrawX + g_nWholeMapOftX,nDrawY + g_nWholeMapOftY);
								}
								else
								{
									//..
								}
							}
							else
							{
								nDrawX = (j - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX();
								nDrawY = (i- m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY() - nTexHeight + 32;

								if(nDrawY < VIEW_HEIGHT + VIEW_OPERATE)
								{
									//	画Objects
									if(!m_pSpr)
									{
										m_pSpr = new hgeSprite(tex, 0, 0, nTexWidth, nTexHeight);
									}
									else
									{
										m_pSpr->SetTexture(tex);
										m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
									}
									m_pSpr->Render(nDrawX + g_nWholeMapOftX,nDrawY + g_nWholeMapOftY);
								}
								else
								{
									//..
								}
							}
							//	End for animation
//////////////////////////////////////////////////////////////////////////

							{
								/*nDrawX = (j - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX();
								nDrawY = (i- m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY() - nTexHeight + 32;

								if(nDrawY < VIEW_HEIGHT + VIEW_OPERATE)
								{
									//	画Objects
									if(!m_pSpr)
									{
										m_pSpr = new hgeSprite(tex, 0, 0, nTexWidth, nTexHeight);
									}
									else
									{
										m_pSpr->SetTexture(tex);
										m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
									}
									m_pSpr->Render(nDrawX + g_nWholeMapOftX,nDrawY + g_nWholeMapOftY);
								}
								else
								{
									//..
								}*/
							}
//////////////////////////////////////////////////////////////////////////
							//	Note for animation
							if(bAlpha)
							{
								//m_pSpr->SetBlendMode(nPreBlendMode);
							}
							//	End for animation
//////////////////////////////////////////////////////////////////////////
						}
					}
				}
			}
			


 			if(i >= m_rcClient.top &&
 				i <= m_rcClient.bottom &&
 				j >= m_rcClient.left &&
 				j <= m_rcClient.right)
 			{
 				GameObject* pObj = NULL;
 				GroundItem* pItem = NULL;
				MagicElement* pMgc = NULL;
				const MemMapObject* pMemMapObjs = NULL;
 				GameTextureManager* pDnItemsTexs = GameResourceManager::GetInstance()->GetTexs(RES_DNITEMS);
 				int nDrawOftX = 0;
 				int nDrawOftY = 0;
 
 				for(int k = 0; k < MAX_CELL_OBJECTS; ++k)
 				{
					pMemMapObjs = GameScene::sThis->GetMemMapObject(j - m_rcClient.left, i - m_rcClient.top);
					if(pMemMapObjs)
					{
						pMgc = GameScene::sThis->GetMemMapObject(j - m_rcClient.left, i - m_rcClient.top)->pMagics[k];
						if(pMgc)
						{
							pMgc->Render();
						}

						pObj = GameScene::sThis->GetMemMapObject(j - m_rcClient.left, i - m_rcClient.top)->pObjects[k];
						if(pObj)
						{
							//tMons.push_back(pObj);
							if(pObj->GetStatus() != PST_DEAD)
							{
								m_xAlphaDrawObjects.push_back(pObj);
								if(!pObj->UpdateHide())
								{
									pObj->SetRenderMode(ORM_NORMAL);
									pObj->Render();
									pObj->SetRenderMode(ORM_NONE);
								}
								/*else
								{
									int c = 0;
									c = c + 1;
								}*/
							}
							// 						if(pObj->GetType() == OBJ_PLAYER ||
							// 							pObj->GetType() == OBJ_OTHERPLAYER)
							// 						{
							// 							static_cast<GameOtherPlayer*>(pObj)->RenderSelfEffect();
							// 						}
							//break;
						}

						pItem = GameScene::sThis->GetMemMapObject(j - m_rcClient.left, i - m_rcClient.top)->pItems[k];
						if(pItem &&
							pDnItemsTexs)
						{
							tex = pDnItemsTexs->GetTexture(pItem->stAttrib.tex);
							//tItems.push_back(pItem);
							m_xShowItems.push_back(pItem);
							if(tex)
							{
								nDrawX = (pItem->wPosX - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX();
								nDrawY = (pItem->wPosY- m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY();
								nTexWidth = pDnItemsTexs->GetTextureWidth(pItem->stAttrib.tex);
								nTexHeight = pDnItemsTexs->GetTextureHeight(pItem->stAttrib.tex);
								nDrawOftX = (UNIT_WIDTH - nTexWidth) / 2;
								nDrawOftY = (UNIT_HEIGHT - nTexHeight) / 2;
								m_pSpr->SetTexture(tex);
								m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
								m_pSpr->Render(nDrawX + nDrawOftX + g_nWholeMapOftX,
									nDrawY + nDrawOftY + g_nWholeMapOftY);
							}

							if(j == GameScene::sThis->GetMouseCoordX() &&
								i == GameScene::sThis->GetMouseCoordY())
							{
								//m_xMouseItems.push_back(pItem);
							}
						}
						/*pMgc = GameScene::sThis->GetMemMapObject(j - m_rcClient.left, i - m_rcClient.top)->pMagics[k];
						if(pMgc)
						{
							pMgc->Render();
						}*/
					}
 				}
 			}
 
 
 			//	同时画人 提前一格避免被地形遮挡
  			if(!bPlayerRendered)
  			{
  				if(i == ((int)GamePlayer::GetInstance()->GetObjy() / UNIT_HEIGHT) &&
  					j == ((int)GamePlayer::GetInstance()->GetObjx() / UNIT_WIDTH))
  				{
					if(!GamePlayer::GetInstance()->UpdateHide())
					{
						GamePlayer::GetInstance()->SetRenderMode(ORM_NORMAL);
						GamePlayer::GetInstance()->Render();
						GamePlayer::GetInstance()->SetRenderMode(ORM_NONE);
					}
					//GamePlayer::GetInstance()->RenderSelfEffect();
  					//tMons.push_back(GamePlayer::GetInstance());
					m_xAlphaDrawObjects.push_back(GamePlayer::GetInstance());
  					bPlayerRendered = true;
  				}
  			}
		}
	}

	//	怪物血条
	/*for(PLAYERLIST::const_iterator iter = m_xAlphaDrawObjects.begin();
		iter != m_xAlphaDrawObjects.end();
		++iter)
	{
		(*iter)->SetRenderMode(ORM_TRANSPARENT);
		(*iter)->TransparentRender();
		(*iter)->SetRenderMode(ORM_NONE);

		if((*iter)->GetType() == OBJ_PLAYER ||
			(*iter)->GetType() == OBJ_OTHERPLAYER)
		{
			GameOtherPlayer* pPlayer = static_cast<GameOtherPlayer*>(*iter);
			pPlayer->RenderHumState();
		}

		if((*iter)->GetType() != OBJ_NPC &&
			(*iter)->GetStatus() != PST_DEAD &&
			(*iter)->IsVisible())
		{
			//	非NPC 和 非死亡状态 才画血条
			(*iter)->RenderHPBar();
		}
		(*iter)->RenderSaying();

		if((*iter)->GetType() == OBJ_NPC)
		{
			(*iter)->RenderName();
		}
	}*/

	//	地面物品名字
	GroundItem* pItem = NULL;
	int nDrawOftX = 0;
	int nDrawOftY = 0;
	static DWORD s_dwLastItemFlashTime = 0;

	//	166 192 251
	//DWORD dwPreCol = AfxGetPrinter()->GetColor();
	DWORD dwNormalColor = ARGB(255, 166, 192, 251);
	//AfxGetPrinter()->SetColor(ARGB(255, 166, 192, 251));
	for(GROUNDITEMS::const_iterator iter = m_xShowItems.begin();
		iter != m_xShowItems.end();
		++iter)
	{
		pItem = *iter;
		nDrawX = (pItem->wPosX - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX();
		nDrawY = (pItem->wPosY - m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY();
		//	闪
		if(GetTickCount() - s_dwLastItemFlashTime > 15 * 100)
		{
			s_dwLastItemFlashTime = GetTickCount();
		}
		else
		{
			if(pTheGame->Config_ItemFlash())
			{
				int nCurFrame = (GetTickCount() - s_dwLastItemFlashTime) / 100;
				GameTextureManager* pOpUI = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);

				if(nCurFrame >= 0 &&
					nCurFrame <= 9)
				{
					tex = pOpUI->GetTexture(TEX_ITEMFLASH + nCurFrame);
					if(tex)
					{
						MagicElement::pEffectRender->SetTexture(tex);
						nTexWidth = pOpUI->GetTextureWidth(TEX_ITEMFLASH + nCurFrame);
						nTexHeight = pOpUI->GetTextureHeight(TEX_ITEMFLASH + nCurFrame);
						MagicElement::pEffectRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
						MagicElement::pEffectRender->Render(nDrawX + pOpUI->GetTextureOffsetX(TEX_ITEMFLASH + nCurFrame) + g_nWholeMapOftX,
							nDrawY + pOpUI->GetTextureOffsetY(TEX_ITEMFLASH + nCurFrame) + g_nWholeMapOftY);
					}
				}
			}
		}

		//	画名字
		nDrawOftY = -(UNIT_HEIGHT / 2 - 10);
		nDrawOftX = (UNIT_WIDTH / 2) - strlen(pItem->stAttrib.name) / 2 * 12 / 2;
		if(MirGame::IsEquipItem(pItem->stAttrib))
		{
			if(HideAttribHelper::GetAllAttribCount(pItem->stAttrib.maxMP) != 0)
			{
				AfxGetPrinter()->SetColor(ARGB_RED);
			}
			else if(LOBYTE(pItem->stAttrib.level) > 0)
			{
				AfxGetPrinter()->SetColor(ARGB_YELLOW);
			}
			else
			{
				AfxGetPrinter()->SetColor(dwNormalColor);
			}
		}
		else
		{
			AfxGetPrinter()->SetColor(dwNormalColor);
		}


		/*char szItemShowName[25] = {0};
		if(MirGame::IsEquipItem(pItem->stAttrib) &&
			HideAttribHelper::GetAllAttribCount(pItem->stAttrib.maxMP) != 0)
		{
			sprintf(szItemShowName, "%s(*)", pItem->stAttrib.name);
		}
		else
		{
			strcpy(szItemShowName, pItem->stAttrib.name);
		}*/

		if(pItem->bVisible)
		{
			AfxGetPrinter()->Print(nDrawX + nDrawOftX + g_nWholeMapOftX,
				nDrawY + nDrawOftY + g_nWholeMapOftY, pItem->stAttrib.name);
		}
	}
	//AfxGetPrinter()->SetColor(dwPreCol);

	//	画魔法
	GameScene::sThis->RenderMagic();

	//	勾出目标
	GameObject* pGameTarget = GameScene::sThis->GetTarget();
	if(NULL != pGameTarget)
	{
		//	outline target
		int nTexMgr = pGameTarget->GetTextureMgrIndex();
		GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(nTexMgr);
		int nTextureIndex = pGameTarget->GetCurrentTextureIndex();

		if(NULL != pTexMgr &&
			0 != nTextureIndex)
		{
			HTEXTURE texNormal = pTexMgr->GetTexture(nTextureIndex);
			if(texNormal)
			{
				HTEXTURE texOutline = 0;

				if(pGameTarget->GetType() == OBJ_MONS ||
					pGameTarget->GetType() == OBJ_NPC)
				{
					texOutline = OutlineTextureManager::GetInstance()->GetOutilneTextureMonster(texNormal, pGameTarget->GetAttrib()->id, nTextureIndex);
				}
				else if(pGameTarget->GetType() == OBJ_OTHERPLAYER)
				{
					/*GameOtherPlayer* pOtherPlayer = (GameOtherPlayer*)pGameTarget;
					int nResIndex = 0;
					pOtherPlayer->CalTextureIndex(NULL, &nResIndex);
					texOutline = OutlineTextureManager::GetInstance()->GetOutlineTextureHum(texNormal, nResIndex, nTextureIndex);*/
				}

				if(texOutline)
				{
					m_pTargetRender->SetTexture(texOutline);
					m_pTargetRender->SetTextureRect(0, 0, pTexMgr->GetTextureWidth(nTextureIndex), pTexMgr->GetTextureHeight(nTextureIndex));
					float fx = pGameTarget->GetScreenPosX() + pTexMgr->GetTextureOffsetX(nTextureIndex);
					float fy = pGameTarget->GetScreenPosY() + pTexMgr->GetTextureOffsetY(nTextureIndex);
					m_pTargetRender->Render(fx, fy);
				}
			}
		}

	}

	//	怪物血条
	for(PLAYERLIST::const_iterator iter = m_xAlphaDrawObjects.begin();
		iter != m_xAlphaDrawObjects.end();
		++iter)
	{
		GameObject* pObject = *iter;
		(*iter)->SetRenderMode(ORM_TRANSPARENT);
		if(pObject->GetStatus() != PST_DEAD)
		{
			//	死亡状态下不进行透明绘制，避免爆出物品被遮挡
			(*iter)->TransparentRender();
		}
		(*iter)->SetRenderMode(ORM_NONE);

		if((*iter)->GetType() == OBJ_PLAYER ||
			(*iter)->GetType() == OBJ_OTHERPLAYER)
		{
			GameOtherPlayer* pPlayer = static_cast<GameOtherPlayer*>(*iter);
			pPlayer->RenderHumState();
		}

		if((*iter)->GetType() != OBJ_NPC &&
			(*iter)->GetStatus() != PST_DEAD &&
			(*iter)->IsVisible())
		{
			//	非NPC 和 非死亡状态 才画血条
			(*iter)->RenderHPBar();
		}
		(*iter)->RenderSaying();

		if((*iter)->GetType() == OBJ_NPC)
		{
			(*iter)->RenderName();
		}

		//	画攻击数字
		pObject->RenderAttackNumber();
	}

	//	黑夜效果
	int nDarkMode = pTheGame->GetDarkMode();
	if(0 != nDarkMode)
	{
		HTEXTURE texDark = pTheGame->GetDarkModeTexture();
		if(texDark)
		{
			m_pRender->SetTexture(texDark);
			m_pRender->SetTextureRect(0, 0, 800, 600);
			m_pRender->Render(0, -60);
		}
	}

	//	攻击数字
	GameScene::sThis->RenderAttackNumber();

	//	目标名字
	if(pGameTarget)
	{
		pGameTarget->SetRenderMode(ORM_HIGHLIGHT);
		pGameTarget->HighlightRender();
		pGameTarget->SetRenderMode(ORM_NONE);
		pGameTarget->RenderName();
	}
	/*if(GameScene::sThis->GetTarget())
	{
		GameScene::sThis->GetTarget()->RenderName();
	}*/

	//	自己名字永远画
	if(pTheGame->Config_SelfName())
	{
		GamePlayer::GetInstance()->RenderName();
	}

	//	检测是否显示物品
	//if(AfxGetHge()->Input_GetKeyState(HGEK_ALT))
	{
		if(!m_xMouseItems.empty())
		{
			GROUNDITEMS::const_iterator begiter = m_xMouseItems.begin();
			int nCount = 0;
			int nTextWidth = 0;

			tex = pTheGame->GetGameRes()->GetTexture("bmcolor");
			if(tex)
			{
				m_pSpr->SetColor(ARGB(168, 255, 255, 255));
				m_pSpr->SetTexture(tex);

				for(begiter;
					begiter != m_xMouseItems.end();
					++begiter)
				{
					pItem = *begiter;
					nTextWidth = strlen(pItem->stAttrib.name) * 6;
					if(tex)
					{
						m_pSpr->SetTextureRect(0, 0, nTextWidth + 4, 12 + 4);
						
						nDrawX = (pItem->wPosX - m_rcClient.left) * UNIT_WIDTH - pPlayer->GetShiftX();
						nDrawY = (pItem->wPosY - m_rcClient.top) * UNIT_HEIGHT - pPlayer->GetShiftY();
						nDrawOftY = -(UNIT_HEIGHT / 2 - 10) - nCount * 16;
						nDrawOftX = (UNIT_WIDTH / 2) - nTextWidth / 2;
						m_pSpr->Render(nDrawX + nDrawOftX + g_nWholeMapOftX, nDrawY + nDrawOftY + g_nWholeMapOftY);

						if(MirGame::IsEquipItem(pItem->stAttrib))
						{
							if(HideAttribHelper::GetAllAttribCount(pItem->stAttrib.maxMP) != 0)
							{
								AfxGetPrinter()->SetColor(ARGB_RED);
							}
							else if(LOBYTE(pItem->stAttrib.level) > 0)
							{
								AfxGetPrinter()->SetColor(ARGB_YELLOW);
							}
							else
							{
								AfxGetPrinter()->SetColor(dwNormalColor);
							}
						}
						else
						{
							AfxGetPrinter()->SetColor(dwNormalColor);
						}

						/*char szItemShowName[25] = {0};
						if(MirGame::IsEquipItem(pItem->stAttrib) &&
							HideAttribHelper::GetAllAttribCount(pItem->stAttrib.maxMP) != 0)
						{
							sprintf(szItemShowName, "%s(*)", pItem->stAttrib.name);
						}
						else
						{
							strcpy(szItemShowName, pItem->stAttrib.name);
						}*/

						AfxGetPrinter()->Print(nDrawX + nDrawOftX + 2 + g_nWholeMapOftX,
							nDrawY + nDrawOftY + 2 + g_nWholeMapOftY, pItem->stAttrib.name);
					}
					++nCount;
				}

				m_pSpr->SetColor(ARGB(255, 255, 255, 255));
			}
		}
	}

	//	死亡灰色屏幕
	if(pPlayer->GetStatus() == PST_DEAD)
	{
		GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
		HTEXTURE texWhite = pPngMgr->GetTexture(14);
		if(texWhite)
		{
			m_pSpr->SetTexture(texWhite);
			m_pSpr->SetTextureRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			m_pSpr->SetColor(ARGB(128, 128, 128, 128));
			m_pSpr->Render(0, 0);
			m_pSpr->SetColor(ARGB_WHITE);
		}
	}

	//	小退提示
	if(pPlayer->IsRequestSmallQuit())
	{
		GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
		HTEXTURE texWhite = pPngMgr->GetTexture(14);
		if(texWhite)
		{
			m_pSpr->SetTexture(texWhite);
			m_pSpr->SetTextureRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			m_pSpr->SetColor(ARGB_BLACK);
			m_pSpr->Render(0, 0);
			m_pSpr->SetColor(ARGB_WHITE);

			AfxGetPrinter()->SetColor(ARGB_WHITE);
			AfxGetPrinter()->Print(WINDOW_WIDTH / 2 - 72, WINDOW_HEIGHT / 2 - 50, "正在请求退出，请稍候...");
		}
	}

	m_xAlphaDrawObjects.clear();
	m_xShowItems.clear();
	m_xMouseItems.clear();
}

void GameMapManager::RenderMid()
{
	//	渲染小背景
}

void GameMapManager::Render()
{
	UpdateClientRect();
	RenderBk();
	RenderFrImg();
	memcpy(&m_rcOldClient, &m_rcClient, sizeof(RECT));
}

void GameMapManager::RedrawBuffer()
{
	if(!m_pxSurface)
	{
		m_pxSurface = new MapSurface;
	}

	UpdateClientRect();
	ZeroMemory(&m_rcOldClient, sizeof(RECT));
	m_pxSurface->DrawEntire();
	memcpy(&m_rcOldClient, &m_rcClient, sizeof(RECT));
}

void GameMapManager::Update(float _dt)
{
	//	更新怪物 主要是自动走路 攻击
	int nCenRow = 0;
	int nCenCol = 0;
//	MapInfo info;
	//GetMapData()->GetMapInfo(info);
	////bool bAtk = false;
	//bool bFindTarget = false;

	//if(GamePlayer::GetInstance())
	//{
	//	nCenRow = GamePlayer::GetInstance()->GetObjy() / UNIT_HEIGHT;
	//	nCenCol = GamePlayer::GetInstance()->GetObjx() / UNIT_WIDTH;
	//}

	////	确定左上角坐标
	//nCenRow -= (VIEW_HEIGHT / UNIT_HEIGHT - 1) / 2;
	//nCenCol -= (VIEW_WIDTH / UNIT_WIDTH - 1) / 2;
	//GameObject* pObj = NULL;

	////	行的上面的偏移可以少点
	//for(int i = nCenRow - 2; i < nCenRow + VIEW_HEIGHT / UNIT_HEIGHT + VIEW_OFFSET; ++i)
	//{
	//	for(int j = nCenCol + VIEW_WIDTH / UNIT_WIDTH + 4; j >= nCenCol - 4; --j)
	//	{
	//		pObj = GetMonster(j, i);
	//		if(pObj)
	//		{
	//			if(pObj->GetType() == OBJ_MONS)
	//			{
	//				UpdateTheMonster((GameMonster*)pObj, _dt);
	//			}
	//			else if(pObj->GetType() == OBJ_NPC)
	//			{
	//				pObj->Update(_dt);
	//			}
	//		}
	//	}
	//}

	UpdateClientRect();
	GameScene::sThis->UpdateMapData(_dt);
}

void GameMapManager::UpdateTheMonster(GameMonster* _pmons, float _dt)
{
/*
#ifndef NETWORK_MODE
	GameMonster* pMons = _pmons;
	bool bFindTarget = false;
	static DWORD dwTimerCounter = 0;
	POINT pt;

	static const int nFindPos[] = {-1,-1,0,-1,1,-1,-1,0,1,0,-1,1,0,1,1,1};

	if(pMons->GetStatus() != PST_DEAD ||
		/ *没死或者没有到最后一帧 需要更新* /
		!pMons->IsLastFrame())
	{
		pMons->ProcUserCmd(pt);
		pMons->UpdateNetPacket();
		pMons->Update(_dt);
	}
#endif*/
}

//	辅助判断函数
bool GameMapManager::CanThrough(int _x, int _y)
{
	//	_x是x轴坐标 表示列 _y是行 
	MapInfo info;
	m_mapData.GetMapInfo(info);

	//	越界无法通过
	if(_x < 0 ||
		_y < 0 ||
		_x >= info.nCol ||
		_y >= info.nRow)
	{
		return false;
	}

	//	判断阻挡
	WORD wTile = m_mapData.GetCellData(_y, _x)->wBkImg;
	if(wTile & 0x8000)
	{
		return false;
	}
	wTile = m_mapData.GetCellData(_y, _x)->wFrImg;
	if(wTile & 0x8000)
	{
		return false;
	}

	//	有门的情况下 还需判断门是否打开
	if(HaveDoor(_x, _y))
	{
		return true;
		return IsDoorOpen(_x, _y);
	}

	//	被怪物阻挡了
	//GameObject* pObj = (GameObject*)m_mapData.GetCellData(_y, _x)->wMonster;
	const MemMapObject* pObjs = GameScene::sThis->GetMappedObjectsRelative(_x, _y);
	GameObject* pObj = NULL;
	if(pObjs)
	{
		for(int i = 0; i < MAX_CELL_OBJECTS; ++i)
		{
			pObj = pObjs->pObjects[i];
			if(pObj)
			{
				if(pObj->GetStatus() != PST_DEAD)
				{
					return false;
				}
			}
		}
	}
	
	/*GameObject* pObj = GameScene::sThis->GetMonster(_x, _y);
	if(pObj)
	{
		if(pObj->GetStatus() != PST_DEAD)
		{
			return false;
		}
	}*/
	return true;
}

bool GameMapManager::HaveDoor(int _x, int _y)
{
	MapInfo info;
	m_mapData.GetMapInfo(info);

	if(_x < 0 ||
		_y < 0 ||
		_x >= info.nCol ||
		_y >= info.nRow)
	{
		return false;
	}

	MapTile* pTile = m_mapData.GetCellData(_y, _x);

	if(pTile->bDoorIndex & 80)
	{
		return true;
	}

	return false;
}

bool GameMapManager::IsDoorOpen(int _x, int _y)
{
	MapInfo info;
	m_mapData.GetMapInfo(info);

	if(_x < 0 ||
		_y < 0 ||
		_x >= info.nCol ||
		_y >= info.nRow)
	{
		return false;
	}

	MapTile* pTile = m_mapData.GetCellData(_y, _x);

	//	判断是否有门
	if(pTile->bDoorIndex & 80)
	{
		//	在判断门是否开了
		if(pTile->bDoorOffset & 80)
		{
			return true;
		}
	}

	return false;
}

int GameMapManager::GetDoorIndex(int _x, int _y)
{
	MapInfo info;
	m_mapData.GetMapInfo(info);

	if(_x < 0 ||
		_y < 0 ||
		_x >= info.nCol ||
		_y >= info.nRow)
	{
		return 0;
	}

	MapTile* pTile = m_mapData.GetCellData(_y, _x);

	//	判断是否有门
	if(pTile->bDoorIndex & 80)
	{
		return pTile->bDoorIndex & (~80);
	}

	return 0;
}

bool GameMapManager::IsValidPoint(const POINT& _pt)
{
	MapInfo info;
	m_mapData.GetMapInfo(info);

	if(_pt.x < 0 ||
		_pt.y < 0 ||
		_pt.x >= info.nCol ||
		_pt.y >= info.nRow)
	{
		return false;
	}

	return true;
}



//	怪物部分
// bool GameMapManager::CreateMonster(int _x, int _y, int _id)
// {
// 	//GameObject* pObj = NULL;
// 	if(CanThrough(_x, _y))
// 	{
// 		if(GetMonster(_x, _y))
// 		{
// 			return false;
// 		}
// 
// 		if(_id < 40000)
// 		{
// 			//	Monster
// 			GameMonster* pObj = new GameMonster;
// 			if(!pObj->LoadProperty(_id))
// 			{
// 				AfxGetHge()->System_Log("读取怪物[%d]信息失败", _id);
// 			}
// 
// 			pObj->InitOtherRes(GameResourceManager::GetInstance()->GetTexs((GAME_RES)(RES_MON1 + pObj->m_renderInfo.wil)));
// 			pObj->SetReallyCoord(_x, _y);
// 			GetMapData()->GetCellData(_y, _x)->wMonster = (DWORD)pObj;
// 			return true;
// 		}
// 		else
// 		{
// 			//	NPC
// 			GameNPC* pObj = new GameNPC;
// 			if(!pObj->LoadProperty(_id))
// 			{
// 				AfxGetHge()->System_Log("读取怪物[%d]信息失败", _id);
// 			}
// 			pObj->SetReallyCoord(_x, _y);
// 			GetMapData()->GetCellData(_y, _x)->wMonster = (DWORD)pObj;
// 			return true;
// 		}
// 	}
// 	return false;
// }
// 
// GameObject* GameMapManager::GetMonster(int _x, int _y)
// {
// 	MapInfo info;
// 	GetMapData()->GetMapInfo(info);
// 	if(_x >= info.nCol ||
// 		_x < 0 ||
// 		_y >= info.nRow ||
// 		_y < 0)
// 	{
// 		return NULL;
// 	}
// 	GameObject* pObj = (GameObject*)(GetMapData()->GetCellData(_y, _x)->wMonster);
// 	if(pObj != NULL)
// 	{
// 		if(pObj->GetType() == OBJ_MONS ||
// 			pObj->GetType() == OBJ_NPC)
// 		{
// 			return (GameObject*)pObj;
// 		}
// 		else
// 		{
// 			return NULL;
// 		}
// 	}
// 	return NULL;
// }
// 
// void GameMapManager::DeleteMonster(int _x, int _y)
// {
// 	GameObject* pObj = (GameObject*)GetMapData()->GetCellData(_y, _x)->wMonster;
// 	if(pObj)
// 	{
// 		delete pObj;
// 		GetMapData()->GetCellData(_y, _x)->wMonster = 0;
// 	}
// }
// 
// void GameMapManager::RemoveAllMonster()
// {
// 	TileMap& map = *GetMapData();
// 	MapInfo info;
// 	map.GetMapInfo(info);
// 	GameObject* pObj = NULL;
// 	for(int i = 0; i < info.nRow; ++i)
// 	{
// 		for(int j = 0; j < info.nCol; ++j)
// 		{
// 			pObj = (GameObject*)map.GetCellData(i, j)->wMonster;
// 			if(pObj)
// 			{
// 				delete pObj;
// 				map.GetCellData(i, j)->wMonster = 0;
// 			}
// 		}
// 	}
// }
// 
// bool GameMapManager::MoveMonster(int _dx, int _dy, GameMonster* _mons)
// {
// 	if(_dx == 0 &&
// 		_dy == 0)
// 	{
// 		return true;
// 	}
// 
// 	BYTE drt = 0;
// 	PLAYER_DIRECTION pd;
// 	//	视野之内 不断靠近
// 	if(_dx > 0)
// 	{
// 		drt |= DRT_RIGHT;
// 	}
// 	else if(_dx < 0)
// 	{
// 		drt |= DRT_LEFT;
// 	}
// 
// 	if(_dy > 0)
// 	{
// 		drt |= DRT_DOWN;
// 	}
// 	else if(_dy < 0)
// 	{
// 		drt |= DRT_UP;
// 	}
// 
// 	if(drt != 0)
// 	{
// 		//int cx, cy;
// 		if(drt == DRT_LEFT)
// 		{
// 			pd = PDT_LEFT;
// 		}
// 		else if(drt == DRT_RIGHT)
// 		{
// 			pd = PDT_RIGHT;
// 		}
// 		else if(drt == DRT_UP)
// 		{
// 			pd = PDT_UP;
// 		}
// 		else if(drt == DRT_DOWN)
// 		{
// 			pd = PDT_DOWN;
// 		}
// 		else if(drt == DRT_LEFTUP)
// 		{
// 			pd = PDT_LEFTUP;
// 		}
// 		else if (drt == DRT_LEFTDOWN)
// 		{
// 			pd = PDT_LEFTDOWN;
// 		}
// 		else if(drt == DRT_RIGHTUP)
// 		{
// 			pd = PDT_RIGHTUP;
// 		}
// 		else if(drt == DRT_RIGHTDOWN)
// 		{
// 			pd = PDT_RIGHTDOWN;
// 		}
// 	}
// 
// 	int sx = _mons->GetCoordX();
// 	int sy = _mons->GetCoordY();
// 	assert(GetMapData()->GetCellData(sy, sx)->wMonster == (DWORD)_mons);
// 
// 	if(GetMapData()->GetCellData(sy + _dy, sx + _dx)->wMonster == 0 &&
// 		CanThrough(sx + _dx, sy + _dy))
// 	{
// 		GetMapData()->GetCellData(sy, sx)->wMonster = 0;
// // 		_mons->SetDirection(pd);
// // 		_mons->SetStatus(PST_WALK);
// // 		_mons->SetCurFrame(0);
// // 		_mons->SetOffsetCoordX(_dx);
// // 		_mons->SetOffsetCoordY(_dy);
// 		ActionPacket* pPacket = new ActionPacket;
// 		pPacket->wCmd = PC_ACTION_WALK;
// 		pPacket->wParam0 = (WORD)pd;
// 		pPacket->wParam1 = (WORD)(_mons->GetCoordX());
// 		pPacket->wParam2 = (WORD)(_mons->GetCoordY());
// 		pPacket->wParam3 = _dx;
// 		pPacket->wParam4 = _dy;
// 		_mons->PushActionPacket(pPacket);
// 		GetMapData()->GetCellData(_mons->GetCoordY(), _mons->GetCoordX())->wMonster = (DWORD)_mons;
// 		return true;
// 	}
// 	return false;
// }