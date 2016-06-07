#include "GameFloatDlg.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
#define FLOATMESSAGEBAR_SPEED	400
#define FLOATMESSAGEBAR_POSY	10
#define FLOATMESSAGEBAR_HEIGHT	20
#define FLOATMESSAGEITEM_SPEED	100
#define FLOATMESSAGEWAIT_TIME	100
//////////////////////////////////////////////////////////////////////////
GameFloatDlg::GameFloatDlg()
{
	m_eState = FloatMessageState_Hide;
	m_dwLastBarHidingTime = 0;
	m_dwLastBarShowingTime = 0;
	m_dwLastMsgShowTime = 0;
	m_dwLastWaitingTime = 0;
	m_nListBkX = WINDOW_WIDTH;

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
}

GameFloatDlg::~GameFloatDlg()
{
	SAFE_DELETE(m_pRender);
}

void GameFloatDlg::Update(float _dt)
{
	//	
	if(m_eState == FloatMessageState_Hide)
	{
		if(!m_xMsgList.empty())
		{
			//	显示Bar
			m_eState = FloatMessageState_Showing;
			m_dwLastBarShowingTime = GetTickCount();

			m_nListBkX -= FLOATMESSAGEBAR_SPEED * _dt;
		}
	}
	else if(FloatMessageState_Hiding == m_eState)
	{
		if(m_xMsgList.empty())
		{
			m_dwLastBarHidingTime = GetTickCount();
			m_nListBkX += FLOATMESSAGEBAR_SPEED * _dt;

			if(m_nListBkX >= WINDOW_WIDTH)
			{
				m_eState = FloatMessageState_Hide;
				m_nListBkX = WINDOW_WIDTH;
			}
		}
		else
		{
			m_eState = FloatMessageState_Showing;
			m_dwLastBarShowingTime = GetTickCount();

			m_nListBkX -= FLOATMESSAGEBAR_SPEED * _dt;
		}
	}
	else if(FloatMessageState_Showing == m_eState)
	{
		m_dwLastBarShowingTime = GetTickCount();
		m_nListBkX -= FLOATMESSAGEBAR_SPEED * _dt;

		if(m_nListBkX <= 0)
		{
			m_nListBkX = 0;
			m_eState = FloatMessageState_Message;
		}
	}
	else if(FloatMessageState_Message == m_eState)
	{
		if(m_xMsgList.empty())
		{
			//	隐藏Bar
			m_eState = FloatMessageState_Hiding;
			m_dwLastBarHidingTime = GetTickCount();

			m_nListBkX += FLOATMESSAGEBAR_SPEED * _dt;
		}
		else
		{
			//	显示信息
			FloatMessageItem* pShowItem = m_xMsgList.front();

			pShowItem->fShowX -= (float)FLOATMESSAGEITEM_SPEED * _dt;
			if(pShowItem->fShowX + pShowItem->nTextLen <= 0)
			{
				pShowItem->fShowX = -pShowItem->nTextLen;
				m_dwLastWaitingTime = GetTickCount();
				m_eState = FloatMessageState_Waiting;
				++pShowItem->dwShowTimes;
			}
		}
	}
	else if(FloatMessageState_Waiting == m_eState)
	{
		//	wait...
		if(GetTickCount() - m_dwLastWaitingTime > FLOATMESSAGEWAIT_TIME)
		{
			if(m_xMsgList.empty())
			{
				m_eState = FloatMessageState_Hiding;
				m_dwLastBarHidingTime = GetTickCount();
			}
			else
			{
				if(m_xMsgList.empty())
				{
					m_eState = FloatMessageState_Hiding;
					m_dwLastBarHidingTime = GetTickCount();
				}
				else
				{
					FloatMessageItem* pShowItem = m_xMsgList.front();
					if(pShowItem->dwShowTimes >= pShowItem->dwTimes)
					{
						m_xMsgList.pop_front();
						SAFE_DELETE(pShowItem);
					}
					else
					{
						pShowItem->fShowX = WINDOW_WIDTH;
						m_eState = FloatMessageState_Message;
					}
				}
			}
		}
	}
}

void GameFloatDlg::Render()
{
	if(FloatMessageState_Hide == m_eState)
	{
		//	nothing
	}
	else if(FloatMessageState_Showing == m_eState ||
		FloatMessageState_Hiding == m_eState ||
		FloatMessageState_Waiting == m_eState)
	{
		if(m_nListBkX >= WINDOW_WIDTH)
		{
			return;
		}

		hgeResourceManager* pResMgr = pTheGame->GetGameRes();

		{
			//	draw bk
			HTEXTURE tex = pResMgr->GetTexture("bmcolor");
			//tex = pPngMgr->GetTexture(16);

			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0,
					0,
					WINDOW_WIDTH - m_nListBkX,
					FLOATMESSAGEBAR_HEIGHT);
				m_pRender->Render(m_nListBkX,
					FLOATMESSAGEBAR_POSY);
			}
		}
	}
	else if(FloatMessageState_Message)
	{
		//	draw the full background
		hgeResourceManager* pResMgr = pTheGame->GetGameRes();

		{
			//	draw bk
			HTEXTURE tex = pResMgr->GetTexture("bmcolor");
			//tex = pPngMgr->GetTexture(16);

			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0,
					0,
					WINDOW_WIDTH,
					FLOATMESSAGEBAR_HEIGHT);
				m_pRender->Render(0,
					FLOATMESSAGEBAR_POSY);
			}
		}

		//	draw message text
		if(m_xMsgList.empty())
		{
			return;
		}

		FloatMessageItem* pShowItem = m_xMsgList.front();

		AfxGetPrinter()->SetColor(pShowItem->dwColor);
		AfxGetPrinter()->PrintWithoutStroke((int)pShowItem->fShowX,
			FLOATMESSAGEBAR_POSY + (FLOATMESSAGEBAR_HEIGHT - 12) / 2,
			pShowItem->xMsg.c_str());
	}
}

void GameFloatDlg::AddFloatMessage(const char* _pszMsg, DWORD _dwColor /* = 0xffffffff */, DWORD _dwTimes /* = 1 */)
{
	if(_dwTimes == 0)
	{
		return;
	}

	FloatMessageItem* pNewItem = new FloatMessageItem;
	pNewItem->xMsg = _pszMsg;
	pNewItem->dwTimes = _dwTimes;
	pNewItem->dwShowTimes = 0;
	pNewItem->fShowX = WINDOW_WIDTH;
	pNewItem->bShowed = false;
	pNewItem->dwColor = _dwColor;
	pNewItem->nTextLen = GetTextWidth(_pszMsg, 12);

	m_xMsgList.push_back(pNewItem);
}