#include "GameDecomposeDlg.h"
#include "../GameScene/GameResourceManager.h"
#include "GameInfoBoardDlg.h"
#include "GameDisplayDlg.h"
#include "../BackMir/BackMir.h"
#include "GameStoveDlg.h"
//////////////////////////////////////////////////////////////////////////
#define DLG_WIDTH	300
#define DLG_HEIGHT	450

#define PAGE_DECOMPOSE	0
#define PAGE_FORGE		1

#define BTN_DECOMPOSE	0
#define BTN_FORGE		1
#define BTN_LEFT		2
#define BTN_RIGHT		3
#define BTN_STOVE		4

#define RENDER_OFTX		DLG_WIDTH / 2 - 244 / 2
#define RENDER_OFTY		50
//////////////////////////////////////////////////////////////////////////
GameDecomposeDlg::GameDecomposeDlg()
{
	m_pRender = NEW_EMPTY_SPRITE;

	m_rcClient.left = WINDOW_WIDTH / 2 - DLG_WIDTH / 2;
	m_rcClient.right = m_rcClient.left + DLG_WIDTH;
	m_rcClient.top = WINDOW_HEIGHT / 2 - DLG_HEIGHT / 2;
	m_rcClient.bottom = m_rcClient.top + DLG_HEIGHT + 20;

	SetVisible(false);
	SetBkStype(1);
	CalcCloseButtonPosition();

	SwitchToDecompose();

	AddCommonButton(CBTN_LEFTARROW, BTN_LEFT, DLG_WIDTH / 2 - 100, DLG_HEIGHT - 60, NULL);
	AddCommonButton(CBTN_RIGHTARROW, BTN_RIGHT, DLG_WIDTH / 2 + 100, DLG_HEIGHT - 60, NULL);
	AddCommonButton(CBTN_NORMAL, BTN_DECOMPOSE, DLG_WIDTH / 2 - 74 / 2, DLG_HEIGHT - 60, "分解装备");
	AddCommonButton(CBTN_NORMAL, BTN_STOVE, DLG_WIDTH / 2 - 74 / 2, DLG_HEIGHT - 25, "手工铸造");

	ZeroMemory(m_rcRect, sizeof(m_rcRect));
	ZeroMemory(m_stItems, sizeof(m_stItems));

	static const int s_nLeftTopOft[] = {104, 59, 46, 96, 165, 96, 73, 151, 139, 151, 105, 208};

	for(int i = 0; i < 6; ++i)
	{
		m_rcRect[i].left = s_nLeftTopOft[i * 2] + RENDER_OFTX;
		m_rcRect[i].top = s_nLeftTopOft[i * 2 + 1] + RENDER_OFTY;
		m_rcRect[i].right = m_rcRect[i].left + 33;
		m_rcRect[i].bottom = m_rcRect[i].top + 31;
	}

	m_nPriceCost = 0;
}

GameDecomposeDlg::~GameDecomposeDlg()
{
	SAFE_DELETE(m_pRender);
}

void GameDecomposeDlg::Render()
{
	__super::Render();

	if(m_bPage == PAGE_DECOMPOSE)
	{
		RenderDecompose();
	}
	else if(m_bPage == PAGE_FORGE)
	{
		RenderForge();
	}
}

bool GameDecomposeDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	GameMainOptUI::GetInstance()->GetIntroDlg()->ShowNothing();

	if(m_bPage == PAGE_DECOMPOSE)
	{
		CommandDecompose(_mp);
	}
	else if(m_bPage == PAGE_FORGE)
	{
		CommandForge(_mp);
	}

	return true;
}

