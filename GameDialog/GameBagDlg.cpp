#include "../GameDialog/GameDlgBase.h"
#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameResourceManager.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
extern ByteBuffer g_xBuffer;

GameBagDlg::GameBagDlg()
{
	//m_pSpr = NULL;
	//m_pFont = new GfxFont("宋体", 12, 0, 0, 0);
	m_pResManager = NULL;
	m_nSel = -1;
// 	m_rcClient.left = VIEW_WIDTH - 257;
// 	m_rcClient.right = VIEW_WIDTH;
// 	m_rcClient.top = 0;
// 	m_rcClient.bottom = 354;
	m_rcClient.right = 257;
	m_rcClient.bottom = 354;
	m_bVisible = false;

	m_rcCloseBtn.left = 232;
	m_rcCloseBtn.top = 13;
	m_rcCloseBtn.right = m_rcCloseBtn.left + 16;
	m_rcCloseBtn.bottom = m_rcCloseBtn.top + 16;

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);

	AddCommonButton(CBTN_LEFTARROW, 0, 76, 279, NULL);
	AddCommonButton(CBTN_RIGHTARROW, 1, 168, 279, NULL);
	AddCommonButton(CBTN_NORMAL, 2, 159, 309, "整理背包");

	//AddCommonButton(CBTN_LEFTARROW, 0, 76, 90);

	UpdateRect();
}

GameBagDlg::~GameBagDlg()
{
	//delete m_pFont;
	//delete m_pSpr;
	SAFE_DELETE(m_pRender);
}

bool GameBagDlg::IsCaptionBar(int _x, int _y)
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


void GameBagDlg::UpdateRect()
{

}

