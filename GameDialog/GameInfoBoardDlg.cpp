#include "GameInfoBoardDlg.h"

GameInfoBoardDlg::GameInfoBoardDlg()
{
	m_pCenterNode = (LPBoardMsgNode)malloc(sizeof(BoardMsgNode) * MAX_BOARDMSG_NUM);
	ZeroMemory(m_pCenterNode, sizeof(BoardMsgNode) * MAX_BOARDMSG_NUM);
	m_font = new GfxFont("宋体", 12, FALSE, FALSE, FALSE);
	m_font->SetColor(ARGB(255, 0, 255, 0));
	m_font->SetKerningHeight(2.0f);

	for(int i = 0; i < MAX_BOARDMSG_NUM; ++i)
	{
		if(i == 0)
		{
			m_pCenterNode[i].pPre = &m_pCenterNode[MAX_BOARDMSG_NUM - 1];
			m_pCenterNode[i].pNext = &m_pCenterNode[1];
		}
		else if(i == MAX_BOARDMSG_NUM - 1)
		{
			m_pCenterNode[i].pNext = &m_pCenterNode[0];
			m_pCenterNode[i].pPre = &m_pCenterNode[MAX_BOARDMSG_NUM - 2];
		}
		else
		{
			m_pCenterNode[i].pNext = &m_pCenterNode[i + 1];
			m_pCenterNode[i].pPre = &m_pCenterNode[i - 1];
		}
	}

	m_pFirstShowNode = m_pLastShowNode = m_pCenterNode;
	m_rcClient.left = 5;
	m_rcClient.top = 5;
}

GameInfoBoardDlg::~GameInfoBoardDlg()
{
	free(m_pCenterNode);
	delete m_font;
}

/*
bool GameInfoBoardDlg::InsertBoardMsg(const char* lpszMsg)
{
	if(m_pLastShowNode->pNext == m_pFirstShowNode)
	{
		//	信息数量超过了
		if(strlen(lpszMsg) >= MAX_BOARDMSG_LENGTH - 1)
		{
			return false;
		}
		else
		{
			//	先移动首
			m_pFirstShowNode = m_pFirstShowNode->pNext;
			strcpy(m_pLastShowNode->szMsg, lpszMsg);
			m_pLastShowNode->dwInsertTime = ::GetTickCount();
			m_pLastShowNode = m_pLastShowNode->pNext;
			return true;
		}
	}
	else
	{
		//	没有超过
		if(strlen(lpszMsg) >= MAX_BOARDMSG_LENGTH - 1)
		{
			return false;
		}
		else
		{
			strcpy(m_pLastShowNode->szMsg, lpszMsg);
			m_pLastShowNode->dwInsertTime = ::GetTickCount();
			m_pLastShowNode = m_pLastShowNode->pNext;
			return true;
		}
	}
}*/

bool GameInfoBoardDlg::InsertBoardMsg(const char* _fmt, ...)
{
	va_list va;

	if(m_pLastShowNode->pNext == m_pFirstShowNode)
	{
		//	信息数量超过了
		if(strlen(_fmt) >= MAX_BOARDMSG_LENGTH - 1)
		{
			return false;
		}
		else
		{
			//	先移动首
			m_pFirstShowNode = m_pFirstShowNode->pNext;

			va_start(va, _fmt);
			vsprintf(m_pLastShowNode->szMsg, _fmt, va);
			va_end(va);
			
			m_pLastShowNode->dwInsertTime = ::GetTickCount();
			m_pLastShowNode->dwCol = ARGB(255, 0, 255, 0);
			m_pLastShowNode = m_pLastShowNode->pNext;
			return true;
		}
	}
	else
	{
		//	没有超过
		if(strlen(_fmt) >= MAX_BOARDMSG_LENGTH - 1)
		{
			return false;
		}
		else
		{
			va_start(va, _fmt);
			vsprintf(m_pLastShowNode->szMsg, _fmt, va);
			va_end(va);

			m_pLastShowNode->dwInsertTime = ::GetTickCount();
			m_pLastShowNode->dwCol = ARGB(255, 0, 255, 0);
			m_pLastShowNode = m_pLastShowNode->pNext;
			return true;
		}
	}
}

