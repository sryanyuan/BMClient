#ifndef _INC_BMPRECONNWND_
#define _INC_BMPRECONNWND_
//////////////////////////////////////////////////////////////////////////
#include <uilib.h>
#include "../../CommonModule/DataParser.h"
#include "../../CommonModule/GamePacket.h"
#ifdef _DEBUG
	#pragma comment(lib, "duilib_d.lib")
#else
	#pragma comment(lib, "duilib.lib")
#endif

using namespace DuiLib;
//////////////////////////////////////////////////////////////////////////
extern bool g_bPrepared;
extern GAME_MODE g_eGameMode;
extern HeroHeader g_stHeroHeader[3];
//////////////////////////////////////////////////////////////////////////
#define DO_GAMESVR_PACKET(op, pkg) case op:{pkg p;g_PreConnWnd->m_xBuffer >> p;g_PreConnWnd->DoGameSvrPacket(p);}break;
#define DO_LOGINSVR_PACKET(op, pkg) case op:{pkg p;g_PreConnWnd->m_xBuffer >> p;g_PreConnWnd->DoLoginSvrPacket(p);}break;
#define DO_GLOBAL_PACKET(op, pkg) case op:{pkg p;g_PreConnWnd->m_xBuffer >> p;g_PreConnWnd->DoPacket(p);}break;
//////////////////////////////////////////////////////////////////////////
class BMPreConnWnd : public CWindowWnd, public INotifyUI
{
public:
	BMPreConnWnd();
	virtual ~BMPreConnWnd();

public:
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/)
	{
		delete this;
	};
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);

public:
	void OnTimer(WPARAM _wTimerID);
	void OnPrepare();
	void OnSocketMsg(SOCKET _s, LPARAM _l);
	void OnSocketGameSvr(SOCKET _s, LPARAM _l);
	void OnSocketLoginSvr(SOCKET _s, LPARAM _l);
	void AppendString(const char* _pszStr);
	void PrepareRunGame();
	void RunGame();
	void PrepareCloseGame();
	void PrepareLoadData();
	void LoadData();

public:
	void DoGameSvrPacket(const PkgLoginGameTypeNot& not);
	void DoPacket(const PkgLoginGameTypeNot& not);

	void DoLoginSvrPacket(const PkgLoginGameTypeNot& not);
	void DoLoginSvrPacket(const PkgLoginVerifyResultAck& ack);
	void DoLoginSvrPacket(const PkgLoginQuickMsgNot& not);
	void DoLoginSvrPacket(const PkgLoginServerAddrNot& not);
	void DoLoginSvr_HeadData(const char* _pData, unsigned int _len);

public:
	static void __stdcall OnFullMsgGameSvr(const void* _pData, unsigned int _len);
	static void __stdcall OnFullMsgLoginSvr(const void* _pData, unsigned int _len);
	static void __stdcall OnFullMsgGlobal(const void* _pData, unsigned int _len);

protected:
	CPaintManagerUI m_pm;
	DataParser m_xParserGameSvr;
	DataParser m_xParserLoginSvr;
	DataParser m_xParserGlobal;
	ByteBuffer m_xBuffer;
	ByteBuffer m_xSndBuf;
	DWORD m_dwLSIdx;
	std::string m_xPsw;
	std::string m_xAct;
};
//////////////////////////////////////////////////////////////////////////
extern BMPreConnWnd* g_PreConnWnd;
//////////////////////////////////////////////////////////////////////////
#endif