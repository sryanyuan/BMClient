#include "../GameDialog/GameDlgBase.h"
#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameResourceManager.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../BackMir/BackMir.h"

//extern ByteBuffer g_xBuffer;

GameHumDlg::GameHumDlg()
{
	ZeroMemory(m_rcOther, sizeof(m_rcOther));
	//m_pSpr = NULL;
	m_pResourceMangager = NULL;
	m_bVisible = false;
	m_page = PAGE_HUM;
	//m_pFont = NULL;

// 	m_rcClient.right = 268;
// 	m_rcClient.bottom = 390;
	m_rcClient.left = VIEW_WIDTH - 268;
	m_rcClient.right = VIEW_WIDTH;
	m_rcClient.bottom = 390;
	UpdateRect();
	INIT_CLOSEBTN(240, 16);

	m_pRender = NEW_EMPTY_SPRITE;

	AddCommonButton(CBTN_LEFTARROW, 0, 83, 353, NULL);
	AddCommonButton(CBTN_RIGHTARROW, 1, 176, 353, NULL);
}

GameHumDlg::~GameHumDlg()
{
	//delete m_pSpr;
	//delete m_pFont;
	SAFE_DELETE(m_pRender);
}

bool GameHumDlg::IsCaptionBar(int _x, int _y)
{
	if(_x >= RELATIVE_X(0) &&
		_x <= RELATIVE_X(268) &&
		_y >= RELATIVE_Y(9) &&
		_y <= RELATIVE_Y(42))
	{
		/*if(_x > RELATIVE_X(240))
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

void GameHumDlg::UpdateRect()
{
	m_rcOther[RECT_PRE].left = RELATIVE_X(83);
	m_rcOther[RECT_PRE].right = m_rcOther[RECT_PRE].left + 10;
	m_rcOther[RECT_PRE].top = RELATIVE_Y(355);
	m_rcOther[RECT_PRE].bottom = m_rcOther[RECT_PRE].top + 12;

	m_rcOther[RECT_NEXT].left = RELATIVE_X(177);
	m_rcOther[RECT_NEXT].right = m_rcOther[RECT_NEXT].left + 10;
	m_rcOther[RECT_NEXT].top = RELATIVE_Y(355);
	m_rcOther[RECT_NEXT].bottom = m_rcOther[RECT_NEXT].top + 12;
}

void GameHumDlg::Init(hgeResourceManager* _res, GameMainOptUI* _parent)
{
	m_pResourceMangager = _res;
	m_pParent = _parent;
	//m_pFont = new GfxFont("宋体", 12, 0, 0, 0);
}


void GameHumDlg::Update(float _dt)
{

}

void GameHumDlg::Render()
{
	if(!m_bVisible ||
		!m_pResourceMangager)
	{
		return;
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();

	HTEXTURE tex = 0;
	//if(pPlayer->GetPlayerSex() == PSX_MAN)
	if(pPlayer->GetAttrib()->sex == 1)
	{
		tex = m_pResourceMangager->GetTexture("hum_nan");
	}
	else
	{
		tex = m_pResourceMangager->GetTexture("hum_nv");
	}
	if(tex)
	{
		INITSPRITE(m_pRender, tex);
		switch(m_page)
		{
		case PAGE_HUM:
			{
				m_pRender->SetTextureRect(0, 0, 268, 390);
			}break;
		case PAGE_ATTRIB:
			{
				m_pRender->SetTextureRect(268, 0, 268, 390);
			}
		}
		m_pRender->Render(m_rcClient.left, m_rcClient.top);
	}

	//	名字
	int nNameLength = strlen(pPlayer->GetAttrib()->name);
	if(nNameLength != 0)
	{
		int nDrawOffset = 93 - nNameLength / 2 * 8;
		nDrawOffset /= 2;
		//m_pFont->SetColor(ARGB_WHITE);
		//m_pFont->Print(m_rcClient.left + nDrawOffset + 87, m_rcClient.top + 62, pPlayer->GetAttrib()->name);
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(m_rcClient.left + nDrawOffset + 87, m_rcClient.top + 62, pPlayer->GetAttrib()->name);
	}

	if(m_page == PAGE_HUM)
	{
		//	画装备
		ItemAttrib* pItem = NULL;
		GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
		int nDrawOffsetX = 0;
		int nDrawOffsetY = 0;
		static const int nDrawTableX[] = {37, 37, 196, 196, 37, 196, 37, 196, 196};
		static const int nDrawTableY[] = {154, 203, 105, 154, 252, 252, 301, 301, 203};

		if(ttex)
		{
			for(int i = PLAYER_ITEM_WEAPON; i < PLAYER_ITEM_TOTAL; ++i)
			{
				pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)(i));
				if(pItem->type != ITEM_NO)
				{
					//	有装备 则画
					tex = ttex->GetTexture(pItem->tex);
					if(tex)
					{
// 						nDrawOffsetX = ttex->GetTextureWidth(tex);
// 						nDrawOffsetY = ttex->GetTextureHeight(tex);
						nDrawOffsetX = ttex->GetTextureWidth(pItem->tex);
						nDrawOffsetY = ttex->GetTextureHeight(pItem->tex);
						m_pRender->SetTexture(tex);
						m_pRender->SetTextureRect(0, 0, nDrawOffsetX, nDrawOffsetY);
						nDrawOffsetX = abs(nDrawOffsetX - 36) / 2;
						nDrawOffsetY = abs(nDrawOffsetY - 36) / 2;
						m_pRender->Render(RELATIVE_X(nDrawTableX[i - PLAYER_ITEM_WEAPON] + nDrawOffsetX),
							RELATIVE_Y(nDrawTableY[i - PLAYER_ITEM_WEAPON] + nDrawOffsetY));
					}
				}
			}
		}
	}
	else
	{
		//	画属性
		//float fPreOffset = m_pFont->GetKerningHeight();
		float fPreOffset = AfxGetPrinter()->GetKerningHeight();
		//m_pFont->SetKerningHeight(2.0f);
		AfxGetPrinter()->SetKerningHeight(2.0f);
		char szAttrib[500];
		ZeroMemory(szAttrib, sizeof(szAttrib));
		sprintf(szAttrib + strlen(szAttrib), "职业    :  %s\n", pPlayer->GetHeroJobName());
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
		sprintf(szAttrib + strlen(szAttrib), "麻痹    :  +%d\n", pPlayer->GetAttrib()->atkPalsy);
		//sprintf(szAttrib + strlen(szAttrib), "中毒    :  +%d\n", pPlayer->GetAttrib()->atkPois);
		sprintf(szAttrib + strlen(szAttrib), "移动速度:  +%d\n", pPlayer->GetAttrib()->moveSpeed);

		//m_pFont->SetColor(ARGB_WHITE);
		//m_pFont->Print(RELATIVE_X(50), RELATIVE_Y(100), szAttrib);
		//m_pFont->SetKerningHeight(fPreOffset);
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(RELATIVE_X(50), RELATIVE_Y(100), szAttrib);
		AfxGetPrinter()->SetKerningHeight(fPreOffset);
	}

	__super::Render();
}

bool GameHumDlg::ProcUserCmd(const POINT& _mp)
{
	POINT ptMouse = _mp;

	__super::ProcUserCmd(_mp);

	m_pParent->GetIntroDlg()->ShowNothing();
	UpdateRect();

	for(int i = RECT_NEXT; i <= RECT_PRE; ++i)
	{
		if(::PtInRect(&m_rcOther[i], ptMouse))
		{
			//	Do
			switch(i)
			{
			case RECT_NEXT:
				{
					//OnNext();
				}break;
			case RECT_PRE:
				{
					//OnPre();
				}break;
			}
			break;
		}
	}

	switch (m_page)
	{
	case PAGE_HUM:
		{
			OnHumPage(ptMouse);
		}break;
	case PAGE_ATTRIB:
		{
			OnAttribPage(ptMouse);
		}break;
	}
	return true;
}

void GameHumDlg::OnNext()
{
	//if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		int nPage = m_page;
		++nPage;
		if(nPage >= PAGE_TOTAL)
		{
			nPage = 0;
		}
		m_page = (HUM_PAGE)nPage;
	}
}

void GameHumDlg::OnPre()
{
	//if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		int nPage = m_page;
		--nPage;
		if(nPage < 0)
		{
			nPage = PAGE_TOTAL - 1;
		}
		m_page = (HUM_PAGE)nPage;
	}
}

void GameHumDlg::OnHumPage(POINT& _pt)
{
	POINT ptDetect = _pt;
	int nPoint = -1;

	if(ptDetect.x >= RELATIVE_X(37) &&
		ptDetect.x <= RELATIVE_X(37 + 36))
	{
		//	左侧
		if(ptDetect.y >= RELATIVE_Y(154) &&
			ptDetect.y <= RELATIVE_Y(301 + 36))
		{
			//	左侧范围内
			ptDetect.y -= RELATIVE_Y(154);
			nPoint = ptDetect.y / 49;
			ptDetect.y -= (nPoint * 49);
			if(ptDetect.y >= 0 &&
				ptDetect.y <= 36)
			{
				switch (nPoint)
				{
				case 0:
					{
						nPoint = PLAYER_ITEM_WEAPON;
					}break;
				case 1:
					{
						nPoint = PLAYER_ITEM_CLOTH;
					}break;
				case 2:
					{
						nPoint = PLAYER_ITEM_BRACELAT1;
					}break;
				case 3:
					{
						nPoint = PLAYER_ITEM_RING1;
					}break;
				default:
					{
						nPoint = -1;
					}break;
				}

				if(nPoint != - 1)
				{
					OnItem((PLAYER_ITEM_TYPE)nPoint);
					return;
				}
			}
		}
	}
	else if(ptDetect.x >= RELATIVE_X(196) &&
		ptDetect.x <= RELATIVE_X(196 + 36))
	{
		//	右侧
		if(ptDetect.y >= RELATIVE_Y(105) &&
			ptDetect.y <= RELATIVE_Y(301 + 36))
		{
			//	右侧范围内
			ptDetect.y -= RELATIVE_Y(105);
			nPoint = ptDetect.y / 49;
			ptDetect.y -= (nPoint * 49);
			if(ptDetect.y >= 0 &&
				ptDetect.y <= 36)
			{
				switch (nPoint)
				{
				case 0:
					{
						nPoint = PLAYER_ITEM_HELMET;
					}break;
				case 1:
					{
						nPoint = PLAYER_ITEM_NECKLACE;
					}break;
				case 2:
					{
						nPoint = PLAYER_ITEM_MEDAL;
					}break;
				case 3:
					{
						nPoint = PLAYER_ITEM_BRACELAT2;
					}break;
				case 4:
					{
						nPoint = PLAYER_ITEM_RING2;
					}break;
				default:
					{
						nPoint = -1;
					}break;
				}

				if(nPoint != - 1)
				{
					OnItem((PLAYER_ITEM_TYPE)nPoint);
					return;
				}
			}
		}
	}
}

void GameHumDlg::OnAttribPage(POINT& _pt)
{

}

void GameHumDlg::OnItem(PLAYER_ITEM_TYPE _itemrc)
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	ItemAttrib* pItem = NULL;
	pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)(_itemrc));
	bool bCanDress = false;

	if(pItem->type != ITEM_NO)
	{
		//	鼠标所在格子不为空
		//	右键收回背包
		m_pParent->GetIntroDlg()->ShowItemAttrib(pItem);
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
			if(m_pParent->GetBagDlg()->IsItemSel())
			{
#ifdef _NET_GAME_
				ItemAttrib* pSelItem = m_pParent->GetBagDlg()->GetSelItem();

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
#else
				DWORD dwErr = 0;
				if(pPlayer->DressEquip(m_pParent->GetBagDlg()->GetSelIndex(),_itemrc, &dwErr))
				{
					m_pParent->GetBagDlg()->CancelSel();
				}
#endif
			}
		}
	}
	else
	{
		//	鼠标所在格子为空
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			if(m_pParent->GetBagDlg()->IsItemSel())
			{
#ifdef _NET_GAME_
				ItemAttrib* pSelItem = m_pParent->GetBagDlg()->GetSelItem();

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
#else
				//	有选中物品
				DWORD dwErr = 0;
				ItemAttrib* pSelItem = m_pParent->GetBagDlg()->GetSelItem();
				if(pPlayer->DressEquip(m_pParent->GetBagDlg()->GetSelIndex(), _itemrc, &dwErr))
				{
					m_pParent->GetBagDlg()->CancelSel();
				}
#endif
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool GameHumDlg::OnCommonButtonClick(int _id)
{
	if(_id == 0)
	{
		OnPre();
	}
	else if(_id == 1)
	{
		OnNext();
	}
	return true;
}