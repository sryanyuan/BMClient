#ifndef _INC_BMPASSWORDWND_
#define _INC_BMPASSWORDWND_
//////////////////////////////////////////////////////////////////////////
#include <uilib.h>
#include <string>
#include "../../CommonModule/loginsvr.pb.h"
#include "../../CommonModule/DataParser.h"
#include "../../CommonModule/GamePacket.h"

using namespace DuiLib;
//////////////////////////////////////////////////////////////////////////
class BMPasswordWnd : public CWindowWnd, public INotifyUI
{
public:
	BMPasswordWnd();
	virtual ~BMPasswordWnd();

public:
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	//	栈上对象 不用delete
	void OnFinalMessage(HWND /*hWnd*/){}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);

public:
	void OnPrepare();

	const std::string& GetAccount()
	{
		return m_xAccount;
	}
	const std::string& GetPassword()
	{
		return m_xPassword;
	}

	void OnRegisterURLHTTPGet(const char* _pData, size_t _uSize);

protected:
	CPaintManagerUI m_pm;
	std::string m_xAccount;
	std::string m_xPassword;
};
//////////////////////////////////////////////////////////////////////////
#endif