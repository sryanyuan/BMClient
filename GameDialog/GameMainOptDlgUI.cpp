#include "../GameDialog/GameDlgBase.h"
#include "../GameScene/GamePlayer.h"
#include "../Common/GlobalFunction.h"
#include "../GameScene/GameResourceManager.h"
#include <assert.h>
#include "../GameDialog/GameInteractiveDlg.h"
#include "../Common/ScriptCommand.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameScene/GameMapManager.h"
#include "../GameDialog/GameChatDlg.h"
#include "../BackMir/BackMir.h"
#include "../GameDialog/GameSkillDlg.h"
#include "../GameDialog/GameHelpDlg.h"
#include "../GameDialog/HumStateDlg.h"
#include "../GameDialog/GameQuitDialog.h"
#include "../GameDialog/GameAssistDlg.h"
#include "../GameDialog/GameTeamDlg.h"
#include "../GameDialog/GameDecomposeDlg.h"
#include "../GameDialog/GameQuestDlg.h"
#include "../GameDialog/GameSettingDlg.h"
#include "../GameDialog/GameShopDlg2.h"
#include "../GameDialog/GameStoreDlg2.h"
#include "../GameDialog/GameOtherHumDlg.h"
#include "../GameDialog/ScriptWnd.h"
#include "../GameDialog/GameAttribDlg.h"
#include "../GameDialog/GameRoleInfoDlg.h"
#include "../GameDialog/GameInputDlg.h"
#include "../GameDialog/GameDonateDlg.h"
#include "../GameDialog/OfflineSellDlg.h"
#include "../GameDialog/GameDisplayDlg.h"
#include "../GameDialog/GameIdentifyDlg.h"
#include "../GameDialog/GameRankDlg.h"
#include "../GameDialog/OlShopDlg.h"
#include "../GameDialog/GameStoveDlg.h"
#include "../GameDialog/GameChargeDlg.h"
#include "../GameDialog/GameBigStoreDlg.h"
#include "../TinyXML/tinyxml.h"
#include "../Common/gfx_utils.h"
//////////////////////////////////////////////////////////////////////////

GameMainOptUI::GameMainOptUI()
{
	//m_pFont = NULL;
	m_pSpr = NULL;
	m_pHumDlg = NULL;
	m_pBagDlg = NULL;
	m_pIntroDlg = NULL;
	m_pActiveDlg = NULL;
	m_pSellDlg = NULL;
	m_pChatDlg = NULL;
	m_pShopDlg = NULL;
	m_pMapDlg = NULL;
	m_pStoreDlg = NULL;
	m_pHumDlg2 = NULL;
	m_pHelpDlg = NULL;
	m_pBagDlg2 = NULL;
	m_pCubeDlg = NULL;
	m_pHumStateDlg = NULL;
	m_pQuitDlg = NULL;
	m_pAssistDlg = NULL;
	m_pQuestDlg = NULL;
	m_pTeamDlg = NULL;
	m_pSettingDlg = NULL;
	m_pShopDlg2 = NULL;
	m_pStoreDlg2 = NULL;
	m_pOtherHumDlg = NULL;
	m_pAttribDlg = NULL;
	m_pTeamMenu = NULL;
	m_pRoleInfoDlg = NULL;
	m_pInputDlg = NULL;
	m_pOfflineSellListDlg = NULL;
	m_pDisplayDlg = NULL;
	m_pIdentifyDlg = NULL;
	m_pRankDlg = NULL;
	m_pOlShopDlg = NULL;
	m_pStoveDlg = NULL;
	m_pChargeDlg = NULL;
	m_pBigStoreDlg = NULL;

	m_rcClient.left = 0;
	m_rcClient.right = m_rcClient.left + VIEW_WIDTH;
	m_rcClient.top = VIEW_HEIGHT + VIEW_OPERATE - 58;
	m_rcClient.bottom = VIEW_HEIGHT + VIEW_OPERATE;

	m_bVisible = true;

	ZeroMemory(m_szMapName, sizeof(m_szMapName));
	CancelSel();

	ZeroMemory(m_rcOtherRect, sizeof(m_rcOtherRect));
	ZeroMemory(m_bBtnState, sizeof(m_bBtnState));

	m_testY = 500;
	m_testX = 600;
	m_bSysMenuBtnState = 0;

	//	RECT
	/*static int nOffsetXTable[] = {256, 294, 330, 367, 405, 441, 478, 515, 551, 589, 627};
	for(int i = RECT_ACC1; i <= RECT_BTNSKILL; ++i)
	{
		m_rcOtherRect[i].left = nOffsetXTable[i - RECT_ACC1];
		m_rcOtherRect[i].right = m_rcOtherRect[i].left + 36;
		m_rcOtherRect[i].top = VIEW_HEIGHT + VIEW_OPERATE - 48;
		m_rcOtherRect[i].bottom = m_rcOtherRect[i].top + 38;
	}*/
	/*static int nOffsetXTable[] = {256, 294, 330, 367, 405, 441, 478, 515, 551, 589, 627, ALIGN_HANDMADEBTN_LEFT};
	for(int i = RECT_ACC1; i <= RECT_BTNHANDMADE; ++i)
	{
		m_rcOtherRect[i].left = nOffsetXTable[i - RECT_ACC1];
		m_rcOtherRect[i].right = m_rcOtherRect[i].left + 36;
		m_rcOtherRect[i].top = VIEW_HEIGHT + VIEW_OPERATE - 48;
		m_rcOtherRect[i].bottom = m_rcOtherRect[i].top + 38;
	}*/

	//	hum
	m_rcOtherRect[RECT_BTNHUM].left = ALIGN_HUMBTN_LEFT;
	m_rcOtherRect[RECT_BTNHUM].top = ALIGN_HUMBTN_TOP;
	m_rcOtherRect[RECT_BTNHUM].right = m_rcOtherRect[RECT_BTNHUM].left + 20;
	m_rcOtherRect[RECT_BTNHUM].bottom = m_rcOtherRect[RECT_BTNHUM].top + 20;
	//	bag
	m_rcOtherRect[RECT_BTNBAG].left = ALIGN_BAGBTN_LEFT;
	m_rcOtherRect[RECT_BTNBAG].top = ALIGN_BAGBTN_TOP;
	m_rcOtherRect[RECT_BTNBAG].right = m_rcOtherRect[RECT_BTNBAG].left + 20;
	m_rcOtherRect[RECT_BTNBAG].bottom = m_rcOtherRect[RECT_BTNBAG].top + 20;
	//	skill
	m_rcOtherRect[RECT_BTNSKILL].left = ALIGN_SKILLBTN_LEFT;
	m_rcOtherRect[RECT_BTNSKILL].top = ALIGN_SKILLBTN_TOP;
	m_rcOtherRect[RECT_BTNSKILL].right = m_rcOtherRect[RECT_BTNSKILL].left + 20;
	m_rcOtherRect[RECT_BTNSKILL].bottom = m_rcOtherRect[RECT_BTNSKILL].top + 20;
	//	quest
	m_rcOtherRect[RECT_BTNTASK].left = ALIGN_QUESTBTN_LEFT;
	m_rcOtherRect[RECT_BTNTASK].top = ALIGN_QUESTBTN_TOP;
	m_rcOtherRect[RECT_BTNTASK].right = m_rcOtherRect[RECT_BTNTASK].left + 20;
	m_rcOtherRect[RECT_BTNTASK].bottom = m_rcOtherRect[RECT_BTNTASK].top + 20;
	//	hand-made
	m_rcOtherRect[RECT_BTNHANDMADE].left = ALIGN_HANDMADEBTN_LEFT;
	m_rcOtherRect[RECT_BTNHANDMADE].top = ALIGN_HANDMADEBTN_TOP;
	m_rcOtherRect[RECT_BTNHANDMADE].right = m_rcOtherRect[RECT_BTNHANDMADE].left + 20;
	m_rcOtherRect[RECT_BTNHANDMADE].bottom = m_rcOtherRect[RECT_BTNHANDMADE].top + 20;

	//	SysMenu Rect
	m_rcOtherRect[RECT_SYSMENU].left = ALIGN_SYSMENUBTN_LEFT;
	m_rcOtherRect[RECT_SYSMENU].top = ALIGN_SYSMENUBTN_TOP;
	m_rcOtherRect[RECT_SYSMENU].right = m_rcOtherRect[RECT_SYSMENU].left + 20;
	m_rcOtherRect[RECT_SYSMENU].bottom = m_rcOtherRect[RECT_SYSMENU].top + 20;

	//	offsell rect
	m_rcOtherRect[RECT_OFFSELL].left = 696;
	m_rcOtherRect[RECT_OFFSELL].top = 484;
	m_rcOtherRect[RECT_OFFSELL].right = m_rcOtherRect[RECT_OFFSELL].left + 40;
	m_rcOtherRect[RECT_OFFSELL].bottom = m_rcOtherRect[RECT_OFFSELL].top + 38;

	//	olshop rect
	m_rcOtherRect[RECT_OLSHOP].left = 744;
	m_rcOtherRect[RECT_OLSHOP].top = 484;
	m_rcOtherRect[RECT_OLSHOP].right = m_rcOtherRect[RECT_OLSHOP].left + 40;
	m_rcOtherRect[RECT_OLSHOP].bottom = m_rcOtherRect[RECT_OLSHOP].top + 38;

	m_pRender = NEW_EMPTY_SPRITE;
}

