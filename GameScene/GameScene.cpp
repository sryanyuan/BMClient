#include "../GameScene/GameScene.h"
#include "../Common/SKnl3Helper.h"
#include "../Common/GlobalFunction.h"
#include <math.h>
#include "../GameScene/GameResourceManager.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameDialog/GameInteractiveDlg.h"
#include "../GameDialog/GameSkillDlg.h"
#include "../Net/ClientSocket.h"
#include "../Common/TimeCounter.h"
#include "../GameDialog/GameChatDlg.h"
#include "../../CommonModule/GamePacket.h"
#include "../GameScene/GameNPC.h"
#include "../BackMir/BackMir.h"
#include "../GameDialog/GameHelpDlg.h"
#include "../GameDialog/GameChatDlg.h"
#include "../GameDialog/GameQuitDialog.h"
#include "../GameDialog/GameDecomposeDlg.h"
#include "../GameDialog/GameQuestDlg.h"
#include "../GameDialog/GameShopDlg2.h"
#include "../GameDialog/GameStoreDlg2.h"
#include "../GameDialog/GameRoleInfoDlg.h"
#include "../GameDialog/GameInputDlg.h"
#include "../GameDialog/GameDisplayDlg.h"
#include "../GameDialog/OfflineSellDlg.h"
#include "../GameDialog/GameRankDlg.h"
#include "../Common/Bmp2Jpg.h"
#include "../Common/OutlineTextureManager.h"
#include <string.h>
#include "../Common/gfx_utils.h"
#include "../BackMir/GlobalLuaConfig.h"
#include "../../CommonModule/DataEncryptor.h"

HGE* GameScene::s_hge = NULL;
GameScene* GameScene::sThis = NULL;

#ifdef _USE_SCRIPT
using namespace LuaPlus;
#endif

//////////////////////////////////////////////////////////////////////////
extern ByteBuffer g_xBuffer;

/*
#ifdef _DEBUG
const char* g_szResFile[] = 
{
	"Tiles.wil",
	"SmTiles.wil",
	"Hum.wil",
	"Objects.wil",
	"Objects2.wil",
	"Objects3.wil",
	"Objects4.wil",
	"Objects5.wil",
	"Objects6.wil",
	"Objects7.wil",
	"Objects8.wil",
	"Objects9.wil",
	"Objects10.sib",
	"Objects11.wil",
	"Objects12.wil",
	"Objects13.sib",
	"Hair.wil",
	"Weapon.sib",
	//"Prguse.wil",
	//"Prguse2.wil",
	"OpUI.sib",
	"Items.wil",
	"DnItems.wil",
	"Mon1.wil",
	"Mon2.sib",	//	just for evil dragon
	"Mon3.wil",
	"Mon4.wil",
	"Mon5.wil",
	"Mon6.wil",
	"Mon7.wil",
	"Mon8.wil",
	"Mon9.wil",
	"Mon10.wil",
	"Mon11.wil",
	"Mon12.wil",
	"Mon13.wil",
	"Mon14.wil",
	"Mon15.wil",
	"Mon16.wil",
	"Mon17.wil",
	"Mon18.wil",
	"Mon19.sib",
	"Mon20.wil",
	"Mon21.wil",
	"Mon22.wil",
	"Mon23.sib",
	"Mon24.wil",
	"Mon25.wil",
	"Mon26.sib",
	"Mon27.wil",
	"Mon28.wil",
	"Mon29.sib",
	"Mon30.wil",
	"Mon31.sib",
	"Mon32.sib",
	"Mon33.sib",
	"mmap.sib",
	"bmap.sib",
	"Magic.wil",
	"Magic2.sib",
	"Magic3.wil",
	"Magic4.wil",
	"Magic5.wil",
	"Magic6.wil",
	"Magic7.wil",
	"Magic8.wil",
	"Magic9.wil",
	"Magic10.wil",
	"npc.sib",
	"StateItem.sib",
	"HumEffect.wil",
	"MagicIcon.sib"
	//"ChrSel.wil"
};
#else
const char* g_szResFile[] = 
{
	"Tiles.sib",
	"SmTiles.sib",
	"Hum.sib",
	"Objects.sib",
	"Objects2.sib",
	"Objects3.sib",
	"Objects4.sib",
	"Objects5.sib",
	"Objects6.sib",
	"Objects7.sib",
	"Objects8.sib",
	"Objects9.sib",
	"Objects10.sib",
	"Objects11.sib",
	"Objects12.sib",
	"Objects13.sib",
	"Hair.sib",
	"Weapon.sib",
	//"Prguse.wil",
	//"Prguse2.wil",
	"OpUI.sib",
	"Items.sib",
	"DnItems.sib",
	"Mon1.sib",
	"Mon2.sib",		//	just for evil dragon
	"Mon3.sib",
	"Mon4.sib",
	"Mon5.sib",
	"Mon6.sib",
	"Mon7.sib",
	"Mon8.sib",
	"Mon9.sib",
	"Mon10.sib",
	"Mon11.sib",
	"Mon12.sib",
	"Mon13.sib",
	"Mon14.sib",
	"Mon15.sib",
	"Mon16.sib",
	"Mon17.sib",
	"Mon18.sib",
	"Mon19.sib",
	"Mon20.sib",
	"Mon21.sib",
	"Mon22.sib",
	"Mon23.sib",
	"Mon24.sib",
	"Mon25.sib",
	"Mon26.sib",
	"Mon27.sib",
	"Mon28.sib",
	"Mon29.sib",
	"Mon30.sib",
	"Mon31.sib",
	"Mon32.sib",
	"Mon33.sib",
	"mmap.sib",
	"bmap.sib",
	"Magic.sib",
	"Magic2.sib",
	"Magic3.sib",
	"Magic4.sib",
	"Magic5.sib",
	"Magic6.sib",
	"Magic7.sib",
	"Magic8.sib",
	"Magic9.sib",
	"Magic10.sib",
	"npc.sib",
	"StateItem.sib",
	"HumEffect.sib",
	"MagicIcon.sib"
	//"ChrSel.wil"
};
#endif*/

static const char* g_MapTable[] = 
{
	//	0 新手村
	"XSC"
};

GameScene::GameScene()
{
	m_bShowDonate = true;
	m_pPlayer = NULL;
	m_pMainOpt = NULL;
	m_pMiniMap = NULL;
	m_pMsgBoard = NULL;
	m_pStatusDlg = NULL;
	m_pFloatDlg = NULL;
	assert(sThis == NULL);
	m_pTarget = NULL;
	sThis = this;

	m_nMouseCoordY = m_nMouseCoordX = 0;

	for(int i = 0; i < MEFF_USERTOTAL; ++i)
	{
		m_dwMgcCostTable[i] = 0;
		m_dwMgcDelayTable[i] = 0;
	}
	m_dwLastDetectTime = 0;

	m_dwLastSendSequence = 0;
	m_bReceiveReply = true;
	m_dwDelaySendTime = 0;
	m_dwNetDelayMillSeconds = 0;
	m_dwLastShowGmMsgTime = 0;

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);

	m_xRecentChatIterator = m_xRecentChat.end();
	m_bInit = false;
}

GameScene::~GameScene()
{
	//delete m_pPlayer;
	delete m_pMainOpt;
	delete m_pMiniMap;
	SAFE_DELETE(m_pFloatDlg);
	SAFE_DELETE(m_pStatusDlg);
	//delete m_pMsgBoard;
}

/************************************************************************/
/* 初始化
/************************************************************************/
bool GameScene::Init(HGE* _hge)
{
	if(m_bInit){return true;}

 	AfxGetHge()->Random_Seed();
 	ZeroMemory(m_stMapObjects, sizeof(m_stMapObjects));
 	//	负责初始化所有资源
 	s_hge = _hge;
 	char szSrcBuf[MAX_PATH];
 	char szFile[MAX_PATH];
 	GetRootPath(szSrcBuf, MAX_PATH);
	//	初始化人物数据
 	m_pPlayer = GamePlayer::GetInstance();
 	AfxInitPlayer(m_pPlayer);
	
	//	下挂玩家 自动更新与绘制
	InsertObject(m_pPlayer);
	//	下挂地图
	InsertObject(GameMapManager::GetInstance());

	//	初始化GameTextureManager
	HWND hWnd = _hge->System_GetState(HGE_HWND);
	//HDC hdc = ::GetDC(hWnd);
	//GameTextureManager::Init(hdc, _hge);
	//::ReleaseDC(hWnd, hdc);

 	/*for(int i = 0; i < RES_TOTAL; ++i)
 	{
 		sprintf(szFile, "%sData\\%s", szSrcBuf, g_szResFile[i]);
 		if(!GameResourceManager::GetInstance()->LoadFile(szFile))
 		{
 			_hge->System_Log("加载资源失败 : %s", szFile);
 			//return false;
 		}
 		::PostMessage(hWnd, WM_SCENE_PROG, RES_TOTAL - 1, i);
 	}
*/

	//	初始化小地图
	m_pMiniMap = new GameMiniMapDlg;
	InsertObject(m_pMiniMap);

	//	初始化状态栏
	m_pStatusDlg = new GameStatusDlg;
	InsertObject(m_pStatusDlg);

	/*m_pStatusDlg->AddStatus(200, 14 * 1000);
	m_pStatusDlg->AddStatus(1, 25 * 1000);
	m_pStatusDlg->AddStatus(3, 60 * 1000);*/

	//	魔法
	//InsertObject(&GamePlayer::GetInstance()->m_mgcs);

	//	初始化操作栏
	m_pMainOpt = GameMainOptUI::GetInstance();
	m_pMainOpt->Init();
	InsertObject(m_pMainOpt);

	//	初始化悬浮消息栏
	m_pFloatDlg = new GameFloatDlg;
	InsertObject(m_pFloatDlg);

	//	初始化信息输出区
	m_pMsgBoard = GameInfoBoardDlg::GetInstance();
	InsertObject(m_pMsgBoard);

	MagicElement::pEffectRender->SetBlendMode(BLEND_ALPHAADD | BLEND_NOZWRITE);

	//::SendMessage(hWnd, WM_ENTER_GAMESCENE, 0, 0);
	m_bInit = true;
	return true;
}

/************************************************************************/
/* 切换地图场景 
/************************************************************************/
bool GameScene::SwitchScene(const char* lpszmapname)
{
	//GameResourceManager::GetInstance()->ReleaseAllFile();
	bool bSuc = GameMapManager::GetInstance()->LoadMapReadOnly(lpszmapname);

	if(!bSuc)
	{
		AfxGetHge()->System_Log("载入地图[%s]失败", lpszmapname);
		return false;
	}
	else
	{
		AfxGetHge()->System_Log("载入地图[%s]成功", lpszmapname);
	}
	
	//m_pMiniMap->SetMap(lpszmapname);
	//GameMapManager::GetInstance()->RedrawBuffer();
	m_pTarget = NULL;
	ZeroMemory(m_stMapObjects, sizeof(m_stMapObjects));

	MAGICLIST::const_iterator mbegiter = m_xMagics.begin();
	MAGICLIST::const_iterator menditer = m_xMagics.end();
	for(mbegiter; mbegiter != menditer; ++mbegiter)
	{
		delete *mbegiter;
	}
	m_xMagics.clear();

	mbegiter = m_xAttackNumber.begin();
	menditer = m_xAttackNumber.end();
	for(mbegiter; mbegiter != menditer; ++mbegiter)
	{
		delete *mbegiter;
	}
	m_xAttackNumber.clear();

	PLAYERLIST::const_iterator pbegiter = m_lstPlayers.begin();
	PLAYERLIST::const_iterator penditer = m_lstPlayers.end();
	for(pbegiter; pbegiter != penditer; ++pbegiter)
	{
		SocketDataCenter::GetInstance().UnregisterObject((*pbegiter)->GetHandlerID());
		delete *pbegiter;
	}
	m_lstPlayers.clear();

	GROUNDITEMS::const_iterator ibegiter = m_lstGroundItems.begin();
	for(ibegiter; ibegiter != m_lstGroundItems.end(); ++ibegiter)
	{
		delete *ibegiter;
	}
	m_lstGroundItems.clear();

	//	Clear the rect cache
	ClearRectCache();

	//	update player status
	GamePlayer::GetInstance()->UpdatePlayerAttrib();

	return bSuc;
}

