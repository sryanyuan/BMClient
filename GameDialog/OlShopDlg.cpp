#include "OlShopDlg.h"
#include "../GameScene/GameInfoManager.h"
#include "../GameScene/GameResourceManager.h"
#include "GameDlgBase.h"
#include "GameDisplayDlg.h"
#include "GameInfoBoardDlg.h"
#include "../BackMir/BackMir.h"
#include "GameInputDlg.h"
#include "../Common/gfx_utils.h"
#include "../BackMir/BMDonateWnd.h"
//////////////////////////////////////////////////////////////////////////
#define BID_SELLITEM	0
#define BID_CHECKSOLD	3
#define BID_LEFT		1
#define BID_RIGHT		2
#define BID_TAKEBACK	4
#define BID_DONATE		5

#define START_POS_X		(10+15)
#define START_POS_Y		(77+10)
#define ITEMS_PER_PAGE	4
#define ITEMS_PER_PAGE_NEW	8

#define MONEY_LABEL_X	62
#define MONEY_LABEL_Y	404

#define DONATE_BUTTON_X (MONEY_LABEL_X + 250)
#define DONATE_BUTTON_Y (MONEY_LABEL_Y - 4)
//////////////////////////////////////////////////////////////////////////
OlShopDlg::OlShopDlg()
{
	m_nCurPage = 0;
	m_nShowItemType = ITEM_NO;
	m_rcClient.right = 540;
	m_rcClient.bottom = 420 + 30;

	SetWindowTitle("商    城");
	int nButtonWidth = RECT_WIDTH(m_rcClient) - 30 * 2;
	int nButtonEachWidth = nButtonWidth / 3;
	int nButtonLeftOffset = (nButtonEachWidth - 64) / 2;
	//AddCommonButton(CBTN_NORMAL, BID_SELLITEM, 30 + 0 * nButtonEachWidth + nButtonLeftOffset, RECT_HEIGHT(m_rcClient) - 50, "寄售物品");
	//AddCommonButton(CBTN_NORMAL, BID_CHECKSOLD, 30 + 1 * nButtonEachWidth + nButtonLeftOffset, RECT_HEIGHT(m_rcClient) - 50, "查看已售");
	//AddCommonButton(CBTN_NORMAL, BID_TAKEBACK, 30 + 2 * nButtonEachWidth + nButtonLeftOffset, RECT_HEIGHT(m_rcClient) - 50, "一键收回");

	AddCommonButton(CBTN_LEFTARROW, BID_LEFT, 30 - 10 + 200, RECT_HEIGHT(m_rcClient) - 45 - 30, "");
	AddCommonButton(CBTN_RIGHTARROW, BID_RIGHT, RECT_WIDTH(m_rcClient) - 30 - 200, RECT_HEIGHT(m_rcClient) - 45 - 30, "");
	AddCommonButton(CBTN_NORMAL, BID_DONATE, DONATE_BUTTON_X, DONATE_BUTTON_Y, "捐赠");

	if (pTheGame->GetGameMode() != GM_LOGIN) {
		GetCommonButtonData(BID_DONATE)->bVisible = false;
	}

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);

	//SetVisible(false);

	SetBkStype(1);
	CalcCloseButtonPosition();
	CenterWindow();
	Move(0, -50);

	SetVisible(false);
}

OlShopDlg::~OlShopDlg()
{

}

bool OlShopDlg::OnShowDialog()
{
	m_nCurPage = 0;
	m_xOlShopItemsRect.clear();

	PkgPlayerGetOlShopListReq req;
	req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(g_xBuffer);

	return true;
}

void OlShopDlg::AddItem(int _nItemID)
{
	
}

void OlShopDlg::AddItems(const int* _pItemIDArray, int _nCount)
{
	m_xOlShopItems.clear();
	m_xOlShopItems.resize(_nCount);

	for(int i = 0; i < _nCount; ++i)
	{
		m_xOlShopItems[i].stAttrib.type = ITEM_NO;
		GameInfoManager::GetInstance()->GetItemAttrib(_pItemIDArray[i], &m_xOlShopItems[i].stAttrib);
	}
}

