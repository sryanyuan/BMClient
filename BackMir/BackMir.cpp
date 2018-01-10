#include "../DuiWnd/AssistPaneWnd.h"
#include "../BackMir/BackMir.h"
#include <io.h>
#include <direct.h>
#include <Shlwapi.h>
#include "../Common/SKnl3Helper.h"
#include "../GameScene/GameTextureManager.h"
#include <time.h>
#include "../Common/GlobalDefine.h"
#include "../Common/GlobalFunction.h"
#include "../GameScene/GameResourceManager.h"
#include "../GameScene/GameMapManager.h"
#include "../GameScene/GamePlayer.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameDialog/GameDlgBase.h"
#include "../Common/TimeCounter.h"
#include "../../CommonModule/ByteBuffer.h"
#include "../Net/PacketHandler.h"
#include "../GameScene/MagicEffect.h"
#include "../GameScene/GameSoundManager.h"
#include "../../CommonModule/SaveFile.h"
#include "../GameDialog/GameChatDlg.h"
#include "../resource.h"
#include "../Common/ExceptionHandler.h"
#include "../../CommonModule/CommandLineHelper.h"
#include "../Net/ClientSocket.h"
#include "../Common/ProcessInfo.h"
#include "../../CommonModule/StoveManager.h"
#include "../Common/OutlineTextureManager.h"
#include "../Common/CursorManager.h"
#include "../../CommonModule/DataEncryptor.h"
#include <UIlib.h>
#include <float.h>
#include "GlobalLuaConfig.h"
#include "BMDonateWnd.h"

#ifdef DEMO
#undef DEMO
#endif

#ifdef _THEMIDA_
#include "../Themida/ThemidaSDK.h"
#endif

//////////////////////////////////////////////////////////////////////////
#define CHECK_MEMORY_STATUS_GAP		(3 * 60 * 1000)
//	1024MB
#define RELEASE_MEMORY_THRESHOLD	(1 * 1024 * 1024 * 1024)		

//////////////////////////////////////////////////////////////////////////
//	The lua++ initialize function
TOLUA_API int  tolua_BackMirClient_open (lua_State* tolua_S);
//////////////////////////////////////////////////////////////////////////
//	Global buffer
ByteBuffer g_xBuffer(10240);
//	Game instance
MirGame* pTheGame = NULL;

//////////////////////////////////////////////////////////////////////////
MirGame* MirGame::GetInstance()
{
	return pTheGame;
}
//////////////////////////////////////////////////////////////////////////
unsigned int SendBufferToGS(ByteBuffer* _pBuf)
{
	DWORD dwPktLength = _pBuf->GetLength();
	DWORD* pWriteData = (DWORD*)const_cast<unsigned char*>(_pBuf->GetBuffer());
	*pWriteData = htonl(dwPktLength);

	if(pTheGame == NULL)
	{
		return 0;
	}

	if(pTheGame->GetGameMode() == GM_NONE)
	{
		//	?
	}
	else if(pTheGame->GetGameMode() == GM_NORMAL)
	{
		//return g_xClientSocket.SendToServer((const char*)_pBuf->GetBuffer(), _pBuf->GetLength());
		return SendBuffer(_pBuf, true);
	}
	else if(pTheGame->GetGameMode() == GM_LOGIN)
	{
		//return g_xClientSocket2.SendToServer((const char*)_pBuf->GetBuffer(), _pBuf->GetLength());
		return SendBuffer2(_pBuf, true);
	}

	return 0;
}
unsigned int SendBufferToGS(ByteBuffer& _xBuf)
{
	return SendBufferToGS(&_xBuf);
}

unsigned int SendBufferToLS(ByteBuffer* _pBuf)
{
	DWORD dwPktLength = _pBuf->GetLength();
	DWORD* pWriteData = (DWORD*)const_cast<unsigned char*>(_pBuf->GetBuffer());
	*pWriteData = htonl(dwPktLength);

	if(pTheGame == NULL)
	{
		return 0;
	}

	if(pTheGame->GetGameMode() == GM_NONE)
	{
		//	?
	}
	else if(pTheGame->GetGameMode() == GM_NORMAL)
	{
		//return g_xClientSocket.SendToServer((const char*)_pBuf->GetBuffer(), _pBuf->GetLength());
	}
	else if(pTheGame->GetGameMode() == GM_LOGIN)
	{
		//return g_xClientSocket.SendToServer((const char*)_pBuf->GetBuffer(), _pBuf->GetLength());
		return SendBuffer(_pBuf, false);
	}

	return 0;
}
unsigned int SendBufferToLS(ByteBuffer& _xBuf)
{
	return SendBufferToLS(&_xBuf);
}

unsigned int SendProto(int _nOpcode, google::protobuf::Message& _refMsg)
{
	int nSize = _refMsg.ByteSize();
	if (0 == nSize)
	{
		return 0;
	}
	nSize += 8;
	unsigned int uSizeBigendian = nSize;
	uSizeBigendian = htonl(uSizeBigendian);

	static char s_bytesBuffer[0xff];
	//	write code
	memcpy(s_bytesBuffer, &uSizeBigendian, sizeof(unsigned int));
	memcpy(s_bytesBuffer + 4, &_nOpcode, sizeof(int));
	
	if (nSize > sizeof(s_bytesBuffer) - 8)
	{
		return 0;
	}

	if (!_refMsg.SerializeToArray(s_bytesBuffer + sizeof(int) * 2, sizeof(s_bytesBuffer) - sizeof(int) * 2))
	{
		return 0;
	}

	return g_xClientSocket.SendToServer((const char*)s_bytesBuffer, nSize);
}

unsigned int SendProto2(int _nOpcode, google::protobuf::Message& _refMsg)
{
	int nSize = _refMsg.ByteSize();
	if (0 == nSize)
	{
		return 0;
	}
	nSize += 8;
	unsigned int uSizeBigendian = nSize;
	uSizeBigendian = htonl(uSizeBigendian);

	static char s_bytesBuffer[0xff];
	//	write code
	memcpy(s_bytesBuffer, &uSizeBigendian, sizeof(unsigned int));
	memcpy(s_bytesBuffer + 4, &_nOpcode, sizeof(int));

	if (nSize > sizeof(s_bytesBuffer) - 8)
	{
		return 0;
	}

	if (!_refMsg.SerializeToArray(s_bytesBuffer + sizeof(int) * 2, sizeof(s_bytesBuffer) - sizeof(int) * 2))
	{
		return 0;
	}

	const char* pBuffer = s_bytesBuffer + 8;
	DWORD* pWriteData = (DWORD*)s_bytesBuffer + 1;
	DWORD dwOpCode = *pWriteData;
	unsigned int uLen = nSize - 8;
	WORD wCheckSum = DataEncryptor::GetCheckSum(dwOpCode, pBuffer, uLen);

	DWORD dwData = MAKELONG(dwOpCode, wCheckSum);
	*pWriteData = dwData;

	return g_xClientSocket2.SendToServer((const char*)s_bytesBuffer, nSize);
}
//////////////////////////////////////////////////////////////////////////

