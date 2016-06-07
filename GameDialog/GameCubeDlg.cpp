#include "GameDlgBase.h"
#include "../GameScene/GameScene.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
#include "GameDisplayDlg.h"
//////////////////////////////////////////////////////////////////////////
const int s_nLeftOft = 48;
const int s_nTopOft = 80;
const int s_nDlgWidth = 260;
const int s_nDlgHeight = 240;
//////////////////////////////////////////////////////////////////////////
GameCubeDlg::GameCubeDlg()
{
	ResetAllItems();

	m_rcClient.right = s_nDlgWidth;
	m_rcClient.bottom = s_nDlgHeight;
	//SetBkStype(1);
	SetVisible(false);

	m_pRender = NEW_EMPTY_SPRITE;
	CalcCloseButtonPosition();
	AddCommonButton(CBTN_NORMAL, 0, s_nDlgWidth / 2 - 36, s_nTopOft + 83 + 20, "合成");

	SetWindowTitle("潘多拉魔盒");
	SetWindowInfo("神奇的合成魔盒");
}

GameCubeDlg::~GameCubeDlg()
{

}
//////////////////////////////////////////////////////////////////////////
void GameCubeDlg::Render()
{
	__super::Render();

	hgeResourceManager* pResMgr = GameScene::sThis->GetMainOpt()->GetResourceMgr();

	HTEXTURE tex = 0;
	tex = pResMgr->GetTexture("cubeitems");

	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 165, 83);
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
		for(int i = 0; i < CUBE_ITEM_NUM; ++i)
		{
			if(NULL != m_pItems[i])
			{
				if(m_pItems[i]->type != ITEM_NO)
				{
					nCol = i;
					if(i > 3)
					{
						nRow = 1;
						nCol -= 4;
					}

					tex = pTexMgr->GetTexture(m_pItems[i]->tex);
					if(0 != tex)
					{
						nTexWidth = pTexMgr->GetTextureWidth(m_pItems[i]->tex);
						nTexHeight = pTexMgr->GetTextureHeight(m_pItems[i]->tex);
						m_pRender->SetTexture(tex);
						m_pRender->SetTextureRect(0, 0,
							nTexWidth,
							nTexHeight);
						nRenderX = RELATIVE_X(s_nLeftOft + 1 + nCol * 41 + 20 - nTexWidth / 2);
						nRenderY = RELATIVE_Y(s_nTopOft + 1 + nRow * 41 + 20 - nTexHeight / 2);
						m_pRender->Render(nRenderX, nRenderY);
					}
				}
			}
		}
	}
}

bool GameCubeDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);
	GameScene::sThis->GetMainOpt()->GetIntroDlg()->ShowNothing();

	POINT ptTest = _mp;
	ptTest.x -= (m_rcClient.left + s_nLeftOft);
	ptTest.y -= (m_rcClient.top + s_nTopOft);

	if(ptTest.x >= 0 &&
		ptTest.x <= 165 &&
		ptTest.y >= 0 &&
		ptTest.y <= 83)
	{
		ptTest.x -= 1;
		ptTest.y -= 1;

		int nRow = 0;
		int nCol = 0;
		if(ptTest.y >= 41)
		{
			nRow = 1;
		}
		nCol = ptTest.x / 41;

		int nIndex = nRow * 4 + nCol;
		if(nIndex < 8)
		{
			OnItem(nIndex);
		}
	}

	return true;
}

bool GameCubeDlg::OnShowDialog()
{
	ResetAllItems();
	return true;
}

bool GameCubeDlg::OnCloseDialog()
{
	return true;
}

bool GameCubeDlg::OnCommonButtonClick(int _id)
{
	int nItemTags[8];
	int nItemSum = 0;

	for(int i = 0; i < CUBE_ITEM_NUM; ++i)
	{
		if(m_pItems[i] != NULL)
		{
			if(m_pItems[i]->type != ITEM_NO)
			{
				nItemTags[i] = m_pItems[i]->tag;
				++nItemSum;
			}
		}
	}

	if(nItemSum > 0)
	{
		PkgPlayerCubeItemsReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		for(int i = 0; i < nItemSum; ++i)
		{
			req.xTags.push_back(nItemTags[i]);
		}
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
		ResetAllItems();
	}
	return true;
}



void GameCubeDlg::ResetAllItems()
{
	for(int i = 0; i < CUBE_ITEM_NUM; ++i)
	{
		m_pItems[i] = NULL;
	}
}

void GameCubeDlg::InsertItems(ItemAttrib* _pItem)
{
	for(int i = 0; i < CUBE_ITEM_NUM; ++i)
	{
		if(NULL != m_pItems[i])
		{
			if(m_pItems[i]->tag == _pItem->tag)
			{
				return;
			}
		}
	}

	for(int i = 0; i < CUBE_ITEM_NUM; ++i)
	{
		if(NULL == m_pItems[i])
		{
			m_pItems[i] = _pItem;
			break;
		}
	}
}

void GameCubeDlg::OnItem(int _idx)
{
	if(m_pItems[_idx] != NULL)
	{
		if(m_pItems[_idx]->type != ITEM_NO)
		{
			GameScene::sThis->GetMainOpt()->GetIntroDlg()->ShowItemAttrib(m_pItems[_idx]);
			GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(m_pItems[_idx]);
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

					InsertItems(pSelItem);
					GameScene::sThis->GetMainOpt()->GetBagDlg2()->CancelSel();
				}
			}
		}
	}
}