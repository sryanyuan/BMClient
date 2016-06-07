#include "GameDlgBase.h"
#include "../GameScene/GameResourceManager.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../../CommonModule/ShareData.h"
#include "../GameScene/GameScene.h"
#include "../BackMir/BackMir.h"
#include "GameDisplayDlg.h"
#include "GameAttribDlg.h"
//////////////////////////////////////////////////////////////////////////
static const int s_nEquipOftX = 22;
static const int s_nEquipOftY = 72;
//////////////////////////////////////////////////////////////////////////
GameHumDlg2::GameHumDlg2()
{
	m_rcClient.left = WINDOW_WIDTH - (s_nEquipOftX * 2 +168);
	m_rcClient.right = WINDOW_WIDTH/* - (s_nEquipOftX * 2 +168)*/;
	m_rcClient.bottom = 234 + 72 + 50;
	SetWindowTitle("人物");
	SetWindowInfo(GamePlayer::GetInstance()->GetAttrib()->name);
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	CalcCloseButtonPosition();

	//AddCommonButton(CBTN_LEFTARROW, 0, 55, 320, NULL);
	//AddCommonButton(CBTN_RIGHTARROW, 1, 145, 320, NULL);
	AddCommonButton(CBTN_RIGHTARROW, 2, (RECT_WIDTH(m_rcClient) - 14) / 2, 320, "", 1);

	m_pParent = GameScene::sThis->GetMainOpt();
	m_nPage = HUM_EQUIP;

	ZeroMemory(m_ptItems, sizeof(m_ptItems));

	SET_POINT(m_ptItems[PLAYER_ITEM_BRACELAT1], 4, 124);
	SET_POINT(m_ptItems[PLAYER_ITEM_RING1], 4, 163);
	SET_POINT(m_ptItems[PLAYER_ITEM_NECKLACE], 130, 35);
	SET_POINT(m_ptItems[PLAYER_ITEM_MEDAL], 130, 73);
	SET_POINT(m_ptItems[PLAYER_ITEM_BRACELAT2], 130, 124);
	SET_POINT(m_ptItems[PLAYER_ITEM_RING2], 130, 163);
	SET_POINT(m_ptItems[PLAYER_ITEM_CHARM], 5, 203);
	SET_POINT(m_ptItems[PLAYER_ITEM_BELT], 45, 203);
	SET_POINT(m_ptItems[PLAYER_ITEM_SHOE], 89, 203);
	SET_POINT(m_ptItems[PLAYER_ITEM_GEM], 130, 203);

	//m_pAttribDlg = new GameAttribDlg;
	m_bShowAttribWhenOpen = false;
	SetVisible(false);
}

GameHumDlg2::~GameHumDlg2()
{
	SAFE_DELETE(m_pRender);
	//SAFE_DELETE(m_pAttribDlg);
}