bool OlShopDlg::OnCommonButtonClick(int _id)
{
	int nMaxPage = (m_xOlShopItems.size() + ITEMS_PER_PAGE_NEW - 1) / ITEMS_PER_PAGE_NEW;

	if(BID_LEFT == _id)
	{
		if(m_nCurPage > 0)
		{
			--m_nCurPage;
			m_xOlShopItemsRect.clear();
		}
	}
	else if(BID_RIGHT == _id)
	{
		if(m_nCurPage < nMaxPage - 1)
		{
			++m_nCurPage;
			m_xOlShopItemsRect.clear();
		}
	}
	else if(BID_SELLITEM == _id)
	{
		
	}
	else if(BID_CHECKSOLD == _id)
	{
		PkgPlayerOffCheckSoldReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(g_xBuffer);
	}
	else if(BID_TAKEBACK == _id)
	{
		PkgPlayerOffTakeBackReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(g_xBuffer);
	} else if (BID_DONATE == _id) {
		pTheGame->GetDonateValueWnd()->ShowWindow(true);
		pTheGame->GetDonateValueWnd()->CenterWindow();
	}

	return true;
}

bool OlShopDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	for(int i = 0; i < m_xOlShopItemsRect.size(); ++i)
	{
		OlShopItemRect& refItemRect = m_xOlShopItemsRect[i];

		POINT ptRelative = {_mp.x - m_rcClient.left, _mp.y - m_rcClient.top};

		if(PtInRect(&refItemRect.rcItem, ptRelative))
		{
			OlShopItem* pSellItem = NULL;
			if(refItemRect.nSellItemIndex >= 0 &&
				refItemRect.nSellItemIndex < m_xOlShopItems.size())
			{
				pSellItem = &m_xOlShopItems[refItemRect.nSellItemIndex];
			}
			else
			{
				break;
			}

			//GameMainOptUI::GetInstance()->GetIntroDlg()->ShowItemAttrib(&pSellItem->stAttrib);
			GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(&pSellItem->stAttrib);

			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				//	show buy dialog
				int nPrice = pSellItem->stAttrib.price;
				if(pTheGame->GetGameMode() == GM_NORMAL)
				{
					nPrice *= 10000;
				}

				if(GamePlayer::GetInstance()->GetPlayerBag()->GetMoney() >= nPrice)
				{
					GameMainOptUI::GetInstance()->GetInputDlg()->ShowAsMode(InputDlg_QueryOlShopBuy, (int)pSellItem->stAttrib.name, nPrice, pSellItem->stAttrib.id);
				}
				else
				{
					GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "您没有足够的金钱");
				}
			}

			break;
		}
	}

	return true;
}