MirGame::MirGame() : SGameBase("BackMir", WINDOW_WIDTH, WINDOW_HEIGHT)
//MirGame::MirGame() : SGameBase("BackMir", 800, 600)
{
	m_bScene = SCENE_LOGIN;
	m_pxLoginScene = NULL;
	m_pxGameRes = NULL;
	m_pGameScene = new GameScene;
	m_bInitialized = false;
	memset(m_szIP, 0, sizeof(m_szIP));
	memset(m_szHero, 0, sizeof(m_szHero));
	m_wPort = 0;
	m_bSoundEnable = false;
	m_bSoundForbid = true;
	m_bConnect = false;
	m_xLoginMsgParser.SetHandlePacket(&MirGame::_HandleLoginMsg);
	pTheGame = this;
	m_nFps = 0;
	m_dwLastCheckMemoryStatus = GetTickCount();
	m_pAssistPaneWnd = NULL;
	m_pDonateValueWnd = NULL;
	m_pDonateWnd = NULL;
	m_nDarkMode = 1;
	m_texDarkMode = 0;
	m_bShowMapSnap = true;
	m_nCursorResourceId = 0;

	// initialize static variables
	InitGameFonts();
	MagicElement::InitRender();
	EffectAttackNumber::InitRender();
}

MirGame::~MirGame()
{

}


bool MirGame::UserInitial()
{
	/*MONITORINFOEX mi;
	mi.cbSize = sizeof( mi );
	::GetMonitorInfo( m_hMonitor, &mi );

	DEVMODE devmode;
	devmode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
	if(devmode.dmBitsPerPel != 32)
	{
		MessageBox(NULL, "显示器显示位数不为真彩色(32位),请调整为真彩色,否则游戏将无法正常进行", MB_OK | MB_ICONERROR);
	}*/
#if _MSC_VER == 1500
	// Set the double precision
	unsigned int uPrevPrecision;
	 _controlfp_s(&uPrevPrecision, 0, 0);
	 _controlfp_s(0, _PC_53, MCW_PC);
#endif
	//	设置图标
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), LPCSTR(IDI_ICON1));
	::SendMessage(m_hWin, WM_SETICON, FALSE, (LPARAM)hIcon);

	CHAR strError[256] = {0}; 
	AfxInitHge(m_pHGE);
	InitLogfile();
	LoadIndexInfo();
	char szBuf[MAX_PATH];

	if(!CheckParam())
	{
		return false;
	}

#ifdef _DEBUG
#else

#ifdef _THEMIDA_
	VM_START
#endif
	/*char szMutexName[] = "fn,,szieu!";
	HANDLE hMutex = CreateMutex(NULL, false, szMutexName);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		PostQuitMessage(0);
		return false;
	}*/
#ifdef _THEMIDA_
	VM_END
#endif

#endif

	//	初始化DC句柄
	HWND hWnd = m_pHGE->System_GetState(HGE_HWND);
	HDC hDC = ::GetDC(hWnd);
	GameTextureManager::Init(hDC, m_pHGE);
	::ReleaseDC(hWnd, hDC);
	m_pHGE->System_SetState(HGE_HIDEMOUSE, false);
	AfxInitHge(m_pHGE);

	//	将服务器的socket重定位到此窗体
	if(GetGameMode() == GM_NORMAL)
	{
		//	仅有游戏服务器 则ClientSocket就是游戏服务器socket
		if ( WSAAsyncSelect(g_xClientSocket.GetSocket(), hWnd, WM_SOCKMSG, FD_CONNECT|FD_READ|FD_CLOSE ) == SOCKET_ERROR)
		{
			wsprintf(strError, "WSAAsyncSelect() generated error %d\n", WSAGetLastError());
			OutputDebugString(strError);
			return false;
		}
	}
	else if(GetGameMode() == GM_LOGIN)
	{
		//	登陆游戏服务器 则ClientSocket是登陆服务器 ClientSocket2是游戏服务器 2个都重定位
		if ( WSAAsyncSelect(g_xClientSocket.GetSocket(), hWnd, WM_SOCKMSG, FD_CONNECT|FD_READ|FD_CLOSE ) == SOCKET_ERROR)
		{
			wsprintf(strError, "WSAAsyncSelect() generated error %d\n", WSAGetLastError());
			OutputDebugString(strError);
			return false;
		}

		if ( WSAAsyncSelect(g_xClientSocket2.GetSocket(), hWnd, WM_SOCKMSG, FD_CONNECT|FD_READ|FD_CLOSE ) == SOCKET_ERROR)
		{
			wsprintf(strError, "WSAAsyncSelect() generated error %d\n", WSAGetLastError());
			OutputDebugString(strError);
			return false;
		}

		//	心跳包计时器
		SetTimer(m_hWin, TIMER_SEND_LOGIN_HEARTBEAT, 15 * 1000, NULL);
	}
	else
	{
		return false;
	}

	//	设置窗口标题
	UpdateWindowTitle(-1);

	//	设置全局异常处理器
	SetUnhandledExceptionFilter(&BM_UnhandledExceptionFilter);

	//	版本判断
	if(!CheckVersion())
	{
		::MessageBox(NULL, "文件版本不匹配", "错误", MB_ICONERROR | MB_TASKMODAL);
		return false;
	}

	//	读取套装信息
	InitItemExtraAttrib();

	//	读取分解信息
	InitItemGrade();

	//	读取锻造信息
	if(!StoveManager::GetInstance()->Init())
	{
		::MessageBox(NULL, "读取锻造信息失败", "错误", MB_ICONERROR | MB_TASKMODAL);
		return false;
	}

	//	读取全局配置
	/*sprintf(szBuf, "%s\\cfg.ini",
		GetRootPath());
	SettingLoader::GetInstance()->LoadSetting(szBuf);*/
	LoadGameSetting();

#ifdef NDEBUG

#ifdef _THEMIDA_
	VM_START
#endif
	sprintf(szBuf, "%s\\Data\\GameRes.res",
		GetRootPath());
	static char szPsw[10] = {0};
	szPsw[0] = 'X';
	szPsw[1] = 't';
	szPsw[2] = '3';
	szPsw[3] = 'F';
	szPsw[4] = 'z';
	if(false == AfxGetHge()->Resource_AttachPack(szBuf, szPsw))
	{
		AfxGetHge()->System_Log("资源包载入错误[%s]",
			szBuf);
	}

#ifdef _THEMIDA_
	VM_END
#endif

#endif

	m_pxResMgr = new hgeResourceManager("GameRes\\login.dat");
	m_pxLoginScene = new LoginScene;
	m_pxLoginScene->m_wPort = m_wPort;
	m_pxLoginScene->m_szIP = m_szIP;
	m_pxLoginScene->m_szHero = m_szHero;

	if(!m_pxLoginScene->Init(m_pxResMgr))
	{
		::MessageBox(NULL, "读取人物存档数据失败", "错误", MB_ICONERROR | MB_TASKMODAL);
		return false;
	}

	m_pxGameRes = new hgeResourceManager("GameRes\\game.dat");

	assert(_CrtCheckMemory());

	if(!GameSoundManager::GetInstancePtr()->Initialize())
	{
		AfxGetHge()->System_Log("无法初始化声音系统");
	}

	m_pHGE->System_SetState(HGE_FPS,				Config_GetFPS());  

	//	魔法消耗
	if(!GameInfoManager::GetInstance()->InitMgcCostTable())
	{
		AfxGetHge()->System_Log("魔法信息读取失败");
	}

	/*if(false == GetRunMapDataEx(m_xRunMapData))
	{
		AfxGetHge()->System_Log("地图配置读取失败");
		return false;
	}
	if(false == GetInstanceMapDataEx(m_xInsMapData))
	{
		AfxGetHge()->System_Log("副本地图配置读取失败");
		return false;
	}
	const char* pFirst = GetRunMap(0);*/

	m_bInitialized = true;

	//GameSoundManager::GetInstancePtr()->PlayBkSound(3);

	sprintf(szBuf, "%s\\Snapshot",
		GetRootPath());
	if(!PathFileExists(szBuf))
	{
		mkdir(szBuf);
	}

	//	设置脚本读取路径
