#ifndef _INC_CHATDATAMODEL_
#define _INC_CHATDATAMODEL_
//////////////////////////////////////////////////////////////////////////
#include "CircleBuffer.h"
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////
#define CHAT_MESSAGE_LENGTH 100
//////////////////////////////////////////////////////////////////////////
struct ChatDataItem
{
	char szMsg[CHAT_MESSAGE_LENGTH];
	char szSender[20];
	DWORD dwExtraMsg;
	BYTE bLines;
	int nNodeType;

	ChatDataItem()
	{
		dwExtraMsg = 0;
		bLines = 0;
		nNodeType = 0;
		szMsg[0] = 0;
		szSender[0] = 0;
	}
};
//////////////////////////////////////////////////////////////////////////
class ChatDataModel : public CircleBuffer<ChatDataItem>
{
public:
	ChatDataModel();

public:
	void Reset();
	int PopItem(ChatDataItem** ppChatItems, int _nCount);
	void SetPageItems(int _nCount);
	void ScrollUp();
	void ScrollDown();
	void ScrollToTop();
	void ScrollToBottom();
	ChatDataItem* AddChatDataItem(const char* _pszMsg);

	static void TestMain();

private:
	int m_nTailOffset;
	int m_nPageItems;
};
//////////////////////////////////////////////////////////////////////////
#endif