/************************************************************************/
/* 切换地图场景 
/************************************************************************/
/*
bool GameScene::SwitchScene(DWORD _id)
{
	char szMapName[MAX_PATH];
	sprintf(szMapName, "%s\\cfg.ini",
		GetRootPath());
	const char* pszMapName = NULL;

	if(_id < 100)
	{
		pszMapName = pTheGame->GetRunMap(_id);
	}
	else
	{
		pszMapName = pTheGame->GetInstanceMap(_id);
	}

#ifdef _DEBUG
	int x = 0;
	int y = 0;
	x = GetPrivateProfileInt("STANDING",
		"x",
		99999,
		szMapName);
	y = GetPrivateProfileInt("STANDING",
		"y",
		99999,
		szMapName);
	if(x != 99999 &&
		y != 99999)
	{
		GamePlayer::GetInstance()->SetReallyCoord(x, y);
	}
#endif

	if(NULL == pszMapName)
	{
		AfxGetHge()->System_Log("不存在的地图ID[%d]",
			_id);
		return false;
	}
	else
	{
		AfxGetHge()->System_Log("载入地图ID[%d]",
			_id);
		//return true;
	}

	//	清除黑夜标记
	pTheGame->SetDarkMode(0);
	//	清楚地图显示标记
	pTheGame->SetShowMapSnap(true);

	//	触发场景更换事件
	LuaEvent_SwitchScene evt = {0};
	evt.nMapId = _id;
	pTheGame->GetScriptEngine()->DispatchEvent(kLuaEvent_SwitchScene, &evt);

	GameResourceManager::GetInstance()->ReleaseAllFile();
	GameSoundManager::GetInstancePtr()->ReleaseCache();
	SelectedTextureManager::GetInstance()->ClearAll();
	OutlineTextureManager::GetInstance()->ClearAll();

	if(m_pMiniMap)
	{
		m_pMiniMap->SetMap(_id);
		GamePlayer::GetInstance()->SetMapID(_id);
		m_pMainOpt->GetMapDlg()->Reset();
	}
	GamePlayer::GetInstance()->SetAtkMonster(NULL);
	GamePlayer::GetInstance()->SetMgcTarget(NULL);
	//m_pMainOpt->GetShopDlg()->SetVisible(false);
	//m_pMainOpt->GetStoreDlg()->SetVisible(false);
	GameScene::sThis->GetMainOpt()->GetShopDlg2()->CloseDialog();
	GameScene::sThis->GetMainOpt()->GetStoreDlg2()->CloseDialog();
	return SwitchScene(pszMapName);
}*/

bool GameScene::SwitchScene(DWORD _id)
{
	const LuaMapInfo* pMapInfo = GetMapConfigManager().GetLuaMapInfo(_id);
	if(NULL == pMapInfo)
	{
		AfxGetHge()->System_Log("不存在的地图ID[%d]",
			_id);
		return false;
	}
	else
	{
		AfxGetHge()->System_Log("载入地图ID[%d]",
			_id);
		//return true;
	}

	//	清除黑夜标记
	pTheGame->SetDarkMode(0);
	//	清楚地图显示标记
	pTheGame->SetShowMapSnap(true);

	//	触发场景更换事件
	LuaEvent_SwitchScene evt = {0};
	evt.nMapId = _id;
	pTheGame->GetScriptEngine()->DispatchEvent(kLuaEvent_SwitchScene, &evt);

	GameResourceManager::GetInstance()->ReleaseAllFile();
	GameSoundManager::GetInstancePtr()->ReleaseCache();
	SelectedTextureManager::GetInstance()->ClearAll();
	OutlineTextureManager::GetInstance()->ClearAll();

	if(m_pMiniMap)
	{
		m_pMiniMap->SetMap(_id);
		GamePlayer::GetInstance()->SetMapID(_id);
		m_pMainOpt->GetMapDlg()->Reset();
	}
	GamePlayer::GetInstance()->SetAtkMonster(NULL);
	GamePlayer::GetInstance()->SetMgcTarget(NULL);
	//m_pMainOpt->GetShopDlg()->SetVisible(false);
	//m_pMainOpt->GetStoreDlg()->SetVisible(false);
	GameScene::sThis->GetMainOpt()->GetShopDlg2()->CloseDialog();
	GameScene::sThis->GetMainOpt()->GetStoreDlg2()->CloseDialog();

	m_pMainOpt->SetMapName(pMapInfo->szMapChName);
	return SwitchScene(pMapInfo->szMapResFile);
}

bool GameScene::LoadProperty(int _id)
{
	return true;
}

/************************************************************************/
/* 处理按键消息
/************************************************************************/
void GameScene::ProcKeyCmd()
{
	if(m_pMainOpt->IsBlockCmd())
	{
		return;
	}

	bool bCtrlState = AfxGetHge()->Input_GetKeyState(HGEK_CTRL);
	bool bShiftState = AfxGetHge()->Input_GetKeyState(HGEK_SHIFT);

	/*
	*	测试
	*/
	if(m_pMainOpt->GetChatDlg()->GetEdit()->IsFocused())
	{
		//	输入框有焦点 不接受输入
		//	处理上下方向键 快捷输入历史记录
		if(!m_xRecentChat.empty())
		{
			ProcKeyCmd_ChatRecord();
		}
		return;
	}

#ifdef _DEBUG
	if(AfxGetHge()->Input_KeyUp(HGEK_Q) &&
		bCtrlState)
	{
		if(pTheGame->LoadQuestScript())
		{
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_WHITE, "重新载入任务提示脚本成功");
		}
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_W) &&
		bCtrlState)
	{
		if(pTheGame->LoadScript(GamePlayer::GetInstance()->GetMapID()))
		{
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_WHITE, "重新载入任务对话脚本成功");
		}
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_F) &&
		bCtrlState)
	{
		char szMsg[MAX_PATH] = {0};
		sprintf(szMsg, "你好，游戏公告啊！！123 XXX fe9s 这是第X条公告。。悬浮");
		m_pFloatDlg->AddFloatMessage(szMsg, ARGB_WHITE, 3);
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_K) &&
		bCtrlState)
	{
		pTheGame->RequestSmallQuit();
	}
#endif

	if(AfxGetHge()->Input_KeyUp(HGEK_H) &&
		bCtrlState)
	{
		PkgPlayerSpeOperateReq opt;
		opt.dwOp = CMD_OP_CHANGEATTACKMODE;
		g_xBuffer.Reset();
		g_xBuffer << opt;
		SendBufferToGS(g_xBuffer);
	}

	if(AfxGetHge()->Input_KeyUp(HGEK_SPACE)/* ||
		AfxGetHge()->Input_KeyUp(192)*/)
	{
		PkgPlayerPickUpItemReq req;

		if(!m_pPlayer->IsMoving())
		{
			WORD wPosX = m_pPlayer->GetCoordX();
			WORD wPosY = m_pPlayer->GetCoordY();
			for(std::list<GroundItem*>::const_iterator iter = m_lstGroundItems.begin();
				iter != m_lstGroundItems.end();
				++iter)
			{
				if(wPosX == (*iter)->wPosX &&
					wPosY == (*iter)->wPosY)
				{
					req.dwTag = (*iter)->stAttrib.tag;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
					break;
				}
			}
		}
	}
	bool bIsVisible = false;
	bool bPreVisible = false;

	DWORD dwCheckedKey = HGEK_F9;
	DWORD dwMappedKey = pTheGame->GetAssistPaneWnd()->CheckMappedKey(dwCheckedKey);
	if(dwMappedKey != 0)
	{
		dwCheckedKey = dwMappedKey;
	}

 	if(AfxGetHge()->Input_KeyUp(dwCheckedKey))
	{
		//if(bCtrlState)
		{
			bPreVisible = m_pMainOpt->GetBagDlg2()->IsVisible();
			if(bPreVisible)
			{
				m_pMainOpt->GetBagDlg2()->CloseDialog();
			}
			else
			{
				m_pMainOpt->GetBagDlg2()->ShowDialog();
			}
		}
 	}

	dwCheckedKey = HGEK_F10;
	dwMappedKey = pTheGame->GetAssistPaneWnd()->CheckMappedKey(dwCheckedKey);
	if(dwMappedKey != 0)
	{
		dwCheckedKey = dwMappedKey;
	}

	if(AfxGetHge()->Input_KeyUp(dwCheckedKey))
	{
		//m_pMainOpt->GetHumDlg()->SetVisible(!m_pMainOpt->GetHumDlg()->IsVisible());
		//m_pMainOpt->GetHumDlg()->SetDlgPage(GameHumDlg::PAGE_HUM);
		bPreVisible = GET_DLG(HumDlg2)->IsVisible();
		if(bPreVisible)
		{
			GET_DLG(HumDlg2)->CloseDialog();
		}
		else
		{
			GET_DLG(HumDlg2)->ShowDialog();
		}
		//bIsVisible = m_pMainOpt->GetHumDlg2()->IsVisible();
		//m_pMainOpt->GetHumDlg2()->SetVisible(!bIsVisible);
	}

	dwCheckedKey = HGEK_F11;
	dwMappedKey = pTheGame->GetAssistPaneWnd()->CheckMappedKey(dwCheckedKey);
	if(dwMappedKey != 0)
	{
		dwCheckedKey = dwMappedKey;
	}

	if(AfxGetHge()->Input_KeyUp(dwCheckedKey))
	{
		bPreVisible = m_pMainOpt->GetSkillDlg()->IsVisible();
		if(bPreVisible)
		{
			m_pMainOpt->GetSkillDlg()->CloseDialog();
		}
		else
		{
			m_pMainOpt->GetSkillDlg()->ShowDialog();
		}
	}

	if(AfxGetHge()->Input_KeyUp(HGEK_M))
	{
		bPreVisible = m_pMainOpt->GetDecomposeDlg()->IsVisible();
		if(bPreVisible)
		{
			m_pMainOpt->GetDecomposeDlg()->CloseDialog();
		}
		else
		{
			m_pMainOpt->GetDecomposeDlg()->ShowDialog();
		}
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_Q) &&
		!bCtrlState)
	{
		bPreVisible = m_pMainOpt->GetQuestDlg()->IsVisible();
		if(bPreVisible)
		{
			m_pMainOpt->GetQuestDlg()->CloseDialog();
		}
		else
		{
			m_pMainOpt->GetQuestDlg()->ShowDialog();
		}
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_S) && !bCtrlState)
	{
		bPreVisible = m_pMainOpt->GetOfflineSellListDlg()->IsVisible();
		if(bPreVisible)
		{
			m_pMainOpt->GetOfflineSellListDlg()->CloseDialog();
		}
		else
		{
			m_pMainOpt->GetOfflineSellListDlg()->ShowDialog();
		}
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_E))
	{
		GameSysMenuDlg* pDlg = GameMainOptUI::GetInstance()->GetSysMenuDlg();
		if(pDlg->IsVisible())
		{
			pDlg->SetVisible(false);
		}
		else
		{
			pDlg->SetVisible(true);
			pDlg->SetNeedTopLevel(true);
		}
	}

 	if(AfxGetHge()->Input_KeyUp(HGEK_ESCAPE))
 	{
 		m_pMainOpt->GetBagDlg2()->SetVisible(false);
		m_pMainOpt->GetBagDlg2()->CancelSel();
 		m_pMainOpt->GetHumDlg2()->CloseDialog();
		m_pMainOpt->GetSkillDlg()->SetVisible(false);
		m_pMainOpt->GetActiveDlg()->SetVisible(false);
		m_pMainOpt->GetMapDlg()->SetVisible(false);
		m_pMainOpt->GetRankDlg()->CloseDialog();
		GameMainOptUI::GetInstance()->GetRoleInfoDlg()->CloseDialog();
		m_pMainOpt->CloseAllDialog();
 	}
	bool bEditInFocus = GameMainOptUI::GetInstance()->IsEditInFocus();
	if(!bEditInFocus)
	{
		for(int i = HGEK_1; i <= HGEK_6; ++i)
		{
			if(AfxGetHge()->Input_KeyUp(i))
			{
				m_pPlayer->UseItemFromAssist(i - HGEK_1);
			}
		}
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_TAB))
	{
		//bIsVisible = m_pMainOpt->GetMapDlg()->IsVisible();
		bool bNeedLocate = !m_pMainOpt->GetMapDlg()->IsVisible();
		bPreVisible = m_pMainOpt->GetMapDlg()->IsVisible();

		if(bPreVisible)
		{
			m_pMainOpt->GetMapDlg()->CloseDialog();
		}
		else
		{
			m_pMainOpt->GetMapDlg()->ShowDialog();
		}
		//m_pMainOpt->GetMapDlg()->SetVisible(true);
		//if(!bIsVisible)
		if(bNeedLocate)
		{
			m_pMainOpt->GetMapDlg()->AutoLocate();
		}
	}
	/*else
	{
		if(m_pMainOpt->GetMapDlg()->IsVisible())
		{
			m_pMainOpt->GetMapDlg()->SetVisible(false);
		}
	}*/
	
	if(bCtrlState &&
		AfxGetHge()->Input_KeyUp(HGEK_A))
	{
		PkgPlayerSlaveStopReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
	}
	if(bCtrlState &&
		AfxGetHge()->Input_KeyUp(HGEK_Z))
	{
		PkgPlayerCallSlaveReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
	}
	if(bCtrlState &&
		AfxGetHge()->Input_KeyUp(HGEK_X))
	{
		PkgPlayerKillSlaveReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
	}
	if(bCtrlState &&
		AfxGetHge()->Input_KeyUp(HGEK_S))
	{
		char szBuf[MAX_PATH];
		SYSTEMTIME st;
		GetLocalTime(&st);
		sprintf(szBuf, "%s\\snapshot\\%04d%02d%02d%02d%02d.bmp",
			GetRootPath(), st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		AfxGetHge()->System_Snapshot(szBuf);

		if(pTheGame->Config_JpgSnap())
		{
			//	to jpg
			char szJpgFile[MAX_PATH];
			sprintf(szJpgFile, "%s\\snapshot\\%04d%02d%02d%02d%02d.jpg",
				GetRootPath(), st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			if(BmpTrans2Jpg(szBuf, szJpgFile))
			{
				DeleteFile(szBuf);
				sprintf(szBuf, "已保存: snapshot\\%04d%02d%02d%02d%02d.jpg",
					st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				//m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(szBuf, ARGB_RED);
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, szBuf);
			}
		}
		else
		{
			sprintf(szBuf, "已保存: snapshot\\%04d%02d%02d%02d%02d.bmp",
				st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			//m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(szBuf, ARGB_RED);
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, szBuf);
		}
		
	}

	//	魔法快捷键
	if(!bEditInFocus)
	{
		int nMagicCount = GamePlayer::GetInstance()->GetMagicCount();
		for(int i = 0; i < nMagicCount; ++i)
		{
			const MagicDetail* pDetail = GamePlayer::GetInstance()->GetMagicByIndex(i);
			if(NULL != pDetail &&
				0 != pDetail->cAccKey)
			{
				if(AfxGetHge()->Input_KeyUp(pDetail->cAccKey))
				{
					GamePlayer::GetInstance()->SetAccMagicID(pDetail->wID);
					break;
				}
			}
		}
	}
}

void GameScene::ProcKeyCmd_ChatRecord()
{
	if(AfxGetHge()->Input_KeyUp(HGEK_UP))
	{
		//	prev message record
		if(m_xRecentChatIterator != m_xRecentChat.begin())
		{
			--m_xRecentChatIterator;
		}

		string& refStringItem = *m_xRecentChatIterator;
		m_pMainOpt->GetChatDlg()->GetEdit()->ClearText();
		m_pMainOpt->GetChatDlg()->GetEdit()->SetText(refStringItem.c_str());
	}
	else if(AfxGetHge()->Input_KeyUp(HGEK_DOWN))
	{
		//	next message record
		if(m_xRecentChatIterator == m_xRecentChat.end())
		{
			string& refStringItem = *(--m_xRecentChatIterator);
			m_pMainOpt->GetChatDlg()->GetEdit()->ClearText();
			m_pMainOpt->GetChatDlg()->GetEdit()->SetText(refStringItem.c_str());

			return;
		}

		++m_xRecentChatIterator;
		if(m_xRecentChatIterator == m_xRecentChat.end())
		{
			string& refStringItem = *(--m_xRecentChatIterator);
			m_pMainOpt->GetChatDlg()->GetEdit()->ClearText();
			m_pMainOpt->GetChatDlg()->GetEdit()->SetText(refStringItem.c_str());

			return;
		}

		string& refStringItem = *m_xRecentChatIterator;
		m_pMainOpt->GetChatDlg()->GetEdit()->ClearText();
		m_pMainOpt->GetChatDlg()->GetEdit()->SetText(refStringItem.c_str());
	}
}

bool GameScene::ProcUserCmd(const POINT& _mp)
{
	//	客户区外不接受指令
	if(!IsInClient())
	{
		return false;
	}

	float dt = AfxGetHge()->Timer_GetDelta();
	POINT ptMouse = _mp;
	
	if(AfxGetHge()->Input_KeyUp(HGEK_ENTER))
	{
		if(!m_pMainOpt->GetChatDlg()->GetEdit()->IsFocused())
		{
			m_pMainOpt->GetChatDlg()->GetEdit()->GainFocus();
			m_pMainOpt->GetChatDlg()->GetEdit()->SetVisible(true);
		}
		else
		{
			//	if focus
			m_pMainOpt->GetChatDlg()->GetEdit()->SetVisible(true);
		}
		/*else
		{
			m_pMainOpt->GetChatDlg()->GetEdit()->LoseFocus();
			m_pMainOpt->GetChatDlg()->GetEdit()->SetVisible(false);
		}*/
	}

	m_pMainOpt->GetIntroDlg()->ShowNothing();
	m_pMainOpt->GetDisplayDlg()->CloseDialog();
	m_pMainOpt->GetDisplayDlg()->ProcUserCmd(_mp);
	if(m_pMainOpt->GetDlgControl()->ProcUserCmd(_mp))
	{
		//	窗口消息处理了 不发往游戏操作
		return true;
	}
	if(ProcChatClick(_mp))
	{
		return true;
	}
	m_pMainOpt->GetIntroDlg()->SetVisible(false);
	/*if(m_pMainOpt->IsPtInObject(ptMouse))
	{
		return m_pMainOpt->ProcUserCmd(_mp);
	}*/
	if(m_pMainOpt->ProcUserCmd(_mp))
	{
		//	
		return true;
	}

	if(m_pMainOpt->GetBagDlg2()->IsItemSel())
	{
		//	游戏区域 有选择
		return ProcItemClick(ptMouse);
	}

	//	状态窗口处理了
	if(m_pStatusDlg->ProcUserCmd(_mp))
	{
		return true;
	}

	//	没有处理 隐藏掉玩家信息对话框
	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON) ||
		AfxGetHge()->Input_KeyUp(HGEK_RBUTTON))
	{
		GameMainOptUI::GetInstance()->GetRoleInfoDlg()->CloseDialog();
	}

	ProcGameCmd(_mp);
	return true;
}

