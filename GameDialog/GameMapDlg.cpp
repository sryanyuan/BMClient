#include "GameCommonDlg.h"
#include "GameDlgBase.h"
#include "../GameScene/GameResourceManager.h"
#include "../GameScene/GameScene.h"
#include "../BackMir/BackMir.h"
#include "GameInfoBoardDlg.h"
//////////////////////////////////////////////////////////////////////////
static const char* s_pszWindowTip = "[移动地图]按住鼠标右键即可拖动地图\
\n[快速传送]装备传送戒指后，点击相应地图区域即可快速传送";
//////////////////////////////////////////////////////////////////////////
GameMapDlg::GameMapDlg()
{
	m_rcClient.right = 500;
	m_rcClient.left = (VIEW_WIDTH - m_rcClient.right) / 2;
	m_rcClient.right = m_rcClient.left + 500;
	m_rcClient.top = 20;
	m_rcClient.bottom = 420;
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	CalcCloseButtonPosition();
	SetVisible(false);

	m_ptLeftTopPos.x = m_ptLeftTopPos.y = 0;

	SetWindowTitle("地图");
	SetWindowTip(s_pszWindowTip);
	//SetWindowInfo("提示:在地图上按住右键可以移动地图");

	m_rcMapView.left = 10;
	m_rcMapView.right = m_rcClient.right - m_rcClient.left - 10;
	m_rcMapView.top = 75;
	m_rcMapView.bottom = m_rcClient.bottom - m_rcClient.top - 22;
	m_bRightDown = false;
	m_nMapHeight = m_nMapWidth = 0;

	m_nShowX = m_nShowY = -1;
}

GameMapDlg::~GameMapDlg()
{

}