GameMainOptUI::~GameMainOptUI()
{
	delete m_pSpr;
	delete m_pChatDlg;
	delete m_pHumDlg;
	delete m_pBagDlg;
	delete m_pIntroDlg;
	delete m_pActiveDlg;
	delete m_pSellDlg;
	delete m_pStaticDlg;
	delete m_pShopDlg;
	delete m_pMapDlg;
	delete m_pStoreDlg;
	delete m_pBagDlg2;
	delete m_pHelpDlg;
	delete m_pCubeDlg;
	delete m_pHumStateDlg;
	delete m_pQuitDlg;
	delete m_pAssistDlg;
	delete m_pTeamDlg;
	delete m_pDecomposeDlg;
	delete m_pQuestDlg;
	delete m_pSettingDlg;
	SAFE_DELETE(m_pShopDlg2);
	SAFE_DELETE(m_pStoreDlg2);
	SAFE_DELETE(m_pOtherHumDlg);
	SAFE_DELETE(m_pAttribDlg);
	SAFE_DELETE(m_pRoleInfoDlg);
	SAFE_DELETE(m_pTeamMenu);
	SAFE_DELETE(m_pInputDlg);
	SAFE_DELETE(m_pDonateDlg);
	SAFE_DELETE(m_pOfflineSellListDlg);
	SAFE_DELETE(m_pDisplayDlg);
	SAFE_DELETE(m_pIdentifyDlg);
	SAFE_DELETE(m_pRankDlg);
	SAFE_DELETE(m_pOlShopDlg);
	SAFE_DELETE(m_pStoveDlg);
	SAFE_DELETE(m_pChargeDlg);
	SAFE_DELETE(m_pBigStoreDlg);
	DeleteScriptWnd();
}


bool GameMainOptUI::Init()
{
	//m_pResManager = new hgeResourceManager("GameRes/game.dat");
	m_pResManager = pTheGame->GetGameRes();
	m_pPlayer = GamePlayer::GetInstance();
	//m_pFont = new GfxFont("宋体", 12, 0, 0, 0);
	//m_pFont->SetKerningHeight(2.0f);
	m_pHumStateDlg = new HumStateDlg;
	m_dlgControl.InsertObject(m_pHumStateDlg);
	m_pChatDlg = new GameChatDlg;
	m_dlgControl.InsertObject(m_pChatDlg);
	m_pIntroDlg = new GameIntroDlg;
	m_pIntroDlg->Init(m_pResManager);
	//m_dlgControl.InsertObject(m_pIntroDlg);
	m_pHumDlg = new GameHumDlg;
	m_pHumDlg->Init(m_pResManager, this);
	m_dlgControl.InsertObject(m_pHumDlg);
	m_pBagDlg = new GameBagDlg;
	m_pBagDlg->Init(m_pResManager, this);
	m_dlgControl.InsertObject(m_pBagDlg);
	m_pActiveDlg = new GameInteractiveDlg;
	m_pActiveDlg->Init(m_pResManager, this);
	m_dlgControl.InsertObject(m_pActiveDlg);
	m_pSellDlg = new GameSellDlg;
	m_pSellDlg->Init(m_pResManager, this);
	m_dlgControl.InsertObject(m_pSellDlg);
	//assert(_CrtCheckMemory() && "ChatDlg");
	m_pShopDlg = new GameShopDlg;
	m_pShopDlg->Init(m_pResManager, this);
	m_dlgControl.InsertObject(m_pShopDlg);
	m_pSysMenuDlg = new GameSysMenuDlg;
	m_pSysMenuDlg->Init(this);
	m_dlgControl.InsertObject(m_pSysMenuDlg);
	m_pStaticDlg = new GameStaticDlg;
	m_pStaticDlg->Init(this);
	m_dlgControl.InsertObject(m_pStaticDlg);
	m_pSkillDlg = new GameSkillDlg;
	m_pSkillDlg->Init(this);
	m_dlgControl.InsertObject(m_pSkillDlg);
	m_pMapDlg = new GameMapDlg;
	m_dlgControl.InsertObject(m_pMapDlg);
	m_pStoreDlg = new GameStoreDlg;
	m_pStoreDlg->Init(this);
	m_dlgControl.InsertObject(m_pStoreDlg);
	m_pHumDlg2 = new GameHumDlg2;
	m_dlgControl.InsertObject(m_pHumDlg2);
	m_pHelpDlg = new GameHelpDlg;
	m_dlgControl.InsertObject(m_pHelpDlg);
	m_pBagDlg2 = new GameBagDlg2;
	m_dlgControl.InsertObject(m_pBagDlg2);
	m_pCubeDlg = new GameCubeDlg;
	m_dlgControl.InsertObject(m_pCubeDlg);
	m_pQuitDlg = new GameQuitDlg;
	m_dlgControl.InsertObject(m_pQuitDlg);
	m_pAssistDlg = new GameAssistDlg;
	m_dlgControl.InsertObject(m_pAssistDlg);
	m_pTeamDlg = new GameTeamDlg;
	m_dlgControl.InsertObject(m_pTeamDlg);
	m_pDecomposeDlg = new GameDecomposeDlg;
	m_dlgControl.InsertObject(m_pDecomposeDlg);
	m_pQuestDlg = new GameQuestDlg;
	m_dlgControl.InsertObject(m_pQuestDlg);
	m_pSettingDlg = new GameSettingDlg;
	m_dlgControl.InsertObject(m_pSettingDlg);
	m_pShopDlg2 = new GameShopDlg2;
	m_dlgControl.InsertObject(m_pShopDlg2);
	m_pStoreDlg2 = new GameStoreDlg2;
	m_dlgControl.InsertObject(m_pStoreDlg2);
	m_pOtherHumDlg = new GameOtherHumDlg;
	m_dlgControl.InsertObject(m_pOtherHumDlg);
	m_pAttribDlg = new GameAttribDlg;
	m_dlgControl.InsertObject(m_pAttribDlg);
	m_pTeamMenu = new GameTeamMenu;
	m_dlgControl.InsertObject(m_pTeamMenu);
	m_pRoleInfoDlg = new GameRoleInfoDlg;
	m_dlgControl.InsertObject(m_pRoleInfoDlg);
	m_pInputDlg = new GameInputDlg;
	m_dlgControl.InsertObject(m_pInputDlg);
	m_pDonateDlg = new GameDonateDlg;
	m_dlgControl.InsertObject(m_pDonateDlg);
	m_pOfflineSellListDlg = new OfflineSellListDlg;
	m_dlgControl.InsertObject(m_pOfflineSellListDlg);
	m_pDisplayDlg = new GameDisplayDlg;
	m_pOfflineSellDlg = new OfflineSellDlg;
	m_dlgControl.InsertObject(m_pOfflineSellDlg);
	m_pIdentifyDlg = new GameIdentifyDlg;
	m_dlgControl.InsertObject(m_pIdentifyDlg);
	m_pRankDlg = new GameRankDlg;
	m_dlgControl.InsertObject(m_pRankDlg);
	m_pOlShopDlg = new OlShopDlg;
	m_dlgControl.InsertObject(m_pOlShopDlg);
	m_pStoveDlg = new GameStoveDlg;
	m_dlgControl.InsertObject(m_pStoveDlg);
	m_pChargeDlg = new GameChargeDlg;
	m_dlgControl.InsertObject(m_pChargeDlg);
	m_pBigStoreDlg = new GameBigStoreDlg;
	m_dlgControl.InsertObject(m_pBigStoreDlg);

	//m_pInputDlg->ShowAsMode(InputDlg_QueryBuySum);

#ifdef _USE_SCRIPT_
	RegisterMemberFunction();
#endif
	LoadScriptWnd();

	::SendMessage(AfxGetHge()->System_GetState(HGE_HWND), WM_DLG_BRINGTOTOP, WPARAM(m_pActiveDlg), 0);
	return true;
}

