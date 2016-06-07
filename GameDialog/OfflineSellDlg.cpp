#include "OfflineSellDlg.h"
#include "../GameScene/GameInfoManager.h"
#include "../GameScene/GameResourceManager.h"
#include "GameDlgBase.h"
#include "GameDisplayDlg.h"
#include "GameInfoBoardDlg.h"
#include "../BackMir/BackMir.h"
#include "GameInputDlg.h"
#include "../Common/gfx_utils.h"
//////////////////////////////////////////////////////////////////////////
#define BID_SELLITEM	0
#define BID_CHECKSOLD	3
#define BID_LEFT		1
#define BID_RIGHT		2
#define BID_TAKEBACK	4

#define START_POS_X		(10+15)
#define START_POS_Y		(77+10)
#define ITEMS_PER_PAGE	4
#define ITEMS_PER_PAGE_NEW	8

#define FILTER_ALL		0
#define FILTER_MY		1
#define FILTER_WEAPON	2
#define FILTER_CLOTH	3
#define FILTER_HELMET	4
#define FILTER_NECKLACE	5
#define FILTER_BRACELAT 6
#define FILTER_RING		7
#define FILTER_MEDAL	8
#define FILTER_BELT		9
#define FILTER_SHOE		10
#define FILTER_GEM		11
#define FILTER_CHARM	12
#define FILTER_BOOK		13
#define FILTER_OTHER	14

const char* GetFilterName(int _nFilter)
{
	static const char* s_szFilterNames[] =
	{
		"全部商品",
		"我的商品",
		"武器",
		"服装",
		"头盔",
		"项链",
		"手镯",
		"戒指",
		"勋章",
		"腰带",
		"鞋子",
		"宝石",
		"符咒",
		"书籍",
		"其它"
	};

	if(_nFilter < 0 ||
		_nFilter >= sizeof(s_szFilterNames) / sizeof(s_szFilterNames[0]))
	{
		return "未知";
	}
	return s_szFilterNames[_nFilter];
}
//////////////////////////////////////////////////////////////////////////
OfflineSellListDlg::OfflineSellListDlg()
{
	m_pFilterDlg = NULL;
	m_pFilterDlg = new OfflineSellFilterDlg;
	m_pFilterDlg->SetListDlg(this);

	m_nCurPage = 0;
	m_nShowItemType = ITEM_NO;
	m_rcClient.right = 540;
	m_rcClient.bottom = 420 + 30;
	m_nFilterType = FILTER_ALL;

	SetWindowTitle("托管商铺");
	int nButtonWidth = RECT_WIDTH(m_rcClient) - 30 * 2;
	int nButtonEachWidth = nButtonWidth / 3;
	int nButtonLeftOffset = (nButtonEachWidth - 64) / 2;
	AddCommonButton(CBTN_NORMAL, BID_SELLITEM, 30 + 0 * nButtonEachWidth + nButtonLeftOffset, RECT_HEIGHT(m_rcClient) - 50, "寄售物品");
	AddCommonButton(CBTN_NORMAL, BID_CHECKSOLD, 30 + 1 * nButtonEachWidth + nButtonLeftOffset, RECT_HEIGHT(m_rcClient) - 50, "查看已售");
	AddCommonButton(CBTN_NORMAL, BID_TAKEBACK, 30 + 2 * nButtonEachWidth + nButtonLeftOffset, RECT_HEIGHT(m_rcClient) - 50, "一键收回");

	AddCommonButton(CBTN_LEFTARROW, BID_LEFT, 30 - 10 + 200, RECT_HEIGHT(m_rcClient) - 45 - 30, "");
	AddCommonButton(CBTN_RIGHTARROW, BID_RIGHT, RECT_WIDTH(m_rcClient) - 30 - 200, RECT_HEIGHT(m_rcClient) - 45 - 30, "");

	SetHotKey(HGEK_S);
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);

	SetVisible(false);

	//	test
	/*SellItem item;
	strcpy(item.szSellerName, "fell");

	for(int i = 0; i < 10; ++i)
	{
		GameInfoManager::GetInstance()->GetItemAttrib(3 + i, &item.stAttrib);
		m_xSellItems.push_back(item);
	}*/
	SetBkStype(1);
	CalcCloseButtonPosition();
	CenterWindow();

