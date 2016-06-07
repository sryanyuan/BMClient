#include "../LoginScene/SelChrScene.h"
#include "../Common/GlobalDefine.h"
#include "../Common/GlobalFunction.h"
#include <hgeresource.h>

//////////////////////////////////////////////////////////////////////////
SelChrScene::SelChrScene()
: WidgetBox(NULL)
{
	//
	m_cCurSel = -1;
}

SelChrScene::~SelChrScene()
{
	//
}

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* Init
/************************************************************************/
bool SelChrScene::Init(hgeResourceManager *_pRes)
{
	m_pxResMgr = _pRes;
	DWORD dwEach = VIEW_WIDTH / 6;
	DWORD dwButtonHalf = 75;

	m_pxBtnDC = new sgui::Button(this);
	m_pxBtnDC->SetWidth(149);
	m_pxBtnDC->SetHeight(26);
	m_pxBtnDC->SetColor(0xFFFFFFFF);
	m_pxBtnDC->SetTexture(_pRes->GetTexture("buttonani"));
	m_pxBtnDC->SetText("战士");
	m_pxBtnDC->MoveWidgetTo(dwEach - dwButtonHalf, 25);
	AddChild(m_pxBtnDC);

	m_pxBtnSC = new sgui::Button(this);
	m_pxBtnSC->SetWidth(149);
	m_pxBtnSC->SetHeight(26);
	m_pxBtnSC->SetColor(0xFFFFFFFF);
	m_pxBtnSC->SetTexture(_pRes->GetTexture("buttonani"));
	m_pxBtnSC->SetText("道士");
	m_pxBtnSC->MoveWidgetTo(dwEach * 3 - dwButtonHalf, 25);
	AddChild(m_pxBtnSC);

	m_pxBtnMC = new sgui::Button(this);
	m_pxBtnMC->SetWidth(149);
	m_pxBtnMC->SetHeight(26);
	m_pxBtnMC->SetColor(0xFFFFFFFF);
	m_pxBtnMC->SetTexture(_pRes->GetTexture("buttonani"));
	m_pxBtnMC->SetText("法师");
	m_pxBtnMC->MoveWidgetTo(dwEach * 5 - dwButtonHalf, 25);
	AddChild(m_pxBtnMC);

	m_pxBtnOk = new sgui::Button(this);
	m_pxBtnOk->SetWidth(149);
	m_pxBtnOk->SetHeight(26);
	m_pxBtnOk->SetColor(0xFFFFFFFF);
	m_pxBtnOk->SetTexture(_pRes->GetTexture("buttonani"));
	m_pxBtnOk->SetText("进入游戏");
	m_pxBtnOk->MoveWidgetTo(dwEach * 3 - dwButtonHalf, 500);
	AddChild(m_pxBtnOk);

	m_pxImgBk = new sgui::Image(this);
	m_pxImgBk->SetWidth(816);
	m_pxImgBk->SetHeight(540);
	m_pxImgBk->SetTexture(_pRes->GetTexture("selchr"));
	AddChild(m_pxImgBk);

//	char szFile[MAX_PATH];
//	m_pxTexMgr = new GameTextureManager;
//	sprintf(szFile, "%s\\Data\\chrsel.wil", GetRootPath());
//	if(!m_pxTexMgr->Load(szFile))
	{
		return false;
	}

	return true;
}


/************************************************************************/
/* OnChildNotify
/************************************************************************/
BOOL SelChrScene::OnChildNotify(WORD _wID, DWORD _dwNot, DWORD _dwMsg)
{
	if(_dwNot == NOTIFY_LBUTTONUP)
	{
		if(_wID == m_pxBtnDC->GetID())
		{
			m_cCurSel = 0;
		}
		else if(_wID == m_pxBtnSC->GetID())
		{
			m_cCurSel = 1;
		}
		else if(_wID == m_pxBtnMC->GetID())
		{
			m_cCurSel = 2;
		}
	}
	return true;
}

/************************************************************************/
/* Render
/************************************************************************/
void SelChrScene::Render()
{
	__super::Render();

	static const int s_nRenderX[] = {100, 300, 500};

	/*HTEXTURE tex = 0;
	if(m_cCurSel == -1)
	{
		tex = m_pxTexMgr->GetTexture(TEX_ANI_DC_STONE_BEGIN);
		if(tex)
		{
			s_pSpr->SetTexture(tex);
			s_pSpr->SetTextureRect(0, 0, m_pxTexMgr->GetTextureWidthEx(TEX_ANI_DC_STONE_BEGIN),
				m_pxTexMgr->GetTextureHeightEx(TEX_ANI_DC_STONE_END));
			s_pSpr->Render(s_nRenderX[0], 200);
		}
	}*/
}