bool GameScene::ProcItemClick(const POINT& _ptMouse)
{
	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		//GameMapManager::GetInstance()->AddFloorItem(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY(), m_pMainOpt->GetBagDlg()->GetSelItem());
		//GroundItem item;
		//item.wPosX = (WORD)m_pPlayer->GetCoordX();
		//item.wPosY = (WORD)m_pPlayer->GetCoordY();
		//memcpy(&item.stAttrib, m_pMainOpt->GetBagDlg()->GetSelItem(), sizeof(ItemAttrib));
		//InsertNewGroundItem(&item);
#ifdef _NET_GAME_

		//PkgPlayerDropItemReq req;
		ItemAttrib* pSelItem = m_pMainOpt->GetBagDlg2()->GetSelItem();
		//req.dwId = pSelItem->tag;
		//req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		//g_xBuffer.Reset();
		//g_xBuffer << req;
		//SendBuffer(&g_xBuffer);
		if(pSelItem->type != ITEM_NO)
		{
			if(TEST_FLAG_BOOL(pSelItem->atkPois, POIS_MASK_BIND))
			{
				m_pMainOpt->GetStaticDlg()->CreateDestoryItemDlg(pSelItem->tag);
			}
			else
			{
				if(pSelItem->type == ITEM_COST &&
					pSelItem->atkSpeed > 1)
				{
					GameMainOptUI::GetInstance()->GetInputDlg()->ShowAsMode(InputDlg_QueryDropSum, pSelItem->tag, pSelItem->atkSpeed);
				}
				else
				{
					PkgPlayerDropItemReq req;
					req.dwId = pSelItem->tag;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
			}
		}
#endif
		//ZeroMemory(m_pMainOpt->GetBagDlg()->GetSelItem(), sizeof(ItemAttrib));
		m_pMainOpt->GetBagDlg2()->CancelSel();
	}
	return true;
}

bool GameScene::ProcChatClick(const POINT& _ptMouse)
{
	//	重置聊天窗口状态
	{
		m_pMainOpt->GetChatDlg()->GetHistoryDlg()->SetDlgFocus(false);
		m_pMainOpt->GetChatDlg()->GetEdit()->Update(AfxGetHge()->Timer_GetDelta());
	}

	GameChatDlg* pChatDlg = m_pMainOpt->GetChatDlg();
	m_pMainOpt->GetChatDlg()->ResetStatus();

	//	On new version, the implement of this function has moved to <GameMainOptUI>
	return false;

	RECT rcTest;

	rcTest.left = /*255 - 23*/0;
	rcTest.right = rcTest.left + 23;
	rcTest.top = pChatDlg->GetEdit()->GetRect().top /*- 6*/+20;
	rcTest.bottom = rcTest.top + 24;

	if(PtInRect(&rcTest, _ptMouse))
	{
		pChatDlg->SetBtnState(3, BTN_STATE_OVER);
		if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
		{
			pChatDlg->SetBtnState(3, BTN_STATE_DOWN);
		}
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			pChatDlg->SetBtnState(3, BTN_STATE_OVER);
			pChatDlg->SetHide(!pChatDlg->IsHide());
		}
		return true;
	}

	rcTest.left = 0;
	rcTest.right = 35;
	rcTest.top = pChatDlg->GetHistoryDlg()->GetRect().top;
	rcTest.bottom = pChatDlg->GetEdit()->GetRect().top - 5 + 20;
	if(!PtInRect(&rcTest, _ptMouse))
	{
		return false;
	}

	if(pChatDlg->IsHide())
	{
		rcTest.left = 0;
		rcTest.right = 35;
		rcTest.top = pChatDlg->GetEdit()->GetRect().top - 5;
		rcTest.bottom = rcTest.top + 20;

		if(PtInRect(&rcTest, _ptMouse))
		{
			pChatDlg->SetBtnState(0, BTN_STATE_OVER);
			if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(0, BTN_STATE_DOWN);
			}
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(0, BTN_STATE_OVER);
				//	send
				SendChatMessage();
			}
			return true;
		}

		rcTest.left = 1;
		rcTest.right = rcTest.left + 15;
		rcTest.top = pChatDlg->GetHistoryDlg()->GetRect().top + 130 - 33;
		rcTest.bottom = rcTest.top + 14;

		if(PtInRect(&rcTest, _ptMouse))
		{
			pChatDlg->SetBtnState(1, BTN_STATE_OVER);
			if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(1, BTN_STATE_DOWN);
			}
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(1, BTN_STATE_OVER);
				pChatDlg->GetHistoryDlg()->ScrollUp();
			}
			return true;
		}

		rcTest.left = 1;
		rcTest.right = rcTest.left + 15;
		rcTest.top = pChatDlg->GetHistoryDlg()->GetRect().top + 114;
		rcTest.bottom = rcTest.top + 14;

		if(PtInRect(&rcTest, _ptMouse))
		{
			pChatDlg->SetBtnState(2, BTN_STATE_OVER);
			if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(2, BTN_STATE_DOWN);
			}
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(2, BTN_STATE_OVER);
				pChatDlg->GetHistoryDlg()->ScrollDown();
			}
			return true;
		}

		rcTest.left = 1;
		rcTest.right = rcTest.left + 15;
		rcTest.top = pChatDlg->GetHistoryDlg()->GetRect().top + 114 + 17;
		rcTest.bottom = rcTest.top + 14;

		if(PtInRect(&rcTest, _ptMouse))
		{
			pChatDlg->SetBtnState(2, BTN_STATE_OVER);
			if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(2, BTN_STATE_DOWN);
			}
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(2, BTN_STATE_OVER);
				pChatDlg->GetHistoryDlg()->ScrollDown();
			}
			return true;
		}
	}
	else
	{
		rcTest.left = 0;
		rcTest.right = 35;
		rcTest.top = pChatDlg->GetEdit()->GetRect().top - 5;
		rcTest.bottom = rcTest.top + 20;

		if(PtInRect(&rcTest, _ptMouse))
		{
			pChatDlg->SetBtnState(0, BTN_STATE_OVER);
			if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(0, BTN_STATE_DOWN);
			}
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(0, BTN_STATE_OVER);
				//	send
				SendChatMessage();
			}
			return true;
		}

		rcTest.left = 1;
		rcTest.right = rcTest.left + 15;
		rcTest.top = pChatDlg->GetHistoryDlg()->GetRect().top;
		rcTest.bottom = rcTest.top + 14;

		if(PtInRect(&rcTest, _ptMouse))
		{
			pChatDlg->SetBtnState(1, BTN_STATE_OVER);
			if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(1, BTN_STATE_DOWN);
			}
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(1, BTN_STATE_OVER);
				pChatDlg->GetHistoryDlg()->ScrollUp();
			}
			return true;
		}

		rcTest.left = 1;
		rcTest.right = rcTest.left + 15;
		rcTest.top = pChatDlg->GetHistoryDlg()->GetRect().top + 114;
		rcTest.bottom = rcTest.top + 14;

		if(PtInRect(&rcTest, _ptMouse))
		{
			pChatDlg->SetBtnState(2, BTN_STATE_OVER);
			if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(2, BTN_STATE_DOWN);
			}
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				pChatDlg->SetBtnState(2, BTN_STATE_OVER);
				pChatDlg->GetHistoryDlg()->ScrollDown();
			}
			return true;
		}
	}

	return false;
}

