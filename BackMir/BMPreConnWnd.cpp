#include "BMPreConnWnd.h"
#include <Windows.h>
#include "../Common/GlobalDefine.h"
#include "../GameScene/GameResourceManager.h"
#include "../Net/ClientSocket.h"
#include "../../CommonModule/CommandLineHelper.h"
#include "BackMir.h"
#include "../../CommonModule/GamePacket.h"
#include "../Common/GlobalFunction.h"
#include "../../CommonModule/loginsvr.pb.h"
#include "../../CommonModule/ProtoType.h"
#include "BMSelServerWnd.h"
#include "BMPasswordWnd.h"
#include "../Common/strutils.h"
#include <sstream>
#include <direct.h>

#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <chrono>
//////////////////////////////////////////////////////////////////////////
bool g_bPrepared = false;
bool g_bDataLoaded = false;
BMPreConnWnd* g_PreConnWnd = NULL;
GAME_MODE g_eGameMode = GM_NONE;
HeroHeader g_stHeroHeader[3];
//////////////////////////////////////////////////////////////////////////
BMPreConnWnd::BMPreConnWnd()
{
	g_PreConnWnd = this;
	m_xParserGameSvr.SetHandlePacket(&BMPreConnWnd::OnFullMsgGameSvr);
	m_xParserLoginSvr.SetHandlePacket(&BMPreConnWnd::OnFullMsgLoginSvr);
	m_xParserGlobal.SetHandlePacket(&BMPreConnWnd::OnFullMsgGlobal);
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
		// Set the custom res dir
		const char* szCustomResDir = xHelper.GetParam("resdir");
		if (NULL != szCustomResDir) {
			GameResourceManager::GetInstance()->SetCustomDir(szCustomResDir);
		}

#ifdef _DEBUG
		std::stringstream ssDebugArg;
		// Print args
		auto args = xHelper.GetAllParams();
		for (auto &argList : args) {
			ssDebugArg << "Run arg list: " << argList << "\n";
		}
		// Print env
		const char* pEnvVal = getenv("RES_DATA_DIR");
		if (nullptr != pEnvVal) {
			ssDebugArg << "RES_DATA_DIR=" << pEnvVal << "\n";
		}
		pEnvVal = getenv("gmcode");
		if (nullptr != pEnvVal) {
			ssDebugArg << "gmcode=" << pEnvVal << "\n";
		}
		// Print cwd
		char szCwd[MAX_PATH];
		szCwd[0] = 0;
		getcwd(szCwd, sizeof(szCwd));
		ssDebugArg << "cwd=" << szCwd;
		MessageBox(NULL, ssDebugArg.str().c_str(), "ARGS && ENVS", MB_OK);
#endif
	}

	m_nLID = 0;
}

BMPreConnWnd::~BMPreConnWnd()
{
	// reset all callback
	m_xParserGameSvr.SetHandlePacket(NULL);
	m_xParserLoginSvr.SetHandlePacket(NULL);
	m_xParserGlobal.SetHandlePacket(NULL);

	//	取消socket事件监听
	CHAR strError[256] = { 0 };
	if (g_eGameMode == GM_NORMAL)
	{
		//	仅有游戏服务器 则ClientSocket就是游戏服务器socket
		if (WSAAsyncSelect(g_xClientSocket.GetSocket(), NULL, WM_SOCKMSG, FD_CONNECT | FD_READ | FD_CLOSE) == SOCKET_ERROR)
		{
			wsprintf(strError, "WSAAsyncSelect() generated error %d\n", WSAGetLastError());
			OutputDebugString(strError);
		}
	}
	else if (g_eGameMode == GM_LOGIN)
	{
		//	登陆游戏服务器 则ClientSocket是登陆服务器 ClientSocket2是游戏服务器 2个都重定位
		if (WSAAsyncSelect(g_xClientSocket.GetSocket(), NULL, WM_SOCKMSG, FD_CONNECT | FD_READ | FD_CLOSE) == SOCKET_ERROR)
		{
			wsprintf(strError, "WSAAsyncSelect() generated error %d\n", WSAGetLastError());
			OutputDebugString(strError);
		}

		if (WSAAsyncSelect(g_xClientSocket2.GetSocket(), NULL, WM_SOCKMSG, FD_CONNECT | FD_READ | FD_CLOSE) == SOCKET_ERROR)
		{
			wsprintf(strError, "WSAAsyncSelect() generated error %d\n", WSAGetLastError());
			OutputDebugString(strError);
		}
	}

	g_PreConnWnd = NULL;
}

