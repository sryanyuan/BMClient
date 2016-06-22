#include "GameDlgBase.h"
#include "../GameScene/GameScene.h"
#include "GameDisplayDlg.h"
#include "../GameScene/GameResourceManager.h"
#include "GameInfoBoardDlg.h"
#include "../BackMir/BackMir.h"
#include "GameInputDlg.h"
//////////////////////////////////////////////////////////////////////////
const int s_nLeftOft = 14;
const int s_nTopOft = 76;
static const char* s_pszWindowTip = "[拆分物品]使用Alt+鼠标左键点击消耗品，即可拆分物品\n[整理包裹]Alt+R可以整理包裹";
//////////////////////////////////////////////////////////////////////////
GameBagDlg2::GameBagDlg2()
{
	m_rcClient.right = 354;
	m_rcClient.bottom = 310;

	m_nSel = INVALID_SEL_INDEX;

	m_pRender = NEW_EMPTY_SPRITE;
	SetBkStype(1);
	if(GetBkStyle() == 1)
	{
		m_rcClient.bottom -= 10;
	}
	CalcCloseButtonPosition();
	SetWindowTitle("背包");
	SetWindowTip(s_pszWindowTip);
	SetVisible(false);
}

GameBagDlg2::~GameBagDlg2()
{
	SAFE_DELETE(m_pRender);
}
//////////////////////////////////////////////////////////////////////////
bool GameBagDlg2::OnShowDialog()
{
	m_nSel = INVALID_SEL_INDEX;
	SetNeedTopLevel(true);
	CancelSel();
	return true;
}

bool GameBagDlg2::OnCloseDialog()
{
	m_nSel = INVALID_SEL_INDEX;
	CancelSel();
	return true;
}

void GameBagDlg2::Render()
{
	__super::Render();

	static const int nEachWidth = 34;
	static const int nEachHeight = 34;
	static const int nGridOftX = 7;
	static const int nGridOftY = 7;
	static const int nDrawX = 4;
	static const int nDrawY = 4;

	hgeResourceManager* pResMgr = GameScene::sThis->GetMainOpt()->GetResourceMgr();
	HTEXTURE tex = 0;
	int nDrawOffsetX = 0;
	int nDrawOffsetY = 0;

	tex = pResMgr->GetTexture("bagitems");
	if(0 != tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 328, 206);
		m_pRender->Render(RELATIVE_X(s_nLeftOft), RELATIVE_Y(s_nTopOft));
	}

	//	物品
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
			nDrawOffsetX = pRes->GetTextureWidth(pItem->tex);
			nDrawOffsetY = pRes->GetTextureHeight(pItem->tex);
			nDrawOffsetX = abs(nDrawOffsetX - nEachWidth) / 2;
			nDrawOffsetY = abs(nDrawOffsetY - nEachHeight) / 2;

			m_pRender->SetTexture(tex);

			m_pRender->SetTextureRect(0, 0, pRes->GetTextureWidth(pItem->tex), pRes->GetTextureHeight(pItem->tex));
			m_pRender->Render((i % 8) * (nEachWidth + nGridOftX) + nDrawX + m_rcClient.left + nDrawOffsetX + s_nLeftOft,
				(i / 8) * (nEachHeight + nGridOftY) + nDrawY + m_rcClient.top + nDrawOffsetY + s_nTopOft);

			if(pItem->type == ITEM_COST)
			{
				//	消耗品数量
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				AfxGetPrinter()->Print((i % 8) * (nEachWidth + nGridOftX) + nDrawX + m_rcClient.left + 5 + s_nLeftOft,
					(i / 8) * (nEachHeight + nGridOftY) + nDrawY + m_rcClient.top + 20 + s_nTopOft,
					"%d",
					pItem->atkSpeed);

				if(pItem->curse == 0)
				{
					//	药品 在冷却
					int nPercent = GamePlayer::GetInstance()->GetDrugCoolDownController()->GetCoolPercent(pItem->id);
					if(nPercent != 0)
					{
						//nPercent = 27 - nPercent;

						tex = pResMgr->GetTexture("cooldown");
						if(tex != 0)
						{
							m_pRender->SetTexture(tex);
							m_pRender->SetTextureRect(nPercent % 8 * 32, nPercent / 8 * 32, 32, 32);
							m_pRender->Render((i % 8) * (nEachWidth + nGridOftX) + nDrawX + m_rcClient.left + 2 + s_nLeftOft,
								(i / 8) * (nEachHeight + nGridOftY) + nDrawY + m_rcClient.top + 2 + s_nTopOft);
						}
					}
				}
			}
		}
	}

	//	金币
	//int nMoney = GamePlayer::GetInstance()->GetPlayerBag()->GetMoney();
	//AfxGetPrinter()->SetColor(ARGB_WHITE);
	//AfxGetPrinter()->Print(RELATIVE_X(40), RELATIVE_Y(GetRect().bottom - GetRect().top - 40), "金币: %d",
	//	nMoney);

	//	画选中物品
	/*if(IsItemSel())
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
	}*/

	RenderWindowTip();
}