bool GameDecomposeDlg::OnCommonButtonClick(int _id)
{
	if(_id == BTN_LEFT ||
		_id == BTN_RIGHT)
	{
		if(m_bPage == PAGE_DECOMPOSE)
		{
			m_bPage = PAGE_FORGE;
			CommonButton* pBtnData = GetCommonButtonData(BTN_DECOMPOSE);
			if(pBtnData)
			{
				strcpy(pBtnData->szText, "锻造合成");
			}

			SetWindowInfo("");
			SetWindowTitle("锻造与合成");

			m_nPriceCost = 0;
			ZeroMemory(m_stItems, sizeof(m_stItems));
		}
		else
		{
			m_bPage = PAGE_DECOMPOSE;
			CommonButton* pBtnData = GetCommonButtonData(BTN_DECOMPOSE);
			if(pBtnData)
			{
				strcpy(pBtnData->szText, "分解装备");
			}

			SetWindowInfo("");
			SetWindowTitle("装备分解");

			m_nPriceCost = 0;
			ZeroMemory(m_stItems, sizeof(m_stItems));
		}
	}
	else if(_id == BTN_DECOMPOSE)
	{
		//	分解装备
		if(m_bPage == PAGE_DECOMPOSE)
		{
			if(GamePlayer::GetInstance()->GetPlayerBag()->GetMoney() < m_nPriceCost)
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg("您没有足够金币");
			}
			else
			{
				if(m_nPriceCost != 0 &&
					m_stItems[5].type != ITEM_NO)
				{
					PkgPlayerDecomposeReq req;
					req.uUserId = GETSELFPLAYERHANDLERID;
					req.dwItemTag = m_stItems[5].tag;

					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);

					//	Clear all data
					m_nPriceCost = 0;
					ZeroMemory(m_stItems, sizeof(m_stItems));
				}
			}
		}
		else if(m_bPage == PAGE_FORGE)
		{
			if(m_nPriceCost != 0)
			{
				PkgPlayerForgeItemReq req;
				req.uUserId = GETSELFPLAYERHANDLERID;
				for(int i = 0; i < 6; ++i)
				{
					if(m_stItems[i].type != ITEM_NO)
					{
						req.xItems.push_back(m_stItems[i].tag);
					}
				}
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBufferToGS(&g_xBuffer);

				//	Clear all data
				m_nPriceCost = 0;
				ZeroMemory(m_stItems, sizeof(m_stItems));
			}
		}
	}
	else if(_id == BTN_STOVE)
	{
		GameMainOptUI::GetInstance()->GetStoveDlg()->ShowDialog();
		CloseDialog();
	}
	return true;
}

bool GameDecomposeDlg::OnShowDialog()
{
	ZeroMemory(m_stItems, sizeof(m_stItems));
	SetWindowInfo("");
	SwitchToDecompose();
	SetNeedTopLevel(true);

	return true;
}

void GameDecomposeDlg::RenderDecompose()
{
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	if(pTexMgr)
	{
		HTEXTURE tex = pTexMgr->GetTexture(TEX_DECOMPOSE);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(TEX_DECOMPOSE),
				pTexMgr->GetTextureHeight(TEX_DECOMPOSE));
			m_pRender->Render(RELATIVE_X(RENDER_OFTX),
				RELATIVE_Y(RENDER_OFTY));
		}
	}

	GameTextureManager* pTexMgrItems = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	if(NULL == pTexMgr)
	{
		return;
	}

	HTEXTURE tex = 0;
	int nRenderX = 0;
	int nRenderY = 0;
	int nTexWidth = 0;
	int nTexHeight = 0;

	for(int i = 0; i < 6; ++i)
	{
		if(m_stItems[i].type != ITEM_NO)
		{
			tex = pTexMgrItems->GetTexture(m_stItems[i].tex);

			if(tex)
			{
				nTexWidth = pTexMgrItems->GetTextureWidth(m_stItems[i].tex);
				nTexHeight = pTexMgrItems->GetTextureHeight(m_stItems[i].tex);

				nRenderX = m_rcRect[i].left + 32 / 2 - nTexWidth / 2 + m_rcClient.left;
				nRenderY = m_rcRect[i].top + 30 / 2 - nTexHeight / 2 + m_rcClient.top;

				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
				m_pRender->Render(nRenderX, nRenderY);

				if(i != 5)
				{
					AfxGetPrinter()->SetColor(ARGB_WHITE);
					nRenderX = m_rcRect[i].left + m_rcClient.left;
					nRenderY = m_rcRect[i].top + m_rcClient.top + 20;
					AfxGetPrinter()->Print(nRenderX, nRenderY, "%d", m_stItems[i].atkSpeed);
				}
			}
		}
	}

	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->Print(RELATIVE_X(RENDER_OFTX + 48), RELATIVE_Y(275 + RENDER_OFTY), "分解所需金币:%d", m_nPriceCost);
}

