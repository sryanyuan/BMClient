#include "ChatDataModel.h"
//////////////////////////////////////////////////////////////////////////
ChatDataModel::ChatDataModel()
{
	m_nTailOffset = 0;
	m_nPageItems = 1;
}

void ChatDataModel::Reset()
{
	
}

void ChatDataModel::SetPageItems(int _nCount)
{
	m_nPageItems = _nCount;
}

int ChatDataModel::PopItem(ChatDataItem** ppChatItems, int _nCount)
{
	int nInsetIndex = 0;
	int nFetchBeginIndex = m_nTailOffset + _nCount;
	memset(ppChatItems, 0, sizeof(ChatDataItem*) * _nCount);

	for(int i = nFetchBeginIndex; i > nFetchBeginIndex - m_nPageItems; --i)
	{
		ChatDataItem* pItem = GetItemFromTail(i);
		if(pItem->szMsg[0] != 0)
		{
			//	valid item
			ppChatItems[nInsetIndex++] = pItem;
		}
	}

	return nInsetIndex;
}

void ChatDataModel::ScrollUp()
{
	//	not reach one page
	if(m_nInsertTimes <= m_nPageItems)
	{
		return;
	}

	if(m_nInsertTimes <= m_nBufferSize)
	{
		//	not full
		int nNextTailOffset = m_nTailOffset + 1;
		int nTailIndex = m_nTailPtr - 1 - nNextTailOffset;
		int nHeadIndex = nTailIndex - m_nPageItems + 1;

		if(nHeadIndex >= 0)
		{
			++m_nTailOffset;
		}
	}
	else
	{
		//	already full
		int nFinalIndex = m_nTailPtr % m_nBufferSize;
		if(m_nTailOffset + m_nPageItems <= m_nBufferSize - 1)
		{
			++m_nTailOffset;
		}
	}
}

void ChatDataModel::ScrollDown()
{
	if(m_nInsertTimes <= m_nPageItems)
	{
		return;
	}

	--m_nTailOffset;
	if(m_nTailOffset < 0)
	{
		m_nTailOffset = 0;
	}
}

void ChatDataModel::ScrollToTop()
{
	if(m_nInsertTimes <= m_nPageItems)
	{
		return;
	}

	int nTopIndex = 0;
	if(m_nInsertTimes <= m_nBufferSize)
	{
		nTopIndex = m_nPageItems - 1;
		m_nTailOffset = m_nTailPtr - 1 - nTopIndex;
	}
	else
	{
		m_nTailOffset = m_nBufferSize - m_nPageItems;
	}	
}

void ChatDataModel::ScrollToBottom()
{
	if(m_nInsertTimes <= m_nPageItems)
	{
		return;
	}

	m_nTailOffset = 0;
}

ChatDataItem* ChatDataModel::AddChatDataItem(const char* _pszMsg)
{
	ChatDataItem* pItem = GetCurrentItem();
	SetTailPtrOffset(1);

	//	copy content
	strcpy(pItem->szMsg, _pszMsg);
	return pItem;
}




void ChatDataModel::TestMain()
{
	const int nModelSize = 20;
	const int nModelPage = 10;

	bool stop = false;
	int nMsgSeq = 0;
	int nOptSeq = 0;
	ChatDataModel model;
	model.Init(nModelSize);
	model.SetPageItems(nModelPage);

	while(1)
	{
		char ch = getchar();

		switch (ch)
		{
		case 'q':
			{
				stop = true;
			}break;
		case 'a':
			{
				char szMsg[100];
				sprintf(szMsg, "Message seq %d", nMsgSeq++);
				model.AddChatDataItem(szMsg);
			}break;
		case 'w':
			{
				model.ScrollUp();
			}break;
		case 's':
			{
				model.ScrollDown();
			}break;
		case 't':
			{
				model.ScrollToTop();
			}break;
		case 'b':
			{
				model.ScrollToBottom();
			}break;
		default:
			break;
		}

		while(ch != '\n' &&
			ch != EOF)
		{
			ch = getchar();
		}

		if(stop)
		{
			break;
		}
		else
		{
			printf("==============Operation %d==================\n", nOptSeq++);
			ChatDataItem* ppChatItems[nModelPage] = {0};
			model.PopItem(ppChatItems, nModelPage);

			for(int i = 0; i < nModelPage; ++i)
			{
				if(NULL == ppChatItems[i])
				{
					printf("null\n");
				}
				else
				{
					ChatDataItem* pItem = ppChatItems[i];
					printf("Chat: %s\n", pItem->szMsg);
				}
			}
			printf("\n");
		}
	}
}