bool GameInfoBoardDlg::InsertBoardMsg(DWORD _col, const char* _fmt, ...)
{
	va_list va;

	if(m_pLastShowNode->pNext == m_pFirstShowNode)
	{
		//	信息数量超过了
		if(strlen(_fmt) >= MAX_BOARDMSG_LENGTH - 1)
		{
			return false;
		}
		else
		{
			//	先移动首
			m_pFirstShowNode = m_pFirstShowNode->pNext;

			va_start(va, _fmt);
			vsprintf(m_pLastShowNode->szMsg, _fmt, va);
			va_end(va);

			m_pLastShowNode->dwInsertTime = ::GetTickCount();
			m_pLastShowNode->dwCol = _col;
			m_pLastShowNode = m_pLastShowNode->pNext;
			return true;
		}
	}
	else
	{
		//	没有超过
		if(strlen(_fmt) >= MAX_BOARDMSG_LENGTH - 1)
		{
			return false;
		}
		else
		{
			va_start(va, _fmt);
			vsprintf(m_pLastShowNode->szMsg, _fmt, va);
			va_end(va);

			m_pLastShowNode->dwInsertTime = ::GetTickCount();
			m_pLastShowNode->dwCol = _col;
			m_pLastShowNode = m_pLastShowNode->pNext;
			return true;
		}
	}
}

bool GameInfoBoardDlg::InsertEquipErrorMsg(int _err)
{
	DWORD dwErrCol = ARGB(255, 255, 0, 0);
	switch(_err)
	{
	case REQ_ERR_LEVEL:
		{
			return InsertBoardMsg(dwErrCol, "您的等级不够,无法装备");
		}break;
	case REQ_ERR_DC:
		{
			return InsertBoardMsg(dwErrCol, "您的攻击力不够,无法装备");
		}break;
	case REQ_ERR_MC:
		{
			return InsertBoardMsg(dwErrCol, "您的魔法力不够,无法装备");
		}break;
	case REQ_ERR_SC:
		{
			return InsertBoardMsg(dwErrCol, "您的道术不够,无法装备");
		}break;
	case REQ_ERR_SEX:
		{
			return InsertBoardMsg(dwErrCol, "您的性别不符,无法装备");
		}break;
	default:
		{
			return InsertBoardMsg(dwErrCol, "由于未知原因[%d]无法装备", _err);
		}break;
	}
}

void GameInfoBoardDlg::ShowItemMsg(const ItemAttrib* _pItem)
{
	if(_pItem->type == ITEM_NO)
	{
		return;
	}
	if(m_pLastShowNode->pNext == m_pFirstShowNode)
	{
		//	信息数量超过了
		{
			//	先移动首
			m_pFirstShowNode = m_pFirstShowNode->pNext;
			sprintf(m_pLastShowNode->szMsg, "发现 %s", _pItem->name);
			m_pLastShowNode->dwInsertTime = ::GetTickCount();
			m_pLastShowNode->dwCol = ARGB(255, 0, 255, 0);
			m_pLastShowNode = m_pLastShowNode->pNext;
		}
	}
	else
	{
		//	没有超过
		{
			sprintf(m_pLastShowNode->szMsg, "发现 %s", _pItem->name);
			m_pLastShowNode->dwInsertTime = ::GetTickCount();
			m_pLastShowNode->dwCol = ARGB(255, 0, 255, 0);
			m_pLastShowNode = m_pLastShowNode->pNext;
		}
	}
}

void GameInfoBoardDlg::Clear()
{
	ZeroMemory(m_pCenterNode->szMsg, sizeof(m_pCenterNode->szMsg));
	m_pLastShowNode = m_pFirstShowNode = m_pCenterNode;
}

void GameInfoBoardDlg::Update(float _dt)
{
	DWORD dwCurrentMs = ::GetTickCount();

	while(m_pFirstShowNode != m_pLastShowNode)
	{
		if(dwCurrentMs - m_pFirstShowNode->dwInsertTime> MSG_DELETE_TIME_MS)
		{
			m_pFirstShowNode = m_pFirstShowNode->pNext;
		}
		else
		{
			break;
		}
	}
}

void GameInfoBoardDlg::Render()
{
	LPBoardMsgNode pFirst = m_pFirstShowNode;
	DWORD dwLine = 0;
	while(pFirst != m_pLastShowNode)
	{
		//m_font->SetColor(pFirst->dwCol);
		AfxGetPrinter()->SetColor(pFirst->dwCol);
		AfxGetPrinter()->Print(m_rcClient.left, m_rcClient.top + DRAW_LINE_OFFSET * dwLine, pFirst->szMsg);
		++dwLine;
		pFirst = pFirst->pNext;
	}
}