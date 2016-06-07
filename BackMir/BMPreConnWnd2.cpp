#include "BMPreConnWnd2.h"
#include <Windows.h>
#include "../Common/GlobalDefine.h"
#include "../GameScene/GameResourceManager.h"
#include "../Net/ClientSocket.h"
#include "../../CommonModule/CommandLineHelper.h"
#include "BackMir.h"
#include "../../CommonModule/GamePacket.h"
#include "../Common/GlobalFunction.h"
//////////////////////////////////////////////////////////////////////////
BMPreConnWnd2* g_PreConnWnd2 = NULL;
//////////////////////////////////////////////////////////////////////////
BMPreConnWnd2::BMPreConnWnd2()
{
	SetConnStep(kPreConnStep_None);
	m_eGameMode = GM_NONE;
	m_bPrepared = false;
	g_PreConnWnd2 = this;
	m_xParserGameSvr.SetHandlePacket(&BMPreConnWnd2::OnFullMsgGameSvr);
	m_xParserLoginSvr.SetHandlePacket(&BMPreConnWnd2::OnFullMsgLoginSvr);
	m_xParserGlobal.SetHandlePacket(&BMPreConnWnd2::OnFullMsgGlobal);
	ZeroMemory(g_stHeroHeader, sizeof(g_stHeroHeader));
	m_dwLSIdx = 0;

	CommandLineHelper xHelper;
	if(xHelper.InitParam())
	{
		const char* pszValue = xHelper.GetParam("account");
		if(pszValue != NULL)
		{
			m_xAct = pszValue;
		}
		pszValue = xHelper.GetParam("password");
		if(pszValue != NULL)
		{
			m_xPsw = pszValue;
		}
	}
}

BMPreConnWnd2::~BMPreConnWnd2()
{
	g_PreConnWnd2 = NULL;
}

void BMPreConnWnd2::SetConnStep(PreConnStepType _eType)
{
	m_eConnStep = _eType;
	m_dwLastSetStepTime = GetTickCount();

	if(_eType == kPreConnStep_Init)
	{
		AppendString("Init...");
	}
	else if(_eType == kPreConnStep_InitResultFailed)
	{
		AppendString("\rInit failed.");
	}
	else if(_eType == kPreConnStep_InitResultOk)
	{
		AppendString("\rInit ok...");
	}
	else if(_eType == kPreConnStep_WaitClose)
	{
		AppendString("\rClose...");
	}
	else if(_eType == kPreConnStep_ConnectServer)
	{
		AppendString("\rConnecting...");
	}
	else if(_eType == kPreConnStep_ConnectServerResultOk)
	{
		AppendString("\rOk...");
	}
	else if(_eType == kPreConnStep_ConnectServerResultFailed)
	{
		AppendString("\rFailed...");
	}
	else if(_eType == kPreConnStep_ConnectServerResultTimeout)
	{
		AppendString("\Timeout...");
	}
}