#ifdef _DEBUG
	sprintf(szBuf, "%s\\Help\\",
		GetRootPath());
#else
	sprintf(szBuf, "%s\\Help\\dog.idx",
		GetRootPath());
#endif
	m_xScript.SetLuaLoadPath(szBuf);
	LoadScript(0);

	//	读取任务脚本
	if(!LoadQuestScript())
	{
		AfxGetHge()->System_Log("无法读取任务脚本");
	}

	//	初始化辅助面板
	m_pAssistPaneWnd = new AssistPaneWnd;
	m_pAssistPaneWnd->SetParentHWND(hWnd);
	m_pAssistPaneWnd->CreateDuiWindow(hWnd, "AssistPane");
	m_pAssistPaneWnd->ShowWindow();
	m_pAssistPaneWnd->AdjustWindowPos();

	// Initialize donate wnd
	m_pDonateWnd = new BMDonateWnd;
	m_pDonateWnd->CreateDuiWindow(hWnd, "DonateWnd");
	m_pDonateWnd->SetLoginAccount(m_xLoginAccount);

	m_pDonateValueWnd = new BMDonateValueWnd;
	m_pDonateValueWnd->m_pDonateWnd = m_pDonateWnd;
	m_pDonateValueWnd->CreateDuiWindow(hWnd, "DonateValueWnd");

	//	选择纹理的hge指针
	SelectedTextureManager::GetInstance()->SetHGE(m_pHGE);
	OutlineTextureManager::GetInstance()->SetHGE(m_pHGE);

	//	鼠标
	//CursorManager::GetInstance()->UpdateCursor(hWnd, kCursor_Default);
	//HTEXTURE texCursor = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM)->GetTexture(42);
	//SetGameCursor(texCursor);
	SetGameCursor(42);

	return true;
}

void MirGame::UserUninitial()
{
	//	删除单例
	delete GamePlayer::GetInstance();
	delete GameResourceManager::GetInstance();
	delete GameMapManager::GetInstance();
	delete GameInfoBoardDlg::GetInstance();
	delete GameSoundManager::GetInstancePtr();
	m_xScript.Destroy();
	//m_xQuestScript.Destroy();

	//m_pAssistPaneWnd->Close(IDOK);
	//delete m_pAssistPaneWnd;
	//m_pAssistPaneWnd = NULL;
	if(m_pAssistPaneWnd)
	{
		m_pAssistPaneWnd->WriteConfigToLocal();
	}

	if(m_pAssistPaneWnd &&
		IsWindow(m_pAssistPaneWnd->GetHWND()))
	{
		m_pAssistPaneWnd->Close();
	}
}

/************************************************************************/
/* 接受用户输入 更新
/************************************************************************/
void MirGame::UserFrameFunc()
{
	if(!m_bInitialized)
	{
		return;
	}

	float x, y;
	m_pHGE->Input_GetMousePos(&x, &y);
	POINT ptMouse = {(LONG)x, (LONG)y};

	switch(m_bScene)
	{
	case SCENE_LOGIN:
		{
			m_pxLoginScene->ProcUserCmd(ptMouse);
			m_pxLoginScene->Update(m_pHGE->Timer_GetDelta());
		}break;
	case SCENE_SELCHR:
	case SCENE_GAME:
		{
#ifdef _DEBUG
			TimerCounter xCounter;
			xCounter.Start();
#endif
			m_pGameScene->ProcKeyCmd();
			m_pGameScene->ProcUserCmd(ptMouse);
			m_pGameScene->Update(m_pHGE->Timer_GetDelta());
#ifdef _DEBUG
			if(xCounter.Stop() > 100)
			{
				m_pHGE->System_Log("[SCENE_GAME] in %s cost %d",
					__FUNCTION__, xCounter.Stop());
			}
#endif
		}break;
	}

	//	check memory status
	if(GetTickCount() - m_dwLastCheckMemoryStatus > CHECK_MEMORY_STATUS_GAP)
	{
		m_dwLastCheckMemoryStatus = GetTickCount();

		DWORD dwMemoryUsage = GetCurrentProcessMemoryUsage();
		if(dwMemoryUsage > RELEASE_MEMORY_THRESHOLD)
		{
			GameResourceManager::GetInstance()->ReleaseAllFile();
			SelectedTextureManager::GetInstance()->ClearAll();
			OutlineTextureManager::GetInstance()->ClearAll();
		}
	}
}

/************************************************************************/
/* 绘制函数
/************************************************************************/
void MirGame::UserRenderFunc()
{
	if(!m_bInitialized)
	{
		return;
	}

	if(TRUE)
	{
		if(NULL == g_hWndDC)
		{
			//	初始化DC句柄
			HWND hWnd = m_pHGE->System_GetState(HGE_HWND);
			g_hWndDC = ::GetDC(hWnd);
			GameTextureManager::Init(g_hWndDC, m_pHGE);
			::ReleaseDC(hWnd, g_hWndDC);
		}
	}

	switch(m_bScene)
	{
	case SCENE_LOGIN:
		{
			m_pxLoginScene->Render();
		}break;
	case SCENE_SELCHR:
	case SCENE_GAME:
		{
#ifdef _DEBUG
			TimerCounter xCounter;
			xCounter.Start();
#endif
			m_pGameScene->Render();
#ifdef _DEBUG
			if(xCounter.Stop() > 100)
			{
				m_pHGE->System_Log("[SCENE_GAME] in %s cost %d",
					__FUNCTION__, xCounter.Stop());
			}
#endif
		}break;
	}

	//	render cursor
	float x, y;
	m_pHGE->Input_GetMousePos(&x, &y);
	RenderCursor(x, y);
}


/************************************************************************/
/* 设置标题
/************************************************************************/
const char* MirGame::GetDifficultyLevelName(int _nDifficultyLevel)
{
	static const char* s_szDifficultyLevel[] =
	{
		"普通",
		"新手",
		"简单",
		"普通",
		"地狱"
	};

	if(_nDifficultyLevel < 0 ||
		_nDifficultyLevel >= sizeof(s_szDifficultyLevel) / sizeof(s_szDifficultyLevel[0]))
	{
		return "未知";
	}

	return s_szDifficultyLevel[_nDifficultyLevel];
}

void MirGame::UpdateWindowTitle(int _nDifficultyLevel)
{
	char szWndTitle[100];
	if(_nDifficultyLevel < 0)
	{
		sprintf(szWndTitle, "BackMIR-[%s %s]",
			BACKMIR_CURVERSION, BACKMIR_VERSION_CHNAME);
	}
	else
	{
		sprintf(szWndTitle, "BackMIR-[%s %s] (%s)",
			BACKMIR_CURVERSION, BACKMIR_VERSION_CHNAME,
			GetDifficultyLevelName(_nDifficultyLevel));
	}
	
	SetWindowText(m_hWin, szWndTitle);
}


/************************************************************************/
/* 初始化日志
/************************************************************************/
void MirGame::InitLogfile()
{
	char szfile[MAX_PATH];
	SYSTEMTIME time;
	::GetLocalTime(&time);
	sprintf(szfile, "%s\\clientlog\\", GetRootPath());
	if(!PathFileExists(szfile))
	{
		mkdir(szfile);
	}
	sprintf(szfile, "%s\\clientlog\\%02d-%02d-%02d.log",GetRootPath(), time.wYear, time.wMonth, time.wDay);
	m_pHGE->System_SetState(HGE_LOGFILE, szfile);
	m_pHGE->System_Log("日志文件初始化成功");
}

