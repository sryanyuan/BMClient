#include "BMDonateWnd.h"
#include "../BackMir/GlobalLuaConfig.h"

BMDonateWnd::BMDonateWnd()
{
	m_pWeb = NULL;
}

BMDonateWnd::~BMDonateWnd()
{

}

DuiLib::CDuiString BMDonateWnd::GetSkinFolder()
{
	return DuiLib::CDuiString("");
}
DuiLib::CDuiString BMDonateWnd::GetSkinFile()
{
	return DuiLib::CDuiString("DonateWnd/main.xml");
}


LRESULT BMDonateWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MSG msg;
	msg.hwnd = GetHWND();
	msg.message = uMsg;
	msg.wParam = wParam;
	msg.lParam = lParam;
	TranslateMessage(&msg);

	return __super::HandleMessage(uMsg, wParam, lParam);
	//return __super::HandleMessage(uMsg, wParam, lParam);
	/*if (uMsg == WM_CREATE)
	{
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("DonateWnd/main.xml"), (UINT)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);
		//Init();
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		//PostQuitMessage(0);
	}
	else if (uMsg == WM_ERASEBKGND)
	{
		return 1;
	}
	else if (uMsg == WM_NCACTIVATE) {

		if (!::IsIconic(m_hWnd)) {

			return (wParam == 0) ? TRUE : FALSE;

		}

	}
	else if (uMsg == WM_NCCALCSIZE) {

		return 0;

	}
	else if (uMsg == WM_NCPAINT) {

		return 0;

	}
	else if (uMsg == WM_NCHITTEST)
	{
		//return HTCLIENT;
	}

	LRESULT lRes = 0;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);*/
}

void BMDonateWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == "windowinit")
	{
		m_pWeb = (DuiLib::CWebBrowserUI*)m_PaintManager.FindControl("browser");
		//m_pWeb = (DuiLib::CWebBrowserUI*)m_pm.FindControl("browser");
		//m_pWeb->SetWebBrowserEventHandler(new DuiLib::CWebBrowserEventHandler);
		//m_pWeb->Navigate2();
		// Get donate url
		const std::string& refURL = GetGlobalLuaConfig("gHTTPDonateURL");
		if (refURL.empty())
		{
			::MessageBox(NULL, "无法获取请求", "错误", MB_OK | MB_ICONERROR);
			return;
		}
		m_xDonateURL = refURL;
		m_xDonateURL += "?account=";
		m_xDonateURL += m_xLoginAccount;

		std::string xDonateURL = m_xDonateURL;

		m_pWeb->Navigate2(xDonateURL.c_str());
		m_pWeb->Refresh();
	}
	else if (msg.sType == "click")
	{
		if (0 == strcmp(msg.pSender->GetName(), "closebtn"))
		{
			ShowWindow(false);
			return;
		} else if (0 == strcmp(msg.pSender->GetName(), "button_web")) {
			ShellExecute(NULL, "open", m_xDonateURL.c_str(), NULL, NULL, SW_SHOW);
		}
	}

	__super::Notify(msg);
}

void BMDonateWnd::ShowDonateWnd(int _nValue) {
	char szDonateURL[MAX_PATH];
	//sprintf(szDonateURL, "%s&value=%d", m_xDonateURL.c_str(), _nValue);
	strcpy(szDonateURL, m_xDonateURL.c_str());

	if (NULL != m_pWeb) {
		m_pWeb->Navigate2(szDonateURL);
	}
	ShowWindow(true);
	CenterWindow();
}

