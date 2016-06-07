#include "../GameDialog/GameMiniMapDlg.h"
#include "../GameScene/GameInfoManager.h"
#include "../Common/GlobalFunction.h"
#include "../GameScene/GameResourceManager.h"
#include <io.h>
#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameMapManager.h"
#include <assert.h>
#include "../GameScene/GameScene.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
#define MAP_TRANSPARENT_COLOR	ARGB(190, 255, 255, 255)
//////////////////////////////////////////////////////////////////////////

GameMiniMapDlg::GameMiniMapDlg()
{
	m_rcClient.left = WINDOW_WIDTH - 157;
	m_rcClient.right = WINDOW_WIDTH;
	m_rcClient.top = 5;
	m_rcClient.bottom = m_rcClient.top + 110 + 5;
	//		m_fScaleX = m_fScaleY = 1.0f;
	//		m_pSpr = NULL;
	m_pPoint = NULL;
	//		m_nOffsetX = m_nOffsetY = 0;
	m_texPoint = 0;
	m_texMap = 0;
	SetVisible(true);
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_bLoadOk = false;
	m_nMMapHeight = m_nMMapWidth = 0;
	m_nPointPosX = m_nPointPosY = 0;
	m_bMiniBtnState = BTN_STATE_NORMAL;
	m_bMiniMode = false;

	m_rcMini.left = WINDOW_WIDTH - 160;
	m_rcMini.right = m_rcMini.left + 12;
	m_rcMini.top = 52;
	m_rcMini.bottom = 67;

	m_dwMapID = 0;
}

bool GameMiniMapDlg::ProcUserCmd(const POINT& _mp)
{
	return true;
}

