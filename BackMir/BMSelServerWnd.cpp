#include "BMSelServerWnd.h"
//////////////////////////////////////////////////////////////////////////
BMSelServerWnd::BMSelServerWnd()
{
	m_nSelServerIndex = -1;
}

BMSelServerWnd::~BMSelServerWnd()
{

}

void BMSelServerWnd::SetServerList(protocol::MServerListNtf& _refList)
{
	m_serverList = _refList;
}

int BMSelServerWnd::GetSelServerIndex()
{
	return m_nSelServerIndex;
}

LRESULT BMSelServerWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_CREATE)
	{
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("BMSelServerWnd.xml"), (UINT)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);
		//Init();
		return 0;
	}
	else if(uMsg == WM_DESTROY)
	{
		//PostQuitMessage(0);
	}
	else if(uMsg == WM_ERASEBKGND)
	{
		return 1;
	}
	else if( uMsg == WM_NCACTIVATE ) {

		if( !::IsIconic(m_hWnd) ) {

			return (wParam == 0) ? TRUE :FALSE;

		}

	}
	else if( uMsg == WM_NCCALCSIZE ) {

		return 0;

	}
	else if( uMsg == WM_NCPAINT ) {

		return 0;

	}
	else if(uMsg == WM_NCHITTEST)
	{
		return HTCLIENT;
	}

	LRESULT lRes = 0;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void BMSelServerWnd::Notify(TNotifyUI& msg)
{
	if(msg.sType == "windowinit")
	{
		OnPrepare();
	}
	else if (msg.sType == "itemclick")
	{
		m_nSelServerIndex = atoi(msg.pSender->GetUserData());
		Close(IDOK);
	}
}

void BMSelServerWnd::OnPrepare()
{
	if (m_serverList.servers_size() == 0)
	{
		return;
	}

	CListUI* pList = (CListUI*)m_pm.FindControl("serverList");
	if (NULL == pList)
	{
		return;
	}

	for (int i = 0; i < m_serverList.servers_size(); ++i)
	{
		const protocol::MServerListItem& refItem = m_serverList.servers(i);

		CListLabelElementUI* pLabel = new CListLabelElementUI;
		const char* pText = refItem.servername().c_str();
		pLabel->SetText(pText);
		pLabel->SetAttribute("align", "hcenter");
		char buf[10] = {0};
		pLabel->SetUserData(itoa(i, buf, 10));
		pList->Add(pLabel);
	}
}