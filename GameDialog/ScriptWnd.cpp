#include "ScriptWnd.h"
#include "../GameScene/GameResourceManager.h"
//////////////////////////////////////////////////////////////////////////
TOLUA_API int  tolua_BackMirClient_open (lua_State* tolua_S);
//////////////////////////////////////////////////////////////////////////
WndScriptEngine::WndScriptEngine()
{

}

void WndScriptEngine::RegisterCallback(int _nCallType, const char* _pszCallFuncName)
{
	AddCallItem(_nCallType, _pszCallFuncName, 0);
}

int WndScriptEngine::Call_WndButtonClick(GameScriptWnd* _pWnd, int _nButtonID)
{
	LuaBaseCallList::const_iterator begIter = m_xBaseCallList.begin();
	LuaBaseCallList::const_iterator endIter = m_xBaseCallList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		LuaBaseCallItem* pItem = *begIter;

		if(pItem->nCallType == WNDCALLBACK_BUTTONCLICK)
		{
			lua_State* pState = GetVM();

			lua_getglobal(pState, pItem->xCallFuncName.c_str());
			tolua_pushusertype(pState, _pWnd, "GameScriptWnd");
			lua_pushnumber(pState, _nButtonID);

			int nRet = lua_pcall(pState, 2, 0, 0);
			if(nRet != 0)
			{
				Log("[%s] Can't call WndButtonClick.Error[%s]",
					__FUNCTION__, lua_tostring(pState, -1));
				lua_pop(pState, 1);
			}

			pItem->dwLastCallTime = GetTickCount();
		}
	}

	return 0;
}

int WndScriptEngine::Call_OnShowWindow(GameScriptWnd* _pWnd)
{
	LuaBaseCallList::const_iterator begIter = m_xBaseCallList.begin();
	LuaBaseCallList::const_iterator endIter = m_xBaseCallList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		LuaBaseCallItem* pItem = *begIter;

		if(pItem->nCallType == WNDCALLBACK_BUTTONCLICK)
		{
			lua_State* pState = GetVM();

			lua_getglobal(pState, pItem->xCallFuncName.c_str());
			tolua_pushusertype(pState, _pWnd, "GameScriptWnd");

			int nRet = lua_pcall(pState, 1, 0, 0);
			if(nRet != 0)
			{
				Log("[%s] Can't call OnShowWindow.Error[%s]",
					__FUNCTION__, lua_tostring(pState, -1));
				lua_pop(pState, 1);
			}

			pItem->dwLastCallTime = GetTickCount();
		}
	}

	return 0;
}

int WndScriptEngine::Call_OnCloseWindow(GameScriptWnd* _pWnd)
{
	LuaBaseCallList::const_iterator begIter = m_xBaseCallList.begin();
	LuaBaseCallList::const_iterator endIter = m_xBaseCallList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		LuaBaseCallItem* pItem = *begIter;

		if(pItem->nCallType == WNDCALLBACK_BUTTONCLICK)
		{
			lua_State* pState = GetVM();

			lua_getglobal(pState, pItem->xCallFuncName.c_str());
			tolua_pushusertype(pState, _pWnd, "GameScriptWnd");

			int nRet = lua_pcall(pState, 1, 0, 0);
			if(nRet != 0)
			{
				Log("[%s] Can't call OnShowWindow.Error[%s]",
					__FUNCTION__, lua_tostring(pState, -1));
				lua_pop(pState, 1);
			}

			pItem->dwLastCallTime = GetTickCount();
		}
	}

	return 0;
}

void WndScriptEngine::Output(const char* _pszLog)
{
	AfxGetHge()->System_Log(_pszLog);
}

//////////////////////////////////////////////////////////////////////////
void WndRenderTexture::RenderToWnd(GameScriptWnd* _pWnd)
{
	if(NULL == m_tex)
	{
		return;
	}
	if(!m_bVisible)
	{
		return;
	}

	hgeSprite* pRender = _pWnd->GetRender();
	DWORD dwPrevColor = pRender->GetColor();
	bool bChangedColor = false;

	int nRenderX = m_nX + _pWnd->GetRect().left;
	int nRenderY = m_nY + _pWnd->GetRect().top;

	pRender->SetTexture(m_tex);
	pRender->SetTextureRect(m_rcTexClient.left, m_rcTexClient.top, m_rcTexClient.right, m_rcTexClient.bottom);
	if(m_dwColor != 0)
	{
		pRender->SetColor(m_dwColor);
		bChangedColor = true;
	}
	pRender->Render(nRenderX, nRenderY);

	if(bChangedColor)
	{
		pRender->SetColor(dwPrevColor);
	}
}

