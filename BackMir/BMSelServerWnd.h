#ifndef _INC_BMSELSERVERWND_
#define _INC_BMSELSERVERWND_
//////////////////////////////////////////////////////////////////////////
#include <uilib.h>
#include "../../CommonModule/loginsvr.pb.h"
#include "../../CommonModule/DataParser.h"
#include "../../CommonModule/GamePacket.h"

using namespace DuiLib;
//////////////////////////////////////////////////////////////////////////
class BMSelServerWnd : public CWindowWnd, public INotifyUI
{
public:
	BMSelServerWnd();
	virtual ~BMSelServerWnd();

public:
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	//	栈上对象 不用delete
	void OnFinalMessage(HWND /*hWnd*/){}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);

public:
	void OnPrepare();
	void SetServerList(protocol::MServerListNtf& _refList);
	void AddServer(int _nServerID, const std::string &_refAddr, const std::string &_refName);
	int GetSelServerIndex();
	protocol::MServerListNtf& GetServerList() {
		return m_serverList;
	}

protected:
	CPaintManagerUI m_pm;
	int m_nSelServerIndex;
	protocol::MServerListNtf m_serverList;
};
//////////////////////////////////////////////////////////////////////////
#endif