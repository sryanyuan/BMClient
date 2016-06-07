#ifndef _INC_CDUIDIALOG_
#define _INC_CDUIDIALOG_
//////////////////////////////////////////////////////////////////////////
#include <afxwin.h>
#include "UIlib.h"
#include <string>

using std::string;
using namespace DuiLib;
//////////////////////////////////////////////////////////////////////////
#define dui_msg virtual
//////////////////////////////////////////////////////////////////////////
class CDuiDialog : public CDialog, public INotifyUI, public IMessageFilterUI
{
public:
	CDuiDialog(UINT _nIDTemplate, CWnd* _pParent, const char* _pszXmlFile);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	//	MFC
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

	//	Duilib
	virtual void Notify(TNotifyUI& msg);
	//	此处过滤消息后 默认已处理 不发给Duilib，也不发给MFC
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual bool FilterDUIMessage(UINT _uMsg, WPARAM _wParam, LPARAM _lParam, LRESULT& _lRet);
	virtual bool FilterMFCMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& _lRet);

protected:
	bool InitializeDuiSkin();

	LRESULT Dui_OnNCHitTest(LPARAM _lParam);
	LRESULT Dui_OnSize(WPARAM _wParam, LPARAM _lParam, bool& _bHandled);
	LRESULT Dui_OnGetMaxMinInfo(WPARAM _wParam, LPARAM _lParam, bool& _bHandled);

	//	duilib notify
	dui_msg void OnWindowInit(TNotifyUI& _msg);

protected:
	HICON m_hIcon;
	CPaintManagerUI m_pm;
	string m_xXmlFile;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
//////////////////////////////////////////////////////////////////////////
#endif