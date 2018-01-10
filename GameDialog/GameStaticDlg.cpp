#include "GameDlgBase.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
static const int s_nSkillBtnOftX = 40;
static const int s_nSkillBtnOftY = 65;
static const int s_nSkillCancleX = 155;
static const int s_nSkillCancleY = 115;
//////////////////////////////////////////////////////////////////////////
GameStaticDlg::GameStaticDlg()
{
	m_bIsStatic = true;
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_dwAutoHide = 0;

	m_rcClient.left = (WINDOW_WIDTH - 352) / 2;
	m_rcClient.right = m_rcClient.left + 352;
	m_rcClient.top = 150;
	m_rcClient.bottom = m_rcClient.top + 162;
	SetVisible(false);

	m_rcRevive[0].left = 80;
	m_rcRevive[0].top = 105;
	m_rcRevive[0].right = m_rcRevive[0].left + 75;
	m_rcRevive[0].bottom = m_rcRevive[0].top + 30;

	m_rcRevive[1].left = 210;
	m_rcRevive[1].top = 105;
	m_rcRevive[1].right = m_rcRevive[1].left + 75;
	m_rcRevive[1].bottom = m_rcRevive[1].top + 30;
	m_bBtnState[0] = m_bBtnState[1] = BTN_STATE_NORMAL;

	m_bMode = MODE_STATIC;
	m_nSkillDown = -1;
	m_dwDropTag = 0;
	m_dwMergyDestTag = m_dwMergySrcTag = 0;
}

