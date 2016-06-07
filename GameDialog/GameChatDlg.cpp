#include "GameChatDlg.h"
#include <assert.h>
#include "../GameScene/GameScene.h"
#include "../BackMir/BackMir.h"
#include "../GameScene/GameResourceManager.h"
//////////////////////////////////////////////////////////////////////////
#define CHAT_UI_COLOR	ARGB(168, 255, 255, 255)
#define TEXT_WIDTH		385
/************************************************************************/
/* Class GameChatHistory
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
GameChatHistory::GameChatHistory()
{
	m_nShowIndex = 0;
	m_nInsertCount = 0;
	m_dwAlpha = LOSEFOCUS_ALPHA;
	m_bVisible = true;
	m_bIsMaxSize = true;

	/*m_rcClient.left = 30;
	m_rcClient.top = 375 - 60 + 10 - 3 - 90;
	m_rcClient.right = 286;
	m_rcClient.bottom = 520 - 60 + 10 - 3 - 90;*/

	m_rcClient.left = 230 + 15;
	m_rcClient.top = 450 + 60;
	m_rcClient.right = 486 + 15 + 100;
	m_rcClient.bottom = 450 + 60 + 145;

	/*m_hBk = AfxGetHge()->Texture_Create(2, 2);
	if(m_hBk)
	{
		DWORD* pData = AfxGetHge()->Texture_Lock(m_hBk, false);
		for(int i = 0; i < 4; ++i)
		{
			pData[i] = 0xFF000000;
		}
		AfxGetHge()->Texture_Unlock(m_hBk);
		m_pRender = new hgeSprite(m_hBk, 0, 0, m_rcClient.right - m_rcClient.left,
			m_rcClient.bottom - m_rcClient.top);
	}*/
	m_hBk = GameScene::sThis->GetMainOpt()->GetResourceMgr()->GetTexture("bmcolor");
	m_pRender = new hgeSprite(m_hBk, 0, 0, m_rcClient.right - m_rcClient.left,
		m_rcClient.bottom - m_rcClient.top);
	//m_pRender->SetColor(CHAT_UI_COLOR);
}

GameChatHistory::~GameChatHistory()
{
	SAFE_DELETE(m_pRender);
}

//////////////////////////////////////////////////////////////////////////
void GameChatHistory::Render()
{
	if(!m_pRender)
	{
		return;
	}
	if(!IsVisible())
	{
		return;
	}
	//m_pRender->SetColor(m_dwAlpha);
	//m_pRender->Render((float)m_rcClient.left,
		//(float)m_rcClient.top);

	hgeResourceManager* pResMgr = pTheGame->GetGameRes();
	HTEXTURE tex = 0;

	//	back ground

	/*if(m_bIsMaxSize)
	{
		tex = pResMgr->GetTexture("scroll");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 16, 130);
			m_pRender->Render(225, m_rcClient.top);
		}
	}
	else
	{
		tex = pResMgr->GetTexture("scrollm");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 16, 33);
			m_pRender->Render(0, m_rcClient.top + 130 - 33);
		}
	}*/

	/*GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	tex = pPngMgr->GetTexture(14);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, RECT_WIDTH(m_rcClient), RECT_HEIGHT(m_rcClient));
		m_pRender->Render(m_rcClient.left, m_rcClient.top);
	}*/

	int nShowNumber = 3;
	int nLine = 0;
	if(m_bIsMaxSize)
	{
		nShowNumber = MAX_SHOW_LINE;
	}
	else
	{
		nShowNumber = MIN_SHOW_LINE;
	}

	for(int i = m_nShowIndex; i < nShowNumber + m_nShowIndex; ++i)
	{
		if(i < 0 ||
			i >= CHAT_HISTORY_NUMBER)
		{
			break;
		}
		if(m_stMsg[i].szMsg[0] == 0)
		{
			break;
		}

		DWORD dwUseColor = m_stMsg[i].dwExtraMsg;
		AfxGetPrinter()->SetColor(dwUseColor);

		if(m_bIsMaxSize)
		{
			AfxGetPrinter()->PrintWithoutStroke(m_rcClient.left + 5,
				m_rcClient.top + 5 + nLine * 13,
				m_stMsg[i].szMsg);
		}
		else
		{
			AfxGetPrinter()->PrintWithoutStroke(m_rcClient.left + 5,
				m_rcClient.top + 5 + nLine * 13 + 13 * (MAX_SHOW_LINE - 1),
				m_stMsg[i].szMsg);
			break;
		}
		++nLine;
	}
}

void GameChatHistory::Update(float _dt)
{

}