void GameDecomposeDlg::RenderForge()
{
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	if(pTexMgr)
	{
		HTEXTURE tex = pTexMgr->GetTexture(TEX_DECOMPOSE);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(TEX_DECOMPOSE),
				pTexMgr->GetTextureHeight(TEX_DECOMPOSE));
			m_pRender->Render(RELATIVE_X(RENDER_OFTX),
				RELATIVE_Y(RENDER_OFTY));
		}
	}

	GameTextureManager* pTexMgrItems = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	if(NULL == pTexMgr)
	{
		return;
	}

	HTEXTURE tex = 0;
	int nRenderX = 0;
	int nRenderY = 0;
	int nTexWidth = 0;
	int nTexHeight = 0;

	for(int i = 0; i < 6; ++i)
	{
		if(m_stItems[i].type != ITEM_NO)
		{
			tex = pTexMgrItems->GetTexture(m_stItems[i].tex);

			if(tex)
			{
				nTexWidth = pTexMgrItems->GetTextureWidth(m_stItems[i].tex);
				nTexHeight = pTexMgrItems->GetTextureHeight(m_stItems[i].tex);

				nRenderX = m_rcRect[i].left + 32 / 2 - nTexWidth / 2 + m_rcClient.left;
				nRenderY = m_rcRect[i].top + 30 / 2 - nTexHeight / 2 + m_rcClient.top;

				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
				m_pRender->Render(nRenderX, nRenderY);

				if(i != 5)
				{
					AfxGetPrinter()->SetColor(ARGB_WHITE);
					nRenderX = m_rcRect[i].left + m_rcClient.left;
					nRenderY = m_rcRect[i].top + m_rcClient.top + 20;
					AfxGetPrinter()->Print(nRenderX, nRenderY, "%d", m_stItems[i].atkSpeed);
				}
			}
		}
	}

	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->Print(RELATIVE_X(RENDER_OFTX + 48), RELATIVE_Y(275 + RENDER_OFTY), "合成所需金币:%d", m_nPriceCost);
}

void GameDecomposeDlg::CommandDecompose(const POINT& _ptMouse)
{
	POINT ptTest = _ptMouse;
	ptTest.x -= (m_rcClient.left + 0);
	ptTest.y -= (m_rcClient.top + 0);

	for(int i = 0; i < 6; ++i)
	{
		if(PtInRect(&m_rcRect[i], ptTest))
		{
			OnItemDecompose(i);
			break;
		}
	}
}

void GameDecomposeDlg::OnItemDecompose(int _idx)
{
	ItemAttrib* pSelItem = GameMainOptUI::GetInstance()->GetBagDlg2()->GetSelItem();

	if(m_stItems[_idx].type != ITEM_NO)
	{
		GameMainOptUI::GetInstance()->GetIntroDlg()->ShowItemAttrib(&m_stItems[_idx]);
		GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(&m_stItems[_idx]);
	}

	if(m_bPage == PAGE_DECOMPOSE)
	{
		if(_idx == 5)
		{
			if(pSelItem != NULL &&
				pSelItem->type != ITEM_NO)
			{
				if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
				{
					ZeroMemory(m_stItems, sizeof(m_stItems));
					m_nPriceCost = 0;
					memcpy(&m_stItems[_idx], pSelItem, sizeof(ItemAttrib));
					GameMainOptUI::GetInstance()->GetBagDlg2()->CancelSel();
					ParseItemDecompose(pSelItem);
				}
			}
		}
	}
}