void BMPreConnWnd::OnTimer(WPARAM _wTimerID)
{
	if(_wTimerID == TIMER_CONNECTSVR)
	{
		KillTimer(m_hWnd, TIMER_CONNECTSVR);
		//	Ready to connect server
		CommandLineHelper xCmd;
		AppendString("\rConnect...");
		bool bFailed = true;

		//	connect server
		do
		{
			if(!xCmd.InitParam())
			{
				AppendString("FAILED");
				break;
			}

			const char* pszIPAddr = xCmd.GetParam("svrip");
			if(NULL == pszIPAddr)
			{
				AppendString("FAILED");
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
				AppendString("FAILED");
				bFailed = true;
				break;
			}

			if(!g_xClientSocket.ConnectToServer(m_hWnd, szIP, wPort))
			{
				AppendString("FAILED");
				bFailed = true;
				break;
			}

			SetTimer(m_hWnd, TIMER_CONNTIMEOUT, 15000, NULL);
			bFailed = false;
			//AppendString("OK");
		}while(0);

		if(bFailed)
		{
			Sleep(1000);
			PrepareCloseGame();
		}
		else
		{
			Sleep(500);
		}
	}
	else if(_wTimerID == TIMER_CONNTIMEOUT)
	{
		AppendString("FAILED");
		PrepareCloseGame();
		if(g_eGameMode == GM_NONE)
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
	else if(_wTimerID == TIMER_SEND_LOGIN_HEARTBEAT)
	{
		if(g_eGameMode == GM_LOGIN)
		{
			PkgLoginHeartBeatNot not;
			m_xSndBuf.Reset();
			m_xSndBuf << not;
			SendBuffer(&m_xSndBuf);
		}
	}
}

LRESULT BMPreConnWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_CREATE)
	{
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("BMPreConnWnd.xml"), (UINT)0, NULL, &m_pm);
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

void BMPreConnWnd::Notify(TNotifyUI& msg)
{
	if(msg.sType == "windowinit")
	{
		OnPrepare();
	}
}

void BMPreConnWnd::OnPrepare()
{
	AppendString("Init...");
	WSADATA wsaData;
	int nRet;
	if((nRet = WSAStartup(MAKEWORD(2,2),&wsaData)) != 0){
		AppendString("FAILED");
		PrepareCloseGame();
	}else{
		AppendString("OK");
	}
	
	SetTimer(m_hWnd, TIMER_CONNECTSVR, 1000, NULL);
}

void BMPreConnWnd::AppendString(const char* _pszStr)
{
	CRichEditUI* pEdit = (CRichEditUI*)m_pm.FindControl("richedit");
	if(pEdit)
	{
		pEdit->AppendText(_pszStr, false);
	}
}

void BMPreConnWnd::PrepareRunGame()
{
	AppendString("\rEnter...");
	SetTimer(m_hWnd, TIMER_RUNGAME, 1000, NULL);
}

void BMPreConnWnd::RunGame()
{
	if(g_eGameMode == GM_NONE)
	{
		return;
	}
	else if(g_eGameMode == GM_NORMAL)
	{
		//	普通游戏模式 将此socket消息取消
		WSAAsyncSelect(g_xClientSocket.GetSocket(), m_hWnd, 0, 0);
	}
	else if(g_eGameMode == GM_LOGIN)
	{
		//	登陆服务器 需取消登录服务器 游戏服务器的socket消息
		WSAAsyncSelect(g_xClientSocket.GetSocket(), m_hWnd, 0, 0);
		WSAAsyncSelect(g_xClientSocket2.GetSocket(), m_hWnd, 0, 0);
		KillTimer(m_hWnd, TIMER_SEND_LOGIN_HEARTBEAT);
	}
	
	g_bPrepared = true;
	g_bDataLoaded = true;
	Close();
}

void BMPreConnWnd::PrepareCloseGame()
{
	AppendString("\rClose game...");
	SetTimer(m_hWnd, TIMER_CLOSEGAME, 5000, NULL);
}