void OlShopDlg::Render()
{
	__super::Render();

	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	GameTextureManager* pItemsMgr = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	HTEXTURE tex = 0;

	bool bNeedUpdateRect = false;
	if(m_xOlShopItemsRect.empty())
	{
		bNeedUpdateRect = true;
	}

	int nSize = m_xOlShopItems.size();
	int nCol = 0;
	int nRow = 0;
	
	for(int i = m_nCurPage * ITEMS_PER_PAGE_NEW; i < m_nCurPage * ITEMS_PER_PAGE_NEW + ITEMS_PER_PAGE_NEW; ++i)
	{
		if(i >= nSize)
		{
			break;
		}

		OlShopItem& refItem = m_xOlShopItems[i];
		int nPageIndex = i - m_nCurPage * ITEMS_PER_PAGE_NEW;

		nCol = nPageIndex % 2;
		nRow = nPageIndex / 2;

		if(refItem.stAttrib.type != ITEM_NO)
		{
			int nRenderX = RELATIVE_X(START_POS_X);
			if(nCol == 1)
			{
				nRenderX += 250;
			}
			int nRenderY = RELATIVE_Y(START_POS_Y) + nPageIndex * 70;
			nRenderY = RELATIVE_Y(START_POS_Y) + nRow * 70;

			tex = pPngMgr->GetTexture(11);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, 240, 69);
				m_pRender->Render(nRenderX, nRenderY);
			}

			tex = pItemsMgr->GetTexture(refItem.stAttrib.tex);
			int nItemX = nRenderX + 18 + (34 - pItemsMgr->GetTextureWidth(refItem.stAttrib.tex)) / 2;
			int nItemY = nRenderY + 19 + (34 - pItemsMgr->GetTextureHeight(refItem.stAttrib.tex)) / 2;
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0,
					pItemsMgr->GetTextureWidth(refItem.stAttrib.tex),
					pItemsMgr->GetTextureHeight(refItem.stAttrib.tex));
				m_pRender->Render(nItemX, nItemY);
			}

			if(bNeedUpdateRect)
			{
				OlShopItemRect rc;
				//rc.pItem = &refItem;
				rc.nSellItemIndex = i;
				rc.rcItem.left = nRenderX + 18 - m_rcClient.left;
				rc.rcItem.right = rc.rcItem.left + 34;
				rc.rcItem.top = nRenderY + 19 - m_rcClient.top;
				rc.rcItem.bottom = rc.rcItem.top + 34;
				m_xOlShopItemsRect.push_back(rc);
			}

			int nSellerX = nRenderX + 70;
			int nSellerY = nRenderY + 18;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			AfxGetPrinter()->Print(nSellerX, nSellerY, "商品： %s", refItem.stAttrib.name);

			int nMoneyX = nRenderX + 70;
			int nMoneyY = nRenderY + 40;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			if(pTheGame->GetGameMode() == GM_LOGIN)
			{
				AfxGetPrinter()->Print(nMoneyX, nMoneyY, "售价(贡献点)：%d 贡献点", refItem.stAttrib.price);
			}
			else
			{
				//AfxGetPrinter()->Print(nMoneyX, nMoneyY, "出售金额：%d 金币", refItem.stAttrib.price*100000);
				AfxGetPrinter()->Print(nMoneyX, nMoneyY, "售价(金币)：");
				Gfx_PrintNumberWithComma(AfxGetPrinter(), nMoneyX + 6 * 12, nMoneyY, refItem.stAttrib.price * 10000);
			}
		}
	}

	//	render page
	char szPageText[20];
	int nItemsCount = m_xOlShopItems.size();
	sprintf(szPageText, "%d/%d", m_nCurPage + 1, (nItemsCount + ITEMS_PER_PAGE_NEW - 1) / ITEMS_PER_PAGE_NEW);
	int nTextWidth = GetTextWidth(szPageText, 12);
	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->PrintWithoutStroke(m_rcClient.left + RECT_WIDTH(m_rcClient) / 2 - nTextWidth / 2, RELATIVE_Y(RECT_HEIGHT(m_rcClient) - 45 - 30 + 2), szPageText);

	//	render money
	const char* pszMoneyLabel = "金币：";
	if(pTheGame->GetGameMode() == GM_LOGIN)
	{
		pszMoneyLabel = "贡献点：";
	}

	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(MONEY_LABEL_X), RELATIVE_Y(MONEY_LABEL_Y + 5), pszMoneyLabel);

	tex = pPngMgr->GetTexture(10);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 181, 32);
		m_pRender->Render(RELATIVE_X(MONEY_LABEL_X + 60), RELATIVE_Y(MONEY_LABEL_Y - 5));

		int nMoney = GamePlayer::GetInstance()->GetPlayerBag()->GetMoney();
		if(pTheGame->GetGameMode() == GM_LOGIN)
		{
			nMoney = GamePlayer::GetInstance()->GetDonateLeft();
		}
		AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(MONEY_LABEL_X + 60 + 30), RELATIVE_Y(MONEY_LABEL_Y + 5), "%d", nMoney);
	}
}