void GameMiniMapDlg::Render()
{
	static float fFlash = 0.0f;
	fFlash += AfxGetHge()->Timer_GetDelta();
	HTEXTURE tex = 0;
	hgeResourceManager* pMgr = GameScene::sThis->GetMainOpt()->GetResourceMgr();
	int nViewWidth = m_rcClient.right - m_rcClient.left;
	int nViewHeight = m_rcClient.bottom - m_rcClient.top;
	int nDrawX = 0;
	int nDrawY = 0;

	m_texMap = GameResourceManager::GetInstance()->GetTexs(RES_MMAP)->GetTexture(m_dwMapID);
	bool bMapExist = m_texMap != NULL ? true : false;
	bMapExist = true;

	if(!m_bMiniMode)
	{
		if(m_bLoadOk &&
			IsVisible())
		{
			if(m_texMap)
			{
				m_pRender->SetTexture(m_texMap);
			}
			else
			{
				m_pRender->SetTexture(0);
			}
			if(UpdatePos())
			{
				if(m_pRender)
				{
					//m_pSpr->RenderEx(m_rcClient.left, m_rcClient.top, 0.0f, m_fScale, m_fScale);
					bool bNeedback = false;

					if(m_nMMapWidth < nViewWidth)
					{
						nDrawX = (nViewWidth - m_nMMapWidth) / 2;
						bNeedback = true;
					}
					if(m_nMMapHeight < nViewHeight)
					{
						nDrawY = (nViewHeight - m_nMMapHeight) / 2;
						bNeedback = true;
					}

					if(pTheGame->GetShowMapSnap() &&
						bMapExist)
					{
						if(bNeedback)
						{
							DWORD dwPreCol = m_pRender->GetColor();
							float fX = 0;
							float fY = 0;
							float fW = 0;
							float fH = 0;
							m_pRender->GetTextureRect(&fX, &fY, &fW, &fH);
							m_pRender->SetColor(ARGB_WHITE);
							HTEXTURE texbk = GameScene::sThis->GetMainOpt()->GetResourceMgr()->GetTexture("bmcolor");
							if(texbk)
							{
								m_pRender->SetTexture(texbk);
								m_pRender->SetTextureRect(0, 0, nViewWidth, nViewHeight);
								m_pRender->Render(m_rcClient.left, m_rcClient.top);
							}

							m_pRender->SetTexture(m_texMap);
							m_pRender->SetTextureRect(fX, fY, fW, fH);
						}
						
						m_pRender->SetColor(MAP_TRANSPARENT_COLOR);
						m_pRender->Render(nDrawX + m_rcClient.left, nDrawY + m_rcClient.top);
						m_pRender->SetColor(ARGB_WHITE);
					}
					else
					{
						hgeSprite spr(0, 0, 0, 160, 120);
						spr.SetColor(ARGB(170, 0, 0, 0));
						spr.Render(WINDOW_WIDTH - 160, 0);

						AfxGetPrinter()->SetColor(ARGB_WHITE);
						AfxGetPrinter()->Print(WINDOW_WIDTH - 160 + 50, 50, "%s", "无可用地图");
					}
				}
				if(m_pPoint)
				{
					if(pTheGame->GetShowMapSnap() &&
						bMapExist)
					{
						HTEXTURE texPoint = 0;
						DWORD dwPreColor = m_pPoint->GetColor();
						hgeResourceManager* pResMgr = pTheGame->GetGameRes();

						if(fFlash > 1.5f)
						{
							fFlash = 0.0f;
						}
						else if(fFlash > 0.75f)
						{
							/*texPoint = pResMgr->GetTexture("minimap_self");
							if(texPoint)
							{
								m_pPoint->SetTexture(texPoint);
								m_pPoint->SetTextureRect(0, 0, 10, 10);
								m_pPoint->Render(m_nPointPosX - 5, m_nPointPosY - 5);
							}*/
						}

						PLAYERLIST& xNPCs = GameScene::sThis->GetNPCList();
						int nOftX = 0;
						int nOftY = 0;

						for(PLAYERLIST::const_iterator begiter = xNPCs.begin();
							begiter != xNPCs.end();
							++begiter)
						{
							nOftX = (*begiter)->GetCoordX() * 1.5f - GamePlayer::GetInstance()->GetCoordX() * 1.5f + m_nPointPosX - m_rcClient.left;
							nOftY = (*begiter)->GetCoordY() - GamePlayer::GetInstance()->GetCoordY() + m_nPointPosY - m_rcClient.top;

							if(nOftX > 0 &&
								nOftX < (m_rcClient.right - m_rcClient.left) &&
								nOftY > 0 &&
								nOftY < (m_rcClient.bottom - m_rcClient.top - 16))
							{
								bool bNeedRender = false;
								string xRenderType;

								if((*begiter)->GetType() == OBJ_MONS &&
									(*begiter)->GetAttrib()->HP != 0 &&
									(*begiter)->GetStatus() != PST_DEAD)
								{
									if(TEST_FLAG_BOOL((*begiter)->GetAttrib()->maxEXPR, MAXEXPR_MASK_BOSS))
									{
										//m_pPoint->SetColor(ARGB_YELLOW);
										xRenderType = "minimap_boss";
									}
									else
									{
										//m_pPoint->SetColor(ARGB_RED);
										xRenderType = "minimap_mons";
									}

									bNeedRender = true;
								}
								else if((*begiter)->GetType() == OBJ_NPC)
								{
									//m_pPoint->SetColor(ARGB_GREEN);
									xRenderType = "minimap_npc";
									bNeedRender = true;
								}
								else if((*begiter)->GetType() == OBJ_OTHERPLAYER)
								{
									//m_pPoint->SetColor(ARGB_BLUE);
									xRenderType = "minimap_player";
									bNeedRender = true;
								}

								if(bNeedRender)
								{
									//if(fFlash > 0.75f)
									{
										//m_pPoint->Render(nOftX + m_rcClient.left, nOftY + m_rcClient.top);
										if(!xRenderType.empty())
										{
											texPoint = pResMgr->GetTexture(xRenderType.c_str());
											if(texPoint)
											{
												//m_pPoint->SetColor(MAP_TRANSPARENT_COLOR);
												m_pPoint->SetTexture(texPoint);
												if(xRenderType == "minimap_npc")
												{
													m_pPoint->SetTextureRect(0, 0, 10, 10);
													m_pPoint->Render(nOftX + m_rcClient.left - 5, nOftY + m_rcClient.top - 5);
												}
												else
												{
													m_pPoint->SetTextureRect(0, 0, 6, 6);
													m_pPoint->Render(nOftX + m_rcClient.left - 3, nOftY + m_rcClient.top - 3);
												}
												m_pRender->SetColor(ARGB_WHITE);
												//m_pPoint->Render(nOftX + m_rcClient.left, nOftY + m_rcClient.top);
											}
										}
									}
								}
							}
						}

						//if(fFlash > 0.75f)
						{
							texPoint = pResMgr->GetTexture("minimap_self");
							if(texPoint)
							{
								m_pPoint->SetTexture(texPoint);
								m_pPoint->SetTextureRect(0, 0, 10, 10);
								m_pPoint->Render(m_nPointPosX - 5, m_nPointPosY - 5);
							}
						}

						//m_pPoint->SetColor(dwPreColor);
					}
				}
			}
		}
		else
		{
			hgeSprite spr(0, 0, 0, 160, 120);
			spr.SetColor(ARGB(170, 0, 0, 0));
			spr.Render(WINDOW_WIDTH - 160, 0);

			AfxGetPrinter()->SetColor(ARGB_WHITE);
			AfxGetPrinter()->Print(WINDOW_WIDTH - 160 + 50, 50, "%s", "无可用地图");
		}

		if(pMgr)
		{
			tex = pMgr->GetTexture("mmapbk");
			if(tex)
			{
				m_pRender->SetColor(MAP_TRANSPARENT_COLOR);
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, 160, 120);
				m_pRender->Render(WINDOW_WIDTH - 160, 0);
				m_pRender->SetColor(ARGB_WHITE);
			}
		}
	}

	if(!m_bMiniMode)
	{
		tex = pMgr->GetTexture("crabtn");
	}
	else
	{
		tex = pMgr->GetTexture("clabtn");
	}

	if(tex)
	{
		m_pRender->SetColor(MAP_TRANSPARENT_COLOR);
		m_pRender->SetTexture(tex);
		if(m_bMiniBtnState == BTN_STATE_NORMAL)
		{
			m_pRender->SetTextureRect(0, 0, 12, 15);
		}
		else if(m_bMiniBtnState == BTN_STATE_OVER)
		{
			m_pRender->SetTextureRect(0, 15 * 1, 12, 15);
		}
		else if(m_bMiniBtnState == BTN_STATE_DOWN)
		{
			m_pRender->SetTextureRect(0, 15 * 2, 12, 15);
		}
		m_pRender->Render(m_rcMini.left, m_rcMini.top);
		m_pRender->SetColor(ARGB_WHITE);
	}
}

