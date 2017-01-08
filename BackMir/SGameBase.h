#ifndef _SGAMEBASE_H
#define _SGAMEBASE_H

#include <hge.h>
#include <hgegui.h>  
#include <hgeSprite.h>   
#include <hgefont.h>  
#include <Windows.h>

//////////////////////////////////////////////////////   
//////////////////////////////////////////////////////   
//  
// 
//
//
//	SGameBase Ver 1.1
//				2012.3.9
//
//
//
//
//////////////////////////////////////////////////////  
//////////////////////////////////////////////////////  


//宏   
#define S_DEBUG\
	{MessageBoxA(NULL,\
	m_pHGE->System_GetErrorMessage(),\
	"HGE Error Code",\
	MB_OK|MB_ICONERROR);abort();}  
#define S_DEBUG_(M)\
	{char buf[200];\
	sprintf(buf,"Line:%d    Function:%s   Run;",__LINE__,__FUNCTION__);\
	MessageBoxA(NULL,\
	M,\
	buf,\
	MB_OK);abort();}     


#define S_INITIAL_TEST(O)\
	if(O==NULL)\
	{MessageBoxA(NULL,\
	m_pHGE->System_GetErrorMessage(),\
	"HGE Error Code",\
	MB_OK|MB_ICONERROR);return false;}   

#define S_MSG(msg)\
	MessageBoxA(NULL,\
	msg,"SPTGAMEBASE MESSAGE",\
	MB_OK|MB_ICONINFORMATION);

////////////////////////////////  


class SGameBase  
{  
	//////////////////////////////////////////////////////  
	//  
	//  需要(可以)重新实现的函数
	//  
	///////////////////////////////////////////////////////  
protected:
	//
	//	事件过滤函数
	//
	virtual LRESULT                 WinEventFilter(HWND h, UINT u, WPARAM w, LPARAM l);  
	//
	//	获得焦点 失去焦点 保存图像
	//
	virtual bool                    RealFocusGainFunc();  
	virtual bool                    RealFocusLostFunc();  
	virtual bool                    RealGfxRestoreFunc();  
	//
	//	载入用户游戏资源
	//
	virtual bool                    UserInitial();  
	//
	//	释放用户游戏资源
	//
	virtual void                    UserUninitial();  
	//
	//	用户图像计算函数
	//
	virtual void                    UserFrameFunc();  
	//
	//	用户绘制函数
	//
	virtual void                    UserRenderFunc(); 
	virtual bool                    UserProcCommand(); 

public:  
	SGameBase(const char* szTitle=NULL,int nViewWidth=800,int nViewHeight=600);  
	virtual ~SGameBase();  

public:  
	//
	//	静态函数用来调用实例的成员函数  由系统自动调用
	//
	static bool                     FrameFunc();  
	static bool                     RenderFunc();  
	static bool                     FocusGainFunc();  
	static bool                     FocusLostFunc();  
	static bool                     GfxRestoreFrameFunc();  
	//
	//窗口过程  
	//
	static LRESULT                  MainWinProc(HWND h, UINT u, WPARAM w, LPARAM l);  


public:  
	//
	//外部接口
	//
	bool						    SetGameCursor(const char* szCursorPath);  
	bool							SetGameCursor(HTEXTURE _texCursor);
	bool						    SetGameTitle(const char* szTitle);  
	void							SetGameFps(int nFps);
	void							SetGameLogFile(const char* szFile);
	bool							SetGameTimer(UINT nEventID,UINT nElapse);

	void							AddLog(const char* szLog);
	void							KillGameTimer(UINT nEventID);

	char*                           GetGameTitle(){return m_szGameTitle;}  
	const char*                     GetGameTitle()const{return m_szGameTitle;}  
	RECT&							GetGameViewRectNew();
	RECT&							GetGameViewRect();
	int								GetGameViewWidth(){return m_nViewWidth;}
	int								GetGameViewHeight(){return m_nViewHeight;}

	bool							Init();
	void				            EnableSound(bool);  
	void                            Exec();  

	virtual bool					IsWindowMode()	{return true;}

protected:  
	//
	//获取实例指针  
	//
	static SGameBase *              InitialInstance(SGameBase * p=NULL);  
	//
	//	获取实例窗口过程  
	//
	static WNDPROC                  InitialProc(WNDPROC p=NULL);  

private:  
	bool                            HgeInitial(const char* szTitle);  
	void                            HgeUninitial();  

protected:   
	//
	//	真实的处理函数	一般不需要重新实现	在UserFrameFunc UserRenderFunc中处理	在ProcCommand中处理GUI消息
	//
	virtual bool                    RealFrameFunc();  
	virtual bool                    RealRenderFunc();   

	//
	//	GUI操作处理函数  
	//
	virtual void                    UpdateCommand();  
	//
	//	辅助函数
	//
	virtual bool					IsCaptionBar(int x,int y);
	virtual void					OnGameTimer(UINT nEventID);

protected:  
	HGE *                           m_pHGE;  
	hgeGUI *                        m_pGUI;  
	int                             m_nCommandID;  
	HWND							m_hWin;

protected:  
	HTEXTURE                        m_hCursor;  
	char                            m_szGameTitle[50];  
	hgeSprite*                      m_pSprCursor;  
	bool                            m_bHgeInitial;  
	float                           m_fTimeDelta;  
	int								m_nViewWidth;
	int								m_nViewHeight;
	RECT							m_rcView;
};  


#endif