//////////////////////////////////////////////////////////////////////////
bool GameMapDlg::CreateDlg()
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
void GameMapDlg::Render()
{
	__super::Render();

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_BMAP);
	if(NULL == pTexMgr)
	{
		return;
	}

	if(!pTheGame->GetShowMapSnap())
	{
		return;
	}

	int nViewWidth = m_rcMapView.right - m_rcMapView.left;
	int nViewHeight = m_rcMapView.bottom - m_rcMapView.top;
	int nDrawX = 0;
	int nDrawY = 0;
	int nMapID = GamePlayer::GetInstance()->GetMapID();
	HTEXTURE tex = pTexMgr->GetTexture(nMapID);
	int nRectX = GamePlayer::GetInstance()->GetCoordX() * 3;
	int nRectY = GamePlayer::GetInstance()->GetCoordY() * 2;
	/*HTEXTURE texPoint = GameScene::sThis->GetMiniMapDlg()->GetPointTexture();*/
	HTEXTURE texPoint = 0;

	static float fFlash = 0.0f;
	static bool bShow = true;
	fFlash += AfxGetHge()->Timer_GetDelta();
	if(fFlash > 0.5f)
	{
		fFlash = 0.0f;
		bShow = !bShow;
	}
	bShow = true;

	int nTexWidth = 0;
	int nTexHeight = 0;
	if(tex)
	{
		nTexHeight = pTexMgr->GetTextureHeight(nMapID);
		nTexWidth = pTexMgr->GetTextureWidth(nMapID);
		m_nMapWidth = nTexWidth;
		m_nMapHeight = nTexHeight;

		if(nTexWidth < nViewWidth)
		{
			nDrawX = (nViewWidth - nTexWidth) / 2;
		}
		if(nTexHeight < nViewHeight)
		{
			nDrawY = (nViewHeight - nTexHeight) / 2;
		}

		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(m_ptLeftTopPos.x, m_ptLeftTopPos.y, nTexWidth > nViewWidth ? nViewWidth : nTexWidth, nTexHeight > nViewHeight ? nViewHeight : nTexHeight);
		m_pRender->Render(RELATIVE_X(nDrawX + m_rcMapView.left), RELATIVE_Y(nDrawY + m_rcMapView.top));
	}

		/*if(m_nShowX != -1)
		{
			AfxGetPrinter()->SetColor(ARGB_GREEN);
			AfxGetPrinter()->Print(m_ptMouse.x, m_ptMouse.y - 15, "%d,%d", m_nShowX, m_nShowY);
		}*/

		DWORD dwPreCol = m_pRender->GetColor();
		hgeResourceManager* pResMgr = pTheGame->GetGameRes();

		if(/*texPoint &&*/
			bShow)
		{
			/*if(nRectX - m_ptLeftTopPos.x > 0 &&
				nRectX - m_ptLeftTopPos.x < nViewWidth &&
				nRectY - m_ptLeftTopPos.y > 0 &&
				nRectY - m_ptLeftTopPos.y < nViewHeight)
			{
				texPoint = pResMgr->GetTexture("minimap_self");
				if(texPoint)
				{
					m_pRender->SetTexture(texPoint);
					m_pRender->SetTextureRect(0, 0, 10, 10);
					m_pRender->Render(RELATIVE_X(nDrawX + m_rcMapView.left + (nRectX - m_ptLeftTopPos.x) - 5),
						RELATIVE_Y(nDrawY + m_rcMapView.top + (nRectY - m_ptLeftTopPos.y) - 5));
				}
			}*/
			int nSelfRenderX = RELATIVE_X(nDrawX + m_rcMapView.left + (nRectX - m_ptLeftTopPos.x) - 5);
			int nSelfRenderY = RELATIVE_Y(nDrawY + m_rcMapView.top + (nRectY - m_ptLeftTopPos.y) - 5);

			PLAYERLIST& xNPCs = GameScene::sThis->GetNPCList();
			PLAYERLIST::const_iterator begiter = xNPCs.begin();
			GameObject* pObj = NULL;
			for(begiter;
				begiter != xNPCs.end();
				++begiter)
			{
				pObj = *begiter;
				if(pObj->GetType() == OBJ_MONS &&
					pObj->GetStatus() != PST_DEAD)
				{
					nRectX = pObj->GetCoordX() * 3;
					nRectY = pObj->GetCoordY() * 2;
					if(nRectX - m_ptLeftTopPos.x > 0 &&
						nRectX - m_ptLeftTopPos.x < nViewWidth &&
						nRectY - m_ptLeftTopPos.y > 0 &&
						nRectY - m_ptLeftTopPos.y < nViewHeight)
					{
						if(TEST_FLAG_BOOL(pObj->GetAttrib()->maxEXPR, MAXEXPR_MASK_BOSS))
						{
							//m_pRender->SetColor(ARGB_YELLOW);
							texPoint = pResMgr->GetTexture("minimap_boss");
							if(texPoint)
							{
								m_pRender->SetTexture(texPoint);
								m_pRender->SetTextureRect(0, 0, 6, 6);
								m_pRender->Render(RELATIVE_X(nDrawX + m_rcMapView.left + (nRectX - m_ptLeftTopPos.x) - 3),
									RELATIVE_Y(nDrawY + m_rcMapView.top + (nRectY - m_ptLeftTopPos.y) - 3));
							}
						}
						else
						{
							//m_pRender->SetColor(ARGB_RED);
							texPoint = pResMgr->GetTexture("minimap_mons");
							if(texPoint)
							{
								m_pRender->SetTexture(texPoint);
								m_pRender->SetTextureRect(0, 0, 6, 6);
								m_pRender->Render(RELATIVE_X(nDrawX + m_rcMapView.left + (nRectX - m_ptLeftTopPos.x) - 3),
									RELATIVE_Y(nDrawY + m_rcMapView.top + (nRectY - m_ptLeftTopPos.y) - 3));
							}
						}

						/*m_pRender->SetTexture(texPoint);
						m_pRender->SetTextureRect(0, 0, 3, 3);
						m_pRender->Render(RELATIVE_X(nDrawX + m_rcMapView.left + (nRectX - m_ptLeftTopPos.x)),
							RELATIVE_Y(nDrawY + m_rcMapView.top + (nRectY - m_ptLeftTopPos.y)));*/
					}
				}
				else if(pObj->GetType() == OBJ_NPC)
				{
					nRectX = pObj->GetCoordX() * 3;
					nRectY = pObj->GetCoordY() * 2;
					if(nRectX - m_ptLeftTopPos.x > 0 &&
						nRectX - m_ptLeftTopPos.x < nViewWidth &&
						nRectY - m_ptLeftTopPos.y > 0 &&
						nRectY - m_ptLeftTopPos.y < nViewHeight)
					{
						/*m_pRender->SetTexture(texPoint);
						m_pRender->SetTextureRect(0, 0, 3, 3);
						m_pRender->SetColor(ARGB_GREEN);
						m_pRender->Render(RELATIVE_X(nDrawX + m_rcMapView.left + (nRectX - m_ptLeftTopPos.x)),
							RELATIVE_Y(nDrawY + m_rcMapView.top + (nRectY - m_ptLeftTopPos.y)));*/

						texPoint = pResMgr->GetTexture("minimap_npc");
						if(texPoint)
						{
							m_pRender->SetTexture(texPoint);
							m_pRender->SetTextureRect(0, 0, 10, 10);
							//m_pRender->SetColor(ARGB_GREEN);
							m_pRender->Render(RELATIVE_X(nDrawX + m_rcMapView.left + (nRectX - m_ptLeftTopPos.x) - 5),
								RELATIVE_Y(nDrawY + m_rcMapView.top + (nRectY - m_ptLeftTopPos.y) - 5));
						}
					}
				}
				else if(pObj->GetType() == OBJ_OTHERPLAYER)
				{
					nRectX = pObj->GetCoordX() * 3;
					nRectY = pObj->GetCoordY() * 2;
					if(nRectX - m_ptLeftTopPos.x > 0 &&
						nRectX - m_ptLeftTopPos.x < nViewWidth &&
						nRectY - m_ptLeftTopPos.y > 0 &&
						nRectY - m_ptLeftTopPos.y < nViewHeight)
					{
						/*m_pRender->SetTexture(texPoint);
						m_pRender->SetTextureRect(0, 0, 3, 3);
						m_pRender->SetColor(ARGB_BLUE);
						m_pRender->Render(RELATIVE_X(nDrawX + m_rcMapView.left + (nRectX - m_ptLeftTopPos.x)),
							RELATIVE_Y(nDrawY + m_rcMapView.top + (nRectY - m_ptLeftTopPos.y)));*/

						texPoint = pResMgr->GetTexture("minimap_player");
						if(texPoint)
						{
							m_pRender->SetTexture(texPoint);
							m_pRender->SetTextureRect(0, 0, 6, 6);
							//m_pRender->SetColor(ARGB_BLUE);
							m_pRender->Render(RELATIVE_X(nDrawX + m_rcMapView.left + (nRectX - m_ptLeftTopPos.x) - 3),
								RELATIVE_Y(nDrawY + m_rcMapView.top + (nRectY - m_ptLeftTopPos.y) - 3));
						}
					}
				}
			}

			//	render self
			nRectX = GamePlayer::GetInstance()->GetCoordX() * 3;
			nRectY = GamePlayer::GetInstance()->GetCoordY() * 2;
			if(nRectX - m_ptLeftTopPos.x > 0 &&
				nRectX - m_ptLeftTopPos.x < nViewWidth &&
				nRectY - m_ptLeftTopPos.y > 0 &&
				nRectY - m_ptLeftTopPos.y < nViewHeight)
			{
				texPoint = pResMgr->GetTexture("minimap_self");
				if(texPoint)
				{
					m_pRender->SetTexture(texPoint);
					m_pRender->SetTextureRect(0, 0, 10, 10);
					m_pRender->Render(nSelfRenderX, nSelfRenderY);
				}
			}

			m_pRender->SetColor(dwPreCol);
		}

	if(m_nShowX != -1)
	{
		AfxGetPrinter()->SetColor(ARGB_GREEN);
		AfxGetPrinter()->Print(m_ptMouse.x, m_ptMouse.y - 15, "%d,%d", m_nShowX, m_nShowY);
	}

	RenderWindowTip();
}
//////////////////////////////////////////////////////////////////////////
void GameMapDlg::AutoLocate()
{
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_BMAP);
	if(NULL == pTexMgr)
	{
		return;
	}
	int nMapID = GamePlayer::GetInstance()->GetMapID();

	int nTexWidth = 0;
	int nTexHeight = 0;
	HTEXTURE tex = 0;
	tex = pTexMgr->GetTexture(nMapID);
	if(tex)
	{
		nTexHeight = pTexMgr->GetTextureHeight(nMapID);
		nTexWidth = pTexMgr->GetTextureWidth(nMapID);
		m_nMapWidth = nTexWidth;
		m_nMapHeight = nTexHeight;

		int nRectX = GamePlayer::GetInstance()->GetCoordX() * 3;
		int nRectY = GamePlayer::GetInstance()->GetCoordY() * 2;
		int nViewWidth = m_rcMapView.right - m_rcMapView.left;
		int nViewHeight = m_rcMapView.bottom - m_rcMapView.top;

		if(m_nMapWidth > nViewWidth)
		{
			m_ptLeftTopPos.x = nRectX - nViewWidth / 2;
			if(m_ptLeftTopPos.x < 0)
			{
				m_ptLeftTopPos.x = 0;
			}
			else if(m_ptLeftTopPos.x + nViewWidth > m_nMapWidth)
			{
				m_ptLeftTopPos.x = m_nMapWidth - nViewWidth;
			}
		}
		if(m_nMapHeight > nViewHeight)
		{
			m_ptLeftTopPos.y = nRectY - nViewHeight / 2;
			if(m_ptLeftTopPos.y < 0)
			{
				m_ptLeftTopPos.y = 0;
			}
			else if(m_ptLeftTopPos.y + nViewHeight > m_nMapHeight)
			{
				m_ptLeftTopPos.y = m_nMapHeight - nViewHeight;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool GameMapDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	if(m_nMapWidth == 0)
	{
		return true;
	}

	if(!pTheGame->GetShowMapSnap())
	{
		return true;
	}

	POINT ptTest;
	ptTest.x = _mp.x - m_rcClient.left;
	ptTest.y = _mp.y - m_rcClient.top;
	m_ptMouse = _mp;
	int nViewWidth = m_rcMapView.right - m_rcMapView.left;
	int nViewHeight = m_rcMapView.bottom - m_rcMapView.top;

	if(PtInRect(&m_rcMapView, ptTest))
	{
		ptTest.x -= 10;
		ptTest.y -= 75;
		if(AfxGetHge()->Input_KeyDown(HGEK_RBUTTON))
		{
			if(!m_bRightDown)
			{
				m_bRightDown = true;
				m_ptRightPos = _mp;
			}
		}
		else if(AfxGetHge()->Input_KeyUp(HGEK_RBUTTON))
		{
			if(m_bRightDown)
			{
				m_bRightDown = false;
				m_ptRightPos.x = m_ptRightPos.y = 0;
			}
		}

		m_nShowX = (m_ptLeftTopPos.x + ptTest.x) / 3;
		if(m_nMapWidth < nViewWidth)
		{
			m_nShowX = (m_ptLeftTopPos.x + ptTest.x - (nViewWidth - m_nMapWidth) / 2) / 3;
		}
		m_nShowY = (m_ptLeftTopPos.y + ptTest.y) / 2;
		if(m_nMapHeight < nViewHeight)
		{
			m_nShowY = (m_ptLeftTopPos.y + ptTest.y - (nViewHeight - m_nMapHeight) / 2) / 2;
		}

		if(m_nShowX < 0 ||
			m_nShowY < 0 ||
			m_nShowX >= m_nMapWidth / 3 ||
			m_nShowY >= m_nMapHeight / 2)
		{
			m_nShowX = m_nShowY = -1;
		}
	}
	else
	{
		m_nShowX = m_nShowY = -1;
	}

	if(m_bRightDown)
	{
		int nOftX = _mp.x - m_ptRightPos.x;
		int nOftY = _mp.y - m_ptRightPos.y;

		int nViewWidth = m_rcMapView.right - m_rcMapView.left;
		int nViewHeight = m_rcMapView.bottom - m_rcMapView.top;

		if(nViewWidth < m_nMapWidth)
		{
			m_ptLeftTopPos.x -= nOftX;
			if(m_ptLeftTopPos.x < 0)
			{
				m_ptLeftTopPos.x = 0;
			}
			else if(m_ptLeftTopPos.x + nViewWidth > m_nMapWidth)
			{
				m_ptLeftTopPos.x = m_nMapWidth - nViewWidth;
			}
		}
		if(nViewHeight < m_nMapHeight)
		{
			m_ptLeftTopPos.y -= nOftY;
			if(m_ptLeftTopPos.y < 0)
			{
				m_ptLeftTopPos.y = 0;
			}
			else if(m_ptLeftTopPos.y + nViewHeight > m_nMapHeight)
			{
				m_ptLeftTopPos.y = m_nMapHeight - nViewHeight;
			}
		}
		m_ptRightPos = _mp;
	}

	if(m_nShowX != -1 &&
		m_nShowY != -1 &&
		AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		bool bHaveTransRing = false;
#ifdef _DEBUG
		bHaveTransRing = true;
#else
		ItemAttrib* pItem = GamePlayer::GetInstance()->GetPlayerItem(PLAYER_ITEM_RING1);
		if(pItem &&
			pItem->id == 172)
		{
			bHaveTransRing = true;
		}
		pItem = GamePlayer::GetInstance()->GetPlayerItem(PLAYER_ITEM_RING2);
		if(pItem &&
			pItem->id == 172)
		{
			bHaveTransRing = true;
		}
#endif

		TileMap* pMap = GameMapManager::GetInstance()->GetMapData();
		if(pMap &&
			bHaveTransRing)
		{
			MapInfo info;
			pMap->GetMapInfo(info);

			if(m_nShowX >= 0 &&
				m_nShowX < info.nCol &&
				m_nShowY >= 0 &&
				m_nShowY < info.nRow)
			{
				if(GameMapManager::GetInstance()->CanThrough(m_nShowX, m_nShowY))
				{
#ifdef _DEBUG
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_GMOVE;
					req.dwParam = MAKELONG(m_nShowX, m_nShowY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
#else
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_MOVE;
					req.dwParam = MAKELONG(m_nShowX, m_nShowY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
#endif
				}
				else
				{
					GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "该坐标无法传送");
				}
			}
		}
		else if(!bHaveTransRing)
		{
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "请佩戴传送戒指");
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
void GameMapDlg::ResetStatus()
{
	//m_ptLeftTopPos.x = m_ptLeftTopPos.y = 0;
	m_bRightDown = false;
	m_nShowX = m_nShowY = -1;
}