void GameMainOptUI::DeleteScriptWnd()
{
	std::list<GameScriptWnd*>::const_iterator begIter = m_xScriptWndList.begin();
	std::list<GameScriptWnd*>::const_iterator endIter = m_xScriptWndList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		GameScriptWnd* pWnd = *begIter;
		SAFE_DELETE(pWnd);
	}

	m_xScriptWndList.clear();
}

void GameMainOptUI::LoadScriptWnd()
{
	char szXmlFile[MAX_PATH];
	sprintf(szXmlFile, "%s/plugins/window/config.xml",
		GetRootPath());

	if(!PathFileExists(szXmlFile))
	{
		return;
	}

	TiXmlDocument doc;
	if(!doc.LoadFile(szXmlFile))
	{
		return;
	}

	TiXmlElement* pRoot = doc.RootElement();

	TiXmlElement* pChild = pRoot->FirstChildElement();
	while(pChild)
	{
		const char* pszWndName = pChild->Value();
		const char* pszFileName = pChild->Attribute("filename");
		const char* pszZipFileName = pChild->Attribute("zipfilename");

		AfxGetHge()->System_Log("开始载入窗体插件[%s]", pszWndName);

		//	补全路径
		char szDestFileName[MAX_PATH];
		char szDestZipFileName[MAX_PATH];
		if(pszFileName)
		{
			strcpy(szDestFileName, pszFileName);

			if(PathIsFileSpec(szDestFileName))
			{
				sprintf(szDestFileName, "%s/plugins/window/%s",
					GetRootPath(), pszFileName);
			}
		}
		if(pszZipFileName)
		{
			strcpy(szDestZipFileName, pszZipFileName);

			if(PathIsFileSpec(szDestZipFileName))
			{
				sprintf(szDestZipFileName, "%s/plugins/window/%s",
					GetRootPath(), pszZipFileName);
			}
		}

		GameScriptWnd* pWnd = NULL;
		if(pszFileName &&
			pszZipFileName)
		{
			pWnd = GameScriptWnd::CreateWithZipFile(szDestZipFileName, szDestFileName);
		}
		else if(pszFileName)
		{
			pWnd = GameScriptWnd::CreateWithFile(szDestFileName);
		}

		if(pWnd)
		{
			m_xScriptWndList.push_back(pWnd);
			m_dlgControl.InsertObject(pWnd);

			AfxGetHge()->System_Log("成功载入窗体插件[%s] ID[%d]", pszWndName, pWnd->GetID());
		}

		pChild = pChild->NextSiblingElement();
	}
}

void GameMainOptUI::UpdateScriptWnd()
{
	if(IsBlockCmd())
	{
		return;
	}

	std::list<GameScriptWnd*>::const_iterator begIter = m_xScriptWndList.begin();
	std::list<GameScriptWnd*>::const_iterator endIter = m_xScriptWndList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		GameScriptWnd* pWnd = *begIter;
		if(pWnd->GetHotKey() != 0)
		{
			if(AfxGetHge()->Input_KeyUp(pWnd->GetHotKey()))
			{
				bool bVisible = pWnd->IsVisible();
				bVisible = !bVisible;
				//pWnd->SetVisible(bVisible);
				if(bVisible)
				{
					pWnd->ShowDialog();
				}
				else
				{
					pWnd->CloseDialog();
				}
			}
		}
	}

	if(AfxGetHge()->Input_KeyUp(HGEK_ESCAPE))
	{
		for(begIter;
			begIter != endIter;
			++begIter)
		{
			GameScriptWnd* pWnd = *begIter;
			if(pWnd->IsESCClose())
			{
				bool bVisible = pWnd->IsVisible();

				if(bVisible)
				{
					pWnd->CloseDialog();
				}
			}
		}
	}
}

void GameMainOptUI::CloseAllScriptWnd()
{
	std::list<GameScriptWnd*>::const_iterator begIter = m_xScriptWndList.begin();
	std::list<GameScriptWnd*>::const_iterator endIter = m_xScriptWndList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		GameScriptWnd* pWnd = *begIter;
		if(pWnd->GetHotKey() != 0)
		{
			if(AfxGetHge()->Input_KeyDown(pWnd->GetHotKey()))
			{
				pWnd->CloseDialog();
			}
		}
	}
}

void GameMainOptUI::ResetStatus()
{
	m_bSysMenuBtnState = 0;
}

void GameMainOptUI::CloseAllDialog()
{
	m_pShopDlg2->CloseDialog();
	m_pStoreDlg2->CloseDialog();
	m_pIdentifyDlg->CloseDialog();
	m_pBigStoreDlg->CloseDialog();
}

void GameMainOptUI::SetMapName(const char* lpszMapName)
{
	if(strlen(lpszMapName) > 19)
	{
		AfxGetHge()->System_Log("地图名字越界 %s", __FUNCTION__);
		return;
	}
	strcpy(m_szMapName, lpszMapName);
}