//////////////////////////////////////////////////////////////////////////
void WndRenderString::RenderToWnd(GameScriptWnd* _pWnd)
{
	if(m_xString.empty())
	{
		return;
	}
	if(!m_bVisible)
	{
		return;
	}

	hgeSprite* pRender = _pWnd->GetRender();
	DWORD dwPrevColor = pRender->GetColor();
	bool bChangedColor = false;

	int nRenderX = m_nX + _pWnd->GetRect().left;
	int nRenderY = m_nY + _pWnd->GetRect().top;

	if(m_dwColor != 0)
	{
		AfxGetPrinter()->SetColor(m_dwColor);
		bChangedColor = true;
	}

	AfxGetPrinter()->Print(nRenderX, nRenderY, m_xString.c_str());

	if(bChangedColor)
	{
		AfxGetPrinter()->SetColor(dwPrevColor);
	}
}
//////////////////////////////////////////////////////////////////////////
GameScriptWnd::GameScriptWnd()
{

}

GameScriptWnd* GameScriptWnd::CreateWithFile(const char* _pszFileName)
{
	GameScriptWnd* pWnd = new GameScriptWnd;

	if(!pWnd->m_xScript.Create())
	{
		SAFE_DELETE(pWnd);
		return NULL;
	}

	tolua_BackMirClient_open(pWnd->m_xScript.GetVM());

	if(!pWnd->m_xScript.LoadFile(_pszFileName))
	{
		SAFE_DELETE(pWnd);
		return NULL;
	}

	lua_State* pState = pWnd->GetVM();

	lua_getglobal(pState, "OnWndCreate");
	tolua_pushusertype(pState, pWnd, "GameScriptWnd");

	int nRet = lua_pcall(pState, 1, 0, 0);
	if(nRet != 0)
	{
		pWnd->m_xScript.Log("[%s] Can't create(%s) with file .Error[%s]",
			__FUNCTION__, _pszFileName, lua_tostring(pState, -1));
		lua_pop(pState, 1);

		SAFE_DELETE(pWnd);
		return NULL;
	}

	pWnd->m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	pWnd->CalcCloseButtonPosition();

	return pWnd;
}

GameScriptWnd* GameScriptWnd::CreateWithZipFile(const char* _pszZipFile, const char* _pszFileName)
{
	GameScriptWnd* pWnd = new GameScriptWnd;

	if(!pWnd->m_xScript.Create())
	{
		SAFE_DELETE(pWnd);
		return NULL;
	}

	tolua_BackMirClient_open(pWnd->m_xScript.GetVM());

	if(!pWnd->m_xScript.LoadFileInZip(_pszZipFile, _pszFileName, NULL))
	{
		SAFE_DELETE(pWnd);
		return NULL;
	}

	lua_State* pState = pWnd->GetVM();

	lua_getglobal(pState, "OnWndCreate");
	tolua_pushusertype(pState, pWnd, "GameScriptWnd");

	int nRet = lua_pcall(pState, 1, 0, 0);
	if(nRet != 0)
	{
		pWnd->m_xScript.Log("[%s] Can't create(%s) with file .Error[%s]",
			__FUNCTION__, _pszFileName, lua_tostring(pState, -1));
		lua_pop(pState, 1);

		SAFE_DELETE(pWnd);
		return NULL;
	}

	pWnd->m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	pWnd->CalcCloseButtonPosition();

	return pWnd;
}

void GameScriptWnd::RemoveRenderString(int _nID)
{
	RenderStringList::const_iterator begIter = m_xRenderStringList.begin();
	RenderStringList::const_iterator endIter = m_xRenderStringList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		WndRenderString* pItem = *begIter;
		if(pItem->GetID() == _nID)
		{
			SAFE_DELETE(pItem);
			m_xRenderStringList.erase(begIter);
			return;
		}
	}
}