void BMPreConnWnd2::OnTimer(WPARAM _wTimerID)
{
	DWORD dwTick = GetTickCount();

	if(_wTimerID == TIMER_PRECONN_UPDATE)
	{
		if(m_eConnStep == kPreConnStep_InitResultFailed)
		{
			if(dwTick - m_dwLastSetStepTime > 2000)
			{
				SetConnStep(kPreConnStep_WaitClose);
			}
		}
		else if(m_eConnStep == kPreConnStep_InitResultOk)
		{
			if(dwTick - m_dwLastSetStepTime > 2000)
			{
				SetConnStep(kPreConnStep_ConnectServer);
				OnConnectServer();
			}
		}
		else if(m_eConnStep == kPreConnStep_ConnectServer)
		{
			if(dwTick - m_dwLastSetStepTime > 5000)
			{
				SetConnStep(kPreConnStep_ConnectServerResultTimeout);
			}
		}
		else if(m_eConnStep == kPreConnStep_ConnectServerResultTimeout)
		{
			SetConnStep(kPreConnStep_WaitClose);
		}
		else if(m_eConnStep == kPreConnStep_ConnectServerResultFailed)
		{
			SetConnStep(kPreConnStep_WaitClose);
		}
		else if(m_eConnStep == kPreConnStep_ConnectServerResultOk)
		{

		}
	}


	if(_wTimerID == TIMER_CONNECTSVR)
	{
		KillTimer(m_hWnd, TIMER_CONNECTSVR);
		//	Ready to connect server
		
		AppendString("\rConnect...");
		

		//	connect server
		
	}
	else if(_wTimerID == TIMER_CONNTIMEOUT)
	{
		AppendString("FAILED");
		PrepareCloseGame();
		if(m_eGameMode == GM_NONE)
		{
			g_xClientSocket.DisconnectToServer();
			KillTimer(m_hWnd, TIMER_CONNTIMEOUT);
		}
	}
	else if(_wTimerID == TIMER_CLOSEGAME)
	{
		KillTimer(m_hWnd, TIMER_CLOSEGAME);
		Close();
	}
	else if(_wTimerID == TIMER_LOADDATA)
	{
		KillTimer(m_hWnd, TIMER_LOADDATA);
		LoadData();
		AppendString("OK");
		//	数据读完后 才进入游戏
		PrepareRunGame();
	}
	else if(_wTimerID == TIMER_RUNGAME)
	{
		KillTimer(m_hWnd, TIMER_RUNGAME);
		RunGame();
		AppendString("OK");
	}
	else if(_wTimerID == TIMER_FTYPETIMEOUT)
	{
		KillTimer(m_hWnd, TIMER_FTYPETIMEOUT);
		AppendString("FAILED");
		PrepareCloseGame();
		g_xClientSocket.DisconnectToServer();
	}
	else if(_wTimerID == TIMER_VERIFYTIMEOUT)
	{
		KillTimer(m_hWnd, TIMER_VERIFYTIMEOUT);
		AppendString("FAILED");
		PrepareCloseGame();
		g_xClientSocket.DisconnectToServer();
	}
}

LRESULT BMPreConnWnd2::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_CREATE)
	{
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("BMPreConnWnd2.xml"), (UINT)0, NULL, &m_pm);
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
	else if(uMsg == WM_TIMER)
	{
		OnTimer(wParam);
	}
	else if(uMsg == WM_SOCKMSG)
	{
		OnSocketMsg(wParam, lParam);
	}

	LRESULT lRes = 0;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void BMPreConnWnd2::Notify(TNotifyUI& msg)
{
	if(msg.sType == "windowinit")
	{
		OnPrepare();
	}
}

void BMPreConnWnd2::OnPrepare()
{
	AppendString("Init...");
	WSADATA wsaData;
	int nRet;
	if((nRet = WSAStartup(MAKEWORD(2,2),&wsaData)) != 0){
		SetConnStep(kPreConnStep_InitResultFailed);
	}else{
		SetConnStep(kPreConnStep_InitResultOk);
	}
}

void BMPreConnWnd2::OnConnectServer()
{
	CommandLineHelper xCmd;
	bool bFailed = true;

	do
	{
		if(!xCmd.InitParam())
		{
			break;
		}

		const char* pszIPAddr = xCmd.GetParam("svrip");
		if(NULL == pszIPAddr)
		{
			break;
		}

		char szIP[16];
		char szPort[10] = {0};
		WORD wPort = 0;
		szIP[15] = 0;
		int nPortPos = 0;
		for(int i = 0; i < 16; ++i)
		{
			if(pszIPAddr[i] == ':')
			{
				nPortPos = i;
				szIP[i] = 0;
				break;
			}
			else
			{
				szIP[i] = pszIPAddr[i];
			}
		}
		++nPortPos;

		for(int i = nPortPos; ; ++i)
		{
			if(pszIPAddr[i] == 0)
			{
				szPort[i - nPortPos] = 0;
				break;
			}
			szPort[i - nPortPos] = pszIPAddr[i];
		}
		wPort = (WORD)atoi(szPort);

		if(szIP[0] == 0 ||
			wPort == 0)
		{
			bFailed = true;
			break;
		}

		if(!g_xClientSocket.ConnectToServer(m_hWnd, szIP, wPort))
		{
			bFailed = true;
			break;
		}

		bFailed = false;
	}while(0);

	if(bFailed)
	{
		SetConnStep(kPreConnStep_ConnectServerResultFailed);
	}
}