void GameMainOptUI::Render()
{
	HTEXTURE tex = 0;

#define _NEW_UI

#ifdef _NEW_UI
	GameTextureManager* pOpUI = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	if(!pOpUI)
	{
		return;
	}

	//	Operation bar
	tex = pOpUI->GetTexture(TEX_MAINOPTUI_NEW);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0,
			pOpUI->GetTextureWidth(TEX_MAINOPTUI_NEW),
			pOpUI->GetTextureHeight(TEX_MAINOPTUI_NEW));
		m_pRender->Render(0, VIEW_HEIGHT + VIEW_OPERATE - pOpUI->GetTextureHeight(TEX_MAINOPTUI_NEW));
	}

	//	Level
	AfxGetFont10()->SetColor(ARGB_WHITE);
	int nPrintLeft = 4;
	int nLevel = m_pPlayer->GetAttrib()->level;
	if(nLevel > 99)
	{
		nPrintLeft = 4 + 20 / 2 - 5 * 3 / 2;
	}
	else if(nLevel > 9)
	{
		nPrintLeft = 4 + 20 / 2 - 5 * 2 / 2;
	}
	else
	{
		nPrintLeft = 4 + 20 / 2 - 5 * 1 / 2;
	}
	AfxGetFont10()->Print(nPrintLeft, 450 + 110, "%d", m_pPlayer->GetAttrib()->level);

	//	Money
	AfxGetFont10()->SetColor(ARGB_WHITE);
	//AfxGetFont10()->Print(697, 450 + 122, "%d", m_pPlayer->GetPlayerBag()->GetMoney());
	Gfx_PrintNumberWithComma(AfxGetFont10(), 697, 450 + 122, m_pPlayer->GetPlayerBag()->GetMoney());

	//	Button states
	if(m_bBtnState[RECT_BTNHUM] == BTN_STATE_OVER)
	{
		tex = pOpUI->GetTexture(TEX_HUMBTN + 1);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_HUMBTN_LEFT, ALIGN_HUMBTN_TOP);
		}
	}
	else if(m_bBtnState[RECT_BTNHUM] == BTN_STATE_DOWN)
	{
		tex = pOpUI->GetTexture(TEX_HUMBTN + 2);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_HUMBTN_LEFT, ALIGN_HUMBTN_TOP);
		}
	}
	if(m_bBtnState[RECT_BTNBAG] == BTN_STATE_OVER)
	{
		tex = pOpUI->GetTexture(TEX_BAGBTN + 1);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_BAGBTN_LEFT, ALIGN_HUMBTN_TOP);
		}
	}
	else if(m_bBtnState[RECT_BTNBAG] == BTN_STATE_DOWN)
	{
		tex = pOpUI->GetTexture(TEX_BAGBTN + 2);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_BAGBTN_LEFT, ALIGN_HUMBTN_TOP);
		}
	}
	if(m_bBtnState[RECT_BTNSKILL] == BTN_STATE_OVER)
	{
		tex = pOpUI->GetTexture(TEX_SKILLBTN + 1);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_SKILLBTN_LEFT, ALIGN_SKILLBTN_TOP);
		}
	}
	else if(m_bBtnState[RECT_BTNSKILL] == BTN_STATE_DOWN)
	{
		tex = pOpUI->GetTexture(TEX_SKILLBTN + 2);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_SKILLBTN_LEFT, ALIGN_SKILLBTN_TOP);
		}
	}
	if(m_bBtnState[RECT_BTNTASK] == BTN_STATE_OVER)
	{
		tex = pOpUI->GetTexture(TEX_QUESTBTN + 1);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_QUESTBTN_LEFT, ALIGN_QUESTBTN_TOP);
		}
	}
	else if(m_bBtnState[RECT_BTNTASK] == BTN_STATE_DOWN)
	{
		tex = pOpUI->GetTexture(TEX_QUESTBTN + 2);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_QUESTBTN_LEFT, ALIGN_QUESTBTN_TOP);
		}
	}
	if(m_bBtnState[RECT_BTNHANDMADE] == BTN_STATE_OVER)
	{
		tex = pOpUI->GetTexture(TEX_HANDMADEBTN + 1);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_HANDMADEBTN_LEFT, ALIGN_HANDMADEBTN_TOP);
		}
	}
	else if(m_bBtnState[RECT_BTNHANDMADE] == BTN_STATE_DOWN)
	{
		tex = pOpUI->GetTexture(TEX_HANDMADEBTN + 2);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_HANDMADEBTN_LEFT, ALIGN_HANDMADEBTN_TOP);
		}
	}
	if(m_bBtnState[RECT_SYSMENU] == BTN_STATE_OVER)
	{
		tex = pOpUI->GetTexture(TEX_SYSMENUBTN + 1);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_SYSMENUBTN_LEFT, ALIGN_SYSMENUBTN_TOP);
		}
	}
	else if(m_bBtnState[RECT_SYSMENU] == BTN_STATE_DOWN)
	{
		tex = pOpUI->GetTexture(TEX_SYSMENUBTN + 2);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 20);
			m_pRender->Render(ALIGN_SYSMENUBTN_LEFT, ALIGN_SYSMENUBTN_TOP);
		}
	}

	if(m_bBtnState[RECT_OFFSELL] == BTN_STATE_OVER)
	{
		tex = pOpUI->GetTexture(826 + 1);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 40, 38);
			m_pRender->Render(m_rcOtherRect[RECT_OFFSELL].left,
				m_rcOtherRect[RECT_OFFSELL].top);
		}
	}
	else if(m_bBtnState[RECT_OFFSELL] == BTN_STATE_DOWN)
	{
		tex = pOpUI->GetTexture(826 + 2);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 40, 38);
			m_pRender->Render(m_rcOtherRect[RECT_OFFSELL].left,
				m_rcOtherRect[RECT_OFFSELL].top);
		}
	}

	if(m_bBtnState[RECT_OLSHOP] == BTN_STATE_OVER)
	{
		tex = pOpUI->GetTexture(823 + 1);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 40, 38);
			m_pRender->Render(m_rcOtherRect[RECT_OLSHOP].left,
				m_rcOtherRect[RECT_OLSHOP].top);
		}
	}
	else if(m_bBtnState[RECT_OLSHOP] == BTN_STATE_DOWN)
	{
		tex = pOpUI->GetTexture(823 + 2);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 40, 38);
			m_pRender->Render(m_rcOtherRect[RECT_OLSHOP].left,
				m_rcOtherRect[RECT_OLSHOP].top);
		}
	}
	/*else if(m_bBtnState[RECT_OFFSELL] == BTN_STATE_NORMAL)
	{
		tex = pOpUI->GetTexture(826);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 40, 38);
			m_pRender->Render(m_rcOtherRect[RECT_OFFSELL].left,
				m_rcOtherRect[RECT_OFFSELL].top);
		}
	}

	if(AfxGetHge()->Input_GetKeyState(HGEK_UP))
	{
		--m_rcOtherRect[RECT_OFFSELL].top;
		--m_rcOtherRect[RECT_OFFSELL].bottom;
	}
	if(AfxGetHge()->Input_GetKeyState(HGEK_DOWN))
	{
		++m_rcOtherRect[RECT_OFFSELL].top;
		++m_rcOtherRect[RECT_OFFSELL].bottom;
	}
	if(AfxGetHge()->Input_GetKeyState(HGEK_LEFT))
	{
		--m_rcOtherRect[RECT_OFFSELL].left;
		--m_rcOtherRect[RECT_OFFSELL].right;
	}
	if(AfxGetHge()->Input_GetKeyState(HGEK_RIGHT))
	{
		++m_rcOtherRect[RECT_OFFSELL].left;
		++m_rcOtherRect[RECT_OFFSELL].right;
	}*/

	//	HP		MP
	tex = pOpUI->GetTexture(TEX_HPMP_BAR);
	if(tex)
	{
		float fScale = 0.0f;

		m_pRender->SetTexture(tex);
		
		fScale = (float)m_pPlayer->GetAttrib()->HP / (float)m_pPlayer->GetAttrib()->maxHP;
		int nTexHeight = 80;
		nTexHeight *= fScale;
		m_pRender->SetTextureRect(0, 80 - nTexHeight, ALIGN_HPMP_MP_LEFT, nTexHeight);
		m_pRender->Render(ALIGN_HPMP_HP_LEFT, ALIGN_HPMP_HP_TOP + 80 - nTexHeight);

		fScale = (float)m_pPlayer->GetAttrib()->MP / (float)m_pPlayer->GetAttrib()->maxMP;
		nTexHeight = 80;
		nTexHeight *= fScale;
		m_pRender->SetTextureRect(ALIGN_HPMP_MP_LEFT, 80 - nTexHeight, 104 - ALIGN_HPMP_MP_LEFT, nTexHeight);
		m_pRender->Render(ALIGN_HPMP_MP_LEFT, ALIGN_HPMP_MP_TOP + 80 - nTexHeight);
	}

	//	HP MP text
	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->Print(40 / 2 + 9 - GetNumberBit(m_pPlayer->GetAttrib()->HP) * 6 / 2,
		VIEW_HEIGHT + VIEW_OPERATE - 150 + 52, "%d", m_pPlayer->GetAttrib()->HP);
	AfxGetHge()->Gfx_RenderLine(15, VIEW_HEIGHT + VIEW_OPERATE - 150 + 70,
		45, VIEW_HEIGHT + VIEW_OPERATE - 150 + 70);
	AfxGetPrinter()->Print(40 / 2 + 9 - GetNumberBit(m_pPlayer->GetAttrib()->maxHP) * 6 / 2,
		VIEW_HEIGHT + VIEW_OPERATE - 150 + 76, "%d", m_pPlayer->GetAttrib()->maxHP);

	AfxGetPrinter()->Print(48 + 40 / 2 - GetNumberBit(m_pPlayer->GetAttrib()->MP) * 6 / 2,
		VIEW_HEIGHT + VIEW_OPERATE - 150 + 52, "%d", m_pPlayer->GetAttrib()->MP);
	AfxGetHge()->Gfx_RenderLine(48 + 6, VIEW_HEIGHT + VIEW_OPERATE - 150 + 70,
		52 + 36, VIEW_HEIGHT + VIEW_OPERATE - 150 + 70);
	AfxGetPrinter()->Print(48 + 40 / 2 - GetNumberBit(m_pPlayer->GetAttrib()->maxMP) * 6 / 2,
		VIEW_HEIGHT + VIEW_OPERATE - 150 + 76, "%d", m_pPlayer->GetAttrib()->maxMP);

	//	Expr
	tex = pOpUI->GetTexture(TEX_EXPR_BAR);
	if(tex)
	{
		float fScale = 0.0f;
		m_pRender->SetTexture(tex);
		int nTexWidth = 784;
		fScale = (float)m_pPlayer->GetAttrib()->EXPR / (float)m_pPlayer->GetAttrib()->maxEXPR;
		nTexWidth = fScale * 784;

		m_pRender->SetTextureRect(0, 0, nTexWidth, 7);
		m_pRender->Render(ALIGN_EXPR_LEFT, ALIGN_EXPR_TOP);
	}

	//	Map name and coordinates of the player
	AfxGetFont10()->SetColor(ARGB_WHITE);
	AfxGetFont10()->Print(7, 450 + 125, "%s (%d,%d)",
		m_szMapName, (int)m_pPlayer->GetObjx() / UNIT_WIDTH, (int)m_pPlayer->GetObjy() / UNIT_HEIGHT);

	/*AfxGetFont10()->Print(175, 450 + 81 - 2, "%d/%d",
		m_pPlayer->GetAttrib()->HP, m_pPlayer->GetAttrib()->maxHP);
	AfxGetFont10()->Print(175, 450 + 94 - 2, "%d/%d",
		m_pPlayer->GetAttrib()->MP, m_pPlayer->GetAttrib()->maxMP);*/
	if(m_pPlayer->GetAttrib()->maxEXPR != 0)
	{
		float fExprScale = (float)m_pPlayer->GetAttrib()->EXPR / (float)m_pPlayer->GetAttrib()->maxEXPR;
		char szExpr[32] = {0};
		sprintf(szExpr, "%.2f%%", fExprScale * 100);
		/*AfxGetFont10()->Print(175, 450 + 107 - 2, "%.2f%%",
			fExprScale * 100);*/
		AfxGetPrinter()->Print(390, 600 - 11, "%s", szExpr);
	}