//////////////////////////////////////////////////////////////////////////
void GameHumDlg2::Render()
{
	__super::Render();

	GameTextureManager* pMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	GameTextureManager* pItemsMgr = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	GameTextureManager* pStateMgr = GameResourceManager::GetInstance()->GetTexs(RES_STATE);
	HTEXTURE tex = 0;

	GamePlayer* pPlayer = GamePlayer::GetInstance();

	//	名字
	/*int nNameLength = strlen(pPlayer->GetAttrib()->name);
	if(nNameLength != 0)
	{
		int nDrawOffset = (m_rcClient.right - m_rcClient.left - GetTextWidth(pPlayer->GetAttrib()->name, 12)) / 2;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(m_rcClient.left + nDrawOffset, m_rcClient.top + 40, pPlayer->GetAttrib()->name);
	}*/

	if(m_nPage == HUM_EQUIP)
	{
		if(pPlayer->GetAttrib()->sex == 1 ||
			pPlayer->GetAttrib()->sex == 0)
		{
			tex = pMgr->GetTexture(TEX_HUMPAGE_MALE);
		}
		else if(pPlayer->GetAttrib()->sex == 2)
		{
			tex = pMgr->GetTexture(TEX_HUMPAGE_FEMALE);
		}

		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 168, 234);
			m_pRender->Render(RELATIVE_X(s_nEquipOftX), RELATIVE_Y(s_nEquipOftY));
		}
		
		SHORT sOftX = 0;
		SHORT sOftY = 0;
		if(pPlayer->GetAttrib()->sex == 1 ||
			pPlayer->GetAttrib()->sex == 0)
		{
			tex = pMgr->GetTexture(TEX_HUMHAIR_MALE);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, 16, 14);
				sOftX = pMgr->GetTextureOffsetX(TEX_HUMHAIR_MALE);
				sOftY = pMgr->GetTextureOffsetY(TEX_HUMHAIR_MALE);
				m_pRender->Render(RELATIVE_X(-7 + sOftX + s_nEquipOftX), RELATIVE_Y(45 + sOftY + s_nEquipOftY));
			}
		}
		else if(pPlayer->GetAttrib()->sex == 2)
		{
			tex = pMgr->GetTexture(TEX_HUMHAIR_FEMALE);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, 20, 24);
				sOftX = pMgr->GetTextureOffsetX(TEX_HUMHAIR_FEMALE);
				sOftY = pMgr->GetTextureOffsetY(TEX_HUMHAIR_FEMALE);
				m_pRender->Render(RELATIVE_X(-7 + sOftX + s_nEquipOftX), RELATIVE_Y(45 + sOftY + s_nEquipOftY));
			}
		}

		//	画装备
		ItemAttrib* pItem = NULL;
		int nTexWidth = 0;
		int nTexHeight = 0;
		int nDrawX = 0;
		int nDrawY = 0;
		int nOftX = 0;
		int nOftY = 0;

		static int s_nXOft = -7;
		static int s_nYOft = 45;

		//	衣服
		pItem = pPlayer->GetPlayerItem(PLAYER_ITEM_CLOTH);
		if(pItem->type != ITEM_NO)
		{
			tex = pStateMgr->GetTexture(pItem->tex);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				nTexWidth = pStateMgr->GetTextureWidth(pItem->tex);
				nTexHeight = pStateMgr->GetTextureHeight(pItem->tex);
				m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
				nOftX = pStateMgr->GetTextureOffsetX(pItem->tex);
				nOftY = pStateMgr->GetTextureOffsetY(pItem->tex);
				nDrawX = m_rcClient.left + nOftX + s_nEquipOftX;
				nDrawY = m_rcClient.top + nOftY + s_nEquipOftY;
				m_pRender->Render(nDrawX + s_nXOft, nDrawY + s_nYOft);
			}
		}
		//	头盔
		pItem = pPlayer->GetPlayerItem(PLAYER_ITEM_HELMET);
		if(pItem->type != ITEM_NO)
		{
			tex = pStateMgr->GetTexture(pItem->tex);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				nTexWidth = pStateMgr->GetTextureWidth(pItem->tex);
				nTexHeight = pStateMgr->GetTextureHeight(pItem->tex);
				m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
				nOftX = pStateMgr->GetTextureOffsetX(pItem->tex);
				nOftY = pStateMgr->GetTextureOffsetY(pItem->tex);
				nDrawX = m_rcClient.left + nOftX + s_nEquipOftX;
				nDrawY = m_rcClient.top + nOftY + s_nEquipOftY;
				m_pRender->Render(nDrawX + s_nXOft, nDrawY + s_nYOft);
			}
		}
		//	武器
		pItem = pPlayer->GetPlayerItem(PLAYER_ITEM_WEAPON);
		if(pItem->type != ITEM_NO)
		{
			tex = pStateMgr->GetTexture(pItem->tex);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				nTexWidth = pStateMgr->GetTextureWidth(pItem->tex);
				nTexHeight = pStateMgr->GetTextureHeight(pItem->tex);
				m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
				nOftX = pStateMgr->GetTextureOffsetX(pItem->tex);
				nOftY = pStateMgr->GetTextureOffsetY(pItem->tex);
				nDrawX = m_rcClient.left + nOftX + s_nEquipOftX;
				nDrawY = m_rcClient.top + nOftY + s_nEquipOftY;
				m_pRender->Render(nDrawX + s_nXOft, nDrawY + s_nYOft);
			}

			/*if(pPlayer->GetPlayerItem(PLAYER_ITEM_WEAPON)->id == 1880)
			{
				//	倚天剑
				static float fFlash = 0.0f;
				fFlash += AfxGetHge()->Timer_GetDelta();
				if(fFlash > 2.0f)
				{
					fFlash = 0.0f
				}

				int nTextIndex = (int)(fFlash / 0.2f);
				if(nTextIndex >= 0 &&
					nTextIndex < 10)
				{
					tex = pStateMgr->GetTexture(1890 + nTextIndex);
					if(tex)
					{
						MagicElement::pEffectRender->SetTexture(tex);
						nTexWidth = pStateMgr->GetTextureWidth(1890 + nTextIndex);
						nTexHeight = pStateMgr->GetTextureHeight(1890 + nTextIndex);
						MagicElement::pEffectRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
						nOftX = pStateMgr->GetTextureOffsetX(1890 + nTextIndex);
						nOftY = pStateMgr->GetTextureOffsetY(1890 + nTextIndex);
						nDrawX = m_rcClient.left + nOftX + s_nEquipOftX;
						nDrawY = m_rcClient.top + nOftY + s_nEquipOftY;
						MagicElement::pEffectRender->Render(nDrawX + s_nXOft, nDrawY + s_nYOft);
					}
				}
			}*/
		}

		for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
		{
			if(m_ptItems[i].x != m_ptItems[i].y)
			{
				pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)i);
				if(i == PLAYER_ITEM_CLOTH)
				{
					continue;
				}
				if(i == PLAYER_ITEM_HELMET)
				{
					continue;
				}
				if(i == PLAYER_ITEM_WEAPON)
				{
					continue;
				}
				if(pItem->type != ITEM_NO)
				{
					tex = pItemsMgr->GetTexture(pItem->tex);
					if(tex)
					{
						m_pRender->SetTexture(tex);
						nTexWidth = pItemsMgr->GetTextureWidth(pItem->tex);
						nTexHeight = pItemsMgr->GetTextureHeight(pItem->tex);
						m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
						nDrawX = (35 - nTexWidth) / 2 + m_rcClient.left + s_nEquipOftX + m_ptItems[i].x;
						nDrawY = (32 - nTexHeight) / 2 + m_rcClient.top + s_nEquipOftY + m_ptItems[i].y;
						m_pRender->Render(nDrawX, nDrawY);
					}
				}
			}
		}
	}
	else
	{
		//	属性
		//int nJob = pPlayer->GetAttrib()->price;
		int nJob = pPlayer->GetHeroJob();
		int nWanLi = pPlayer->GetWanLi();
		float fPreOffset = AfxGetPrinter()->GetKerningHeight();
		AfxGetPrinter()->SetKerningHeight(3.0f);
		char szAttrib[500];
		ZeroMemory(szAttrib, sizeof(szAttrib));
		sprintf(szAttrib + strlen(szAttrib), "职业    :  %s\n", pPlayer->GetHeroJobName());
		sprintf(szAttrib + strlen(szAttrib), "等级    :  %d\n", pPlayer->GetAttrib()->level);
		sprintf(szAttrib + strlen(szAttrib), "防御    :  %d - %d\n", pPlayer->GetAttrib()->AC, pPlayer->GetAttrib()->maxAC);
		sprintf(szAttrib + strlen(szAttrib), "魔御    :  %d - %d\n", pPlayer->GetAttrib()->MAC, pPlayer->GetAttrib()->maxMAC);
		sprintf(szAttrib + strlen(szAttrib), "攻击    :  %d - %d\n", pPlayer->GetAttrib()->DC, pPlayer->GetAttrib()->maxDC);
		sprintf(szAttrib + strlen(szAttrib), "魔法    :  %d - %d\n", pPlayer->GetAttrib()->MC, pPlayer->GetAttrib()->maxMC);
		sprintf(szAttrib + strlen(szAttrib), "道术    :  %d - %d\n", pPlayer->GetAttrib()->SC, pPlayer->GetAttrib()->maxSC);
		//sprintf(szAttrib + strlen(szAttrib), "魔法    :  %d - %d\n", pPlayer->GetAttrib()->MC, pPlayer->GetAttrib()->maxMC);
		sprintf(szAttrib + strlen(szAttrib), "躲避    :  +%d\n", pPlayer->GetAttrib()->hide);
		sprintf(szAttrib + strlen(szAttrib), "准确    :  +%d\n", pPlayer->GetAttrib()->accuracy);
		sprintf(szAttrib + strlen(szAttrib), "幸运    :  +%d\n", pPlayer->GetAttrib()->lucky);
		sprintf(szAttrib + strlen(szAttrib), "攻速    :  +%d\n", pPlayer->GetAttrib()->atkSpeed);
		//sprintf(szAttrib + strlen(szAttrib), "麻痹    :  +%d\n", pPlayer->GetAttrib()->atkPalsy);
		//sprintf(szAttrib + strlen(szAttrib), "中毒    :  +%d\n", pPlayer->GetAttrib()->atkPois);
		sprintf(szAttrib + strlen(szAttrib), "腕力    :  %d\n", nWanLi);
		sprintf(szAttrib + strlen(szAttrib), "移动速度:  +%d\n", pPlayer->GetAttrib()->moveSpeed);

		//m_pFont->SetColor(ARGB_WHITE);
		//m_pFont->Print(RELATIVE_X(50), RELATIVE_Y(100), szAttrib);
		//m_pFont->SetKerningHeight(fPreOffset);
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(RELATIVE_X(50), RELATIVE_Y(85), szAttrib);
		AfxGetPrinter()->SetKerningHeight(fPreOffset);
	}
}