bool GameChatHistory::ProcUserCmd(const POINT& _mp)
{
	if(!PtInRect(&GetRect(),_mp))
	{
		return false;
	}
	//SetDlgFocus(true);
	int nLine = (_mp.y - m_rcClient.top - 5) / 13;
	if(nLine >= 0 &&
		nLine < MAX_SHOW_LINE)
	{
		if(AfxGetHge()->Input_KeyUp(HGEK_RBUTTON))
		{
			int nShowIndex = m_nShowIndex + nLine;

			if(nShowIndex >= 0 &&
				nShowIndex < CHAT_HISTORY_NUMBER)
			{
				MsgNode* pNode = &m_stMsg[nShowIndex];

				if((pNode->nNodeType == MSGNODE_PRIVAECHAT || pNode->nNodeType == MSGNODE_PUBLICCHAT) &&
					pNode->szSender[0] != 0)
				{
					GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->GainFocus();
					GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->ClearText();
					GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->SetText("/");
					GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->SetText(pNode->szSender);
					GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->SetText(" ");
					GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->SetVisible(true);
				}
			}
		}
	}
	return true;
}

void GameChatHistory::ScrollUp()
{
	--m_nShowIndex;
	if(m_nShowIndex < 0)
	{
		m_nShowIndex = 0;
	}
}

void GameChatHistory::SetMaxSize(bool _b)
{
	m_bIsMaxSize = _b;

	if(m_bIsMaxSize)
	{
		m_nShowIndex = m_nInsertCount - MAX_SHOW_LINE;
		if(m_nShowIndex < 0)
		{
			m_nShowIndex = 0;
		}
	}
	else
	{
		m_nShowIndex = m_nInsertCount - 1;
		if(m_nShowIndex < 0)
		{
			m_nShowIndex = 0;
		}
	}
}

void GameChatHistory::ScrollDown()
{
	int nDownIndex = 0;
	if(m_bIsMaxSize)
	{
		nDownIndex = m_nShowIndex + MAX_SHOW_LINE;
	}
	else
	{
		nDownIndex = m_nShowIndex + MIN_SHOW_LINE;
	}
	if(nDownIndex + 1 >= CHAT_HISTORY_NUMBER)
	{
		return;
	}
	if(m_stMsg[nDownIndex].szMsg[0] == 0)
	{
		return;
	}
	++m_nShowIndex;
	if(m_nShowIndex > m_nInsertCount - 1)
	{
		m_nShowIndex = m_nShowIndex - 1;
	}
}

void GameChatHistory::InsertPrivateSelfChatMessage(const char* _pszSender, const char* _pszMsg)
{
	/*
	if(strlen(_pszSender) > 19)
		{
			return;
		}
	
		if(m_nInsertCount >= CHAT_HISTORY_NUMBER - 1)
		{
			m_nInsertCount = 0;
			m_nShowIndex = 0;
			ClearChatMessage();
		}
	
		++m_nInsertCount;
		if(strlen(_pszMsg) < CHAT_MESSAGE_LENGTH)
		{
			strcpy(m_stMsg[m_nInsertCount - 1].szMsg, _pszMsg);
			strcpy(m_stMsg[m_nInsertCount - 1].szSender, _pszSender);
			m_stMsg[m_nInsertCount - 1].dwExtraMsg = COLOR_PRIVATECHAT;
			m_stMsg[m_nInsertCount - 1].nNodeType = MSGNODE_PRIVAECHAT;
		}
	
		if(m_bIsMaxSize)
		{
			if(m_nShowIndex + MAX_SHOW_LINE < m_nInsertCount)
			{
				m_nShowIndex = m_nInsertCount - MAX_SHOW_LINE;
			}
		}
		else
		{
			m_nShowIndex = m_nInsertCount - MIN_SHOW_LINE;
		}*/
	
	InsertRawMessage(_pszSender, _pszMsg, MSGNODE_PRIVAECHAT, COLOR_PRIVATECHAT);
}

void GameChatHistory::InsertPrivateChatMessage(const char* _pszSender, const char* _pszMsg)
{
	/*
	if(strlen(_pszSender) > 19)
		{
			return;
		}
	
		if(m_nInsertCount >= CHAT_HISTORY_NUMBER - 1)
		{
			m_nInsertCount = 0;
			m_nShowIndex = 0;
			ClearChatMessage();
		}
	
		++m_nInsertCount;
		if(strlen(_pszMsg) < CHAT_MESSAGE_LENGTH)
		{
			strcpy(m_stMsg[m_nInsertCount - 1].szMsg, _pszMsg);
			strcpy(m_stMsg[m_nInsertCount - 1].szSender, _pszSender);
			m_stMsg[m_nInsertCount - 1].dwExtraMsg = COLOR_PRIVATECHAT;
			m_stMsg[m_nInsertCount - 1].nNodeType = MSGNODE_PRIVAECHAT;
		}
	
		if(m_bIsMaxSize)
		{
			if(m_nShowIndex + MAX_SHOW_LINE < m_nInsertCount)
			{
				m_nShowIndex = m_nInsertCount - MAX_SHOW_LINE;
			}
		}
		else
		{
			m_nShowIndex = m_nInsertCount - MIN_SHOW_LINE;
		}*/
	InsertRawMessage(_pszSender, _pszMsg, MSGNODE_PRIVAECHAT, COLOR_PRIVATECHAT);
}

