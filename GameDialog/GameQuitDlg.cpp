#include "GameQuitDialog.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
#define QUITDLG_WIDTH	350
#define QUITDLG_HEIGHT	160
//////////////////////////////////////////////////////////////////////////
GameQuitDlg::GameQuitDlg()
{
	m_rcClient.left = WINDOW_WIDTH / 2 - QUITDLG_WIDTH / 2;
	m_rcClient.right = m_rcClient.left + QUITDLG_WIDTH;
	m_rcClient.top = 150;
	m_rcClient.bottom = m_rcClient.top + QUITDLG_HEIGHT;

	SetVisible(false);
	//SetFixed();
	SetStatic(true);
	SetCanDrag(false);
	
	CalcCloseButtonPosition();

	m_pRender = NEW_EMPTY_SPRITE;
}

void GameQuitDlg::CreateQuitDlg()
{
	SetWindowTitle("退出确认");
	SetWindowInfo("退出前请存档,存档会绑定物品");
	RemoveCommonButton(QDB_OK);
	RemoveCommonButton(QDB_QUIT);
	RemoveCommonButton(QDB_SAVE);
	AddCommonButton(CBTN_NORMAL, QDB_QUIT, QUITDLG_WIDTH / 2 + 20, 100, "直接退出");
	AddCommonButton(CBTN_NORMAL, QDB_SAVE, QUITDLG_WIDTH / 2 - 20 - 64, 100, "保存存档");

	ShowDialog();
}

void GameQuitDlg::CreateMsgDlg(const char* _pszMsg)
{
	RemoveCommonButton(QDB_OK);
	RemoveCommonButton(QDB_QUIT);
	RemoveCommonButton(QDB_SAVE);
	AddCommonButton(CBTN_NORMAL, QDB_OK, QUITDLG_WIDTH / 2 - 64 / 2, 100, "确定");

	SetWindowTitle("提示");
	SetWindowInfo(_pszMsg);

	ShowDialog();
}

void GameQuitDlg::CreateSaveDlg()
{
	SetWindowTitle("确认存档");
	SetWindowInfo("存档会绑定所有的物品");

	RemoveCommonButton(QDB_OK);
	RemoveCommonButton(QDB_QUIT);
	RemoveCommonButton(QDB_SAVE);
	AddCommonButton(CBTN_NORMAL, QDB_SAVE, QUITDLG_WIDTH / 2 - 64 / 2, 100, "保存存档");

	ShowDialog();
}

bool GameQuitDlg::OnCommonButtonClick(int _id)
{
	if(_id == QDB_QUIT)
	{
		if(pTheGame->GetGameMode() == GM_LOGIN)
		{
			GamePlayer::GetInstance()->WriteBagItemOrder();
		}
		
		PostQuitMessage(0);
	}
	else if(_id == QDB_SAVE)
	{
		PkgPlayerUserDataReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);

		SetWindowInfo("正在请求服务器数据...");
	}
	else if(_id == QDB_OK)
	{
		CloseDialog();
	}

	return true;
}

bool GameQuitDlg::ProcUserCmd(const POINT& _mp)
{
	return __super::ProcUserCmd(_mp);
}