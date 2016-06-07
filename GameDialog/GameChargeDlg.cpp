#include "GameChargeDlg.h"
#include "GameDlgBase.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
#include "../GameDialog/GameInfoBoardDlg.h"
//////////////////////////////////////////////////////////////////////////
#define BID_NEXT	1
//////////////////////////////////////////////////////////////////////////
int GameChargeDlg::Callback_OnCharInput_Number(char _cKey)
{
	if(_cKey >= '0' &&
		_cKey <= '9')
	{
		return 0;
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////
GameChargeDlg::GameChargeDlg()
{
	m_rcClient.right = 360 + 12;
	m_rcClient.bottom = 420 + 80;

	SetWindowTitle("捐助游戏");

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_pEdit = new MyGfxEdit(235, ARGB_BLACK);
	m_pEdit->SetRenderBorder(false);
	m_pEdit->SetOnCharInputCallback(&GameChargeDlg::Callback_OnCharInput_Number);

	AddCommonButton(CBTN_NORMAL, BID_NEXT, RECT_WIDTH(m_rcClient) / 2 - 35, m_rcClient.bottom - 45, "下一步");

	SetBkStype(1);
	CalcCloseButtonPosition();
	CenterWindow();
	Move(0, -50);

	ResetData();

	SetVisible(false);
}

GameChargeDlg::~GameChargeDlg()
{
	SAFE_DELETE(m_pRender);
	SAFE_DELETE(m_pEdit);
}

void GameChargeDlg::ResetData()
{
	m_ePage = kChargePage_Charge;
}

#define OFFSET_X_BEGIN	35
#define OFFSET_Y_BEGIN	80

bool GameChargeDlg::OnShowDialog()
{
	ResetData();

	return true;
}

bool GameChargeDlg::OnCommonButtonClick(int _id)
{
	if(_id == BID_NEXT)
	{
		if(m_ePage == kChargePage_Charge)
		{
			m_ePage = kChargePage_EnterOrderId;
		}
		else if(m_ePage == kChargePage_EnterOrderId)
		{
			//	send request
			const char* pszContent = m_pEdit->GetCookie();
			if(strlen(pszContent) > 32)
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "您输入的交易号不正确");
				return true;
			}

			PkgPlayerChargeReq req;
			req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
			req.xOrderId = pszContent;
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(g_xBuffer);
		}
	}
	return true;
}

void GameChargeDlg::Render()
{
	__super::Render();

	if(m_ePage == kChargePage_Charge)
	{
		RenderPageCharge();
	}
	else if(m_ePage == kChargePage_EnterOrderId)
	{
		RenderPageOrderId();
	}
}

bool GameChargeDlg::ProcUserCmd(const POINT& _mp)
{
	bool bRet = __super::ProcUserCmd(_mp);

	if(m_ePage == kChargePage_Charge)
	{
		ProcPageCharge(_mp);
	}
	else if(m_ePage == kChargePage_EnterOrderId)
	{
		ProcPageOrderId(_mp);
	}

	return bRet;
}

void GameChargeDlg::RenderPageCharge()
{
	HTEXTURE tex = 0;
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	tex = pPngMgr->GetTexture(35);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 320, 360);
		m_pRender->Render(RELATIVE_X(OFFSET_X_BEGIN), RELATIVE_Y(OFFSET_Y_BEGIN));
	}
}

void GameChargeDlg::ProcPageCharge(const POINT& _mp)
{

}

void GameChargeDlg::RenderPageOrderId()
{
	HTEXTURE tex = 0;
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	tex = pPngMgr->GetTexture(36);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 320, 360);
		m_pRender->Render(RELATIVE_X(OFFSET_X_BEGIN), RELATIVE_Y(OFFSET_Y_BEGIN));
	}

	m_pEdit->Render(RELATIVE_X(OFFSET_X_BEGIN + 45), RELATIVE_Y(OFFSET_Y_BEGIN + 58));
}

void GameChargeDlg::ProcPageOrderId(const POINT& _mp)
{
	m_pEdit->Updata();
}