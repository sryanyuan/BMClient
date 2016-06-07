#include "GameRoleInfoDlg.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
#include "GameChatDlg.h"
//////////////////////////////////////////////////////////////////////////
GameTeamMenu::GameTeamMenu()
{
	m_rcClient.right = 52;
	m_rcClient.bottom = 70;
	m_nPlayerID = 0;

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);

	SetCustomBk();
	SetVisible(false);

	AddCommonButton(CBTN_NORMAL, 0, 2, 5, "邀请", 9);
	AddCommonButton(CBTN_NORMAL, 1, 2, 25, "加入", 9);
	AddCommonButton(CBTN_NORMAL, 2, 2, 45, "退出", 9);

	Move(0, 100);
}

void GameTeamMenu::Render()
{
	HTEXTURE tex = 0;
	int nRenderX = 0;
	int nRenderY = 0;
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	tex = pPngMgr->GetTexture(8);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 52, 70);
		nRenderX = RELATIVE_X(0);
		nRenderY = RELATIVE_Y(0);
		m_pRender->Render(nRenderX, nRenderY);
	}

	__super::Render();
}

bool GameTeamMenu::OnCommonButtonClick(int _id)
{
	CloseDialog();

	if(0 == m_nPlayerID)
	{
		return true;
	}

	if(_id == 2)
	{
		PkgPlayerSpeOperateReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		req.dwOp = CMD_OP_QUITTEAM;
		req.dwParam = 0;
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
	}
	else if(0 == _id)
	{
		PkgPlayerSpeOperateReq req;
		req.uUserId = GETSELFPLAYERHANDLERID;
		req.dwOp = CMD_OP_ADDTEAMMATE;
		req.dwParam = m_nPlayerID;
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
	}
	
	return true;
}
//////////////////////////////////////////////////////////////////////////
static const int s_nMaleOftX = -12;
static const int s_nMaleOftY = -30;
static const int s_nFemaleOftX = 0;
static const int s_nFemaleOftY = -28;
//////////////////////////////////////////////////////////////////////////
//	true 男的
//////////////////////////////////////////////////////////////////////////
GameRoleInfoDlg::GameRoleInfoDlg()
{
	m_nPlayerID = 0;
	m_nPlayerLevel = 0;
	m_cPlayerSex = 0;

	m_rcClient.right = 219;
	m_rcClient.bottom = 81;

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);

	AddCommonButton(CBTN_NORMAL, 0, 73, 24, "查看", 4);
	AddCommonButton(CBTN_NORMAL, 1, 23, 61, "组队", 5);
	AddCommonButton(CBTN_NORMAL, 2, 73+32, 24, "私聊", 4);
	SetCustomBk();
	SetVisible(false);
}

void GameRoleInfoDlg::ShowWithPlayerID(int _nID)
{
	m_nPlayerID = _nID;
}

void GameRoleInfoDlg::ShowWithPlayer(GameObject* _pObj)
{
	m_xPlayerName = _pObj->GetAttrib()->name;
	m_nPlayerLevel = _pObj->GetAttrib()->level;
	m_cPlayerSex = _pObj->GetAttrib()->sex;
	m_nPlayerID = _pObj->GetHandlerID();

	ShowDialog();
	MoveTo(15, 30);
}

void GameRoleInfoDlg::Render()
{
	/*if(AfxGetHge()->Input_KeyUp(HGEK_LEFT))
	{
		s_nFemaleOftX -= 1;
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_RIGHT))
	{
		s_nFemaleOftX += 1;
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_UP))
	{
		s_nFemaleOftY -= 1;
	}
	if(AfxGetHge()->Input_KeyUp(HGEK_DOWN))
	{
		s_nFemaleOftY += 1;
	}

	AfxGetPrinter()->Print(0, 0, "coordinate %d,%d", s_nFemaleOftX, s_nFemaleOftY);*/

	HTEXTURE tex = 0;
	int nRenderX = 0;
	int nRenderY = 0;
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	tex = pPngMgr->GetTexture(3);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 219, 81);
		nRenderX = RELATIVE_X(0);
		nRenderY = RELATIVE_Y(0);
		m_pRender->Render(nRenderX, nRenderY);
	}

	if(m_cPlayerSex == 1)
	{
		tex = pPngMgr->GetTexture(7);
	}
	else
	{
		tex = pPngMgr->GetTexture(6);
	}
	if(tex)
	{
		if(m_cPlayerSex == 1)
		{
			nRenderX = RELATIVE_X(s_nMaleOftX);
			nRenderY = RELATIVE_Y(s_nMaleOftY);
		}
		else
		{
			nRenderX = RELATIVE_X(s_nFemaleOftX);
			nRenderY = RELATIVE_Y(s_nFemaleOftY);
		}
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 80, 100);
		m_pRender->Render(nRenderX, nRenderY);
	}

	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->Print(RELATIVE_X(73), RELATIVE_Y(48), "%d", m_nPlayerLevel);

	nRenderY = RELATIVE_Y(8);
	nRenderX = RELATIVE_X(100 + 50 - GetTextWidth(m_xPlayerName.c_str(), 12) / 2);
	AfxGetPrinter()->Print(nRenderX, nRenderY, m_xPlayerName.c_str());

	__super::Render();
}

bool GameRoleInfoDlg::OnCommonButtonClick(int _id)
{
	if(0 == _id)
	{
		if(0 != m_nPlayerID)
		{
			PkgPlayerSpeOperateReq req;
			req.uUserId = GETSELFPLAYERHANDLERID;
			req.dwOp = CMD_OP_VIEWEQUIP;
			req.dwParam = m_nPlayerID;
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
		}
	}
	else if(1 == _id)
	{
		GameTeamMenu* pTeamMenu = GameMainOptUI::GetInstance()->GetTeamMenu();
		bool bVisible = pTeamMenu->IsVisible();

		if(bVisible)
		{
			pTeamMenu->CloseDialog();
		}
		else
		{
			pTeamMenu->ShowDialog();
			pTeamMenu->SetPlayerID(m_nPlayerID);
			OnMove(0, 0);
		}
	}
	else if(2 == _id)
	{
		if(m_xPlayerName.empty())
		{
			return true;
		}

		GameLineEdit* pEdit = GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit();
		pEdit->GainFocus();
		pEdit->ClearText();
		pEdit->SetText("/");
		pEdit->SetText(m_xPlayerName.c_str());
		pEdit->SetText(" ");
		pEdit->SetVisible(true);
	}
	return true;	
}

bool GameRoleInfoDlg::OnShowDialog()
{
	GameTeamMenu* pTeamMenu = GameMainOptUI::GetInstance()->GetTeamMenu();
	pTeamMenu->CloseDialog();

	return true;
}

bool GameRoleInfoDlg::OnCloseDialog()
{
	GameTeamMenu* pTeamMenu = GameMainOptUI::GetInstance()->GetTeamMenu();
	pTeamMenu->CloseDialog();
	m_nPlayerID = 0;
	return true;
}

void GameRoleInfoDlg::OnMove(int _nOftX, int _nOftY)
{
	int nLeft = 40 - 52 / 2;
	int nTop = RECT_HEIGHT(m_rcClient);

	GameTeamMenu* pTeamMenu = GameMainOptUI::GetInstance()->GetTeamMenu();
	pTeamMenu->MoveTo(RELATIVE_X(nLeft), RELATIVE_Y(nTop));
}