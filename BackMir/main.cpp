#include "backmir.h"
#include "BMPreConnWnd.h"
#include "GlobalLuaConfig.h"
#include <direct.h>
#include <Windows.h>

int CreateGameDirs()
{
	char szRoot[MAX_PATH];
	GetRootPath(szRoot, MAX_PATH);

	char szDir[MAX_PATH];
	sprintf(szDir, "%s\\conf", szRoot);

	mkdir(szDir);

	return 0;
}

int CALLBACK WinMain( IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nShowCmd )
{
	//	设置语言
	setlocale(LC_ALL, "");

	// create all directory
	CreateGameDirs();

#ifdef _DELAY_LOAD_DLL
	// set dll directory
	const char* pszAppPath = GetRootPath();
	char szDLLDir[MAX_PATH];
	strcpy(szDLLDir, pszAppPath);
#ifdef _DEBUG
	strcat_s(szDLLDir, "\\deps_d\\");
#else
	strcat_s(szDLLDir, "\\deps\\");
#endif
	if (TRUE != SetDllDirectory(szDLLDir))
	{
		::MessageBox(NULL, "无法初始化DLL模块", "错误", MB_ICONERROR | MB_OK);
		return 1;
	}
#endif

#ifdef _DEBUG
	if(!IsDebuggerPresent())
	{
		MessageBox(NULL, "", "", MB_OK);
	}
#endif
	// load global lua config
	if (0 != LoadGlobalLuaConfig())
	{
		::MessageBox(NULL, "无法读取全局配置", "错误", MB_ICONERROR | MB_OK);
		return 1;
	}

	//	Load PreConnWnd
	char szDuiResPath[MAX_PATH];
	GetRootPath(szDuiResPath, MAX_PATH);
#ifdef _DEBUG
	strcat(szDuiResPath, "/../BackMirClientVC12/UI");
#else
	strcat(szDuiResPath, "/UI");
#endif
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(szDuiResPath);

	HRESULT hr = CoInitialize(NULL);
	if(FAILED(hr))	return 0;

	//	Create PreConnWnd
	BMPreConnWnd* pPreConnWnd = new BMPreConnWnd;
	pPreConnWnd->Create(NULL, "PreConnWnd", UI_WNDSTYLE_FRAME, 0, 0, 0, 300, 180, 0);
	pPreConnWnd->CenterWindow();
	pPreConnWnd->ShowModal();

	if(!g_bPrepared)
	{
		return -1;
	}
	if (!g_bDataLoaded)
	{
		return -1;
	}
	if(g_eGameMode != GM_LOGIN &&
		g_eGameMode != GM_NORMAL)
	{
		return -1;
	}

	//	Run the game
	MirGame game;
	game.SetGameMode(g_eGameMode);
	game.Init();
	game.Exec();

	//	Release
	CoUninitialize();
	return 0;
}