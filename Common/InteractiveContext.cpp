#include "InteractiveContext.h"
#include "../Common/GlobalFunction.h"
#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <hge.h>

//	Context
InteractiveContext::InteractiveContext()
{
	Reset();
}

InteractiveContext::~InteractiveContext()
{

}

void InteractiveContext::Reset()
{
	m_nCurReadpage = m_nCurWritePage =
		m_nCurReadPageOffsetBegin = m_nCurReadPageOffsetEnd
		= m_nPageSum = m_nCurReadDataLength = 0;
	ZeroMemory(m_szData, sizeof(m_szData));
}

void InteractiveContext::Init()
{
	m_nCurReadpage = m_nCurWritePage = m_nCurReadPageOffsetBegin = m_nCurReadPageOffsetEnd = m_nPageSum = 0;
	m_nPageSum = GetPageCount();
	InternalUpdateDataLength();
	//	同时需要更新第一页的End Offset
	for(int i = 1; (i < strlen(m_szData)) && (i < sizeof(m_szData) - 1); ++i)
	{
		if(m_szData[i] == '/' &&
			m_szData[i + 1] == 'p')
		{
			break;
		}
		++m_nCurReadPageOffsetEnd;
	}
}

bool InteractiveContext::CheckValid()
{
	if(m_szData[0] != '/' &&
		m_szData[1] != 'p')
	{
		return false;
	}

	//for(int i = 0; (i < strlen(m_szData)) && (i < sizeof(m_szData) - 1); ++i)
	for(int i = 0; i < m_nCurReadDataLength; ++i)
	{
		if(m_szData[i] == '/')
		{
			if(m_szData[i + 1] == 'p' ||
				m_szData[i + 1] == 'b' ||
				m_szData[i + 1] == 'h' ||
				m_szData[i + 1] == 't' ||
				m_szData[i + 1] == 'e')
			{
				//	Valid
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

bool InteractiveContext::LoadFromFile(const char* lpszFileName)
{
	HANDLE hFile = ::CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwLength = ::GetFileSize(hFile, NULL);

	DWORD dwRead = 0;
	DWORD dwTotal = 0;
	char value = 0;

	for(int i = 0; i < dwLength; ++i)
	{
		::ReadFile(hFile, &value, 1, &dwRead, NULL);
		if(value == '\n' ||
			value == '\r')
		{
			continue;
		}
		m_szData[strlen(m_szData)] = value;
	}

	::CloseHandle(hFile);
	if(dwTotal == dwLength)
	{
		return true;
	}
	return false;
}

//	外部
void InteractiveContext::AddPage()
{
	int nInsertIdx = strlen(m_szData);
	if(InternalCheck())
	{
		sprintf(m_szData + nInsertIdx, "/p");
		++m_nCurWritePage;
	}
}

void InteractiveContext::AddHead(const char* _text)
{
	int nInsertIdx = strlen(m_szData);
	if(InternalCheck())
	{
		sprintf(m_szData + nInsertIdx, "/h%s", _text);
	}
}

void InteractiveContext::AddText(const char* _text)
{
	int nInsertIdx = strlen(m_szData);
	if(InternalCheck())
	{
		sprintf(m_szData + nInsertIdx, "/t%s", _text);
	}
}

void InteractiveContext::AddButton(int _id, const char* _text)
{
	int nInsertIdx = strlen(m_szData);
	if(InternalCheck())
	{
		sprintf(m_szData + nInsertIdx, "/b%d%s", _id, _text);
	}
}

//	内部
int InteractiveContext::GetPageCount()
{
	int nCounter = 0;
	for(int i = 0; (i < strlen(m_szData)) && (i < sizeof(m_szData) - 1); ++i)
	{
		if(m_szData[i] == '/' &&
			m_szData[i + 1] == 'p')
		{
			++nCounter;
		}
	}

	return nCounter;
}

int InteractiveContext::GetCurPage()
{
	return m_nCurReadpage;
}

const char* InteractiveContext::GetTitle()
{
	m_szRet[0] = '\0';
	int nCounter = 0;

	for(int i = m_nCurReadPageOffsetBegin; i < m_nCurReadPageOffsetEnd; ++i)
	{
		if(m_szData[i] == '/' &&
			m_szData[i + 1] == 'h')
		{
			//	到了检测这儿了
			for(int j = i + 2; j <= m_nCurReadPageOffsetEnd; ++j)
			{
				if(m_szData[j] != '/')
				{
					m_szRet[j - i - 2] = m_szData[j];
					++nCounter;
				}
				else
				{
					break;
				}
			}
			break;
		}
	}

	m_szRet[nCounter] = '\0';
	return m_szRet;
}

const char* InteractiveContext::GetText()
{
	m_szRet[0] = '\0';
	int nCounter = 0;

	for(int i = m_nCurReadPageOffsetBegin; i < m_nCurReadPageOffsetEnd; ++i)
	{
		if(m_szData[i] == '/' &&
			m_szData[i + 1] == 't')
		{
			for(int j = i + 2; j <= m_nCurReadPageOffsetEnd; ++j)
			{
				if(m_szData[j] != '/')
				{
					m_szRet[j - i - 2] = m_szData[j];
					++nCounter;
				}
				else
				{
					break;
				}
			}
			break;
		}
	}

	m_szRet[nCounter] = '\0';
	return m_szRet;
}

const char* InteractiveContext::GetButtonText(int _idx)
{
	m_szRet[0] = '\0';
	int nCounter = 0;
	int nInsertCounter = 0;

	for(int i = m_nCurReadPageOffsetBegin; i < m_nCurReadPageOffsetEnd; ++i)
	{
		if(m_szData[i] == '/' &&
			m_szData[i + 1] == 'b')
		{
			//	按钮
			if(nCounter == _idx)
			{
				for(int j = i + 3; j <= m_nCurReadPageOffsetEnd; ++j)
				{
					if(m_szData[j] >= 48 &&
						m_szData[j] <= 57)
					{
						//	是数字 不计算进去
						continue;
					}
					else if(m_szData[j] != '/')
					{
						//m_szRet[j - i - 3] = m_szData[j];
						//++nInsertCounter;
						m_szRet[nInsertCounter++] = m_szData[j];
					}
					else
					{
						break;
					}
				}
				break;
			}
			++nCounter;
		}
	}

	m_szRet[nInsertCounter] = '\0';
	return m_szRet;
}

int InteractiveContext::GetButtonSum()
{
	int nCounter = 0;

	for(int i = m_nCurReadPageOffsetBegin; i < m_nCurReadPageOffsetEnd; ++i)
	{
		if(m_szData[i] == '/' &&
			m_szData[i + 1] == 'b')
		{
			//	按钮
			++nCounter;
		}
	}

	return nCounter;
}

int InteractiveContext::GetButtonID(int _idx)
{
	int nCounter = 0;
	int nInsertIdx = 0;
	char szValue[10] = {0};

	for(int i = m_nCurReadPageOffsetBegin; i < m_nCurReadPageOffsetEnd; ++i)
	{
		if(m_szData[i] == '/' &&
			m_szData[i + 1] == 'b')
		{
			//	按钮
			if(nCounter == _idx)
			{
				//return m_szData[i + 2] - 48;
				for(int j = i + 2; j < m_nCurReadPageOffsetEnd; ++j)
				{
					if(m_szData[j] >= 48 &&
						m_szData[j] <= 57)
					{
						//	是数字
						szValue[nInsertIdx++] = m_szData[j];
					}
					else
					{
						break;
					}
				}
				break;
			}
			++nCounter;
		}
	}

	//return -1;
	if(szValue[0] == '\0')
	{
		return -1;
	}
	else
	{
		return ::atoi(szValue);
	}
}

bool InteractiveContext::NextPage()
{
	if(m_nCurReadpage + 1 >= m_nPageSum)
	{
		return false;
	}
	++m_nCurReadpage;
	InternalUpdateReadOffset(true);
	return true;
}

bool InteractiveContext::PrevPage()
{
	if(m_nCurReadpage - 1 < 0)
	{
		return false;
	}
	--m_nCurReadpage;
	InternalUpdateReadOffset(false);
	return true;
}



//	Private
bool InteractiveContext::InternalCheck()
{
	int nInsertIdx = strlen(m_szData);
	if(nInsertIdx >= (sizeof(m_szData) - 10))
	{
		assert("缓存不足");
		return false;
	}
	return true;
}

void InteractiveContext::InternalUpdateReadOffset(bool _forward)
{
	//	在前一页的基础上 进行偏移操作
	if(_forward)
	{
		assert(m_szData[m_nCurReadPageOffsetBegin] == '/' &&
			m_szData[m_nCurReadPageOffsetBegin + 1] == 'p');
		assert(m_szData[m_nCurReadPageOffsetEnd + 1] == '/' &&
			m_szData[m_nCurReadPageOffsetEnd + 2] == 'p');

		m_nCurReadPageOffsetBegin = m_nCurReadPageOffsetEnd + 1;
		m_nCurReadPageOffsetEnd = m_nCurReadPageOffsetBegin;

		//for(int i = m_nCurReadPageOffsetBegin + 1; (i < strlen(m_szData)) && (i < sizeof(m_szData) - 1); ++i)
		for(int i = m_nCurReadPageOffsetBegin + 1; i <= m_nCurReadDataLength; ++i)
		{
			if(m_szData[i] == '/' &&
				m_szData[i + 1] == 'p')
			{
				break;
			}
			++m_nCurReadPageOffsetEnd;
		}
	}
	else
	{
		assert(m_szData[m_nCurReadPageOffsetBegin] == '/' &&
			m_szData[m_nCurReadPageOffsetBegin + 1] == 'p');

		m_nCurReadPageOffsetEnd = m_nCurReadPageOffsetBegin - 1;
		m_nCurReadPageOffsetBegin = m_nCurReadPageOffsetEnd;

		for(int i = m_nCurReadPageOffsetEnd; i >0; --i)
		{
			if(m_szData[i] == 'p' &&
				m_szData[i - 1] == '/')
			{
				--m_nCurReadPageOffsetBegin;
				break;
			}
			--m_nCurReadPageOffsetBegin;
		}
	}
}

void InteractiveContext::InternalUpdateDataLength()
{
	for(int i = 0; (i < sizeof(m_szData) - 1) && (i < strlen(m_szData) - 1); ++i)
	{
		if(m_szData[i] == '/' &&
			m_szData[i + 1] == 'e')
		{
			break;
		}
		++m_nCurReadDataLength;
	}
}


//////////////////////////////////////////////////////////////////////////
InteractiveContextEx::InteractiveContextEx() : m_xContext(2048)
{
	//
}
InteractiveContextEx::~InteractiveContextEx()
{
	//
}
//////////////////////////////////////////////////////////////////////////
void InteractiveContextEx::Clear()
{
	m_xContext.Clear();
	if(!m_xNodes.empty())
	{
		CONTEXTNODELIST::const_iterator iter = m_xNodes.begin();
		for(iter;
			iter != m_xNodes.end();
			++iter)
		{
			delete *iter;
		}
		m_xNodes.clear();
	}
}
//////////////////////////////////////////////////////////////////////////
void InteractiveContextEx::AddContextNode(int _x, int _y, const char* _pMsg, int _nId)
{
	size_t nAvailable = m_xContext.GetAvailableLength();
	if(nAvailable < strlen(_pMsg) + 1 + 4)
	{
		AfxGetHge()->System_Log("没有更多的缓存[InteractiveContextEx]");
	}
	else
	{
		int nCurPos = m_xContext.GetLength();
		ContextNode* pNewNode = new ContextNode;
		pNewNode->rcLocate.left = _x;
		pNewNode->rcLocate.top = _y;
		if(_nId > 0)
		{
			pNewNode->rcLocate.right = _x + GetTextWidth(_pMsg, 12);
			pNewNode->rcLocate.bottom = _y + 12;
		}
		pNewNode->dwID = _nId;
		pNewNode->szMsg = (const char*)m_xContext.GetBuffer() + m_xContext.GetLength() + 4;
		m_xContext << _pMsg;
		m_xNodes.push_back(pNewNode);
	}
}
//////////////////////////////////////////////////////////////////////////
int InteractiveContextEx::GetButtonID(int _idx)
{
	if(_idx >= m_xNodes.size())
	{
		return 0;
	}
	CONTEXTNODELIST::const_iterator begiter = m_xNodes.begin();
	CONTEXTNODELIST::const_iterator enditer = m_xNodes.end();
	ContextNode* pNode = NULL;

	for(;begiter != enditer; ++begiter)
	{
		pNode = *begiter;
		if(pNode->dwID == _idx)
		{
			return _idx;
		}
	}
	return 0;
}