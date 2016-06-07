#include "backmir.h"
#include "BMPreConnWnd.h"
#include <Windows.h>

int CALLBACK WinMain( IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nShowCmd )
{
	//	ÉèÖÃÓïÑÔ
	setlocale(LC_ALL, "");

#ifdef _DEBUG
	if(!IsDebuggerPresent())
	{
		MessageBox(NULL, "", "", MB_OK);
	}
#endif

	//	Load PreConnWnd
	CPaintManagerUI::SetInstance(hInstance);
#ifdef _DEBUG
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + "/../BackMirClient/UI");
#else
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + "/UI");
#endif

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