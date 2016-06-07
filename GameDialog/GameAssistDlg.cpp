#include "GameAssistDlg.h"
#include "../BackMir/BackMir.h"
#include "../GameScene/GameResourceManager.h"
#include "GameDisplayDlg.h"
//////////////////////////////////////////////////////////////////////////
const int g_nAccOftX = 12;
const int g_nAccOftY = 3;
//////////////////////////////////////////////////////////////////////////
GameAssistDlg::GameAssistDlg()
{
	m_bHoriz = true;
	m_bMaxSize = true;
	SetVisible(true);

	RECT* pRc = &m_rcClient;
	ZeroMemory(pRc, sizeof(RECT));

	pRc->left = 130;
	pRc->right = pRc->left + 240;
	pRc->top = 450 + 50 - 40;
	pRc->bottom = pRc->top + 38;

	m_pRender = NEW_EMPTY_SPRITE;
	ZeroMemory(m_bBtnState, sizeof(m_bBtnState));
	ZeroMemory(m_rcBtnRect, sizeof(m_rcBtnRect));

	pRc = &m_rcBtnRect[RC_SHOWHIDE];
	pRc->left = 0;
	pRc->right = pRc->left + 8;
	pRc->top = 0;
	pRc->bottom = pRc->top + 38;

	pRc = &m_rcBtnRect[RC_TRANSFORM];
	pRc->left = 220;
	pRc->right = pRc->left + 16;
	pRc->top = 2;
	pRc->bottom = pRc->top + 16;
}

void GameAssistDlg::TransformVerticalToHoriz()
{
	RECT* pRc = &m_rcClient;

	if(!m_bHoriz)
	{
		m_bHoriz = true;
		pRc->right = pRc->left + 240;
		pRc->bottom = pRc->top + 38;

		pRc = &m_rcBtnRect[RC_SHOWHIDE];
		pRc->right = pRc->left + 8;
		pRc->bottom = pRc->top + 38;

		pRc = &m_rcBtnRect[RC_TRANSFORM];
		pRc->left = 220;
		pRc->right = pRc->left + 16;
		pRc->top = 2;
		pRc->bottom = pRc->top + 16;
	}
}

void GameAssistDlg::TransformHorizToVertical()
{
	RECT* pRc = &m_rcClient;

	if(m_bHoriz)
	{
		m_bHoriz = false;
		pRc->right = pRc->left + 40;
		pRc->bottom = pRc->top + 241;

		pRc = &m_rcBtnRect[RC_SHOWHIDE];
		pRc->right = pRc->left + 40;
		pRc->bottom = pRc->top + 10;

		pRc = &m_rcBtnRect[RC_TRANSFORM];
		pRc->left = 19;
		pRc->right = pRc->left + 16;
		pRc->top = 222;
		pRc->bottom = pRc->top + 16;
	}
}