void BMPreConnWnd::PrepareLoadData()
{
	AppendString("\rLoad data...");
	SetTimer(m_hWnd, TIMER_LOADDATA, 1000, NULL);
}

void BMPreConnWnd::LoadData()
{
	for (int i = 0; i < RES_TOTAL; ++i)
	{
		GameResourceManager::GetInstance()->LoadFileWithFilename(g_szResFile[i]);
	}
}

void BMPreConnWnd::OnSocketMsg(SOCKET _s, LPARAM _l)
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
		if(g_eGameMode == GM_NONE)
		{
			//	游戏模式为空 第一次请求游戏模式
			KillTimer(m_hWnd, TIMER_CONNTIMEOUT);
			AppendString("OK");
			AppendString("\rPrepare...");
			//	第一次请求游戏模式超时
			SetTimer(m_hWnd, TIMER_FTYPETIMEOUT, 15000, NULL);
		}
		else if(g_eGameMode == GM_LOGIN)
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
		if(g_eGameMode == GM_NONE)
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
		else if(g_eGameMode == GM_NORMAL)
		{
			//	普通游戏模式 g_xClientSocket就是游戏服务器连接
			if(_s == g_xClientSocket.GetSocket())
			{
				OnSocketGameSvr(_s, _l);
			}
		}
		else if(g_eGameMode == GM_LOGIN)
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

void BMPreConnWnd::OnSocketLoginSvr(SOCKET _s, LPARAM _l)
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

void BMPreConnWnd::OnSocketGameSvr(SOCKET _s, LPARAM _l)
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

void __stdcall BMPreConnWnd::OnFullMsgGlobal(const void* _pData, unsigned int _nLen)
{
	if(g_PreConnWnd == NULL)
	{
		return;
	}

	const LoginHeader* pPacket = (const LoginHeader*)_pData;
	LoginHeader* pReadPacket = const_cast<LoginHeader*>(pPacket);
	pReadPacket->uLen = _nLen;

	g_PreConnWnd->m_xBuffer.Reset();
	unsigned int uOpCode = pPacket->uOp;

	if (uOpCode >= protocol::LSOp_MIN &&
		uOpCode <= protocol::LSOp_MAX)
	{
		SetProtoType(ProtoType_Protobuf);
	}
	if (ProtoType_Protobuf == GetProtoType())
	{
		g_PreConnWnd->DoLoginSvrPacketProtobuf((const char*)_pData, _nLen);
		return;
	}

	g_PreConnWnd->m_xBuffer.Write(_pData, _nLen);

	switch(uOpCode)
	{
		DO_GLOBAL_PACKET(PKG_LOGIN_GAMETYPE_NOT, PkgLoginGameTypeNot)
	}
}

void __stdcall BMPreConnWnd::OnFullMsgGameSvr(const void* _pData, unsigned int _nLen)
{
	if(g_PreConnWnd == NULL)
	{
		return;
	}

	const LoginHeader* pPacket = (const LoginHeader*)_pData;
	LoginHeader* pReadPacket = const_cast<LoginHeader*>(pPacket);
	pReadPacket->uLen = _nLen;

	unsigned int uOpCode = pPacket->uOp;

	g_PreConnWnd->m_xBuffer.Reset();
	g_PreConnWnd->m_xBuffer.Write(_pData, _nLen);

	switch(uOpCode)
	{
	DO_GAMESVR_PACKET(PKG_LOGIN_GAMETYPE_NOT, PkgLoginGameTypeNot)
	}
}

