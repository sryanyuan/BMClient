#include "../GameScene/GameResourceManager.h"
#include "GameDlgBase.h"
#include "../GameScene/GameScene.h"
#include "GameIdentifyDlg.h"
#include "GameInfoBoardDlg.h"
#include "GameDisplayDlg.h"
#include "../BackMir/BackMir.h"
#include "../../CommonModule/HideAttribHelper.h"
//////////////////////////////////////////////////////////////////////////
const int s_nLeftOft = 108;
const int s_nTopOft = 100;
const int s_nDlgWidth = 260;
const int s_nDlgHeight = 240;
//////////////////////////////////////////////////////////////////////////
GameIdentifyDlg::GameIdentifyDlg()
{
	m_nDlgMode = -1;
	m_pItem = NULL;
	m_nPrice = 0;

	m_rcClient.right = s_nDlgWidth;
	m_rcClient.bottom = s_nDlgHeight;
	SetVisible(false);

	m_pRender = NEW_EMPTY_SPRITE;
	CalcCloseButtonPosition();

	AddCommonButton(CBTN_NORMAL, 0, s_nDlgWidth / 2 - 36, s_nTopOft + 63 + 20, "确定");
	SetDlgMode(GameIdentifyDlgMode_Identify);
}

GameIdentifyDlg::~GameIdentifyDlg()
{

}

void GameIdentifyDlg::SetDlgMode(int _nMode)
{
	if(m_nDlgMode == _nMode)
	{
		return;
	}

	m_nDlgMode = _nMode;
	if(GameIdentifyDlgMode_Identify == m_nDlgMode)
	{
		//	辨识
		SetWindowTitle("鉴定装备");
	}
	else if(GameIdentifyDlgMode_Unbind == m_nDlgMode)
	{
		//	解绑
		SetWindowTitle("解绑装备");
	}
}

void GameIdentifyDlg::Render()
{
	__super::Render();


	hgeResourceManager* pResMgr = GameScene::sThis->GetMainOpt()->GetResourceMgr();

	HTEXTURE tex = 0;
	tex = pResMgr->GetTexture("cubeitems");

	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 42, 42);
		m_pRender->Render(RELATIVE_X(s_nLeftOft),
			RELATIVE_Y(s_nTopOft));
	}

	int nRenderX = 0;
	int nRenderY = 0;
	int nRow = 0;
	int nCol = 0;
	int nTexWidth = 0;
	int nTexHeight = 0;

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	if(NULL != pTexMgr)
	{
		if(NULL != m_pItem)
		{
			if(m_pItem->type != ITEM_NO)
			{
				tex = pTexMgr->GetTexture(m_pItem->tex);
				if(0 != tex)
				{
					nTexWidth = pTexMgr->GetTextureWidth(m_pItem->tex);
					nTexHeight = pTexMgr->GetTextureHeight(m_pItem->tex);
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0,
						nTexWidth,
						nTexHeight);
					nRenderX = RELATIVE_X(s_nLeftOft + 1 - nTexWidth / 2 + 20);
					nRenderY = RELATIVE_Y(s_nTopOft + 1 - nTexHeight / 2 + 20);
					m_pRender->Render(nRenderX, nRenderY);
				}
			}
		}
	}

	AfxGetFont()->SetColor(ARGB_WHITE);
	AfxGetFont()->Print(RELATIVE_X(100),
		RELATIVE_Y(155),
		"鉴定价格：%d",
		m_nPrice);
}

bool GameIdentifyDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);
	GameScene::sThis->GetMainOpt()->GetIntroDlg()->ShowNothing();

	POINT ptTest = _mp;
	ptTest.x -= (m_rcClient.left + s_nLeftOft);
	ptTest.y -= (m_rcClient.top + s_nTopOft);

	if(ptTest.x >= 0 &&
		ptTest.x <= 42 &&
		ptTest.y >= 0 &&
		ptTest.y <= 42)
	{
		ptTest.x -= 1;
		ptTest.y -= 1;

		OnItem();
	}

	return true;
}

void GameIdentifyDlg::OnItem()
{
	if(m_pItem != NULL)
	{
		if(m_pItem->type != ITEM_NO)
		{
			GameScene::sThis->GetMainOpt()->GetIntroDlg()->ShowItemAttrib(m_pItem);
			GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(m_pItem);
		}
	}

	//if(m_pItems[_idx] == NULL)
	{
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			if(GameScene::sThis->GetMainOpt()->GetBagDlg2()->IsItemSel())
			{
				ItemAttrib* pSelItem = GameScene::sThis->GetMainOpt()->GetBagDlg2()->GetSelItem();
				if(pSelItem->type != ITEM_NO)
				{
					GameScene::sThis->GetMainOpt()->GetIntroDlg()->ShowItemAttrib(pSelItem);
					GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(pSelItem);

					SetItem(pSelItem);
					GameScene::sThis->GetMainOpt()->GetBagDlg2()->CancelSel();
				}
			}
		}
	}
}

void GameIdentifyDlg::SetItem(ItemAttrib* _pItem)
{
	m_pItem = _pItem;

	if(NULL == _pItem)
	{
		m_nPrice = 0;
	}
	else
	{
		if(pTheGame->IsEquipItem(*_pItem))
		{
			int nLevel = GetItemUpgrade(_pItem->level);
			int nEquipLevel = GetItemGrade(_pItem->id);

			if (nEquipLevel == 0 ||
				nEquipLevel > 3)
			{
				m_nPrice = (nLevel + 1) * 10000;
			}
			else
			{
				m_nPrice = (nLevel + 1) * 1000;
			}
		}
		else
		{
			m_nPrice = 0;
		}
	}
}

bool GameIdentifyDlg::OnCommonButtonClick(int _id)
{
	int nItemTag = 0;

	if(NULL == m_pItem)
	{
		GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "请放入物品");
		return true;
	}

	if(m_nDlgMode == GameIdentifyDlgMode_Identify)
	{
		if(0 == HideAttribHelper::GetAllAttribCount(m_pItem->maxMP))
		{
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "该物品无法鉴定");
			return true;
		}
		if(HideAttribHelper::GetActiveAttribCount(m_pItem->maxMP) >= HideAttribHelper::GetAllAttribCount(m_pItem->maxMP))
		{
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "该物品无法继续进行鉴定");
			return true;
		}

		PkgPlayerIdentifyItemReq req;
		req.dwTag = m_pItem->tag;
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
	}
	else if(m_nDlgMode == GameIdentifyDlgMode_Unbind)
	{
		PkgPlayerUnbindItemReq req;
		req.dwTag = m_pItem->tag;
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
	}

	SetItem(NULL);

	return true;
}