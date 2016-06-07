#include "GameShopDlg2.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameScene/GameNPC.h"
#include "../GameDialog/GameInputDlg.h"
#include "GameDisplayDlg.h"
#include "../Common/gfx_utils.h"
//////////////////////////////////////////////////////////////////////////
#define DLG_WIDTH	275
#define DLG_HEIGHT	370

const int s_nLeftOft = 14;
const int s_nTopOft = 76;
//////////////////////////////////////////////////////////////////////////
GameShopDlg2::GameShopDlg2()
{
	m_rcClient.left = WINDOW_WIDTH - DLG_WIDTH;
	m_rcClient.right = m_rcClient.left + DLG_WIDTH;
	m_rcClient.top = 20;
	m_rcClient.bottom = m_rcClient.top + DLG_HEIGHT;
	m_nPrice = 0;
	m_nHandleID = 0;
	m_nAttribID = 0;
	ClearItem();

	SetBkStype(1);
	CalcCloseButtonPosition();
	m_pRender = NEW_EMPTY_SPRITE;
	SetWindowTitle("商店");
	SetVisible(false);
}

GameShopDlg2::~GameShopDlg2()
{
	SAFE_DELETE(m_pRender);
}


void GameShopDlg2::Render()
{
	__super::Render();

	HTEXTURE tex = 0;
	hgeResourceManager* pResMgr = pTheGame->GetGameRes();
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	tex = pResMgr->GetTexture("shopitems");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 246, 247);
		m_pRender->Render(RELATIVE_X(s_nLeftOft), RELATIVE_Y(s_nTopOft));
	}

	tex = pPngMgr->GetTexture(21);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 170, 30);
		m_pRender->Render(RELATIVE_X(50), RELATIVE_Y(DLG_HEIGHT - 40));
	}

	const int s_nRx = RELATIVE_X(124);
	const int s_nRy = RELATIVE_Y(DLG_HEIGHT - 30);
	AfxGetPrinter()->SetColor(ARGB_WHITE);
	//AfxGetPrinter()->PrintWithoutStroke(s_nRx, s_nRy, "%d", m_nPrice);
	Gfx_PrintNumberWithComma(AfxGetPrinter(), s_nRx, s_nRy, m_nPrice);

	int nItemIndex = 0;
	ItemAttrib* pItem = NULL;
	int nRenderX = 0;
	int nRenderY = 0;
	int nTexWidth = 0;
	int nTexHeight = 0;
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);

	for(int i = 0; i < 36; ++i)
	{
		pItem = &m_stItems[i];
		if(pItem->type != ITEM_NO)
		{
			nRenderX = i % 6 * 41 + 4 + s_nLeftOft;
			nRenderY = i / 6 * 41 + 4 + s_nTopOft;
			tex = pTexMgr->GetTexture(pItem->tex);
			if(tex)
			{
				nTexWidth = pTexMgr->GetTextureWidth(pItem->tex);
				nTexHeight = pTexMgr->GetTextureHeight(pItem->tex);

				nRenderX += (34 / 2 - nTexWidth / 2);
				nRenderY += (34 / 2 - nTexHeight / 2);

				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0,
					nTexWidth, nTexHeight);
				m_pRender->Render(RELATIVE_X(nRenderX),
					RELATIVE_Y(nRenderY));
			}
		}
	}
}

bool GameShopDlg2::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	m_nPrice = 0;

	POINT ptTest = _mp;
	ptTest.x -= m_rcClient.left;
	ptTest.y -= m_rcClient.top;
	ptTest.x -= s_nLeftOft;
	ptTest.y -= s_nTopOft;
	bool bInItem = false;
	GameBagDlg2* pBagDlg2 = GameMainOptUI::GetInstance()->GetBagDlg2();

	if(ptTest.x >= 0 &&
		ptTest.x <= 246 &&
		ptTest.y >= 0 &&
		ptTest.y <= 246)
	{
		int nCol = -1;
		int nRow = -1;

		nCol = ptTest.x / 41;
		nRow = ptTest.y / 41;

		if(nCol >= 0 &&
			nCol < 6 &&
			nRow >= 0 &&
			nRow < 6)
		{
			if(pBagDlg2->IsItemSel())
			{
				m_nPrice = pBagDlg2->GetSelItem()->price / SELL_ITEM_MULTI;

				if(pBagDlg2->GetSelItem()->type == ITEM_COST)
				{
					m_nPrice *= pBagDlg2->GetSelItem()->atkSpeed;
				}
				else if(pTheGame->IsEquipItem(*pBagDlg2->GetSelItem()))
				{
					WORD wLevel = pBagDlg2->GetSelItem()->level;

					if(wLevel != 0)
					{
						int nValue = GetItemUpgrade(wLevel);
						if(nValue > 0 &&
							nValue <= 8)
						{
							//	recalc price
							m_nPrice *= (float)(1.0f + (float)((float)nValue / 4));
						}
					}
				}

				if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
				{
					PkgPlayerShopOpReq req;
					req.bOp = SHOP_SELL;
					req.dwData = pBagDlg2->GetSelItem()->tag;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
					pBagDlg2->CancelSel();
				}
			}

			int nColLeft = ptTest.x % 41;
			int nRowLeft = ptTest.y % 41;

			if(nColLeft >= 4 &&
				nColLeft <= 38 &&
				nRowLeft >= 4 &&
				nRowLeft <= 38)
			{
				OnItem(nCol + nRow * 6);
				bInItem = true;
			}
		}
	}

	if(!bInItem)
	{
		GameMainOptUI::GetInstance()->GetIntroDlg()->ShowNothing();
	}

	return true;
}

