#include "../GameDialog/GameDlgBase.h"
#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
#include "../Common/gfx_utils.h"
//////////////////////////////////////////////////////////////////////////
#define COLOR_SUITNAME			ARGB(255, 178, 234, 97)
#define COLOR_SUITNAMEITEM_ACTIVE	ARGB(255, 152, 199, 146)
#define COLOR_SUITNAMEITEM_UNACTIVE	ARGB(255, 135, 135, 135)
#define COLOR_EXTRAATRRIB_ACTIVE	ARGB(255, 103, 224, 246)
#define COLOR_EXTRAATTRIB_UNACTIVE	ARGB(255, 135, 135, 135)
//////////////////////////////////////////////////////////////////////////
const int nDrawOftY = 14;
//////////////////////////////////////////////////////////////////////////

GameIntroDlg::GameIntroDlg()
{
	m_pSpr = NULL;
	m_pResManager = NULL;
	m_bVisible = false;
	m_nLinefeedNum = 0;
	//m_pFont = NULL;
	ShowNothing();
	m_dwItemNameColor = ARGB_YELLOW;
	m_nColorIndex = 0;

	ZeroMemory(&m_rcClient, sizeof(RECT));
	ZeroMemory(&m_rcItemRect, sizeof(RECT));

	m_nNamePosX = 0;
	ZeroMemory(&m_oriItem, sizeof(ItemAttrib));

	m_pSpr = new hgeSprite(NULL, 0, 0, 0, 0);
}

GameIntroDlg::~GameIntroDlg()
{
	delete m_pSpr;
	m_pSpr = NULL;
}


void GameIntroDlg::Init(hgeResourceManager* _res)
{
	m_pResManager = _res;
	//m_pFont = new GfxFont("宋体", 12, 0, 0, 0);
	//m_pFont->SetKerningHeight(3.0f);
}