void BMDonateWnd::OnRegisterURLHTTPGet(const char* _pData, size_t _uSize)
{
	/*if (NULL == _pData ||
		0 == _uSize)
	{
		::MessageBox(NULL, "无法获得注册信息", "错误", MB_OK | MB_ICONERROR);
		return;
	}

	// parse json object
	cJSON* pRoot = cJSON_Parse(_pData);

	if (NULL == pRoot)
	{
		::MessageBox(NULL, "无法获得注册信息", "错误", MB_OK | MB_ICONERROR);
		return;
	}

	bool bParseOk = false;
	const char* pszErrTip = "解析注册信息失败";
	std::string xRegURL;
	int nCode = -1;

	do
	{
		cJSON* pCode = cJSON_GetObjectItem(pRoot, "code");
		if (NULL == pCode)
		{
			break;
		}
		if (cJSON_Number != pCode->type)
		{
			break;
		}
		nCode = pCode->valueint;

		cJSON* pMsg = cJSON_GetObjectItem(pRoot, "message");
		if (NULL == pMsg)
		{
			break;
		}
		if (cJSON_String != pMsg->type)
		{
			break;
		}
		xRegURL = pMsg->valuestring;
		bParseOk = true;
	} while (0);
	cJSON_Delete(pRoot);
	pRoot = NULL;

	if (!bParseOk)
	{
		::MessageBox(NULL, pszErrTip, "错误", MB_OK | MB_ICONERROR);
		return;
	}
	if (0 != nCode ||
		0 == xRegURL.size())
	{
		::MessageBox(NULL, "无法获得注册页面地址", "错误", MB_OK | MB_ICONERROR);
		return;
	}

	// open url
	ShellExecute(NULL, "open", xRegURL.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);*/
}



// Donate value wnd
BMDonateValueWnd::BMDonateValueWnd()
{
	m_pDonateWnd = NULL;
}

BMDonateValueWnd::~BMDonateValueWnd()
{

}

DuiLib::CDuiString BMDonateValueWnd::GetSkinFolder()
{
	return DuiLib::CDuiString("");
}
DuiLib::CDuiString BMDonateValueWnd::GetSkinFile()
{
	return DuiLib::CDuiString("DonateWnd/value.xml");
}


LRESULT BMDonateValueWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MSG msg;
	msg.hwnd = GetHWND();
	msg.message = uMsg;
	msg.wParam = wParam;
	msg.lParam = lParam;
	TranslateMessage(&msg);

	return __super::HandleMessage(uMsg, wParam, lParam);

	/*if (uMsg == WM_CREATE)
	{
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("DonateWnd/value.xml"), (UINT)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);
		//Init();
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		//PostQuitMessage(0);
	}
	else if (uMsg == WM_ERASEBKGND)
	{
		return 1;
	}
	else if (uMsg == WM_NCACTIVATE) {

		if (!::IsIconic(m_hWnd)) {

			return (wParam == 0) ? TRUE : FALSE;

		}

	}
	else if (uMsg == WM_NCCALCSIZE) {

		return 0;

	}
	else if (uMsg == WM_NCPAINT) {

		return 0;

	}
	else if (uMsg == WM_NCHITTEST)
	{
		//return HTCLIENT;
	}

	LRESULT lRes = 0;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);*/
}

void BMDonateValueWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == "windowinit")
	{
		m_pEdit = (CRichEditUI*)m_PaintManager.FindControl("edit_value");
	}
	else if (msg.sType == "click")
	{
		if (0 == strcmp(msg.pSender->GetName(), "closebtn"))
		{
			ShowWindow(false);
			return;
		} else if (0 == strcmp(msg.pSender->GetName(), "button_donate")) {
			if (NULL == m_pDonateWnd) {
				return;
			}
			if (NULL == m_pEdit) {
				return;
			}

			const char* pszValue = m_pEdit->GetText();
			if (NULL == pszValue) {
				::MessageBox(NULL, "请输入正确的点数", "ERROR", MB_ICONERROR | MB_OK);
				return;
			}

			int nValue = atoi(pszValue);
			if (nValue < 10) {
				::MessageBox(NULL, "请输入正确的点数", "ERROR", MB_ICONERROR | MB_OK);
				return;
			}

			m_pDonateWnd->ShowDonateWnd(nValue);
			ShowWindow(false);
		}
	}

	__super::Notify(msg);
}