bool GameBagDlg2::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

#ifdef _DEBUG
	/*ItemAttrib item;
	item.type = ITEM_NO;
	static int nID = 29;
	if(AfxGetHge()->Input_KeyUp(HGEK_A))
	{
		GameInfoManager::GetInstance()->GetItemAttrib(nID++, &item);
		GamePlayer::GetInstance()->GetPlayerBag()->AddItem(&item);
	}*/
#endif

	if(AfxGetHge()->Input_GetKeyState(HGEK_ALT) &&
		AfxGetHge()->Input_KeyDown(HGEK_R))
	{
		GamePlayer::GetInstance()->GetPlayerBag()->SortBag();
	}

	static const int nEachWidth = 34;
	static const int nEachHeight = 34;
	static const int nGridOftX = 7;
	static const int nGridOftY = 7;
	static const int nDrawX = 4;
	static const int nDrawY = 4;

	bool bInItem = false;

	if(_mp.x > RELATIVE_X(s_nLeftOft + nDrawX) &&
		_mp.x < RELATIVE_X(s_nLeftOft + nDrawX + nEachWidth * 8 + nGridOftX * 7) &&
		_mp.y > RELATIVE_Y(s_nTopOft + nDrawY) &&
		_mp.y < RELATIVE_Y(s_nTopOft + nDrawY + nEachHeight * 5 + nGridOftY * 4))
	{
		//	In the area
		int nAbsoluteX = _mp.x - m_rcClient.left - s_nLeftOft - nDrawX;
		int nAbsoluteY = _mp.y - m_rcClient.top - s_nTopOft - nDrawY;

		int nSelCol = nAbsoluteX / (nEachWidth + nGridOftX);
		int nSelColLeft = nAbsoluteX % (nEachWidth + nGridOftX);
		int nSelRow = nAbsoluteY / (nEachHeight + nGridOftY);
		int nSelRowLeft = nAbsoluteY % (nEachHeight + nGridOftY);

		if(nSelColLeft < nEachWidth &&
			nSelRowLeft < nEachHeight)
		{
			OnItem(nSelCol, nSelRow);
			bInItem = true;
		}
	}

	if(!bInItem)
	{
		GameMainOptUI::GetInstance()->GetIntroDlg()->ShowNothing();
		GameMainOptUI::GetInstance()->GetIntroDlg()->SetVisible(false);
	}

	return true;
}

