#include "GameDlgBase.h"
#include "../GameScene/GameInfoManager.h"
#include "../BackMir/BackMir.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameScene/GameResourceManager.h"
#include "../GameScene/GameNPC.h"
#include "GameDisplayDlg.h"
//////////////////////////////////////////////////////////////////////////
GameShopDlg::GameShopDlg()
{
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	ZeroMemory(m_xItems, sizeof(m_xItems));
	m_nItemCounter = 0;
	m_nPage = 0;
	m_rcClient.right = 257;
	m_rcClient.bottom = 354;
	m_bVisible = false;
	m_nPrice = 0;

	INIT_CLOSEBTN(232, 13);
}

GameShopDlg::~GameShopDlg()
{
	SAFE_DELETE(m_pRender);
}
//////////////////////////////////////////////////////////////////////////
void GameShopDlg::ClearItem()
{
	ZeroMemory(m_xItems, sizeof(m_xItems));
	m_nItemCounter = 0;
	m_nPage = 0;
	m_nPrice = 0;
}
//////////////////////////////////////////////////////////////////////////
void GameShopDlg::AddItem(int _id)
{
	if(m_nItemCounter >= MAX_SHOP_ITEM)
	{
		return;
	}

	if(GameInfoManager::GetInstance()->GetItemAttrib(_id, &m_xItems[m_nItemCounter]))
	{
		++m_nItemCounter;
	}
	else
	{
		m_xItems[m_nItemCounter].type = ITEM_NO;
	}
}
//////////////////////////////////////////////////////////////////////////
bool GameShopDlg::IsCaptionBar(int _x, int _y)
{
	if(_x >= RELATIVE_X(0) &&
		_x <= RELATIVE_X(257) &&
		_y >= RELATIVE_Y(9) &&
		_y <= RELATIVE_Y(39))
	{
		/*if(_x > RELATIVE_X(232))
		{
			//	关闭
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				SetVisible(!m_bVisible);
			}
			return false;
		}*/
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool GameShopDlg::ProcUserCmd(const POINT &_mp)
{
	static const POINT ptTopLeft = {20, 48};
	static const int nEachWidth = 31;
	static const int nEachHeight = 31;
	static const int nOffsetX = 6;
	static const int nOffsetY = 6;

	POINT ptMouse = _mp;
	POINT ptDetect = ptMouse;

	m_nPrice = 0;

	__super::ProcUserCmd(_mp);

	//	计算坐标
	ptDetect.x -= ptTopLeft.x;
	ptDetect.y -= ptTopLeft.y;
	ptDetect.x -= m_rcClient.left;
	ptDetect.y -= m_rcClient.top;

	int nActX = -1;
	int nActY = -1;
	nActX = ptDetect.x / (nEachWidth + nOffsetX);
	nActY = ptDetect.y / (nEachHeight + nOffsetY);
	bool bInItem = false;

	if(ptDetect.x >= 0 &&
		ptDetect.y >= 0)
	{
		if(nActX < 6 &&
			nActX >= 0)
		{
			//	小于六才在背包范围内
			if(nActY < 6 &&
				nActY >= 0)
			{
				//	在范围内 再判断是否在格子内
				if(m_pParent->GetBagDlg2()->IsItemSel())
				{
					ItemAttrib* pItem = m_pParent->GetBagDlg2()->GetSelItem();
					if(pItem->type != ITEM_NO)
					{
						//AfxGetPrinter()->SetColor(ARGB_WHITE);
						//AfxGetPrinter()->Print(RELATIVE_X(76), RELATIVE_Y(312), "%d", pItem->price);
						m_nPrice = pItem->price / 2;

						if(pItem->type == ITEM_COST &&
							pItem->atkSpeed != 1)
						{
							m_nPrice *= pItem->atkSpeed;
						}

						if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
						{
							PkgPlayerShopOpReq req;
							req.bOp = SHOP_SELL;
							req.dwData = pItem->tag;
							req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
							g_xBuffer.Reset();
							g_xBuffer << req;
							SendBufferToGS(&g_xBuffer);
							m_pParent->GetBagDlg2()->CancelSel();
						}
					}
				}
				else
				{
					if(ptDetect.x - nActX * (nEachWidth + nOffsetX) < nEachWidth &&
						ptDetect.y - nActY * (nEachHeight + nOffsetY) < nEachHeight)
					{
						OnItem(nActX, nActY);
						bInItem = true;
					}
				}
			}
		}
	}

	if(!bInItem)
	{
		m_pParent->GetIntroDlg()->SetVisible(false);
		m_pParent->GetIntroDlg()->ShowNothing();
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
void GameShopDlg::OnItem(int _x, int _y)
{
	int nSelIndex = m_nPage * (MAX_SHOP_ITEM / 2) + (_y * 6 + _x);
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	//if(!m_pParent->GetBagDlg()->IsItemSel())
	{
		if(nSelIndex >= MAX_SHOP_ITEM)
		{
			return;
		}
		bool bHaveItem = false;
		if(m_xItems[nSelIndex].type != ITEM_NO)
		{
			m_pParent->GetIntroDlg()->ShowItemAttrib(&m_xItems[nSelIndex]);
			m_pParent->GetDisplayDlg()->ShowItemAttrib(&m_xItems[nSelIndex]);
			bHaveItem = true;
		}
		else
		{
			m_pParent->GetIntroDlg()->ShowNothing();
		}

		if(bHaveItem)
		{
			//AfxGetPrinter()->SetColor(ARGB_WHITE);
			//AfxGetPrinter()->Print(RELATIVE_X(76), RELATIVE_Y(312), "%d", m_xItems[nSelIndex].price);
			m_nPrice = m_xItems[nSelIndex].price;

			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				int nTotalPrice = m_xItems[nSelIndex].price;
				bool bBuyDozen = AfxGetHge()->Input_GetKeyState(HGEK_CTRL);

				if(bBuyDozen)
				{
					if(m_xItems[nSelIndex].type == ITEM_COST)
					{
						nTotalPrice *= GRID_MAX;
					}
					else
					{
						bBuyDozen = false;
					}
				}

#ifdef _DEBUG
				nTotalPrice = 0;
#endif

				if(pPlayer->GetPlayerBag()->GetMoney() >= nTotalPrice)
				{
					PkgPlayerShopOpReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.uTargetId = m_nHandleID;
					req.bOp = SHOP_BUY;
					req.dwData = m_xItems[nSelIndex].id;
					if(bBuyDozen)
					{
						req.bNumber = GRID_MAX;
					}
					//req.dwData = nSelIndex;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
				else
				{
					GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "您没有足够的金币");
				}
			}
		}
	}
	// 	else
	// 	{
	// 		ItemAttrib* pItem = m_pParent->GetAssistItemSel();
	// 		if(pItem->type != ITEM_NO)
	// 		{
	// 			AfxGetPrinter()->SetColor(ARGB_WHITE);
	// 			AfxGetPrinter()->Print(RELATIVE_X(76), RELATIVE_Y(312), "%d", pItem->price);
	// 
	// 			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	// 			{
	// 				PkgPlayerShopOpReq req;
	// 				req.bOp = SHOP_SELL;
	// 				req.dwData = pItem->tag;
	// 				g_xBuffer.Reset();
	// 				g_xBuffer << req;
	// 				SendBuffer(&g_xBuffer);
	// 				m_pParent->GetBagDlg()->CancelSel();
	// 			}
	// 		}
	// 	}
}
//////////////////////////////////////////////////////////////////////////
void GameShopDlg::Render()
{
	static const POINT ptTopLeft = {20, 48};
	static const int nEachWidth = 31;
	static const int nEachHeight = 31;
	static const int nOffsetX = 6;
	static const int nOffsetY = 6;

	if(!m_bVisible ||
		!m_pRes)
	{
		return;
	}

	int nDrawOffsetX = 0;
	int nDrawOffsetY = 0;

	HTEXTURE tex = 0;
	tex = m_pRes->GetTexture("shop");
	if(tex)
	{
		INITSPRITE(m_pRender, tex);
		m_pRender->Render(m_rcClient.left, m_rcClient.top);
	}

	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->Print(RELATIVE_X(76), RELATIVE_Y(312), "%d", m_nPrice);

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	GameTextureManager* pRes = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	ItemAttrib* pItem = NULL;
	for(int i = 0; i < MAX_SHOP_NUM; ++i)
	{
		pItem = &m_xItems[i + MAX_SHOP_NUM * m_nPage];
		if(pItem->type != ITEM_NO)
		{
			tex = pRes->GetTexture(pItem->tex);
			// 			nDrawOffsetX = pRes->GetTextureWidth(tex);
			// 			nDrawOffsetY = pRes->GetTextureHeight(tex);
			nDrawOffsetX = pRes->GetTextureWidth(pItem->tex);
			nDrawOffsetY = pRes->GetTextureHeight(pItem->tex);
			nDrawOffsetX = abs(nDrawOffsetX - nEachWidth) / 2;
			nDrawOffsetY = abs(nDrawOffsetY - nEachHeight) / 2;
			//SETSPRITE(tex, m_pSpr);
			m_pRender->SetTexture(tex);
			//m_pSpr->SetTextureRect(0, 0, pRes->GetTextureWidth(tex), pRes->GetTextureHeight(tex));
			m_pRender->SetTextureRect(0, 0, pRes->GetTextureWidth(pItem->tex), pRes->GetTextureHeight(pItem->tex));
			m_pRender->Render((i % 6) * (nEachWidth + nOffsetX) + ptTopLeft.x + m_rcClient.left + nDrawOffsetX,
				(i / 6) * (nEachHeight + nOffsetY) + ptTopLeft.y + m_rcClient.top + nDrawOffsetY);
		}
	}

	AfxGetPrinter()->SetColor(ARGB_RED);
	AfxGetPrinter()->Print(RELATIVE_X(25), RELATIVE_Y(354 - 4 - 12), "提示:按住CTRL买消耗品时可以买一组");

	__super::Render();
}
//////////////////////////////////////////////////////////////////////////
void GameShopDlg::Update(float _dt)
{

}
//////////////////////////////////////////////////////////////////////////
void GameShopDlg::CreateByID(GameNPC* _pnpc)
{
	MoveTo(0, 0);
#ifdef _DEBUG
	if(_pnpc == NULL)
	{
		m_nHandleID = 0;
		SetVisible(true);
	}
	else
	{
		m_nAttribID = _pnpc->GetAttrib()->id;
		m_nHandleID = _pnpc->GetHandlerID();
		SetVisible(true);
	}
#else
	if(_pnpc == NULL)
	{
		
	}
	else
	{
		m_nAttribID = _pnpc->GetAttrib()->id;
		m_nHandleID = _pnpc->GetHandlerID();
		SetVisible(true);
	}
#endif
}