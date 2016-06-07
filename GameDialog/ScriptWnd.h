#ifndef _INC_SCRIPTWND_
#define _INC_SCRIPTWND_
//////////////////////////////////////////////////////////////////////////
#include "../../CommonModule/LuaBaseEngine.h"
#include "../../CommonModule/LuaBaseExecutor.h"
#include "GameCommonDlg.h"
#include <string>
#include <list>
//////////////////////////////////////////////////////////////////////////
using std::string;
using std::list;
class GameScriptWnd;
//////////////////////////////////////////////////////////////////////////
#define WNDCALLBACK_BUTTONCLICK		1
#define WNDCALLBACK_ONSHOWWINDOW	2
#define WNDCALLBACK_ONCLOSEWINDOW	3

class WndScriptEngine : public LuaBaseEngine, public LuaBaseExecutor
{
public:
	WndScriptEngine();
	virtual ~WndScriptEngine(){}

public:
	virtual void Output(const char* _pszLog);

public:
	void RegisterCallback(int _nCallType, const char* _pszCallFuncName);

	int Call_WndButtonClick(GameScriptWnd* _pWnd, int _nButtonID);
	int Call_OnShowWindow(GameScriptWnd* _pWnd);
	int Call_OnCloseWindow(GameScriptWnd* _pWnd);
};
//////////////////////////////////////////////////////////////////////////
class WndRenderTexture
{
	friend class GameScriptWnd;
public:
	WndRenderTexture()
	{
		m_nID = 0;
		m_tex = 0;
		m_dwColor = 0;
		ZeroMemory(&m_rcTexClient, sizeof(RECT));
		m_nX = m_nY = 0;
		m_nTexIndex = 0;
		m_bVisible = true;
	}
	int GetID() const
	{
		return m_nID;
	}
public:
	virtual void RenderToWnd(GameScriptWnd* _pWnd);

protected:
	HTEXTURE m_tex;
	DWORD m_dwColor;
	RECT m_rcTexClient;
	int m_nX;
	int m_nY;
	int m_nID;
	int m_nTexIndex;
	bool m_bVisible;
};
typedef list<WndRenderTexture*> RenderTextureList;

class WndRenderString : public WndRenderTexture
{
	friend class GameScriptWnd;
public:
	virtual void RenderToWnd(GameScriptWnd* _pWnd);

protected:
	string m_xString;
};
typedef list<WndRenderString*> RenderStringList;
//////////////////////////////////////////////////////////////////////////
struct RenderTextureFinderByID
{  
	RenderTextureFinderByID(int _nID)
	{
		nDestFindID = _nID;
	}
	bool operator () (WndRenderTexture* _pDetected) const  
	{  
		return _pDetected->GetID() == nDestFindID;
	}  

	int nDestFindID;
};  

struct RenderStringFinderByID
{  
	RenderStringFinderByID(int _nID)
	{
		nDestFindID = _nID;
	}
	bool operator () (WndRenderString* _pDetected) const  
	{  
		return _pDetected->GetID() == nDestFindID;
	}  

	int nDestFindID;
};  
//////////////////////////////////////////////////////////////////////////
class GameScriptWnd : public GameNewDlg
{
protected:
	GameScriptWnd();
public:
	static GameScriptWnd* CreateWithFile(const char* _pszFileName);
	static GameScriptWnd* CreateWithZipFile(const char* _pszZipFile, const char* _pszFileName);

public:
	virtual ~GameScriptWnd()
	{
		RemoveAllRenderTexture();
		RemoveAllRenderString();
	}

public:
	virtual void Render();
	virtual bool OnCommonButtonClick(int _id);
	virtual bool OnShowDialog();
	virtual bool OnCloseDialog();

public:
	//	render
	int AddRenderTexture(const char* _pszFileName, int _nTextureID, int _nX, int _nY, int _nLeft = 0, int _nTop = 0, int _nWidth = 0, int _nHeight = 0, unsigned int _uColor = 0);
	int AddRenderString(const char* _pszString, int _nLeft, int _nTop, unsigned int _uColor = 0);

	void RemoveRenderTexture(int _nID);
	void RemoveRenderString(int _nID);

	void SetRenderTexture_Texture(int _nID, const char* _pszFileName, int _nTextureID);
	void SetRenderTexture_Rect(int _nID, int _nLeft, int _nTop, int _nWidth = 0, int _nHeight = 0);
	void SetRenderTexture_Position(int _nID, int _nX, int _nY);
	void SetRenderTexture_Color(int _nID, unsigned int _uColor);

	void SetRenderString_String(int _nID, const char* _pszString);
	void SetRenderString_Position(int _nID, int _nX, int _nY);
	void SetRenderString_Color(int _nID, unsigned int _uColor);

	void SetRenderTexture_Visible(int _nID, bool _bVisible);
	void SetRenderString_Visible(int _nID, bool _bVisible);

	void RemoveAllRenderTexture();
	void RemoveAllRenderString();

	//	command
	void RegisterOnButtonClick(const char* _pszFuncName);
	void RegisterOnShowWindow(const char* _pszFuncName);
	void RegisterOnCloseWindow(const char* _pszFuncName);

public:
	lua_State* GetVM()
	{
		return m_xScript.GetVM();
	}

protected:
	static int GetRenderItemID()
	{
		static int s_nRenderItemID = 0;
		return ++s_nRenderItemID;
	}

	WndRenderTexture* GetRenderTexture(int _nID)
	{
		RenderTextureList::iterator fndIter = std::find_if(m_xRenderTextureList.begin(), m_xRenderTextureList.end(), RenderTextureFinderByID(_nID));

		if(fndIter != m_xRenderTextureList.end())
		{
			return *fndIter;
		}
		return NULL;
	}
	WndRenderString* GetRenderString(int _nID)
	{
		RenderStringList::iterator fndIter = std::find_if(m_xRenderStringList.begin(), m_xRenderStringList.end(), RenderStringFinderByID(_nID));

		if(fndIter != m_xRenderStringList.end())
		{
			return *fndIter;
		}
		return NULL;
	}

protected:
	WndScriptEngine m_xScript;
	RenderTextureList m_xRenderTextureList;
	RenderStringList m_xRenderStringList;
};

//////////////////////////////////////////////////////////////////////////
#endif