bool GameMiniMapDlg::UpdatePos()
{
	if(!m_bLoadOk)
	{
		return false;
	}
	m_texMap = GameResourceManager::GetInstance()->GetTexs(RES_MMAP)->GetTexture(m_dwMapID);

	if(0 == m_texMap)
	{
		return false;
	}
	float fPosx = GamePlayer::GetInstance()->GetObjx() + UNIT_WIDTH * 0.5;
	float fPosy = GamePlayer::GetInstance()->GetObjy() + UNIT_HEIGHT * 0.5;
	fPosx /= 32.0f;
	fPosy /= 32.0f;
	float fDrawPosx = fPosx - (m_rcClient.right - m_rcClient.left) * 0.5;
	float fDrawPosy = fPosy - (m_rcClient.bottom - m_rcClient.top) * 0.5;
	int nRectX = GamePlayer::GetInstance()->GetCoordX() * 1.5;
	int nRectY = GamePlayer::GetInstance()->GetCoordY();
	int nWidth = m_rcClient.right - m_rcClient.left;
	int nHeight = m_rcClient.bottom - m_rcClient.top;

	nRectX -= ((nWidth - 1) / 2);
	nRectY -= ((nHeight - 1) / 2);
	m_nPointPosX = m_rcClient.left + (nWidth - 1) / 2;
	m_nPointPosY = m_rcClient.top + (nHeight - 1) / 2;
	//	人物坐标即为小地图坐标

	int nDrawX = 0;
	int nDrawY = 0;
	int nViewWidth = nWidth;
	int nViewHeight = nHeight;

	if(m_nMMapWidth < nViewWidth)
	{
		nDrawX = (nViewWidth - m_nMMapWidth) / 2;
	}
	if(m_nMMapHeight < nViewHeight)
	{
		nDrawY = (nViewHeight - m_nMMapHeight) / 2;
	}

	if(m_nMMapWidth >= nWidth &&
		m_nMMapHeight >= nHeight)
	{
		if(nRectX < 0)
		{
			m_nPointPosX += nRectX;
			nRectX = 0;
		}
		else if(nRectX + nWidth > m_nMMapWidth)
		{
			m_nPointPosX += (nRectX + nWidth - m_nMMapWidth);
			nRectX = m_nMMapWidth - nWidth;
		}

		if(nRectY < 0)
		{
			m_nPointPosY += nRectY;
			nRectY = 0;
		}
		else if(nRectY + nHeight > m_nMMapHeight)
		{
			m_nPointPosY += (nRectY + nHeight - m_nMMapHeight);
			nRectY = m_nMMapHeight - nHeight;
		}
		if(m_pRender)
		{
			m_pRender->SetTextureRect(nRectX, nRectY, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
		}
	}
	else
	{
		if(m_nMMapWidth < nWidth &&
			m_nMMapHeight >= nHeight)
		{
			if(nRectY < 0)
			{
				m_nPointPosY += nRectY;
				nRectY = 0;
			}
			else if(nRectY + nHeight > m_nMMapHeight)
			{
				m_nPointPosY += (nRectY + nHeight - m_nMMapHeight);
				nRectY = m_nMMapHeight - nHeight;
			}
			m_nPointPosX = m_rcClient.left + nDrawX + GamePlayer::GetInstance()->GetCoordX() * 1.5;

			if(m_pRender)
			{
				m_pRender->SetTextureRect(0, nRectY, m_nMMapWidth, m_rcClient.bottom - m_rcClient.top);
			}
		}
		else if(m_nMMapHeight < nHeight &&
			m_nMMapWidth >= nWidth)
		{
			if(nRectX < 0)
			{
				m_nPointPosX += nRectX;
				nRectX = 0;
			}
			else if(nRectX + nWidth > m_nMMapWidth)
			{
				m_nPointPosX += (nRectX + nWidth - m_nMMapWidth);
				nRectX = m_nMMapWidth - nWidth;
			}
			m_nPointPosY = m_rcClient.top + nDrawY + GamePlayer::GetInstance()->GetCoordY();

			if(m_pRender)
			{
				m_pRender->SetTextureRect(nRectX, 0, m_rcClient.right - m_rcClient.left, m_nMMapHeight);
			}
		}
		else
		{
			m_nPointPosX = m_rcClient.left + nDrawX + GamePlayer::GetInstance()->GetCoordX() * 1.5;
			m_nPointPosY = m_rcClient.top + nDrawY + GamePlayer::GetInstance()->GetCoordY();
			if(m_pRender)
			{
				m_pRender->SetTextureRect(0, 0, m_nMMapWidth, m_nMMapHeight);
			}
		}
	}
	return true;
}

void GameMiniMapDlg::Update(float _dt)
{
	m_bMiniBtnState = BTN_STATE_NORMAL;
	POINT ptMouse;
	float x, y;
	AfxGetHge()->Input_GetMousePos(&x, &y);
	ptMouse.x = x;
	ptMouse.y = y;
	if(PtInRect(&m_rcMini, ptMouse))
	{
		m_bMiniBtnState = BTN_STATE_OVER;
		if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
		{
			m_bMiniBtnState = BTN_STATE_DOWN;
		}
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			if(m_bMiniMode)
			{
				m_bMiniMode = false;
				m_rcMini.left = WINDOW_WIDTH - 160;
				m_rcMini.right = m_rcMini.left + 12;
			}
			else
			{
				m_bMiniMode = true;
				m_rcMini.left = WINDOW_WIDTH - 12;
				m_rcMini.right = WINDOW_WIDTH;
			}
		}
	}
}