#else
	tex = m_pResManager->GetTexture("mainboard");
	if(tex)
	{
		INITSPRITE(m_pSpr, tex);
		m_pSpr->Render(0, VIEW_HEIGHT + VIEW_OPERATE - 150);
	}

	int nDrawOffset = 0;

	tex = m_pResManager->GetTexture("mp");
	if(tex)
	{
		nDrawOffset = 105 - ((float)m_pPlayer->GetAttrib()->MP / (float)m_pPlayer->GetAttrib()->maxMP) * 105;
		m_pSpr->SetTexture(tex);
		m_pSpr->SetTextureRect(0, nDrawOffset, 71, 105 - nDrawOffset);
		m_pSpr->Render(70, 431 + nDrawOffset);
	}

	tex = m_pResManager->GetTexture("hp");
	if(tex)
	{
		nDrawOffset = 104 - ((float)m_pPlayer->GetAttrib()->HP / (float)m_pPlayer->GetAttrib()->maxHP) * 104;
		m_pSpr->SetTexture(tex);
		m_pSpr->SetTextureRect(0, nDrawOffset, 69, 104 - nDrawOffset);
		m_pSpr->Render(31, 427 + nDrawOffset);
	}

	tex = m_pResManager->GetTexture("exp");
	if(tex)
	{
		if(m_pPlayer->GetAttrib()->EXPR == 0)
		{

		}
		else if(m_pPlayer->GetAttrib()->EXPR > m_pPlayer->GetAttrib()->maxEXPR)
		{
			m_pSpr->SetTexture(tex);
			m_pSpr->SetTextureRect(0, 0, 499, 9);
			m_pSpr->Render(257, 531);
		}
		else
		{
			nDrawOffset = ((float)m_pPlayer->GetAttrib()->EXPR / (float)m_pPlayer->GetAttrib()->maxEXPR) * 499;
			m_pSpr->SetTexture(tex);
			m_pSpr->SetTextureRect(0, 0, nDrawOffset, 9);
			m_pSpr->Render(257, 531);
		}
	}

	AfxGetPrinter()->SetColor(TEXT_ARGB_WHITE);
	AfxGetPrinter()->Print(631, 500, "\n%s  %d,%d", m_szMapName,
		(int)m_pPlayer->GetObjx() / UNIT_WIDTH/*m_pPlayer->GetCoordX()*/,
		(int)m_pPlayer->GetObjy() / UNIT_HEIGHT/*m_pPlayer->GetCoordY()*/);
	//	Print time
	static SYSTEMTIME st;
	static DWORD dwLastGetTime = 0;
	if(GetTickCount() - dwLastGetTime > 60 * 1000)
	{
		::GetLocalTime(&st);
		dwLastGetTime = GetTickCount();
	}
	//m_pFont->Print(700, 500, "%02d:%02d", st.wHour, st.wMinute);
	AfxGetPrinter()->SetColor(TEXT_ARGB_WHITE);
	AfxGetPrinter()->Print(631, 500, "%02d:%02d", st.wHour, st.wMinute);

	//	级数信息
	tex = m_pResManager->GetTexture("lvbk");
	if(tex)
	{
		m_pSpr->SetTexture(tex);
		m_pSpr->SetTextureRect(0, 0, 54, 21);
		m_pSpr->Render(152, 513);
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(152 + 2 + 4, 513 + 2 + 2, "LV:%d", m_pPlayer->GetAttrib()->level);
	}

	//	画格子物品
	GameTextureManager* pRes = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	ItemAttrib* pItem = NULL;
	if(pRes)
	{
		int nDrawOffsetX = 0;
		int nDrawOffsetY = 0;

		for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
		{
			pItem = &m_pPlayer->GetPlayerBag()->GetAssistItemList()[i];
			if(pItem->type != ITEM_NO &&
				m_nSel != i)
			{
				tex = pRes->GetTexture(pItem->tex);
				if(tex)
				{
// 					nDrawOffsetX = pRes->GetTextureWidth(tex);
// 					nDrawOffsetY = pRes->GetTextureHeight(tex);
					nDrawOffsetX = pRes->GetTextureWidth(pItem->tex);
					nDrawOffsetY = pRes->GetTextureHeight(pItem->tex);
					m_pSpr->SetTexture(tex);
					m_pSpr->SetTextureRect(0, 0, nDrawOffsetX, nDrawOffsetY);
					nDrawOffsetX = abs(nDrawOffsetX - 36) / 2;
					nDrawOffsetY = abs(nDrawOffsetY - 36) / 2;
					m_pSpr->Render(m_rcOtherRect[RECT_ACC1 + i].left + nDrawOffsetX,
						m_rcOtherRect[RECT_ACC1 + i].top + nDrawOffsetY);

					AfxGetPrinter()->SetColor(ARGB_WHITE);
					AfxGetPrinter()->Print(m_rcOtherRect[RECT_ACC1 + i].left + 5,
						m_rcOtherRect[RECT_ACC1 + i].top + 20,
						"%d",
						pItem->atkSpeed);
				}
			}
		}

		//	画选中物品
		if(IsAssistItemSel())
		{
			pItem = GetAssistItemSel();
			if(pItem->type != ITEM_NO)
			{
				tex = pRes->GetTexture(pItem->tex);
				if(tex)
				{
// 					nDrawOffsetX = pRes->GetTextureWidth(tex);
// 					nDrawOffsetY = pRes->GetTextureHeight(tex);
					nDrawOffsetX = pRes->GetTextureWidth(pItem->tex);
					nDrawOffsetY = pRes->GetTextureHeight(pItem->tex);
					m_pSpr->SetTexture(tex);
					m_pSpr->SetTextureRect(0, 0, nDrawOffsetX, nDrawOffsetY);
					nDrawOffsetX /= 2;
					nDrawOffsetY /= 2;

					float x, y;
					AfxGetHge()->Input_GetMousePos(&x, &y);
					m_pSpr->Render(x - nDrawOffsetX,
						y - nDrawOffsetY);
				}
			}
		}
	}

	//	SysMenuBtn
	tex = m_pResManager->GetTexture("arrowbtnup");
	if(tex)
	{
		m_pSpr->SetTexture(tex);
		if(m_bSysMenuBtnState == 0)
		{
			m_pSpr->SetTextureRect(0, 0, 23, 24);
		}
		else if(TEST_FLAG(m_bSysMenuBtnState, BTN_OVER_MASK))
		{
			m_pSpr->SetTextureRect(0, 24, 23, 24);
		}
		else if(TEST_FLAG(m_bSysMenuBtnState, BTN_DOWN_MASK))
		{
			m_pSpr->SetTextureRect(0, 24*2, 23, 24);
		}
		m_pSpr->Render(m_rcOtherRect[RECT_SYSMENU].left,
			m_rcOtherRect[RECT_SYSMENU].top);
	}