void GameBagDlg::Render()
{
	static const POINT ptTopLeft = {20, 48};
	static const int nEachWidth = 31;
	static const int nEachHeight = 31;
	static const int nOffsetX = 6;
	static const int nOffsetY = 6;

	if(!m_bVisible ||
		!m_pResManager)
	{
		return;
	}

	int nDrawOffsetX = 0;
	int nDrawOffsetY = 0;

	HTEXTURE tex = 0;
	tex = m_pResManager->GetTexture("bag");
	if(tex)
	{
		INITSPRITE(m_pRender, tex);
		m_pRender->Render(m_rcClient.left, m_rcClient.top);
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	GameTextureManager* pRes = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	ItemAttrib* pItem = NULL;
	for(int i = 0; i < HERO_MAINBAG_SIZE_CUR; ++i)
	{
		pItem = &pPlayer->GetPlayerBag()->GetItemList()[i];
		if(pItem->type != ITEM_NO &&
			i != m_nSel)
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

	//	画金钱
	//m_pFont->SetColor(ARGB_WHITE);
	//m_pFont->Print(RELATIVE_X(76), RELATIVE_Y(312), "%d", pPlayer->GetPlayerBag()->GetMoney());
	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->Print(RELATIVE_X(76), RELATIVE_Y(312), "%d", pPlayer->GetPlayerBag()->GetMoney());

	//	画选中物品
	if(IsItemSel())
	{
		tex = pRes->GetTexture(GetSelItem()->tex);
		if(tex)
		{
			float x,y;
			AfxGetHge()->Input_GetMousePos(&x, &y);
			//SETSPRITE(tex, m_pSpr);
// 			nDrawOffsetX = pRes->GetTextureWidth(tex);
// 			nDrawOffsetY = pRes->GetTextureHeight(tex);
			nDrawOffsetX = pRes->GetTextureWidth(GetSelItem()->tex);
			nDrawOffsetY = pRes->GetTextureHeight(GetSelItem()->tex);
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, nDrawOffsetX, nDrawOffsetY);
			nDrawOffsetX /= 2;
			nDrawOffsetY /= 2;
			m_pRender->Render(x - nDrawOffsetX, y - nDrawOffsetY);
		}
	}

	__super::Render();
}

void GameBagDlg::Update(float _dt)
{

}

bool GameBagDlg::ProcUserCmd(const POINT& _mp)
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
				if(ptDetect.x - nActX * (nEachWidth + nOffsetX) < nEachWidth &&
					ptDetect.y - nActY * (nEachHeight + nOffsetY) < nEachHeight)
				{
					OnItem(nActX, nActY);
					bInItem = true;
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

void GameBagDlg::OnItem(int _x, int _y)
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	ItemAttrib& item = pPlayer->GetPlayerBag()->GetItemList()[_x + _y * 6];
	int nSelIndex = _x + _y * 6;
	bool bHaveItem = false;
	if(item.type != ITEM_NO &&
		nSelIndex != m_nSel)
	{
		//	move
		m_pParent->GetIntroDlg()->ShowItemAttrib(&item);
		bHaveItem = true;
	}
// 	else
// 	{
// 		m_pParent->GetIntroDlg()->ShowNothing();
// 		return;
// 	}
	if(!bHaveItem)
	{
		m_pParent->GetIntroDlg()->ShowNothing();
	}

//	if(!m_pParent->IsAssistItemSel())
	{
		//	没有辅助栏的东西被选中
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			::PostMessage(AfxGetHge()->System_GetState(HGE_HWND), WM_DLG_BRINGTOTOP, (WPARAM)this, 0);

			if(IsItemSel())
			{
				//	有东西被选中的情况下
				if(bHaveItem)
				{
					//	有东西 则交换
					pPlayer->GetPlayerBag()->ChangeBagItem(nSelIndex, m_nSel);
				}
				else
				{
					//	没东西 直接放下去
					pPlayer->GetPlayerBag()->ChangeBagItem(nSelIndex, m_nSel);
					CancelSel();
				}
			}
			else
			{
				//	没有的话
				if(bHaveItem)
				{
					m_nSel = nSelIndex;
				}
			}
		}

		if(AfxGetHge()->Input_KeyUp(HGEK_RBUTTON) &&
			bHaveItem)
		{
			if(IsItemSel())
			{
				//	有东西选中了 不处理
				return;
			}

			//	判断点击的是装备还是可以使用的东西
			DWORD dwErr = 0;
#ifdef _NET_GAME_
			if(pPlayer->IsEquipItem(item))
			{
				ItemAttrib* pSelItem = &item;
				bool bCanDress = true;

				if(pSelItem->type == ITEM_WEAPON)
				{
					if(pPlayer->GetWanLi() < pSelItem->weight)
					{
						bCanDress = false;
					}
				}

				if(pSelItem->reqType == REQ_LEVEL)
				{
					if(pPlayer->GetAttrib()->level < pSelItem->reqValue)
					{
						bCanDress = false;
						GameInfoBoardDlg::GetInstance()->InsertEquipErrorMsg(REQ_ERR_LEVEL);
					}
				}
				else if(pSelItem->reqType == REQ_DC)
				{
					if(pPlayer->GetAttrib()->maxDC < pSelItem->reqValue)
					{
						bCanDress = false;
						GameInfoBoardDlg::GetInstance()->InsertEquipErrorMsg(REQ_ERR_DC);
					}
				}
				else if(pSelItem->reqType == REQ_SC)
				{
					if(pPlayer->GetAttrib()->maxSC < pSelItem->reqValue)
					{
						bCanDress = false;
						GameInfoBoardDlg::GetInstance()->InsertEquipErrorMsg(REQ_ERR_SC);
					}
				}
				else if(pSelItem->reqType == REQ_MC)
				{
					if(pPlayer->GetAttrib()->maxMC < pSelItem->reqValue)
					{
						bCanDress = false;
						GameInfoBoardDlg::GetInstance()->InsertEquipErrorMsg(REQ_ERR_MC);
					}
				}
				if(pSelItem->sex != 0)
				{
					if(pPlayer->GetAttrib()->sex != pSelItem->sex)
					{
						bCanDress = false;
						GameInfoBoardDlg::GetInstance()->InsertEquipErrorMsg(REQ_ERR_SEX);
					}
				}

				if(!bCanDress)
				{
					//GameInfoBoardDlg::GetInstance()->InsertBoardMsg("检测");
					return;
				}
			}
			else if(item.type == ITEM_COST)
			{
				//	检测是否藏宝图
				if(item.curse == 2)
				{
					int nMapID = GamePlayer::GetInstance()->GetMapID();
					bool bValid = true;

					if(nMapID != item.accuracy)
					{
						bValid = false;
					}
					else
					{
						int nCurPosX = GamePlayer::GetInstance()->GetCoordX();
						int nCurPosY = GamePlayer::GetInstance()->GetCoordY();

						int nOftX = abs((int)item.maxAC - nCurPosX);
						int nOftY = abs((int)item.AC - nCurPosY);

						if(nOftX >= 5 ||
							nOftY >= 5)
						{
							bValid = false;
						}
					}
					if(!bValid)
					{
						char szMsg[100];
						std::string xMapName;

						if(pTheGame->GetMapName(item.accuracy, xMapName))
						{
							sprintf(szMsg, "藏宝图:挖宝地点[%s](%d,%d)附近",
								xMapName.c_str(), item.maxAC, item.AC);
							GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, szMsg);
						}

						return;
					}
				}
			}
			PkgPlayerUseItemReq req;
			req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
			req.dwTag = item.tag;
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
#else
			if(pPlayer->IsEquipItem(item))
			{
				if(pPlayer->DressEquip(nSelIndex, &dwErr))
				{
					m_pParent->GetIntroDlg()->ShowNothing();
				}
			}
			else if(pPlayer->IsCostItem(item))
			{
				if(pPlayer->UseItemFromBag(nSelIndex))
				{
					m_pParent->GetIntroDlg()->ShowNothing();
				}
			}
#endif
		}
	}
// 	else
// 	{
// 		//	辅助栏的东西被选中
// 		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
// 		{
// 			if(bHaveItem)
// 			{
// 				//	鼠标位置有物品
// 			}
// 			else
// 			{
// 				//	无物品 可以放回去
// 				pPlayer->GetPlayerBag()->MoveAssistItemToBag(nSelIndex, m_pParent->GetAssistSelIndex());
// 			}
// 		}
// 	}
}

bool GameBagDlg::IsItemSel()
{
	return m_nSel != -1;
}

ItemAttrib* GameBagDlg::GetSelItem()
{
	if(IsItemSel())
	{
		return &GamePlayer::GetInstance()->GetPlayerBag()->GetItemList()[m_nSel];
	}
	return NULL;
}

void GameBagDlg::CancelSel()
{
	m_nSel = -1;
}