#ifndef LOGINSCENE_H_
#define LOGINSCENE_H_

#include "../Common/GameRenderObject.h"
#include "../LoginScene/SelChrScene.h"
#include "../LoginScene/SelChrDlg.h"
#include "../LoginScene/DeleteConfirmDlg.h"
#include "../GameDialog/GameDlgBase.h"

#define PAGE_LOGIN	0
#define PAGE_MADE	1
#define PAGE_LOAD	2
#define PAGE_LOGIN_CONNECT	3
#define PAGE_SELCHR	4
#define PAGE_ABOUT	5
#define PAGE_PREFACE	6
#define PAGE_THANKS		7

struct PacketBase;

/************************************************************************/
/* ÓÎÏ·µÇÂ½³¡¾°
/************************************************************************/
class hgeResourceManager;
class GameTextureManagerSib;

struct FireworksInfo
{
	BYTE bCurFrame;
	BYTE bTotalFrame;
	DWORD dwPos;
	BYTE bType;
	DWORD dwLastUpdateTime;
};

typedef std::list<FireworksInfo*> FireworksRenderList;

class LoginScene : public RenderObject
{
public:
	enum RECT_RIGION
	{
		RECT_SINGLE,
		RECT_NET,
		RECT_SETTING,
		RECT_MADE,
		RECT_QUIT,
		RECT_TOTAL,
	};

public:
	LoginScene();
	~LoginScene();

public:
	bool Init(hgeResourceManager* _pRes);
	void OnMessage(const PacketBase* _pPacket);
	void OnConnect();
	void OnDisconnect();
	void EnterLoadingPage();

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render();
	virtual void Update(float _dt);

public:
	static unsigned __stdcall LoadGameScene(void*);

public:
	void OnButton(RECT_RIGION _rc);
	void OnSceneProg(int _nTotal, int _nCur);
	void RenderBk();
	void OnPressNetButton();

public:
	inline void SetPage(WORD _wPage)	{m_wCurPage = _wPage;}
	inline WORD GetPage()				{return m_wCurPage;}
	inline HANDLE GetLoadThread()		{return m_hThread;}
	SelChrDlg* GetSelChrDlg()		{return m_pSelChrDlg;}
	int LoadHumData();

private:
	void ResetMouseState();
	void RenderLogin();
	void RenderLoad();
	void RenderSelChr();
	void RenderAbout();
	void RenderPreface();
	void RenderThanks();
	bool CommandLogin(const POINT& _ptMouse);
	bool CommandLoad(const POINT& _ptMouse);
	bool CommandSelChr(const POINT& _ptMouse);
	bool CommandAbout(const POINT& _ptMouse);
	bool CommandPreface(const POINT& _ptMouse);
	bool CommandThanks(const POINT& _ptMouse);

private:
	hgeResourceManager* m_pxResMgr;
	RECT m_stRect[RECT_TOTAL];
	HANDLE m_hThread;

	WORD m_wTotalProg;
	WORD m_wCurProg;

	WORD m_wCurPage;

	WORD m_wCurOver;
	WORD m_wCurDown;

	SelChrScene* m_pxSelChrScene;
	bool m_bIsConnectiong;

	HeroHeader m_stHero[3];
	int m_nHumCount;

	SelChrDlg* m_pSelChrDlg;

	GameTextureManagerSib* m_pTexMgr;
	hgeSprite* m_pEffRender;
	FireworksRenderList m_xFireworks;

public:
	char* m_szIP;
	char* m_szHero;
	WORD m_wPort;
};


#endif