void __stdcall BMPreConnWnd::OnFullMsgLoginSvr(const void* _pData, unsigned int _nLen)
{
	if(g_PreConnWnd == NULL)
	{
		return;
	}

	const PacketHeader* pPacket = (const PacketHeader*)_pData;
	PacketHeader* pReadPacket = const_cast<PacketHeader*>(pPacket);
	pReadPacket->uLen = _nLen;

	unsigned int uOpCode = pPacket->uOp;

	//	is a protobuf message ?
	if (uOpCode >= protocol::LSOp_MIN &&
		uOpCode <= protocol::LSOp_MAX)
	{
		SetProtoType(ProtoType_Protobuf);
	}
	if (ProtoType_Protobuf == GetProtoType())
	{
		g_PreConnWnd->DoLoginSvrPacketProtobuf((const char*)_pData, _nLen);
		return;
	}

	g_PreConnWnd->m_xBuffer.Reset();
	g_PreConnWnd->m_xBuffer.Write(_pData, _nLen);

	if(uOpCode == PKG_LOGIN_HEADDATA_NOT)
	{
		g_PreConnWnd->DoLoginSvr_HeadData((const char*)_pData, _nLen);
	}
	else
	{
		switch(uOpCode)
		{
			DO_LOGINSVR_PACKET(PKG_LOGIN_GAMETYPE_NOT, PkgLoginGameTypeNot)
				DO_LOGINSVR_PACKET(PKG_LOGIN_VERIFYRESULT_ACK, PkgLoginVerifyResultAck)
				DO_LOGINSVR_PACKET(PKG_LOGIN_QUICKMSG_NOT, PkgLoginQuickMsgNot)
				DO_LOGINSVR_PACKET(PKG_LOGIN_SERVERADDR_NOT, PkgLoginServerAddrNot)
				DO_LOGINSVR_PACKET(PKG_LOGIN_SERVERADDRV2_RSP, PkgLoginServerAddrV2Not)
				//DO_LOGINSVR_PACKET(PKG_LOGIN_HEADDATA_NOT, PkgLoginHeadDataNot)
		}
	}
}