void GameHumDlg2::OnMove(int _nOftX, int _nOftY)
{
	GameAttribDlg* pAttribDlg = GameMainOptUI::GetInstance()->GetAttribDlg();
	bool bVisible = pAttribDlg->IsVisible();

	if(bVisible)
	{
		pAttribDlg->MoveTo(m_rcClient.right, m_rcClient.top);
	}
}

bool GameHumDlg2::OnCommonButtonClick(int _id)
{
	if(_id == 0)
	{
		OnPre();
	}
	else if(_id == 1)
	{
		OnNext();
	}
	else if(_id == 2)
	{
		GameAttribDlg* pAttribDlg = GameMainOptUI::GetInstance()->GetAttribDlg();
		bool bVisible = pAttribDlg->IsVisible();

		if(bVisible)
		{
			pAttribDlg->CloseDialog();

			GetCommonButtonData(2)->nType = CBTN_RIGHTARROW;
		}
		else
		{
			GetCommonButtonData(2)->nType = CBTN_LEFTARROW;

			pAttribDlg->ShowDialog();
			OnMove(0, 0);

			if(pAttribDlg->GetRect().right > WINDOW_WIDTH)
			{
				int nOft = WINDOW_WIDTH - pAttribDlg->GetRect().right;

				Move(nOft, 0);
			}
		}
	}
	return true;
}