void GameScriptWnd::RemoveRenderTexture(int _nID)
{
	RenderTextureList::const_iterator begIter = m_xRenderTextureList.begin();
	RenderTextureList::const_iterator endIter = m_xRenderTextureList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		WndRenderTexture* pItem = *begIter;
		if(pItem->GetID() == _nID)
		{
			SAFE_DELETE(pItem);
			m_xRenderTextureList.erase(begIter);
			return;
		}
	}
}

void GameScriptWnd::RemoveAllRenderString()
{
	RenderStringList::const_iterator begIter = m_xRenderStringList.begin();
	RenderStringList::const_iterator endIter = m_xRenderStringList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		WndRenderString* pItem = *begIter;
		SAFE_DELETE(pItem);
	}

	m_xRenderStringList.clear();
}

void GameScriptWnd::RemoveAllRenderTexture()
{
	RenderTextureList::const_iterator begIter = m_xRenderTextureList.begin();
	RenderTextureList::const_iterator endIter = m_xRenderTextureList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		WndRenderTexture* pItem = *begIter;
		SAFE_DELETE(pItem);
	}

	m_xRenderTextureList.clear();
}

void GameScriptWnd::Render()
{
	__super::Render();

	{
		RenderStringList::const_iterator begIter = m_xRenderStringList.begin();
		RenderStringList::const_iterator endIter = m_xRenderStringList.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			WndRenderString* pItem = *begIter;
			pItem->RenderToWnd(this);
		}
	}
	
	{
		RenderTextureList::const_iterator begIter = m_xRenderTextureList.begin();
		RenderTextureList::const_iterator endIter = m_xRenderTextureList.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			WndRenderTexture* pItem = *begIter;
			pItem->RenderToWnd(this);
		}
	}
}

int GameScriptWnd::AddRenderTexture(const char* _pszFileName, int _nTextureID, int _nX, int _nY, int _nLeft /* = 0 */, int _nTop /* = 0 */, int _nWidth /* = 0 */, int _nHeight /* = 0 */, unsigned int _uColor /* = 0 */)
{
	int nResIndex = GameResourceManager::GetInstance()->GetResourceIndex(_pszFileName);
	if(-1 == nResIndex)
	{
		return 0;
	}

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(nResIndex);
	if(NULL == pTexMgr)
	{
		return 0;
	}

	HTEXTURE tex = pTexMgr->GetTexture(_nTextureID);
	if(0 == tex)
	{
		return 0;
	}

	WndRenderTexture* pRenderTexture = new WndRenderTexture;
	pRenderTexture->m_dwColor = _uColor;
	pRenderTexture->m_tex = tex;
	pRenderTexture->m_nX = _nX;
	pRenderTexture->m_nY = _nY;
	pRenderTexture->m_rcTexClient.left = _nLeft;
	pRenderTexture->m_rcTexClient.top = _nTop;
	pRenderTexture->m_rcTexClient.right = _nWidth;
	pRenderTexture->m_rcTexClient.bottom = _nHeight;
	pRenderTexture->m_nTexIndex = _nTextureID;
	pRenderTexture->m_nID = GetRenderItemID();

	if(0 == _nWidth ||
		0 == _nHeight)
	{
		pRenderTexture->m_rcTexClient.right = pTexMgr->GetTextureWidth(_nTextureID);
		pRenderTexture->m_rcTexClient.bottom = pTexMgr->GetTextureHeight(_nTextureID);
	}

	m_xRenderTextureList.push_back(pRenderTexture);
	return pRenderTexture->m_nID;
}

int GameScriptWnd::AddRenderString(const char* _pszString, int _nLeft, int _nTop, unsigned int _uColor /* = 0 */)
{
	if(NULL == _pszString ||
		0 == strlen(_pszString))
	{
		return 0;
	}

	WndRenderString* pRenderString = new WndRenderString;
	pRenderString->m_xString = _pszString;
	pRenderString->m_nX = _nLeft;
	pRenderString->m_nY = _nTop;
	pRenderString->m_dwColor = _uColor;
	pRenderString->m_nID = GetRenderItemID();

	m_xRenderStringList.push_back(pRenderString);
	return pRenderString->m_nID;
}

void GameScriptWnd::SetRenderTexture_Color(int _nID, unsigned int _uColor)
{
	WndRenderTexture* pRenderTexture = GetRenderTexture(_nID);

	if(NULL != pRenderTexture)
	{
		pRenderTexture->m_dwColor = _uColor;
	}
}

