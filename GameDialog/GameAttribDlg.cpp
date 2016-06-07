#include "GameAttribDlg.h"
#include "GameDlgBase.h"
#include "../GameScene/GameResourceManager.h"
//////////////////////////////////////////////////////////////////////////
#define FONTCOLOR_ATTRIB	0xff857e54
#define OFFSET_ATTRIBNAME_X			8
#define OFFSET_ATTRIBVALUE_LEFT		60
#define OFFSET_ATTRIBVALUE_RIGHT	180
//////////////////////////////////////////////////////////////////////////
GameAttribDlg::GameAttribDlg()
{
	SetVisible(false);
	m_rcClient.right = 197;
	m_rcClient.bottom = 381;
	SetCustomBk();
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
}

GameAttribDlg::~GameAttribDlg()
{

}

void GameAttribDlg::Render()
{
	__super::Render();

	GamePlayer* pPlayer = GamePlayer::GetInstance();

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	HTEXTURE tex = 0;
	int nRenderX = 0;
	int nRenderY = 0;

	tex = pTexMgr->GetTexture(2);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 197, 381);
		nRenderX = RELATIVE_X(0);
		nRenderY = RELATIVE_Y(0);
		m_pRender->Render(nRenderX, nRenderY);
	}

	AfxGetPrinter()->SetColor(FONTCOLOR_ATTRIB);
	AfxGetPrinter()->Print(RELATIVE_X(OFFSET_ATTRIBNAME_X), RELATIVE_Y(320), "攻    速");
	AfxGetPrinter()->Print(RELATIVE_X(OFFSET_ATTRIBNAME_X), RELATIVE_Y(339), "腕    力");
	AfxGetPrinter()->Print(RELATIVE_X(OFFSET_ATTRIBNAME_X), RELATIVE_Y(359), "移动速度");

	AfxGetPrinter()->SetColor(ARGB_WHITE);
	char szText[MAX_PATH];
	strcpy(szText, pPlayer->GetHeroJobName());
	int nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(40), szText);

	itoa(pPlayer->GetAttrib()->level, szText, 10);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(63), szText);

	sprintf(szText, "%d-%d", pPlayer->GetAttrib()->AC, pPlayer->GetAttrib()->maxAC);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(122), szText);

	sprintf(szText, "%d-%d", pPlayer->GetAttrib()->MAC, pPlayer->GetAttrib()->maxMAC);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(144), szText);

	sprintf(szText, "%d-%d", pPlayer->GetAttrib()->DC, pPlayer->GetAttrib()->maxDC);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(166), szText);

	sprintf(szText, "%d-%d", pPlayer->GetAttrib()->MC, pPlayer->GetAttrib()->maxMC);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(188), szText);

	sprintf(szText, "%d-%d", pPlayer->GetAttrib()->SC, pPlayer->GetAttrib()->maxSC);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(210), szText);

	itoa(pPlayer->GetAttrib()->hide, szText, 10);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(261), szText);

	itoa(pPlayer->GetAttrib()->accuracy, szText, 10);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(280), szText);

	itoa(pPlayer->GetAttrib()->lucky, szText, 10);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(299), szText);

	itoa(pPlayer->GetAttrib()->atkSpeed, szText, 10);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(320), szText);

	itoa(pPlayer->GetWanLi(), szText, 10);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(339), szText);

	itoa(pPlayer->GetAttrib()->moveSpeed, szText, 10);
	nTextLength = GetTextWidth(szText, 12);
	nRenderX = RELATIVE_X(OFFSET_ATTRIBVALUE_LEFT + (OFFSET_ATTRIBVALUE_RIGHT - OFFSET_ATTRIBVALUE_LEFT - nTextLength) / 2);
	AfxGetPrinter()->Print(nRenderX, RELATIVE_Y(359), szText);
}