void GameAssistDlg::Render()
{
	GameTextureManager* pOpUI = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	if(NULL == pOpUI)
	{
		return;
	}

	HTEXTURE tex = 0;

	if(m_bHoriz)
	{
		//	Horiz
		if(m_bMaxSize)
		{
			tex = pOpUI->GetTexture(TEX_ASSISTMAIN_HORIZ);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, pOpUI->GetTextureWidth(TEX_ASSISTMAIN_HORIZ), pOpUI->GetTextureHeight(TEX_ASSISTMAIN_HORIZ));
				m_pRender->Render(m_rcClient.left, m_rcClient.top);
			}

			if(BTN_STATE_NORMAL != m_bBtnState[RC_SHOWHIDE])
			{
				tex = pOpUI->GetTexture(TEX_ASSISTSHOWHIDE_HORIZ + m_bBtnState[RC_SHOWHIDE]);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, pOpUI->GetTextureWidth(TEX_ASSISTSHOWHIDE_HORIZ + m_bBtnState[RC_SHOWHIDE]), pOpUI->GetTextureHeight(TEX_ASSISTSHOWHIDE_HORIZ + m_bBtnState[RC_SHOWHIDE]));
					m_pRender->Render(RELATIVE_X(m_rcBtnRect[RC_SHOWHIDE].left + 1), RELATIVE_Y(m_rcBtnRect[RC_SHOWHIDE].top));
				}
			}

			if(BTN_STATE_NORMAL != m_bBtnState[RC_TRANSFORM])
			{
				tex = pOpUI->GetTexture(TEX_ASSISTTRANSFORM + m_bBtnState[RC_TRANSFORM]);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, pOpUI->GetTextureWidth(TEX_ASSISTTRANSFORM + m_bBtnState[RC_TRANSFORM]), pOpUI->GetTextureHeight(TEX_ASSISTTRANSFORM + m_bBtnState[RC_TRANSFORM]));
					m_pRender->Render(RELATIVE_X(m_rcBtnRect[RC_TRANSFORM].left + 1), RELATIVE_Y(m_rcBtnRect[RC_TRANSFORM].top));
				}
			}

			//	Draw Items
			{
				ItemList& assistItems = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList();
				GameTextureManager* pItemTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
				int nDrawX = 0;
				int nDrawY = 0;

				if(NULL != pItemTexMgr)
				{
					for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
					{
						if(assistItems[i].type == ITEM_COST)
						{
							tex = pItemTexMgr->GetTexture(assistItems[i].tex);
							if(0 != tex)
							{
								m_pRender->SetTexture(tex);
								m_pRender->SetTextureRect(0, 0, pItemTexMgr->GetTextureWidth(assistItems[i].tex),
									pItemTexMgr->GetTextureHeight(assistItems[i].tex));

								nDrawX = m_rcClient.left + g_nAccOftX + i * 35;
								nDrawX += 32 / 2 - pItemTexMgr->GetTextureWidth(assistItems[i].tex) / 2;
								nDrawY = m_rcClient.top + g_nAccOftY;
								nDrawY += 32 / 2 - pItemTexMgr->GetTextureHeight(assistItems[i].tex) / 2;

								m_pRender->Render(nDrawX, nDrawY);

								nDrawX = m_rcClient.left + g_nAccOftX + i * 35;
								nDrawY = m_rcClient.top + g_nAccOftY + 20;
								AfxGetPrinter()->SetColor(ARGB_WHITE);
								AfxGetPrinter()->Print(nDrawX, nDrawY, "%d", assistItems[i].atkSpeed);
							}

							if(assistItems[i].hide != 0)
							{
								int nPercent = GamePlayer::GetInstance()->GetDrugCoolDownController()->GetCoolPercent(assistItems[i].id);
								if(nPercent != 0)
								{
									hgeResourceManager* pResMgr = pTheGame->GetGameRes();

									tex = pResMgr->GetTexture("cooldown");
									if(tex != 0)
									{
										m_pRender->SetTexture(tex);
										m_pRender->SetTextureRect(nPercent % 8 * 32, nPercent / 8 * 32, 32, 32);
										m_pRender->Render(m_rcClient.left + g_nAccOftX + i * 35,
											m_rcClient.top + g_nAccOftY);
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			tex = pOpUI->GetTexture(TEX_ASSISTSHOWHIDE_HORIZ + m_bBtnState[RC_SHOWHIDE]);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, pOpUI->GetTextureWidth(TEX_ASSISTSHOWHIDE_HORIZ + m_bBtnState[RC_SHOWHIDE]), pOpUI->GetTextureHeight(TEX_ASSISTSHOWHIDE_HORIZ + m_bBtnState[RC_SHOWHIDE]));
				m_pRender->Render(RELATIVE_X(m_rcBtnRect[RC_SHOWHIDE].left + 1), RELATIVE_Y(m_rcBtnRect[RC_SHOWHIDE].top));
			}
		}
	}
	else
	{
		//	Vertical
		if(m_bMaxSize)
		{
			tex = pOpUI->GetTexture(TEX_ASSISTMAIN_VERTICAL);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, pOpUI->GetTextureWidth(TEX_ASSISTMAIN_VERTICAL), pOpUI->GetTextureHeight(TEX_ASSISTMAIN_VERTICAL));
				m_pRender->Render(m_rcClient.left, m_rcClient.top);
			}

			if(BTN_STATE_NORMAL != m_bBtnState[RC_SHOWHIDE])
			{
				tex = pOpUI->GetTexture(TEX_ASSISTSHOWHIDE_VERTICAL + m_bBtnState[RC_SHOWHIDE]);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, pOpUI->GetTextureWidth(TEX_ASSISTSHOWHIDE_VERTICAL + m_bBtnState[RC_SHOWHIDE]), pOpUI->GetTextureHeight(TEX_ASSISTSHOWHIDE_VERTICAL + m_bBtnState[RC_SHOWHIDE]));
					m_pRender->Render(RELATIVE_X(m_rcBtnRect[RC_SHOWHIDE].left), RELATIVE_Y(m_rcBtnRect[RC_SHOWHIDE].top));
				}
			}

			if(BTN_STATE_NORMAL != m_bBtnState[RC_TRANSFORM])
			{
				tex = pOpUI->GetTexture(TEX_ASSISTTRANSFORM + m_bBtnState[RC_TRANSFORM]);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, pOpUI->GetTextureWidth(TEX_ASSISTTRANSFORM + m_bBtnState[RC_TRANSFORM]), pOpUI->GetTextureHeight(TEX_ASSISTTRANSFORM + m_bBtnState[RC_TRANSFORM]));
					m_pRender->Render(RELATIVE_X(m_rcBtnRect[RC_TRANSFORM].left), RELATIVE_Y(m_rcBtnRect[RC_TRANSFORM].top));
				}
			}

			//	Draw Items
			{
				ItemList& assistItems = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList();
				GameTextureManager* pItemTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
				int nDrawX = 0;
				int nDrawY = 0;

				if(NULL != pItemTexMgr)
				{
					for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
					{
						if(assistItems[i].type == ITEM_COST)
						{
							tex = pItemTexMgr->GetTexture(assistItems[i].tex);
							if(0 != tex)
							{
								m_pRender->SetTexture(tex);
								m_pRender->SetTextureRect(0, 0, pItemTexMgr->GetTextureWidth(assistItems[i].tex),
									pItemTexMgr->GetTextureHeight(assistItems[i].tex));

								nDrawX = m_rcClient.left + g_nAccOftY;
								nDrawX += 32 / 2 - pItemTexMgr->GetTextureWidth(assistItems[i].tex) / 2;
								nDrawY = m_rcClient.top + g_nAccOftX + i * 35;
								nDrawY += 32 / 2 - pItemTexMgr->GetTextureHeight(assistItems[i].tex) / 2;

								m_pRender->Render(nDrawX, nDrawY);
							}
						}
					}
				}
			}
		}
		else
		{
			tex = pOpUI->GetTexture(TEX_ASSISTSHOWHIDE_VERTICAL + m_bBtnState[RC_SHOWHIDE]);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, pOpUI->GetTextureWidth(TEX_ASSISTSHOWHIDE_VERTICAL + m_bBtnState[RC_SHOWHIDE]), pOpUI->GetTextureHeight(TEX_ASSISTSHOWHIDE_VERTICAL + m_bBtnState[RC_SHOWHIDE]));
				m_pRender->Render(RELATIVE_X(m_rcBtnRect[RC_SHOWHIDE].left), RELATIVE_Y(m_rcBtnRect[RC_SHOWHIDE].top));
			}
		}
	}
}