void GameHumDlg2::OnPre()
{
	--m_nPage;
	if(m_nPage < 0)
	{
		m_nPage = 1;
	}
}

void GameHumDlg2::OnNext()
{
	++m_nPage;
	if(m_nPage > 1)
	{
		m_nPage = 0;
	}
}

void GameHumDlg2::OnHumPage(const POINT& _mp)
{
	POINT ptDetect = _mp;
	ptDetect.x -= s_nEquipOftX;
	ptDetect.y -= s_nEquipOftY;
	ptDetect.x -= m_rcClient.left;
	ptDetect.y -= m_rcClient.top;

	PLAYER_ITEM_TYPE eItem = PLAYER_ITEM_TOTAL;

	if(ptDetect.x >= 4 &&
		ptDetect.x <= 39 &&
		ptDetect.y >= 124 &&
		ptDetect.y <= 194)
	{
		//	第一列
		if(ptDetect.y >= 124 &&
			ptDetect.y <= 155)
		{
			//	手镯1
			eItem = PLAYER_ITEM_BRACELAT1;
		}
		else if(ptDetect.y >= 163 &&
			ptDetect.y <= 194)
		{
			eItem = PLAYER_ITEM_RING1;
		}
	}
	else if(ptDetect.x >= 130 &&
		ptDetect.x <= 165 &&
		ptDetect.y >= 35 &&
		ptDetect.y <= 194)
	{
		if(ptDetect.y >= 35 &&
			ptDetect.y <= 66)
		{
			eItem = PLAYER_ITEM_NECKLACE;
		}
		else if(ptDetect.y >= 73 &&
			ptDetect.y <= 104)
		{
			eItem = PLAYER_ITEM_MEDAL;
		}
		else if(ptDetect.y >= 124 &&
			ptDetect.y <= 155)
		{
			eItem = PLAYER_ITEM_BRACELAT2;
		}
		else if(ptDetect.y >= 163 &&
			ptDetect.y <= 194)
		{
			eItem = PLAYER_ITEM_RING2;
		}
	}
	else if(ptDetect.x >= 66 &&
		ptDetect.x <= 104 &&
		ptDetect.y >= 64 &&
		ptDetect.y <= 190)
	{
		eItem = PLAYER_ITEM_CLOTH;
	}
	else if(ptDetect.x >= 72 &&
		ptDetect.x <= 93 &&
		ptDetect.y >= 44 &&
		ptDetect.y <= 56)
	{
		eItem = PLAYER_ITEM_HELMET;
	}
	else if(ptDetect.x >= 17 &&
		ptDetect.x <= 41 &&
		ptDetect.y >= 36 &&
		ptDetect.y <= 114)
	{
		eItem = PLAYER_ITEM_WEAPON;
	}
	else if(ptDetect.y >= 203 &&
		ptDetect.y <= 232)
	{
		if(ptDetect.x >= 5 &&
			ptDetect.x <= 39)
		{
			eItem = PLAYER_ITEM_CHARM;
		}
		else if(ptDetect.x >= 47 &&
			ptDetect.x <= 81)
		{
			eItem = PLAYER_ITEM_BELT;
		}
		else if(ptDetect.x >= 89 &&
			ptDetect.x <= 123)
		{
			eItem = PLAYER_ITEM_SHOE;
		}
		else if(ptDetect.x >= 130 &&
			ptDetect.x <= 164)
		{
			eItem = PLAYER_ITEM_GEM;
		}
	}

	if(eItem != PLAYER_ITEM_TOTAL)
	{
		OnItem(eItem);
	}
	else
	{
		m_pParent->GetIntroDlg()->ShowNothing();
	}
}