GameStaticDlg::~GameStaticDlg()
{
	SAFE_DELETE(m_pRender);
}
//////////////////////////////////////////////////////////////////////////
void GameStaticDlg::Render()
{
	hgeResourceManager* pMgr = m_pParent->GetResourceMgr();
	if(NULL == pMgr)
	{
		return;
	}
	if(!IsVisible())
	{
		return;
	}

	HTEXTURE tex = pMgr->GetTexture("static");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 352, 162);
		m_pRender->Render(m_rcClient.left, m_rcClient.top);

		if(!m_xMsg.empty())
		{
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			AfxGetPrinter()->Print(RELATIVE_X(22), RELATIVE_Y(66), m_xMsg.c_str());
		}

		if(!m_xTitle.empty())
		{
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			AfxGetPrinter()->Print(RELATIVE_X(m_nTitleX), RELATIVE_Y(8), m_xTitle.c_str());
		}
	}

	if(m_bMode == MODE_SKILL)
	{
		GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
		if(pTexMgr)
		{
			for(int i = 0; i < 8; ++i)
			{
				tex = pTexMgr->GetTexture(2 + i * 2);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 32, 30);
					m_pRender->Render(RELATIVE_X(s_nSkillBtnOftX + i * 34), RELATIVE_Y(s_nSkillBtnOftY));
				}
			}

			tex = pTexMgr->GetTexture(26);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, 40, 30);
				m_pRender->Render(RELATIVE_X(s_nSkillCancleX), RELATIVE_Y(s_nSkillCancleY));
			}

			if(m_nSkillDown != -1)
			{
				if(m_nSkillDown != 8)
				{
					tex = pTexMgr->GetTexture(2 + m_nSkillDown * 2 + 1);
					if(tex)
					{
						m_pRender->SetTexture(tex);
						m_pRender->SetTextureRect(0, 0, 32, 30);
						m_pRender->Render(RELATIVE_X(s_nSkillBtnOftX + m_nSkillDown * 34), RELATIVE_Y(s_nSkillBtnOftY));
					}
				}
				else
				{
					tex = pTexMgr->GetTexture(27);
					if(tex)
					{
						m_pRender->SetTexture(tex);
						m_pRender->SetTextureRect(0, 0, 40, 30);
						m_pRender->Render(RELATIVE_X(s_nSkillCancleX), RELATIVE_Y(s_nSkillCancleY));
					}
				}
			}
		}
	}
	else if(m_bMode == MODE_DEAD)
	{
		hgeResourceManager* pMgr = m_pParent->GetResourceMgr();
		int nBtnTextX = 0;
		static const char* s_szBtnText[] =
		{
			"回城复活",
			"原地复活"
		};

		if(pMgr)
		{
			tex = pMgr->GetTexture("sysbtn");
			if(tex)
			{
				for(int i = 0; i < 2; ++i)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 30 * m_bBtnState[i], 75, 30);
					m_pRender->Render(RELATIVE_X(m_rcRevive[i].left), RELATIVE_Y(m_rcRevive[i].top));

					nBtnTextX = m_rcRevive[i].left + (75 - 4 * 12) / 2;
					AfxGetPrinter()->SetColor(ARGB_RED);
					AfxGetPrinter()->Print(RELATIVE_X(nBtnTextX), RELATIVE_Y(m_rcRevive[i].top + 9), s_szBtnText[i]);
				}
			}
		}
	}
	else if(m_bMode == MODE_DROP)
	{
		hgeResourceManager* pMgr = m_pParent->GetResourceMgr();
		int nBtnTextX = 0;
		static const char* s_szBtnText[] =
		{
			"确认丢弃",
			"取消丢弃"
		};

		if(pMgr)
		{
			tex = pMgr->GetTexture("sysbtn");
			if(tex)
			{
				for(int i = 0; i < 2; ++i)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 30 * m_bBtnState[i], 75, 30);
					m_pRender->Render(RELATIVE_X(m_rcRevive[i].left), RELATIVE_Y(m_rcRevive[i].top));

					nBtnTextX = m_rcRevive[i].left + (75 - 4 * 12) / 2;
					AfxGetPrinter()->SetColor(ARGB_RED);
					AfxGetPrinter()->Print(RELATIVE_X(nBtnTextX), RELATIVE_Y(m_rcRevive[i].top + 9), s_szBtnText[i]);
				}
			}
		}
	}
	else if(m_bMode == MODE_SAVE)
	{
		hgeResourceManager* pMgr = m_pParent->GetResourceMgr();
		int nBtnTextX = 0;
		static const char* s_szBtnText[] =
		{
			"确认存档",
			"取消存档"
		};

		if(pMgr)
		{
			tex = pMgr->GetTexture("sysbtn");
			if(tex)
			{
				for(int i = 0; i < 2; ++i)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 30 * m_bBtnState[i], 75, 30);
					m_pRender->Render(RELATIVE_X(m_rcRevive[i].left), RELATIVE_Y(m_rcRevive[i].top));

					nBtnTextX = m_rcRevive[i].left + (75 - 4 * 12) / 2;
					AfxGetPrinter()->SetColor(ARGB_RED);
					AfxGetPrinter()->Print(RELATIVE_X(nBtnTextX), RELATIVE_Y(m_rcRevive[i].top + 9), s_szBtnText[i]);
				}
			}
		}
	}
	else if(m_bMode == MODE_CLOSE)
	{
		hgeResourceManager* pMgr = m_pParent->GetResourceMgr();
		int nBtnTextX = 0;
		static const char* s_szBtnText[] =
		{
			"我要存档",
			"我要退出"
		};

		if(pMgr)
		{
			tex = pMgr->GetTexture("sysbtn");
			if(tex)
			{
				for(int i = 0; i < 2; ++i)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 30 * m_bBtnState[i], 75, 30);
					m_pRender->Render(RELATIVE_X(m_rcRevive[i].left), RELATIVE_Y(m_rcRevive[i].top));

					nBtnTextX = m_rcRevive[i].left + (75 - 4 * 12) / 2;
					AfxGetPrinter()->SetColor(ARGB_RED);
					AfxGetPrinter()->Print(RELATIVE_X(nBtnTextX), RELATIVE_Y(m_rcRevive[i].top + 9), s_szBtnText[i]);
				}
			}
		}
	}
	else if(m_bMode == MODE_MERGY)
	{
		hgeResourceManager* pMgr = m_pParent->GetResourceMgr();
		int nBtnTextX = 0;
		static const char* s_szBtnText[] =
		{
			"确认合并",
			"取消合并"
		};

		if(pMgr)
		{
			tex = pMgr->GetTexture("sysbtn");
			if(tex)
			{
				for(int i = 0; i < 2; ++i)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 30 * m_bBtnState[i], 75, 30);
					m_pRender->Render(RELATIVE_X(m_rcRevive[i].left), RELATIVE_Y(m_rcRevive[i].top));

					nBtnTextX = m_rcRevive[i].left + (75 - 4 * 12) / 2;
					AfxGetPrinter()->SetColor(ARGB_RED);
					AfxGetPrinter()->Print(RELATIVE_X(nBtnTextX), RELATIVE_Y(m_rcRevive[i].top + 9), s_szBtnText[i]);
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool GameStaticDlg::CreateSkillButtonDlg(DWORD _dwMgcID)
{
	if(!GamePlayer::GetInstance()->IsMagicExist(_dwMgcID))
	{
		return false;
	}

	m_nSkillDown = -1;
	const MagicDetail* pDetail = NULL;
	for(int i = 0; i < USER_MAGIC_NUM; ++i)
	{
		if(GamePlayer::GetInstance()->GetMagicByIndex(i)->wID == _dwMgcID)
		{
			pDetail = GamePlayer::GetInstance()->GetMagicByIndex(i);
			break;
		}
	}

	if(NULL == pDetail)
	{
		return false;
	}

	m_bMode = MODE_SKILL;
	m_xMsg.clear();
	SetTitle("设置技能按键");
	m_dwMgc = _dwMgcID;

	if(pDetail->cKey >= HGEK_F1 &&
		pDetail->cKey <= HGEK_F8)
	{
		m_nSkillDown = pDetail->cKey - HGEK_F1;
	}

	m_dwAutoHide = 0;
	SetVisible(true);
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameStaticDlg::CreateDeadDlg()
{
	if(GamePlayer::GetInstance()->GetStatus() != PST_DEAD)
	{
		return false;
	}
	m_xMsg = "回城复活:损失10%%经验 原地复活:损失50%%经验,损失10%%金币";
	SetTitle("死亡复活");
	m_bMode = MODE_DEAD;

	m_dwAutoHide = 0;
	SetVisible(true);
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameStaticDlg::CreateCloseDlg()
{
	m_xMsg = "直接退出将丢失存档数据，是否确认?";
	SetTitle("确认退出");
	m_bMode = MODE_CLOSE;

	m_dwAutoHide = 0;
	SetVisible(true);
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameStaticDlg::CreateDestoryItemDlg(DWORD _dwTag)
{
	m_xMsg = "已拣取的物品丢弃销毁，确认?";
	SetTitle("销毁物品");
	m_bMode = MODE_DROP;
	m_dwDropTag = _dwTag;
	m_dwAutoHide = 0;
	SetVisible(true);
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameStaticDlg::CreateSaveBindDlg()
{
	m_xMsg = "保存存档将使所有物品绑定，是否确认存档?";
	SetTitle("确认存档");
	m_bMode = MODE_SAVE;
	m_dwAutoHide = 0;
	SetVisible(true);
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameStaticDlg::CreateItemMergyDlg(DWORD _dwMergyDestTag, DWORD _dwMergySrcTag)
{
	m_xMsg = "绑定物品与非绑定物品合并将绑定非绑定物品";
	SetTitle("确认合并");
	m_bMode = MODE_MERGY;
	m_dwMergyDestTag = _dwMergyDestTag;
	m_dwMergySrcTag = _dwMergySrcTag;
	m_dwAutoHide = 0;
	SetVisible(true);
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameStaticDlg::ProcUserCmd(const POINT& _mp)
{
	if(m_dwAutoHide != 0)
	{
		if(GetTickCount() >= m_dwAutoHide)
		{
			SetVisible(false);
			//m_pParent->GetDlgControl()->SetUnBlock();
			return true;
		}
	}
	RECT rcTest;
	static bool s_bLeftUpProcessed = false;

	if(m_bMode == MODE_SKILL)
	{
		if(_mp.y > RELATIVE_Y(s_nSkillBtnOftY) &&
			_mp.y < RELATIVE_Y(s_nSkillBtnOftY + 30))
		{
			if(_mp.x > RELATIVE_X(s_nSkillBtnOftX) &&
				_mp.x < RELATIVE_X(s_nSkillBtnOftX + 8 * 34))
			{
				int nOft = (_mp.x - m_rcClient.left - s_nSkillBtnOftX) % 34;
				if(nOft >= 0 &&
					nOft <= 32)
				{
					if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
					{
						m_nSkillDown = (_mp.x - m_rcClient.left - s_nSkillBtnOftX) / 34;
					}
					else if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
					{
						GamePlayer::GetInstance()->SetMagicKey(m_dwMgc, HGEK_F1 + m_nSkillDown);
						SetVisible(false);
						GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
						GamePlayer::GetInstance()->WriteAccMagicKeyCfg();
						//m_pParent->GetDlgControl()->SetUnBlock();
					}
				}
			}
		}
		else if(_mp.x > RELATIVE_X(s_nSkillCancleX) &&
			_mp.x < RELATIVE_X(s_nSkillCancleX + 40) &&
			_mp.y > RELATIVE_Y(s_nSkillCancleY) &&
			_mp.y < RELATIVE_Y(s_nSkillCancleY + 30))
		{
			if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
			{
				m_nSkillDown = 8;
			}
			else if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				GamePlayer::GetInstance()->SetMagicKey(m_dwMgc, 0);
				SetVisible(false);
				GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
				//m_pParent->GetDlgControl()->SetUnBlock();
			}
		}
	}
	else if(m_bMode == MODE_DEAD)
	{
		for(int i = 0; i < 2; ++i)
		{
			m_bBtnState[i] = BTN_STATE_NORMAL;
			rcTest.left = RELATIVE_X(m_rcRevive[i].left);
			rcTest.right = RELATIVE_X(m_rcRevive[i].right);
			rcTest.top = RELATIVE_Y(m_rcRevive[i].top);
			rcTest.bottom = RELATIVE_Y(m_rcRevive[i].bottom);
			if(PtInRect(&rcTest, _mp))
			{
				m_bBtnState[i] = BTN_STATE_OVER;

				if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
				{
					m_bBtnState[i] = BTN_STATE_DOWN;
				}
				if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
				{
					m_bBtnState[i] = BTN_STATE_OVER;
					PkgPlayerReviveReq req;
					req.bMode = i + 1;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
					//SetVisible(false);
				}
			}
		}
	}
	else if(m_bMode == MODE_DROP)
	{
		for(int i = 0; i < 2; ++i)
		{
			m_bBtnState[i] = BTN_STATE_NORMAL;
			rcTest.left = RELATIVE_X(m_rcRevive[i].left);
			rcTest.right = RELATIVE_X(m_rcRevive[i].right);
			rcTest.top = RELATIVE_Y(m_rcRevive[i].top);
			rcTest.bottom = RELATIVE_Y(m_rcRevive[i].bottom);
			if(PtInRect(&rcTest, _mp))
			{
				m_bBtnState[i] = BTN_STATE_OVER;

				if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
				{
					m_bBtnState[i] = BTN_STATE_DOWN;
				}
				if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
				{
					/*
					m_bBtnState[i] = BTN_STATE_OVER;
										PkgPlayerReviveReq req;
										req.bMode = i + 1;
										req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
										g_xBuffer.Reset();
										g_xBuffer << req;
										SendBuffer(&g_xBuffer);*/
					if(i == 0)
					{
						PkgPlayerDropItemReq req;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
						req.dwId = m_dwDropTag;
						g_xBuffer.Reset();
						g_xBuffer << req;
						SendBufferToGS(&g_xBuffer);
						SetVisible(false);
					}
					else if(i == 1)
					{
						SetVisible(false);
					}
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
				}
			}
		}
	}
	else if(m_bMode == MODE_SAVE)
	{
		for(int i = 0; i < 2; ++i)
		{
			m_bBtnState[i] = BTN_STATE_NORMAL;
			rcTest.left = RELATIVE_X(m_rcRevive[i].left);
			rcTest.right = RELATIVE_X(m_rcRevive[i].right);
			rcTest.top = RELATIVE_Y(m_rcRevive[i].top);
			rcTest.bottom = RELATIVE_Y(m_rcRevive[i].bottom);
			if(PtInRect(&rcTest, _mp))
			{
				m_bBtnState[i] = BTN_STATE_OVER;

				if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
				{
					m_bBtnState[i] = BTN_STATE_DOWN;
				}
				if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
				{
					if(i == 0)
					{
						Create("正在请求服务器数据", 5000);

						PkgPlayerUserDataReq req;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
						g_xBuffer.Reset();
						g_xBuffer << req;
						SendBufferToGS(&g_xBuffer);
					}
					else if(i == 1)
					{
						SetVisible(false);
					}
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
				}
			}
		}
	}
	else if(m_bMode == MODE_CLOSE)
	{
		for(int i = 0; i < 2; ++i)
		{
			m_bBtnState[i] = BTN_STATE_NORMAL;
			rcTest.left = RELATIVE_X(m_rcRevive[i].left);
			rcTest.right = RELATIVE_X(m_rcRevive[i].right);
			rcTest.top = RELATIVE_Y(m_rcRevive[i].top);
			rcTest.bottom = RELATIVE_Y(m_rcRevive[i].bottom);
			if(PtInRect(&rcTest, _mp))
			{
				m_bBtnState[i] = BTN_STATE_OVER;

				if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
				{
					m_bBtnState[i] = BTN_STATE_DOWN;
				}
				if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
				{
					if(i == 0)
					{
						Create("正在请求服务器数据", 5000);

						PkgPlayerUserDataReq req;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
						g_xBuffer.Reset();
						g_xBuffer << req;
						SendBufferToGS(&g_xBuffer);
					}
					else if(i == 1)
					{
						PostQuitMessage(0);
					}
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
				}
			}
		}
	}
	else if(m_bMode == MODE_MERGY)
	{
		for(int i = 0; i < 2; ++i)
		{
			m_bBtnState[i] = BTN_STATE_NORMAL;
			rcTest.left = RELATIVE_X(m_rcRevive[i].left);
			rcTest.right = RELATIVE_X(m_rcRevive[i].right);
			rcTest.top = RELATIVE_Y(m_rcRevive[i].top);
			rcTest.bottom = RELATIVE_Y(m_rcRevive[i].bottom);
			if(PtInRect(&rcTest, _mp))
			{
				m_bBtnState[i] = BTN_STATE_OVER;

				if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
				{
					m_bBtnState[i] = BTN_STATE_DOWN;
				}
				if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
				{
					m_bBtnState[i] = BTN_STATE_OVER;
					
					if(i == 0)
					{
						PkgPlayerMergyCostItemReq req;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();

						ItemAttrib* pDestItem = GamePlayer::GetInstance()->GetPlayerBag()->GetItemByTag(m_dwMergyDestTag);
						ItemAttrib* pSrcItem = GamePlayer::GetInstance()->GetPlayerBag()->GetItemByTag(m_dwMergySrcTag);

						if(pDestItem &&
							pSrcItem)
						{
							if(pDestItem->id == pSrcItem->id &&
								pDestItem->type == ITEM_COST &&
								pSrcItem->type == ITEM_COST)
							{
								req.dwDestTag = pDestItem->tag;
								req.dwSrcTag = pSrcItem->tag;
								g_xBuffer.Reset();
								g_xBuffer << req;
								SendBufferToGS(&g_xBuffer);
							}
						}
					}
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
					SetVisible(false);
				}
			}
		}
	}

	return true;
}