#ifdef _DEBUG
	SellItem item;
	item.nGold = 999;
	item.nItemID = 3;
	item.nUID = 3;
	strcpy(item.szSellerName, "ff");
	GameInfoManager::GetInstance()->GetItemAttrib(3, &item.stAttrib);

	for(int i = 0; i < 20; ++i)
	{
		//AddItem(item);
	}
#endif
}

OfflineSellListDlg::~OfflineSellListDlg()
{
	SAFE_DELETE(m_pFilterDlg);
}

bool OfflineSellListDlg::OnShowDialog()
{
	m_pFilterDlg->MoveTo(m_rcClient.left + 35, m_rcClient.top + 55);
	FilterItems(FILTER_ALL);
	m_pFilterDlg->SetFilterType(FILTER_ALL);

	m_nCurPage = 0;
	m_xSellItemsRect.clear();

	PkgPlayerOffGetListReq req;
	req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(g_xBuffer);

	return true;
}

bool OfflineSellListDlg::OnCommonButtonClick(int _id)
{
	int nMaxPage = (m_xSellItems.size() + ITEMS_PER_PAGE_NEW - 1) / ITEMS_PER_PAGE_NEW;

	if(BID_LEFT == _id)
	{
		if(m_nCurPage > 0)
		{
			--m_nCurPage;
			m_xSellItemsRect.clear();
		}
	}
	else if(BID_RIGHT == _id)
	{
		if(m_nCurPage < nMaxPage - 1)
		{
			++m_nCurPage;
			m_xSellItemsRect.clear();
		}
	}
	else if(BID_SELLITEM == _id)
	{
		GameMainOptUI::GetInstance()->GetOfflineSellDlg()->ShowDialog();
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
		/*PkgPlayerOffTakeBackReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(g_xBuffer);*/
		GameMainOptUI::GetInstance()->GetInputDlg()->ShowAsMode(InputDlg_QueryOffTackBack);
	}

	return true;
}

void OfflineSellListDlg::ResetItems()
{
	m_xSellItems.clear();
	m_xSellItemsRect.clear();
	m_xAllSellItems.clear();

	FilterItems(FILTER_ALL);
}

void OfflineSellListDlg::AddItems(const SellItemVector& _refItems)
{
	for(int i = 0; i < _refItems.size(); ++i)
	{
		m_xAllSellItems.push_back(_refItems[i]);
	}
	FilterItems(FILTER_ALL);
}

void OfflineSellListDlg::AddItem(const SellItem& _refItem)
{
	//m_xSellItems.push_back(_refItem);
	m_xAllSellItems.push_back(_refItem);
}

