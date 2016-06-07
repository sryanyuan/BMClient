#include "HumStateDlg.h"
#include "../GameScene/GameResourceManager.h"
#include "../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
HumStateDlg::HumStateDlg()
{
	SetFixed();
	SetCustomBk();
	m_rcClient.left = 0;
	m_rcClient.right = 176;
	m_rcClient.top = 0;
	m_rcClient.bottom = 82;

	m_pRender = NEW_EMPTY_SPRITE;
	SetVisible(false);
}

HumStateDlg::~HumStateDlg()
{
	SAFE_DELETE(m_pRender);
}

//////////////////////////////////////////////////////////////////////////
void HumStateDlg::Render()
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	HTEXTURE tex = 0;

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	tex = pTexMgr->GetTexture(36);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 176, 82);
		m_pRender->Render(0, 0);
	}

	static int nNameWidth = 0;
	if(nNameWidth == 0)
	{
		nNameWidth = GetTextWidth(pPlayer->GetAttrib()->name, 12);
	}
	int nRenderX = 98 / 2 - nNameWidth / 2;
	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->Print(75 + nRenderX, 6, pPlayer->GetAttrib()->name);

	int nFaceIndex = pPlayer->GetHeroJob() + 30;
	if(pPlayer->GetAttrib()->sex == 2)
	{
		nFaceIndex += 3;
	}
	tex = pTexMgr->GetTexture(nFaceIndex);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 56, 54);
		m_pRender->Render(17, 18);
	}

	float fSale = 0.0f;
	if(pPlayer->GetAttrib()->maxHP != 0)
	{
		fSale = (float)pPlayer->GetAttrib()->HP / pPlayer->GetAttrib()->maxHP;
		if(fSale > 1.0f)
		{
			fSale = 1.0f;
		}

		tex = pTexMgr->GetTexture(37);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 96 * fSale, 6);
			m_pRender->Render(75, 24);
		}
	}

	if(pPlayer->GetAttrib()->maxMP != 0)
	{
		fSale = (float)pPlayer->GetAttrib()->MP / pPlayer->GetAttrib()->maxMP;
		if(fSale > 1.0f)
		{
			fSale = 1.0f;
		}

		tex = pTexMgr->GetTexture(38);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 92 * fSale, 6);
			m_pRender->Render(81, 37);
		}
	}

	if(pPlayer->GetAttrib()->maxEXPR != 0)
	{
		fSale = (float)pPlayer->GetAttrib()->EXPR / pPlayer->GetAttrib()->maxEXPR;
		if(fSale > 1.0f)
		{
			fSale = 1.0f;
		}

		tex = pTexMgr->GetTexture(39);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 92 * fSale, 6);
			m_pRender->Render(80, 50);
		}
	}

	if(pPlayer->GetAttrib()->level >= 10)
	{
		AfxGetPrinter()->Print(7, 60, "%d", pPlayer->GetAttrib()->level);
	}
	else
	{
		AfxGetPrinter()->Print(10, 60, "%d", pPlayer->GetAttrib()->level);
	}
}

bool HumStateDlg::ProcUserCmd(const POINT& _mp)
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	GameIntroDlg* pIntroDlg = GameScene::sThis->GetMainOpt()->GetIntroDlg();
	pIntroDlg->ShowNothing();

	char szInfo[100];
	if(_mp.x > 75 &&
		_mp.x < 75 + 96 &&
		_mp.y > 24 &&
		_mp.y < 24 + 6)
	{
		sprintf(szInfo, "HP:%d/%d",
			pPlayer->GetAttrib()->HP,
			pPlayer->GetAttrib()->maxHP);
		pIntroDlg->ShowString(szInfo);
	}
	else if(_mp.x > 81 &&
		_mp.x < 81 + 92 &&
		_mp.y > 37 &&
		_mp.y < 37 + 6)
	{
		sprintf(szInfo, "MP:%d/%d",
			pPlayer->GetAttrib()->MP,
			pPlayer->GetAttrib()->maxMP);
		pIntroDlg->ShowString(szInfo);
	}
	else if(_mp.x > 80 &&
		_mp.x < 80 + 92 &&
		_mp.y > 50 &&
		_mp.y < 50 + 6)
	{
		sprintf(szInfo, "EXP:%d/%d",
			pPlayer->GetAttrib()->EXPR,
			pPlayer->GetAttrib()->maxEXPR);
		pIntroDlg->ShowString(szInfo);
	}
	return true;
}