bool GameAssistDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	GameMainOptUI::GetInstance()->GetIntroDlg()->ShowNothing();
	ZeroMemory(m_bBtnState, sizeof(m_bBtnState));

	POINT ptTest = _mp;
	ptTest.x -= m_rcClient.left;
	ptTest.y -= m_rcClient.top;

	for(int i = 0; i < RC_TOTAL; ++i)
	{
		if(::PtInRect(&m_rcBtnRect[i], ptTest))
		{
			OnBtn(i);
			return true;
		}
	}

	if(m_bHoriz)
	{
		if(_mp.x > RELATIVE_X(12) &&
			_mp.x < RELATIVE_X(222) &&
			_mp.y > RELATIVE_Y(3) &&
			_mp.y < RELATIVE_Y(36))
		{
			int nOftX = _mp.x - g_nAccOftX - m_rcClient.left;
			int nSel = nOftX / 35;
			int nLeft = nOftX % 35;

			if(nSel >= 0 &&
				nSel < 6 &&
				nLeft < 35)
			{
				OnAccelerate(nSel);
			}
		}
	}
	else
	{
		if(_mp.x > RELATIVE_X(3) &&
			_mp.x < RELATIVE_X(36) &&
			_mp.y > RELATIVE_Y(12) &&
			_mp.y < RELATIVE_Y(222))
		{
			int nOftY = _mp.y - g_nAccOftX - m_rcClient.top;
			int nSel = nOftY / 35;
			int nLeft = nOftY % 35;

			if(nSel >= 0 &&
				nSel < 6 &&
				nLeft < 35)
			{
				OnAccelerate(nSel);
			}
		}
	}

	return true;
}