void GameDecomposeDlg::ParseItemDecompose(ItemAttrib* _pItem)
{
	bool bRet = false;

	if(_pItem)
	{
		ItemAttrib oriItem;

		if(GameInfoManager::GetInstance()->GetItemAttrib(_pItem->id, &oriItem))
		{
			int nItemLevel = GetItemGrade(_pItem->id);
			if(0 != nItemLevel)
			{
				int nACUp = 0;
				int nMACUp = 0;
				int nDCUp = 0;
				int nMCUp = 0;
				int nSCUp = 0;

				nACUp = _pItem->maxAC - oriItem.maxAC;
				nMACUp = _pItem->maxMAC - oriItem.maxMAC;
				nDCUp = _pItem->maxDC - oriItem.maxDC;
				nMCUp = _pItem->maxMC - oriItem.maxMC;
				nSCUp = _pItem->maxSC - oriItem.maxSC;

				int nDCGetStone = 0;
				int nMCGetStone = 0;
				int nSCGetStone = 0;

				//	Now just decompose DC MC SC
				if(nDCUp > 0)
				{
					/*if(nDCUp <= 3)
					{
						nDCGetStone = 1;
					}
					else if(nDCUp <= 6)
					{
						nDCGetStone = 2;
					}
					else
					{
						nDCGetStone = 3;
					}*/
					nDCGetStone = nDCUp;
				}

				if(nMCUp > 0)
				{
					/*if(nMCUp <= 3)
					{
						nMCGetStone = 1;
					}
					else if(nMCUp <= 6)
					{
						nMCGetStone = 2;
					}
					else
					{
						nMCGetStone = 3;
					}*/
					nMCGetStone = nMCUp;
				}

				if(nSCUp > 0)
				{
					/*if(nSCUp <= 3)
					{
						nSCGetStone = 1;
					}
					else if(nSCUp <= 6)
					{
						nSCGetStone = 2;
					}
					else
					{
						nSCGetStone = 3;
					}*/
					nSCGetStone = nSCUp;
				}

				/*static const int s_nDCStoneTable[6] = {685,686,687,688,689,691};
				static const int s_nMCStoneTable[6] = {692,693,694,695,696,697};
				static const int s_nSCStoneTable[6] = {698,699,700,701,702,703};*/

				if(nDCGetStone > 0)
				{
					for(int i = 0; i < 5; ++i)
					{
						if(m_stItems[i].type == ITEM_NO)
						{
							GameInfoManager::GetInstance()->GetItemAttrib(g_nDCStoneTable[nItemLevel - 2], &m_stItems[i]);
							m_stItems[i].atkSpeed = nDCGetStone;
							bRet = true;
							m_nPriceCost += ((nItemLevel - 1) * 1000 * nDCGetStone);
							break;
						}
					}
				}

				if(nMCGetStone > 0)
				{
					for(int i = 0; i < 5; ++i)
					{
						if(m_stItems[i].type == ITEM_NO)
						{
							GameInfoManager::GetInstance()->GetItemAttrib(g_nMCStoneTable[nItemLevel - 2], &m_stItems[i]);
							m_stItems[i].atkSpeed = nMCGetStone;
							bRet = true;
							m_nPriceCost += ((nItemLevel - 1) * 1000 * nMCGetStone);
							break;
						}
					}
				}

				if(nSCGetStone > 0)
				{
					for(int i = 0; i < 5; ++i)
					{
						if(m_stItems[i].type == ITEM_NO)
						{
							GameInfoManager::GetInstance()->GetItemAttrib(g_nSCStoneTable[nItemLevel - 2], &m_stItems[i]);
							m_stItems[i].atkSpeed = nSCGetStone;
							bRet = true;
							m_nPriceCost += ((nItemLevel - 1) * 1000 * nSCGetStone);
							break;
						}
					}
				}

#ifdef _DEBUG
				AfxGetHge()->System_Log("ACUP:%d MACUP:%d DCUP:%d MCUP:%d SCUP:%d",
					nACUp, nMACUp, nDCUp,  nMCUp, nSCUp);
#endif
			}
		}
	}

	if(!bRet)
	{
		SetWindowInfo("无法分解此物品");
	}
	else
	{
		SetWindowInfo("该物品可以分解得到以下精华石");
	}
}