void GameScene::ProcGameCmd(const POINT& _mp)
{
	//	下面为游戏区域的处理
	//	取消辅助栏的选择
	m_pMainOpt->CancelSel();

	//	死亡 不处理消息
	if(GamePlayer::GetInstance()->GetStatus() == PST_DEAD)
	{
		return;
	}

	//GameObject* pObj = NULL;
	//pObj = FindGameMonsterByArea(_mp.x, _mp.y);
	m_pTarget = FindGameMonsterByArea(_mp.x, _mp.y);

	/*if(m_pPlayer->HandleMagicInput(_mp))
	{
		return;
	}*/
	if(m_pPlayer->HandleNoneSpellMagic(_mp))
	{
		return;
	}

	if(m_pPlayer->GetStatus() == PST_ATTACKWEAPON ||
		m_pPlayer->GetStatus() == PST_ATTACKNOWEAPON ||
		m_pPlayer->GetStatus() == PST_ATTACKED ||
		m_pPlayer->GetStatus() == PST_DEAD ||
		m_pPlayer->GetStatus() == PST_SK_LEIDIANSHU)
	{
		//	攻击中不允许移动
		return;
	}

	if(m_pPlayer->IsLocked())
	{
		return;
	}

	//POINT pt1 = {VIEW_WIDTH / 2, VIEW_HEIGHT / 2};
	//PLAYER_DIRECTION pd = GetAngle(pt1, _mp);

	//	处理用户输入
	m_pPlayer->ProcUserCmd(_mp);
}

/************************************************************************/
/* 寻找鼠标点处的物体
/************************************************************************/
GameObject* GameScene::FindGameMonsterByArea(float _mx, float _my)
{
	//	看选取的怪物
	//{
		// 用于探测鼠标点的怪物
		GameObject* pObj = NULL;
		const MemMapObject* pObjs = NULL;
// 		int nMouseX = (VIEW_WIDTH - 1) / 2;
// 		int nMouseY = (VIEW_HEIGHT - 1) / 2;
 		bool bFindMonster = false;
// 		nMouseX = _mx - nMouseX;
// 		nMouseY = _my - nMouseY;
// 		nMouseX = ((int)m_pPlayer->GetObjx() + nMouseX) / UNIT_WIDTH;
// 		nMouseY = ((int)m_pPlayer->GetObjy() + nMouseY) / UNIT_HEIGHT;
// 		POINT ptMouse = {_mx, _my};
		POINT ptMouse;
		POINT ptSrc = {_mx, _my};
		MousePtToScreenPt(ptSrc, ptMouse);
		int nDetectedY = ptMouse.y / UNIT_HEIGHT;
		int nDetectedX = ptMouse.x / UNIT_WIDTH;
		ptMouse.x = _mx;
		ptMouse.y = _my;

// 		{
// 			static float fDebug = 0.0f;
// 			fDebug += AfxGetHge()->Timer_GetDelta();
// 			if(fDebug > 1.0f)
// 			{
// 				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_YELLOW, "%d,%d", nDetectedX, nDetectedY);
// 				fDebug = 0.0f;
// 			}
// 		}

		//for(int i = /*nMouseY*/nDetectedY - MOUSE_SEARCH_Y; i <= /*nMouseY*/nDetectedY + MOUSE_SEARCH_Y; ++i)
		for(int i = /*nMouseY*/nDetectedY + MOUSE_SEARCH_Y; i >= /*nMouseY*/nDetectedY - MOUSE_SEARCH_Y; --i)
		{
			if(bFindMonster)
			{
				break;
			}
			//for(int j = /*nMouseX*/nDetectedX - MOUSE_SEARCH_X; j <= /*nMouseX*/nDetectedX + MOUSE_SEARCH_X; ++j)
			for(int j = /*nMouseX*/nDetectedX + MOUSE_SEARCH_X; j >= /*nMouseX*/nDetectedX - MOUSE_SEARCH_X; --j)
			{
				if(bFindMonster)
				{
					break;
				}
				/*pObj = GetMonster(j, i);
				if(pObj)
				{
					if(pObj->IsSelected(ptMouse) &&
						pObj->GetStatus() != PST_DEAD)
					{
						bFindMonster = true;
					}
				}*/
				pObjs = GetMappedObjectsRelative(j, i);
				if(pObjs)
				{
					for(int k = 0; k < MAX_CELL_OBJECTS; ++k)
					{
						pObj = pObjs->pObjects[k];
						if(NULL != pObj)
						{
							if(pObj->GetStatus() != PST_DEAD)
							{
								if(pObj->IsSelected(ptMouse))
								{
									bFindMonster = true;
								}
							}
						}
						if(bFindMonster)
						{
							break;
						}
					}
				}
			}
		}
	//}

	if(bFindMonster)
	{
		return pObj;
	}
	else
	{
		return NULL;
	}
}

void GameScene::ProcMagicCmd(GameObject* _obj)
{
	
}

/************************************************************************/
/* 把窗口提到顶层
/************************************************************************/
void GameScene::BringDlgToTop(RenderObject* _obj)
{
	m_pMainOpt->GetDlgControl()->BringToTop(_obj);
}

/************************************************************************/
/* 是否在游戏窗体内
/************************************************************************/
bool GameScene::IsInClient()
{
	if(::GetForegroundWindow() != AfxGetHge()->System_GetState(HGE_HWND))
	{
		return false;
	}
	return true;
}

/************************************************************************/
/* 鼠标坐标转为屏幕坐标
/************************************************************************/
void GameScene::MousePtToScreenPt(const POINT& _src, POINT& _dest)
{
	//	屏幕中心点的坐标是与玩家坐标对应的
	int nOffsetX = _src.x - CENTRAL_X;
	int nOffsetY = _src.y - CENTRAL_Y;
	nOffsetX += (int)m_pPlayer->GetObjx();
	nOffsetY += (int)m_pPlayer->GetObjy();
	_dest.x = nOffsetX;
	_dest.y = nOffsetY;
}

void GameScene::Render()
{
	int nObjRendered = 0;
//	__try
	{
		for(OBJECTLIST::const_iterator iter = m_objList.begin(); iter != m_objList.end(); ++iter)
		{
			RenderObject* pRenderObj = *iter;

			if((*iter)->IsVisible())
			{
				//xCounter.Start();
				if((*iter) == GamePlayer::GetInstance())
				{
					continue;
				}
				(*iter)->Render();
				++nObjRendered;

				if((*iter) == GameMapManager::GetInstance())
				{
					//	地图画好后 画魔法
					//RenderMagic();
				}
			}
		}
	}

	//	背包的选取
	GameBagDlg2* pBagDlg = GameMainOptUI::GetInstance()->GetBagDlg2();
	const ItemAttrib* pSelItem = pBagDlg->GetSelItem();
	if(pBagDlg->IsVisible() &&
		pBagDlg->IsItemSel() &&
		NULL != pSelItem)
	{
		GameTextureManager* pRes = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
		HTEXTURE tex = pRes->GetTexture(pSelItem->tex);

		if(tex)
		{
			float x,y;
			AfxGetHge()->Input_GetMousePos(&x, &y);
			//SETSPRITE(tex, m_pSpr);
			// 			nDrawOffsetX = pRes->GetTextureWidth(tex);
			// 			nDrawOffsetY = pRes->GetTextureHeight(tex);
			int nDrawOffsetX = pRes->GetTextureWidth(pSelItem->tex);
			int nDrawOffsetY = pRes->GetTextureHeight(pSelItem->tex);
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, nDrawOffsetX, nDrawOffsetY);
			nDrawOffsetX /= 2;
			nDrawOffsetY /= 2;
			m_pRender->Render(x - nDrawOffsetX, y - nDrawOffsetY);
		}
	}

	//	GM公告
	if(!m_xGmMsg.empty())
	{
		if(GetTickCount() - m_dwLastShowGmMsgTime > 8000)
		{
			m_xGmMsg.clear();
			m_dwLastShowGmMsgTime = 0;
		}
		else
		{
			AfxGetGmFont()->SetColor(ARGB_YELLOW);
			AfxGetGmFont()->Print(m_rcGmMsg.left, m_rcGmMsg.top, "%s", m_xGmMsg.c_str());
		}
	}

	//	鼠标
	float fMx, fMy = 0;
	AfxGetHge()->Input_GetMousePos(&fMx, &fMy);
	if(GamePlayer::GetInstance()->GetStatus() != PST_DEAD &&
		AfxGetHge()->Input_GetKeyState(HGEK_CTRL))
	{
		//	attack arrow
		pTheGame->SetGameCursor(41);
	}
	else
	{
		pTheGame->SetGameCursor(42);
	}

	//Gfx_RenderChatBack(m_pRender, 310, 20, 20, 20, 100, 50);

// 	__except(ProcExcept(1))
// 	{
// 
// 	}
}

int GameScene::ProcExcept(int nExpCode)
{
	return 1;
}
//////////////////////////////////////////////////////////////////////////
bool GameScene::InsertNewMagic(MagicElement* _pMgc)
{
	/*if(_pMgc->GetStage() == MAGIC_STAGE_SELF &&
		_pMgc->GetCurFrame() == 0)
	{
		if(_pMgc->GetFire()->GetType() == OBJ_OTHERPLAYER ||
			_pMgc->GetFire()->GetType() == OBJ_PLAYER)
		{
			GameOtherPlayer* pPlayer = static_cast<GameOtherPlayer*>(_pMgc->GetFire());
			pPlayer->SetSelfEffect(_pMgc);
			return true;
		}
	}*/
	if(_pMgc->IsHumEff())
	{
		bool bUpdate = false;
		MagicElement* pMgc = NULL;
		MAGICLIST::const_iterator begIter = m_xMagics.begin();
		MAGICLIST::const_iterator endIter = m_xMagics.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			pMgc = *begIter;
			if(pMgc->GetMagicID() == _pMgc->GetMagicID() &&
				pMgc->IsHumEff() &&
				pMgc->GetFire() == _pMgc->GetFire())
			{
				//	Update...
				pMgc->SetCreateTime(GetTickCount());
				pMgc->SetAniTime(_pMgc->GetAniTime());
				pMgc->SetActive();

				//	Delete the previos one
				SAFE_DELETE(_pMgc);
				bUpdate = true;
				break;
			}
		}

		if(!bUpdate)
		{
			m_xMagics.push_back(_pMgc);
		}
	}
	else
	{
		m_xMagics.push_back(_pMgc);
	}
	
	return true;
}

/************************************************************************/
/* 更新下挂的RenderObject                                                      
/************************************************************************/
static bool s_bShowDonate = true;

void GameScene::Update(float _dt)
{
	//UpdateMapData(_dt);
	if(GetTickCount() - m_dwLastDetectTime > 60*1000)
	{
		if(pTheGame->IsTimerFunctionRepalaced())
		{
			PostQuitMessage(0);
			return;
		}
		m_dwLastDetectTime = GetTickCount();
	}
	static DWORD s_dwUpdateSoundTime = 0;
	if(GetTickCount() - s_dwUpdateSoundTime > 1000)
	{
		GameSoundManager::GetInstancePtr()->UpdateSound();
		s_dwUpdateSoundTime = GetTickCount();
	}

	static DWORD s_dwUpdateNetDelayTime = 0;
	if(GetTickCount() - s_dwUpdateNetDelayTime > 3000 &&
		m_bReceiveReply)
	{
		m_dwDelaySendTime = GetTickCount();
		m_bReceiveReply = false;

		PkgPlayerNetDelayReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		req.dwSendSequence = ++m_dwLastSendSequence;
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);

		s_dwUpdateNetDelayTime = GetTickCount();
		m_dwDelaySendTime = s_dwUpdateNetDelayTime;
	}

	UpdateMouseCoord();

	__super::Update(_dt);
}

