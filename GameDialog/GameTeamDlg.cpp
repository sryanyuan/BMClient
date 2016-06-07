#include "GameTeamDlg.h"
//////////////////////////////////////////////////////////////////////////
static const int s_nWindowWidth = 550;
static const int s_nWindowHeight = 350;
//////////////////////////////////////////////////////////////////////////
GameTeamDlg::GameTeamDlg()
{
	m_rcClient.left = WINDOW_WIDTH / 2 - s_nWindowWidth / 2;
	m_rcClient.right = m_rcClient.left + s_nWindowWidth;
	m_rcClient.top = 50;
	m_rcClient.bottom = m_rcClient.top + s_nWindowHeight;
	
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);

	CalcCloseButtonPosition();
	SetVisible(false);
	SetWindowTitle("∂”ŒÈ");
}

GameTeamDlg::~GameTeamDlg()
{
	SAFE_DELETE(m_pRender);
}

void GameTeamDlg::Render()
{
	__super::Render();
}

bool GameTeamDlg::ProcUserCmd(const POINT& _ptMouse)
{
	__super::ProcUserCmd(_ptMouse);

	return true;
}