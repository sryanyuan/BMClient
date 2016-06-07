#ifndef _INC_GAMEFLOATDLG_
#define _INC_GAMEFLOATDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include "GameDlgBase.h"
#include <list>
#include <string>
//////////////////////////////////////////////////////////////////////////
using std::string;
using std::list;
//////////////////////////////////////////////////////////////////////////
struct FloatMessageItem
{
	string xMsg;
	DWORD dwTimes;
	//	protected
	DWORD dwShowTimes;
	DWORD dwColor;
	float fShowX;
	int nTextLen;
	bool bShowed;
};
typedef list<FloatMessageItem*> FloatMessageList;

enum FloatMessageState
{
	FloatMessageState_Hide,
	FloatMessageState_List,
	FloatMessageState_Message,
	FloatMessageState_Showing,
	FloatMessageState_Hiding,
	FloatMessageState_Waiting
};

class GameFloatDlg : public GameCommonDlg
{
public:
	GameFloatDlg();
	virtual ~GameFloatDlg();

public:
	virtual void Update(float _dt);
	virtual void Render();

public:
	void AddFloatMessage(const char* _pszMsg, DWORD _dwColor = 0xffffffff, DWORD _dwTimes = 1);

protected:
	FloatMessageList m_xMsgList;
	FloatMessageState m_eState;
	DWORD m_dwLastBarShowingTime;
	DWORD m_dwLastBarHidingTime;
	DWORD m_dwLastMsgShowTime;
	DWORD m_dwLastWaitingTime;
	int m_nListBkX;
};
//////////////////////////////////////////////////////////////////////////
#endif