/************************************************************************/
/* 获取命令行参数
/************************************************************************/
bool MirGame::CheckParam()
{
	//assert(false);

	bool bRet = true;

	CommandLineHelper xHelper;
	if(!xHelper.InitParam())
	{
		::MessageBox(NULL, "游戏启动失败，请使用登陆器登陆", "错误", MB_ICONERROR);
		return false;
	}
	else
	{
		const char* pszLauncherKey = xHelper.GetParam("lk");
		if(pszLauncherKey == NULL ||
			0 != strcmp(pszLauncherKey, "lk0x"))
		{
			::MessageBox(NULL, "游戏启动失败，请使用登陆器(BMLauncher.exe)登陆", "错误", MB_ICONERROR);
			return false;
		}

		const char* pszValue = xHelper.GetParam("svrip");
		if(pszValue == NULL)
		{
			::MessageBox(NULL, "游戏启动失败，请使用登陆器登陆", "错误", MB_ICONERROR);
			return false;
		}
		else
		{
			char szIP[16];
			szIP[15] = 0;
			int nPortPos = 0;
			for(int i = 0; i < 16; ++i)
			{
				if(pszValue[i] == ':')
				{
					nPortPos = i;
					szIP[i] = 0;
					break;
				}
				else
				{
					szIP[i] = pszValue[i];
				}
			}
			strcpy(m_szIP, szIP);
			++nPortPos;

			for(int i = nPortPos; ; ++i)
			{
				if(pszValue[i] == 0)
				{
					szIP[i - nPortPos] = 0;
					break;
				}
				szIP[i - nPortPos] = pszValue[i];
			}
			m_wPort = (WORD)atoi(szIP);
#ifdef _DEBUG
			AfxGetHge()->System_Log("%s:%d",
				m_szIP, m_wPort);
#endif

			return true;
		}
	}

	return false;

	LPWSTR *szArglist = NULL;   
	int nArgs = 0;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);   
	if( NULL != szArglist)   
	{   
		//szArglist就是保存参数的数组   
		//nArgs是数组中参数的个数   
		//数组的第一个元素表示进程的path，也就是szArglist[0]，其他的元素依次是输入参数
		if(nArgs <= 1)
		{
			::MessageBox(NULL, "游戏启动失败，请使用登陆器登陆", "错误", MB_ICONERROR);
			//LocalFree(szArglist); 
			//PostQuitMessage(0);
			bRet = false;
		}
		if(nArgs != 2)
		{
			bRet = false;
		}

		if(bRet)
		{
			char szArgs[MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, szArglist[1], -1, szArgs, MAX_PATH, NULL, NULL);

			AfxGetHge()->System_Log(szArgs);

			char szIP[16];
			szIP[15] = 0;
			int nPortPos = 0;
			for(int i = 0; i < 16; ++i)
			{
				if(szArgs[i] == '|')
				{
					nPortPos = i;
					szIP[i] = 0;
					break;
				}
				else
				{
					szIP[i] = szArgs[i];
				}
			}
			strcpy(m_szIP, szIP);
			++nPortPos;

			for(int i = nPortPos; ; ++i)
			{
				if(szArgs[i] == 0)
				{
					szIP[i - nPortPos] = 0;
					break;
				}
				szIP[i - nPortPos] = szArgs[i];
			}
			m_wPort = (WORD)atoi(szIP);

			AfxGetHge()->System_Log("%s:%d",
				m_szIP, m_wPort);
			/*strcpy(m_szHero, szArgs);

			BYTE bIP[4];
			for(int i = 2; i < 6; ++i)
			{
				WideCharToMultiByte(CP_ACP, 0, szArglist[i], -1, szArgs, 8, NULL, NULL);
				bIP[i - 2] = (BYTE)atoi(szArgs);
			}
			sprintf(m_szIP, "%d.%d.%d.%d", bIP[0], bIP[1], bIP[2], bIP[3]);
			WideCharToMultiByte(CP_ACP, 0, szArglist[6], -1, szArgs, 12, NULL, NULL);
			m_wPort = (WORD)atoi(szArgs);
			m_pHGE->System_Log("IP:%s PORT:%d", m_szIP, m_wPort);*/
		}
		else
		{
#ifdef _DEBUG
			

			/*strcpy(m_szIP, "127.0.0.1");
			strcpy(m_szHero, "New");
			m_wPort = 8300;
			m_pHGE->System_Log("DEBUG模式");
			//GamePlayer::GetInstance()->GetAttrib()->moveSpeed = 140;
			GamePlayer::GetInstance()->SetReallyCoord(50, 10);
			GamePlayer::GetInstance()->GetAttrib()->maxHP = 100;
			//GameMapManager::GetInstance()->UpdateClientRect();
			bRet = true;;*/
#endif
		}

		//取得参数后，释放CommandLineToArgvW申请的空间   
		LocalFree(szArglist); 
	}   
	//m_pHGE->System_Log("参数检测成功，进入游戏");

	return bRet;
}
//////////////////////////////////////////////////////////////////////////
void MirGame::Log(const char* _plog)
{
	if(m_pHGE &&
		m_bInitialized)
	{
		m_pHGE->System_Log(_plog);
	}
}
//////////////////////////////////////////////////////////////////////////
void MirGame::LoadIndexInfo()
{
	//	Cloth

	int nIndex = 0;

	char szIndexFile[MAX_PATH];
	char szKey[10];
	szKey[0] = 0;
	char szValue[10];
	szValue[0] = 0;
	int nKey = 0;
	int nValue = 0;
	bool bReadKey = true;
	bool bReadValue = false;
	int nKeyIndex = 0;
	int nValueIndex = 0;
	int nRead = 0;

	sprintf(szIndexFile, "%s\\Help\\draw.idx",
		GetRootPath());

	char szBuf[10240];

	if(0 == _access(szIndexFile, 0))
	{
		//	Cloth1
		nRead = GetPrivateProfileSection("Cloth1", szBuf, sizeof(szBuf) - 1, szIndexFile);
		if(nRead > 0)
		{
			for(int i = 0; i < nRead; ++i)
			{
				if(szBuf[i] == 0)
				{
					bReadValue = false;
					bReadKey = true;

					if(nKeyIndex &&
						nValueIndex)
					{
						szKey[nKeyIndex++] = 0;
						szValue[nValueIndex++] = 0;
						nKey = atoi(szKey);
						nValue = atoi(szValue);

						if(nKey != 0)
						{
							nValue = MAKELONG(nValue, 0);
							m_xClothTable.insert(std::make_pair(nKey, nValue));
						}
					}
					nKeyIndex = nValueIndex = 0;
					continue;
				}
				else if(szBuf[i] == '=')
				{
					bReadKey = false;
					bReadValue =true;
					continue;
				}

				if(bReadKey &&
					szBuf[i] != 0)
				{
					szKey[nKeyIndex++] = szBuf[i];
				}
				else if(bReadValue &&
					szBuf[i] != 0)
				{
					szValue[nValueIndex++] = szBuf[i];
				}				
			}
		}

		//	Cloth2
		szKey[0] = 0;
		szValue[0] = 0;
		nKey = 0;
		nValue = 0;
		bReadKey = true;
		bReadValue = false;
		nKeyIndex = 0;
		nValueIndex = 0;
		nRead = 0;

		nRead = GetPrivateProfileSection("Cloth2", szBuf, sizeof(szBuf) - 1, szIndexFile);
		if(nRead > 0)
		{
			for(int i = 0; i < nRead; ++i)
			{
				if(szBuf[i] == 0)
				{
					bReadValue = false;
					bReadKey = true;

					if(nKeyIndex &&
						nValueIndex)
					{
						szKey[nKeyIndex++] = 0;
						szValue[nValueIndex++] = 0;
						nKey = atoi(szKey);
						nValue = atoi(szValue);
						if(nKey != 0)
						{
							nValue = MAKELONG(nValue, 1);
							m_xClothTable.insert(std::make_pair(nKey, nValue));
						}
					}
					nKeyIndex = nValueIndex = 0;
					continue;
				}
				else if(szBuf[i] == '=')
				{
					bReadKey = false;
					bReadValue =true;
					continue;
				}

				if(bReadKey &&
					szBuf[i] != 0)
				{
					szKey[nKeyIndex++] = szBuf[i];
				}
				else if(bReadValue &&
					szBuf[i] != 0)
				{
					szValue[nValueIndex++] = szBuf[i];
				}				
			}
		}

		//	Weapon1
		szKey[0] = 0;
		szValue[0] = 0;
		nKey = 0;
		nValue = 0;
		bReadKey = true;
		bReadValue = false;
		nKeyIndex = 0;
		nValueIndex = 0;
		nRead = 0;

		nRead = GetPrivateProfileSection("Weapon1", szBuf, sizeof(szBuf) - 1, szIndexFile);
		if(nRead > 0)
		{
			for(int i = 0; i < nRead; ++i)
			{
				if(szBuf[i] == 0)
				{
					bReadValue = false;
					bReadKey = true;

					if(nKeyIndex &&
						nValueIndex)
					{
						szKey[nKeyIndex++] = 0;
						szValue[nValueIndex++] = 0;
						nKey = atoi(szKey);
						nValue = atoi(szValue);
						if(nKey != 0)
						{
							nValue = MAKELONG(nValue, 0);
							m_xWeaponTable.insert(std::make_pair(nKey, nValue));
						}
					}
					nKeyIndex = nValueIndex = 0;
					continue;
				}
				else if(szBuf[i] == '=')
				{
					bReadKey = false;
					bReadValue =true;
					continue;
				}

				if(bReadKey &&
					szBuf[i] != 0)
				{
					szKey[nKeyIndex++] = szBuf[i];
				}
				else if(bReadValue &&
					szBuf[i] != 0)
				{
					szValue[nValueIndex++] = szBuf[i];
				}				
			}
		}

		//	Weapon2
		szKey[0] = 0;
		szValue[0] = 0;
		nKey = 0;
		nValue = 0;
		bReadKey = true;
		bReadValue = false;
		nKeyIndex = 0;
		nValueIndex = 0;
		nRead = 0;

		nRead = GetPrivateProfileSection("Weapon2", szBuf, sizeof(szBuf) - 1, szIndexFile);
		if(nRead > 0)
		{
			for(int i = 0; i < nRead; ++i)
			{
				if(szBuf[i] == 0)
				{
					bReadValue = false;
					bReadKey = true;

					if(nKeyIndex &&
						nValueIndex)
					{
						szKey[nKeyIndex++] = 0;
						szValue[nValueIndex++] = 0;
						nKey = atoi(szKey);
						nValue = atoi(szValue);
						if(nKey != 0)
						{
							nValue = MAKELONG(nValue, 1);
							m_xWeaponTable.insert(std::make_pair(nKey, nValue));
						}
					}
					nKeyIndex = nValueIndex = 0;
					continue;
				}
				else if(szBuf[i] == '=')
				{
					bReadKey = false;
					bReadValue =true;
					continue;
				}

				if(bReadKey &&
					szBuf[i] != 0)
				{
					szKey[nKeyIndex++] = szBuf[i];
				}
				else if(bReadValue &&
					szBuf[i] != 0)
				{
					szValue[nValueIndex++] = szBuf[i];
				}				
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool MirGame::LoadQuestScript()
{
	/*char szPath[MAX_PATH];

#ifdef _DEBUG
	sprintf(szPath, "%s\\Help\\questtip.lua",
		GetRootPath());

	m_xQuestScript.Destroy();
	m_xQuestScript.Create();
	tolua_BackMirClient_open(m_xQuestScript.GetVM());
	if(!m_xQuestScript.LoadFile(szPath))
	{
		return false;
	}
	return true;
#else
	sprintf(szPath, "%s\\Help\\dog.idx",
		GetRootPath());
	char szSubFile[50];
	//sprintf(szSubFile, "questtip.bbt");
	strcpy(szSubFile, "questtip.bbt");

	m_xQuestScript.Destroy();
	m_xQuestScript.Create();
	tolua_BackMirClient_open(m_xQuestScript.GetVM());
	if(!m_xQuestScript.ExecuteZip(szPath, szSubFile))
	{
		return false;
	}
	return true;
#endif*/
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool MirGame::LoadScript(int _nMapID)
{
	char szPath[MAX_PATH];

	//	清除黑夜标记
	SetDarkMode(0);
	//	清楚地图显示标记
	SetShowMapSnap(true);

	strcpy(szPath, "client/main");
	m_xScript.Destroy();
	m_xScript.Create();
	tolua_BackMirClient_open(m_xScript.GetVM());

	if(!m_xScript.DoModule(szPath))
	{
		return false;
	}

	return true;

#ifdef _DEBUG
	sprintf(szPath, "%s\\Help\\react%d.lua",
		GetRootPath(), _nMapID);

	m_xScript.Destroy();
	m_xScript.Create();
	tolua_BackMirClient_open(m_xScript.GetVM());
	if(!m_xScript.DoModule(szPath))
	{
		return false;
	}

	//	初始化地图函数
	lua_getglobal(m_xScript.GetVM(), "LoadScene");
	lua_pushinteger(m_xScript.GetVM(), _nMapID);
	tolua_pushusertype(m_xScript.GetVM(), this, "MirGame");

	int nRet = lua_pcall(m_xScript.GetVM(), 2, 0, 0);
	if(nRet != 0)
	{
		//LOG(ERROR) << "ERROR ON EXECUTE OnMonsterDead : " << lua_tostring(GetLocateScene()->GetLuaState(), -1);
		Log(lua_tostring(m_xScript.GetVM(), -1));
		lua_pop(m_xScript.GetVM(), 1);
	}

	return true;
#else
	sprintf(szPath, "%s\\Help\\dog.idx",
		GetRootPath());
	char szSubFile[50];
	sprintf(szSubFile, "react%d.bbt",
		_nMapID);

	m_xScript.Destroy();
	m_xScript.Create();
	tolua_BackMirClient_open(m_xScript.GetVM());
	if(!m_xScript.ExecuteZip(szPath, szSubFile))
	{
		return false;
	}

	//	初始化地图函数
	lua_getglobal(m_xScript.GetVM(), "LoadScene");
	lua_pushinteger(m_xScript.GetVM(), _nMapID);
	tolua_pushusertype(m_xScript.GetVM(), this, "MirGame");

	int nRet = lua_pcall(m_xScript.GetVM(), 2, 0, 0);
	if(nRet != 0)
	{
		//LOG(ERROR) << "ERROR ON EXECUTE OnMonsterDead : " << lua_tostring(GetLocateScene()->GetLuaState(), -1);
		Log(lua_tostring(m_xScript.GetVM(), -1));
		lua_pop(m_xScript.GetVM(), 1);
	}

	return true;
#endif
}
//////////////////////////////////////////////////////////////////////////
bool MirGame::GetMapName(int _id, std::string& _xName)
{
	/*int nMapID = _id;
	const char* pLetterName = GetRunMap(nMapID);
	if(NULL == pLetterName)
	{
		return false;
	}

	char szPath[MAX_PATH];
	char szMapName[20];
	sprintf(szPath, "%s\\Config\\Map.ini",
		GetRootPath());
	GetPrivateProfileString("MapNameInfo", pLetterName, "",
		szMapName, 20, szPath);
	if(strlen(szMapName) == 0)
	{
		return false;
	}
	else
	{
		_xName = szMapName;
		return true;
	}*/
	const LuaMapInfo* pMapInfo = GetMapConfigManager().GetLuaMapInfo(_id);
	if (NULL == pMapInfo) {
		// Map info not found
		return false;
	}
	_xName = pMapInfo->szMapChName;

	return true;
}

//////////////////////////////////////////////////////////////////////////
HTEXTURE MirGame::GetDarkModeTexture()
{
	static int s_nLastDarkMode = -1;

	hgeResourceManager* pResMgr = m_pxGameRes;
	if(NULL == pResMgr)
	{
		return 0;
	}

	if(0 != GetDarkMode())
	{
		if(s_nLastDarkMode != GetDarkMode())
		{
			s_nLastDarkMode = GetDarkMode();

			char szTextName[32] = {0};
			sprintf(szTextName, "dark_%d", s_nLastDarkMode);
			m_texDarkMode = pResMgr->GetTexture(szTextName);
		}
	}
	else
	{
		return 0;
	}

	return m_texDarkMode;
}

/************************************************************************/
/* 处理Socket消息
/************************************************************************/
void MirGame::OnSocketMessage(SOCKET _s, LPARAM lParam)
{
	//	解析数据包
	//static char s_szBuf[1024 * 5];

	if(WSAGETSELECTERROR(lParam))
	{
		//closesocket(_s);
		if(_s == g_xClientSocket.GetSocket())
		{
			g_xClientSocket.CloseSocket();
		}
		else if(_s == g_xClientSocket2.GetSocket())
		{
			g_xClientSocket2.CloseSocket();
		}
		return;
	}

	if(GetGameMode() == GM_NORMAL)
	{
		//	普通 只有游戏服务器
		if(_s == g_xClientSocket.GetSocket())
		{
			OnGameSvrMsg(_s, lParam);
		}
	}
	else if(GetGameMode() == GM_LOGIN)
	{
		//	登陆模式 游戏和登录服务器
		if(_s == g_xClientSocket.GetSocket())
		{
			OnLoginSvrMsg(_s, lParam);
		}
		else if(_s == g_xClientSocket2.GetSocket())
		{
			OnGameSvrMsg(_s, lParam);
		}
	}

	/*switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		{
			m_pxLoginScene->OnConnect();
		}break;
	case FD_READ:
		{
			int nRead = recv(_s, s_szBuf, 1024 * 5, 0);
			if(nRead == SOCKET_ERROR)
			{
				closesocket(_s);

				ALERT_MSGBOX("与服务器断开连接");
#ifdef _DEBUG
#else
				PostQuitMessage(0);
#endif
			}
			else
			{
				SocketDataCenter::GetInstance().PushData(s_szBuf, nRead);
			}
		}break;
	case FD_CLOSE:
		{
			closesocket(_s);

			ALERT_MSGBOX("与服务器断开连接");
#ifdef _DEBUG
#else
			PostQuitMessage(0);
#endif
		}break;
	}*/
}

void MirGame::OnLoginSvrMsg(SOCKET _s, LPARAM lParam)
{
	static char s_szBuf[1024 * 5];

	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		{
			
		}break;
	case FD_READ:
		{
			int nRead = recv(_s, s_szBuf, 1024 * 5, 0);
			if(nRead == SOCKET_ERROR)
			{
				closesocket(_s);

				ALERT_MSGBOX("与登陆服务器断开连接");
			}
			else
			{
				m_xLoginMsgParser.OnSocketData(s_szBuf, nRead);
			}
		}break;
	case FD_CLOSE:
		{
			//closesocket(_s);
			if(_s == g_xClientSocket.GetSocket())
			{
				g_xClientSocket.CloseSocket();
			}
			else if(_s == g_xClientSocket2.GetSocket())
			{
				g_xClientSocket2.CloseSocket();
			}

			ALERT_MSGBOX("与登陆服务器断开连接");
		}break;
	}
}

void MirGame::OnGameSvrMsg(SOCKET _s, LPARAM lParam)
{
	static char s_szBuf[1024 * 5];

	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		{
			m_pxLoginScene->OnConnect();
		}break;
	case FD_READ:
		{
			int nRead = recv(_s, s_szBuf, 1024 * 5, 0);
			if(nRead == SOCKET_ERROR)
			{
				if(_s == g_xClientSocket.GetSocket())
				{
					g_xClientSocket.CloseSocket();
				}
				else if(_s == g_xClientSocket2.GetSocket())
				{
					g_xClientSocket2.CloseSocket();
				}

				ALERT_MSGBOX("与游戏服务器断开连接");
#ifdef _DEBUG
#else
				PostQuitMessage(0);
#endif
			}
			else
			{
				SocketDataCenter::GetInstance().PushData(s_szBuf, nRead);
			}
		}break;
	case FD_CLOSE:
		{
			//closesocket(_s);
			if(_s == g_xClientSocket.GetSocket())
			{
				g_xClientSocket.CloseSocket();
			}
			else if(_s == g_xClientSocket2.GetSocket())
			{
				g_xClientSocket2.CloseSocket();
			}

			ALERT_MSGBOX("与游戏服务器断开连接");
#ifdef _DEBUG
#else
			PostQuitMessage(0);
#endif
		}break;
	}
}
 
/************************************************************************/
/* 过滤windows消息
/************************************************************************/
 LRESULT MirGame::WinEventFilter(HWND h, UINT u, WPARAM w, LPARAM l)
 {
	LRESULT lRet = 0;
	MSG msg;
	msg.message = u;
	msg.wParam = w;
	msg.lParam = l;
	msg.hwnd = h;
	//DuiLib::CPaintManagerUI::TranslateMessage(&msg);

	if(m_pAssistPaneWnd)
	{
		if(h == m_pAssistPaneWnd->GetHWND())
		{
			TranslateMessage(&msg);
		}
	}

 	switch (u)
 	{
 	case WM_DLG_BRINGTOTOP:
 		{
 			m_pGameScene->BringDlgToTop((RenderObject*)w);
 		}break;
	case WM_SOCKMSG:
		{
			OnSocketMessage(w, l);
		}break;
	case WM_SCENE_PROG:
		{
			//	场景进度
			m_pxLoginScene->OnSceneProg(w, l);
		}break;
	case WM_ENTER_GAMESCENE:
		{
			//	进入游戏场景
			if(m_pxLoginScene->GetPage() == PAGE_LOAD)
			{
				SAFE_DELETE(m_pxLoginScene);
				m_bScene = SCENE_GAME;
				ReleaseLoginRes();
				GameSoundManager::GetInstancePtr()->StopBkSound();
				GameSoundManager::GetInstancePtr()->PlayBkSound(2);
				PkgGameLoadedAck ack;
				ack.uUserId = GamePlayer::GetInstance()->GetHandlerID();
				g_xBuffer.Reset();
				g_xBuffer << ack;
				SendBufferToGS(&g_xBuffer);
				m_pGameScene->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[公告]欢迎进入梦回传奇的世界",ARGB_YELLOW);
				m_pGameScene->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[提示]如需帮助，按E键，选择游戏帮助。游戏地图内网址均与本游戏无关，均为网络素材",ARGB_YELLOW);
				m_pGameScene->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[提示]CTRL+H 更改攻击模式",ARGB_YELLOW);
				//m_pGameScene->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[帮助]人物 F9",ARGB_RED);
				//m_pGameScene->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[帮助]背包 F10",ARGB_RED);
				//m_pGameScene->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[帮助]技能 F11",ARGB_RED);
				//m_pGameScene->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[帮助]大地图 TAB",ARGB_RED);
				//m_pGameScene->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[帮助]捡取 空格 或 CTRL加鼠标左键",ARGB_RED);
				//m_pGameScene->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[帮助]截图 CTRL+S",ARGB_RED);
				PkgPlayerQuickMsgNtf ntf;
				ntf.nMsgID = QMSG_ATTACKMODE;
				ntf.nParam = 0;
				GamePlayer::GetInstance()->DoPacket(ntf);
				//	游戏场景初始化
				Sleep(50);
			}
		}break;
	case WM_TIMER:
		{
			if(g_xClientSocket.IsConnected())
			{
				PkgLoginHeartBeatNot not;
				g_xBuffer.Reset();
				g_xBuffer << not;
				SendBufferToLS(g_xBuffer);
			}
		}break;
	case WM_CLOSE:
		{
			if(m_bScene == SCENE_GAME)
			{
				if(0 != m_nCursorResourceId)
				{
					HCURSOR hPrev = SetCursor(NULL);
					while(0 != hPrev)
					{
						hPrev = SetCursor(NULL);
					}
				}
				if(m_pGameScene)
				{
					m_pGameScene->RequestCloseGame();
					//	processed
					return 1;
				}
				else
				{
					return 0;
				}
			}
			return 0;
		}break;
	case WM_MOVE:
		{
			if(m_pAssistPaneWnd &&
				IsWindow(m_pAssistPaneWnd->GetHWND()))
			{
				m_pAssistPaneWnd->AdjustWindowPos();
			}
		}break;
	case WM_SETCURSOR:
		{
			//HCURSOR hCursor = CursorManager::GetInstance()->GetTheCursor();
			/*HCURSOR hCursor = LoadCursor(NULL, IDC_CROSS);
			if(NULL != hCursor)
			{
				SetCursor(hCursor);
			}*/
		}break;
	case WM_BACK_SELCHR:
		{
			if(GetCurStage() == SCENE_GAME)
			{
				//	回到人物界面
				GameSoundManager::GetInstancePtr()->StopBkSound();
				GameSoundManager::GetInstancePtr()->PlayBkSound(3);
				SetCurState(SCENE_LOGIN);
				m_pxLoginScene = new LoginScene;
				m_pxLoginScene->m_wPort = m_wPort;
				m_pxLoginScene->m_szIP = m_szIP;
				m_pxLoginScene->m_szHero = m_szHero;
				m_pxLoginScene->Init(m_pxResMgr);
				m_pxLoginScene->OnPressNetButton();
			}
		}break;
	case WM_HIDE_ASSITWND:
		{
			if (GetCurStage() == SCENE_GAME) {
				// Notify GameScene to show open assit wnd button
				if (NULL != m_pGameScene) {
					m_pGameScene->OnAssistWndClose();
				}
			}
		}break;
 	}

	return __super::WinEventFilter(h, u, w, l);
 }







//
bool MirGame::IsTimerFunctionRepalaced()
{
	HMODULE hModule = LoadLibrary("kernel32.dll");
	bool bCheat = false;
	if(hModule)
	{
		FARPROC pFunc = GetProcAddress(hModule, "GetTickCount");
		BYTE bSig = *(BYTE*)pFunc;

		//	E9 is jmp command
		if(bSig == 0xE9)
		{
			bCheat = true;
		}

		FreeLibrary(hModule);
	}
	return bCheat;
}

//////////////////////////////////////////////////////////////////////////
void MirGame::GetSnapshotFile(std::string& _file)
{
	char szBuf[MAX_PATH];
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(szBuf, "%s\\snapshot\\%04d%02d%02d%02d%02d.jpg",
		GetRootPath(), st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	_file = szBuf;
}
//////////////////////////////////////////////////////////////////////////
bool MirGame::CheckItemValid(const ItemAttrib* _pChecked)
{
	if(GamePlayer::GetInstance()->IsEquipItem(*_pChecked))
	{
		ItemAttrib oriitem;
		if(GameInfoManager::GetInstance()->GetItemAttrib(_pChecked->id, &oriitem))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

bool MirGame::IsPassiveSkill(int _nMgcID)
{
	if(_nMgcID == MEFF_BASESWORD ||
		_nMgcID == MEFF_GONGSHA ||
		_nMgcID == MEFF_MOUNTAIN ||
		_nMgcID == MEFF_BERSERKER ||
		_nMgcID == MEFF_KTSWORD ||
		_nMgcID == MEFF_SPIRIT)
	{
		return true;
	}
	return false;
}

bool MirGame::IsWindowMode()
{
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\cfg.ini", GetRootPath());

	int nFullScreen = GetPrivateProfileInt("SETTING", "FULLSCREEN", 0, szPath);
	//m_pHGE->System_Log("Full screen : %d", nFullScreen);

	return (nFullScreen != 0 ? false : true);
}

void MirGame::LoadGameSetting()
{
	char szBuf[MAX_PATH];
	sprintf(szBuf, "%s\\conf\\game.ini",
		GetRootPath());

	if(!PathFileExists(szBuf))
	{
		HANDLE hFile = ::CreateFile(szBuf, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
		if(INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}
		else
		{
			CloseHandle(hFile);
			m_xSettings.LoadSetting(szBuf);
			WriteDefaultSetting();
			m_xSettings.SaveToFile(szBuf);
		}
	}
	else
	{
		m_xSettings.LoadSetting(szBuf);
	}

	UpdateConfigCache();
}

void MirGame::WriteSettingsToFile()
{
	char szBuf[MAX_PATH];
	sprintf(szBuf, "%s\\conf\\game.ini",
		GetRootPath());
	m_xSettings.SaveToFile(szBuf);

	UpdateConfigCache();
}

void MirGame::UpdateConfigCache()
{
	m_bConfigCache[0] = (m_xSettings.GetIntValue("SOUNDENABLE") != 0);
	m_bConfigCache[1] = (m_xSettings.GetIntValue("ITEMFLASH") != 0);
	m_bConfigCache[2] = (m_xSettings.GetIntValue("RENDERHPBAR") != 0);
	m_bConfigCache[3] = (m_xSettings.GetIntValue("WNDMODE") != 0);
	m_bConfigCache[4] = (m_xSettings.GetIntValue("SELFNAME") != 0);
	m_bConfigCache[5] = (m_xSettings.GetIntValue("ENHANCESEL") != 0);
	m_bConfigCache[6] = (m_xSettings.GetIntValue("JPGSNAP") != 0);
	m_bConfigCache[7] = (m_xSettings.GetIntValue("HPANIMATION") != 0);
	m_bConfigCache[8] = (m_xSettings.GetIntValue("TILESURFACE") != 0);
	m_bConfigCache[9] = (m_xSettings.GetIntValue("MOUSEPICKUP") != 0);
	m_bConfigCache[10] = (m_xSettings.GetIntValue("DISABLERENDERNAMEFRAME") != 0);
	m_bConfigCache[11] = (m_xSettings.GetIntValue("DISABLERENDERCHATFRAME") != 0);
	m_nFps = m_xSettings.GetIntValue("FPS");
}

void MirGame::WriteDefaultSetting()
{
	m_xSettings.WriteIntValue("SOUNDENABLE", 1);
	m_xSettings.WriteIntValue("RENDERHPBAR", 1);
	m_xSettings.WriteIntValue("ITEMFLASH",	 1);
	m_xSettings.WriteIntValue("WNDMODE",	 1);
	m_xSettings.WriteIntValue("SELFNAME",	 1);
	m_xSettings.WriteIntValue("ENHANCESEL",	 1);
	m_xSettings.WriteIntValue("FPS",		 90);
	m_xSettings.WriteIntValue("JPGSNAP",	0);
	m_xSettings.WriteIntValue("HPANIMATION", 1);
	m_xSettings.WriteIntValue("TILESURFACE", 0);
	m_xSettings.WriteIntValue("MOUSEPICKUP", 1);
	m_xSettings.WriteIntValue("DISABLERENDERNAMEFRAME", 0);
	m_xSettings.WriteIntValue("DISABLERENDERCHATFRAME", 0);
}

bool MirGame::Config_EnableSound()
{
	return m_bConfigCache[0];
	return (m_xSettings.GetIntValue("SOUNDENABLE") != 0);
}

bool MirGame::Config_ItemFlash()
{
	return m_bConfigCache[1];
	return (m_xSettings.GetIntValue("ITEMFLASH") != 0);
}

bool MirGame::Config_RenderHPBar()
{
	return m_bConfigCache[2];
	return (m_xSettings.GetIntValue("RENDERHPBAR") != 0);
}

bool MirGame::Config_WindowMode()
{
	return m_bConfigCache[3];
	return (m_xSettings.GetIntValue("WNDMODE") != 0);
}

bool MirGame::Config_SelfName()
{
	return m_bConfigCache[4];
	return (m_xSettings.GetIntValue("SELFNAME") != 0);
}

bool MirGame::Config_EnhanceSel()
{
	return m_bConfigCache[5];
}

bool MirGame::Config_JpgSnap()
{
	return m_bConfigCache[6];
}

bool MirGame::Config_HPAnimation()
{
	return m_bConfigCache[7];
}

bool MirGame::Config_TileSurface()
{
	return m_bConfigCache[8];
}

bool MirGame::Config_MousePickUp()
{
	return m_bConfigCache[9];
}

int MirGame::Config_GetFPS()
{
	if(m_nFps == 0)
	{
		return 60;
	}
	return m_nFps;
}

bool MirGame::Config_DisableRenderNameFrame()
{
	return m_bConfigCache[10];
}

bool MirGame::Config_DisableRenderChatFrame()
{
	return m_bConfigCache[11];
}

void MirGame::PlayItemPutDownSound(const ItemAttrib* _pItem)
{
	if(pTheGame->IsEquipItem(*_pItem))
	{
		if(_pItem->type == ITEM_WEAPON)
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMWEAPOND);
		}
		else if(_pItem->type == ITEM_CLOTH ||
			_pItem->type == ITEM_HELMET ||
			_pItem->type == ITEM_BELT ||
			_pItem->type == ITEM_SHOE)
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMCLOTHD);
		}
		else
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMRINGD);
		}
	}
	else
	{
		if(_pItem->type == ITEM_COST &&
			_pItem->curse == 0)
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMDRUGD);
		}
		else if(_pItem->type == ITEM_COST &&
			(_pItem->curse == 1 || _pItem->curse == 2))
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMSCROLLD);
		}
		else
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMDEFAULTD);
		}
	}
}