void GameChatHistory::InsertPublicChatMessage(const char* _pszSender, const char* _pszMsg, DWORD _dwColor /* = 0 */)
{
	/*
	if(strlen(_pszSender) > 19)
		{
			return;
		}
	
		if(m_nInsertCount >= CHAT_HISTORY_NUMBER - 1)
		{
			m_nInsertCount = 0;
			m_nShowIndex = 0;
			ClearChatMessage();
		}
	
		++m_nInsertCount;
		if(strlen(_pszMsg) < CHAT_MESSAGE_LENGTH)
		{
			strcpy(m_stMsg[m_nInsertCount - 1].szMsg, _pszMsg);
			strcpy(m_stMsg[m_nInsertCount - 1].szSender, _pszSender);
			m_stMsg[m_nInsertCount - 1].dwExtraMsg = _dwColor;
			m_stMsg[m_nInsertCount - 1].nNodeType = MSGNODE_PUBLICCHAT;
		}
	
		if(m_bIsMaxSize)
		{
			if(m_nShowIndex + MAX_SHOW_LINE < m_nInsertCount)
			{
				m_nShowIndex = m_nInsertCount - MAX_SHOW_LINE;
			}
		}
		else
		{
			m_nShowIndex = m_nInsertCount - MIN_SHOW_LINE;
		}*/
	InsertRawMessage(_pszSender, _pszMsg, MSGNODE_PUBLICCHAT, _dwColor);
}

void GameChatHistory::InsertChatMessage(const char* _pszMsg, DWORD _dwExtra /* = 0 */)
{
	/*
	if(m_nInsertCount >= CHAT_HISTORY_NUMBER - 1)
		{
			m_nInsertCount = 0;
			m_nShowIndex = 0;
			ClearChatMessage();
		}
	
		++m_nInsertCount;
		if(strlen(_pszMsg) < CHAT_MESSAGE_LENGTH)
		{
			strcpy(m_stMsg[m_nInsertCount - 1].szMsg, _pszMsg);
			m_stMsg[m_nInsertCount - 1].dwExtraMsg = _dwExtra;
		}
	
		if(m_bIsMaxSize)
		{
			if(m_nShowIndex + MAX_SHOW_LINE < m_nInsertCount)
			{
				m_nShowIndex = m_nInsertCount - MAX_SHOW_LINE;
			}
		}
		else
		{
			m_nShowIndex = m_nInsertCount - MIN_SHOW_LINE;
		}*/
	InsertRawMessage(NULL, _pszMsg, MSGNODE_NORMAL, _dwExtra);
}

void GameChatHistory::InsertChatMessage(const char* _pszName, const char* _pszMsg, DWORD _dwExtra /* = 0 */)
{
	if(strlen(_pszName) + strlen(_pszMsg) >= CHAT_MESSAGE_LENGTH)
	{
		return;
	}

	char szText[CHAT_MESSAGE_LENGTH] = {0};
	sprintf(szText, "%s :%s", _pszName, _pszMsg);
	InsertRawMessage(NULL, szText, MSGNODE_NORMAL, _dwExtra);
}

void GameChatHistory::InsertMapSelfChatMessage(const char* _pszName, const char* _pszMsg, DWORD _dwExtra/* = 0*/)
{
	if(strlen(_pszName) + strlen(_pszMsg) >= CHAT_MESSAGE_LENGTH)
	{
		return;
	}

	char szText[CHAT_MESSAGE_LENGTH] = {0};
	sprintf(szText, "[地图]%s说:%s", _pszName, _pszMsg);
	InsertRawMessage(NULL, szText, MSGNODE_NORMAL, _dwExtra);
}

void GameChatHistory::InsertMapChatMessage(const char* _pszSender, const char* _pszMsg, DWORD _dwColor/* = 0*/)
{
	if(strlen(_pszSender) + strlen(_pszMsg) >= CHAT_MESSAGE_LENGTH)
	{
		return;
	}

	char szText[CHAT_MESSAGE_LENGTH] = {0};
	sprintf(szText, "[地图]%s说:%s", _pszSender, _pszMsg);
	InsertRawMessage(_pszSender, szText, MSGNODE_PUBLICCHAT, _dwColor);
}