#endif

	//	延迟时间
	int nDelayLevel = 1;
	DWORD dwNetDelayMillSeconds = GameScene::sThis->GetNetDelayMillSeconds();
	if(dwNetDelayMillSeconds < 50)
	{
		nDelayLevel = 1;
	}
	else if(dwNetDelayMillSeconds < 100)
	{
		nDelayLevel = 2;
	}
	else if(dwNetDelayMillSeconds < 150)
	{
		nDelayLevel = 3;
	}
	else if(dwNetDelayMillSeconds < 200)
	{
		nDelayLevel = 4;
	}
	HTEXTURE hDelayTexture = 0;
	hgeResourceManager* pResMgr = pTheGame->GetGameRes();
	if(pResMgr)
	{
		char szTextName[32];
		sprintf(szTextName, "state_%d", nDelayLevel);
		hDelayTexture = pResMgr->GetTexture(szTextName);

		if(hDelayTexture)
		{
			m_pRender->SetTexture(hDelayTexture);
			m_pRender->SetTextureRect(0, 0,
				16, 16);
			m_pRender->Render(164, 505);
		}
	}

	//	快捷魔法
	int nAccMagicID = GamePlayer::GetInstance()->GetAccMagicID();
	if(0 != nAccMagicID)
	{
		const MagicDetail* pDetail = GamePlayer::GetInstance()->GetMagicByID(nAccMagicID);
		if(NULL != pDetail)
		{
			//AfxGetPrinter()->Print(VIEW_WIDTH - 100, WINDOW_HEIGHT - 200, "鼠标中键释放:\n%s", pDetail->szName);

			GameTextureManager* pTexIcon = GameResourceManager::GetInstance()->GetTexs(RES_MAGICICON);
			tex = pTexIcon->GetTexture(pDetail->wID);
			if(tex)
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0,
					pTexIcon->GetTextureWidth(pDetail->wID),
					pTexIcon->GetTextureHeight(pDetail->wID));
				m_pRender->Render(VIEW_WIDTH - 100 + 30,
					WINDOW_HEIGHT - 200 + 30);
			}
		}
	}

	//	控制下属对话框绘制
	m_dlgControl.Render();
	if(m_pIntroDlg)
	{
		m_pIntroDlg->Render();
	}
	if(m_pDisplayDlg)
	{
		m_pDisplayDlg->Render();
	}
}

void GameMainOptUI::Update(float _dt)
{
	//m_pIntroDlg->ShowNothing();
	m_dlgControl.Update(_dt);
	UpdateScriptWnd();
}