void BMPreConnWnd2::AppendString(const char* _pszStr)
{
	CRichEditUI* pEdit = (CRichEditUI*)m_pm.FindControl("richedit");
	if(pEdit)
	{
		pEdit->AppendText(_pszStr, false);
	}
}

void BMPreConnWnd2::PrepareRunGame()
{
	AppendString("\rEnter...");
	SetTimer(m_hWnd, TIMER_RUNGAME, 1000, NULL);
}

void BMPreConnWnd2::RunGame()
{
	if(m_eGameMode == GM_NONE)
	{
		return;
	}
	else if(m_eGameMode == GM_NORMAL)
	{
		//	普通游戏模式 将此socket消息取消
		WSAAsyncSelect(g_xClientSocket.GetSocket(), m_hWnd, 0, 0);
	}
	else if(m_eGameMode == GM_LOGIN)
	{
		//	登陆服务器 需取消登录服务器 游戏服务器的socket消息
		WSAAsyncSelect(g_xClientSocket.GetSocket(), m_hWnd, 0, 0);
		WSAAsyncSelect(g_xClientSocket2.GetSocket(), m_hWnd, 0, 0);
	}
	
	m_bPrepared = true;
	Close();
}

void BMPreConnWnd2::PrepareCloseGame()
{
	AppendString("\rClose game...");
	SetTimer(m_hWnd, TIMER_CLOSEGAME, 5000, NULL);
}

void BMPreConnWnd2::PrepareLoadData()
{
	AppendString("\rLoad data...");
	SetTimer(m_hWnd, TIMER_LOADDATA, 1000, NULL);
}

void BMPreConnWnd2::LoadData()
{
	//char szSrcBuf[MAX_PATH];
	char szFile[MAX_PATH];

	for(int i = 0; i < RES_TOTAL; ++i)
	{
		sprintf(szFile, "%s\\Data\\%s", GetRootPath(), g_szResFile[i]);
		if(!GameResourceManager::GetInstance()->LoadFile(szFile))
		{
			//_hge->System_Log("加载资源失败 : %s", szFile);
			//return false;
		}
		//::PostMessage(hWnd, WM_SCENE_PROG, RES_TOTAL - 1, i);
	}

}

void BMPreConnWnd2::OnSocketMsg(SOCKET _s, LPARAM _l)
{	
	//	解析数据包
	static char s_szBuf[1024 * 5];

	if(WSAGETSELECTERROR(_l))
	{
		closesocket(_s);
		return;
	}

	if(WSAGETSELECTEVENT(_l) == FD_CONNECT)
	{
		if(m_eGameMode == GM_NONE)
		{
			//	游戏模式为空 第一次请求游戏模式
			KillTimer(m_hWnd, TIMER_CONNTIMEOUT);
			AppendString("OK");
			AppendString("\rPrepare...");
			//	第一次请求游戏模式超时
			SetTimer(m_hWnd, TIMER_FTYPETIMEOUT, 15000, NULL);
		}
		else if(m_eGameMode == GM_LOGIN)
		{
			//	登陆游戏模式 需要得到游戏服务器的连接
			KillTimer(m_hWnd, TIMER_CONNTIMEOUT);
			AppendString("OK");
			AppendString("\rPrepare...");
			//	第二次请求游戏模式超时
			SetTimer(m_hWnd, TIMER_STYPETIMEOUT, 15000, NULL);
		}
	}
	else if(WSAGETSELECTEVENT(_l) == FD_CLOSE)
	{
		g_xClientSocket.DisconnectToServer();
		g_xClientSocket2.DisconnectToServer();
		AppendString("\rLost connection...");
		PrepareCloseGame();
	}
	else
	{
		if(m_eGameMode == GM_NONE)
		{
			//	只接受游戏类型设置 只要设置了类型 就可以区分连接的服务器类型
			if(WSAGETSELECTEVENT(_l) == FD_READ)
			{
				int nRead = recv(_s, s_szBuf, 1024 * 5, 0);
				if(nRead == SOCKET_ERROR)
				{
					closesocket(_s);
					AppendString("\nLost connection...");
					PrepareCloseGame();
				}
				else
				{
					m_xParserGlobal.OnSocketData(s_szBuf, nRead);
				}
			}
		}
		else if(m_eGameMode == GM_NORMAL)
		{
			//	普通游戏模式 g_xClientSocket就是游戏服务器连接
			if(_s == g_xClientSocket.GetSocket())
			{
				OnSocketGameSvr(_s, _l);
			}
		}
		else if(m_eGameMode == GM_LOGIN)
		{
			//	登陆服务器模式 g_xClientSocket是登陆服务器 g_xClientSocket2是游戏服务器
			if(_s == g_xClientSocket.GetSocket())
			{
				OnSocketLoginSvr(_s, _l);
			}
			else if(_s == g_xClientSocket2.GetSocket())
			{
				OnSocketGameSvr(_s, _l);
			}
		}
	}
	
	return;
}

