#include "GameDlgBase.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
GameStoreDlg::GameStoreDlg()
{
	ClearItem();
	m_pParent = NULL;

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_rcClient.right = 257;
	m_rcClient.bottom = 354;

	INIT_CLOSEBTN(232, 13);
	SetVisible(false);
}

GameStoreDlg::~GameStoreDlg()
{

}
//////////////////////////////////////////////////////////////////////////
void GameStoreDlg::ClearItem()
{
	for(int i = 0; i < MAX_STORE_NUMBER; ++i)
	{
		m_xItems[i].type = 0;
		m_xItems[i].tag = 0;
	}
}

ItemAttrib* GameStoreDlg::GetItem(DWORD _dwTag)
{
	for(int i = 0; i < MAX_STORE_NUMBER; ++i)
	{
		if(m_xItems[i].type != ITEM_NO)
		{
			if(m_xItems[i].tag == _dwTag)
			{
				return &m_xItems[i];
			}
		}
	}
	return NULL;
}

void GameStoreDlg::AddItem(const ItemAttrib* _pItem)
{
	int nIndex = -1;
	for(int i = 0; i < MAX_STORE_NUMBER; ++i)
	{
		if(m_xItems[i].type == ITEM_NO)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex != -1)
	{
		memcpy(&m_xItems[nIndex], _pItem, sizeof(ItemAttrib));
	}
}

void GameStoreDlg::Update(float _dt)
{

}

bool GameStoreDlg::IsCaptionBar(int _x, int _y)
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

void GameStoreDlg::Render()
{
	static const POINT ptTopLeft = {20, 48};
	static const int nEachWidth = 31;
	static const int nEachHeight = 31;
	static const int nOffsetX = 6;
	static const int nOffsetY = 6;
	hgeResourceManager* pMgr = m_pParent->GetResourceMgr();

	if(!m_bVisible ||
		!pMgr)
	{
		return;
	}

	int nDrawOffsetX = 0;
	int nDrawOffsetY = 0;

	HTEXTURE tex = 0;
	tex = pMgr->GetTexture("store");
	if(tex)
	{
		INITSPRITE(m_pRender, tex);
		m_pRender->Render(m_rcClient.left, m_rcClient.top);
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	GameTextureManager* pRes = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	ItemAttrib* pItem = NULL;
	for(int i = 0; i < MAX_STORE_NUMBER; ++i)
	{
		pItem = &m_xItems[i];
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

			if(pItem->type == ITEM_COST)
			{
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				AfxGetPrinter()->Print((i % 6) * (nEachWidth + nOffsetX) + ptTopLeft.x + m_rcClient.left + 0,
					(i / 6) * (nEachHeight + nOffsetY) + ptTopLeft.y + m_rcClient.top + 20,
					"%d",
					pItem->atkSpeed);
			}
		}
	}

	__super::Render();
}

bool GameStoreDlg::ProcUserCmd(const POINT& _mp)
{
	static const POINT ptTopLeft = {20, 48};
	static const int nEachWidth = 31;
	static const int nEachHeight = 31;
	static const int nOffsetX = 6;
	static const int nOffsetY = 6;

	POINT ptMouse = _mp;
	POINT ptDetect = ptMouse;

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
						if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
						{
							PkgPlayerShopOpReq req;
							req.bOp = SHOP_STORE;
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

void GameStoreDlg::OnItem(int _x, int _y)
{
	//int nSelIndex = m_nPage * (MAX_SHOP_ITEM / 2) + (_y * 6 + _x);
	int nSelIndex = (_y * 6 + _x);
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	//if(!m_pParent->GetBagDlg()->IsItemSel())
	{
		if(nSelIndex >= MAX_STORE_NUMBER)
		{
			return;
		}
		bool bHaveItem = false;
		if(m_xItems[nSelIndex].type != ITEM_NO)
		{
			m_pParent->GetIntroDlg()->ShowItemAttrib(&m_xItems[nSelIndex]);
			bHaveItem = true;
		}
		else
		{
			m_pParent->GetIntroDlg()->ShowNothing();
		}

		if(bHaveItem)
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				PkgPlayerShopOpReq req;
				req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
				req.bOp = SHOP_FETCH;
				req.dwData = m_xItems[nSelIndex].tag;
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBufferToGS(&g_xBuffer);
			}
		}
	}
}