bool GameMainOptUI::ProcUserCmd(const POINT& _mp)
{
	POINT ptMouse = _mp;
	int nClicked = -1;

	ZeroMemory(m_bBtnState, sizeof(m_bBtnState));
	bool bCanProcess = false;

	//	If we can filter this point
	if(_mp.x < 85 &&
		_mp.y > 450 &&
		_mp.y < 500)
	{
		bCanProcess = true;
	}

	if(_mp.x > 680 &&
		_mp.x < WINDOW_WIDTH &&
		_mp.y > 450 + 30 &&
		_mp.y < 450 + 52)
	{
		bCanProcess = true;
	}

	if(_mp.y >= 450 + 52)
	{
		bCanProcess = true;
	}

	if(_mp.x >= VIEW_WIDTH - 100 + 30 &&
		_mp.x <= VIEW_WIDTH - 100 + 30 + 33 &&
		_mp.y >= WINDOW_HEIGHT - 200 + 30 &&
		_mp.y <= WINDOW_HEIGHT - 200 + 30 + 33)
	{
		bCanProcess = true;
	}

	if(!bCanProcess)
	{
		return false;
	}

	if(m_pChatDlg->ProcUserCmd(_mp))
	{
		return true;
	}

	/*if(AfxGetHge()->Input_KeyUp(HGEK_LEFT))
	{
		m_rcOtherRect[RECT_OLSHOP].left++;
		m_rcOtherRect[RECT_OLSHOP].right++;
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_RIGHT))
	{
		m_rcOtherRect[RECT_OLSHOP].left--;
		m_rcOtherRect[RECT_OLSHOP].right--;
	}*/

//	char szBuf[10];

	for(int i = RECT_ACC1; i <= RECT_OLSHOP; ++i)
	{
		if(::PtInRect(&m_rcOtherRect[i], ptMouse))
		{
			switch(i)
			{
			case RECT_ACC1:
			case RECT_ACC2:
			case RECT_ACC3:
			case RECT_ACC4:
			case RECT_ACC5:
			case RECT_ACC6:
				{
					//	药品
					OnAcc(i);
					nClicked = i;
				}break;
			case RECT_BTNHUM:
			case RECT_BTNBAG:
			case RECT_BTNTASK:
			case RECT_BTNSKILL:
			case RECT_BTNHANDMADE:
			case RECT_OFFSELL:
			case RECT_OLSHOP:
				{
					OnBtn(i);
					nClicked = i;
				}break;
			case RECT_SYSMENU:
				{
					OnSysMenuBtn();
					nClicked = i;
				}break;
			}
			//	跳出检测
			return true;;
		}
	}

	if(nClicked != RECT_SYSMENU)
	{
		m_bSysMenuBtnState = 0;
	}

	//	检测其余区域
	/*if(_mp.x > 254 &&
		_mp.x < 750 &&
		_mp.y < VIEW_HEIGHT + VIEW_OPERATE &&
		_mp.y > VIEW_OPERATE + VIEW_HEIGHT - 7)
	{
		m_pIntroDlg->ShowExp();
		return true;
	}
	else if(_mp.x > 30 &&
		_mp.x < 83)
	{
		m_pIntroDlg->ShowHP();
		return true;
	}
	else if(_mp.x >= 83 &&
		_mp.x < 138)
	{
		m_pIntroDlg->ShowMP();
		return true;
	}*/
	//	快捷魔法提示
	if(_mp.x >= VIEW_WIDTH - 100 + 30 &&
		_mp.x <= VIEW_WIDTH - 100 + 30 + 42 &&
		_mp.y >= WINDOW_HEIGHT - 200 + 30 &&
		_mp.y <= WINDOW_HEIGHT - 200 + 30 + 42)
	{
		int nAccMagicID = GamePlayer::GetInstance()->GetAccMagicID();
		if(0 != nAccMagicID)
		{
			const MagicDetail* pDetail = GamePlayer::GetInstance()->GetMagicByID(nAccMagicID);
			if(NULL != pDetail)
			{
				char szTip[MAX_PATH] = {0};
				sprintf(szTip, "~键(数字1旁边按键)释放:%s", pDetail->szName);
				m_pIntroDlg->ShowString(szTip);
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
void GameMainOptUI::OnSysMenuBtn()
{
	//m_bSysMenuBtnState = 0;
	//SET_FLAG(m_bSysMenuBtnState, BTN_OVER_MASK);
	m_bBtnState[RECT_SYSMENU] = BTN_STATE_OVER;

	if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
	{
		//m_bSysMenuBtnState = 0;
		//SET_FLAG(m_bSysMenuBtnState, BTN_DOWN_MASK);
		m_bBtnState[RECT_SYSMENU] = BTN_STATE_DOWN;
	}

	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		bool bVisible = m_pSysMenuDlg->IsVisible();
		m_pSysMenuDlg->SetVisible(!bVisible);
		if(m_pSysMenuDlg->IsVisible())
		{
			m_pSysMenuDlg->SetNeedTopLevel(true);
		}
		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
	}

	m_pIntroDlg->ShowString("系统菜单(E)");
}

void GameMainOptUI::OnAcc(int _idx)
{
	//	1-6快捷键
	bool bHaveItem = false;
	int nAccIndex = _idx - 1;
	ItemAttrib* pItem = &m_pPlayer->GetPlayerBag()->GetAssistItemList()[nAccIndex];
	if(pItem->type != ITEM_NO &&
		nAccIndex != m_nSel)
	{
		//	有东西存在
		m_pIntroDlg->ShowItemAttrib(pItem);
	}

	if(m_pBagDlg2->IsItemSel())
	{
		//	有东西选中
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			if(m_pBagDlg2->GetSelItem()->type != ITEM_COST)
			{
				return;
			}

			ItemList& assistItems = m_pPlayer->GetPlayerBag()->GetAssistItemList();

			if(assistItems[nAccIndex].type == ITEM_NO)
			{
				for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
				{
					if(assistItems[i].id == m_pBagDlg2->GetSelItem()->id)
					{
						m_pPlayer->GetPlayerBag()->ChangeAssistItem(nAccIndex, i);
						CancelSel();
						GetBagDlg2()->CancelSel();
						return;
					}
				}
			}
			
			if(m_pPlayer->MoveBagItemToAssistItem(m_pBagDlg2->GetSelIndex(), nAccIndex))
			{
				m_pBagDlg2->CancelSel();
			}
		}
	}
	else if(IsAssistItemSel())
	{
		//	辅助栏有东西被选中
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			m_pPlayer->GetPlayerBag()->ChangeAssistItem(nAccIndex, GetAssistSelIndex());
			CancelSel();
		}
	}
	else
	{
		//	没有
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			m_nSel = nAccIndex;
		}
		else if(AfxGetHge()->Input_KeyUp(HGEK_RBUTTON))
		{
			m_pPlayer->UseItemFromAssist(nAccIndex);
		}
	}
}

void GameMainOptUI::OnBtn(int _idx)
{
	//	UI
	m_bBtnState[_idx] = BTN_STATE_OVER;
	if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
	{
		m_bBtnState[_idx] = BTN_STATE_DOWN;
	}

	//	command
	switch(_idx)
	{
	case RECT_BTNHUM:
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				//m_pHumDlg->SetVisible(!m_pHumDlg->IsVisible());
				//m_pHumDlg2->SetVisible(!m_pHumDlg2->IsVisible());
				bool bPreVisible = GetHumDlg2()->IsVisible();
				if(bPreVisible)
				{
					m_pHumDlg2->CloseDialog();
				}
				else
				{
					m_pHumDlg2->ShowDialog();
				}
				GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
			}
			m_pIntroDlg->ShowString("人物(F10)");
		}break;
	case RECT_BTNBAG:
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				bool bPreVisible = GetBagDlg2()->IsVisible();
				if(bPreVisible)
				{
					GetBagDlg2()->CloseDialog();
				}
				else
				{
					GetBagDlg2()->ShowDialog();
				}
				//m_pBagDlg->SetVisible(!m_pBagDlg->IsVisible());
				GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
			}
			m_pIntroDlg->ShowString("背包(F9)");
		}break;
	case RECT_BTNTASK:
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				//m_pBagDlg->SetVisible(!m_pBagDlg->IsVisible());
				bool bPreVisible = GetQuestDlg()->IsVisible();
				if(bPreVisible)
				{
					GetQuestDlg()->CloseDialog();
				}
				else
				{
					GetQuestDlg()->ShowDialog();
				}
				GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
			}
			m_pIntroDlg->ShowString("任务(Q)");
		}break;
	case RECT_BTNSKILL:
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				//m_pBagDlg->SetVisible(!m_pBagDlg->IsVisible());
				//m_pSkillDlg->SetVisible(!m_pSkillDlg->IsVisible());
				bool bPreVisible = GetSkillDlg()->IsVisible();
				if(bPreVisible)
				{
					GetSkillDlg()->CloseDialog();
				}
				else
				{
					GetSkillDlg()->ShowDialog();
				}
				GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
			}
			m_pIntroDlg->ShowString("技能(F11)");
		}break;
	case RECT_BTNHANDMADE:
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				bool bPreVisible = GetDecomposeDlg()->IsVisible();
				if(bPreVisible)
				{
					GetDecomposeDlg()->CloseDialog();
				}
				else
				{
					GetDecomposeDlg()->ShowDialog();
				}
				GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
			}
			m_pIntroDlg->ShowString("锻造(M)");
		}break;
	case RECT_OFFSELL:
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				bool bPreVisible = GetOfflineSellListDlg()->IsVisible();
				if(bPreVisible)
				{
					GetOfflineSellListDlg()->CloseDialog();
				}
				else
				{
					GetOfflineSellListDlg()->ShowDialog();
				}
				GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
			}
			m_pIntroDlg->ShowString("寄售店铺(S)");
		}break;
	case RECT_OLSHOP:
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				bool bPreVisible = GetOlShopDlg()->IsVisible();
				if(bPreVisible)
				{
					GetOlShopDlg()->CloseDialog();
				}
				else
				{
					GetOlShopDlg()->ShowDialog();
				}
				GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
			}
			m_pIntroDlg->ShowString("商城");
		}break;
	}
}