void BMPreConnWnd2::OnSocketLoginSvr(SOCKET _s, LPARAM _l)
{
	static char s_szBuf[1024 * 5];

	switch(WSAGETSELECTEVENT(_l))
	{
	case FD_READ:
		{
			int nRead = recv(_s, s_szBuf, 1024 * 5, 0);
			if(nRead == SOCKET_ERROR)
			{
				closesocket(_s);
				AppendString("\nLost connection...");
				PrepareCloseGame();
			}
			else
			{
				m_xParserLoginSvr.OnSocketData(s_szBuf, nRead);
			}
		}break;
	}
}

void BMPreConnWnd2::OnSocketGameSvr(SOCKET _s, LPARAM _l)
{
	static char s_szBuf[1024 * 5];

	switch(WSAGETSELECTEVENT(_l))
	{
	case FD_CONNECT:
		{
			KillTimer(m_hWnd, TIMER_CONNTIMEOUT);
			AppendString("OK");
			AppendString("\rPrepare...");
			SetTimer(m_hWnd, TIMER_FTYPETIMEOUT, 15000, NULL);
		}break;
	case FD_READ:
		{
			int nRead = recv(_s, s_szBuf, 1024 * 5, 0);
			if(nRead == SOCKET_ERROR)
			{
				closesocket(_s);
				AppendString("\nLost connection...");
				PrepareCloseGame();
			}
			else
			{
				m_xParserGameSvr.OnSocketData(s_szBuf, nRead);
			}
		}break;
	case FD_CLOSE:
		{
			closesocket(_s);
		}break;
	}
}

void __stdcall BMPreConnWnd2::OnFullMsgGlobal(const void* _pData, unsigned int _nLen)
{
	if(g_PreConnWnd2 == NULL)
	{
		return;
	}

	const LoginHeader* pPacket = (const LoginHeader*)_pData;
	LoginHeader* pReadPacket = const_cast<LoginHeader*>(pPacket);
	pReadPacket->uLen = _nLen;

	g_PreConnWnd2->m_xBuffer.Reset();
	unsigned int uOpCode = pPacket->uOp;

	g_PreConnWnd2->m_xBuffer.Write(_pData, _nLen);

	switch(uOpCode)
	{
		DO_GLOBAL_PACKET2(PKG_LOGIN_GAMETYPE_NOT, PkgLoginGameTypeNot)
	}
}

void __stdcall BMPreConnWnd2::OnFullMsgGameSvr(const void* _pData, unsigned int _nLen)
{
	if(g_PreConnWnd2 == NULL)
	{
		return;
	}

	const LoginHeader* pPacket = (const LoginHeader*)_pData;
	LoginHeader* pReadPacket = const_cast<LoginHeader*>(pPacket);
	pReadPacket->uLen = _nLen;

	unsigned int uOpCode = pPacket->uOp;

	g_PreConnWnd2->m_xBuffer.Reset();
	g_PreConnWnd2->m_xBuffer.Write(_pData, _nLen);

	switch(uOpCode)
	{
	DO_GAMESVR_PACKET2(PKG_LOGIN_GAMETYPE_NOT, PkgLoginGameTypeNot)
	}
}