void GameChatHistory::ClearChatMessage()
{
	for(int i = 0; i < CHAT_HISTORY_NUMBER; ++i)
	{
		m_stMsg[i].szMsg[0] = 0;
		m_stMsg[i].nNodeType = 0;
		m_stMsg[i].szSender[0] = 0;
	}
}

void GameChatHistory::InsertRawMessage(const char *_pszSender, const char *_pszMsg, int _nNodeType, DWORD _dwExtra /*= 0*/)
{
	if(NULL == _pszMsg)
	{
		return;
	}

	if(_nNodeType != MSGNODE_NORMAL &&
		_nNodeType != MSGNODE_PRIVAECHAT &&
		_nNodeType != MSGNODE_PUBLICCHAT)
	{
		return;
	}

	if(_nNodeType == MSGNODE_NORMAL ||
		_nNodeType == MSGNODE_PUBLICCHAT)
	{
		//	nothing
	}
	else if(_nNodeType == MSGNODE_PRIVAECHAT)
	{
		if(NULL == _pszSender)
		{
			return;
		}
	}

	if(strlen(_pszMsg) >= CHAT_MESSAGE_LENGTH)
	{
		return;
	}
	if(NULL != _pszSender)
	{
		if(strlen(_pszSender) > 19)
		{
			return;
		}
	}

	StringList xStrings;
	int nLines = SplitTextWithLength(_pszMsg, 12, TEXT_WIDTH, xStrings);

	if(0 == nLines)
	{
		return;
	}


	if(0 == nLines)
	{
		return;
	}

	if(m_nInsertCount + nLines >= CHAT_HISTORY_NUMBER)
	{
		m_nInsertCount = 0;
		m_nShowIndex = 0;
		ClearChatMessage();
	}

	m_nInsertCount += nLines;
	int nInsertLines = nLines;

	for(StringList::iterator begIter = xStrings.begin();
		begIter != xStrings.end();
		++begIter)
	{
		string& refStringItem = *begIter;

		strcpy(m_stMsg[m_nInsertCount - nInsertLines].szMsg, refStringItem.c_str());
		if(NULL != _pszSender)
		{
			strcpy(m_stMsg[m_nInsertCount - nInsertLines].szSender, _pszSender);
		}
		else
		{
			m_stMsg[m_nInsertCount - nInsertLines].szSender[0] = 0;
		}
		
		m_stMsg[m_nInsertCount - nInsertLines].dwExtraMsg = _dwExtra;
		m_stMsg[m_nInsertCount - nInsertLines].nNodeType = _nNodeType;

		if(_nNodeType == MSGNODE_PRIVAECHAT)
		{
			m_stMsg[m_nInsertCount - nInsertLines].dwExtraMsg = COLOR_PRIVATECHAT;
		}

		--nInsertLines;
	}

	if(m_bIsMaxSize)
	{
		if(m_nShowIndex + MAX_SHOW_LINE < m_nInsertCount)
		{
			m_nShowIndex = m_nInsertCount - MAX_SHOW_LINE;
		}
	}
	else
	{
		m_nShowIndex = m_nInsertCount - MIN_SHOW_LINE;
	}
}
/************************************************************************/
/* Class GameChatHistoryEx
/************************************************************************/
GameChatHistoryEx::GameChatHistoryEx()
{
	m_nShowIndex = 0;
	m_nInsertCount = 0;
	m_dwAlpha = LOSEFOCUS_ALPHA;
	m_bVisible = true;
	m_bIsMaxSize = true;
	memset(m_pChatDataItems, 0, sizeof(m_pChatDataItems));

	m_rcClient.left = 230 + 15;
	m_rcClient.top = 450 + 60;
	m_rcClient.right = 486 + 15 + 100;
	m_rcClient.bottom = 450 + 60 + 145;

	m_hBk = GameScene::sThis->GetMainOpt()->GetResourceMgr()->GetTexture("bmcolor");
	m_pRender = new hgeSprite(m_hBk, 0, 0, m_rcClient.right - m_rcClient.left,
		m_rcClient.bottom - m_rcClient.top);
	m_xChatDataModel.SetPageItems(CHAT_SHOW_LINE);
	m_xChatDataModel.Init(CHAT_HISTORY_NUMBER);
}

GameChatHistoryEx::~GameChatHistoryEx()
{
	SAFE_DELETE(m_pRender);
}

