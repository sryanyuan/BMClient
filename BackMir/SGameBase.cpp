#include "SGameBase.h"

#include <Windows.h>  
#include <assert.h>  
#include <stdio.h>  
#include <io.h>  

//////////////////////////////////////////////////////  
//  
//  构造析构  
//  
///////////////////////////////////////////////////////  

SGameBase::SGameBase(const char* szTitle,int nViewWidth/* =800 */,int nViewHeight/* =600 */)
{  
	m_pGUI=NULL;  
	m_pHGE=NULL;  
	m_pSprCursor=NULL;  
	m_nViewWidth=nViewWidth;
	m_nViewHeight=nViewHeight;


	//得到自身实例  
	SGameBase::InitialInstance(this);  

// 	if(!szTitle)  
// 	{  
// 		memset(m_szGameTitle,0,sizeof(m_szGameTitle));  
// 	}  
// 	else  
// 	{  
// 		strcpy(m_szGameTitle,szTitle);  
// 	}  
}  

SGameBase::~SGameBase()  
{  
	//this->HgeUninitial();  
}  

//////////////////////////////////////////////////////////////////////////
bool SGameBase::Init()
{
	const char* szTitle = GetGameTitle();
	this->HgeInitial(szTitle);  

	m_hWin=NULL;
	this->m_hWin=m_pHGE->System_GetState(HGE_HWND);
	if(!m_hWin)
	{
		S_DEBUG;
		abort();
	}

	SGameBase::InitialProc((WNDPROC)GetWindowLong(m_pHGE->System_GetState(HGE_HWND),GWL_WNDPROC));  
	SetWindowLong(m_pHGE->System_GetState(HGE_HWND),GWL_WNDPROC,(LONG)SGameBase::MainWinProc);  

	return true;
}

//////////////////////////////////////////////////////  
//  
//  静态函数实现  
//  
///////////////////////////////////////////////////////  

SGameBase*            SGameBase::InitialInstance(SGameBase * p/* =NULL */)  
{  
	//保存一个实例  
	//参数空则返回上次初始化的实例  

	static SGameBase* pInstance=NULL;  

	if(!p)  
	{  
		return pInstance;  
	}  

	pInstance=p;  
	return pInstance;  
}  

WNDPROC             SGameBase::InitialProc(WNDPROC p)  
{  
	static WNDPROC pFunc=NULL;  

	if(!p)  
	{  
		return pFunc;  
	}  

	pFunc=p;  
	return pFunc;  
}  

LRESULT             SGameBase::MainWinProc(HWND h, UINT u, WPARAM w, LPARAM l)  
{  
	LRESULT lRet = SGameBase::InitialInstance()->WinEventFilter(h,u,w,l);
	if(u == WM_CLOSE)
	{
		if(lRet == 1)
		{
			return 0;
		}
	}
	return CallWindowProc(SGameBase::InitialProc(),h,u,w,l);
}  

bool                SGameBase::FrameFunc()  
{  
	return SGameBase::InitialInstance()->RealFrameFunc();  
}  

bool                SGameBase::RenderFunc()  
{  
	return SGameBase::InitialInstance()->RealRenderFunc();  
}  

bool                SGameBase::FocusGainFunc()  
{  
	return SGameBase::InitialInstance()->RealFocusGainFunc();  
}  

bool                SGameBase::FocusLostFunc()  
{  
	return SGameBase::InitialInstance()->RealFocusLostFunc();  
}  

bool                SGameBase::GfxRestoreFrameFunc()  
{  
	return SGameBase::InitialInstance()->RealGfxRestoreFunc();  
}  



