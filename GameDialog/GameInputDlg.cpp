#include "GameInputDlg.h"
#include "../BackMir/BackMir.h"
#include "GameInfoBoardDlg.h"
//////////////////////////////////////////////////////////////////////////
#define DLG_WIDTH	300
#define DLG_HEIGHT	200

#define EDIT_WIDTH	164

#define EDIT_LEFT	(DLG_WIDTH - EDIT_WIDTH) / 2
#define EDIT_TOP	(110+20)

#define TEXT_LEFT	20
#define TEXT_TOP	(80+20)

#define BID_OK		1
#define BID_CANCEL	0

#define TID_LINEEDIT	0

#define BUTTON_GAP	40
#define BUTTON_TOP	(140+20)
//////////////////////////////////////////////////////////////////////////
int GameInputDlg::Callback_OnCharInput_Number(char _cKey)
{
	if(_cKey >= '0' &&
		_cKey <= '9')
	{
		return 0;
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////
GameInputDlg::GameInputDlg()
{
	SetCanDrag(false);
	SetStatic(true);

	m_rcClient.right = DLG_WIDTH;
	m_rcClient.bottom = DLG_HEIGHT;
	int nCenterX = (WINDOW_WIDTH - DLG_WIDTH) / 2;
	int nCenterY = (WINDOW_HEIGHT - VIEW_OPERATE - DLG_HEIGHT) / 2;
	SetBkStype(1);
	CalcCloseButtonPosition();
	MoveTo(nCenterX, nCenterY);

	m_pEdit = new MyGfxEdit(EDIT_WIDTH, ARGB_WHITE);
	m_pEdit->SetRenderBorder(false);
	m_pRender = NEW_EMPTY_SPRITE;

	AddCommonButton(CBTN_NORMAL, BID_OK, (DLG_WIDTH - 75 * 2 - BUTTON_GAP) / 2, BUTTON_TOP, "确定");
	AddCommonButton(CBTN_NORMAL, BID_CANCEL, (DLG_WIDTH - 75 * 2 - BUTTON_GAP) / 2 + BUTTON_GAP + 75, BUTTON_TOP, "取消");
	AddStaticTexture(TID_LINEEDIT, EDIT_LEFT - 8, EDIT_TOP - 10, 10);

	SetWindowTitle("询问");

	SetEditVisible(true);
	SetVisible(false);

	m_eDlgType = InputDlg_None;
}

GameInputDlg::~GameInputDlg()
{
	SAFE_DELETE(m_pEdit);
}

bool GameInputDlg::ProcUserCmd(const POINT& _mp)
{
	if(m_bEditVisible)
	{
		m_pEdit->Updata();
	}

	if(AfxGetHge()->Input_KeyUp(HGEK_ESCAPE))
	{
		OnCommonButtonClick(BID_CANCEL);
		return true;
	}
	else if(AfxGetHge()->Input_KeyUp(HGEK_ENTER))
	{
		OnCommonButtonClick(BID_OK);
		return true;
	}

	return __super::ProcUserCmd(_mp);
}

void GameInputDlg::Render()
{
	__super::Render();

	if(m_bEditVisible)
	{
		m_pEdit->Render(RELATIVE_X(EDIT_LEFT), RELATIVE_Y(EDIT_TOP));
	}

	if(m_bTextVisible &&
		!m_xText.empty())
	{
		AfxGetFont()->SetColor(ARGB_WHITE);
		AfxGetFont()->Print(RELATIVE_X(TEXT_LEFT), RELATIVE_Y(TEXT_TOP), m_xText.c_str());
	}
}

bool GameInputDlg::OnCommonButtonClick(int _id)
{
	if(_id == BID_CANCEL)
	{
		CloseDialog();
	}
	else if(_id == BID_OK)
	{
		CloseDialog();

		if(InputDlg_QueryBuySum == m_eDlgType)
		{
			OnQueryBuySumConfirm();
		}
		else if(InputDlg_QueryDropSum == m_eDlgType)
		{
			OnQueryDropSumConfirm();
		}
		else if(InputDlg_QuerySplitSum == m_eDlgType)
		{
			OnQuerySplitSumConfirm();
		}
		else if(InputDlg_QueryOffBuy == m_eDlgType)
		{
			OnQueryOffBuyConfirm();
		}
		else if(InputDlg_QueryOffTackBack == m_eDlgType)
		{
			OnQueryOffTackOffConfirm();
		}
		else if(InputDlg_QueryOlShopBuy == m_eDlgType)
		{
			OnQueryOlShopBuyConfirm();
		}
	}

	return true;
}

bool GameInputDlg::OnShowDialog()
{
	//m_pEdit->ClearCookie();
	m_pEdit->SetFocus();
	GameMainOptUI::GetInstance()->GetIntroDlg()->ShowNothing();

	return true;
}

bool GameInputDlg::OnCloseDialog()
{
	m_pEdit->KillFocus();
	return true;
}

void GameInputDlg::ShowAsMode(InputDlg_Type _eType, int _nParam0 /* = 0 */, int _nParam1 /* = 0 */, int _nParam2 /* = 0 */, int _nParam3 /* = 0 */)
{
	m_eDlgType = _eType;
	SetTextVisible(false);
	SetEditVisible(false);
	m_pEdit->SetOnCharInputCallback(NULL);

	m_nParam0 = _nParam0;
	m_nParam1 = _nParam1;
	m_nParam2 = _nParam2;
	m_nParam3 = _nParam3;

	if(_eType == InputDlg_QueryBuySum)
	{
		SetTextVisible(true);
		SetText("请输入要购买的数量:");
		SetEditVisible(true);
		m_pEdit->SetOnCharInputCallback(&GameInputDlg::Callback_OnCharInput_Number);
		m_pEdit->ClearCookie();
		char szNumber[5] = {0};
		itoa(GRID_MAX, szNumber, 10);
		m_pEdit->InsertCookie(szNumber);
	}
	else if(_eType == InputDlg_QueryDropSum)
	{
		//	param0:item tag		param1:item sum
		SetTextVisible(true);
		SetText("请输入要丢弃的物品数量:");
		SetEditVisible(true);
		m_pEdit->SetOnCharInputCallback(&GameInputDlg::Callback_OnCharInput_Number);
		m_pEdit->ClearCookie();
		char szNumber[5] = {0};
		itoa(m_nParam1, szNumber, 10);
		m_pEdit->InsertCookie(szNumber);
	}
	else if(_eType == InputDlg_QuerySplitSum)
	{
		//	param0:item tag		param1:split sum
		SetTextVisible(true);
		SetText("请输入要拆分的物品数量:");
		SetEditVisible(true);
		m_pEdit->SetOnCharInputCallback(&GameInputDlg::Callback_OnCharInput_Number);
		m_pEdit->ClearCookie();
		m_pEdit->InsertCookie("1");
	}
	else if(_eType == InputDlg_QueryOffBuy)
	{
		SetEditVisible(false);
		SetTextVisible(true);
		char szBuf[MAX_PATH];
		sprintf(szBuf, "确认要购买 [%s], 价值 %d?",
			(const char*)_nParam0, _nParam1);
		SetText(szBuf);
	}
	else if(_eType == InputDlg_QueryOffTackBack)
	{
		SetEditVisible(false);
		SetTextVisible(true);
		SetText("您确定要收回所有的寄售物品?收回将绑定所有物品。");
	}
	else if(_eType == InputDlg_QueryOlShopBuy)
	{
		SetEditVisible(false);
		SetTextVisible(true);
		char szBuf[MAX_PATH];
		sprintf(szBuf, "确认要购买 [%s], 价值 %d %s?",
			(const char*)_nParam0, _nParam1, pTheGame->GetGameMode() == GM_LOGIN ? "贡献币" : "金币");
		SetText(szBuf);
	}

	ShowDialog();
}

void GameInputDlg::OnQueryOffBuyConfirm()
{
	PkgPlayerOffBuyItemReq req;
	req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
	req.dwItemID = m_nParam2;
	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(g_xBuffer);
}

void GameInputDlg::OnQuerySplitSumConfirm()
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	const char* pszNumber = m_pEdit->GetCookie();
	int nSum = atoi(pszNumber);

	if(nSum < 0 ||
		nSum >= m_nParam1)
	{
		GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "请输入 1-%d 内的数量", m_nParam1 - 1);
		return;
	}

	PkgPlayerSplitItemReq req;
	req.dwTag = m_nParam0;
	req.cSum = nSum;
	req.uUserId = pPlayer->GetHandlerID();
	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(&g_xBuffer);
}