void OfflineSellListDlg::FilterItems(int _nFilterType)
{
	m_nCurPage = 0;
	m_xSellItemsRect.clear();
	m_xSellItems.clear();

	m_nFilterType = _nFilterType;
	m_pFilterDlg->SetFilterType(_nFilterType);
	m_pFilterDlg->PullDown(false);

	if(m_xAllSellItems.size() == 0)
	{
		return;
	}

	m_xSellItems.reserve(m_xAllSellItems.size());

	SellItemVector::const_iterator begIter = m_xAllSellItems.begin();
	SellItemVector::const_iterator endIter = m_xAllSellItems.end();
	for(begIter;
		begIter != endIter;
		++begIter)
	{
		bool bOk = false;
		const SellItem& refItem = *begIter;

		if(_nFilterType == FILTER_ALL)
		{
			bOk = true;
		}
		else if(_nFilterType == FILTER_MY)
		{
			char szName[20] = {0};
			GamePlayer::GetInstance()->GetHeroNameInMask(szName);
			if(strcmp(refItem.szSellerName, szName) == 0)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_WEAPON)
		{
			if(refItem.stAttrib.type == ITEM_WEAPON)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_CLOTH)
		{
			if(refItem.stAttrib.type == ITEM_CLOTH)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_HELMET)
		{
			if(refItem.stAttrib.type == ITEM_HELMET)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_NECKLACE)
		{
			if(refItem.stAttrib.type == ITEM_NECKLACE)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_BRACELAT)
		{
			if(refItem.stAttrib.type == ITEM_BRACELAT)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_RING)
		{
			if(refItem.stAttrib.type == ITEM_RING)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_MEDAL)
		{
			if(refItem.stAttrib.type == ITEM_MEDAL)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_BELT)
		{
			if(refItem.stAttrib.type == ITEM_BELT)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_SHOE)
		{
			if(refItem.stAttrib.type == ITEM_SHOE)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_GEM)
		{
			if(refItem.stAttrib.type == ITEM_GEM)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_CHARM)
		{
			if(refItem.stAttrib.type == ITEM_CHARM)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_BOOK)
		{
			if(refItem.stAttrib.type == ITEM_BOOK)
			{
				bOk = true;
			}
		}
		else if(_nFilterType == FILTER_OTHER)
		{
			if(!MirGame::IsEquipItem(refItem.stAttrib) &&
				refItem.stAttrib.type != ITEM_BOOK)
			{
				bOk = true;
			}
		}

		if(bOk)
		{
			m_xSellItems.push_back(refItem);
		}
	}
}

bool OfflineSellListDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	//	处理筛选窗口
	m_pFilterDlg->MoveTo(m_rcClient.left + 35, m_rcClient.top + 55);
	RECT rcFilterDlg = m_pFilterDlg->GetRect();
	m_pFilterDlg->ProcUserCmd(_mp);

	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		if(PtInRect(&rcFilterDlg, _mp))
		{
			return true;
		}
		else
		{
			if(m_pFilterDlg->IsPullDown())
			{
				m_pFilterDlg->PullDown(false);
				return true;
			}
		}
	}

	for(int i = 0; i < m_xSellItemsRect.size(); ++i)
	{
		SellItemRect& refItemRect = m_xSellItemsRect[i];

		POINT ptRelative = {_mp.x - m_rcClient.left, _mp.y - m_rcClient.top};

		if(PtInRect(&refItemRect.rcItem, ptRelative))
		{
			SellItem* pSellItem = NULL;
			if(refItemRect.nSellItemIndex >= 0 &&
				refItemRect.nSellItemIndex < m_xSellItems.size())
			{
				pSellItem = &m_xSellItems[refItemRect.nSellItemIndex];
			}
			else
			{
				break;
			}

			GameMainOptUI::GetInstance()->GetIntroDlg()->ShowItemAttrib(&pSellItem->stAttrib);
			GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(&pSellItem->stAttrib);

			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				//	show buy dialog
				if(GamePlayer::GetInstance()->GetPlayerBag()->GetMoney() >= pSellItem->nMoney)
				{
					GameMainOptUI::GetInstance()->GetInputDlg()->ShowAsMode(InputDlg_QueryOffBuy, (int)pSellItem->stAttrib.name, pSellItem->nMoney, pSellItem->nItemID);
				}
				else
				{
					if(GamePlayer::GetInstance()->GetUID() == pSellItem->nUID)
					{
						GameMainOptUI::GetInstance()->GetInputDlg()->ShowAsMode(InputDlg_QueryOffBuy, (int)pSellItem->stAttrib.name, pSellItem->nMoney, pSellItem->nItemID);
					}
					else
					{
						GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "您没有足够的金钱");
					}
				}
			}

			break;
		}
	}

	return true;
}