void __stdcall BMPreConnWnd2::OnFullMsgLoginSvr(const void* _pData, unsigned int _nLen)
{
	if(g_PreConnWnd2 == NULL)
	{
		return;
	}

	const PacketHeader* pPacket = (const PacketHeader*)_pData;
	PacketHeader* pReadPacket = const_cast<PacketHeader*>(pPacket);
	pReadPacket->uLen = _nLen;

	unsigned int uOpCode = pPacket->uOp;

	g_PreConnWnd2->m_xBuffer.Reset();
	g_PreConnWnd2->m_xBuffer.Write(_pData, _nLen);

	if(uOpCode == PKG_LOGIN_HEADDATA_NOT)
	{
		g_PreConnWnd2->DoLoginSvr_HeadData((const char*)_pData, _nLen);
	}
	else
	{
		switch(uOpCode)
		{
			DO_LOGINSVR_PACKET2(PKG_LOGIN_GAMETYPE_NOT, PkgLoginGameTypeNot)
				DO_LOGINSVR_PACKET2(PKG_LOGIN_VERIFYRESULT_ACK, PkgLoginVerifyResultAck)
				DO_LOGINSVR_PACKET2(PKG_LOGIN_QUICKMSG_NOT, PkgLoginQuickMsgNot)
				DO_LOGINSVR_PACKET2(PKG_LOGIN_SERVERADDR_NOT, PkgLoginServerAddrNot)
				//DO_LOGINSVR_PACKET(PKG_LOGIN_HEADDATA_NOT, PkgLoginHeadDataNot)
		}
	}
}

void BMPreConnWnd2::DoGameSvrPacket(const PkgLoginGameTypeNot& not)
{
	//	游戏服务器 在当前模式为登陆模式下 会收到该包
	if(m_eGameMode == GM_LOGIN)
	{
		if(not.bType == PLGTN_GAMESERVER)
		{
			KillTimer(m_hWnd, TIMER_STYPETIMEOUT);
			m_bPrepared = true;
			AppendString("OK");
			PrepareLoadData();
			//Sleep(800);
			//RunGame();
			//PrepareRunGame();
			//	将索引发给游戏服务器
			PkgLoginConnIdxNot not;
			not.dwGSIdx = 0;
			not.dwLSIdx = m_dwLSIdx;
			g_xBuffer.Reset();
			g_xBuffer << not;
			SendBuffer2(&g_xBuffer);
		}
	}
}

void BMPreConnWnd2::DoLoginSvr_HeadData(const char* _pData, unsigned int _len)
{
	const char* pData = _pData;
	pData += 8;

	char index = *pData;
	pData ++;

	if(index >= 0 &&
		index <= 2)
	{
		char namelen = *pData;
		pData += 1;

		char szName[20] = {0};
		memcpy(szName, pData, namelen + 1);
		pData += namelen + 1;

		char sex = *pData;
		++pData;

		char job = *pData;
		++pData;

		short level = *(short*)pData;

		g_stHeroHeader[index].bJob = job;
		g_stHeroHeader[index].bSex = sex;
		strcpy(g_stHeroHeader[index].szName, szName);
		g_stHeroHeader[index].uLevel = level;
	}
}

void BMPreConnWnd2::DoLoginSvrPacket(const PkgLoginServerAddrNot& not)
{
	//	游戏服务器地址 连接
	if(!not.xAddr.empty())
	{
		AppendString("OK");
		KillTimer(m_hWnd, TIMER_VERIFYTIMEOUT);
		size_t uPos = not.xAddr.find(':');
		if(uPos == std::string::npos)
		{
			return;
		}

		std::string xLsAddr;
		int nPort = 0;
		xLsAddr = not.xAddr.substr(0, uPos);
		nPort = atoi(not.xAddr.substr(uPos+1).c_str());

		char szIP[50];
		strcpy(szIP, xLsAddr.c_str());

		//	第二个连接
		g_xClientSocket2.ConnectToServer(m_hWnd, szIP, nPort);
		SetTimer(m_hWnd, TIMER_CONNTIMEOUT, 15000, NULL);
		AppendString("\rConnect GameServer...");
	}
}