void BMPreConnWnd::DoLoginSvrPacketProtobuf(const char* _pData, size_t _uLength)
{
	const PacketHeader* pPacket = (const PacketHeader*)_pData;
	const char* pData = _pData + 8;
	size_t uLen = _uLength - 8;

	unsigned int uOpCode = pPacket->uOp;

	switch (uOpCode)
	{
	case protocol::LSAccessNtf:
		{
			protocol::MLSAccessNtf ntf;
			if (!ntf.ParseFromArray(pData, uLen))
			{
				return;
			}
			m_nLID = ntf.lid();
			m_xAccessToken = ntf.accesstoken();

			if(g_eGameMode == GM_NONE)
			{
				//	未设置游戏模式
				KillTimer(m_hWnd, TIMER_FTYPETIMEOUT);
				g_eGameMode = GM_LOGIN;
				AppendString("OK");

				// Request for enter account and password
				if (m_xPsw.empty() ||
					m_xAct.empty())
				{
					AskForSignIn();
				}

				if(m_xPsw.empty() ||
					m_xAct.empty())
				{
					MessageBox(NULL, "请输入账号密码", "错误", MB_OK | MB_ICONERROR);
					PrepareCloseGame();
					exit(-1);
					return;
				}

				//	发送账号密码请求
				AppendString("\rVerify...");
				SetTimer(m_hWnd, TIMER_VERIFYTIMEOUT, 15000, NULL);

				//	验证账户密码
				protocol::MVerifyAccountReq req;
				req.set_account(m_xAct.c_str());
				req.set_password(m_xPsw.c_str());
				SendProto(protocol::VerifyAccountReq, req);

				//	定时器
				SetTimer(m_hWnd, TIMER_SEND_LOGIN_HEARTBEAT, 15 * 1000, NULL);
			}
		}break;
	case protocol::QuickMessageNtf:
		{
			protocol::MQuickMessageNtf ntf;
			if (!ntf.ParseFromArray(pData, uLen))
			{
				return;
			}

			if(ntf.msgid() == 8)
			{
				MessageBox(NULL, "用户名或者密码错误", "错误", MB_OK | MB_ICONERROR);
				PrepareCloseGame();
			}
			else if(ntf.msgid() == 7)
			{
				MessageBox(NULL, "没有可用的游戏服务器", "错误", MB_OK | MB_ICONERROR);
				PrepareCloseGame();
			}
			else if(ntf.msgid() == 10)
			{
				char msg[100];
				sprintf(msg, "您在服务器[%d]中已登录了，无法重复登录", ntf.param());
				MessageBox(NULL, msg, "错误", MB_OK | MB_ICONERROR);
			}
		}break;
	case protocol::ServerListNtf:
		{
			protocol::MServerListNtf ntf;
			if (!ntf.ParseFromArray(pData, uLen))
			{
				return;
			}

			//	选择服务器
			KillTimer(m_hWnd, TIMER_VERIFYTIMEOUT);
			AppendString("OK\n");
			BMSelServerWnd wnd;
			wnd.SetServerList(ntf);
			wnd.Create(NULL, "SelServerWnd", UI_WNDSTYLE_FRAME, 0, 0, 0, 200, 350, 0);
			wnd.CenterWindow();
			int nSel = wnd.ShowModal();
			int nServerIndex = wnd.GetSelServerIndex();

			if (IDOK == nSel &&
				nServerIndex >= 0 &&
				nServerIndex < ntf.servers_size())
			{
				const protocol::MServerListItem& refServer = ntf.servers(nServerIndex);
				AppendString("\nConnecting to Game server ...");
				const char* pServerAddr = refServer.serveraddress().c_str();
				char* pAddr = new char[strlen(pServerAddr) + 1];
				strcpy(pAddr, pServerAddr);
				char* pszColon = strchr(pAddr, ':');
				if (NULL == pszColon)
				{
					delete []pAddr;
					pAddr = NULL;
					return;
				}

				*pszColon = '\0';
				int nPort = atoi(pszColon + 1);

				g_xClientSocket2.ConnectToServer(m_hWnd, pAddr, nPort);
				delete []pAddr;
				pAddr = NULL;

				SetTimer(m_hWnd, TIMER_CONNTIMEOUT, 15000, NULL);
				AppendString("\rConnect GameServer...");
			}
			else
			{
				MessageBox(NULL, "不可用的游戏服务器", "错误", MB_OK | MB_ICONERROR);
				PrepareCloseGame();
			}
		}break;
	case protocol::VerifyAccountRsp:
		{
			protocol::MVerifyAccountRsp rsp;
			if (!rsp.ParseFromArray(pData, uLen))
			{
				return;
			}
		}break;
	case protocol::PlayerHumBaseDataNtf:
		{
			protocol::MPlayerHumBaseDataNtf ntf;
			if (!ntf.ParseFromArray(pData, uLen))
			{
				return;
			}

			for (int index = 0; index < ntf.roles_size(); index++)
			{
				const protocol::MPlayerHumBaseData& refRoleData = ntf.roles(index);
				
				//DoLoginSvr_HeadData(refRoleData.roledata().c_str(), refRoleData.roledata().size());
				const char* pRoleData = refRoleData.roledata().c_str();
				char nRoleIndex = refRoleData.roleindex();

				if(nRoleIndex >= 0 &&
					nRoleIndex <= 2)
				{
					char namelen = *pRoleData;
					pRoleData += 1;

					char szName[20] = {0};
					memcpy(szName, pRoleData, namelen + 1);
					pRoleData += namelen + 1;

					char sex = *pRoleData;
					++pRoleData;

					char job = *pRoleData;
					++pRoleData;

					short level = *(short*)pRoleData;

					g_stHeroHeader[nRoleIndex].bJob = job;
					g_stHeroHeader[nRoleIndex].bSex = sex;
					strcpy(g_stHeroHeader[nRoleIndex].szName, szName);
					g_stHeroHeader[nRoleIndex].uLevel = level;
				}
			}
		}break;
	/*case protocol::PlayerHumBaseDataNtf:
		{
			protocol::MPlayerHumBaseDataNtf ntf;
			if (!ntf.ParseFromArray(pData, uLen))
			{
				return;
			}

			UpdateGlobalHeroHeader(ntf);
		}break;*/
	}
}

void BMPreConnWnd::DoGameSvrPacket(const PkgLoginGameTypeNot& not)
{
	//	游戏服务器 在当前模式为登陆模式下 会收到该包
	if(g_eGameMode == GM_LOGIN)
	{
		if(not.bType == PLGTN_GAMESERVER)
		{
			KillTimer(m_hWnd, TIMER_STYPETIMEOUT);
			g_bPrepared = true;
			AppendString("OK");
			PrepareLoadData();
			//Sleep(800);
			//RunGame();
			//PrepareRunGame();
			//	将索引发给游戏服务器
			if (GetProtoType() == ProtoType_ByteBuffer)
			{
				PkgLoginConnIdxNot not;
				not.dwGSIdx = 0;
				not.dwLSIdx = m_dwLSIdx;
				g_xBuffer.Reset();
				g_xBuffer << not;
				//SendBuffer2(&g_xBuffer);
				SendBuffer2(&g_xBuffer, true);
			}
			else
			{
				//	使用protobuf 有accessToken
				protocol::MRegisterClientReq req;
				req.set_sid(not.dwConnIdx);
				req.set_lid(m_nLID);
				req.set_accesstoken(m_xAccessToken.c_str());
				SendProto2(protocol::RegisterClientReq, req);
			}
		}
	}
}