void GameInputDlg::OnQueryDropSumConfirm()
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	const char* pszNumber = m_pEdit->GetCookie();
	int nSum = atoi(pszNumber);

	if(nSum > GRID_MAX)
	{
		nSum = GRID_MAX;
	}
	else if(nSum <= 0)
	{
		GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "请输入 1-%d 内的数量", m_nParam1);
		return;
	}

	if(0 == GamePlayer::GetInstance()->GetPlayerBag()->GetEmptyBagSum())
	{
		GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "请留下 1 个空格");
		return;
	}

	for(int i = 0; i < nSum; ++i)
	{
		PkgPlayerDropItemReq req;
		req.dwId = m_nParam0;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
	}
}

void GameInputDlg::OnQueryBuySumConfirm()
{
	const char* pszNumber = m_pEdit->GetCookie();
	int nSum = atoi(pszNumber);

	if(nSum > 0)
	{
		if(nSum > 255)
		{
			nSum = 255;
		}

		int nGridNeed = nSum / GRID_MAX;
		if(nSum % GRID_MAX != 0)
		{
			++nGridNeed;
		}

		if(nGridNeed > GamePlayer::GetInstance()->GetPlayerBag()->GetEmptyBagSum())
		{
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "请留下 %d 个空格", nGridNeed);
			return;
		}

		int nTotalPrice = m_nParam2 * nSum;

		if(GamePlayer::GetInstance()->GetPlayerBag()->GetMoney() >= nTotalPrice)
		{
			PkgPlayerShopOpReq req;
			req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
			req.uTargetId = m_nParam0;
			req.bOp = SHOP_BUY;
			req.dwData = m_nParam1;
			req.bNumber = nSum;

			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
		}
		else
		{
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "您没有足够的金币");
		}
	}
}

void GameInputDlg::OnQueryOffTackOffConfirm()
{
	PkgPlayerOffTakeBackReq req;
	req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(g_xBuffer);
}

void GameInputDlg::OnQueryOlShopBuyConfirm()
{
	PkgPlayerBuyOlShopItemReq req;
	req.nItemId = m_nParam2;
	req.cCount = 1;
	req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(g_xBuffer);
}