void BMPreConnWnd2::DoLoginSvrPacket(const PkgLoginVerifyResultAck& ack)
{

}

void BMPreConnWnd2::DoLoginSvrPacket(const PkgLoginQuickMsgNot& not)
{
	if(not.uMsg == 8)
	{
		MessageBox(NULL, "用户名或者密码错误", "错误", MB_OK | MB_ICONERROR);
	}
	else if(not.uMsg == 7)
	{
		MessageBox(NULL, "没有可用的游戏服务器", "错误", MB_OK | MB_ICONERROR);
	}
}

void BMPreConnWnd2::DoLoginSvrPacket(const PkgLoginGameTypeNot& not)
{
	//	登陆服务器不会再接收到类型包
	/*if(m_eGameMode == GM_NONE)
	{
		//	未设置游戏模式
		if(not.bType == PLGTN_GAMESERVER)
		{
			//	直接连游戏服务器 普通模式 成功登陆
			m_eGameMode = GM_NORMAL;
			m_bPrepared = true;
			AppendString("OK");
			PrepareLoadData();
			//Sleep(800);
			//Close();
			//PrepareRunGame();
		}
		else if(not.bType == PLGTN_LOGINSERVER)
		{
			m_eGameMode = GM_LOGIN;
		}
		else
		{
			//	错误的服务器类型
			AppendString("FAILED");
			PrepareCloseGame();
		}
	}
	else if(m_eGameMode == GM_LOGIN)
	{
		if(not.bType == PLGTN_GAMESERVER)
		{
			m_bPrepared = true;
			AppendString("OK");
			PrepareLoadData();
			//Sleep(800);
			//RunGame();
			//PrepareRunGame();
		}
	}*/
}

void BMPreConnWnd2::DoPacket(const PkgLoginGameTypeNot& not)
{
	if(m_eGameMode == GM_NONE)
	{
		//	未设置游戏模式
		if(not.bType == PLGTN_GAMESERVER)
		{
			//	直接连游戏服务器 普通模式 成功登陆
			m_eGameMode = GM_NORMAL;
			m_bPrepared = true;
			AppendString("OK");
			PrepareLoadData();
			KillTimer(m_hWnd, TIMER_FTYPETIMEOUT);
			//Sleep(800);
			//Close();
			//PrepareRunGame();
		}
		else if(not.bType == PLGTN_LOGINSERVER)
		{
			KillTimer(m_hWnd, TIMER_FTYPETIMEOUT);
			m_eGameMode = GM_LOGIN;
			AppendString("OK");
			//	发送账号密码请求
			AppendString("\rVerify...");
			SetTimer(m_hWnd, TIMER_VERIFYTIMEOUT, 15000, NULL);

			if(m_xPsw.empty() ||
				m_xAct.empty())
			{
				MessageBox(NULL, "请输入账号密码", "错误", MB_OK | MB_ICONERROR);
				PrepareCloseGame();
				return;
			}

			//	记录下LS的索引
			m_dwLSIdx = not.dwConnIdx;

			m_xSndBuf.Reset();
			m_xSndBuf << (int)0;
			m_xSndBuf << (int)PKG_LOGIN_CLIENTVERIFY_REQ;
			//std::string xAccount = "test1";
			//std::string xPsw = "test1";
			m_xSndBuf << (char)m_xAct.length();
			m_xSndBuf.Write(m_xAct.c_str(), m_xAct.length());
			m_xSndBuf << (char)m_xPsw.length();
			m_xSndBuf.Write(m_xPsw.c_str(), m_xPsw.length());
			//	登陆游戏模式 socket1就是登录服务器连接
			SendBuffer(&m_xSndBuf);
		}
		else
		{
			//	错误的服务器类型
			AppendString("FAILED");
			PrepareCloseGame();
		}
	}
	else if(m_eGameMode == GM_LOGIN)
	{
		if(not.bType == PLGTN_GAMESERVER)
		{
			m_bPrepared = true;
			AppendString("OK");
			PrepareLoadData();
			//Sleep(800);
			//RunGame();
			//PrepareRunGame();
		}
	}
}