void BMPreConnWnd::DoLoginSvr_HeadData(const char* _pData, unsigned int _len)
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

void BMPreConnWnd::DoLoginSvrPacket(const PkgLoginServerAddrNot& not)
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

void BMPreConnWnd::DoLoginSvrPacket(const PkgLoginServerAddrV2Not& not) {
	if (not.vecServers.empty()) {
		MessageBox(NULL, "无可用的游戏服务器", "错误", MB_OK | MB_ICONERROR);
		return;
	}

	std::vector<std::string> vecResult;
	BMSelServerWnd wnd;

	for (int i = 0; i < not.vecServers.size(); i++) {
		vecResult.clear();
		const std::string& refServerInfo = not.vecServers[i];

		SplitString(refServerInfo, vecResult, "_");
		if (vecResult.size() != 3) {
			continue;
		}

		// serverid_serveraddr_servername
		int nServerID = atoi(vecResult[0].c_str());
		if (0 == nServerID) {
			continue;
		}
		const std::string &refAddr = vecResult[1];
		if (refAddr.empty()) {
			continue;
		}
		const std::string &refServerName = vecResult[2];
		if (refServerName.empty()) {
			continue;
		}

		wnd.AddServer(nServerID, refAddr, refServerName);
	}

	//	选择服务器
	KillTimer(m_hWnd, TIMER_VERIFYTIMEOUT);
	AppendString("OK\n");
	wnd.Create(NULL, "SelServerWnd", UI_WNDSTYLE_FRAME, 0, 0, 0, 200, 350, 0);
	wnd.CenterWindow();
	int nSel = wnd.ShowModal();
	int nServerIndex = wnd.GetSelServerIndex();

	if (IDOK == nSel &&
		nServerIndex >= 0 &&
		nServerIndex < not.vecServers.size())
	{
		const protocol::MServerListItem& refServer = wnd.GetServerList().servers(nServerIndex);

		// Send gs serverid to ls
		PkgLoginClientSelGSRsp rsp;
		rsp.uGSServerID = (short)refServer.serverid();
		g_xBuffer.Reset();
		g_xBuffer << rsp;
		SendBuffer(&g_xBuffer);

		// Show select server wnd
		AppendString("\nConnecting to Game server ...");
		const char* pServerAddr = refServer.serveraddress().c_str();
		char* pAddr = new char[strlen(pServerAddr) + 1];
		strcpy(pAddr, pServerAddr);
		char* pszColon = strchr(pAddr, ':');
		if (NULL == pszColon)
		{
			delete []pAddr;
			pAddr = NULL;
			return;
		}

		*pszColon = '\0';
		int nPort = atoi(pszColon + 1);

		g_xClientSocket2.ConnectToServer(m_hWnd, pAddr, nPort);
		delete []pAddr;
		pAddr = NULL;

		SetTimer(m_hWnd, TIMER_CONNTIMEOUT, 15000, NULL);
		AppendString("\rConnect GameServer...");
	}
	else
	{
		MessageBox(NULL, "不可用的游戏服务器", "错误", MB_OK | MB_ICONERROR);
		PrepareCloseGame();
	}
}

void BMPreConnWnd::DoLoginSvrPacket(const PkgLoginVerifyResultAck& ack)
{

}

void BMPreConnWnd::DoLoginSvrPacket(const PkgLoginQuickMsgNot& not)
{
	if(not.uMsg == 8)
	{
		MessageBox(NULL, "用户名或者密码错误", "错误", MB_OK | MB_ICONERROR);
		PrepareCloseGame();
	}
	else if(not.uMsg == 7)
	{
		MessageBox(NULL, "没有可用的游戏服务器", "错误", MB_OK | MB_ICONERROR);
		PrepareCloseGame();
	}
}

