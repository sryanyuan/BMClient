#include "BMPasswordWnd.h"
#include "GlobalLuaConfig.h"
#include "FastDelegate.h"
#include "../../CommonModule/BMHttpManager.h"
#include "../../CommonModule/cJSON.h"
//////////////////////////////////////////////////////////////////////////
BMPasswordWnd::BMPasswordWnd()
{
	
}

BMPasswordWnd::~BMPasswordWnd()
{

}


LRESULT BMPasswordWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("PasswordWnd/BMPasswordWnd.xml"), (UINT)0, NULL, &m_pm);
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
		return HTCLIENT;
	}

	LRESULT lRes = 0;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void BMPasswordWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == "windowinit")
	{
		OnPrepare();
	}
	else if (msg.sType == "click")
	{
		if (0 == strcmp(msg.pSender->GetName(), "button_ok"))
		{
			m_xAccount = m_pm.FindControl("account")->GetText();
			m_xPassword = m_pm.FindControl("password")->GetText();
			Close(IDOK);
		}
		else if (0 == strcmp(msg.pSender->GetName(), "button_register"))
		{
			// request for url
			const std::string& refURL = GetGlobalLuaConfig("gHTTPKVAddress");
			if (refURL.empty())
			{
				::MessageBox(NULL, "无法获取请求", "错误", MB_OK | MB_ICONERROR);
				return;
			}
			char szReqURL[MAX_PATH];
			sprintf(szReqURL, "%s/get?key=app_reg_url", refURL.c_str());
			// request for battle net address
			BMHttpManager::GetInstance()->DoGetRequestSync(szReqURL, std::bind(&BMPasswordWnd::OnRegisterURLHTTPGet, this,
				std::placeholders::_1, std::placeholders::_2));
		}
	}
}

void BMPasswordWnd::OnPrepare()
{
	
}

void BMPasswordWnd::OnRegisterURLHTTPGet(const char* _pData, size_t _uSize)
{
	if (NULL == _pData ||
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
	ShellExecute(NULL, "open", xRegURL.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
}
