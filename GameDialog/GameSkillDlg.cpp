#include "GameSkillDlg.h"
#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
static const int s_nOftX = 22 - 3;
static const int s_nOftY = 57 + 20 + 10;
static const int s_nIconX = 14+2;
static const int s_nIconY = 11+2;
static const char* s_pszSkillTip = "[帮助]技能释放有两种模式。"
								   "\n1：点击技能图标按钮，设置F1-F9进行释放"
								   "\n2：移到技能图标按钮上，点击按键A-Z进行设置，然后用A-Z选择技能，退格键清空（←），使用~(数字1旁边按键)进行释放";
//////////////////////////////////////////////////////////////////////////
GameSkillDlg::GameSkillDlg()
{
	m_pRender = NEW_EMPTY_SPRITE;
	/*m_rcClient.left = */m_rcClient.top = 0;
	m_rcClient.left = WINDOW_WIDTH - 264;
	m_rcClient.right = /*264*/WINDOW_WIDTH;
	m_rcClient.bottom = 400+15;
	m_nSkillSum = 0;
	m_nPage = 0;
	SetVisible(false);

	SetWindowTitle("技能");
	SetWindowInfo("点击技能图标设置按键");
	SetWindowTip(s_pszSkillTip);

	//INIT_CLOSEBTN(238, 18);
	SetBkStype(1);
	CalcCloseButtonPosition();
	AddCommonButton(CBTN_LEFTARROW, 0, 75, 340 + 20 + 20, NULL);
	AddCommonButton(CBTN_RIGHTARROW, 1, 180, 340 + 20 + 20, NULL);
}

GameSkillDlg::~GameSkillDlg()
{
	SAFE_DELETE(m_pRender);
}
//////////////////////////////////////////////////////////////////////////
bool GameSkillDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	static const int s_nColGap = 56;

	m_pParent->GetIntroDlg()->ShowNothing();
	if(_mp.x > RELATIVE_X(22 + 13) &&
		_mp.x < RELATIVE_X(22 + 49))
	{
		do 
		{
			int nLeft = (_mp.y - m_rcClient.top - s_nOftY) % s_nColGap;
			int nRow = (_mp.y - m_rcClient.top - s_nOftY) / s_nColGap;
			if(nLeft <= 10 ||
				nLeft >= 46) {
					continue;
			}
			if(nRow < 0 ||
				nRow >= 5) {
					continue;
			}
			const MagicDetail* pDetail = GamePlayer::GetInstance()->GetMagicByIndex(nRow + m_nPage * 5);
			if(NULL == pDetail) {
				continue;
			}

			if(pDetail->wID == 0) {
				continue;
			}
			m_pParent->GetIntroDlg()->ShowLinefeedString(g_szMgcDescriptor[pDetail->wID], 20);

			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON) &&
				!pTheGame->IsPassiveSkill(pDetail->wID))
			{
				m_pParent->GetIntroDlg()->ShowNothing();
				m_pParent->GetStaticDlg()->CreateSkillButtonDlg(pDetail->wID);
			}
			else if(!pTheGame->IsPassiveSkill(pDetail->wID))
			{
				//	在没有edit有焦点的情况下监听A-Z
				if(!GameMainOptUI::GetInstance()->IsEditInFocus())
				{
					for(int i = HGEK_A; i <= HGEK_Z; ++i)
					{
						if(i == HGEK_Q ||
							i == HGEK_S ||
							i == HGEK_E ||
							i == HGEK_M)
						{
							continue;
						}
						if(AfxGetHge()->Input_KeyUp(i))
						{
							GamePlayer::GetInstance()->SetMagicAccKey(pDetail->wID, i);
							GamePlayer::GetInstance()->WriteAccMagicKeyCfg();
							break;
						}
					}

					if(AfxGetHge()->Input_KeyUp(HGEK_BACKSPACE))
					{
						GamePlayer::GetInstance()->SetMagicAccKey(pDetail->wID, 0);
						GamePlayer::GetInstance()->WriteAccMagicKeyCfg();
					}
				}
			}
		} while (0);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