bool GameHumDlg2::ProcUserCmd(const POINT &_mp)
{
	__super::ProcUserCmd(_mp);

	//m_pParent->GetIntroDlg()->ShowNothing();
	if(m_nPage == HUM_EQUIP)
	{
		OnHumPage(_mp);
	}
	return true;
}

void GameHumDlg2::OnItem(PLAYER_ITEM_TYPE _type)
{
	PLAYER_ITEM_TYPE _itemrc = _type;
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	ItemAttrib* pItem = NULL;
	pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)(_itemrc));
	bool bCanDress = false;

	if(pItem->type != ITEM_NO)
	{
		//	鼠标所在格子不为空
		//	右键收回背包
		m_pParent->GetIntroDlg()->ShowItemAttrib(pItem);
		m_pParent->GetDisplayDlg()->ShowItemAttrib(pItem);

		if(AfxGetHge()->Input_KeyUp(HGEK_RBUTTON))
		{
			//	收回背包
#ifdef _NET_GAME_
			PkgPlayerUndressItemReq req;
			req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
			req.dwTag = pItem->tag;
			req.bPos = (BYTE)_itemrc;
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
#else
			pPlayer->UnDressEquip(_itemrc);
#endif
		}
		else if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			//	左键交换装备
			if(m_pParent->GetBagDlg2()->IsItemSel())
			{
#ifdef _NET_GAME_
				ItemAttrib* pSelItem = m_pParent->GetBagDlg2()->GetSelItem();

				bCanDress = false;
				if(pSelItem->type == ITEM_WEAPON &&
					_itemrc == PLAYER_ITEM_WEAPON)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_CLOTH &&
					_itemrc == PLAYER_ITEM_CLOTH)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_NECKLACE &&
					_itemrc == PLAYER_ITEM_NECKLACE)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_RING &&
					(_itemrc == PLAYER_ITEM_RING1 || _itemrc == PLAYER_ITEM_RING2))
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_BRACELAT &&
					(_itemrc == PLAYER_ITEM_BRACELAT1 || _itemrc == PLAYER_ITEM_BRACELAT2))
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_HELMET &&
					_itemrc == PLAYER_ITEM_HELMET)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_MEDAL &&
					_itemrc == PLAYER_ITEM_MEDAL)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_SHOE &&
					_itemrc == PLAYER_ITEM_SHOE)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_GEM &&
					_itemrc == PLAYER_ITEM_GEM)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_BELT &&
					_itemrc == PLAYER_ITEM_BELT)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_CHARM &&
					_itemrc == PLAYER_ITEM_CHARM)
				{
					bCanDress = true;
				}

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
				/*if(pSelItem->type == ITEM_WEAPON ||
					pSelItem->type == ITEM_CLOTH ||
					pSelItem->type == ITEM_NECKLACE ||
					pSelItem->type == ITEM_RING ||
					pSelItem->type == ITEM_BRACELAT ||
					pSelItem->type == ITEM_HELMET ||
					pSelItem->type == ITEM_MEDAL)*/
				if(bCanDress)
				{
					PkgPlayerDressItemReq req;
					req.bPos = (BYTE)_itemrc;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwTag = pSelItem->tag;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}

				if(bCanDress)
				{
					pTheGame->PlayItemPutDownSound(pSelItem);
				}
