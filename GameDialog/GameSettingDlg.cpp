#include "GameSettingDlg.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
#define DLG_WIDTH	280
#define DLG_HEIGHT	380

#define SETBTN_ENABLESOUND	0
#define SETBTN_RENDERHPBAR	1
#define SETBTN_ITEMFLASH	2
#define SETBTN_WNDMODE		3
#define SETBTN_SELFNAME		4
#define SETBTN_ENHANDCESEL	5
#define SETBTN_HPANIMATION	6
#define SETBTN_TILESURFACE	8
#define SETBTN_MOUSEPICKUP	9
#define SETBTN_DISABLENAMEFRAME	10
#define SETBTN_DISABLECHATFRAME 11
//////////////////////////////////////////////////////////////////////////
GameSettingDlg::GameSettingDlg()
{
	m_pRender = NEW_EMPTY_SPRITE;
	m_rcClient.left = WINDOW_WIDTH / 2 - DLG_WIDTH / 2;
	m_rcClient.right = m_rcClient.left + DLG_WIDTH;
	m_rcClient.top = 50;
	m_rcClient.bottom = m_rcClient.top + DLG_HEIGHT;
	m_nCurPage = -1;

	SetVisible(false);
	SetBkStype(1);
	CalcCloseButtonPosition();

	AddCommonButton(CBTN_CHECKBOX, SETBTN_ENABLESOUND, 18, 80, "开启声音");
	AddCommonButton(CBTN_CHECKBOX, SETBTN_RENDERHPBAR, 98, 80, "血量显示");
	AddCommonButton(CBTN_CHECKBOX, SETBTN_ITEMFLASH, 178, 80, "物品闪烁");

	AddCommonButton(CBTN_CHECKBOX, SETBTN_WNDMODE, 18, 98, "窗口模式");
	AddCommonButton(CBTN_CHECKBOX, SETBTN_SELFNAME, 98, 98, "自身人名");
	AddCommonButton(CBTN_CHECKBOX, SETBTN_ENHANDCESEL, 178, 98, "强化选取");

	AddCommonButton(CBTN_CHECKBOX, SETBTN_HPANIMATION, 18, 116, "飘血动画");
	AddCommonButton(CBTN_CHECKBOX, SETBTN_TILESURFACE, 98, 116, "性能优先");
	AddCommonButton(CBTN_CHECKBOX, SETBTN_MOUSEPICKUP, 178, 116, "鼠标捡物");

	AddCommonButton(CBTN_CHECKBOX, SETBTN_DISABLENAMEFRAME, 18, 132, "隐藏名字装饰");
	AddCommonButton(CBTN_CHECKBOX, SETBTN_DISABLECHATFRAME, 98 + 12 + 15, 132, "隐藏聊天装饰");

	SetWindowTitle("游戏设置");
}

GameSettingDlg::~GameSettingDlg()
{
	SAFE_DELETE(m_pRender);
}

void GameSettingDlg::SwitchPage(int _nPage)
{
	//	First save the content of the previous page...
	if(m_nCurPage != -1)
	{
		SavePageData(m_nCurPage);
		m_nCurPage = _nPage;
	}

	switch (_nPage)
	{
	case 0:
		{
			AddCommonButton(CBTN_CHECKBOX, SETBTN_ENABLESOUND, 18, 80, "开启声音");
			AddCommonButton(CBTN_CHECKBOX, SETBTN_RENDERHPBAR, 98, 80, "绘制血量数字");
			AddCommonButton(CBTN_CHECKBOX, SETBTN_ITEMFLASH, 178, 80, "物品闪烁");
		}break;
	}
}

void GameSettingDlg::SavePageData(int _nPage)
{
	switch(_nPage)
	{
	case 0:
		{

		}break;
	}
}

void GameSettingDlg::LoadPageData(int _nPage)
{
	switch(_nPage)
	{

	}
}

void GameSettingDlg::Render()
{
	__super::Render();
}

bool GameSettingDlg::OnCommonButtonClick(int _id)
{
	return true;
}