void BMPreConnWnd::DoLoginSvrPacket(const PkgLoginGameTypeNot& not)
{
	//	登陆服务器不会再接收到类型包
	/*if(g_eGameMode == GM_NONE)
	{
		//	未设置游戏模式
		if(not.bType == PLGTN_GAMESERVER)
		{
			//	直接连游戏服务器 普通模式 成功登陆
			g_eGameMode = GM_NORMAL;
			g_bPrepared = true;
			AppendString("OK");
			PrepareLoadData();
			//Sleep(800);
			//Close();
			//PrepareRunGame();
		}
		else if(not.bType == PLGTN_LOGINSERVER)
		{
			g_eGameMode = GM_LOGIN;
		}
		else
		{
			//	错误的服务器类型
			AppendString("FAILED");
			PrepareCloseGame();
		}
	}
	else if(g_eGameMode == GM_LOGIN)
	{
		if(not.bType == PLGTN_GAMESERVER)
		{
			g_bPrepared = true;
			AppendString("OK");
			PrepareLoadData();
			//Sleep(800);
			//RunGame();
			//PrepareRunGame();
		}
	}*/
}

void BMPreConnWnd::DoPacket(const PkgLoginGameTypeNot& not)
{
	if(g_eGameMode == GM_NONE)
	{
		//	未设置游戏模式
		if(not.bType == PLGTN_GAMESERVER)
		{
			//	直接连游戏服务器 普通模式 成功登陆
			g_eGameMode = GM_NORMAL;
			g_bPrepared = true;
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
			g_eGameMode = GM_LOGIN;
			AppendString("OK");

			//	定时器
			SetTimer(m_hWnd, TIMER_SEND_LOGIN_HEARTBEAT, 15 * 1000, NULL);

			// Request for enter account and password
			if (m_xPsw.empty() ||
				m_xAct.empty())
			{
				AskForSignIn();
			}

			if(m_xPsw.empty() ||
				m_xAct.empty())
			{
				MessageBox(NULL, "请输入账号密码", "错误", MB_OK | MB_ICONERROR);
				PrepareCloseGame();
				exit(-1);
				return;
			}

			//	发送账号密码请求
			AppendString("\rVerify...");
			SetTimer(m_hWnd, TIMER_VERIFYTIMEOUT, 15000, NULL);

			//	记录下LS的索引
			m_dwLSIdx = not.dwConnIdx;

			m_xSndBuf.Reset();
			m_xSndBuf << (int)0;
			// Verify v1
			//m_xSndBuf << (int)PKG_LOGIN_CLIENTVERIFY_REQ;
			// Verify v2
			m_xSndBuf << (int)PKG_LOGIN_CLIENTVERIFYV2_REQ;
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
	else if(g_eGameMode == GM_LOGIN)
	{
		if(not.bType == PLGTN_GAMESERVER)
		{
			g_bPrepared = true;
			AppendString("OK");
			PrepareLoadData();
			//Sleep(800);
			//RunGame();
			//PrepareRunGame();
		}
	}
}

void UpdateGlobalHeroHeader(const protocol::MPlayerHumBaseDataNtf& _refHum)
{
	int nRoleSize = _refHum.roles_size();

	for (int i = 0; i < nRoleSize; ++i)
	{
		const protocol::MPlayerHumBaseData& refBaseData = _refHum.roles(i);

		const char* pData = refBaseData.roledata().c_str();
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
}

void BMPreConnWnd::AskForSignIn()
{
	// Request for enter account and password
	BMPasswordWnd wnd;
	wnd.Create(NULL, "PasswordWnd", UI_WNDSTYLE_FRAME, 0, 0, 0, 200, 350, 0);
	wnd.CenterWindow();
	SetForegroundWindow(wnd.GetHWND());
	SetFocus(wnd.GetHWND());
	::ShowWindow(m_hWnd, SW_SHOWMINIMIZED);
	int nSel = wnd.ShowModal();
	if (IDOK == nSel)
	{
		m_xAct = wnd.GetAccount();
		m_xPsw = wnd.GetPassword();
	}
	::ShowWindow(m_hWnd, SW_NORMAL);
}