void GameDecomposeDlg::CommandForge(const POINT& _ptMouse)
{
	POINT ptTest = _ptMouse;
	ptTest.x -= (m_rcClient.left + 0);
	ptTest.y -= (m_rcClient.top + 0);

	for(int i = 0; i < 6; ++i)
	{
		if(PtInRect(&m_rcRect[i], ptTest))
		{
			OnItemForge(i);
			break;
		}
	}
}

void GameDecomposeDlg::OnItemForge(int _idx)
{
	ItemAttrib* pSelItem = GameMainOptUI::GetInstance()->GetBagDlg2()->GetSelItem();

	if(m_stItems[_idx].type != ITEM_NO)
	{
		GameMainOptUI::GetInstance()->GetIntroDlg()->ShowItemAttrib(&m_stItems[_idx]);
		GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(&m_stItems[_idx]);
	}

	if(NULL == pSelItem)
	{
		return;
	}

	if(m_bPage == PAGE_FORGE)
	{
		if(_idx == 5)
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				if(MirGame::IsEquipItem(*pSelItem))
				{
					m_nPriceCost = 0;
					memcpy(&m_stItems[_idx], pSelItem, sizeof(ItemAttrib));
					GameMainOptUI::GetInstance()->GetBagDlg2()->CancelSel();

					ParseItemForge();
				}
			}
		}
		else
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				if(pSelItem->type == ITEM_COST &&
					pSelItem->curse == 3 &&
					pSelItem->lucky == 1)
				{
					ItemAttrib* pCopy = NULL;
					for(int i = 0; i < 5; ++i)
					{
						if(m_stItems[i].tag == pSelItem->tag)
						{
							pCopy = &m_stItems[i];
							break;
						}
					}
					if(NULL != pCopy)
					{
						ZeroMemory(pCopy, sizeof(ItemAttrib));
					}

					m_nPriceCost = 0;
					memcpy(&m_stItems[_idx], pSelItem, sizeof(ItemAttrib));
					GameMainOptUI::GetInstance()->GetBagDlg2()->CancelSel();

					ParseItemForge();
				}
				else if(pSelItem->type == ITEM_COST &&
					pSelItem->curse == 3 &&
					pSelItem->lucky == 2)
				{
					ItemAttrib* pCopy = NULL;
					for(int i = 0; i < 5; ++i)
					{
						if(m_stItems[i].tag == pSelItem->tag)
						{
							pCopy = &m_stItems[i];
							break;
						}
					}
					if(NULL != pCopy)
					{
						ZeroMemory(pCopy, sizeof(ItemAttrib));
					}

					m_nPriceCost = 0;
					memcpy(&m_stItems[_idx], pSelItem, sizeof(ItemAttrib));
					GameMainOptUI::GetInstance()->GetBagDlg2()->CancelSel();

					ParseItemForge();
				}
			}
		}
		/*if(_idx == 5)
		{
			if(pSelItem != NULL &&
				pSelItem->type != ITEM_NO)
			{
				if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
				{
					ZeroMemory(m_stItems, sizeof(m_stItems));
					m_nPriceCost = 0;
					memcpy(&m_stItems[_idx], pSelItem, sizeof(ItemAttrib));
					GameMainOptUI::GetInstance()->GetBagDlg2()->CancelSel();
					ParseItemDecompose(pSelItem);
				}
			}
		}*/
	}
}