void GameBagDlg2::OnItem(int _x, int _y)
{
	int nSelIndex = INVALID_SEL_INDEX;
	ItemAttrib* pSelItem = NULL;
	bool bHaveItem = false;

	if(_x >= 0 &&
		_x < 8 &&
		_y >= 0 &&
		_y < 5)
	{
		nSelIndex = _y * 8 + _x;

		if(nSelIndex >= 0 &&
			nSelIndex < HERO_MAINBAG_SIZE_CUR)
		{
			pSelItem = &GamePlayer::GetInstance()->GetPlayerBag()->GetItemList()[nSelIndex];
		}
	}

	if(NULL == pSelItem)
	{
		GameMainOptUI::GetInstance()->GetIntroDlg()->ShowNothing();
		return;
	}

	if(pSelItem->type == ITEM_NO)
	{
		GameMainOptUI::GetInstance()->GetIntroDlg()->ShowNothing();
	}
	else
	{
		GameMainOptUI::GetInstance()->GetIntroDlg()->ShowItemAttrib(pSelItem);
		GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(pSelItem);
		bHaveItem = true;
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();

	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		SetNeedTopLevel(true);

		if(IsItemSel())
		{
			//	有东西被选中的情况下
			if(bHaveItem)
			{
				//	有东西 则交换
				if(pSelItem->type == ITEM_COST &&
					GetSelItem()->type == ITEM_COST &&
					pSelItem->id == GetSelItem()->id &&
					pSelItem->tag != GetSelItem()->tag)
				{
					//	Mergy the selected item
					/*PkgPlayerMergyCostItemReq req;
					req.dwDestTag = pSelItem->tag;
					req.dwSrcTag = GetSelItem()->tag;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBuffer(&g_xBuffer);*/
					bool bNeedTip = false;
					if(TEST_FLAG_BOOL(pSelItem->atkPois, POIS_MASK_BIND) &&
						!TEST_FLAG_BOOL(GetSelItem()->atkPois, POIS_MASK_BIND))
					{
						bNeedTip = true;
					}
					else if(!TEST_FLAG_BOOL(pSelItem->atkPois, POIS_MASK_BIND) &&
						TEST_FLAG_BOOL(GetSelItem()->atkPois, POIS_MASK_BIND))
					{
						bNeedTip = true;
					}

					if(bNeedTip)
					{
						GameMainOptUI::GetInstance()->GetStaticDlg()->CreateItemMergyDlg(pSelItem->tag, GetSelItem()->tag);
					}
					else
					{
						PkgPlayerMergyCostItemReq req;
						req.dwDestTag = pSelItem->tag;
						req.dwSrcTag = GetSelItem()->tag;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
						g_xBuffer.Reset();
						g_xBuffer << req;
						SendBufferToGS(&g_xBuffer);
					}
					
					CancelSel();
				}
				else
				{
					pPlayer->GetPlayerBag()->ChangeBagItem(nSelIndex, m_nSel);
					CancelSel();
				}

				pTheGame->PlayItemPutDownSound(pSelItem);
			}
			else
			{
				//	没东西 直接放下去
				pPlayer->GetPlayerBag()->ChangeBagItem(nSelIndex, m_nSel);
				CancelSel();

				pTheGame->PlayItemPutDownSound(pSelItem);
			}
		}
		else
		{
			//	没有的话
			if(bHaveItem)
			{
				if(AfxGetHge()->Input_GetKeyState(HGEK_ALT) &&
					pSelItem->type == ITEM_COST &&
					pSelItem->atkSpeed > 1)
				{
					GameMainOptUI::GetInstance()->GetIntroDlg()->ShowNothing();
					GameMainOptUI::GetInstance()->GetInputDlg()->ShowAsMode(InputDlg_QuerySplitSum, pSelItem->tag, pSelItem->atkSpeed);
				}
				else
				{
					m_nSel = nSelIndex;

					pTheGame->PlayItemClickUpSound(pSelItem);
				}
			}
		}
	}

	if(AfxGetHge()->Input_KeyUp(HGEK_RBUTTON) &&
		bHaveItem)
	{
		if(IsItemSel())
		{
			return;
		}

		//	判断点击的是装备还是可以使用的东西
		DWORD dwErr = 0;
		ItemAttrib& item = *pSelItem;

		if(pPlayer->IsEquipItem(item))
		{
			//ItemAttrib* pSelItem = &item;
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

			if(bCanDress)
			{
				if(pSelItem->type == ITEM_WEAPON)
				{
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMWEAPONC);
				}
				else if(pSelItem->type == ITEM_CLOTH ||
					pSelItem->type == ITEM_HELMET ||
					pSelItem->type == ITEM_BELT ||
					pSelItem->type == ITEM_SHOE)
				{
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMCLOTHC);
				}
				else
				{
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMRINGC);
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
			else if(item.curse == 0)
			{
				//	药品
				if(!GamePlayer::GetInstance()->GetDrugCoolDownController()->CanUse(item.id, 1000 * item.hide))
				{
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
	}
}

bool GameBagDlg2::IsItemSel()
{
	return m_nSel != INVALID_SEL_INDEX;
}

ItemAttrib* GameBagDlg2::GetSelItem()
{
	if(IsItemSel())
	{
		return &GamePlayer::GetInstance()->GetPlayerBag()->GetItemList()[m_nSel];
	}
	return NULL;
}

void GameBagDlg2::CancelSel()
{
	m_nSel = INVALID_SEL_INDEX;
}