//////////////////////////////////////////////////////////////////////////
void GameChatHistoryEx::Render()
{
	if(!m_pRender)
	{
		return;
	}
	if(!IsVisible())
	{
		return;
	}

	hgeResourceManager* pResMgr = pTheGame->GetGameRes();
	HTEXTURE tex = 0;

	//	back ground

	for(int i = 0; i < CHAT_SHOW_LINE; ++i)
	{
		ChatDataItem* pItem = m_pChatDataItems[i];
		if(NULL == pItem)
		{
			break;
		}
		if(pItem->szMsg[0] == 0)
		{
			break;
		}

		//	render text
		DWORD dwUseColor = pItem->dwExtraMsg;
		AfxGetPrinter()->SetColor(dwUseColor);

		AfxGetPrinter()->PrintWithoutStroke(m_rcClient.left + 5,
			m_rcClient.top + 5 + i * 13,
			pItem->szMsg);
	}
}

void GameChatHistoryEx::Update(float _dt)
{

}

bool GameChatHistoryEx::ProcUserCmd(const POINT& _mp)
{
	if(!PtInRect(&GetRect(),_mp))
	{
		return false;
	}

	int nLine = (_mp.y - m_rcClient.top - 5) / 13;
	if(nLine >= 0 &&
		nLine < CHAT_SHOW_LINE)
	{
		if(AfxGetHge()->Input_KeyUp(HGEK_RBUTTON))
		{
			ChatDataItem* pNode = m_pChatDataItems[nLine];
			if((pNode->nNodeType == MSGNODE_PRIVAECHAT || pNode->nNodeType == MSGNODE_PUBLICCHAT) &&
				pNode->szSender[0] != 0)
			{
				GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->GainFocus();
				GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->ClearText();
				GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->SetText("/");
				GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->SetText(pNode->szSender);
				GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->SetText(" ");
				GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->SetVisible(true);
			}
		}
	}
	return true;
}

void GameChatHistoryEx::ScrollUp()
{
	m_xChatDataModel.ScrollUp();
	m_xChatDataModel.PopItem(m_pChatDataItems, CHAT_SHOW_LINE);
}

void GameChatHistoryEx::SetMaxSize(bool _b)
{
	
}

void GameChatHistoryEx::ScrollDown()
{
	m_xChatDataModel.ScrollDown();
	m_xChatDataModel.PopItem(m_pChatDataItems, CHAT_SHOW_LINE);
}

void GameChatHistoryEx::ScrollToTop()
{
	m_xChatDataModel.ScrollToTop();
	m_xChatDataModel.PopItem(m_pChatDataItems, CHAT_SHOW_LINE);
}

void GameChatHistoryEx::ScrollToBottom()
{
	m_xChatDataModel.ScrollToBottom();
	m_xChatDataModel.PopItem(m_pChatDataItems, CHAT_SHOW_LINE);
}

void GameChatHistoryEx::InsertPrivateSelfChatMessage(const char* _pszSender, const char* _pszMsg)
{
	InsertRawMessage(_pszSender, _pszMsg, MSGNODE_PRIVAECHAT, COLOR_PRIVATECHAT);
}

void GameChatHistoryEx::InsertPrivateChatMessage(const char* _pszSender, const char* _pszMsg)
{
	InsertRawMessage(_pszSender, _pszMsg, MSGNODE_PRIVAECHAT, COLOR_PRIVATECHAT);
}

void GameChatHistoryEx::InsertPublicChatMessage(const char* _pszSender, const char* _pszMsg, DWORD _dwColor /* = 0 */)
{
	InsertRawMessage(_pszSender, _pszMsg, MSGNODE_PUBLICCHAT, _dwColor);
}

void GameChatHistoryEx::InsertChatMessage(const char* _pszMsg, DWORD _dwExtra /* = 0 */)
{
	InsertRawMessage(NULL, _pszMsg, MSGNODE_NORMAL, _dwExtra);
}

void GameChatHistoryEx::InsertChatMessage(const char* _pszName, const char* _pszMsg, DWORD _dwExtra /* = 0 */)
{
	if(strlen(_pszName) + strlen(_pszMsg) >= CHAT_MESSAGE_LENGTH)
	{
		return;
	}

	char szText[CHAT_MESSAGE_LENGTH] = {0};
	sprintf(szText, "%s :%s", _pszName, _pszMsg);
	InsertRawMessage(NULL, szText, MSGNODE_NORMAL, _dwExtra);
}

void GameChatHistoryEx::InsertMapSelfChatMessage(const char* _pszName, const char* _pszMsg, DWORD _dwExtra/* = 0*/)
{
	if(strlen(_pszName) + strlen(_pszMsg) >= CHAT_MESSAGE_LENGTH)
	{
		return;
	}

	char szText[CHAT_MESSAGE_LENGTH] = {0};
	sprintf(szText, "[地图]%s说:%s", _pszName, _pszMsg);
	InsertRawMessage(NULL, szText, MSGNODE_NORMAL, _dwExtra);
}