void GameScriptWnd::SetRenderTexture_Position(int _nID, int _nX, int _nY)
{
	WndRenderTexture* pRenderTexture = GetRenderTexture(_nID);

	if(NULL != pRenderTexture)
	{
		pRenderTexture->m_nX = _nX;
		pRenderTexture->m_nY = _nY;
	}
}

void GameScriptWnd::SetRenderTexture_Texture(int _nID, const char* _pszFileName, int _nTextureID)
{
	WndRenderTexture* pRenderTexture = GetRenderTexture(_nID);

	if(NULL != pRenderTexture)
	{
		int nResIndex = GameResourceManager::GetInstance()->GetResourceIndex(_pszFileName);
		if(-1 == nResIndex)
		{
			return;
		}

		GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(nResIndex);
		if(NULL == pTexMgr)
		{
			return;
		}

		HTEXTURE tex = pTexMgr->GetTexture(_nTextureID);
		if(0 == tex)
		{
			return;
		}

		pRenderTexture->m_tex = tex;
	}
}

void GameScriptWnd::SetRenderTexture_Rect(int _nID, int _nLeft, int _nTop, int _nWidth /* = 0 */, int _nHeight /* = 0 */)
{
	WndRenderTexture* pRenderTexture = GetRenderTexture(_nID);

	if(NULL != pRenderTexture)
	{
		pRenderTexture->m_rcTexClient.left = _nLeft;
		pRenderTexture->m_rcTexClient.top = _nTop;
		pRenderTexture->m_rcTexClient.right = _nWidth;
		pRenderTexture->m_rcTexClient.bottom = _nHeight;
	}
}

void GameScriptWnd::SetRenderString_Color(int _nID, unsigned int _uColor)
{
	WndRenderString* pRenderString = GetRenderString(_nID);

	if(pRenderString)
	{
		pRenderString->m_dwColor = _uColor;
	}
}

void GameScriptWnd::SetRenderString_Position(int _nID, int _nX, int _nY)
{
	WndRenderString* pRenderString = GetRenderString(_nID);

	if(pRenderString)
	{
		pRenderString->m_nX = _nX;
		pRenderString->m_nY = _nY;
	}
}

void GameScriptWnd::SetRenderString_String(int _nID, const char* _pszString)
{
	if(NULL == _pszString)
	{
		return;
	}

	WndRenderString* pRenderString = GetRenderString(_nID);

	if(pRenderString)
	{
		pRenderString->m_xString = _pszString;
	}
}

void GameScriptWnd::SetRenderTexture_Visible(int _nID, bool _bVisible)
{
	WndRenderTexture* pRenderTexture = GetRenderTexture(_nID);

	if(NULL != pRenderTexture)
	{
		pRenderTexture->m_bVisible = _bVisible;
	}
}

void GameScriptWnd::SetRenderString_Visible(int _nID, bool _bVisible)
{
	WndRenderString* pRenderString = GetRenderString(_nID);

	if(pRenderString)
	{
		pRenderString->m_bVisible = _bVisible;
	}
}

void GameScriptWnd::RegisterOnButtonClick(const char* _pszFuncName)
{
	m_xScript.AddCallItem(WNDCALLBACK_BUTTONCLICK, _pszFuncName, 0);
}

void GameScriptWnd::RegisterOnShowWindow(const char* _pszFuncName)
{
	m_xScript.AddCallItem(WNDCALLBACK_ONSHOWWINDOW, _pszFuncName, 0);
}

void GameScriptWnd::RegisterOnCloseWindow(const char* _pszFuncName)
{
	m_xScript.AddCallItem(WNDCALLBACK_ONCLOSEWINDOW, _pszFuncName, 0);
}

bool GameScriptWnd::OnCommonButtonClick(int _id)
{
	m_xScript.Call_WndButtonClick(this, _id);

	return __super::OnCommonButtonClick(_id);
}

bool GameScriptWnd::OnShowDialog()
{
	m_xScript.Call_OnShowWindow(this);

	return true;
}

bool GameScriptWnd::OnCloseDialog()
{
	m_xScript.Call_OnCloseWindow(this);

	return true;
}