void OfflineSellListDlg::Render()
{
	__super::Render();

	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	GameTextureManager* pItemsMgr = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	HTEXTURE tex = 0;

	bool bNeedUpdateRect = false;
	if(m_xSellItemsRect.empty())
	{
		bNeedUpdateRect = true;
	}

	int nSize = m_xSellItems.size();
	int nCol = 0;
	int nRow = 0;
	
	for(int i = m_nCurPage * ITEMS_PER_PAGE_NEW; i < m_nCurPage * ITEMS_PER_PAGE_NEW + ITEMS_PER_PAGE_NEW; ++i)
	{
		if(i >= nSize)
		{
			break;
		}

		SellItem& refItem = m_xSellItems[i];
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
				SellItemRect rc;
				//rc.pItem = &refItem;
				rc.nSellItemIndex = i;
				rc.rcItem.left = nRenderX + 18 - m_rcClient.left;
				rc.rcItem.right = rc.rcItem.left + 34;
				rc.rcItem.top = nRenderY + 19 - m_rcClient.top;
				rc.rcItem.bottom = rc.rcItem.top + 34;
				m_xSellItemsRect.push_back(rc);
			}

			int nSellerX = nRenderX + 70;
			int nSellerY = nRenderY + 18;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			AfxGetPrinter()->Print(nSellerX, nSellerY, "售卖者： %s", refItem.szSellerName);

			int nMoneyX = nRenderX + 70;
			int nMoneyY = nRenderY + 40;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			//AfxGetPrinter()->Print(nMoneyX, nMoneyY, "出售金额： %d", refItem.nMoney);
			AfxGetPrinter()->Print(nMoneyX, nMoneyY, "出售金额：");
			Gfx_PrintNumberWithComma(AfxGetPrinter(), nMoneyX + 5 * 12, nMoneyY, refItem.nMoney);
		}
	}

	//	render page
	char szPageText[20];
	int nItemsCount = m_xSellItems.size();
	sprintf(szPageText, "%d/%d", m_nCurPage + 1, (nItemsCount + ITEMS_PER_PAGE_NEW - 1) / ITEMS_PER_PAGE_NEW);
	int nTextWidth = GetTextWidth(szPageText, 12);
	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->PrintWithoutStroke(m_rcClient.left + RECT_WIDTH(m_rcClient) / 2 - nTextWidth / 2, RELATIVE_Y(RECT_HEIGHT(m_rcClient) - 45 - 30 + 2), szPageText);

	//	渲染筛选窗口
	m_pFilterDlg->Render();
}








//////////////////////////////////////////////////////////////////////////
OfflineSellDlg::OfflineSellDlg()
{
	//	270*240
	ZeroMemory(&m_stSellItem, sizeof(ItemAttrib));

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_rcClient.left = (WINDOW_WIDTH - 300) / 2;
	m_rcClient.top = 150;
	m_rcClient.right = m_rcClient.left + 300;
	m_rcClient.bottom = m_rcClient.top + 240;
	SetBkStype(1);

	CalcCloseButtonPosition();
	SetVisible(false);

	SetWindowInfo("手续费:1000");
	SetWindowTitle("商铺寄售");

	m_pEdit = new MyGfxEdit(190, ARGB_WHITE);
	m_pEdit->SetRenderBorder(false);
	m_pEdit->SetOnCharInputCallback(&OfflineSellDlg::Callback_OnCharInput);

	AddCommonButton(CBTN_NORMAL, BID_SELLITEM, 95+15, 185+10, "寄售");
}

int __stdcall OfflineSellDlg::Callback_OnCharInput(char _cKey)
{
	if(_cKey >= '0' &&
		_cKey <= '9')
	{
		MyGfxEdit* pEdit = GameMainOptUI::GetInstance()->GetOfflineSellDlg()->m_pEdit;

		const char* pszInput = pEdit->GetCookie();
		char szInput[32] = {0};
		strcpy(szInput, pszInput);
		szInput[strlen(szInput)] = _cKey;

		int nMoney = atoi(szInput);
		if(nMoney > MAX_MONEY)
		{
			char szMaxMoney[32] = {0};
			itoa(MAX_MONEY, szMaxMoney, 10);
			pEdit->ClearCookie();
			pEdit->InsertCookie(szMaxMoney);
			return 1;
		}

		return 0;
	}

	return 1;
}