void MirGame::PlayItemClickUpSound(const ItemAttrib* _pItem)
{
	if(pTheGame->IsEquipItem(*_pItem))
	{
		if(_pItem->type == ITEM_WEAPON)
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMWEAPONC);
		}
		else if(_pItem->type == ITEM_CLOTH ||
			_pItem->type == ITEM_HELMET ||
			_pItem->type == ITEM_BELT ||
			_pItem->type == ITEM_SHOE)
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMCLOTHC);
		}
		else
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMRINGC);
		}
	}
	else
	{
		if(_pItem->type == ITEM_COST &&
			_pItem->curse == 0)
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMDRUGC);
		}
		else if(_pItem->type == ITEM_COST &&
			(_pItem->curse == 1 || _pItem->curse == 2))
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMSCROLLC);
		}
		else
		{
			GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ITEMDEFAULTC);
		}
	}
}

bool MirGame::IsInPlayerViewRange(int _nCoordX, int _nCoordY)
{
	GamePlayer* pSelf = GamePlayer::GetInstance();

	RECT rcView;
	rcView.left = (int)pSelf->GetCoordX() - VIEW_WIDTH / UNIT_WIDTH;
	rcView.right = (int)pSelf->GetCoordX() + VIEW_WIDTH / UNIT_WIDTH;
	rcView.top = (int)pSelf->GetCoordY() - VIEW_HEIGHT / UNIT_HEIGHT;
	rcView.bottom = (int)pSelf->GetCoordY() + VIEW_HEIGHT / UNIT_HEIGHT;

	POINT ptPos = {_nCoordX, _nCoordY};

	if(PtInRect(&rcView, ptPos))
	{
		return true;
	}
	return false;
}