bool GameSettingDlg::OnShowDialog()
{
	CommonButton* pBtn = NULL;
	pTheGame->LoadGameSetting();
	SettingLoader* pSettings = pTheGame->GetGameSettings();


	pBtn = GetCommonButtonData(SETBTN_ENABLESOUND);
	if(pBtn)
	{
		if(pSettings->GetIntValue("SOUNDENABLE") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_RENDERHPBAR);
	if(pBtn)
	{
		if(pSettings->GetIntValue("RENDERHPBAR") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_ITEMFLASH);
	if(pBtn)
	{
		if(pSettings->GetIntValue("ITEMFLASH") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_WNDMODE);
	if(pBtn)
	{
		if(pSettings->GetIntValue("WNDMODE") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_SELFNAME);
	if(pBtn)
	{
		if(pSettings->GetIntValue("SELFNAME") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_ENHANDCESEL);
	if(pBtn)
	{
		if(pSettings->GetIntValue("ENHANCESEL") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_HPANIMATION);
	if(pBtn)
	{
		if(pSettings->GetIntValue("HPANIMATION") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_TILESURFACE);
	if(pBtn)
	{
		if(pSettings->GetIntValue("TILESURFACE") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_MOUSEPICKUP);
	if(pBtn)
	{
		if(pSettings->GetIntValue("MOUSEPICKUP") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_DISABLENAMEFRAME);
	if(pBtn)
	{
		if(pSettings->GetIntValue("DISABLERENDERNAMEFRAME") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_DISABLECHATFRAME);
	if(pBtn)
	{
		if(pSettings->GetIntValue("DISABLERENDERCHATFRAME") != 0)
		{
			pBtn->bState = BTN_STATE_CHECKED;
		}
		else
		{
			pBtn->bState = BTN_STATE_UNCHECKED;
		}
	}

	SetNeedTopLevel(true);
	return __super::OnShowDialog();
}

bool GameSettingDlg::OnCloseDialog()
{
	CommonButton* pBtn = NULL;
	SettingLoader* pSettings = pTheGame->GetGameSettings();
	int nChgSndMode = 0;

	pBtn = GetCommonButtonData(SETBTN_ENABLESOUND);
	if(pBtn)
	{
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			pSettings->WriteIntValue("SOUNDENABLE", 1);
			nChgSndMode = 1;
		}
		else
		{
			pSettings->WriteIntValue("SOUNDENABLE", 0);
			nChgSndMode = 2;
		}
	}

	pBtn = GetCommonButtonData(SETBTN_RENDERHPBAR);
	if(pBtn)
	{
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			pSettings->WriteIntValue("RENDERHPBAR", 1);
		}
		else
		{
			pSettings->WriteIntValue("RENDERHPBAR", 0);
		}
	}

	pBtn = GetCommonButtonData(SETBTN_ITEMFLASH);
	if(pBtn)
	{
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			pSettings->WriteIntValue("ITEMFLASH", 1);
		}
		else
		{
			pSettings->WriteIntValue("ITEMFLASH", 0);
		}
	}

	pBtn = GetCommonButtonData(SETBTN_WNDMODE);
	if(pBtn)
	{
		bool bWnd = AfxGetHge()->System_GetState(HGE_WINDOWED);
		bool bSetWnd = true;
		
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			pSettings->WriteIntValue("WNDMODE", 1);
		}
		else
		{
			pSettings->WriteIntValue("WNDMODE", 0);
			bSetWnd = false;
		}

		if(bSetWnd != bWnd)
		{
			AfxGetHge()->System_SetState(HGE_WINDOWED, bSetWnd);
		}
	}

	pBtn = GetCommonButtonData(SETBTN_SELFNAME);
	if(pBtn)
	{
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			pSettings->WriteIntValue("SELFNAME", 1);
		}
		else
		{
			pSettings->WriteIntValue("SELFNAME", 0);
		}
	}

	pBtn = GetCommonButtonData(SETBTN_ENHANDCESEL);
	if(pBtn)
	{
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			pSettings->WriteIntValue("ENHANCESEL", 1);
		}
		else
		{
			pSettings->WriteIntValue("ENHANCESEL", 0);
		}
	}

	pBtn = GetCommonButtonData(SETBTN_HPANIMATION);
	if(pBtn)
	{
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			pSettings->WriteIntValue("HPANIMATION", 1);
		}
		else
		{
			pSettings->WriteIntValue("HPANIMATION", 0);
		}
	}

	pBtn = GetCommonButtonData(SETBTN_TILESURFACE);
	if(pBtn)
	{
		bool bUseTileSurface = false;
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			bUseTileSurface = true;
			pSettings->WriteIntValue("TILESURFACE", 1);
		}
		else
		{
			pSettings->WriteIntValue("TILESURFACE", 0);
		}

		GameMapManager::GetInstance()->SetEnableTileSurface(bUseTileSurface);
		if(bUseTileSurface)
		{
			GameMapManager::GetInstance()->UpdateClientRect();
		}
	}

	pBtn = GetCommonButtonData(SETBTN_MOUSEPICKUP);
	if(pBtn)
	{
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			pSettings->WriteIntValue("MOUSEPICKUP", 1);
		}
		else
		{
			pSettings->WriteIntValue("MOUSEPICKUP", 0);
		}
	}

	pBtn = GetCommonButtonData(SETBTN_DISABLENAMEFRAME);
	if(pBtn)
	{
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			pSettings->WriteIntValue("DISABLERENDERNAMEFRAME", 1);
		}
		else
		{
			pSettings->WriteIntValue("DISABLERENDERNAMEFRAME", 0);
		}
	}

	pBtn = GetCommonButtonData(SETBTN_DISABLECHATFRAME);
	if(pBtn)
	{
		if(pBtn->bState == BTN_STATE_CHECKED)
		{
			pSettings->WriteIntValue("DISABLERENDERCHATFRAME", 1);
		}
		else
		{
			pSettings->WriteIntValue("DISABLERENDERCHATFRAME", 0);
		}
	}

	pTheGame->WriteSettingsToFile();
	pTheGame->UpdateConfigCache();

	if(0 != nChgSndMode)
	{
		if(1 == nChgSndMode)
		{
			GameSoundManager::GetInstancePtr()->ResumeBkSound();
		}
		else
		{
			GameSoundManager::GetInstancePtr()->PauseBkSound();
		}
	}

	return __super::OnCloseDialog();
}