bool OfflineSellDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	m_pEdit->Updata();

	POINT ptRelative = {_mp.x - m_rcClient.left, _mp.y - m_rcClient.top};

	RECT rcItem;
	rcItem.left = START_POS_X + 47;
	rcItem.top = START_POS_Y + 15;
	rcItem.right = rcItem.left + 35;
	rcItem.bottom = rcItem.top + 35;

	if(PtInRect(&rcItem, ptRelative))
	{
		if(m_stSellItem.type != 0)
		{
			GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(&m_stSellItem);
		}

		GameBagDlg2* pBagDlg = GameMainOptUI::GetInstance()->GetBagDlg2();
		if(pBagDlg->IsItemSel())
		{
			ItemAttrib* pSelItem = pBagDlg->GetSelItem();
			if(pSelItem)
			{
				if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
				{
					if(TEST_FLAG_BOOL(pSelItem->atkPois, POIS_MASK_BIND))
					{
						GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "无法寄售绑定物品");
						return true;
					}
					if(GamePlayer::GetInstance()->GetPlayerBag()->GetMoney() < 1000)
					{
						GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "没有足够的金钱支付手续费");
						return true;
					}
					memcpy(&m_stSellItem, pSelItem, sizeof(ItemAttrib));
					pBagDlg->CancelSel();
				}
			}
		}
	}

	return true;
}

void OfflineSellDlg::Render()
{
	__super::Render();

	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	GameTextureManager* pItems = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);

	HTEXTURE tex = pPngMgr->GetTexture(13);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 251, 102);
		m_pRender->Render(RELATIVE_X(START_POS_X), RELATIVE_Y(START_POS_Y));
	}

	int nRenderClientX = RELATIVE_X(START_POS_X);
	int nRenderClientY = RELATIVE_Y(START_POS_Y);
	m_pEdit->Render(nRenderClientX + 72, nRenderClientY + 79);

	if(m_stSellItem.type != 0)
	{
		tex = pItems->GetTexture(m_stSellItem.tex);
		if(tex)
		{
			int nRenderX = nRenderClientX + 47 + (35 - pItems->GetTextureWidth(m_stSellItem.tex)) / 2;
			int nRenderY = nRenderClientY + 15 + (35 - pItems->GetTextureHeight(m_stSellItem.tex)) / 2;
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0,
				pItems->GetTextureWidth(m_stSellItem.tex),
				pItems->GetTextureHeight(m_stSellItem.tex));
			m_pRender->Render(nRenderX, nRenderY);
		}
	}
}

bool OfflineSellDlg::OnCommonButtonClick(int _id)
{
	if(_id == BID_SELLITEM)
	{
		int nMoney = atoi(m_pEdit->GetCookie());
		if(nMoney <= 0)
		{
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "请确认输入金额");
			return true;
		}

		PkgPlayerOffSellItemReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		req.dwMoney = nMoney;
		req.dwTag = m_stSellItem.tag;
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(g_xBuffer);
	}

	return true;
}