void GameIntroDlg::Render()
{
	if(m_rcClient.left == m_rcClient.right ||
		m_rcClient.top == m_rcClient.bottom ||
		m_pResManager == NULL ||
		!m_bVisible)
	{
		return;
	}

	/*if(RenderItemAttribSec() &&
		m_pItemAttrib != NULL)
	{
		return;
	}*/

	float x, y;
	AfxGetHge()->Input_GetMousePos(&x, &y);
	//	调整矩形区域
	RECT rcDrawRect = m_rcClient;
	rcDrawRect.left += (x - 20);
	rcDrawRect.right += (x - 20);
	rcDrawRect.top += (y + 20);
	rcDrawRect.bottom += (y + 20);

	int nDrawOffset = 0;
	if(rcDrawRect.right >= WINDOW_WIDTH)
	{
		nDrawOffset = rcDrawRect.right - WINDOW_WIDTH - 1;
		rcDrawRect.right = WINDOW_WIDTH - 1;
		rcDrawRect.left -= nDrawOffset;
	}
	if(rcDrawRect.bottom >= VIEW_OPERATE + VIEW_HEIGHT)
	{
		nDrawOffset = rcDrawRect.bottom - VIEW_HEIGHT - VIEW_OPERATE - 1;
		rcDrawRect.bottom = VIEW_HEIGHT + VIEW_OPERATE - 1;
		rcDrawRect.top -= nDrawOffset;
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	//	画底色
	HTEXTURE tex = 0;
	//tex = m_pResManager->GetTexture("bmcolor");
	tex = pPngMgr->GetTexture(19);
	if(tex)
	{
		/*
		INITSPRITE(m_pSpr, tex);
				SETSPRITEWH(tex, m_pSpr, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
				m_pSpr->Render(rcDrawRect.left, rcDrawRect.top);*/
		RECT rcScale9 = {9, 9, 9, 9};
		RECT rcTex = {0, 0, 32, 32};
		Gfx_SetRenderState(kGfx_9Path_CenterStretch, 1);
		Gfx_Render9Path(m_pSpr, tex, &rcTex, &rcScale9, rcDrawRect.left, rcDrawRect.top, RECT_WIDTH(m_rcClient), RECT_HEIGHT(m_rcClient));
	}

	//	画边框
	/*
	tex = m_pResManager->GetTexture("horbord");
		if(tex)
		{
			SETSPRITEW(tex, m_pSpr, m_rcClient.right - m_rcClient.left + 1/ *- 2* /);
			m_pSpr->Render( rcDrawRect.left + 1/ *+ 1* /, rcDrawRect.top);
			m_pSpr->Render(rcDrawRect.left + 1/ *+ 1* /, rcDrawRect.bottom);
		}
		tex = m_pResManager->GetTexture("verbord");
		if(tex)
		{
			SETSPRITEH(tex, m_pSpr, m_rcClient.bottom - m_rcClient.top - 1/ * - 2* /);
			m_pSpr->Render(rcDrawRect.left, rcDrawRect.top + 1 + 1);
			m_pSpr->Render(rcDrawRect.right, rcDrawRect.top + 1 + 1);
		}*/
	

	if(m_bShowExp)
	{
		//m_pFont->SetColor(ARGB_YELLOW);
		//m_pFont->Print(rcDrawRect.left + 5, rcDrawRect.top + 5, "经验值:%d / %d", pPlayer->GetAttrib()->EXPR, pPlayer->GetAttrib()->maxEXPR);
		AfxGetPrinter()->SetColor(ARGB_YELLOW);
		AfxGetPrinter()->PrintWithoutStroke(rcDrawRect.left + 5, rcDrawRect.top + 5, "经验值:%d / %d", pPlayer->GetAttrib()->EXPR, pPlayer->GetAttrib()->maxEXPR);
	}
	else if(m_bShowHP)
	{
		//m_pFont->SetColor(ARGB_YELLOW);
		//m_pFont->Print(rcDrawRect.left + 5, rcDrawRect.top + 5, "生命:%d / %d", pPlayer->GetAttrib()->HP, pPlayer->GetAttrib()->maxHP);
		AfxGetPrinter()->SetColor(ARGB_YELLOW);
		AfxGetPrinter()->PrintWithoutStroke(rcDrawRect.left + 5, rcDrawRect.top + 5, "生命:%d / %d", pPlayer->GetAttrib()->HP, pPlayer->GetAttrib()->maxHP);
	}
	else if(m_bShowMP)
	{
		//m_pFont->SetColor(ARGB_YELLOW);
		//m_pFont->Print(rcDrawRect.left + 5, rcDrawRect.top + 5, "魔法:%d / %d", pPlayer->GetAttrib()->MP, pPlayer->GetAttrib()->maxMP);
		AfxGetPrinter()->SetColor(ARGB_YELLOW);
		AfxGetPrinter()->PrintWithoutStroke(rcDrawRect.left + 5, rcDrawRect.top + 5, "魔法:%d / %d", pPlayer->GetAttrib()->MP, pPlayer->GetAttrib()->maxMP);
	}
	else if(!m_strInfo.empty())
	{
		//m_pFont->SetColor(ARGB_YELLOW);
		//m_pFont->Print(rcDrawRect.left + 5, rcDrawRect.top + 5, m_strInfo.c_str());
		AfxGetPrinter()->SetColor(ARGB_YELLOW);
		AfxGetPrinter()->PrintWithoutStroke(rcDrawRect.left + 5, rcDrawRect.top + 5, m_strInfo.c_str());
	}
	else if(!m_xLinefeedStr.empty() &&
		m_nLinefeedNum != 0)
	{
		float fKerning = AfxGetPrinter()->GetKerningHeight();
		AfxGetPrinter()->SetKerningHeight(2.0f);
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->LineFeedRender(rcDrawRect.left + 5, rcDrawRect.top + 5, m_xLinefeedStr.c_str(), m_nLinefeedNum * 12);
		AfxGetPrinter()->SetKerningHeight(fKerning);
	}
}

void GameIntroDlg::Update(float _dt)
{

}

void GameIntroDlg::ShowItemAttrib(ItemAttrib* _item)
{
	return;

	if(!_item)
	{
		m_pItemAttrib = _item;
		return;
	}
	//	Other set null ...
	if(IsVisible() &&
		m_pItemAttrib == _item)
	{
		return;
	}
	ShowNothing();
	m_pItemAttrib = _item;

	if(CalcItemAttribRect())
	{
		return;
	}

	int nLine = 0;
	int nWidth = 0;
	int nTestWidth = 0;
	nWidth = GetTextWidth(_item->name, 12);

	{
		//	名字
		++nLine;
	}

	{
		//	重量
		++nLine;

		nTestWidth = 7 * 6 + GetNumberBit(_item->weight) * 6;
		if(nTestWidth > nWidth)
		{
			nWidth = nTestWidth;
		}
	}

	{
		//	持久
		/*if(_item->type == ITEM_CLOTH ||
			_item->type == ITEM_WEAPON ||
			_item->type == ITEM_RING ||
			_item->type == ITEM_BRACELAT ||
			_item->type == ITEM_MEDAL ||
			_item->type == ITEM_NECKLACE ||
			_item->type == ITEM_HELMET)
		{
			++nLine;
			int nDura = HIWORD(_item->maxHP);
			int nDuraMax = LOWORD(_item->maxHP);
			nTestWidth = 7 * 6 + GetNumberBit(nDura) * 6 + GetNumberBit(nDuraMax) * 6;
		}*/
	}

	{
		//	药品加血
		/*if(_item->type == ITEM_COST)
		{
			if(_item->HP)
			{
				++nLine;

				nTestWidth = 7 * 6 + GetNumberBit(_item->HP) * 6;
				if(nTestWidth > nWidth)
				{
					nWidth = nTestWidth;
				}
			}
			if(_item->MP)
			{
				++nLine;

				nTestWidth = 7 * 6 + GetNumberBit(_item->HP) * 6;
				if(nTestWidth > nWidth)
				{
					nWidth = nTestWidth;
				}
			}
		}*/
	}

	if(GamePlayer::GetInstance()->IsEquipItem(*m_pItemAttrib))
	{
		//	品质
		++nLine;
		nTestWidth = 8 * 6 + 12 * 2;
		if(nTestWidth > nWidth)
		{
			nWidth = nTestWidth;
		}
		//	持久
		++nLine;
		int nDura = HIWORD(_item->maxHP);
		int nDuraMax = LOWORD(_item->maxHP);
		nTestWidth = 7 * 6 + GetNumberBit(nDura) * 6 + GetNumberBit(nDuraMax) * 6;

		if(_item->AC ||
			_item->maxAC)
		{
			if(_item->type != ITEM_WEAPON &&
				_item->type != ITEM_NECKLACE)
			{
				//	武器和项链不加防御
				++nLine;

				nTestWidth = 8 * 6 + GetNumberBit(_item->AC) * 6 + GetNumberBit(_item->maxAC) * 6;
				if(nTestWidth > nWidth)
				{
					nWidth = nTestWidth;
				}
			}
		}
		/*if(_item->atkPalsy)
		{
			if(_item->type == ITEM_RING)
			{
				//	戒指加麻痹
				++nLine;

				nTestWidth = 8 * 6 + GetNumberBit(_item->atkPalsy) * 6;
				if(nTestWidth > nWidth)
				{
					nWidth = nTestWidth;
				}
			}
		}*/
		if(_item->lucky)
		{
			if(_item->type == ITEM_WEAPON ||
				_item->type == ITEM_NECKLACE)
			{
				//	武器和项链加幸运
				++nLine;

				nTestWidth = 8 * 6 + GetNumberBit(_item->lucky) * 6;
				if(nTestWidth > nWidth)
				{
					nWidth = nTestWidth;
				}
			}
		}
		/*
		if(_item->atkPois)
				{
					++nLine;
		
					nTestWidth = 8 * 6 + GetNumberBit(_item->atkPois) * 6;
					if(nTestWidth > nWidth)
					{
						nWidth = nTestWidth;
					}
				}*/
		
		if(_item->atkSpeed)
		{
			if(_item->type == ITEM_NECKLACE ||
				_item->type == ITEM_RING)
			{
				//	项链和戒指加攻速
				++nLine;

				nTestWidth = 8 * 6 + GetNumberBit(_item->atkSpeed) * 6;
				if(nTestWidth > nWidth)
				{
					nWidth = nTestWidth;
				}
			}
		}
		/*if(_item->curse)
		{
			++nLine;
		}*/
		if(_item->DC ||
			_item->maxDC)
		{
			++nLine;

			nTestWidth = 8 * 6 + GetNumberBit(_item->DC) * 6 + GetNumberBit(_item->maxDC) * 6;
			if(nTestWidth > nWidth)
			{
				nWidth = nTestWidth;
			}
		}
		/*if(_item->HP)
		{
			++nLine;

			nTestWidth = 7 * 6 + GetNumberBit(_item->HP) * 6;
			if(nTestWidth > nWidth)
			{
				nWidth = nTestWidth;
			}
		}*/
		if(_item->MAC ||
			_item->maxMAC)
		{
			if(_item->type != ITEM_WEAPON ||
				_item->type != ITEM_NECKLACE)
			{
				//	项链和武器不加防御
				++nLine;

				nTestWidth = 8 * 6 + GetNumberBit(_item->MAC) * 6 + GetNumberBit(_item->maxMAC) * 6;
				if(nTestWidth > nWidth)
				{
					nWidth = nTestWidth;
				}
			}
		}
		if(_item->maxMC ||
			_item->MC)
		{
			++nLine;

			nTestWidth = 8 * 6 + GetNumberBit(_item->AC) * 6 + GetNumberBit(_item->maxAC) * 6;
			if(nTestWidth > nWidth)
			{
				nWidth = nTestWidth;
			}
		}
		if(_item->maxSC ||
			_item->SC)
		{
			++nLine;

			nTestWidth = 8 * 6 + GetNumberBit(_item->SC) * 6 + GetNumberBit(_item->maxSC) * 6;
			if(nTestWidth > nWidth)
			{
				nWidth = nTestWidth;
			}
		}
		/*if(_item->MP)
		{
			++nLine;

			nTestWidth = 7 * 6 + GetNumberBit(_item->HP) * 6;
			if(nTestWidth > nWidth)
			{
				nWidth = nTestWidth;
			}
		}*/
		if(_item->moveSpeed)
		{
			++nLine;

			nTestWidth = 8 * 6 + GetNumberBit(_item->moveSpeed) * 6;
			if(nTestWidth > nWidth)
			{
				nWidth = nTestWidth;
			}
		}
		if(_item->accuracy)
		{
			if(_item->type == ITEM_NECKLACE ||
				_item->type == ITEM_WEAPON ||
				_item->type == ITEM_BRACELAT)
			{
				//	项链 武器 手镯 加准确
				++nLine;

				nTestWidth = 8 * 6 + GetNumberBit(_item->accuracy) * 6;
				if(nTestWidth > nWidth)
				{
					nWidth = nTestWidth;
				}
			}
		}
		if(_item->hide)
		{
			if(_item->type == ITEM_BRACELAT ||
				_item->type == ITEM_NECKLACE)
			{
				//	手镯 项链 加躲避
				++nLine;

				nTestWidth = 8 * 6 + GetNumberBit(_item->hide) * 6;
				if(nTestWidth > nWidth)
				{
					nWidth = nTestWidth;
				}
			}
		}
		if(_item->reqType != 0)
		{
			switch(_item->reqType)
			{
			case REQ_LEVEL:
			case REQ_SC:
				{
					nTestWidth = 11 * 6 + GetNumberBit(_item->reqValue) * 6;
					if(nTestWidth > nWidth)
					{
						nWidth = nTestWidth;
					}
				}break;
			case REQ_MC:
			case REQ_DC:
				{
					nTestWidth = 13 * 6 + GetNumberBit(_item->reqValue) * 6;
					if(nTestWidth > nWidth)
					{
						nWidth = nTestWidth;
					}
				}break;
			}
			++nLine;
		}
	}
	else if(_item->type == ITEM_COST &&
		_item->curse == 0)
	{
		if(_item->HP)
		{
			++nLine;

			nTestWidth = 7 * 6 + GetNumberBit(_item->HP) * 6;
			if(nTestWidth > nWidth)
			{
				nWidth = nTestWidth;
			}
		}

		if(_item->MP)
		{
			++nLine;

			nTestWidth = 7 * 6 + GetNumberBit(_item->HP) * 6;
			if(nTestWidth > nWidth)
			{
				nWidth = nTestWidth;
			}
		}

		if(_item->hide != 0)
		{
			//	cool down time
			++nLine;

			nTestWidth = 12 * 2 + 12 + GetNumberBit(_item->hide) * 6 + 6;
			if(nTestWidth > nWidth)
			{
				nWidth = nTestWidth;
			}
		}
	}
	else if(_item->type == ITEM_BOOK)
	{
		if(_item->reqType != 0)
		{
			switch(_item->reqType)
			{
			case REQ_LEVEL:
			case REQ_SC:
				{
					nTestWidth = 11 * 6 + GetNumberBit(_item->reqValue) * 6;
					if(nTestWidth > nWidth)
					{
						nWidth = nTestWidth;
					}
				}break;
			case REQ_MC:
			case REQ_DC:
				{
					nTestWidth = 13 * 6 + GetNumberBit(_item->reqValue) * 6;
					if(nTestWidth > nWidth)
					{
						nWidth = nTestWidth;
					}
				}break;
			}
			++nLine;
		}
	}
	else
	{
		//	nothing
	}
	if(TEST_FLAG_BOOL(_item->atkPois, POIS_MASK_BIND))
	{
		++nLine;
		nTestWidth = 48;
		if(nTestWidth > nWidth)
		{
			nWidth = nTestWidth;
		}
	}

	//	空了两行
	nLine += 2;
	if(_item->type == ITEM_COST ||
		_item->type == ITEM_OTHER ||
		_item->type == ITEM_BALE)
	{
		--nLine;
	}

	//	计算颜色色彩
	m_dwItemNameColor = ARGB_YELLOW;
	m_nColorIndex = 0;
	if(GamePlayer::GetInstance()->IsEquipItem(*_item))
	{
		WORD wLevel = m_pItemAttrib->level;
		m_dwItemNameColor = ARGB_WHITE;

		if(wLevel != 0)
		{
			int nValue = 0;

			/*BYTE bLow = LOBYTE(wLevel);
			BYTE bHigh = HIBYTE(wLevel);

			bHigh &= 0x7A;
			bool bZero = false;

			if((bHigh & 0x40) == 0)
			{
				bZero = ((bHigh & 0x10) != 0 ? false : true);
				if(bZero)
				{
					bHigh |= 0x10;
				}
				else
				{
					bHigh &= (~0x10);
				}
			}

			if((bHigh & 0x02) == 0)
			{
				bZero = ((bHigh & 0x08) != 0 ? false : true);
				if(bZero)
				{
					bHigh |= 0x08;
				}
				else
				{
					bHigh &= (~0x08);
				}
			}

			static int s_nValueTable[] =
			{
				9, 2, 1, 3, 4, 7, 8, 5
			};
			static BYTE s_btMaskTable[] =
			{
				0x80, 0x40, 0x20, 0x10,
				0x08, 0x04, 0x02, 0x01
			};

			for(int i = 0; i < 8; ++i)
			{
				if((bHigh & s_btMaskTable[i]) != 0)
				{
					nValue += s_nValueTable[i];
				}
			}

			nValue = (int)bLow - nValue;*/
			nValue = GetItemUpgrade(wLevel);
			if(nValue >= 0 &&
				nValue <= 8)
			{
				m_dwItemNameColor = g_dwItemColor[nValue];
				m_nColorIndex = nValue;
			}
		}
	}

	int nAttribHeight = nLine * 15 + 10;
	//	10为绘制的时候左上角偏移5
	int nAttribWidth = nWidth + 10;
	m_rcClient.left = m_rcClient.top = 0;
	m_rcClient.right = nAttribWidth;
	m_rcClient.bottom = nAttribHeight;

	if(_item->atkPalsy != 0)
	{
		CalcSuitAttrib();
	}

	SetVisible(true);
}


void GameIntroDlg::ShowExp()
{
	ShowNothing();
	char szText[MAX_PATH];
	sprintf(szText, "EXP : %d/%d",
		GamePlayer::GetInstance()->GetAttrib()->EXPR,
		GamePlayer::GetInstance()->GetAttrib()->maxEXPR);
	ShowString(szText);
	SetVisible(true);
}

void GameIntroDlg::ShowHP()
{
	ShowNothing();
	char szText[MAX_PATH];
	sprintf(szText, "HP : %d/%d",
		GamePlayer::GetInstance()->GetAttrib()->HP,
		GamePlayer::GetInstance()->GetAttrib()->maxHP);
	ShowString(szText);
	SetVisible(true);
}

void GameIntroDlg::ShowMP()
{
	ShowNothing();
	char szText[MAX_PATH];
	sprintf(szText, "MP : %d/%d",
		GamePlayer::GetInstance()->GetAttrib()->MP,
		GamePlayer::GetInstance()->GetAttrib()->maxMP);
	ShowString(szText);
	SetVisible(true);
}

void GameIntroDlg::ShowString(const char* lpszInfo)
{
	ShowNothing();
	m_strInfo = lpszInfo;
	int nWidth = GetTextWidth(lpszInfo, 12);
	m_rcClient.left = m_rcClient.top = 0;
	m_rcClient.right = nWidth + 10;
	m_rcClient.bottom = 12 + 10;
	SetVisible(true);
}

void GameIntroDlg::ShowLinefeedString(const char* _pszText, int _nwords)
{
	ShowNothing();
	m_nLinefeedNum = _nwords;
	m_xLinefeedStr = _pszText;

	m_rcClient.left = m_rcClient.top = 0;
	int nWidth = GetTextWidth(_pszText, 12);
	if(nWidth <= _nwords * 12)
	{
		m_rcClient.right = nWidth + 10;
		m_rcClient.bottom = 12 + 10;
		SetVisible(true);
	}
	else
	{
		int nLine = nWidth / (_nwords * 12);
		m_rcClient.right = _nwords * 12 + 10;
		m_rcClient.bottom = (nLine + 1) * 14 + 10;
		SetVisible(true);
	}
}

void GameIntroDlg::ShowNothing()
{
	m_bShowExp = m_bShowHP = m_bShowMP = false;
	m_pItemAttrib = NULL;
	m_strInfo.clear();
	m_xLinefeedStr.clear();
	m_nLinefeedNum = 0;
	m_bVisible = false;

	ZeroMemory(&m_stSuitAttribRenderInfo, sizeof(m_stSuitAttribRenderInfo));
}

void GameIntroDlg::RenderSuitAttrib()
{
#define SUIT_PRINT_COLOR ARGB(255, 39,150, 216)

	if(!m_stSuitAttribRenderInfo.bCanShow)
	{
		return;
	}

	if(m_pItemAttrib == NULL)
	{
		return;
	}

	if(m_pItemAttrib->atkPalsy == 0)
	{
		return;
	}

	//	计算矩形
	if(m_rcSuit.left == m_rcSuit.right ||
		m_rcSuit.top == m_rcSuit.bottom ||
		m_pResManager == NULL ||
		!m_bVisible)
	{
		return;
	}

	float x, y;
	AfxGetHge()->Input_GetMousePos(&x, &y);
	//	调整矩形区域
	RECT rcDrawRect = m_rcClient;
	rcDrawRect.left += (x - 20);
	rcDrawRect.right += (x - 20);
	rcDrawRect.top += (y + 20);
	rcDrawRect.bottom += (y + 20);

	//int nAttribHeight = m_rcClient.bottom - m_rcClient.top;
	//rcDrawRect.top += nAttribHeight + 0;
	//rcDrawRect.bottom += nAttribHeight + 0;
	//rcDrawRect.bottom = rcDrawRect.top + m_rcSuit.bottom - m_rcSuit.top;
	//rcDrawRect.right = rcDrawRect.left + m_rcSuit.right - m_rcSuit.left;

	int nDrawOffset = 0;
	if(rcDrawRect.right >= VIEW_WIDTH)
	{
		nDrawOffset = rcDrawRect.right - VIEW_WIDTH - 1;
		rcDrawRect.right = VIEW_WIDTH - 1;
		rcDrawRect.left -= nDrawOffset;
	}
	if(rcDrawRect.bottom >= VIEW_OPERATE + VIEW_HEIGHT)
	{
		nDrawOffset = rcDrawRect.bottom - VIEW_HEIGHT - VIEW_OPERATE - 1;
		rcDrawRect.bottom = VIEW_HEIGHT + VIEW_OPERATE - 1;
		rcDrawRect.top -= nDrawOffset;
	}

	rcDrawRect.top += rcDrawRect.bottom - rcDrawRect.top;
	rcDrawRect.bottom = rcDrawRect.top + m_rcSuit.bottom - m_rcSuit.top;
	rcDrawRect.right = rcDrawRect.left + m_rcSuit.right - m_rcSuit.left;

	if(rcDrawRect.right >= VIEW_WIDTH)
	{
		nDrawOffset = rcDrawRect.right - VIEW_WIDTH - 1;
		rcDrawRect.right = VIEW_WIDTH - 1;
		rcDrawRect.left -= nDrawOffset;
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();

	//	画底色
	HTEXTURE tex = 0;
	tex = m_pResManager->GetTexture("bmcolor");
	if(tex)
	{
		INITSPRITE(m_pSpr, tex);
		SETSPRITEWH(tex, m_pSpr, m_rcSuit.right - m_rcSuit.left, m_rcSuit.bottom - m_rcSuit.top);
		m_pSpr->Render(rcDrawRect.left, rcDrawRect.top);
	}

	//	画边框
	tex = m_pResManager->GetTexture("horbord");
	if(tex)
	{
		SETSPRITEW(tex, m_pSpr, m_rcSuit.right - m_rcSuit.left + 1/*- 2*/);
		m_pSpr->Render( rcDrawRect.left + 1/*+ 1*/, rcDrawRect.top);
		m_pSpr->Render(rcDrawRect.left + 1/*+ 1*/, rcDrawRect.bottom);
	}
	tex = m_pResManager->GetTexture("verbord");
	if(tex)
	{
		SETSPRITEH(tex, m_pSpr, m_rcSuit.bottom - m_rcSuit.top - 1/* - 2*/);
		m_pSpr->Render(rcDrawRect.left, rcDrawRect.top + 1 + 1);
		m_pSpr->Render(rcDrawRect.right, rcDrawRect.top + 1 + 1);
	}

	//	套装名称
	int nDrawY = rcDrawRect.top + 5;
	AfxGetPrinter()->SetColor(COLOR_SUITNAME);
	AfxGetPrinter()->Print(rcDrawRect.left + 5, nDrawY, "%s(%d/%d)",
		m_stSuitAttribRenderInfo.pExtraAttribList->szSuitChName,
		m_stSuitAttribRenderInfo.nActiveItemSum,
		m_stSuitAttribRenderInfo.nActiveItemAll);
	nDrawY += nDrawOftY;

	//	套装各个装备
	for(int i = 0; i < /*m_stSuitAttribRenderInfo.nActiveAll*/10; ++i)
	{
		if(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[i] == 0)
		{
			break;
		}

		if(m_stSuitAttribRenderInfo.nActiveItemPos[i] != 0)
		{
			AfxGetPrinter()->SetColor(COLOR_SUITNAMEITEM_ACTIVE);
		}
		else
		{
			AfxGetPrinter()->SetColor(COLOR_SUITNAMEITEM_UNACTIVE);
		}

		AfxGetPrinter()->Print(rcDrawRect.left + 5 + 12, nDrawY, m_xSuitItemName[i].c_str());
		nDrawY += nDrawOftY;
	}

	//	套装属性
	nDrawY += nDrawOftY;
	AfxGetPrinter()->SetColor(SUIT_PRINT_COLOR);
	if(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitShowType == 1)
	{
		AfxGetPrinter()->Print(rcDrawRect.left + 5, nDrawY, "特殊效果");
	}
	else
	{
		AfxGetPrinter()->Print(rcDrawRect.left + 5, nDrawY, "套装效果");
	}
	nDrawY += nDrawOftY;

	//	套装属性值
	char szPrintBuf[100];
	ItemExtraAttribItem extraItem;

	for(int i = 0; i < /*m_stSuitAttribRenderInfo.nAttribAll*/10; ++i)
	{
		if(m_stSuitAttribRenderInfo.pExtraAttribList->stExtraAttrib[i].nAttribID == 0)
		{
			break;
		}

		ZeroMemory(szPrintBuf, sizeof(szPrintBuf));

		if(m_stSuitAttribRenderInfo.nActiveAttribSum > i)
		{
			AfxGetPrinter()->SetColor(COLOR_EXTRAATRRIB_ACTIVE);
		}
		else
		{
			AfxGetPrinter()->SetColor(COLOR_EXTRAATTRIB_UNACTIVE);
		}

		extraItem = m_stSuitAttribRenderInfo.pExtraAttribList->stExtraAttrib[i];
		if(extraItem.nAttribID == EAID_AC ||
			extraItem.nAttribID == EAID_MAC ||
			extraItem.nAttribID == EAID_DC ||
			extraItem.nAttribID == EAID_MC ||
			extraItem.nAttribID == EAID_SC)
		{
			sprintf(szPrintBuf, "%s  %d-%d",
				g_szExtraAttribDescriptor[extraItem.nAttribID], HIWORD(extraItem.nAttribValue), LOWORD(extraItem.nAttribValue));
		}
		else if(extraItem.nAttribID >= EAID_LUCKY &&
			extraItem.nAttribID <= EAID_IGNOREMCDAM)
		{
			sprintf(szPrintBuf, "%s +%d",
				g_szExtraAttribDescriptor[extraItem.nAttribID], extraItem.nAttribValue);
		}
		AfxGetPrinter()->Print(rcDrawRect.left + 5 + 12, nDrawY, szPrintBuf);
		nDrawY += nDrawOftY;
	}
}

void GameIntroDlg::CalcSuitAttrib()
{
	ZeroMemory(&m_stSuitAttribRenderInfo, sizeof(m_stSuitAttribRenderInfo));

	if(m_pItemAttrib == NULL)
	{
		return;
	}

	if(m_pItemAttrib->atkPalsy == 0)
	{
		return;
	}

	m_stSuitAttribRenderInfo.bCanShow = true;
	m_stSuitAttribRenderInfo.pExtraAttribList = GameInfoManager::GetInstance()->GetItemExtraSuitAttribList(m_pItemAttrib->atkPalsy);

	if(NULL == m_stSuitAttribRenderInfo.pExtraAttribList)
	{
		ZeroMemory(&m_stSuitAttribRenderInfo, sizeof(m_stSuitAttribRenderInfo));
		return;
	}

	ItemAttrib item;
	for(int i = 0; i < 10; ++i)
	{
		if(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[i] != 0)
		{
			++m_stSuitAttribRenderInfo.nActiveItemAll;
			ZeroMemory(&item, sizeof(ItemAttrib));

			if(GameInfoManager::GetInstance()->GetItemAttrib(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[i], &item))
			{
				m_xSuitItemName[i] = item.name;
			}
		}
	}

	const ItemAttrib* pPlayerItem = NULL;
	bool bActived[10];
	ZeroMemory(bActived, sizeof(bActived));

	for(int i = PLAYER_ITEM_WEAPON; i < PLAYER_ITEM_TOTAL; ++i)
	{
		pPlayerItem = GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i);

		if(pPlayerItem->atkPalsy == m_stSuitAttribRenderInfo.pExtraAttribList->nSuitID)
		{
			//	检测是否存在过
			for(int j = 0; j < 10; ++j)
			{
				if(pPlayerItem->id == m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[j] &&
					!bActived[j] &&
					m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[j] != 0)
				{
					bActived[j] = true;

					m_stSuitAttribRenderInfo.nActiveItemPos[j] = i + 1;
					++m_stSuitAttribRenderInfo.nActiveItemSum;

					break;
				}
			}
		}
	}

	int nActiveMax = 0;

	for(int i = 0; i < MAX_EXTRAATTIRB; ++i)
	{
		if(m_stSuitAttribRenderInfo.nActiveItemSum >= m_stSuitAttribRenderInfo.pExtraAttribList->nActiveSum[i] &&
			m_stSuitAttribRenderInfo.pExtraAttribList->nActiveSum[i] > nActiveMax)
		{
			nActiveMax = m_stSuitAttribRenderInfo.pExtraAttribList->nActiveSum[i];

			m_stSuitAttribRenderInfo.nActiveAttribSum = m_stSuitAttribRenderInfo.pExtraAttribList->nActiveAttribSum[i];
		}
	}

	//	计算矩形
	int nMaxHeight = 0;
	int nMaxWidth = 0;
	int nTestWidth = 0;
	int nLine = 0;

	//	套装名字
	nTestWidth = 3 * 6 + GetTextWidth(m_stSuitAttribRenderInfo.pExtraAttribList->szSuitChName, 12) + GetNumberBit(m_stSuitAttribRenderInfo.nActiveItemAll) * 6 + GetNumberBit(m_stSuitAttribRenderInfo.nActiveItemSum) * 6;
	nMaxWidth = nTestWidth;
	++nLine;

	//  各个装备名称
	for(int i = 0; i < 10; ++i)
	{
		if(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[i] == 0)
		{
			break;
		}

		nTestWidth = 12 + GetTextWidth(m_xSuitItemName[i].c_str(), 12);
		if(nTestWidth > nMaxWidth)
		{
			nMaxWidth = nTestWidth;
		}
		++nLine;
	}

	//	空了一行
	++nLine;

	//	套装属性
	nTestWidth = 4 * 12;
	if(nTestWidth > nMaxWidth)
	{
		nMaxWidth = nTestWidth;
	}
	++nLine;

	//	属性值
	ItemExtraAttribItem extraAttrib;

	for(int i = 0; i < 10; ++i)
	{
		extraAttrib = m_stSuitAttribRenderInfo.pExtraAttribList->stExtraAttrib[i];
		if(extraAttrib.nAttribID == EAID_NONE)
		{
			break;
		}

		if(extraAttrib.nAttribID == EAID_AC ||
			extraAttrib.nAttribID == EAID_MAC ||
			extraAttrib.nAttribID == EAID_DC ||
			extraAttrib.nAttribID == EAID_MC ||
			extraAttrib.nAttribID == EAID_SC)
		{
			nTestWidth = 12 + GetTextWidth(g_szExtraAttribDescriptor[extraAttrib.nAttribID], 12) + 2 * 6 + GetNumberBit(LOWORD(extraAttrib.nAttribValue)) * 6 + GetNumberBit(HIWORD(extraAttrib.nAttribValue)) * 6 + 6;
		}
		else if(extraAttrib.nAttribID >= EAID_LUCKY &&
			extraAttrib.nAttribID <= EAID_IGNOREMCDAM)
		{
			nTestWidth = 12 + GetTextWidth(g_szExtraAttribDescriptor[extraAttrib.nAttribID], 12) + 6 + 6 + GetNumberBit(extraAttrib.nAttribValue) * 6;
		}

		if(nTestWidth > nMaxWidth)
		{
			nMaxWidth = nTestWidth;
		}

		++nLine;
	}

	m_rcSuit.left = 0;
	m_rcSuit.top = 0;
	m_rcSuit.bottom = nLine * nDrawOftY + 10;
	m_rcSuit.right = nMaxWidth + 10;
}


//////////////////////////////////////////////////////////////////////////
//	Ex
static const char* s_szItemTypeName[] =
{
	//	杂物
//#define ITEM_NO			0
	"无类型",
	//	书
//#define ITEM_BOOK		1
	"武功秘籍",
	//	衣服
//#define ITEM_CLOTH		2
	"衣服",
	//	项链
//#define ITEM_NECKLACE	3
	"项链",
	//	手镯
//#define ITEM_BRACELAT	4
	"手镯",
	//	戒指
//#define ITEM_RING		5
	"戒指",
	//	勋章
//#define ITEM_MEDAL		6
	"勋章",
	//	头盔
//#define ITEM_HELMET		7
	"头盔",
	//	药品
//#define ITEM_YAO_SPE	8
	"药品",
	//	武器
//#define ITEM_WEAPON		9
	"武器",
	//	杂物
//#define ITEM_OTHER		10
	"杂物",
	//	普通药品 可以使用的物品(药品 卷轴)
//#define ITEM_COST		11
	"消耗品",
	//	捆
//#define ITEM_BALE		12
	"捆装物品",
	//	卷轴
//#define ITEM_SCROLL		13
	"卷轴",
	//	鞋子
//#define ITEM_SHOE		14
	"鞋子",
	//	腰带
//#define ITEM_BELT		15
	"腰带",
	//	宝石
//#define ITEM_GEM		16
	"宝石",
	//	符咒
//#define ITEM_CHARM		17
	"符咒"
};

static const DWORD s_dwItemNameColor = ARGB(255, 195, 119, 69);

static const int s_nTitleHeight = 16;
static const int s_nTopBoundary = 5;
static const int s_nLeftBoundary = 5;
static const int s_nEachLineHeight = 13;
static const int s_nItemIconWidth = 48;
static const int s_nItemIconHeight = 48;
static const int s_nItemIntroGapX = 5;

bool GameIntroDlg::CalcItemAttribRect()
{
	//return false;

	if(!m_pItemAttrib)
	{
		return true;
	}

	//	Reset the original item attribute
	ZeroMemory(&m_oriItem, sizeof(ItemAttrib));
	if(MirGame::IsEquipItem(*m_pItemAttrib))
	{
		m_oriItem.id = m_pItemAttrib->id;
		if(GameInfoManager::GetInstance()->GetItemAttrib(m_pItemAttrib->id, &m_oriItem))
		{
			//	Calculate the addition of the equipment
#define GET_ITEM_ADDITION(ATTRIB)	m_oriItem.ATTRIB = m_pItemAttrib->ATTRIB - m_oriItem.ATTRIB;
			GET_ITEM_ADDITION(maxAC);
			GET_ITEM_ADDITION(maxMAC);
			GET_ITEM_ADDITION(maxDC);
			GET_ITEM_ADDITION(maxMC);
			GET_ITEM_ADDITION(maxSC);
			GET_ITEM_ADDITION(lucky);
			GET_ITEM_ADDITION(atkSpeed);
			GET_ITEM_ADDITION(moveSpeed);
			GET_ITEM_ADDITION(accuracy);
			GET_ITEM_ADDITION(hide);
		}
		else
		{
			ZeroMemory(&m_oriItem, sizeof(ItemAttrib));
		}
	}

	ZeroMemory(&m_rcClient, sizeof(RECT));
	m_rcClient.right = 1;
	m_rcClient.bottom = 1;

	RECT rcItem;
	ZeroMemory(&rcItem, sizeof(RECT));
	int nMaxWidth = 0;
	int nCurWidth = 0;

	//	First, 5 pixel boundary on top
	rcItem.right += s_nLeftBoundary;
	rcItem.bottom += s_nTopBoundary;

	//	Item name
	nCurWidth = GetTextWidth(m_pItemAttrib->name, 14);
	nMaxWidth = nCurWidth;
	rcItem.bottom += s_nTitleHeight;

	//	Item icon
	rcItem.bottom += s_nItemIconHeight;

	//	The introduction on the right of the icon
	//	Type
	nCurWidth = s_nItemIconWidth + s_nItemIntroGapX + GetTextWidth(s_szItemTypeName[m_pItemAttrib->type], 12) + 3 * 12;
	if(nCurWidth > nMaxWidth)
	{
		nMaxWidth = nCurWidth;
	}
	//	Weight
	nCurWidth = s_nItemIconWidth + s_nItemIntroGapX + GetNumberBit(m_pItemAttrib->weight) * 6 + 3 * 12;
	if(nCurWidth > nMaxWidth)
	{
		nMaxWidth = nCurWidth;
	}
	//	持久
	/*if(MirGame::IsEquipItem(*m_pItemAttrib))
	{
		int nDura = HIWORD(m_pItemAttrib->maxHP) / 1000;
		int nDuraMax = LOWORD(m_pItemAttrib->maxHP) / 1000;
		nCurWidth = s_nItemIconWidth + s_nItemIntroGapX + GetNumberBit(nDura) * 6 + 6 + GetNumberBit(nDuraMax) * 6 + 3 * 12;
		if(nCurWidth > nMaxWidth)
		{
			nMaxWidth = nCurWidth;
		}
	}*/
	//	Binded or not
	nCurWidth = s_nItemIconWidth + s_nItemIntroGapX + 4 * 12;
	if(nCurWidth > nMaxWidth)
	{
		nMaxWidth = nCurWidth;
	}

	//	Details
#define TEST_ATTRIB(NAME)	m_pItemAttrib->NAME
	//	A line blank
	rcItem.bottom += s_nItemIntroGapX;

	if(MirGame::IsEquipItem(*m_pItemAttrib))
	{
		//	[装备信息]
		rcItem.bottom += s_nEachLineHeight;
		//	Dura
		int nDura = HIWORD(m_pItemAttrib->maxHP) / 1000;
		int nDuraMax = LOWORD(m_pItemAttrib->maxHP) / 1000;
		nCurWidth = GetNumberBit(nDura) * 6 + 6 + GetNumberBit(nDuraMax) * 6 + 3 * 12;
		if(nCurWidth > nMaxWidth)
		{
			nMaxWidth = nCurWidth;
		}
		rcItem.bottom += s_nEachLineHeight;
		//	quality
		nCurWidth = 5 * 12;
		if(nCurWidth > nMaxWidth)
		{
			nMaxWidth = nCurWidth;
		}
		rcItem.bottom += s_nEachLineHeight;

		//	level
		int nLevel = GameInfoManager::GetInstance()->GetItemGradeInFullAttrib(m_pItemAttrib->id);
		nCurWidth = 3 * 12 + GameInfoManager::GetInstance()->GetItemGradeInFullAttrib(nLevel);
		if(nCurWidth > nMaxWidth)
		{
			nMaxWidth = nCurWidth;
		}
		rcItem.bottom += s_nEachLineHeight;

		//	A line blank
		rcItem.bottom += s_nEachLineHeight;

		//	[基础属性]
		rcItem.bottom += s_nEachLineHeight;
		
		//	AC
		if(m_pItemAttrib->AC != 0 ||
			m_pItemAttrib->maxAC != 0)
		{
			if(m_pItemAttrib->type != ITEM_WEAPON &&
				m_pItemAttrib->type != ITEM_NECKLACE)
			{
				nCurWidth = GetNumberBit(m_pItemAttrib->AC) * 6 + GetNumberBit(m_pItemAttrib->maxAC) * 6 + 6 + 3 * 12;
				if(m_oriItem.maxAC != 0)
				{
					nCurWidth += GetNumberBit(m_oriItem.maxAC) * 6 + 6 * 3;
				}
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
			}
		}
		//	MAC
		if(m_pItemAttrib->MAC != 0 ||
			m_pItemAttrib->maxMAC != 0)
		{
			if(m_pItemAttrib->type != ITEM_WEAPON &&
				m_pItemAttrib->type != ITEM_NECKLACE)
			{
				nCurWidth = GetNumberBit(m_pItemAttrib->MAC) * 6 + GetNumberBit(m_pItemAttrib->maxMAC) * 6 + 6 + 3 * 12;
				if(m_oriItem.maxMAC != 0)
				{
					nCurWidth += GetNumberBit(m_oriItem.maxMAC) * 6 + 6 * 3;
				}
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
			}
		}
		//	DC
		if(m_pItemAttrib->DC != 0 ||
			m_pItemAttrib->maxDC != 0)
		{
			nCurWidth = GetNumberBit(m_pItemAttrib->DC) * 6 + GetNumberBit(m_pItemAttrib->maxDC) * 6 + 6 + 3 * 12;
			if(m_oriItem.maxDC != 0)
			{
				nCurWidth += GetNumberBit(m_oriItem.maxDC) * 6 + 6 * 3;
			}
			if(nCurWidth > nMaxWidth)
			{
				nMaxWidth = nCurWidth;
			}
			rcItem.bottom += s_nEachLineHeight;
		}
		//	MC
		if(m_pItemAttrib->MC != 0 ||
			m_pItemAttrib->maxMC != 0)
		{
			nCurWidth = GetNumberBit(m_pItemAttrib->MC) * 6 + GetNumberBit(m_pItemAttrib->maxMC) * 6 + 6 + 3 * 12;
			if(m_oriItem.maxMC != 0)
			{
				nCurWidth += GetNumberBit(m_oriItem.maxMC) * 6 + 6 * 3;
			}
			if(nCurWidth > nMaxWidth)
			{
				nMaxWidth = nCurWidth;
			}
			rcItem.bottom += s_nEachLineHeight;
		}
		//	SC
		if(m_pItemAttrib->SC != 0 ||
			m_pItemAttrib->maxSC != 0)
		{
			nCurWidth = GetNumberBit(m_pItemAttrib->SC) * 6 + GetNumberBit(m_pItemAttrib->maxSC) * 6 + 6 + 3 * 12;
			if(m_oriItem.maxSC != 0)
			{
				nCurWidth += GetNumberBit(m_oriItem.maxSC) * 6 + 6 * 3;
			}
			if(nCurWidth > nMaxWidth)
			{
				nMaxWidth = nCurWidth;
			}
			rcItem.bottom += s_nEachLineHeight;
		}

		//	A line blank
		//rcItem.bottom += s_nEachLineHeight;

		//	[特殊属性]
		//rcItem.bottom += s_nEachLineHeight;

		//	Lucky
		if(m_pItemAttrib->lucky != 0)
		{
			if(m_pItemAttrib->type == ITEM_WEAPON ||
				m_pItemAttrib->type == ITEM_NECKLACE)
			{
				nCurWidth = GetNumberBit(m_pItemAttrib->lucky) * 6 + 6 + 3 * 12;
				if(m_oriItem.lucky != 0)
				{
					nCurWidth += GetNumberBit(m_oriItem.lucky) * 6 + 6 * 3;
				}
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
			}
		}
		//	AtkSpeed
		if(m_pItemAttrib->atkSpeed != 0)
		{
			if(m_pItemAttrib->type == ITEM_RING ||
				m_pItemAttrib->type == ITEM_NECKLACE)
			{
				nCurWidth = GetNumberBit(m_pItemAttrib->atkSpeed) * 6 + 6 + 5 * 12;
				if(m_oriItem.atkSpeed != 0)
				{
					nCurWidth += GetNumberBit(m_oriItem.atkSpeed) * 6 + 6 * 3;
				}
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
			}
		}
		//	MoveSpeed
		if(m_pItemAttrib->moveSpeed != 0)
		{
			//if(m_pItemAttrib->type == ITEM_RING ||
				//m_pItemAttrib->type == ITEM_NECKLACE)
			{
				nCurWidth = GetNumberBit(m_pItemAttrib->moveSpeed) * 6 + 6 + 5 * 12;
				if(m_oriItem.moveSpeed != 0)
				{
					nCurWidth += GetNumberBit(m_oriItem.moveSpeed) * 6 + 6 * 3;
				}
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
			}
		}
		//	Accuracy
		if(m_pItemAttrib->accuracy != 0)
		{
			if(m_pItemAttrib->type == ITEM_BRACELAT ||
			m_pItemAttrib->type == ITEM_NECKLACE ||
			m_pItemAttrib->type == ITEM_WEAPON)
			{
				nCurWidth = GetNumberBit(m_pItemAttrib->accuracy) * 6 + 6 + 3 * 12;
				if(m_oriItem.accuracy != 0)
				{
					nCurWidth += GetNumberBit(m_oriItem.accuracy) * 6 + 6 * 3;
				}
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
			}
		}
		//	Hide
		if(m_pItemAttrib->hide != 0)
		{
			if(m_pItemAttrib->type == ITEM_BRACELAT ||
			m_pItemAttrib->type == ITEM_NECKLACE)
			{
				nCurWidth = GetNumberBit(m_pItemAttrib->hide) * 6 + 6 + 3 * 12;
				if(m_oriItem.hide != 0)
				{
					nCurWidth += GetNumberBit(m_oriItem.hide) * 6 + 6 * 3;
				}
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
			}
		}
	}
	else if(m_pItemAttrib->type == ITEM_BOOK)
	{
		//	[秘籍信息]
		{
			rcItem.bottom += s_nEachLineHeight;
		}
		//	Job
		{
			nCurWidth = 4 * 12;
			if(nCurWidth > nMaxWidth)
			{
				nMaxWidth = nCurWidth;
			}
			rcItem.bottom += s_nEachLineHeight;
		}
	}
	else if(m_pItemAttrib->type == ITEM_COST)
	{
		//	[物品信息]
		{
			rcItem.bottom += s_nEachLineHeight;
		}
		//	Count
		{
			nCurWidth = GetNumberBit(m_pItemAttrib->atkSpeed) * 6 + 3 * 12;
			if(nCurWidth > nMaxWidth)
			{
				nMaxWidth = nCurWidth;
			}
			rcItem.bottom += s_nEachLineHeight;
		}
		//	HP MP
		if(m_pItemAttrib->curse == 0)
		{
			if(m_pItemAttrib->HP != 0)
			{
				nCurWidth = GetNumberBit(m_pItemAttrib->HP) * 6 + 6 * 12;
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
			}
			if(m_pItemAttrib->MP != 0)
			{
				nCurWidth = GetNumberBit(m_pItemAttrib->MP) * 6 + 6 * 12;
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
			}
			if(m_pItemAttrib->hide != 0)
			{
				nCurWidth = GetNumberBit(m_pItemAttrib->hide) * 6 + 6 * 12;
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
			}
		}
	}

	//	Equipment and book
	if(m_pItemAttrib->type == ITEM_BOOK ||
		MirGame::IsEquipItem(*m_pItemAttrib))
	{
		//	A line blank
		rcItem.bottom += s_nEachLineHeight;

		if(m_pItemAttrib->reqType != 0)
		{
			nCurWidth = 5 * 12 + GetNumberBit(m_pItemAttrib->reqValue);
		}
		else
		{
			nCurWidth = 5 * 12;
		}

		if(nCurWidth > nMaxWidth)
		{
			nMaxWidth = nCurWidth;
		}
		rcItem.bottom += s_nEachLineHeight;
	}
	
	//	Other details

	//	Suit attribute
	if(MirGame::IsEquipItem(*m_pItemAttrib) &&
		m_pItemAttrib->atkPalsy != 0)
	{
		//	A line blank
		rcItem.bottom += s_nEachLineHeight;

		//	Get the rect
		ZeroMemory(&m_stSuitAttribRenderInfo, sizeof(SuitAttribRenderInfo));
		m_stSuitAttribRenderInfo.bCanShow = true;
		m_stSuitAttribRenderInfo.pExtraAttribList = GameInfoManager::GetInstance()->GetItemExtraSuitAttribList(m_pItemAttrib->atkPalsy);

		if(NULL == m_stSuitAttribRenderInfo.pExtraAttribList)
		{
			ZeroMemory(&m_stSuitAttribRenderInfo, sizeof(m_stSuitAttribRenderInfo));
			//return;
		}
		else
		{
			ItemAttrib item;
			for(int i = 0; i < 10; ++i)
			{
				if(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[i] != 0)
				{
					++m_stSuitAttribRenderInfo.nActiveItemAll;
					ZeroMemory(&item, sizeof(ItemAttrib));

					if(GameInfoManager::GetInstance()->GetItemAttrib(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[i], &item))
					{
						m_xSuitItemName[i] = item.name;
					}
				}
			}

			const ItemAttrib* pPlayerItem = NULL;
			bool bActived[10];
			ZeroMemory(bActived, sizeof(bActived));

			for(int i = PLAYER_ITEM_WEAPON; i < PLAYER_ITEM_TOTAL; ++i)
			{
				pPlayerItem = GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i);

				if(pPlayerItem->atkPalsy == m_stSuitAttribRenderInfo.pExtraAttribList->nSuitID)
				{
					//	检测是否存在过
					for(int j = 0; j < 10; ++j)
					{
						if(pPlayerItem->id == m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[j] &&
							!bActived[j] &&
							m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[j] != 0)
						{
							bActived[j] = true;

							m_stSuitAttribRenderInfo.nActiveItemPos[j] = i + 1;
							++m_stSuitAttribRenderInfo.nActiveItemSum;

							break;
						}
					}
				}
			}

			int nActiveMax = 0;

			for(int i = 0; i < MAX_EXTRAATTIRB; ++i)
			{
				if(m_stSuitAttribRenderInfo.nActiveItemSum >= m_stSuitAttribRenderInfo.pExtraAttribList->nActiveSum[i] &&
					m_stSuitAttribRenderInfo.pExtraAttribList->nActiveSum[i] > nActiveMax)
				{
					nActiveMax = m_stSuitAttribRenderInfo.pExtraAttribList->nActiveSum[i];

					m_stSuitAttribRenderInfo.nActiveAttribSum = m_stSuitAttribRenderInfo.pExtraAttribList->nActiveAttribSum[i];
				}
			}

			//if(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitShowType == 0)
			{
				//	Adjust the rect
				//	[SUIT NAME](2/6)
				nCurWidth = 3 * 6 + GetTextWidth(m_stSuitAttribRenderInfo.pExtraAttribList->szSuitChName, 12) + GetNumberBit(m_stSuitAttribRenderInfo.nActiveItemAll) * 6 + GetNumberBit(m_stSuitAttribRenderInfo.nActiveItemSum) * 6;
				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}
				rcItem.bottom += s_nEachLineHeight;
				//	Suit item name
				for(int i = 0; i < 10; ++i)
				{
					if(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[i] == 0)
					{
						break;
					}

					nCurWidth = GetTextWidth(m_xSuitItemName[i].c_str(), 12);
					if(nCurWidth > nMaxWidth)
					{
						nMaxWidth = nCurWidth;
					}
					rcItem.bottom += s_nEachLineHeight;
				}
			}
			

			//	A line blank
			rcItem.bottom += s_nEachLineHeight;
			//	[套装效果]
			rcItem.bottom += s_nEachLineHeight;
			//	Suit effect
			ItemExtraAttribItem extraAttrib;
			for(int i = 0; i < 10; ++i)
			{
				extraAttrib = m_stSuitAttribRenderInfo.pExtraAttribList->stExtraAttrib[i];
				if(extraAttrib.nAttribID == EAID_NONE)
				{
					break;
				}

				if(extraAttrib.nAttribID == EAID_AC ||
					extraAttrib.nAttribID == EAID_MAC ||
					extraAttrib.nAttribID == EAID_DC ||
					extraAttrib.nAttribID == EAID_MC ||
					extraAttrib.nAttribID == EAID_SC)
				{
					nCurWidth = GetTextWidth(g_szExtraAttribDescriptor[extraAttrib.nAttribID], 12) + GetNumberBit(LOWORD(extraAttrib.nAttribValue)) * 6 + GetNumberBit(HIWORD(extraAttrib.nAttribValue)) * 6 + 6;
				}
				else if(extraAttrib.nAttribID >= EAID_LUCKY &&
					extraAttrib.nAttribID <= EAID_ADDTIGER)
				{
					nCurWidth = GetTextWidth(g_szExtraAttribDescriptor[extraAttrib.nAttribID], 12) + 6 + GetNumberBit(extraAttrib.nAttribValue) * 6;
				}

				if(nCurWidth > nMaxWidth)
				{
					nMaxWidth = nCurWidth;
				}

				rcItem.bottom += s_nEachLineHeight;
			}

			//////////////////////////////////////////////////////////////////////////
		}
	}

	m_nSameUpgradeAddition = 0;

	/*if(MirGame::IsEquipItem(*m_pItemAttrib))
	{
		int nPrevValidUpgrade = 0;

		for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
		{
			ItemAttrib* pPlayerItem = GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i);

			if(pPlayerItem->type != ITEM_NO)
			{
				nPrevValidUpgrade = GetItemUpgrade(pPlayerItem->level);
				break;
			}
		}

		if(nPrevValidUpgrade != 0)
		{
			int nSameGrade = nPrevValidUpgrade;

			for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
			{
				ItemAttrib* pPlayerItem = GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i);

				if(pPlayerItem->type != ITEM_NO)
				{
					int nItemGrade = GetItemUpgrade(pPlayerItem->level);

					if(nItemGrade != nSameGrade)
					{
						nSameGrade = 0;
						break;
					}
				}
			}

			if(nSameGrade > 0)
			{
				m_nSameUpgradeAddition = nSameGrade;

				//	A line blank
				rcItem.bottom += s_nEachLineHeight;
				//	[加成效果]
				rcItem.bottom += s_nEachLineHeight;

				//if(nSameGrade >= )
			}
		}
	}*/

	//	No save, No sell
	if(TEST_FLAG_BOOL(m_pItemAttrib->EXPR, EXPR_MASK_NOSAVE) ||
		TEST_FLAG_BOOL(m_pItemAttrib->EXPR, EXPR_MASK_NOSELL))
	{
		//	A line blank
		rcItem.bottom += s_nEachLineHeight;

		if(TEST_FLAG_BOOL(m_pItemAttrib->EXPR, EXPR_MASK_NOSAVE))
		{
			//	No save
			rcItem.bottom += s_nEachLineHeight;
		}
		if(TEST_FLAG_BOOL(m_pItemAttrib->EXPR, EXPR_MASK_NOSELL))
		{
			//	No sell
			rcItem.bottom += s_nEachLineHeight;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//	Last,Get the maximum width of the itemrect, and the 5 pixel of the boundary
	rcItem.right += s_nLeftBoundary;
	rcItem.right += nMaxWidth;
	rcItem.bottom += s_nTopBoundary;

	//	Get the position of the name
	m_nNamePosX = nMaxWidth / 2 - GetTextWidth(m_pItemAttrib->name, 14) / 2 + s_nLeftBoundary;

	m_rcItemRect = rcItem;
	SetVisible(true);

	//	Get the item color
	m_dwItemNameColor = ARGB_YELLOW;
	m_nColorIndex = 0;
	if(GamePlayer::GetInstance()->IsEquipItem(*m_pItemAttrib))
	{
		WORD wLevel = m_pItemAttrib->level;
		m_dwItemNameColor = ARGB_WHITE;

		if(wLevel != 0)
		{
			/*BYTE bLow = LOBYTE(wLevel);
			BYTE bHigh = HIBYTE(wLevel);

			bHigh &= 0x7A;
			bool bZero = false;

			if((bHigh & 0x40) == 0)
			{
				bZero = ((bHigh & 0x10) != 0 ? false : true);
				if(bZero)
				{
					bHigh |= 0x10;
				}
				else
				{
					bHigh &= (~0x10);
				}
			}

			if((bHigh & 0x02) == 0)
			{
				bZero = ((bHigh & 0x08) != 0 ? false : true);
				if(bZero)
				{
					bHigh |= 0x08;
				}
				else
				{
					bHigh &= (~0x08);
				}
			}

			static int s_nValueTable[] =
			{
				9, 2, 1, 3, 4, 7, 8, 5
			};
			static BYTE s_btMaskTable[] =
			{
				0x80, 0x40, 0x20, 0x10,
				0x08, 0x04, 0x02, 0x01
			};

			int nValue = 0;
			for(int i = 0; i < 8; ++i)
			{
				if((bHigh & s_btMaskTable[i]) != 0)
				{
					nValue += s_nValueTable[i];
				}
			}*/

			int nValue = GetItemUpgrade(wLevel);
			if(nValue >= 0 &&
				nValue <= 8)
			{
				m_dwItemNameColor = g_dwItemColor[nValue];
				m_nColorIndex = nValue;
			}
		}
	}

	return true;
}

bool GameIntroDlg::RenderItemAttribSec()
{
	//return false;

	if(!m_pItemAttrib)
	{
		return true;
	}

	RECT rcDrawRect = m_rcItemRect;

	float fx,fy;
	AfxGetHge()->Input_GetMousePos(&fx, &fy);
	fx += 5.0f;
	fy += 5.0f;
	int nRectWidth = m_rcItemRect.right - m_rcItemRect.left;
	int nRectHeight = m_rcItemRect.bottom - m_rcItemRect.top;
	rcDrawRect.left = fx;
	rcDrawRect.right = rcDrawRect.left + nRectWidth;
	rcDrawRect.top = fy;
	rcDrawRect.bottom = rcDrawRect.top + nRectHeight;

	if(rcDrawRect.bottom > WINDOW_HEIGHT)
	{
		rcDrawRect.top -= (rcDrawRect.bottom - WINDOW_HEIGHT);
		rcDrawRect.bottom = rcDrawRect.top + nRectHeight;
	}
	if(rcDrawRect.right > WINDOW_WIDTH)
	{
		rcDrawRect.left -= (rcDrawRect.right - WINDOW_WIDTH);
		rcDrawRect.right = rcDrawRect.left + nRectWidth;
	}

	HTEXTURE tex = 0;
	tex = m_pResManager->GetTexture("bmcolor");
	if(tex)
	{
		INITSPRITE(m_pSpr, tex);
		SETSPRITEWH(tex, m_pSpr, m_rcItemRect.right - m_rcItemRect.left, m_rcItemRect.bottom - m_rcItemRect.top);
		m_pSpr->Render(rcDrawRect.left, rcDrawRect.top);
	}

#define POS_LEFT rcDrawRect.left + s_nLeftBoundary
#define POS_TOP	 rcDrawRect.top + s_nTopBoundary

	int nCurX = rcDrawRect.left;
	int nCurY = rcDrawRect.top;
	int nNamePosX = m_nNamePosX + rcDrawRect.left;

	//	Render Name
	nCurX += s_nLeftBoundary;
	nCurY += s_nTopBoundary;
	AfxGetFont14()->SetColor(ARGB_YELLOW);
	AfxGetFont14()->Print(nNamePosX, nCurY, m_pItemAttrib->name);

	//	Item icon
	int nTexWidth = 0;
	int nTexHeight = 0;
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	/*nCurX = POS_LEFT;
	nCurY = POS_TOP;
	nCurY += s_nTitleHeight;
	tex = pTexMgr->GetTexture(m_pItemAttrib->tex);
	if(tex)
	{
		nTexWidth = pTexMgr->GetTextureWidth(m_pItemAttrib->tex);
		nTexHeight = pTexMgr->GetTextureHeight(m_pItemAttrib->tex);
		m_pSpr->SetTexture(tex);
		m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
		m_pSpr->Render(nCurX + s_nItemIconWidth / 2 - nTexWidth / 2, nCurY + s_nItemIconHeight / 2 - nTexHeight / 2);
	}*/
	//	Item boarder
	nCurX = POS_LEFT;
	nCurY = POS_TOP;
	nCurY += s_nTitleHeight;
	/*AfxGetHge()->Gfx_RenderLine(nCurX, nCurY, nCurX + s_nItemIconWidth, nCurY, ARGB(255, 69, 89, 137));
	AfxGetHge()->Gfx_RenderLine(nCurX, nCurY + s_nItemIconHeight, nCurX + s_nItemIconWidth, nCurY + s_nItemIconHeight, ARGB(255, 69, 89, 137));
	AfxGetHge()->Gfx_RenderLine(nCurX, nCurY, nCurX, nCurY + s_nItemIconHeight, ARGB(255, 69, 89, 137));
	AfxGetHge()->Gfx_RenderLine(nCurX + s_nItemIconWidth, nCurY, nCurX + s_nItemIconWidth, nCurY + s_nItemIconHeight, ARGB(255, 69, 89, 137));
*/
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	tex = pPngMgr->GetTexture(0);
	if(tex)
	{
		nTexWidth = pPngMgr->GetTextureWidth(0);
		nTexHeight = pPngMgr->GetTextureHeight(0);
		m_pSpr->SetTexture(tex);
		m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
		m_pSpr->Render(nCurX, nCurY);
	}

	//	draw item icon
	nCurX = POS_LEFT;
	nCurY = POS_TOP;
	nCurY += s_nTitleHeight;
	tex = pTexMgr->GetTexture(m_pItemAttrib->tex);
	if(tex)
	{
		nTexWidth = pTexMgr->GetTextureWidth(m_pItemAttrib->tex);
		nTexHeight = pTexMgr->GetTextureHeight(m_pItemAttrib->tex);
		m_pSpr->SetTexture(tex);
		m_pSpr->SetTextureRect(0, 0, nTexWidth, nTexHeight);
		m_pSpr->Render(nCurX + s_nItemIconWidth / 2 - nTexWidth / 2, nCurY + s_nItemIconHeight / 2 - nTexHeight / 2);
	}

	int nIntroLine = 0;
	//	Type
	nCurX = POS_LEFT;
	nCurY = POS_TOP + 5;
	nCurY += s_nTitleHeight + nIntroLine * s_nEachLineHeight;
	nCurX += s_nItemIconWidth;
	nCurX += s_nItemIntroGapX;
	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "类型：%s", s_szItemTypeName[m_pItemAttrib->type]);
	++nIntroLine;
	//	Weight
	nCurX = POS_LEFT;
	nCurY = POS_TOP + 5;
	nCurY += s_nTitleHeight + nIntroLine * s_nEachLineHeight;
	nCurX += s_nItemIconWidth;
	nCurX += s_nItemIntroGapX;
	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "重量：%d", m_pItemAttrib->weight);
	++nIntroLine;
	//	Dura
	/*if(MirGame::IsEquipItem(*m_pItemAttrib))
	{
		int nDura = HIWORD(m_pItemAttrib->maxHP) / 1000;
		int nDuraMax = LOWORD(m_pItemAttrib->maxHP) / 1000;

		nCurX = POS_LEFT;
		nCurY = POS_TOP + 5;
		nCurY += s_nTitleHeight + nIntroLine * s_nEachLineHeight;
		nCurX += s_nItemIconWidth;
		nCurX += s_nItemIntroGapX;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "持久：%d/%d", nDura, nDuraMax);
		++nIntroLine;
	}*/
	//	Binded or not
	nCurX = POS_LEFT;
	nCurY = POS_TOP + 5;
	nCurY += s_nTitleHeight + nIntroLine * s_nEachLineHeight;
	nCurX += s_nItemIconWidth;
	nCurX += s_nItemIntroGapX;
	AfxGetPrinter()->SetColor(ARGB_WHITE);
	AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, TEST_FLAG_BOOL(m_pItemAttrib->atkPois, POIS_MASK_BIND) ? "[已绑定]" : "[未绑定]");
	++nIntroLine;

	//	Details