void GameSkillDlg::Render()
{
	hgeResourceManager* pMgr = m_pParent->GetResourceMgr();
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	GameTextureManager* pTexIcon = GameResourceManager::GetInstance()->GetTexs(RES_MAGICICON);
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	if(NULL == pTexMgr)
	{
		return;
	}
	if(NULL == pMgr)
	{
		return;
	}

	__super::Render();

	HTEXTURE tex = 0;
	//HTEXTURE tex = pMgr->GetTexture("skill");
	/*if(tex)
	{
	m_pRender->SetTexture(tex);
	m_pRender->SetTextureRect(0, 0, 264, 372);
	m_pRender->Render(m_rcClient.left, m_rcClient.top);
	}*/

	//tex = pMgr->GetTexture("skillitem");
	tex = pPngMgr->GetTexture(25);
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	static const int s_nColGap = 58;
	static const int s_nItemPerPage = 5;

	int nDrawX = 0;
	int nDrawY = 0;
	int nPrintStart = 5 * m_nPage;

	HTEXTURE texButton = 0;

	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 222, s_nColGap);
		const MagicDetail* pDetail = NULL;
		for(int i = nPrintStart; i < nPrintStart + 5; ++i)
		{
			pDetail = pPlayer->GetMagicByIndex(i);
			if(pDetail)
			{
				if(pDetail->wID != 0)
				{
					nDrawX = RELATIVE_X(s_nOftX);
					nDrawY = RELATIVE_Y(s_nOftY + (i - nPrintStart) * s_nColGap);
					//tex = pMgr->GetTexture("skillitem");
					tex = pPngMgr->GetTexture(25);
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 222, s_nColGap);
					m_pRender->Render(nDrawX, nDrawY);

					AfxGetPrinter()->SetColor(ARGB_WHITE);
					AfxGetPrinter()->Print(nDrawX + 68, nDrawY + 12, pDetail->szName);
					AfxGetPrinter()->Print(nDrawX + 100, nDrawY + 34, "%d", pDetail->cLevel);

					if(pDetail->cAccKey != 0)
					{
						char szKeyString[2] = {0};
						szKeyString[0] = pDetail->cAccKey;
						AfxGetPrinter()->Print(nDrawX + 100 + 12, nDrawY + 34, "切换:%s", szKeyString);
					}

					if(pDetail->cKey != 0)
					{
						texButton = pTexMgr->GetTexture(18 + pDetail->cKey - HGEK_F1);
						if(texButton)
						{
							m_pRender->SetTexture(texButton);
							m_pRender->SetTextureRect(0, 0, 20, 16);
							m_pRender->Render(nDrawX + 180, nDrawY + 20);
						}
					}

					tex = pTexIcon->GetTexture(pDetail->wID);
					if(tex)
					{
						m_pRender->SetTexture(tex);
						m_pRender->SetTextureRect(0, 0,
							pTexIcon->GetTextureWidth(pDetail->wID),
							pTexIcon->GetTextureHeight(pDetail->wID));
						m_pRender->Render(nDrawX + s_nIconX,
							nDrawY + s_nIconY);
					}
				}
				else
				{
					break;
				}
			}
		}
	}

	RenderWindowTip();
}
//////////////////////////////////////////////////////////////////////////
bool GameSkillDlg::IsCaptionBar(int _x, int _y)
{
	return __super::IsCaptionBar(_x, _y);
	if(_y >= RELATIVE_Y(0) &&
		_y <= RELATIVE_Y(30))
	{
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool GameSkillDlg::OnCommonButtonClick(int _id)
{
	const MagicDetail* pDetail = NULL;
	m_nSkillSum = 0;

	for(int i = 0; i < USER_MAGIC_NUM; ++i)
	{
		pDetail = GamePlayer::GetInstance()->GetMagicByIndex(i);
		if(pDetail)
		{
			if(pDetail->wID != 0)
			{
				++m_nSkillSum;
			}
		}
	}

	if(_id == 0)
	{
		--m_nPage;
		if(m_nPage < 0)
		{
			m_nPage = 0;
		}
	}
	else if(_id == 1)
	{
		int nPageMax = m_nSkillSum / 5;
		int nLeft = m_nSkillSum % 5;
		if(nLeft != 0)
		{
			++nPageMax;
		}
		++m_nPage;
		if(m_nPage >= nPageMax)
		{
			m_nPage = nPageMax - 1;
		}
	}
	return true;
}

bool GameSkillDlg::OnShowDialog()
{
	SetNeedTopLevel(true);
	return true;
}