bool MirGame::SetGameCursor(int _nResourceId)
{
	if(_nResourceId == m_nCursorResourceId)
	{
		return true;
	}

	if(0 == _nResourceId)
	{
		//	use system mouse.
		m_pHGE->System_SetState(HGE_HIDEMOUSE, false);

		if(NULL != m_pSprCursor)
		{
			//	free previous texture
			HTEXTURE texPrev = m_pSprCursor->GetTexture();
			m_pSprCursor->SetTexture(NULL);
			m_pHGE->Texture_Free(texPrev);

			delete m_pSprCursor;
			m_pSprCursor = NULL;
		}

		m_nCursorResourceId = _nResourceId;

		return true;
	}

	HTEXTURE texSource = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM)->GetTexture(_nResourceId);
	if(NULL == texSource)
	{
		return false;
	}

	//	copy an backup
	int nWidth = m_pHGE->Texture_GetWidth(texSource);
	int nHeight = m_pHGE->Texture_GetHeight(texSource);

	HTEXTURE texCursor = m_pHGE->Texture_Create(nWidth, nHeight);
	if(!texCursor)
	{
		return false;
	}

	DWORD* pPtrSrc = m_pHGE->Texture_Lock(texSource);
	DWORD* pPtrDest = m_pHGE->Texture_Lock(texCursor, false);

	for(int i = 0; i < nHeight; ++i)
	{
		for(int j = 0; j < nWidth; ++j)
		{
			pPtrDest[i * nWidth + j] = pPtrSrc[i * nWidth + j];
		}
	}

	m_pHGE->Texture_Unlock(texSource);
	m_pHGE->Texture_Unlock(texCursor);

	if(NULL == m_pSprCursor)
	{
		m_pSprCursor = new hgeSprite(NULL, 0, 0, 0, 0);
	}
	else
	{
		//	free previous texture
		HTEXTURE texPrev = m_pSprCursor->GetTexture();
		m_pSprCursor->SetTexture(NULL);
		m_pHGE->Texture_Free(texPrev);
	}

	m_pSprCursor->SetTexture(texCursor);
	m_pSprCursor->SetTextureRect(0, 0, nWidth, nHeight);
	m_nCursorResourceId = _nResourceId;

	m_pHGE->System_SetState(HGE_HIDEMOUSE, true);

	return true;
}

void MirGame::RenderCursor(float _fx, float _fy)
{
	if(0 == m_nCursorResourceId)
	{
		return;
	}
	if(NULL == m_pSprCursor)
	{
		return;
	}
	if(!m_pHGE->Input_IsMouseOver())
	{
		return;
	}

	m_pSprCursor->Render(_fx, _fy);
}

void MirGame::RequestSmallQuit()
{
	if(GetCurStage() != SCENE_GAME)
	{
		return;
	}
	if(GamePlayer::GetInstance()->IsRequestSmallQuit())
	{
		return;
	}

	PkgPlayerQuitSelChrReq req;
	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(g_xBuffer);

	GamePlayer::GetInstance()->SetRequestSmallQuit(true);
}