#define D_POS_LEFT rcDrawRect.left + s_nLeftBoundary
#define D_POS_TOP  rcDrawRect.top + s_nTopBoundary + s_nTitleHeight + s_nItemIntroGapX + s_nItemIconHeight

	int nDetailLine = 0;

	if(MirGame::IsEquipItem(*m_pItemAttrib))
	{
		//	Dura quality
		nCurX = D_POS_LEFT;
		nCurY = D_POS_TOP;

		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "[装备信息]");
		++nDetailLine;

		nCurX = D_POS_LEFT;
		nCurY = D_POS_TOP;
		nCurY += s_nEachLineHeight * nDetailLine;
		int nDura = HIWORD(m_pItemAttrib->maxHP) / 1000;
		int nDuraMax = LOWORD(m_pItemAttrib->maxHP) / 1000;
		AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "持久：%d/%d", nDura, nDuraMax);
		++nDetailLine;

		static const char* s_pszItemLevel[] =
		{
			"普通",
			"良品",
			"精致",
			"无暇",
			"完美",
			"卓越",
			"登峰",
			"传说",
			"史诗"
		};
		nCurX = D_POS_LEFT;
		nCurY = D_POS_TOP;
		nCurY += s_nEachLineHeight * nDetailLine;
		AfxGetPrinter()->SetColor(g_dwItemColor[m_nColorIndex]);
		AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "品质：%s", s_pszItemLevel[m_nColorIndex]);
		++nDetailLine;

		//	Level
		int nLevel = GameInfoManager::GetInstance()->GetItemGradeInFullAttrib(m_pItemAttrib->id);
		nCurX = D_POS_LEFT;
		nCurY = D_POS_TOP;
		nCurY += s_nEachLineHeight * nDetailLine;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "级别：%d", nLevel);
		++nDetailLine;

		//	A line blank
		++nDetailLine;

		//	[基础属性]
		nCurX = D_POS_LEFT;
		nCurY = D_POS_TOP;
		nCurY += s_nEachLineHeight * nDetailLine;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "[基础属性]");
		++nDetailLine;

		//	AC
		if(m_pItemAttrib->AC != 0 ||
			m_pItemAttrib->maxAC != 0)
		{
			if(m_pItemAttrib->type != ITEM_WEAPON &&
				m_pItemAttrib->type != ITEM_NECKLACE)
			{
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(m_oriItem.maxAC != 0)
				{
					AfxGetPrinter()->SetColor(ARGB_GREEN);
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "防御：%d-%d(+%d)",
						m_pItemAttrib->AC, m_pItemAttrib->maxAC, m_oriItem.maxAC);
				}
				else
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "防御：%d-%d",
						m_pItemAttrib->AC, m_pItemAttrib->maxAC);
				}
				
				++nDetailLine;
			}
		}
		//	MAC
		if(m_pItemAttrib->MAC != 0 ||
			m_pItemAttrib->maxMAC != 0)
		{
			if(m_pItemAttrib->type != ITEM_WEAPON &&
				m_pItemAttrib->type != ITEM_NECKLACE)
			{
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(m_oriItem.maxMAC != 0)
				{
					AfxGetPrinter()->SetColor(ARGB_GREEN);
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "魔御：%d-%d(+%d)",
						m_pItemAttrib->MAC, m_pItemAttrib->maxMAC, m_oriItem.maxMAC);
				}
				else
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "魔御：%d-%d",
						m_pItemAttrib->MAC, m_pItemAttrib->maxMAC);
				}
				
				++nDetailLine;
			}
		}
		//	DC
		if(m_pItemAttrib->DC != 0 ||
			m_pItemAttrib->maxDC != 0)
		{
			nCurX = D_POS_LEFT;
			nCurY = D_POS_TOP;
			nCurY += s_nEachLineHeight * nDetailLine;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			if(m_oriItem.maxDC != 0)
			{
				AfxGetPrinter()->SetColor(ARGB_GREEN);
				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "攻击：%d-%d(+%d)",
					m_pItemAttrib->DC, m_pItemAttrib->maxDC, m_oriItem.maxDC);
			}
			else
			{
				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "攻击：%d-%d",
					m_pItemAttrib->DC, m_pItemAttrib->maxDC);
			}
			
			++nDetailLine;
		}
		//	MC
		if(m_pItemAttrib->MC != 0 ||
			m_pItemAttrib->maxMC != 0)
		{
			nCurX = D_POS_LEFT;
			nCurY = D_POS_TOP;
			nCurY += s_nEachLineHeight * nDetailLine;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			if(m_oriItem.maxMC != 0)
			{
				AfxGetPrinter()->SetColor(ARGB_GREEN);
				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "魔法：%d-%d(+%d)",
					m_pItemAttrib->MC, m_pItemAttrib->maxMC, m_oriItem.maxMC);
			}
			else
			{
				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "魔法：%d-%d",
					m_pItemAttrib->MC, m_pItemAttrib->maxMC);
			}
			
			++nDetailLine;
		}
		//	SC
		if(m_pItemAttrib->SC != 0 ||
			m_pItemAttrib->maxSC != 0)
		{
			nCurX = D_POS_LEFT;
			nCurY = D_POS_TOP;
			nCurY += s_nEachLineHeight * nDetailLine;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			if(m_oriItem.maxSC != 0)
			{
				AfxGetPrinter()->SetColor(ARGB_GREEN);
				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "道术：%d-%d(+%d)",
					m_pItemAttrib->SC, m_pItemAttrib->maxSC, m_oriItem.maxSC);
			}
			else
			{
				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "道术：%d-%d",
					m_pItemAttrib->SC, m_pItemAttrib->maxSC);
			}
			
			++nDetailLine;
		}

		//	A line blank
		//++nDetailLine;

		//	[特殊属性]
		//nCurX = D_POS_LEFT;
		//nCurY = D_POS_TOP;
		//nCurY += s_nEachLineHeight * nDetailLine;
		//AfxGetPrinter()->SetColor(ARGB_WHITE);
		//AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "[特殊属性]");
		//++nDetailLine;

		//	Lucky
		if(m_pItemAttrib->lucky != 0)
		{
			if(m_pItemAttrib->type == ITEM_WEAPON ||
				m_pItemAttrib->type == ITEM_NECKLACE)
			{
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(m_oriItem.lucky != 0)
				{
					AfxGetPrinter()->SetColor(ARGB_GREEN);
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "幸运：+%d(+%d)",
						m_pItemAttrib->lucky, m_oriItem.lucky);
				}
				else
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "幸运：+%d",
						m_pItemAttrib->lucky);
				}
				
				++nDetailLine;
			}
		}
		//	AtkSpeed
		if(m_pItemAttrib->atkSpeed != 0)
		{
			if(m_pItemAttrib->type == ITEM_RING ||
				m_pItemAttrib->type == ITEM_NECKLACE)
			{
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(m_oriItem.atkSpeed != 0)
				{
					AfxGetPrinter()->SetColor(ARGB_GREEN);
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "攻击速度：+%d(+%d)",
						m_pItemAttrib->atkSpeed, m_oriItem.atkSpeed);
				}
				else
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "攻击速度：+%d",
						m_pItemAttrib->atkSpeed);
				}
				
				++nDetailLine;
			}
		}
		//	MoveSpeed
		if(m_pItemAttrib->moveSpeed != 0)
		{
			//if(m_pItemAttrib->type == ITEM_RING ||
				//m_pItemAttrib->type == ITEM_NECKLACE)
			{
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(m_oriItem.moveSpeed != 0)
				{
					AfxGetPrinter()->SetColor(ARGB_GREEN);
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "移动速度：+%d(+%d)",
						m_pItemAttrib->moveSpeed, m_oriItem.moveSpeed);
				}
				else
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "移动速度：+%d",
						m_pItemAttrib->moveSpeed);
				}
				
				++nDetailLine;
			}
		}
		//	Accuracy
		if(m_pItemAttrib->accuracy != 0)
		{
			if(m_pItemAttrib->type == ITEM_BRACELAT ||
			m_pItemAttrib->type == ITEM_NECKLACE ||
			m_pItemAttrib->type == ITEM_WEAPON)
			{
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(m_oriItem.accuracy != 0)
				{
					AfxGetPrinter()->SetColor(ARGB_GREEN);
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "准确：+%d(+%d)",
						m_pItemAttrib->accuracy, m_oriItem.accuracy);
				}
				else
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "准确：+%d",
						m_pItemAttrib->accuracy);
				}
				
				++nDetailLine;
			}
		}
		//	Hide
		if(m_pItemAttrib->hide != 0)
		{
			if(m_pItemAttrib->type == ITEM_BRACELAT ||
			m_pItemAttrib->type == ITEM_NECKLACE)
			{
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(m_oriItem.hide != 0)
				{
					AfxGetPrinter()->SetColor(ARGB_GREEN);
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "躲避：+%d(+%d)",
						m_pItemAttrib->hide, m_oriItem.hide);
				}
				else
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "躲避：+%d",
						m_pItemAttrib->hide);
				}
				
				++nDetailLine;
			}
		}
	}
	else if(m_pItemAttrib->type == ITEM_COST)
	{
		//	[物品信息]
		{
			nCurX = D_POS_LEFT;
			nCurY = D_POS_TOP;
			nCurY += s_nEachLineHeight * nDetailLine;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "[物品信息]");
			++nDetailLine;
		}
		//	Count
		{
			nCurX = D_POS_LEFT;
			nCurY = D_POS_TOP;
			nCurY += s_nEachLineHeight * nDetailLine;
			AfxGetPrinter()->SetColor(ARGB_WHITE);
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "数量：%d",
				m_pItemAttrib->atkSpeed);
			++nDetailLine;
		}

		//	Drug
		if(m_pItemAttrib->curse == 0)
		{
			//	HP
			if(m_pItemAttrib->HP != 0)
			{
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(m_pItemAttrib->lucky == 0)
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "缓慢补充HP：%d",
						m_pItemAttrib->HP);
				}
				else if(m_pItemAttrib->lucky == 1)
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "急速补充HP：%d",
						m_pItemAttrib->HP);
				}
				++nDetailLine;
			}
			//	MP
			if(m_pItemAttrib->MP != 0)
			{
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				if(m_pItemAttrib->lucky == 0)
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "缓慢补充MP：%d",
						m_pItemAttrib->MP);
				}
				else if(m_pItemAttrib->lucky == 1)
				{
					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "急速补充MP：%d",
						m_pItemAttrib->MP);
				}
				++nDetailLine;
			}
			//	Cooldown time
			if(m_pItemAttrib->hide != 0)
			{
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "冷却时间：%d秒",
					m_pItemAttrib->hide);
				++nDetailLine;
			}
		}
	}
	else if(m_pItemAttrib->type == ITEM_BOOK)
	{
		//	[秘籍信息]
		nCurX = D_POS_LEFT;
		nCurY = D_POS_TOP;
		nCurY += s_nEachLineHeight * nDetailLine;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "[秘籍信息]");
		++nDetailLine;

		//	Job
		nCurX = D_POS_LEFT;
		nCurY = D_POS_TOP;
		nCurY += s_nEachLineHeight * nDetailLine;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		if(m_pItemAttrib->lucky == 1)
		{
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "战士秘籍");
		}
		else if(m_pItemAttrib->lucky == 2)
		{
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "法师秘籍");
		}
		else if(m_pItemAttrib->lucky == 3)
		{
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "道士秘籍");
		}
		else
		{
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "无需求");
		}

		++nDetailLine;
	}
	

	if(m_pItemAttrib->type == ITEM_BOOK ||
		MirGame::IsEquipItem(*m_pItemAttrib))
	{
		//	A line blank
		++nDetailLine;

		nCurX = D_POS_LEFT;
		nCurY = D_POS_TOP;
		nCurY += s_nEachLineHeight * nDetailLine;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		
		DWORD dwErr = 0;
		if(!GamePlayer::GetInstance()->CanEquip(m_pItemAttrib, &dwErr))
		{
			AfxGetPrinter()->SetColor(ARGB_RED);
		}

		if(m_pItemAttrib->reqType == REQ_LEVEL)
		{
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "需要等级：%d",
				m_pItemAttrib->reqValue);
		}
		else if(m_pItemAttrib->reqType == REQ_DC)
		{
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "需要攻击：%d",
				m_pItemAttrib->reqValue);
		}
		else if(m_pItemAttrib->reqType == REQ_MC)
		{
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "需要魔法：%d",
				m_pItemAttrib->reqValue);
		}
		else if(m_pItemAttrib->reqType == REQ_SC)
		{
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "需要道术：%d",
				m_pItemAttrib->reqValue);
		}
		else
		{
			AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "无使用需求");
		}
		++nDetailLine;
	}

	//	Suit attribute
	if(MirGame::IsEquipItem(*m_pItemAttrib))
	{
		if(m_pItemAttrib->atkPalsy != 0 &&
			m_stSuitAttribRenderInfo.bCanShow)
		{
			//if(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitShowType == 0)
			{
				//	套装属性才显示各个部件

				//	A line blank
				++nDetailLine;

				//	Suit name
				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;
				AfxGetPrinter()->SetColor(ARGB_GREEN);
				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "[%s](%d/%d)",
					m_stSuitAttribRenderInfo.pExtraAttribList->szSuitChName,
					m_stSuitAttribRenderInfo.nActiveItemSum,
					m_stSuitAttribRenderInfo.nActiveItemAll);
				++nDetailLine;

				for(int i = 0; i < 10; ++i)
				{
					if(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitEquipID[i] == 0)
					{
						break;
					}

					nCurX = D_POS_LEFT;
					nCurY = D_POS_TOP;
					nCurY += s_nEachLineHeight * nDetailLine;

					if(m_stSuitAttribRenderInfo.nActiveItemPos[i] != 0)
					{
						AfxGetPrinter()->SetColor(COLOR_SUITNAMEITEM_ACTIVE);
					}
					else
					{
						AfxGetPrinter()->SetColor(COLOR_SUITNAMEITEM_UNACTIVE);
					}

					AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, m_xSuitItemName[i].c_str());
					++nDetailLine;
				}
			}

			//	A line blank
			++nDetailLine;
			//	[套装效果]
			nCurX = D_POS_LEFT;
			nCurY = D_POS_TOP;
			nCurY += s_nEachLineHeight * nDetailLine;
			AfxGetPrinter()->SetColor(ARGB_GREEN);
			if(m_stSuitAttribRenderInfo.pExtraAttribList->nSuitShowType == 1)
			{
				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "[特殊效果]");
			}
			else
			{
				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, "[套装效果]");
			}
			++nDetailLine;

			//	Suit effect
			char szPrintBuf[100];
			ItemExtraAttribItem extraItem;

			for(int i = 0; i < /*m_stSuitAttribRenderInfo.nAttribAll*/10; ++i)
			{
				if(m_stSuitAttribRenderInfo.pExtraAttribList->stExtraAttrib[i].nAttribID == 0)
				{
					break;
				}

				ZeroMemory(szPrintBuf, sizeof(szPrintBuf));

				if(m_stSuitAttribRenderInfo.nActiveAttribSum > i)
				{
					AfxGetPrinter()->SetColor(COLOR_EXTRAATRRIB_ACTIVE);
				}
				else
				{
					AfxGetPrinter()->SetColor(COLOR_EXTRAATTRIB_UNACTIVE);
				}

				extraItem = m_stSuitAttribRenderInfo.pExtraAttribList->stExtraAttrib[i];
				if(extraItem.nAttribID == EAID_AC ||
					extraItem.nAttribID == EAID_MAC ||
					extraItem.nAttribID == EAID_DC ||
					extraItem.nAttribID == EAID_MC ||
					extraItem.nAttribID == EAID_SC)
				{
					sprintf(szPrintBuf, "%s%d-%d",
						g_szExtraAttribDescriptor[extraItem.nAttribID], HIWORD(extraItem.nAttribValue), LOWORD(extraItem.nAttribValue));
				}
				else if(extraItem.nAttribID >= EAID_LUCKY &&
					extraItem.nAttribID <= EAID_ADDTIGER)
				{
					sprintf(szPrintBuf, "%s+%d",
						g_szExtraAttribDescriptor[extraItem.nAttribID], extraItem.nAttribValue);
				}

				nCurX = D_POS_LEFT;
				nCurY = D_POS_TOP;
				nCurY += s_nEachLineHeight * nDetailLine;

				AfxGetPrinter()->PrintWithoutStroke(nCurX, nCurY, szPrintBuf);
				++nDetailLine;
			}
		}
	}

	//	No save and no sell
	if(TEST_FLAG_BOOL(m_pItemAttrib->EXPR, EXPR_MASK_NOSAVE) ||
		TEST_FLAG_BOOL(m_pItemAttrib->EXPR, EXPR_MASK_NOSELL))
	{
		//	A line blank
		++nDetailLine;

		//	No save
		if(TEST_FLAG_BOOL(m_pItemAttrib->EXPR, EXPR_MASK_NOSAVE))
		{
			nCurX = D_POS_LEFT;
			nCurY = D_POS_TOP;
			nCurY += s_nEachLineHeight * nDetailLine;
			AfxGetPrinter()->SetColor(ARGB_RED);
			AfxGetPrinter()->Print(nCurX, nCurY, "（无法保存）");
			++nDetailLine;
		}
		if(TEST_FLAG_BOOL(m_pItemAttrib->EXPR, EXPR_MASK_NOSELL))
		{
			nCurX = D_POS_LEFT;
			nCurY = D_POS_TOP;
			nCurY += s_nEachLineHeight * nDetailLine;
			AfxGetPrinter()->SetColor(ARGB_RED);
			AfxGetPrinter()->Print(nCurX, nCurY, "（无法出售）");
			++nDetailLine;
		}
	}

	return true;
}