bool GameMiniMapDlg::SetMap(const char* lpszMapName)
{
// 	if(_access(lpszMapName, 0) != 0)
// 	{
// 		//	不存在
// 		return false;
// 	}
// 	delete m_pSpr;
// 	m_pSpr = NULL;

	/*int nIndex = 0;
	if(!GameInfoManager::GetInstance()->GetMiniMapIndex(lpszMapName, &nIndex))
	{
		goto _LABEL_CLEAN_;
	}

	GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_MMAP);
	if(ttex)
	{
		HTEXTURE tex = ttex->GetTexture(nIndex);
		if(tex)
		{
			if(!m_pSpr)
			{
				m_pSpr = new hgeSprite(tex, 0, 0, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
			}
 			else
 			{
 				m_pSpr->SetTexture(tex);
 				m_pSpr->SetTextureRect(0, 0, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
 			}
		}

		if(tex)
		{
			MapInfo info;
			GameMapManager::GetInstance()->GetMapData()->GetMapInfo(info);
// 			m_fScaleX = (info.nCol * info.nWidth) / ttex->GetTextureWidth(tex);
// 			m_fScaleY = (info.nRow * info.nHeight) / ttex->GetTextureHeight(tex);
			m_fScaleX = (info.nCol * info.nWidth) / ttex->GetTextureWidth(nIndex);
			m_fScaleY = (info.nRow * info.nHeight) / ttex->GetTextureHeight(nIndex);
		}

		if(!m_pPoint)
		{
				{
					//m_pPoint = new hgeSprite(tex, 22, 15, 4, 4);
					m_texPoint = AfxGetHge()->Texture_Create(3, 3);
					int nActWidth = AfxGetHge()->Texture_GetWidth(m_texPoint);
					int nActHeight = AfxGetHge()->Texture_GetHeight(m_texPoint);
					DWORD* pBuf = AfxGetHge()->Texture_Lock(m_texPoint, false);
					ZeroMemory(pBuf, nActHeight * nActWidth * sizeof(DWORD));

					{
						pBuf[1] = ARGB(255, 255, 255, 255);
						pBuf[nActWidth + 0] = ARGB(255, 255, 255, 255);
						pBuf[nActWidth + 1] = ARGB(255, 255, 255, 255);
						pBuf[nActWidth + 2] = ARGB(255, 255, 255, 255);
						pBuf[nActWidth * 2 + 1] = ARGB(255, 255, 255, 255);
					}

					AfxGetHge()->Texture_Unlock(m_texPoint);
					m_pPoint = new hgeSprite(m_texPoint, 0, 0, 3, 3);
				}
		}*/
// 		else
// 		{
// 			//	因为切换地图的时候资源释放了 需要重新载入
// 			ttex = GameResourceManager::GetInstance()->GetTexs(RES_DNITEMS);
// 			if(ttex)
// 			{
// 				tex = ttex->GetTexture(116);
// 				if(tex)
// 				{
// 					m_pPoint->SetTexture(tex);
// 					m_pPoint->SetTextureRect(22, 15, 4, 4);
// 				}
// 			}
// 		}
//	}
	
	return true;

_LABEL_CLEAN_:
	//delete m_pPoint;
	//m_pPoint = NULL;
//	delete m_pSpr;
//	m_pSpr = NULL;
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool GameMiniMapDlg::SetMap(DWORD _dwMapID)
{
	m_dwMapID = _dwMapID;
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MMAP);
	m_bLoadOk = false;
	if(pTexMgr == NULL)
	{
		return false;
	}

	m_texMap = pTexMgr->GetTexture(_dwMapID);
	if(NULL == m_texMap)
	{
		return false;
	}
	//m_pRender->SetTexture(m_texMap);
	if(!m_pPoint)
	{
		m_texPoint = AfxGetHge()->Texture_Create(3, 3);
		int nActWidth = AfxGetHge()->Texture_GetWidth(m_texPoint);
		int nActHeight = AfxGetHge()->Texture_GetHeight(m_texPoint);
		DWORD* pBuf = AfxGetHge()->Texture_Lock(m_texPoint, false);
		ZeroMemory(pBuf, nActHeight * nActWidth * sizeof(DWORD));

		{
			pBuf[1] = ARGB(255, 255, 255, 255);
			pBuf[nActWidth + 0] = ARGB(255, 255, 255, 255);
			pBuf[nActWidth + 1] = ARGB(255, 255, 255, 255);
			pBuf[nActWidth + 2] = ARGB(255, 255, 255, 255);
			pBuf[nActWidth * 2 + 1] = ARGB(255, 255, 255, 255);
		}

		AfxGetHge()->Texture_Unlock(m_texPoint);
		m_pPoint = new hgeSprite(m_texPoint, 0, 0, 3, 3);
	}

	if(m_texMap)
	{
		m_nMMapWidth = pTexMgr->GetTextureWidth(_dwMapID);
		m_nMMapHeight = pTexMgr->GetTextureHeight(_dwMapID);
		//MapInfo info;
		//GameMapManager::GetInstance()->GetMapData()->GetMapInfo(info);
		//int nTexWidth = pTexMgr->GetTextureWidth(_dwMapID);
		//int nTexHeight = pTexMgr->GetTextureHeight(_dwMapID);
		//assert(nTexHeight != 0 && nTexWidth != 0);
		//m_fScaleX = (info.nCol * info.nWidth) / nTexWidth;
		//m_fScaleY = (info.nRow * info.nHeight) / nTexHeight;
		//m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
	}
	m_bLoadOk = true;
	return true;
}