void GameChatHistoryEx::InsertMapChatMessage(const char* _pszSender, const char* _pszMsg, DWORD _dwColor/* = 0*/)
{
	if(strlen(_pszSender) + strlen(_pszMsg) >= CHAT_MESSAGE_LENGTH)
	{
		return;
	}

	char szText[CHAT_MESSAGE_LENGTH] = {0};
	sprintf(szText, "[地图]%s说:%s", _pszSender, _pszMsg);
	InsertRawMessage(_pszSender, szText, MSGNODE_PUBLICCHAT, _dwColor);
}

void GameChatHistoryEx::ClearChatMessage()
{

}

void GameChatHistoryEx::InsertRawMessage(const char *_pszSender, const char *_pszMsg, int _nNodeType, DWORD _dwExtra /*= 0*/)
{
	if(NULL == _pszMsg)
	{
		return;
	}

	if(_nNodeType != MSGNODE_NORMAL &&
		_nNodeType != MSGNODE_PRIVAECHAT &&
		_nNodeType != MSGNODE_PUBLICCHAT)
	{
		return;
	}

	if(_nNodeType == MSGNODE_NORMAL ||
		_nNodeType == MSGNODE_PUBLICCHAT)
	{
		//	nothing
	}
	else if(_nNodeType == MSGNODE_PRIVAECHAT)
	{
		if(NULL == _pszSender)
		{
			return;
		}
	}

	if(strlen(_pszMsg) >= CHAT_MESSAGE_LENGTH)
	{
		return;
	}
	if(NULL != _pszSender)
	{
		if(strlen(_pszSender) > 19)
		{
			return;
		}
	}

	StringList xStrings;
	int nLines = SplitTextWithLength(_pszMsg, 12, TEXT_WIDTH, xStrings);

	if(0 == nLines)
	{
		return;
	}

	for(StringList::iterator begIter = xStrings.begin();
		begIter != xStrings.end();
		++begIter)
	{
		//	add one item
		string& refStringItem = *begIter;
		ChatDataItem* pNewItem = m_xChatDataModel.AddChatDataItem(refStringItem.c_str());

		//	sender
		if(NULL != _pszSender)
		{
			strcpy(pNewItem->szSender, _pszSender);
		}
		else
		{
			pNewItem->szSender[0] = 0;
		}
		
		pNewItem->dwExtraMsg = _dwExtra;
		pNewItem->nNodeType = _nNodeType;

		if(_nNodeType == MSGNODE_PRIVAECHAT)
		{
			pNewItem->dwExtraMsg = COLOR_PRIVATECHAT;
		}
	}

	m_xChatDataModel.PopItem(m_pChatDataItems, CHAT_SHOW_LINE);
}
/************************************************************************/
/* Class GameLineEdit
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
int __stdcall EditCanLoseFocus(void* _pEdit)
{
	float x, y;
	AfxGetHge()->Input_GetMousePos(&x, &y);

	RECT rcHistory = GameMainOptUI::GetInstance()->GetChatDlg()->GetHistoryDlg()->GetRect();
	POINT ptMouse = {x, y};

	if(PtInRect(&rcHistory, ptMouse))
	{
		return 1;
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
GameLineEdit::GameLineEdit()
{
	//m_rcClient.left = 42;
	//m_rcClient.top = 390;
	/*m_rcClient.right = 250;
	m_rcClient.bottom = 395;*/
	//m_rcClient.right = 250;
	//m_rcClient.bottom = 0;
	m_rcClient.left = 230 + 80;
	m_rcClient.top = 450 + 125;
	/*m_rcClient.right = 250;
	m_rcClient.bottom = 395;*/
	m_rcClient.right = 438 + 80;
	m_rcClient.bottom = 0;
	m_pEdit = new MyGfxEdit(208 - 23, ARGB(255, 255, 255, 255));
	m_bAcceptEnterInput = false;
	m_pEdit->SetEnterKeyCallback(&GameScene::OnEnterPressed);
	m_pEdit->SetOnLoseFocusCallback(&GameScene::OnLoseFocus);
	//m_pEdit->SetCanLoseFocus(&EditCanLoseFocus);
	m_pEdit->SetRenderBorder(false);
	m_pRender = NEW_EMPTY_SPRITE;
	m_pRender->SetColor(CHAT_UI_COLOR);

	SetVisible(false);
}

GameLineEdit::~GameLineEdit()
{
	delete m_pEdit;
	m_pEdit = NULL;
	SAFE_DELETE(m_pRender);
}

