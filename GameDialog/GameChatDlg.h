#ifndef _INC_GAME_CHATDLG_
#define _INC_GAME_CHATDLG_
//////////////////////////////////////////////////////////////////////////
#include "../Common/GameRenderObject.h"
#include "../Common/GfxEdit.h"
#include "ChatDataModel.h"
//////////////////////////////////////////////////////////////////////////
//typedef MyGfxEdit EditWidget;
class GameChatDlg;
//////////////////////////////////////////////////////////////////////////
#define CHAT_HISTORY_NUMBER	50
//#define CHAT_MESSAGE_LENGTH	100
#define MAX_SHOW_LINE		4
#define MIN_SHOW_LINE		1
#define GETFOCUS_ALPHA		0x80000000
#define LOSEFOCUS_ALPHA		0x60000000

#define CHAT_BTN_NUMBER		4
//////////////////////////////////////////////////////////////////////////
class GameLineEdit : public RenderObject
{
	friend class GameChatDlg;

public:
	GameLineEdit();
	virtual ~GameLineEdit();

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);

public:
	inline bool IsFocused()
	{
		return m_pEdit->IsFocused();
	}
	inline void ChangeFocus()
	{
		bool bFocus = IsFocused();
		if(bFocus)
		{
			m_pEdit->KillFocus();
		}
		else
		{
			m_pEdit->SetFocus();
		}
	}
	inline const char* GetText()
	{
		return m_pEdit->GetCookie();
	}
	inline void ClearText()
	{
		m_pEdit->ClearCookie();
	}
	inline void LoseFocus()
	{
		m_pEdit->KillFocus();
	}
	inline void GainFocus()
	{
		m_pEdit->SetFocus();
	}
	inline void SetText(const char* _pszText)
	{
		m_pEdit->InsertCookie(_pszText);
	}

private:
	MyGfxEdit* m_pEdit;
	bool m_bAcceptEnterInput;
};
//////////////////////////////////////////////////////////////////////////
#define MSGNODE_NORMAL				0
#define MSGNODE_PRIVAECHAT			1
#define MSGNODE_PUBLICCHAT			2

struct MsgNode
{
	char szMsg[CHAT_MESSAGE_LENGTH];
	char szSender[20];
	DWORD dwExtraMsg;
	BYTE bLines;
	int nNodeType;

	MsgNode()
	{
		szMsg[0] = 0;
		szSender[0] = 0;
		dwExtraMsg = 0;
		bLines = 0;
		nNodeType = 0;
	}
};
//////////////////////////////////////////////////////////////////////////
class GameChatHistory : public RenderObject
{
public:
	GameChatHistory();
	virtual ~GameChatHistory();

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);

public:
	void InsertChatMessage(const char* _pszMsg, DWORD _dwExtra = 0);
	void InsertChatMessage(const char* _pszName, const char* _pszMsg, DWORD _dwExtra = 0);
	void InsertMapSelfChatMessage(const char* _pszName, const char* _pszMsg, DWORD _dwExtra = 0);
	void InsertMapChatMessage(const char* _pszSender, const char* _pszMsg, DWORD _dwColor = 0);
	void InsertPrivateChatMessage(const char* _pszSender, const char* _pszMsg);
	void InsertPrivateSelfChatMessage(const char* _pszSender, const char* _pszMsg);
	void InsertPublicChatMessage(const char* _pszSender, const char* _pszMsg, DWORD _dwColor = 0);
	void ClearChatMessage();

	inline void SetDlgFocus(bool _bFocus)
	{
		if(_bFocus)
		{
			m_dwAlpha = GETFOCUS_ALPHA;
		}
		else
		{
			m_dwAlpha = LOSEFOCUS_ALPHA;
		}
	}

	void ScrollUp();
	void ScrollDown();
	void SetMaxSize(bool _b);

protected:
	void InsertRawMessage(const char* _pszSender, const char* _pszMsg, int _nNodeType, DWORD _dwExtra = 0);