bool OfflineSellDlg::OnShowDialog()
{
	ZeroMemory(&m_stSellItem, sizeof(ItemAttrib));
	m_pEdit->ClearCookie();
	m_pEdit->SetFocus();
	SetNeedTopLevel(true);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
	#define FILTER_ALL		0
	#define FILTER_MY		1
	#define FILTER_WEAPON	2
	#define FILTER_CLOTH	3
	#define FILTER_HELMET	4
	#define FILTER_NECKLACE	5
	#define FILTER_BRACELAT 6
	#define FILTER_RING		7
	#define FILTER_MEDAL	8
	#define FILTER_BELT		9
	#define FILTER_SHOE		10
	#define FILTER_GEM		11
	#define FILTER_CHARM	12
	#define FILTER_OTHER	13
	*/

#define BID_FILTER_PULLDOWN		99
#define BID_FILTER_ALL			0
#define BID_FILTER_MY		1
#define BID_FILTER_WEAPON	2
#define BID_FILTER_CLOTH	3
#define BID_FILTER_HELMET	4
#define BID_FILTER_NECKLACE	5
#define BID_FILTER_BRACELAT 6
#define BID_FILTER_RING		7
#define BID_FILTER_MEDAL	8
#define BID_FILTER_BELT		9
#define BID_FILTER_SHOE		10
#define BID_FILTER_GEM		11
#define BID_FILTER_CHARM	12
#define BID_FILTER_BOOK		13
#define BID_FILTER_OTHER	14

OfflineSellFilterDlg::OfflineSellFilterDlg()
{
	m_nFilterType = FILTER_ALL;
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_bPoolDown = false;
	m_rcClient.right = 76;
	m_rcClient.bottom = 20;
	m_pListDlg = NULL;
	SetCustomBk();

	AddCommonButton(CBTN_NORMAL, BID_FILTER_PULLDOWN, 0, 0, GetFilterName(FILTER_ALL), 38);
	for(int i = BID_FILTER_ALL; i <= BID_FILTER_OTHER; ++i)
	{
		AddCommonButton(CBTN_NORMAL, i, 0, 20 * (i + 1), GetFilterName(i), 38);
		GetCommonButtonData(i)->bVisible = false;
	}
}

OfflineSellFilterDlg::~OfflineSellFilterDlg()
{
	SAFE_DELETE(m_pRender);
}

bool OfflineSellFilterDlg::OnCommonButtonClick(int _id)
{
	if(_id == BID_FILTER_PULLDOWN)
	{
		bool bPull = !m_bPoolDown;
		PullDown(bPull);
	}
	else if(_id >= BID_FILTER_ALL &&
		_id <= BID_FILTER_OTHER)
	{
		m_nFilterType = _id;
		strcpy(GetCommonButtonData(BID_FILTER_PULLDOWN)->szText, GetFilterName(m_nFilterType));
		m_pListDlg->FilterItems(_id);
	}

	return true;
}

void OfflineSellFilterDlg::SetFilterType(int _nType)
{
	m_nFilterType = _nType;
	strcpy(GetCommonButtonData(BID_FILTER_PULLDOWN)->szText, GetFilterName(m_nFilterType));
}

void OfflineSellFilterDlg::Render()
{
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	HTEXTURE tex = 0;

	tex = pPngMgr->GetTexture(37);
	if(tex)
	{
		int nHeight = 20 + 20 * 15;
		if(!m_bPoolDown)
		{
			nHeight = 20;
		}
		/*m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 76, nHeight);
		m_pRender->Render(RELATIVE_X(0), RELATIVE_Y(0));*/

		RECT rcScale9 = {6, 6, 6, 6};
		RECT rcTex = {0, 0, 76, 20};
		Gfx_SetRenderState(kGfx_9Path_CenterStretch, 1);
		Gfx_Render9Path(m_pRender, tex, &rcTex, &rcScale9, RELATIVE_X(0), RELATIVE_Y(0), 76, nHeight);
	}

	__super::Render();
}

void OfflineSellFilterDlg::PullDown(bool _bPull)
{
	if(_bPull == m_bPoolDown)
	{
		return;
	}
	m_bPoolDown = _bPull;

	for(int i = BID_FILTER_ALL; i <= BID_FILTER_OTHER; ++i)
	{
		GetCommonButtonData(i)->bVisible = m_bPoolDown;
	}

	if(!m_bPoolDown)
	{
		m_rcClient.bottom = m_rcClient.top + 20;
	}
	else
	{
		m_rcClient.bottom = m_rcClient.top + 20 + 20 * 15;
	}
}