void GameAssistDlg::ResetStatus()
{
	__super::ResetStatus();

	ZeroMemory(m_bBtnState, sizeof(m_bBtnState));
}

void GameAssistDlg::OnAccelerate(int _idx)
{
	ItemList& assistItems = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList();
	ItemAttrib* pSelAssistItem = &assistItems[_idx];
	const ItemAttrib* pBagSelItem = GameMainOptUI::GetInstance()->GetBagDlg2()->GetSelItem();

	if(pSelAssistItem->type != ITEM_NO)
	{
		GameMainOptUI::GetInstance()->GetIntroDlg()->ShowItemAttrib(pSelAssistItem);
		GameMainOptUI::GetInstance()->GetDisplayDlg()->ShowItemAttrib(pSelAssistItem);
	}

	if(NULL == pBagSelItem)
	{
		if(AfxGetHge()->Input_KeyUp(HGEK_RBUTTON))
		{
			GamePlayer::GetInstance()->UseItemFromAssist(_idx);
		}
	}
	else
	{
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			if(pBagSelItem->type == ITEM_COST)
			{
				int nMoveID = pBagSelItem->id;
				for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
				{
					if(assistItems[i].id == nMoveID &&
						assistItems[i].type != ITEM_NO)
					{
						ZeroMemory(&assistItems[i], sizeof(ItemAttrib));
					}
				}

				memcpy(pSelAssistItem, pBagSelItem, sizeof(ItemAttrib));
				GameMainOptUI::GetInstance()->GetBagDlg2()->CancelSel();
				GamePlayer::GetInstance()->UpdateAccelerate(_idx);
				pTheGame->PlayItemPutDownSound(pBagSelItem);
			}
		}
	}
}

void GameAssistDlg::OnBtn(int _idx)
{
	m_bBtnState[_idx] = BTN_STATE_OVER;
	if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
	{
		m_bBtnState[_idx] = BTN_STATE_DOWN;
	}
	bool bUp = AfxGetHge()->Input_KeyUp(HGEK_LBUTTON);

	if(_idx == RC_SHOWHIDE)
	{
		if(bUp)
		{
			m_bMaxSize = !m_bMaxSize;

			if(m_bHoriz)
			{
				if(m_bMaxSize)
				{
					m_rcClient.right = m_rcClient.left + 240;
					m_rcClient.bottom = m_rcClient.top + 38;
				}
				else
				{
					m_rcClient.right = m_rcClient.left + 8;
					m_rcClient.bottom = m_rcClient.top + 38;
				}
			}
			else
			{
				if(m_bMaxSize)
				{
					m_rcClient.right = m_rcClient.left + 38;
					m_rcClient.bottom = m_rcClient.top + 240;
				}
				else
				{
					m_rcClient.right = m_rcClient.left + 38;
					m_rcClient.bottom = m_rcClient.top + 8;
				}
			}
		}
	}
	else if(_idx == RC_TRANSFORM)
	{
		if(bUp)
		{
			if(m_bHoriz)
			{
				TransformHorizToVertical();
			}
			else
			{
				TransformVerticalToHoriz();
			}
		}
	}
}

bool GameAssistDlg::IsCaptionBar(int _x, int _y)
{
	if(m_bHoriz)
	{
		if(_x >= RELATIVE_X(0) &&
			_x <= RELATIVE_X(9) &&
			_y >= RELATIVE_Y(0) &&
			_y <= RELATIVE_Y(38))
		{
			return true;
		}
	}
	else
	{
		if(_x >= RELATIVE_X(0) &&
			_x <= RELATIVE_X(38) &&
			_y >= RELATIVE_Y(0) &&
			_y <= RELATIVE_Y(9))
		{
			return true;
		}
	}

	return false;
}