void GameShopDlg2::OnItem(int _idx)
{
	if(_idx < 0 ||
		_idx >= 36)
	{
		return;
	}

	GameBagDlg2* pBagDlg2 = GameMainOptUI::GetInstance()->GetBagDlg2();
	GameIntroDlg* pIntroDlg = GameMainOptUI::GetInstance()->GetIntroDlg();
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nSelIndex = _idx;

	/*if(pBagDlg2->IsItemSel())
	{
		m_nPrice = pBagDlg2->GetSelItem()->price / 2;

		if(pBagDlg2->GetSelItem()->type == ITEM_COST)
		{
			m_nPrice *= pBagDlg2->GetSelItem()->atkSpeed;
		}

		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			PkgPlayerShopOpReq req;
			req.bOp = SHOP_SELL;
			req.dwData = pBagDlg2->GetSelItem()->tag;
			req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBuffer(&g_xBuffer);
			pBagDlg2->CancelSel();
		}
	}
	else*/
	{
		bool bHaveItem = false;

		if(m_stItems[nSelIndex].type != ITEM_NO)
		{
			pIntroDlg->ShowItemAttrib(&m_stItems[nSelIndex]);
			GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(&m_stItems[nSelIndex]);
			bHaveItem = true;
		}
		else
		{
			pIntroDlg->ShowNothing();
		}

		if(bHaveItem)
		{
			m_nPrice = m_stItems[nSelIndex].price;

			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				bool bEnhancedBuyMode = true;

				if(bEnhancedBuyMode)
				{
					int nTotalPrice = m_stItems[nSelIndex].price;

					if(m_stItems[nSelIndex].type == ITEM_COST)
					{
						GameMainOptUI::GetInstance()->GetIntroDlg()->ShowNothing();
						GameMainOptUI::GetInstance()->GetInputDlg()->ShowAsMode(InputDlg_QueryBuySum, m_nHandleID, m_stItems[nSelIndex].id, m_stItems[nSelIndex].price);
					}
					else
					{
						if(pPlayer->GetPlayerBag()->GetMoney() >= nTotalPrice)
						{
							PkgPlayerShopOpReq req;
							req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
							req.uTargetId = m_nHandleID;
							req.bOp = SHOP_BUY;
							req.dwData = m_stItems[nSelIndex].id;

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
				else
				{
					int nTotalPrice = m_stItems[nSelIndex].price;
					bool bBuyDozen = AfxGetHge()->Input_GetKeyState(HGEK_CTRL);

					if(bBuyDozen)
					{
						if(m_stItems[nSelIndex].type == ITEM_COST)
						{
							nTotalPrice *= GRID_MAX;
						}
						else
						{
							bBuyDozen = false;
						}
					}

					if(pPlayer->GetPlayerBag()->GetMoney() >= nTotalPrice)
					{
						PkgPlayerShopOpReq req;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
						req.uTargetId = m_nHandleID;
						req.bOp = SHOP_BUY;
						req.dwData = m_stItems[nSelIndex].id;
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
	}
}

void GameShopDlg2::CreateByID(GameNPC* _pNPC)
{
	if(_pNPC == NULL)
	{

	}
	else
	{
		m_nAttribID = _pNPC->GetAttrib()->id;
		m_nHandleID = _pNPC->GetHandlerID();
		ShowDialog();
	}
}

void GameShopDlg2::ClearItem()
{
	ZeroMemory(m_stItems, sizeof(m_stItems));
	m_nItemCounter = 0;
}

void GameShopDlg2::AddItem(int _id)
{
	if(m_nItemCounter >= 36)
	{
		return;
	}

	if(GameInfoManager::GetInstance()->GetItemAttrib(_id, &m_stItems[m_nItemCounter]))
	{
		++m_nItemCounter;
	}
	else
	{
		m_stItems[m_nItemCounter].type = ITEM_NO;
	}
}

bool GameShopDlg2::OnShowDialog()
{
	MoveTo(0, 0);
	return true;
}

bool GameShopDlg2::OnCloseDialog()
{
	GameBagDlg2* pBagDlg = GET_DLG(BagDlg2);
	if(pBagDlg->IsVisible())
	{
		pBagDlg->MoveTo(0, 0);
	}
	return true;
}