//////////////////////////////////////////////////////////////////////////
void GameLineEdit::Render()
{
	HTEXTURE tex = 0;
	hgeResourceManager* pMgr = pTheGame->GetGameRes();

	if(!IsVisible())
	{
		return;
	}

	tex = pMgr->GetTexture("line");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 255, 22);
		m_pRender->Render(m_rcClient.left, m_rcClient.top - 5);
	}

	m_pEdit->Render((float)m_rcClient.left + 39, (float)m_rcClient.top);

}

void GameLineEdit::Update(float _dt)
{
	if(!IsVisible())
	{
		return;
	}
	m_pEdit->Updata();
}

bool GameLineEdit::ProcUserCmd(const POINT& _mp)
{
	if(::PtInRect(&m_rcClient, _mp))
	{
		//	Check if the left mouse button is pressed
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			m_pEdit->SetFocus();
		}
	}
	else
	{
		//	The label will lose focus when user click the left mouse button
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			m_pEdit->KillFocus();
		}
	}

	if(m_bAcceptEnterInput)
	{
		if(AfxGetHge()->Input_KeyUp(HGEK_ENTER))
		{
			m_pEdit->SetFocus();
		}
	}

	return true;
}

/************************************************************************/
/* Class GameChatDlg
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
GameChatDlg::GameChatDlg()
{
	m_pHistoryDlg = new GameChatHistoryEx;
	m_pLineEdit = new GameLineEdit;
	assert(_CrtCheckMemory() && "AFTER");
	m_rcClient.left = 30;
	m_rcClient.right = 250;
	m_rcClient.top = 380;
	m_rcClient.bottom = 395;

	m_rcClient.left = 0;
	m_rcClient.right = 0;
	m_rcClient.top = 0;
	m_rcClient.bottom = 0;

	ZeroMemory(m_rcChild, sizeof(m_rcChild));
	RECT* pRc = NULL;
	//	Send button
	pRc = &m_rcChild[CR_SEND];
	pRc->left = m_pLineEdit->GetRect().left;
	pRc->right = pRc->left + 35;
	pRc->top = m_pLineEdit->GetRect().top;
	pRc->bottom = pRc->top + 20;

	//	Scroll up button
	pRc = &m_rcChild[CR_SCROLLUP];
	pRc->left = 224;
	pRc->right = pRc->left + 15;
	pRc->top = 450 + 63;
	pRc->bottom = pRc->top + 14;

	pRc = &m_rcChild[CR_SCROLLDOWN];
	pRc->left = 224;
	pRc->right = pRc->left + 15;
	pRc->top = 117 + 450 - 14;
	pRc->bottom = pRc->top + 14;

	m_bHide = false;

	for(int i = 0 ; i < CR_TOTAL; ++i)
	{
		m_bBtnState[i] = BTN_STATE_NORMAL;
	}

	m_pRender = NEW_EMPTY_SPRITE;
	m_pRender->SetColor(CHAT_UI_COLOR);
}

GameChatDlg::~GameChatDlg()
{
	SAFE_DELETE(m_pLineEdit);
	SAFE_DELETE(m_pRender);
	SAFE_DELETE(m_pHistoryDlg);
}

//////////////////////////////////////////////////////////////////////////
void GameChatDlg::Render()
{
	HTEXTURE tex = 0;
	hgeResourceManager* pMgr = pTheGame->GetGameRes();

	if(m_bHide)
	{
		m_pLineEdit->Render();
		m_pHistoryDlg->Render();

		if(m_bBtnState[0] != BTN_STATE_NORMAL)
		{
			tex = pMgr->GetTexture("send");
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, m_bBtnState[0] * 20, 35, 20);
				m_pRender->Render(0, m_pLineEdit->GetRect().top - 5 + 1);
			}
		}

		tex = pMgr->GetTexture("arrowbtnup");
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, m_bBtnState[3] * 24, 23, 24);
			m_pRender->Render(/*255 - 23*/0, m_pLineEdit->GetRect().top /*- 6*/+20);
		}

		if(m_bBtnState[1] != BTN_STATE_NORMAL)
		{
			tex = pMgr->GetTexture("scrollup");
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, m_bBtnState[1] * 14, 15, 14);
				m_pRender->Render(0 + 1, m_pHistoryDlg->GetRect().top + 130 - 33);
			}
		}

		if(m_bBtnState[2] != BTN_STATE_NORMAL)
		{
			tex = pMgr->GetTexture("scrolldown");
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, m_bBtnState[2] * 14, 15, 14);
				m_pRender->Render(0 + 1, m_pHistoryDlg->GetRect().top + 130 - 33 + 17);
			}
		}
	}
	else
	{
		m_pLineEdit->Render();
		m_pHistoryDlg->Render();

		//	Render the buttons
		//	Send button
		if(m_bBtnState[CR_SEND] != BTN_STATE_NORMAL &&
			m_pLineEdit->IsVisible())
		{
			tex = pMgr->GetTexture("send");
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, m_bBtnState[CR_SEND] * 20, 35, 20);
				m_pRender->Render(m_rcChild[CR_SEND].left, m_rcChild[CR_SEND].top - 5 + 1);
			}
		}
		
		//	Scroll up button
		//if(m_bBtnState[CR_SCROLLUP] != BTN_STATE_NORMAL)
		{
			tex = pMgr->GetTexture("scrollup");
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, m_bBtnState[CR_SCROLLUP] * 14, 15, 14);
				m_pRender->Render(m_rcChild[CR_SCROLLUP].left, m_rcChild[CR_SCROLLUP].top);
			}
		}

		//	Scroll down button
		//if(m_bBtnState[CR_SCROLLDOWN] != BTN_STATE_NORMAL)
		{
			tex = pMgr->GetTexture("scrolldown");
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, m_bBtnState[CR_SCROLLDOWN] * 14, 15, 14);
				m_pRender->Render(m_rcChild[CR_SCROLLDOWN].left, m_rcChild[CR_SCROLLDOWN].top);
			}
		}
	}
}