#else
				DWORD dwErr = 0;
				if(pPlayer->DressEquip(m_pParent->GetBagDlg2()->GetSelIndex(),_itemrc, &dwErr))
				{
					m_pParent->GetBagDlg2()->CancelSel();
				}
#endif
			}
		}
	}
	else
	{
		//	鼠标所在格子为空
		m_pParent->GetIntroDlg()->ShowNothing();

		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			if(m_pParent->GetBagDlg2()->IsItemSel())
			{
#ifdef _NET_GAME_
				ItemAttrib* pSelItem = m_pParent->GetBagDlg2()->GetSelItem();

				bCanDress = false;
				if(pSelItem->type == ITEM_WEAPON &&
					_itemrc == PLAYER_ITEM_WEAPON)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_CLOTH &&
					_itemrc == PLAYER_ITEM_CLOTH)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_NECKLACE &&
					_itemrc == PLAYER_ITEM_NECKLACE)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_RING &&
					(_itemrc == PLAYER_ITEM_RING1 || _itemrc == PLAYER_ITEM_RING2))
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_BRACELAT &&
					(_itemrc == PLAYER_ITEM_BRACELAT1 || _itemrc == PLAYER_ITEM_BRACELAT2))
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_HELMET &&
					_itemrc == PLAYER_ITEM_HELMET)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_MEDAL &&
					_itemrc == PLAYER_ITEM_MEDAL)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_SHOE &&
					_itemrc == PLAYER_ITEM_SHOE)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_GEM &&
					_itemrc == PLAYER_ITEM_GEM)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_BELT &&
					_itemrc == PLAYER_ITEM_BELT)
				{
					bCanDress = true;
				}
				else if(pSelItem->type == ITEM_CHARM &&
					_itemrc == PLAYER_ITEM_CHARM)
				{
					bCanDress = true;
				}

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

				/*if(pSelItem->type == ITEM_WEAPON ||
					pSelItem->type == ITEM_CLOTH ||
					pSelItem->type == ITEM_NECKLACE ||
					pSelItem->type == ITEM_RING ||
					pSelItem->type == ITEM_BRACELAT ||
					pSelItem->type == ITEM_HELMET ||
					pSelItem->type == ITEM_MEDAL)*/
				if(bCanDress)
				{
					PkgPlayerDressItemReq req;
					req.bPos = (BYTE)_itemrc;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwTag = pSelItem->tag;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}

				if(bCanDress)
				{
					pTheGame->PlayItemPutDownSound(pSelItem);
				}
#else
				//	有选中物品
				DWORD dwErr = 0;
				ItemAttrib* pSelItem = m_pParent->GetBagDlg2()->GetSelItem();
				if(pPlayer->DressEquip(m_pParent->GetBagDlg2()->GetSelIndex(), _itemrc, &dwErr))
				{
					m_pParent->GetBagDlg2()->CancelSel();
				}
#endif
			}
		}
	}
}

bool GameHumDlg2::OnShowDialog()
{
	SetNeedTopLevel(true);
	GameAttribDlg* pAttribDlg = GameMainOptUI::GetInstance()->GetAttribDlg();

	if(m_bShowAttribWhenOpen)
	{
		pAttribDlg->ShowDialog();
		m_bShowAttribWhenOpen = false;
	}

	bool bVisible = pAttribDlg->IsVisible();
	if(bVisible)
	{
		OnMove(0, 0);
	}
	return true;
}

bool GameHumDlg2::OnCloseDialog()
{
	GameAttribDlg* pAttribDlg = GameMainOptUI::GetInstance()->GetAttribDlg();
	if(pAttribDlg->IsVisible())
	{
		m_bShowAttribWhenOpen = true;
	}
	pAttribDlg->CloseDialog();

	return __super::OnCloseDialog();
}