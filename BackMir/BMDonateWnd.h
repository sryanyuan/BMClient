#ifndef _INC_BMDONATEWND_
#define _INC_BMDONATEWND_

//////////////////////////////////////////////////////////////////////////
#include <uilib.h>
#include <string>
#include "../../CommonModule/loginsvr.pb.h"
#include "../../CommonModule/DataParser.h"
#include "../../CommonModule/GamePacket.h"

using namespace DuiLib;
//////////////////////////////////////////////////////////////////////////
class BMDonateWnd;

class BMDonateValueWnd : public WindowImplBase {
//class BMDonateValueWnd : public CWindowWnd, public INotifyUI {
public:
	BMDonateValueWnd();
	virtual ~BMDonateValueWnd();

public:
	virtual DuiLib::CDuiString GetSkinFolder();
	virtual DuiLib::CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	//	栈上对象 不用delete
	void OnFinalMessage(HWND /*hWnd*/){}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);

public:
	BMDonateWnd* m_pDonateWnd;
	//CPaintManagerUI m_pm;
	CRichEditUI* m_pEdit;
};


//class BMDonateWnd : public CWindowWnd, public INotifyUI {
class BMDonateWnd : public DuiLib::WindowImplBase {
public:
	BMDonateWnd();
	virtual ~BMDonateWnd();

public:
	virtual DuiLib::CDuiString GetSkinFolder();
	virtual DuiLib::CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	//	栈上对象 不用delete
	void OnFinalMessage(HWND /*hWnd*/){}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);

public:
	void ShowDonateWnd(int _nValue);

	const std::string& GetDonateURL()
	{
		return m_xDonateURL;
	}
	void SetLoginAccount(std::string& _refAccount) {
		m_xLoginAccount = _refAccount;
	}
	void OnRegisterURLHTTPGet(const char* _pData, size_t _uSize);

protected:
	//CPaintManagerUI m_pm;
	std::string m_xDonateURL;
	std::string m_xLoginAccount;
	DuiLib::CWebBrowserUI* m_pWeb;
};
//////////////////////////////////////////////////////////////////////////

#endif