//////////////////////////////////////////////////////  
//  
//  初始化及清理函数  
//  
///////////////////////////////////////////////////////  
bool                SGameBase::HgeInitial(const char* szTitle)  
{  
	m_pHGE=hgeCreate(HGE_VERSION);  
	m_pGUI=new hgeGUI;  

	assert(m_pHGE);  
	assert(m_pGUI);  
	//settings  

	m_pHGE->System_SetState(HGE_FRAMEFUNC,			SGameBase::FrameFunc);  
	m_pHGE->System_SetState(HGE_RENDERFUNC,			SGameBase::RenderFunc);  
	m_pHGE->System_SetState(HGE_FOCUSGAINFUNC,		SGameBase::FocusGainFunc);  
	m_pHGE->System_SetState(HGE_FOCUSLOSTFUNC,		SGameBase::FocusLostFunc);  
	m_pHGE->System_SetState(HGE_GFXRESTOREFUNC,		SGameBase::GfxRestoreFrameFunc);  



	m_pHGE->System_SetState(HGE_DONTSUSPEND,		true);  
	m_pHGE->System_SetState(HGE_SHOWSPLASH,			false);  
#ifdef _DEBUG
	m_pHGE->System_SetState(HGE_FPS,				30);  
#endif
	m_pHGE->System_SetState(HGE_SCREENBPP,			32);  
	if(szTitle)
	{
		m_pHGE->System_SetState(HGE_TITLE,				""); 
	}

	bool bWindowed = IsWindowMode();

	m_pHGE->System_SetState(HGE_USESOUND,			false);  
	m_pHGE->System_SetState(HGE_WINDOWED,			bWindowed);  
	m_pHGE->System_SetState(HGE_HIDEMOUSE,			true);  

	if(bWindowed)
	{
		m_pHGE->System_SetState(HGE_SCREENWIDTH,		m_nViewWidth);  
		m_pHGE->System_SetState(HGE_SCREENHEIGHT,		m_nViewHeight);
	}
	else
	{
		int width=GetSystemMetrics(SM_CXSCREEN);  
		int height=GetSystemMetrics(SM_CYSCREEN);  

		m_pHGE->System_SetState(HGE_SCREENWIDTH,		800);  
		m_pHGE->System_SetState(HGE_SCREENHEIGHT,		600);
	}

	m_bHgeInitial=m_pHGE->System_Initiate();  
	this->GetGameViewRectNew();
	return true;  
}  

void                SGameBase::HgeUninitial()  
{  
	this->UserUninitial();  
	m_pHGE->System_Shutdown();  
	m_pHGE->Release();  
}  

bool                SGameBase::UserInitial()  
{  
	return true;  
}  

void                SGameBase::UserUninitial()  
{  

}  
//////////////////////////////////////////////////////  
//  
//  辅助函数  
//  
///////////////////////////////////////////////////////  

bool				SGameBase::IsCaptionBar(int x,int y)
{
	if(y <= GetSystemMetrics(SM_CYCAPTION))  
	{  
		return true;  
	}  

	return false;  
}
//////////////////////////////////////////////////////  
//  
//  过滤消息的窗口过程  
//  
///////////////////////////////////////////////////////  
LRESULT             SGameBase::WinEventFilter(HWND h, UINT u, WPARAM w, LPARAM l)  
{  
	switch(u)
	{
	case WM_TIMER:
		{
			this->OnGameTimer(w);
		}
		break;
	}
	return w;  
}  
//////////////////////////////////////////////////////  
//  
//  逻辑和渲染函数实现  
//  
///////////////////////////////////////////////////////  
bool                SGameBase::RealFocusGainFunc()  
{  
	return false;  
}  

bool                SGameBase::RealFocusLostFunc()  
{  
	return false;  
}  

bool				SGameBase::RealGfxRestoreFunc()  
{  
	return true;  
}  

bool                SGameBase::RealFrameFunc()  
{  
	//this->UpdateCommand();  
	this->UserFrameFunc();  
	//return this->UserProcCommand();  
	return false;
}  

bool                SGameBase::RealRenderFunc()  
{  
	//
	//	重写时必须调用此基类函数
	//
	m_pHGE->Gfx_BeginScene();  
	m_pHGE->Gfx_Clear(0);  
	this->UserRenderFunc();  
	m_pGUI->Render();  
	m_pHGE->Gfx_EndScene();  
	return false;  
}  

void                SGameBase::UpdateCommand()  
{  
	m_fTimeDelta=m_pHGE->Timer_GetDelta();  
	m_nCommandID=m_pGUI->Update(m_fTimeDelta);  
}  

bool                SGameBase::UserProcCommand()  
{  
// 	if(m_pHGE->Input_GetKeyState(HGEK_ESCAPE))  
// 	{  
// 		return true;  
// 	}  

	//处理GUI消息  
	switch(this->m_nCommandID)  
	{  
	default:  
		break;  
	}  

	//若退出 在switch语句中返回true  
	return false;  
}  

void                SGameBase::UserFrameFunc()  
{  
	//用户实现  
}  

void                SGameBase::UserRenderFunc()  
{  
	//用户实现  
}  

