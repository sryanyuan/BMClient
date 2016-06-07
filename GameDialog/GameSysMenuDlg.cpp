#include "GameDlgBase.h"
#include "../BackMir/BackMir.h"
#include "../GameDialog/GameQuitDialog.h"
#include "../GameDialog/GameHelpDlg.h"
#include "../GameDialog/GameSettingDlg.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameDialog/GameRankDlg.h"
//////////////////////////////////////////////////////////////////////////
const char* g_szBtnTitle[] =
{
	"游戏设置",
	"我要存档",
	"退出游戏",
	"游戏捐助",
	"游戏帮助",
	"游戏排行",
	"选择人物"
};
//////////////////////////////////////////////////////////////////////////
GameSysMenuDlg::GameSysMenuDlg()
{
	m_rcClient.left = 600;
	m_rcClient.right = m_rcClient.left + 75;
	m_rcClient.top = 480 + 20 - RT_TOTAL * 30;
	m_rcClient.bottom = 480 + 20; 
	SetVisible(false);
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);

	ResetStatus();
}

GameSysMenuDlg::~GameSysMenuDlg()
{

}
//////////////////////////////////////////////////////////////////////////
void GameSysMenuDlg::Render()
{
	if(!IsVisible())
	{
		return;
	}

	hgeResourceManager* pMgr = m_pParent->GetResourceMgr();
	if(NULL == pMgr)
	{
		return;
	}

	HTEXTURE tex = pMgr->GetTexture("sysbtn");
	if(0 == tex)
	{
		return;
	}

	m_pRender->SetTexture(tex);
	for(int i = 0; i < RT_TOTAL; ++i)
	{
		if(m_bBtnFlag[i] == 0)
		{
			m_pRender->SetTextureRect(0, 0, 75, 30);
		}
		else if(TEST_FLAG(m_bBtnFlag[i], BTN_OVER_MASK))
		{
			m_pRender->SetTextureRect(0, 30, 75, 30);
		}
		else if(TEST_FLAG(m_bBtnFlag[i], BTN_DOWN_MASK))
		{
			m_pRender->SetTextureRect(0, 30*2, 75, 30);
		}
		m_pRender->Render(m_rcClient.left, m_rcClient.top + i * 30);
		AfxGetPrinter()->SetColor(ARGB_YELLOW);
		/*if(i == RT_SOUND)
		{
			if(pTheGame->IsSoundEnabled())
			{
				AfxGetPrinter()->Print(m_rcClient.left + 14, m_rcClient.top + i * 30 + 10, "关闭声音");
			}
			else
			{
				AfxGetPrinter()->Print(m_rcClient.left + 14, m_rcClient.top + i * 30 + 10, "开启声音");
			}
		}
		else*/
		{
			AfxGetPrinter()->Print(m_rcClient.left + 14, m_rcClient.top + i * 30 + 10, g_szBtnTitle[i]);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool GameSysMenuDlg::ProcUserCmd(const POINT& _mp)
{
	RECT rcTest;
	rcTest.left = m_rcClient.left;
	rcTest.right = m_rcClient.right;
	for(int i = 0; i < RT_TOTAL; ++i)
	{
		rcTest.top = m_rcClient.top + i * 30;
		rcTest.bottom = rcTest.top + 30;
		if(::PtInRect(&rcTest, _mp))
		{
			OnBtn(i);
			return true;
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
void GameSysMenuDlg::OnBtn(int _index)
{
	ResetStatus();

	SET_FLAG(m_bBtnFlag[_index], BTN_OVER_MASK);
	if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
	{
		m_bBtnFlag[_index] = 0;
		SET_FLAG(m_bBtnFlag[_index], BTN_DOWN_MASK);
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		if(_index == RT_SAVE)
		{
			//
			/*
			m_pParent->GetStaticDlg()->Create("正在请求服务器数据", 5000);
						PkgPlayerUserDataReq req;
						g_xBuffer.Reset();
						g_xBuffer << req;
						SendBuffer(&g_xBuffer);*/
			//m_pParent->GetStaticDlg()->CreateSaveBindDlg();
			m_pParent->GetQuitDlg()->CreateSaveDlg();
		}
		/*else if(_index == RT_SOUND)
		{
			bool bSoundState = pTheGame->IsSoundEnabled();
			pTheGame->SetSoundEnabled(!bSoundState);
			if(bSoundState)
			{
				GameSoundManager::GetInstancePtr()->PauseBkSound();
			}
			else
			{
				GameSoundManager::GetInstancePtr()->ResumeBkSound();
			}
		}*/
		else if(_index == RT_CONFIG)
		{
			GameSettingDlg* pSettingDlg = GameMainOptUI::GetInstance()->GetSettingDlg();
			if(pSettingDlg->IsVisible())
			{
				pSettingDlg->CloseDialog();
			}
			else
			{
				pSettingDlg->ShowDialog();
			}
		}
		else if(_index == RT_QUIT)
		{
			HWND hWnd = AfxGetHge()->System_GetState(HGE_HWND);
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		else if(_index == RT_DONATE)
		{
			HWND hWnd = AfxGetHge()->System_GetState(HGE_HWND);
			char szURL[] = "http://sryanyuan.github.io/web/";
			ShellExecute(hWnd, "open", "iexplore.exe", szURL, "", SW_SHOW);
			//MessageBox(NULL, "支付宝账户为:sryan@qq.com,感谢您的捐助", "捐赠信息", MB_OK | MB_ICONINFORMATION);
		}
		else if(_index == RT_HELP)
		{
			GameHelpDlg* pHelpDlg = GameMainOptUI::GetInstance()->GetHelpDlg();

			if(pHelpDlg->IsVisible())
			{
				pHelpDlg->SetVisible(false);
			}
			else
			{
				pHelpDlg->CreateHelpDlg(2);
			}
		}
		else if(_index == RT_RANK)
		{
			if(pTheGame->GetGameMode() == GM_NORMAL)
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "该模式不支持排行榜系统");
			}
			else
			{
				GameRankDlg* pRankDlg = GameMainOptUI::GetInstance()->GetRankDlg();
				if(!pRankDlg->IsVisible())
				{
					pRankDlg->ShowDialog();
				}
			}
		}
		else if(_index == RT_SMALLQUIT)
		{
			PkgPlayerUserDataReq req;
			req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);

			pTheGame->RequestSmallQuit();
		}
		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
		SetVisible(false);
	}
}