void GameChatDlg::Update(float _dt)
{
	m_pLineEdit->Update(_dt);
}

bool GameChatDlg::ProcUserCmd(const POINT& _mp)
{
	/*if(m_pHistoryDlg->IsPtInObject(_mp))
	{
		m_pHistoryDlg->ProcUserCmd(_mp);
	}*/
	/*if(::PtInRect(&m_rcSend, _mp))
	{
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			//	Send
			GameScene::sThis->SendChatMessage();
		}
	}*/
	m_pHistoryDlg->ProcUserCmd(_mp);

	ResetStatus();

	for(int i = CR_SEND; i <= CR_SCROLLDOWN; ++i)
	{
		if(::PtInRect(&m_rcChild[i], _mp))
		{
			return OnBtn(i);
		}
	}
	return false;
}

bool GameChatDlg::OnBtn(int _idx)
{
	m_bBtnState[_idx] = BTN_STATE_OVER;
	if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
	{
		m_bBtnState[_idx] = BTN_STATE_DOWN;
	}
	bool bUp = AfxGetHge()->Input_KeyUp(HGEK_LBUTTON);

	static float s_fPressUpTime = 0;
	static float s_fPressDownTime = 0;

	if(_idx == CR_SCROLLUP)
	{
		if(m_bBtnState[CR_SCROLLUP] != BTN_STATE_DOWN &&
			!bUp)
		{
			s_fPressUpTime = 0;
		}
		else
		{
			s_fPressUpTime += AfxGetHge()->Timer_GetDelta();
		}

		if(AfxGetHge()->Input_KeyDown(HGEK_LBUTTON))
		{
			s_fPressUpTime = 0;
		}
	}
	else if(_idx == CR_SCROLLDOWN)
	{
		if(m_bBtnState[CR_SCROLLDOWN] != BTN_STATE_DOWN &&
			!bUp)
		{
			s_fPressDownTime = 0;
		}
		else
		{
			s_fPressDownTime += AfxGetHge()->Timer_GetDelta();
		}

		if(AfxGetHge()->Input_KeyDown(HGEK_LBUTTON))
		{
			s_fPressDownTime = 0;
		}
	}

	const float fBigScrollTime = 0.5f;

	switch(_idx)
	{
	case CR_SEND:
		{
			if(m_pLineEdit->IsVisible())
			{
				if(bUp)
				{
					GameScene::sThis->SendChatMessage();
				}
			}
			else
			{
				return false;
			}
		}break;
	case CR_SCROLLUP:
		{
			if(bUp)
			{
				if(s_fPressUpTime > fBigScrollTime)
				{
					m_pHistoryDlg->ScrollToTop();
				}
				else
				{
					m_pHistoryDlg->ScrollUp();
					s_fPressUpTime = 0;
				}
			}
		}break;
	case CR_SCROLLDOWN:
		{
			if(bUp)
			{
				if(s_fPressDownTime > fBigScrollTime)
				{
					m_pHistoryDlg->ScrollToBottom();
				}
				else
				{
					m_pHistoryDlg->ScrollDown();
					s_fPressDownTime = 0;
				}
			}
		}break;
	}

	return true;
}

void GameChatDlg::ResetStatus()
{
	__super::ResetStatus();
	for(int i = 0 ; i < CR_TOTAL; ++i)
	{
		m_bBtnState[i] = BTN_STATE_NORMAL;
	}
}