void				SGameBase::OnGameTimer(UINT nEventID)
{

}
//////////////////////////////////////////////////////  
//  
//  外部接口  
//  
///////////////////////////////////////////////////////  
bool                SGameBase::SetGameCursor(const char* szCursorPath)  
{
	//	Free previous resource
	if(m_hCursor)
	{
		m_pHGE->Texture_Free(m_hCursor);
		m_hCursor = NULL;
	}

	m_hCursor=m_pHGE->Texture_Load(szCursorPath);  
	if(m_hCursor)  
	{
		if(NULL == m_pSprCursor)
		{
			m_pSprCursor=new hgeSprite(m_hCursor,
				0.0f,0.0f,
				m_pHGE->Texture_GetWidth(m_hCursor,true),
				m_pHGE->Texture_GetHeight(m_hCursor,true)); 
		}
		else
		{
			m_pSprCursor->SetTexture(m_hCursor);
			m_pSprCursor->SetTextureRect(0, 0, m_pHGE->Texture_GetWidth(m_hCursor,true),
				m_pHGE->Texture_GetHeight(m_hCursor,true));
		}

		m_pSprCursor->SetZ(1);  
		m_pSprCursor->SetHotSpot(0,0);  
		m_pSprCursor->SetColor(0xFFFFFFFF);  
		m_pGUI->SetCursor(m_pSprCursor);  
		//m_pGUI->Enter();
		return true;  
	}  
	S_DEBUG;  
	return false;  
}  

bool				SGameBase::SetGameCursor(HTEXTURE _texCursor)
{
	//	copy an backup
	int nWidth = m_pHGE->Texture_GetWidth(_texCursor);
	int nHeight = m_pHGE->Texture_GetHeight(_texCursor);

	HTEXTURE texCursor = m_pHGE->Texture_Create(nWidth, nHeight);
	if(!texCursor)
	{
		return false;
	}

	DWORD* pPtrSrc = m_pHGE->Texture_Lock(_texCursor);
	DWORD* pPtrDest = m_pHGE->Texture_Lock(texCursor, false);

	for(int i = 0; i < nHeight; ++i)
	{
		for(int j = 0; j < nWidth; ++j)
		{
			pPtrDest[i * nWidth + j] = pPtrSrc[i * nWidth + j];
		}
	}

	m_pHGE->Texture_Unlock(_texCursor);
	m_pHGE->Texture_Unlock(texCursor);

	//	copied and new sprite
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
	m_pSprCursor->SetZ(1);  
	m_pSprCursor->SetHotSpot(0,0);  
	m_pSprCursor->SetColor(0xFFFFFFFF);  
	m_pGUI->SetCursor(m_pSprCursor);  

	return true;
}

void                SGameBase::EnableSound(bool b)  
{  
	m_pHGE->System_SetState(HGE_USESOUND,b);  
}  

bool                SGameBase::SetGameTitle(const char* szTitle)  
{  
// 	if(strlen(szTitle) > sizeof(m_szGameTitle))  
// 	{  
// 		return false;  
// 	}  
// 	strcpy(m_szGameTitle,szTitle);  
	m_pHGE->System_SetState(HGE_TITLE,szTitle);  
	return true;  
}  

void				SGameBase::SetGameFps(int nFps)
{
	m_pHGE->System_SetState(HGE_FPS,nFps);
}

void                SGameBase::Exec()  
{  
	if(m_bHgeInitial)  
	{  
		if(this->UserInitial())  
		{  
			m_pGUI->Enter();
			m_pHGE->System_Start();  
		}  
		else  
		{  
			abort();  
		}  
	}  
	else  
	{  
		abort();  
	}

	HgeUninitial();
}  

void				SGameBase::SetGameLogFile(const char* szFile)
{
	m_pHGE->System_SetState(HGE_LOGFILE,szFile);
}

void				SGameBase::AddLog(const char* szLog)
{
	m_pHGE->System_Log(szLog);
}

RECT&				SGameBase::GetGameViewRectNew()
{
	::GetClientRect(m_hWin,&m_rcView);
	return m_rcView;
}

RECT&				SGameBase::GetGameViewRect()
{
	return m_rcView;
}

bool				SGameBase::SetGameTimer(UINT nEventID,UINT nElapse)
{
	UINT nRet=::SetTimer(this->m_hWin,nEventID,nElapse,NULL);
	return (bool)nRet;
}

void				SGameBase::KillGameTimer(UINT nEventID)
{
	::KillTimer(this->m_hWin,nEventID);
}