private:
	RECT m_stRcScroll;
	RECT m_stRcUp;
	RECT m_stRcDown;
	DWORD m_dwAlpha;

	MsgNode m_stMsg[CHAT_HISTORY_NUMBER];
	int m_nShowIndex;
	int m_nInsertCount;

	HTEXTURE m_hBk;
	bool m_bIsMaxSize;
};
//////////////////////////////////////////////////////////////////////////
#define CHAT_SHOW_LINE		4
class GameChatHistoryEx : public RenderObject
{
public:
	GameChatHistoryEx();
	virtual ~GameChatHistoryEx();

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);

public:
	void InsertChatMessage(const char* _pszMsg, DWORD _dwExtra = 0);
	void InsertChatMessage(const char* _pszName, const char* _pszMsg, DWORD _dwExtra = 0);
	void InsertMapSelfChatMessage(const char* _pszName, const char* _pszMsg, DWORD _dwExtra = 0);
	void InsertMapChatMessage(const char* _pszSender, const char* _pszMsg, DWORD _dwColor = 0);
	void InsertPrivateChatMessage(const char* _pszSender, const char* _pszMsg);
	void InsertPrivateSelfChatMessage(const char* _pszSender, const char* _pszMsg);
	void InsertPublicChatMessage(const char* _pszSender, const char* _pszMsg, DWORD _dwColor = 0);
	void ClearChatMessage();

	inline void SetDlgFocus(bool _bFocus)
	{
		if(_bFocus)
		{
			m_dwAlpha = GETFOCUS_ALPHA;
		}
		else
		{
			m_dwAlpha = LOSEFOCUS_ALPHA;
		}
	}

	void ScrollUp();
	void ScrollDown();
	void ScrollToTop();
	void ScrollToBottom();
	void SetMaxSize(bool _b);

protected:
	void InsertRawMessage(const char* _pszSender, const char* _pszMsg, int _nNodeType, DWORD _dwExtra = 0);

private:
	RECT m_stRcScroll;
	RECT m_stRcUp;
	RECT m_stRcDown;
	DWORD m_dwAlpha;

	ChatDataModel m_xChatDataModel;
	ChatDataItem* m_pChatDataItems[CHAT_SHOW_LINE];
	int m_nShowIndex;
	int m_nInsertCount;

	HTEXTURE m_hBk;
	bool m_bIsMaxSize;
};

//////////////////////////////////////////////////////////////////////////
class GameChatDlg : public RenderObject
{
public:
	enum CHILD_RECT
	{
		CR_SEND,
		CR_SCROLLUP,
		CR_SCROLLDOWN,
		CR_SHOWHIDE,
		CR_TOTAL
	};
public:
	GameChatDlg();
	virtual ~GameChatDlg();

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void ResetStatus();

public:
	inline void SetEnterKeyInput(bool _bEnable)
	{
		m_pLineEdit->m_bAcceptEnterInput = _bEnable;
	}
	inline void ChangeFocus()
	{
		m_pLineEdit->ChangeFocus();

	}
	inline GameLineEdit* GetEdit()
	{
		return m_pLineEdit;
	}
	inline GameChatHistoryEx* GetHistoryDlg()
	{
		return m_pHistoryDlg;
	}
	inline void SetHide(bool _b)
	{
		m_bHide = _b;

		//m_pLineEdit->SetVisible(!m_bHide);
		//m_pHistoryDlg->SetVisible(!m_bHide);
		m_pHistoryDlg->SetMaxSize(!m_bHide);
	}
	inline void SetBtnState(int _idx, int _state)
	{
		m_bBtnState[_idx] = _state;
	}
	inline bool IsHide()
	{
		return m_bHide;
	}
	inline bool IsEditInFocus()
	{
		if(m_pLineEdit)
		{
			return m_pLineEdit->IsFocused();
		}
		return false;
	}

	bool OnBtn(int _idx);

private:
	GameLineEdit* m_pLineEdit;
	GameChatHistoryEx* m_pHistoryDlg;
	//RECT m_rcSend;
	bool m_bHide;

	BYTE m_bBtnState[CR_TOTAL];
	RECT m_rcChild[CR_TOTAL];
};
//////////////////////////////////////////////////////////////////////////
#endif