//////////////////////////////////////////////////////////////////////////
void GameScene::OnReceiveNetDelayReply(const PkgPlayerNetDelayAck& ack)
{
	if(m_dwLastSendSequence == ack.dwSendSequence)
	{
		m_bReceiveReply = true;

		m_dwNetDelayMillSeconds = (GetTickCount() - m_dwDelaySendTime) / 2;
	}
}

/************************************************************************/
/* 处理地图物体（玩家 怪物 NPC更新） 更新映射表
/************************************************************************/
void GameScene::UpdateMapData(float _dt)
{
	ZeroMemory(m_stMapObjects, sizeof(m_stMapObjects));
	GameMapManager* pMap = GameMapManager::GetInstance();
	GameObject* pObj = NULL;

	float x, y;
	AfxGetHge()->Input_GetMousePos(&x, &y);
	POINT ptMouse = {x, y};

#define MAPOFT_X 0
#define MAPOFT_Y 0

	//int nPlayerNum = m_lstPlayers.size();
	if(!m_lstPlayers.empty())
	{
		//	更新物体信息
		PLAYERLIST::const_iterator endIter = m_lstPlayers.end();

		for(PLAYERLIST::const_iterator iter = m_lstPlayers.begin();
			iter != endIter;
			++iter)
		{
			pObj = *iter;
			if(pObj)
			{
				pObj->ProcUserCmd(ptMouse);
				//pObj->UpdateNetPacket();
				pObj->Update(_dt);

				//	同时映射入地图数据
				const RECT& rcMap = pMap->GetRect();
				if(pObj->GetCoordX() >= rcMap.left - MAPOFT_X &&
					pObj->GetCoordX() <= rcMap.right + MAPOFT_X &&
					pObj->GetCoordY() >= rcMap.top - MAPOFT_Y &&
					pObj->GetCoordY() <= rcMap.bottom + MAPOFT_Y)
				{
					//for(int i = 0; i < sizeof(m_stMapObjects) / sizeof(MemMapObject); ++i)
					for(int i = 0; i < MAX_CELL_OBJECTS; ++i)
					{
						if(m_stMapObjects[pObj->GetCoordX() - rcMap.left + MAPOFT_X][pObj->GetCoordY() - rcMap.top + MAPOFT_Y].pObjects[i] == NULL)
						{
							//	为空 加入
							m_stMapObjects[pObj->GetCoordX() - rcMap.left + MAPOFT_X][pObj->GetCoordY() - rcMap.top + MAPOFT_Y].pObjects[i] = pObj;
							break;
						}
					}
				}
			}
		}
	}

	GroundItem* pItem = NULL;
	WORD wPosX = 0;
	WORD wPosY = 0;
	int nItemNum = m_lstGroundItems.size();
	if(nItemNum > 0)
	{
		//	更新地面物体信息
		for(GROUNDITEMS::const_iterator iter = m_lstGroundItems.begin();
			iter != m_lstGroundItems.end();
			++iter)
		{
			pItem = *iter;
			if(pItem)
			{
				//	映射入地图数据
				const RECT& rcMap = pMap->GetRect();
				wPosX = pItem->wPosX;
				wPosY = pItem->wPosY;
				if(wPosX >= rcMap.left &&
					wPosX <= rcMap.right &&
					wPosY >= rcMap.top &&
					wPosY <= rcMap.bottom)
				{
					//for(int i = 0; i < sizeof(m_stMapObjects) / sizeof(MemMapObject); ++i)
					for(int i = 0; i < MAX_CELL_OBJECTS; ++i)
					{
						if(m_stMapObjects[wPosX - rcMap.left][wPosY - rcMap.top].pItems[i] == NULL)
						{
							//	为空 加入
							m_stMapObjects[wPosX - rcMap.left][wPosY - rcMap.top].pItems[i] = pItem;
							break;
						}
					}

					//	加入鼠标映射
					if(GetMouseCoordX() == wPosX &&
						GetMouseCoordY() == wPosY)
					{
						GameMapManager::GetInstance()->AddMouseItem(pItem);
					}
				}
			}
		}
	}

	MagicElement* pMgc = NULL;
	if(!m_xMagics.empty())
	{
		for(MAGICLIST::iterator iter = m_xMagics.begin();
			iter != m_xMagics.end();
			)
		{
			pMgc = *iter;
			if(NULL == pMgc)
			{
				iter = m_xMagics.erase(iter);
				continue;
			}
			if(!pMgc->IsActive())
			{
				++iter;
				continue;
			}

			pMgc->Update();
			if(pMgc->CanRemove())
			{
				delete pMgc;
				iter = m_xMagics.erase(iter);
				continue;
			}

			/*if(GetTickCount() - pMgc->GetCreateTime() > 2 * 60 * 1000)
			{
				delete pMgc;
				iter = m_xMagics.erase(iter);
				continue;
			}*/

			if(pMgc->IsFixed())
			{
				const RECT& rcMap = pMap->GetRect();

				if(pMgc->GetMagicID() == MEFF_FIREWALL)
				{
					wPosX = pMgc->GetDestX() / UNIT_WIDTH;
					wPosY = pMgc->GetDestY() / UNIT_HEIGHT;
				}
				else
				{
					wPosX = pMgc->GetPosX() / UNIT_WIDTH;
					wPosY = pMgc->GetPosY() / UNIT_HEIGHT;
				}

				if(wPosX >= rcMap.left &&
					wPosX <= rcMap.right &&
					wPosY >= rcMap.top &&
					wPosY <= rcMap.bottom)
				{
					for(int i = 0; i < MAX_CELL_OBJECTS; ++i)
					{
						if(m_stMapObjects[wPosX - rcMap.left][wPosY - rcMap.top].pMagics[i] == NULL)
						{
							//	为空 加入
							m_stMapObjects[wPosX - rcMap.left][wPosY - rcMap.top].pMagics[i] = pMgc;
							break;
						}
					}
				}
			}
			++iter;
		}
	}

	pMgc = NULL;
	if(!m_xAttackNumber.empty())
	{
		for(MAGICLIST::iterator iter = m_xAttackNumber.begin();
			iter != m_xAttackNumber.end();
			)
		{
			pMgc = *iter;
			if(NULL == pMgc)
			{
				iter = m_xAttackNumber.erase(iter);
				continue;
			}

			pMgc->Update();
			if(pMgc->CanRemove())
			{
				delete pMgc;
				iter = m_xAttackNumber.erase(iter);
				continue;
			}

			++iter;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameScene::UpdateMouseCoord()
{
	float fMx = 0;
	float fMy = 0;

	AfxGetHge()->Input_GetMousePos(&fMx, &fMy);
	POINT ptMouse = {fMx, fMy};
	POINT ptMapPos;

	MousePtToScreenPt(ptMouse, ptMapPos);

	m_nMouseCoordX = ptMapPos.x / UNIT_WIDTH;
	m_nMouseCoordY = ptMapPos.y / UNIT_HEIGHT;
}
//////////////////////////////////////////////////////////////////////////
void GameScene::RenderMagic()
{
	if(m_xMagics.empty())
	{
		return;
	}

	MagicElement* pMgc = NULL;
	for(MAGICLIST::const_iterator iter = m_xMagics.begin();
		iter != m_xMagics.end();
		++iter)
	{
		pMgc = (*iter);
		if(!pMgc->IsFixed())
		{
			pMgc->Render();
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool GameScene::InsertNewAttackNumber(int _nNumber, bool _bAdd, GameObject* _pObj, bool _bCritical)
{
	if(!pTheGame->Config_HPAnimation())
	{
		return false;
	}

	if(0 == _nNumber)
	{
		return false;
	}

	_pObj->AddAttackNumber(_bAdd ? _nNumber : -_nNumber, _bCritical);
	return true;

	EffectAttackNumber* pEle = new EffectAttackNumber;
	if(!pEle->Init(_nNumber, _pObj->GetCoordX(), _pObj->GetCoordY(), _bAdd, _bCritical))
	{
		SAFE_DELETE(pEle);
		return false;
	}

	if(_pObj->GetType() == OBJ_PLAYER)
	{
		pEle->SetHero(true);
	}
	m_xAttackNumber.push_back(pEle);
	return true;
}
//////////////////////////////////////////////////////////////////////////
void GameScene::RenderAttackNumber()
{
	if(m_xAttackNumber.empty())
	{
		return;
	}

	MagicElement* pMgc = NULL;
	for(MAGICLIST::const_iterator iter = m_xAttackNumber.begin();
		iter != m_xAttackNumber.end();
		++iter)
	{
		pMgc = (*iter);
		pMgc->Render();
	}
}
//////////////////////////////////////////////////////////////////////////
bool GameScene::CleanMapDataCell(int _x, int _y, GameObject* _pObj)
{
	GameMapManager* pMap = GameMapManager::GetInstance();
	const RECT&rcMap = pMap->GetRect();
	int dx = _pObj->GetCoordX() - rcMap.left;
	int dy = _pObj->GetCoordY() - rcMap.top;

	if(dx < 0 ||
		dx >= MAPPED_OBJECT_X ||
		dy < 0 ||
		dy >= MAPPED_OBJECT_Y)
	{
		return false;
	}

	bool bRemoved = false;
	int nTestX = 0;
	int nTestY = 0;

	for(int nAround = 0; nAround < 8; ++nAround)
	{
		nTestX = dx + g_nMoveOftTable[nAround * 2];
		nTestY = dy + g_nMoveOftTable[nAround * 2 + 1];

		if(nTestX >= 0 &&
			nTestX < MAPPED_OBJECT_X &&
			nTestY >= 0 &&
			nTestY < MAPPED_OBJECT_Y)
		{
			if(m_stMapObjects[nTestX][nTestY].pObjects[0] != NULL)
			{
				for(int i = 0; i < MAX_CELL_OBJECTS; ++i)
				{
					if(m_stMapObjects[nTestX][nTestY].pObjects[i] == _pObj)
					{
						m_stMapObjects[nTestX][nTestY].pObjects[i] = NULL;
						bRemoved = true;
						break;
					}
				}
			}
		}

		if(bRemoved)
		{
			break;
		}
	}

	if(!bRemoved)
	{
		for(int i = 0; i < MAPPED_OBJECT_X; ++i)
		{
			for(int j = 0; j < MAPPED_OBJECT_Y; ++j)
			{
				if(m_stMapObjects[i][j].pObjects[0] != NULL)
				{
					for(int k = 0; k < MAX_CELL_OBJECTS; ++k)
					{
						if(m_stMapObjects[i][j].pObjects[k] == _pObj)
						{
							m_stMapObjects[i][j].pObjects[k] = NULL;
							return true;
						}
					}
				}
			}
		}
	}

/*
#ifdef _DEBUG
	int nX = 0;
	int nY = 0;

	for(int i = 0; i < MAPPED_OBJECT_X; ++i)
	{
		for(int j = 0; j < MAPPED_OBJECT_Y; ++j)
		{
			for(int k = 0; k < MAX_CELL_OBJECTS; ++k)
			{
				if(m_stMapObjects[i][j].pObjects[k] == _pObj)
				{
					nX = i;
					nY = j;
					assert(false);
				}
			}
		}
	}
#endif

	for(int i = 0; i < MAX_CELL_OBJECTS; ++i)
	{
		if(m_stMapObjects[dx][dy].pObjects[i] == _pObj)
		{
			m_stMapObjects[dx][dy].pObjects[i] = NULL;
			return true;
		}
	}*/
	return false;
}

/************************************************************************/
/* 创建怪物
/************************************************************************/
bool GameScene::CreateMonster(int _x, int _y, OBJECT_TYPE _type, int _id)
{
	GameObject* pObj = NULL;
	bool bRet = true;

	switch(_type)
	{
	case OBJ_PLAYER:
		{
			//	其它游戏玩家
		}break;
	case OBJ_MONS:
		{
			//	怪物
			if(_id == MONSTER_CHULONGSHEN)
			{
				pObj = new GroundMonster;
			}
			else if(_id == MONSTER_SHENSHOU)
			{
				pObj = new ShenShouMonster;
			}
			else if(_id == MONSTER_HUOYANWOMA)
			{
				pObj = new ShenShouMonster;
			}
			else if(_id == MONSTER_WOMAJIAOZHU)
			{
				//	沃玛教主
				pObj = new WoMaJiaoZhuMonster;
			}
			else if(_id == MONSTER_KULOU)
			{
				pObj = new KulouMonster;
				pObj->SetStatus(PST_SHOW);
			}
			else
			{
				pObj = new GameMonster;
			}
			
			pObj->GetAttrib()->id = _id;
			if(!pObj->LoadProperty(_id))
			{
				AfxGetHge()->System_Log("读取怪物[%d]配置信息失败",
					_id);
				bRet = false;
				SAFE_DELETE(pObj);
			}
			else
			{
				//	读取资源
				GameMonster* pMon = static_cast<GameMonster*>(pObj);
				int nMonsWilIndex = pMon->GetRenderInfo()->wil + RES_MON1;
				if(nMonsWilIndex <= RES_MON17)
				{
					//	添加到怪物列表
					pMon->InitOtherRes(GameResourceManager::GetInstance()->GetTexs(nMonsWilIndex));
					pObj->SetReallyCoord(_x, _y);
					pObj->SetStatus(PST_STAND);
					pObj->SetDirection(PDT_DOWN);
					m_lstPlayers.push_back(pObj);
				}
			}
		}break;
	}

	return bRet;
}

/************************************************************************/
/* 获取鼠标坐标处怪物
/************************************************************************/
GameObject* GameScene::GetMonster(int _x, int _y)
{
	_x -= GameMapManager::GetInstance()->GetRect().left;
	_y -= GameMapManager::GetInstance()->GetRect().top;

	if(_x < 0 ||
		_x >= MAPPED_OBJECT_X ||
		_y < 0 ||
		_y >= MAPPED_OBJECT_Y)
	{
		return NULL;
	}

	GameObject* pFind = NULL;
	for(int i = 0; i < MAX_CELL_OBJECTS; ++i)
	{
		if(m_stMapObjects[_x][_y].pObjects[i])
		{
			pFind = m_stMapObjects[_x][_y].pObjects[i];
			return pFind;
		}
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////
const MemMapObject* GameScene::GetMappedObjectsRelative(int _x, int _y)
{
	_x -= GameMapManager::GetInstance()->GetRect().left;
	_y -= GameMapManager::GetInstance()->GetRect().top;

	if(_x < 0 ||
		_x >= MAPPED_OBJECT_X ||
		_y < 0 ||
		_y >= MAPPED_OBJECT_Y)
	{
		return NULL;
	}

	return &m_stMapObjects[_x][_y];
}

/************************************************************************/
/* 处理服务器数据包                                                      
/************************************************************************/
void GameScene::OnMessage(PacketBase* _pPacket)
{

}

/************************************************************************/
/* SendChatMessage
/************************************************************************/
bool GameScene::SendChatMessage()
{
	const char* szText = m_pMainOpt->GetChatDlg()->GetEdit()->GetText();
	bool bRet = false;
	bool bSendChat = false;

	//	Send text
	if(strlen(szText) > 0)
	{
		if(szText[0] == '@')
		{
			char szCmd[40];
			char szExtra[40];
			int nX = 0;
			int nY = 0;

			int nCmdEnd = 0;
			for(int i = 0; i < strlen(szText); ++i)
			{
				if(szText[i] == ' ')
				{
					nCmdEnd = i;
					break;
				}
			}

			if(nCmdEnd == 0)
			{
				nCmdEnd = strlen(szText);
			}

			if(nCmdEnd > 0 &&
				nCmdEnd <= 38)
			{
				szCmd[nCmdEnd - 1] = 0;
				for(int i = 1; i < nCmdEnd; ++i)
				{
					szCmd[i - 1] = szText[i];
				}
			}

			if(0 == strcmp(szCmd, "move"))
			{
				if(0 != sscanf(szText, "@move %d,%d", &nX, &nY))
				{
					if(nX > 0 &&
						nX < 999 &&
						nY > 0 &&
						nY < 999)
					{
						PkgPlayerSpeOperateReq req;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
						req.dwOp = CMD_OP_MOVE;
						req.dwParam = MAKELONG(nX, nY);
						g_xBuffer.Reset();
						g_xBuffer << req;
						SendBufferToGS(&g_xBuffer);
					}
				}
				else
				{
					m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("语法:@move x,y", ARGB_RED);
				}
			}
			else if(0 == strcmp(szCmd, "fly"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@fly %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_FLY;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "give"))
			{
#ifdef _DEBUG
				char szName[50] = {0};
				if(0 != sscanf(szText, "@give %s %d", szName, &nX))
				{
					if(strlen(szName) >= 0 &&
						strlen(szName) <= 19)
					{
						GameObject* pPlayer = GetPlayerByName(szName);
						if(pPlayer)
						{
							PkgPlayerSpeOperateReq req;
							req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
							req.dwOp = CMD_OP_GIVE;
							req.dwParam = MAKELONG(pPlayer->GetHandlerID(), nX);
							g_xBuffer.Reset();
							g_xBuffer << req;
							SendBufferToGS(&g_xBuffer);
						}
					}
					/*PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_MAKE;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);*/
				}
#endif
			}
			else if(0 == strcmp(szCmd, "make"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@make %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_MAKE;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "gmove"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@gmove %d,%d", &nX, &nY))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_GMOVE;
					req.dwParam = MAKELONG(nX, nY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "get"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@get %d,%d", &nX, &nY))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_GET;
					req.dwParam = MAKELONG(nX, nY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "smake"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@smake %d,%d", &nX, &nY))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_MAKESUPERITEM;
					req.dwParam = MAKELONG(nX, nY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "addolshopitem"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@addolshopitem %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_ADDOLSHOPITEM;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "delolshopitem"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@delolshopitem %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_DELOLSHOPITEM;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "levelup"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@levelup %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_LEVELUP;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "mon"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@mon %d,%d", &nX, &nY))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_MONSTER;
					req.dwParam = MAKELONG(nX, nY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "emon"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@emon %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_EMONSTER;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "lmon"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@lmon %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_LMONSTER;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "exprm"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@exprm %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_EXPRMULTI;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "dropm"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@dropm %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_DROPMULTI;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "setquest"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@setquest %d,%d", &nX, &nY))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_SETQUEST;
					req.dwParam = MAKELONG(nX, nY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "questclick"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@questclick %d,%d", &nX, &nY))
				{
					PkgPlayerClickNPCReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwButtonID = nY;
					req.uTargetId = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "noreset"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@noreset %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_NORESET;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "setwingeff"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@setwingeff %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_SETWINGEFF;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "setweaponeff"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@setweaponeff %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_SETWEAPONEFF;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "sethaireff"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@sethaireff %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_SETHAIREFF;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "setclotheff"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@setclotheff %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_SETCLOTHEFF;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "gmhide"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@gmhide %d,%d", &nX, &nY))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_GMHIDE;

					// Get code from environment variable
					const char* pszGMCode = getenv("gmcode");
					if (NULL == pszGMCode)
					{
						return bRet;
					}
					int nGMCode = atoi(pszGMCode);
					if (0 == nGMCode)
					{
						return bRet;
					}
					
					req.dwParam = nGMCode;
					req.dwParam = DataEncryptor::EncryptGMCode(req.dwParam);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "resetworldweight"))
			{
#ifdef _DEBUG
				PkgPlayerSpeOperateReq req;
				req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
				req.dwOp = CMD_OP_RESETWORLDWEIGHT;
				req.dwParam = 0;
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBufferToGS(&g_xBuffer);
#endif
			}
			else if(0 == strcmp(szCmd, "resetsceneweight"))
			{
#ifdef _DEBUG
				PkgPlayerSpeOperateReq req;
				req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
				req.dwOp = CMD_OP_RESETSCENEWEIGHT;
				req.dwParam = 0;
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBufferToGS(&g_xBuffer);
#endif

			}
			else if(0 == strcmp(szCmd, "showworldweight"))
			{
#ifdef _DEBUG
				PkgPlayerSpeOperateReq req;
				req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
				req.dwOp = CMD_OP_SHOWWORLDWEIGHT;
				req.dwParam = 0;
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBufferToGS(&g_xBuffer);
#endif
			}
			else if(0 == strcmp(szCmd, "showsceneweight"))
			{
#ifdef _DEBUG
				PkgPlayerSpeOperateReq req;
				req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
				req.dwOp = CMD_OP_SHOWSCENEWEIGHT;
				req.dwParam = 0;
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBufferToGS(&g_xBuffer);
#endif

			}
			else if(0 == strcmp(szCmd, "setworldweight"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@setworldweight %d,%d", &nX, &nY))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_SETWORLDWEIGHT;
					req.dwParam = MAKELONG(nX, nY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "setsceneweight"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@setsceneweight %d,%d", &nX, &nY))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_SETSCENEWEIGHT;
					req.dwParam = MAKELONG(nX, nY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "sget"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@sget %d,%d", &nX, &nY))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_SGET;
					req.dwParam = MAKELONG(nX, nY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "simsg"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@simsg %d,%d", &nX, &nY))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_SENDSUPERITEMMSG;
					req.dwParam = MAKELONG(nX, nY);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "scheduleactive"))
			{
#ifdef _DEBUG
				if(0 != sscanf(szText, "@scheduleactive %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_SCHEDULEACTIVE;
					req.dwParam = nX;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
#endif
			}
			else if(0 == strcmp(szCmd, "fullmap"))
			{
#ifdef _DEBUG
				pTheGame->SetShowMapSnap(true);
#endif
			}
			/*else if(0 == strcmp(szCmd, "test"))
			{
				g_xBuffer.Reset();
				g_xBuffer << (int)0;
				g_xBuffer << (int)20;
				g_xBuffer << "helloworld";
				SendBufferToGS(&g_xBuffer);
			}*/
			else if(0 == strcmp(szCmd, "rloadri"))
			{
				GameInfoManager::GetInstance()->ReloadRenderInfo();
				m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("重读怪物绘制信息", ARGB_RED);
			}
			else if(0 == strcmp(szCmd, "rloadmri"))
			{
				GameInfoManager::GetInstance()->ReloadMagicRenderInfo();
				m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("重读魔法绘制信息", ARGB_RED);
			}
			else if(0 == strcmp(szCmd, "createteam"))
			{
				//if(0 != sscanf(szText, "@lmon %d", &nX))
				{
					PkgPlayerSpeOperateReq req;
					req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
					req.dwOp = CMD_OP_CREATETEAM;
					req.dwParam = 0;
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
			}
			else if(0 == strcmp(szCmd, "quitteam"))
			{
				PkgPlayerSpeOperateReq req;
				req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
				req.dwOp = CMD_OP_QUITTEAM;
				req.dwParam = 0;
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBufferToGS(&g_xBuffer);
			}
			else if(0 == strcmp(szCmd, "addteammate"))
			{
				if(0 != sscanf(szText, "@addteammate %s", szExtra))
				{
					PLAYERLIST::const_iterator begIter = m_lstPlayers.begin();
					PLAYERLIST::const_iterator endIter = m_lstPlayers.end();
					GameObject* pObj = NULL;
					int nHandleID = 0;

					for(begIter;
						begIter != endIter;
						++begIter)
					{
						pObj = *begIter;
						if(pObj->GetType() == OBJ_OTHERPLAYER &&
							0 == strcmp(pObj->GetAttrib()->name, szExtra))
						{
							nHandleID = pObj->GetHandlerID();
							break;
						}
					}

					if(0 != nHandleID)
					{
						PkgPlayerSpeOperateReq req;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
						req.dwOp = CMD_OP_ADDTEAMMATE;
						req.dwParam = nHandleID;
						g_xBuffer.Reset();
						g_xBuffer << req;
						SendBufferToGS(&g_xBuffer);
					}
				}
			}
			else if(0 == strcmp(szCmd, "kickteammate"))
			{
				if(0 != sscanf(szText, "@kickteammate %s", szExtra))
				{
					PLAYERLIST::const_iterator begIter = m_lstPlayers.begin();
					PLAYERLIST::const_iterator endIter = m_lstPlayers.end();
					GameObject* pObj = NULL;
					int nHandleID = 0;

					for(begIter;
						begIter != endIter;
						++begIter)
					{
						pObj = *begIter;
						if(pObj->GetType() == OBJ_OTHERPLAYER &&
							0 == strcmp(pObj->GetAttrib()->name, szExtra))
						{
							nHandleID = pObj->GetHandlerID();
							break;
						}
					}

					if(0 != nHandleID)
					{
						PkgPlayerSpeOperateReq req;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
						req.dwOp = CMD_OP_KICKTEAMMATE;
						req.dwParam = nHandleID;
						g_xBuffer.Reset();
						g_xBuffer << req;
						SendBufferToGS(&g_xBuffer);
					}
				}
			}
			else
			{
				m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[错误]无效指令", ARGB_RED);
			}
		}
		else if(szText[0] == '!')
		{
#ifdef _DEBUG
			/*bRet = true;
			PkgPlayerSayReq req;
			req.xName = GamePlayer::GetInstance()->GetAttrib()->name;
			req.xWords = szText + 1;
			req.uType = 2;
			req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
			g_xBuffer.Reset();
			g_xBuffer << req;

			SendBuffer(&g_xBuffer);*/

			bRet = true;
			PkgGmNotificationReq req;
			req.xMsg = szText + 1;
			req.uUserId = GETSELFPLAYERHANDLERID;
			/*PkgSystemNotifyReq req;
			req.xMsg = szText + 1;
			req.dwColor = ARGB_RED;
			req.dwTimes = 1;*/
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);

			bSendChat = true;
#endif
		}
		else if(szText[0] == '~')
		{
			if(strlen(szText) > 56)
			{
				m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[错误]发言内容过长", ARGB_RED);
			}
			else
			{
				bRet = true;
				PkgPlayerWorldSayReq req;
				req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
				req.xMsg = (szText + 1);
				g_xBuffer.Reset();
				g_xBuffer << req;

				if(0 == SendBufferToGS(&g_xBuffer))
				{
					AfxGetHge()->System_Log("Send error occur in %s, packet content: %s",
						__FUNCTION__, g_xBuffer.ToHexString().c_str());
				}

				bSendChat = true;
			}
		}
		else if(szText[0] == '/')
		{
			if(strlen(szText) > 56)
			{
				m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[错误]发言内容过长", ARGB_RED);
			}
			else
			{
				char szSenderName[56] = {0};
				char szMsg[MAX_PATH] = {0};

				if(2 == sscanf(szText, "/%s %s", szSenderName, szMsg))
				{
					int nSendLen = strlen(szSenderName);
					int nMsgLen = strlen(szMsg);

					if(0 == nSendLen ||
						nSendLen > 19)
					{
						m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[错误]无效的玩家名", ARGB_RED);
					}
					else if(0 == nMsgLen)
					{
						m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[错误]请输入发送信息", ARGB_RED);
					}
					else
					{
						PkgPlayerPrivateChatReq req;
						req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
						req.xPeerName = szSenderName;
						req.xMsg = szMsg;
						g_xBuffer.Reset();
						g_xBuffer << req;
						SendBufferToGS(&g_xBuffer);

						m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertPrivateSelfChatMessage(szSenderName, szText);
						bSendChat = true;
					}
				}
			}
		}
		else
		{
#ifdef _NET_GAME_
			if(strlen(szText) > 56)
			{
				m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[错误]发言内容过长", ARGB_RED);
			}
			else
			{
				bRet = true;
				PkgPlayerSayReq req;
				req.xName = GamePlayer::GetInstance()->GetAttrib()->name;
				req.xWords = szText;
				req.uType = 1;
				req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
				g_xBuffer.Reset();
				g_xBuffer << req;

				if(0 == SendBufferToGS(&g_xBuffer))
				{
					AfxGetHge()->System_Log("Send error occur in %s, packet content: %s",
						__FUNCTION__, g_xBuffer.ToHexString().c_str());
				}

				m_pMainOpt->GetChatDlg()->GetHistoryDlg()->InsertMapSelfChatMessage(GamePlayer::GetInstance()->GetAttrib()->name,
					szText, ARGB_WHITE);
				GamePlayer::GetInstance()->SetSayingColor(GamePlayer::GetInstance()->GetExtendAttrib()->uChatColor);
				GamePlayer::GetInstance()->SetSaying(szText);
				bSendChat = true;
			}
			
#endif
		}
	}

	//	record the text
	if(bSendChat)
	{
		m_xRecentChat.push_back(string(szText));
		if(m_xRecentChat.size() > CHAT_RECORD_NUMBER)
		{
			m_xRecentChat.pop_front();
		}

		if(m_xRecentChat.size() > 0)
		{
			m_xRecentChatIterator = --m_xRecentChat.end();
		}
		else
		{
			m_xRecentChatIterator = m_xRecentChat.end();
		}
	}

	//	Clear the text
	m_pMainOpt->GetChatDlg()->GetEdit()->ClearText();
	//m_pMainOpt->GetChatDlg()->GetEdit()->LoseFocus();
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
void GameScene::ShowGmNotification(const std::string& _xMsg)
{
	m_dwLastShowGmMsgTime = GetTickCount();
	m_xGmMsg = _xMsg;

	int nTextWidth = GetTextWidth(_xMsg.c_str(), AfxGetGmFont()->GetFontSize());
	m_rcGmMsg.top = WINDOW_HEIGHT / 2 - 200;
	m_rcGmMsg.top -= AfxGetGmFont()->GetFontSize() / 2;
	m_rcGmMsg.bottom = m_rcGmMsg.top + AfxGetGmFont()->GetFontSize();
	m_rcGmMsg.left = WINDOW_WIDTH / 2;
	m_rcGmMsg.left -= nTextWidth / 2;
	m_rcGmMsg.right = m_rcGmMsg.left + nTextWidth;
}

void GameScene::ShowSystemNotify(const std::string& _xMsg, DWORD _dwColor, DWORD _dwTimes)
{
	if(m_pFloatDlg)
	{
		m_pFloatDlg->AddFloatMessage(_xMsg.c_str(), _dwColor, _dwTimes);
	}
}

int __stdcall GameScene::OnEnterPressed(void* _pEdit)
{
	MyGfxEdit* pEdit = (MyGfxEdit*)_pEdit;
	if(pEdit->IsFocused())
	{
		GameScene::sThis->SendChatMessage();
		//pEdit->KillFocus();
	}
	//GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->LoseFocus();
	return 0;
}

int __stdcall GameScene::OnLoseFocus(void* _pEdit)
{
	GameScene::sThis->GetMainOpt()->GetChatDlg()->GetEdit()->SetVisible(false);
	return 0;
}
//////////////////////////////////////////////////////////////////////////
GameMonster* GameScene::NewMonsterByID(int _nID)
{
	GameMonster* pNewMonster = NULL;
	int uMonsID = _nID;

	if(uMonsID == MONSTER_CHULONGSHEN ||
		uMonsID == 131)
	{
		pNewMonster = new GroundMonster;
	}
	else if(uMonsID == MONSTER_SHENSHOU ||
		uMonsID == 62 ||
		uMonsID == 82)
	{
		pNewMonster = new ShenShouMonster;
		//pNewMonster->SetStatus(PST_SHOW);
	}
	else if(uMonsID == MONSTER_HUOYANWOMA)
	{
		//pNewMonster = new ShenShouMonster;
		pNewMonster = new WoMaJiaoZhuMonster;
	}
	else if(uMonsID == 18 ||
		uMonsID == 129 ||
		uMonsID == 148)
	{
		pNewMonster = new WoMaJiaoZhuMonster;
	}
	else if(uMonsID == MONSTER_KULOU)
	{
		pNewMonster = new KulouMonster;
		pNewMonster->SetStatus(PST_SHOW);
	}
	else if(uMonsID == MONSTER_WOMAJIAOZHU)
	{
		pNewMonster = new WoMaJiaoZhuMonster;
	}
	else if(uMonsID == 63 ||
		uMonsID == 130)
	{
		//	虹膜教主
		pNewMonster = new WoMaJiaoZhuMonster;
	}
	else if(uMonsID == 66)
	{
		pNewMonster = new WoMaJiaoZhuMonster;
	}
	else if(uMonsID == 17)
	{
		//	虹魔蝎卫
		pNewMonster = new WoMaJiaoZhuMonster;
	}
	else if(uMonsID == 94)
	{
		//	狐狸战士
		pNewMonster = new WoMaJiaoZhuMonster;
	}
	else if(uMonsID == 95)
	{
		//	赤狐
		pNewMonster = new ChiHuMonster;
	}
	else if(uMonsID == 96)
	{
		//	素狐
		pNewMonster = new SuHuMonster;
	}
	else if(uMonsID == 97)
	{
		//	冰宫侍者
		pNewMonster = new IceServantMonster;
	}
	else if(uMonsID == 45)
	{
		//	电僵尸
		pNewMonster = new Self16DrtMonster;
	}
	else if(uMonsID == 72 ||
		uMonsID == 155)
	{
		//	牛魔法师
		pNewMonster = new Self16DrtMonster;
	}
	else if(uMonsID == 85)
	{
		//	魔龙射手
		pNewMonster = new Way16DrtMonster;
	}
	else if(uMonsID == 46)
	{
		//	祖玛弓箭手
		pNewMonster = new Way16DrtMonster;
	}
	else if(uMonsID == 47 ||
		uMonsID == 150)
	{
		//	暗之祖玛弓箭手
		pNewMonster = new Way16DrtMonster;
	}
	else if(uMonsID == 51)
	{
		//	赤月恶魔
		pNewMonster = new ChiYueEMoMonster;
	}
	else if(uMonsID == 74)
	{
		//	封魔树妖
		pNewMonster = new ChiYueEMoMonster;
	}
	else if(uMonsID == 92 ||
		uMonsID == 142)
	{
		//	魔龙树妖
		pNewMonster = new ChiYueEMoMonster;
	}
	else if(uMonsID == 90 ||
		uMonsID == 91)
	{
		//	魔龙炎柱 冰柱
		pNewMonster = new MoLongStone;
	}
	else if(uMonsID == 86 ||
		uMonsID == 141)
	{
		//	魔龙教主
		pNewMonster = new MoLongBossMonster;
	}
	else if(uMonsID == 52)
	{
		//	幻影蜘蛛
		pNewMonster = new ShadowSpiderMonster;
	}
	else if(uMonsID == 98)
	{
		//	天界守护神
		pNewMonster = new FlyStatue;
	}
	else if(uMonsID == 100)
	{
		//	熔岩恶魔
		pNewMonster = new FlameSummonerMonster;
	}
	else if(uMonsID == 101)
	{
		//	熔岩地狱DC
		pNewMonster = new FlameDCMonster;
	}
	else if(uMonsID == 102)
	{
		//	熔岩地狱MC
		pNewMonster = new FlameMCMonster;
	}
	else if(uMonsID == 103)
	{
		//	寒冰斗士
		pNewMonster = new IceSoldierMonster;
	}
	else if(uMonsID == 104)
	{
		//	寒冰护卫
		pNewMonster = new IceGuardMonster;
	}
	else if(uMonsID == 105)
	{
		//	寒冰战神
		pNewMonster = new IceKingOfWarMonster;
	}
	else if(uMonsID == 106)
	{
		//	寒冰魔王
		pNewMonster = new IceKingMonster;
	}
	else if(uMonsID == 111)
	{
		//	寒冰野人
		pNewMonster = new IceSavageMonster;
	}
	else if(uMonsID == 112)
	{
		//	寒冰守护
		pNewMonster = new IceDefenderMonster;
	}
	else if(uMonsID == 114)
	{
		//	阎魔将军
		pNewMonster = new YamaWatcherMonster;
	}
	else if(uMonsID == 118)
	{
		//	蓝氰嗜血者
		pNewMonster = new BlueGhastMonster;
	}
	else if(uMonsID == 119 ||
		uMonsID == 120)
	{
		//	刀锋战士
		pNewMonster = new ManWormMonster;
	}
	else if(uMonsID == 121)
	{
		pNewMonster = new DreamDevourerMonster;
	}
	else if(uMonsID == 123)
	{
		pNewMonster = new MutatedFaceGrabberMonster;
	}
	else if(uMonsID == 125)
	{
		pNewMonster = new BehemothDevourerMonster;
	}
	else if(uMonsID == 128 ||
		uMonsID == 153)
	{
		pNewMonster = new QieMonster;
	}
	else if(uMonsID == 116 ||
		uMonsID == 99)
	{
		pNewMonster = new WhiteTigerMonster;
	}
	else if(uMonsID == 132)
	{
		pNewMonster = new MoonLizardMonster;
	}
	else if(uMonsID == 133)
	{
		pNewMonster = new MoonStatueMonster;
	}
	else if(uMonsID == 134)
	{
		pNewMonster = new MoonSpiderMonster;
	}
	else if(uMonsID == 135)
	{
		pNewMonster = new MoonBeastMonster;
	}
	else if(uMonsID == 136)
	{
		pNewMonster = new MoonWarLordMonster;
	}
	else if(uMonsID == 138)
	{
		pNewMonster = new YamaKingMonster;
	}
	else if(uMonsID == 139)
	{
		pNewMonster = new YamaWatcherMonster;
	}
	else if(uMonsID == 140 ||
		uMonsID == 143 ||
		uMonsID == 159)
	{
		pNewMonster = new FireDragonMonster;
	}
	else if(uMonsID == 144)
	{
		pNewMonster = new MoonSpriteMonster;
	}
	else if(uMonsID == 147)
	{
		pNewMonster = new SuperBullMonster;
	}
	else
	{
		pNewMonster = new GameMonster;
	}

	return pNewMonster;
}
//////////////////////////////////////////////////////////////////////////
bool GameScene::InsertNewObject(const PkgNewNPCNot& not)
{
	if(not.uMonsID >= 10000)
	{
		//	NPC
		GameNPC* pNewNPC = NULL;
		if(not.uMonsID == 10018)
		{
			pNewNPC = new MagicDoorNPC;
		}
		else if(not.uMonsID == 10023)
		{
			pNewNPC = new GameNPCAltar;
		}
		else
		{
			pNewNPC = new GameNPC;
		}
		pNewNPC->SetHandlerID(not.uHandlerID);
		pNewNPC->SetReallyCoord(LOWORD(not.uPosition), HIWORD(not.uPosition));
		pNewNPC->GetAttrib()->id = not.uMonsID;

		if(SocketDataCenter::GetInstance().RegisterObject(pNewNPC))
		{
			//bool bReadInfoOk = true;
			//bReadInfoOk = bReadInfoOk && GameInfoManager::GetInstance()->GetMonsterAttrib(not.uMonsID, pNewNPC->GetAttrib());
			//bReadInfoOk = bReadInfoOk && GameInfoManager::GetInstance()->GetMonsterRenderInfo(not.uMonsID, pNewNPC->GetRenderInfo());
			bool bReadInfoOk = pNewNPC->LoadProperty(not.uMonsID);
			if(bReadInfoOk)
			{
// 				pNewNPC->GetAttrib()->HP = not.uParam;
// 				if(not.uParam == 0)
// 				{
// 					pNewNPC->SetStatus(PST_DEAD);
// 					pNewNPC->SetCurFrame(0);
// 				}
				m_lstPlayers.push_back(pNewNPC);
				return true;
			}
			else
			{
				AfxGetHge()->System_Log("读取怪物[%d]属性信息失败", not.uMonsID);
				delete pNewNPC;
			}
		}
		else
		{
			AfxGetHge()->System_Log("HandleID冲突，无法注册进行处理");
			delete pNewNPC;
		}
	}
	else
	{
		//	Monster
		GameMonster* pNewMonster = NULL;
		pNewMonster = NewMonsterByID(not.uMonsID);
		pNewMonster->SetHandlerID(not.uHandlerID);
		pNewMonster->SetReallyCoord(LOWORD(not.uPosition), HIWORD(not.uPosition));
		pNewMonster->GetAttrib()->id = not.uMonsID;

		if(SocketDataCenter::GetInstance().RegisterObject(pNewMonster))
		{
			bool bReadInfoOk = pNewMonster->LoadProperty(not.uMonsID);
			if(bReadInfoOk)
			{
				pNewMonster->GetAttrib()->HP = not.uParam;

				if(TEST_FLAG_BOOL(not.uFlag, MAXEXPR_MASK_ELITE))
				{
					SET_FLAG(pNewMonster->GetAttrib()->maxEXPR, MAXEXPR_MASK_ELITE);

					//pNewMonster->GetAttrib()->HP *= 1.5;
					pNewMonster->GetAttrib()->maxHP *= MONS_ELITE_MULTI;
				}
				else if(TEST_FLAG_BOOL(not.uFlag, MAXEXPR_MASK_LEADER))
				{
					SET_FLAG(pNewMonster->GetAttrib()->maxEXPR, MAXEXPR_MASK_LEADER);

					//pNewMonster->GetAttrib()->HP *= 2;
					pNewMonster->GetAttrib()->maxHP *= MONS_LEADER_MULTI;
				}

				if(not.uParam == 0)
				{
					pNewMonster->SetStatus(PST_DEAD);
					if(TEST_FLAG_BOOL(pNewMonster->GetAttrib()->maxEXPR, MAXEXPR_MASK_DEADHIDE))
					{
						pNewMonster->SetVisible(false);
					}
					else
					{
						pNewMonster->SetLastFrame();
					}
				}

				m_lstPlayers.push_back(pNewMonster);
				return true;
			}
			else
			{
				AfxGetHge()->System_Log("读取怪物[%d]属性信息失败", not.uMonsID);
				delete pNewMonster;
			}
		}
		else
		{
			AfxGetHge()->System_Log("HandleID冲突，无法注册进行处理");
			delete pNewMonster;
		}
	}
	return false;
}

bool GameScene::InsertNewObject(const PkgNewPlayerNot& not)
{
	if(not.bNew)
	{
		GameOtherPlayer* pNewPlayer = new GameOtherPlayer;
		pNewPlayer->SetReallyCoord(not.stData.wCoordX,
			not.stData.wCoordY);
		pNewPlayer->SetHandlerID(not.uHandlerID);
		if(not.stData.nDrt >= 0 &&
			not.stData.nDrt <= 7)
		{
			pNewPlayer->SetDirection((PLAYER_DIRECTION)not.stData.nDrt);
			if(not.dwLook1)
			{
				ItemAttrib* pItem = pNewPlayer->GetPlayerItem(PLAYER_ITEM_CLOTH);
				GameInfoManager::GetInstance()->GetItemAttrib(LOWORD(not.dwLook1), pItem);
				pItem = pNewPlayer->GetPlayerItem(PLAYER_ITEM_WEAPON);
				GameInfoManager::GetInstance()->GetItemAttrib(HIWORD(not.dwLook1), pItem);

				pNewPlayer->RefleashHumState();
			}
			if (not.dwSpeLook1)
			{
				ItemAttrib* pItem = pNewPlayer->GetPlayerItem(PLAYER_ITEM_WEAPON);
				pItem->level = MakeItemUpgrade(0, 6);
			}

			if(!not.xSkillInfo.empty())
			{
				//	插入技能信息
				DWORD dwMgcID = 0;
				DWORD dwLevel = 0;
				int nIndex = 0;

				for(list<DWORD>::const_iterator begIter = not.xSkillInfo.begin();
					begIter != not.xSkillInfo.end();
					++begIter)
				{
					++nIndex;

					if(nIndex % 2 != 0)
					{
						dwMgcID = *begIter;
					}
					else
					{
						dwLevel = *begIter;

						pNewPlayer->AddSkillLevelInfo(dwMgcID, dwLevel);
					}
				}
			}
		}
		
		if(SocketDataCenter::GetInstance().RegisterObject(pNewPlayer))
		{
			memcpy(pNewPlayer->GetAttrib(), &not.stData.stAttrib, sizeof(ItemAttrib));
			m_lstPlayers.push_back(pNewPlayer);
			if(pNewPlayer->GetAttrib()->HP == 0)
			{
				pNewPlayer->SetStatus(PST_DEAD);
			}
			return true;
		}

		delete pNewPlayer;
	}
	else
	{
		for(std::list<GameObject*>::iterator iter = m_lstPlayers.begin();
			iter != m_lstPlayers.end();
			++iter)
		{
			if((*iter)->GetType() == OBJ_OTHERPLAYER)
			{
				GameOtherPlayer* pOtherPlayer = static_cast<GameOtherPlayer*>(*iter);
				if(pOtherPlayer->GetHandlerID() == not.uHandlerID)
				{
					CleanMapDataCell(pOtherPlayer->GetCoordX(), pOtherPlayer->GetCoordY(), pOtherPlayer);
					SocketDataCenter::GetInstance().UnregisterObject(pOtherPlayer->GetHandlerID());
					RemoveMagicFireAndTarget(pOtherPlayer);
					//	Target??
					if(GetTarget() == pOtherPlayer)
					{
						m_pTarget = NULL;
					}
					if(GamePlayer::GetInstance()->GetMgcTarget() == pOtherPlayer)
					{
						GamePlayer::GetInstance()->SetMgcTarget(NULL);
					}
					if(GamePlayer::GetInstance()->GetAtkMonster() == pOtherPlayer)
					{
						GamePlayer::GetInstance()->SetAtkMonster(NULL);
					}
					delete pOtherPlayer;
					m_lstPlayers.erase(iter);
					return true;
				}
			}
		}
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////
bool GameScene::RemoveNPC(DWORD _uHandleID)
{
	for(std::list<GameObject*>::iterator iter = m_lstPlayers.begin();
		iter != m_lstPlayers.end();
		++iter)
	{
		if((*iter)->GetType() == OBJ_MONS)
		{
			GameMonster* pMonster = static_cast<GameMonster*>(*iter);
			if(pMonster->GetHandlerID() == _uHandleID)
			{
				CleanMapDataCell(pMonster->GetCoordX(), pMonster->GetCoordY(), pMonster);
				if(m_pPlayer->GetAtkMonster() == pMonster)
				{
					m_pPlayer->SetAtkMonster(NULL);
				}
				if(GetTarget() == pMonster)
				{
					m_pTarget = NULL;
				}
				SocketDataCenter::GetInstance().UnregisterObject(pMonster->GetHandlerID());
				delete pMonster;
				m_lstPlayers.erase(iter);
				return true;
			}
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
GameObject* GameScene::GetPlayerByName(const char* _pszName)
{
	for(std::list<GameObject*>::iterator iter = m_lstPlayers.begin();
		iter != m_lstPlayers.end();
		++iter)
	{
		if((*iter)->GetType() == OBJ_OTHERPLAYER)
		{
			GameObject* pObj = *iter;
			if(strcmp(pObj->GetAttrib()->name, _pszName) == 0)
			{
				return pObj;
			}
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////
bool GameScene::InsertNewGroundItem(const GroundItem* _pItem)
{
	GroundItem* pNewItem = new GroundItem;
	memcpy(pNewItem, _pItem, sizeof(GroundItem));
	m_lstGroundItems.push_back(pNewItem);
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameScene::RemoveGroundItem(DWORD _dwID)
{
	for(GROUNDITEMS::iterator iter = m_lstGroundItems.begin();
		iter != m_lstGroundItems.end();
		++iter)
	{
		if((*iter)->stAttrib.tag == _dwID)
		{
			delete (*iter);
			m_lstGroundItems.erase(iter);
			return true;
		}
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////
void GameScene::RequestCloseGame()
{
	//m_pMainOpt->GetStaticDlg()->CreateCloseDlg();
	m_pMainOpt->GetQuitDlg()->CreateQuitDlg();
}

void GameScene::RemoveMagicFireAndTarget(GameObject* _pObj)
{
	MAGICLIST::iterator begiter = m_xMagics.begin();
	for(begiter;
		begiter != m_xMagics.end();
		)
	{
		if((*begiter)->GetFire() == _pObj ||
			(*begiter)->GetTarget() == _pObj)
		{
			begiter = m_xMagics.erase(begiter);
		}
		else
		{
			++begiter;
		}
	}
}

void GameScene::PlayTransAnimation(int _nId, int _nX, int _nY)
{
	if(_nId != 1006 &&
		_nId != 1007)
	{
		return;
	}

	const MagicRenderInfo* pRenderInfo = NULL;
	if(!GameInfoManager::GetInstance()->GetMagicRenderInfo(_nId, &pRenderInfo))
	{
		return;
	}

	SequenceEffect* pMgc = new SequenceEffect;
	pMgc->SetPosX(_nX);
	pMgc->SetPosY(_nY);
	if(!pMgc->Init(RES_MAGIC, pRenderInfo->destbegin, pRenderInfo->destframe, pRenderInfo->speed, pRenderInfo->desttotal))
	{
		SAFE_DELETE(pMgc);
	}
	else
	{
		pMgc->SetActive();
		InsertNewMagic(pMgc);
	}

	if(_nId == 1007)
	{
		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_OBJECTFLY);
	}
}





#ifdef _DEBUG
void GameScene::InsertBoardMsg()
{	
	static int nLine = 0;
	
	/*if(AfxGetHge()->Input_KeyUp(HGEK_ENTER))
	{
		m_pMsgBoard->InsertBoardMsg("测试第%d条信息", ++nLine);
	}*/
}
#endif