bool GameMainOptUI::IsAssistItemSel()
{
	return m_nSel != -1;
}

void GameMainOptUI::CancelSel()
{
	m_nSel = -1;
}

ItemAttrib* GameMainOptUI::GetAssistItemSel()
{
	assert(m_nSel >= 0 && m_nSel < HERO_CLIENT_ASSIST_SIZE);
	return &m_pPlayer->GetPlayerBag()->GetAssistItemList()[m_nSel];
}

int GameMainOptUI::GetAssistSelIndex()
{
	return m_nSel;
}

bool GameMainOptUI::IsEditInFocus()
{
	bool bEditInFocus = false;
	if(GetOfflineSellDlg()->IsEditInFocus() ||
		GetChatDlg()->IsEditInFocus() ||
		GetInputDlg()->IsEditInFocus())
	{
		bEditInFocus = true;
	}

	return bEditInFocus;
}

//////////////////////////////////////////////////////////////////////////
/*
int GameMainOptUI::GetAssistItemsEmptySum()
{
	ItemList& xItems = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList();
	int nCounter = 0;

	ItemList::const_iterator begiter = xItems.begin();
	ItemList::const_iterator enditer = xItems.end();
	for(begiter; begiter != enditer; ++begiter)
	{
		if(begiter->type == ITEM_NO)
		{
			++nCounter;
		}
	}
	return nCounter;
}*/

#ifdef _USE_SCRIPT_
void GameMainOptUI::RegisterMemberFunction()
{
	//m_Script->GetGlobals().RegisterDirect("SetCommand", *this, &SetCommand);
	m_Script->GetGlobals().RegisterDirect("ScriptCommand", *this, &GameMainOptUI::ScriptCommand);
}

int GameMainOptUI::ScriptCommand(int _cmd, int _param0, int _param1, int _param2)
{
	//	脚本交互函数
	if(_cmd > SCRIPT_CMD_BEGIN &&
		_cmd < SCRIPT_CMD_END)
	{
		switch(_cmd)
		{
		case SCRIPT_CMD_ADDITEM:
			{
				return Script_AddItem(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_HAVEITEM:
			{
				return Script_HaveItem(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_EMPTY_ITEM:
			{
				return Script_EmptyItem(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_CHANGEHP:
			{
				return Script_ChangeHP(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_CHANGEMP:
			{
				return Script_ChangeMP(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_CHANGEEXP:
			{
				return Script_ChangeEXP(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_MOVEPLAYER:
			{
				return Script_MovePlayer(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_CLOSEDLG:
			{
				return Script_CloseDlg(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_CLOSEALLDLG:
			{
				return Script_CloseAllDlg(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_SHOWTRADEDLG:
			{
				return Script_ShowTradeDlg(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_CREATENPC:
			{
				return Script_CreateNPC(_param0, _param1, _param2);
			}
		case SCRIPT_CMD_GETPLAYERCOORDX:
			{
				return Script_GetPlayerCoordX(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_GETPLAYERCOORDY:
			{
				return Script_GetPlayerCoordY(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_EXECUTEERROR:
			{
				return Script_ExecuteError(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_GETQUESTSTAGE:
			{
				return Script_GetQuestStage(_param0, _param1, _param2);
			}break;
		case SCRIPT_CMD_ADDQUESTSTAGE:
			{
				return Script_AddQuestStage(_param0, _param1, _param2);
			}break;
		default:
			{
				AfxGetHge()->System_Log("脚本命令ID[%d]还未有对应实现", _cmd);
				assert("无法处理已知脚本命令ID");
				return 0;
			}break;
		}
	}
	else
	{
		AfxGetHge()->System_Log("未知的脚本命令ID[%d]", _cmd);
		assert("未知的脚本命令ID");
		return 0;
	}

	return 0;
}

//	脚本交互函数
int GameMainOptUI::Script_AddItem(int _param0, int _param1, int _param2)
{
	ItemAttrib item;
	item.id = _param0;
	if(GameInfoManager::GetInstance()->GetItemAttrib(_param0, &item))
	{
		if(m_pPlayer->GetPlayerBag()->AddItem(&item))
		{
			GameInfoBoardDlg::GetInstance()->ShowItemMsg(&item);
			return 1;
		}
	}
	
	return 0;
}

int GameMainOptUI::Script_ChangeMP(int _param0, int _param1, int _param2)
{
	return 0;
}

int GameMainOptUI::Script_ChangeHP(int _param0, int _param1, int _param2)
{
	return 0;
}

int GameMainOptUI::Script_ChangeEXP(int _param0, int _param1, int _param2)
{
	return 0;
}

int GameMainOptUI::Script_EmptyItem(int _param0, int _param1, int _param2)
{
	return 0;
}

int GameMainOptUI::Script_HaveItem(int _param0, int _param1, int _param2)
{
	if(_param0 < 0)
	{
		//	Ex
		switch(_param0)
		{
		case -1:
			{
				//	金钱
				return m_pPlayer->GetPlayerBag()->GetMoney();
			}break;
		case -2:
			{
				//	等级
				return m_pPlayer->GetAttrib()->level;
			}break;
		}
	}
	else
	{
		//	物品ID
	}

	return 0;
}

int GameMainOptUI::Script_MovePlayer(int _param0, int _param1, int _param2)
{
	if(GameMapManager::GetInstance()->CanThrough(_param0, _param1))
	{
		m_pPlayer->SetReallyCoord(_param0, _param1);
		return 1;
	}
	return 0;
}

int GameMainOptUI::Script_CloseDlg(int _param0, int _param1, int _param2)
{
	m_pActiveDlg->SetVisible(false);
	return 1;
}

int GameMainOptUI::Script_CloseAllDlg(int _param0, int _param1, int _param2)
{
	m_pActiveDlg->SetVisible(false);
	m_pBagDlg->SetVisible(false);
	m_pHumDlg->SetVisible(false);
	return 1;
}

int GameMainOptUI::Script_CreateNPC(int _param0, int _param1, int _param2)
{
//	bool bSuc = GameMapManager::GetInstance()->CreateMonster(_param1, _param2, _param0);
//	return bSuc ? 1 : 0;
	return 0;
}

int GameMainOptUI::Script_GetPlayerCoordX(int _param0, int _param1, int _param2)
{
	return m_pPlayer->GetCoordX();
}

int GameMainOptUI::Script_GetPlayerCoordY(int _param0, int _param1, int _param2)
{
	return m_pPlayer->GetCoordY();
}

int GameMainOptUI::Script_ExecuteError(int _param0, int _param1, int _param2)
{
	GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "脚本[%d]执行出错, 信息 param1[%d] param2[%d]", _param0, _param1, _param2);
	return 1;
}

int GameMainOptUI::Script_ShowTradeDlg(int _param0, int _param1, int _param2)
{
	m_pSellDlg->CreateSellDlg(_param0, _param1, _param2);
	return 1;
}

int GameMainOptUI::Script_GetQuestStage(int _param0, int _param1, int _param2)
{
	return m_quest.GetStage();
}

int GameMainOptUI::Script_AddQuestStage(int _param0, int _param1, int _param2)
{
	m_quest.AddStage(_param0);
	return 1;
}

#endif