void GameDecomposeDlg::ParseItemForge()
{
	if(MirGame::IsEquipItem(m_stItems[5]))
	{
		//	forge equipment
		bool bValid = true;
		int nCounter = 0;
		int nStoneIndex = -1;
		int nTransStoneIndex = -1;

		for(int i = 0; i < 5; ++i)
		{
			if(m_stItems[i].type != ITEM_NO)
			{
				/*if(m_stItems[i].type != ITEM_COST ||
					m_stItems[i].curse != 3 ||
					m_stItems[i].lucky != 1)
				{
					bValid = false;
				}
				else*/
				if(m_stItems[i].type == ITEM_COST &&
					m_stItems[i].curse == 3 &&
					m_stItems[i].lucky == 1)
				{
					nStoneIndex = i;
				}
				else if(m_stItems[i].type == ITEM_COST &&
					m_stItems[i].curse == 3 &&
					m_stItems[i].lucky == 2)
				{
					nTransStoneIndex = i;
				}

				++nCounter;
			}
		}

		/*if(!bValid ||
			nCounter != 1)
		{
			int nTransStoneIndex = -1;
			//	乾坤石
			if(nCounter == 1)
			{
				for(int i = 0; i < 5; ++i)
				{
					if(m_stItems[i].type != ITEM_NO)
					{
						if(m_stItems[i].type == ITEM_COST &&
							m_stItems[i].curse == 3 &&
							m_stItems[i].lucky == 2)
						{
							nTransStoneIndex = i;
							break;
						}
					}
				}

				if(nTransStoneIndex != -1)
				{
					SetWindowInfo("乾坤石将随机分配极品点数");
				}
				else
				{
					SetWindowInfo("无法合成该物品或者精华石");
				}
			}
			else
			{
				SetWindowInfo("无法合成该物品或者精华石");
			}
		}
		else*/
		if(nCounter == 1 &&
			nStoneIndex != -1)
		{
			int nItemGrade = GetItemGrade(m_stItems[5].id);

			if(0 != nItemGrade)
			{
				if(m_stItems[nStoneIndex].accuracy < nItemGrade)
				{
					bValid = false;
				}
			}
			else
			{
				bValid = false;
			}

			if(!bValid)
			{
				SetWindowInfo("无法合成该物品或者精华石");
			}
			else
			{
				//	Check sum
				int nCurValue = -1;

				WORD wLevel = m_stItems[5].level;

				if(wLevel != 0)
				{
					BYTE bLow = LOBYTE(wLevel);
					BYTE bHigh = HIBYTE(wLevel);

					bHigh &= 0x7A;
					bool bZero = false;

					if((bHigh & 0x40) == 0)
					{
						bZero = ((bHigh & 0x10) != 0 ? false : true);
						if(bZero)
						{
							bHigh |= 0x10;
						}
						else
						{
							bHigh &= (~0x10);
						}
					}

					if((bHigh & 0x02) == 0)
					{
						bZero = ((bHigh & 0x08) != 0 ? false : true);
						if(bZero)
						{
							bHigh |= 0x08;
						}
						else
						{
							bHigh &= (~0x08);
						}
					}

					static int s_nValueTable[] =
					{
						9, 2, 1, 3, 4, 7, 8, 5
					};
					static BYTE s_btMaskTable[] =
					{
						0x80, 0x40, 0x20, 0x10,
						0x08, 0x04, 0x02, 0x01
					};

					int nValue = 0;
					for(int i = 0; i < 8; ++i)
					{
						if((bHigh & s_btMaskTable[i]) != 0)
						{
							nValue += s_nValueTable[i];
						}
					}

					nValue = (int)bLow - nValue;
					if(nValue >= 0 &&
						nValue <= 7)
					{
						nCurValue = nValue;
					}

					nCurValue = GetItemUpgrade(wLevel);
				}
				else
				{
					nCurValue = 0;
				}

				//////////////////////////////////////////////////////////////////////////

				if(nCurValue >= 0 &&
					nCurValue < 5)
				{
					if(m_stItems[nStoneIndex].hide == 1)
					{
						SetWindowInfo("该组合将提升装备的攻击力");
					}
					else if(m_stItems[nStoneIndex].hide == 2)
					{
						SetWindowInfo("该组合将提升装备的魔法力");
					}
					else if(m_stItems[nStoneIndex].hide == 3)
					{
						SetWindowInfo("该组合将提升装备的道术");
					}

					//m_nPriceCost = (nCurValue + 1) * 20000;
					m_nPriceCost = (nCurValue + 1) * nItemGrade * 2500;
				}
				else
				{
					SetWindowInfo("无法提升该装备属性");
				}
			}
		}
		else if(nCounter == 1 &&
			nTransStoneIndex != -1)
		{
			//	Check sum
			int nCurValue = -1;

			WORD wLevel = m_stItems[5].level;

			if(wLevel != 0)
			{
				BYTE bLow = LOBYTE(wLevel);
				BYTE bHigh = HIBYTE(wLevel);

				bHigh &= 0x7A;
				bool bZero = false;

				if((bHigh & 0x40) == 0)
				{
					bZero = ((bHigh & 0x10) != 0 ? false : true);
					if(bZero)
					{
						bHigh |= 0x10;
					}
					else
					{
						bHigh &= (~0x10);
					}
				}

				if((bHigh & 0x02) == 0)
				{
					bZero = ((bHigh & 0x08) != 0 ? false : true);
					if(bZero)
					{
						bHigh |= 0x08;
					}
					else
					{
						bHigh &= (~0x08);
					}
				}

				static int s_nValueTable[] =
				{
					9, 2, 1, 3, 4, 7, 8, 5
				};
				static BYTE s_btMaskTable[] =
				{
					0x80, 0x40, 0x20, 0x10,
					0x08, 0x04, 0x02, 0x01
				};

				int nValue = 0;
				for(int i = 0; i < 8; ++i)
				{
					if((bHigh & s_btMaskTable[i]) != 0)
					{
						nValue += s_nValueTable[i];
					}
				}

				nValue = (int)bLow - nValue;
				nValue = GetItemUpgrade(wLevel);

				if(nValue >= 0 &&
					nValue <= 7)
				{
					nCurValue = nValue;
				}
			}
			else
			{
				nCurValue = 0;
			}

			if(nCurValue != 0)
			{
				m_nPriceCost = nCurValue * 15000;
				SetWindowInfo("将随机转移该装备精华");
			}
			else
			{
				SetWindowInfo("无法转移该装备精华");
			}
		}
	}
	else if(m_stItems[5].type == ITEM_NO)
	{
		//	forge stones
		int nCounter = 0;
		int nStoneIndex = 0;
		bool bValid = true;

		for(int i = 0; i < 5; ++i)
		{
			if(m_stItems[i].type != ITEM_NO)
			{
				if(m_stItems[i].type != ITEM_COST ||
					m_stItems[i].curse != 3 ||
					m_stItems[i].lucky != 1)
				{
					bValid = false;
				}
				else
				{
					++nCounter;
					nStoneIndex = i;
				}
			}
		}

		if(!bValid ||
			nCounter != 1)
		{
			SetWindowInfo("无法合成该精华石");
		}
		else
		{
			if(m_stItems[nStoneIndex].atkSpeed < GRID_MAX)
			{
				SetWindowInfo("20个精华石才可升级");
			}
			else
			{
				SetWindowInfo("精华石20个可以向上升级为1个高级精华石");

				m_nPriceCost = m_stItems[nStoneIndex].accuracy * 10000;
			}
		}
	}
}



void GameDecomposeDlg::SwitchToDecompose()
{
	m_bPage = PAGE_DECOMPOSE;
	SetWindowTitle("装备分解");

	//RemoveCommonButton(BTN_FORGE);
	//AddCommonButton(CBTN_NORMAL, BTN_DECOMPOSE, DLG_WIDTH / 2 - 74 / 2, DLG_HEIGHT - 60, "分解装备");
}

void GameDecomposeDlg::SwitchToForge()
{
	m_bPage = PAGE_FORGE;
	SetWindowTitle("装备锻造");

	//RemoveCommonButton(BTN_DECOMPOSE);
	//AddCommonButton(CBTN_NORMAL, BTN_FORGE, DLG_WIDTH / 2 - 74 / 2, DLG_HEIGHT - 60, "锻造");
}