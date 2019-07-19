#include "GameOtherHumDlg.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
#include "../GameDialog/GameDisplayDlg.h"
//////////////////////////////////////////////////////////////////////////
static const int s_nEquipOftX = 22;
static const int s_nEquipOftY = 72;
#define DLG_WIDTH	(s_nEquipOftX * 2 + 168)
#define DLG_HEIGHT	(234 + 72 + 50)
//////////////////////////////////////////////////////////////////////////
GameOtherHumDlg::GameOtherHumDlg()
{
	m_pRender = NEW_EMPTY_SPRITE;

	m_rcClient.left = WINDOW_WIDTH - DLG_WIDTH;
	m_rcClient.right = m_rcClient.left + DLG_WIDTH;
	m_rcClient.bottom = m_rcClient.top + DLG_HEIGHT;
	SetWindowTitle("玩家");
	CalcCloseButtonPosition();
	ClearItems();

	m_bSex = 0;
	SET_POINT(m_ptItems[PLAYER_ITEM_BRACELAT1], 4, 124);
	SET_POINT(m_ptItems[PLAYER_ITEM_RING1], 4, 163);
	SET_POINT(m_ptItems[PLAYER_ITEM_NECKLACE], 130, 35);
	SET_POINT(m_ptItems[PLAYER_ITEM_MEDAL], 130, 73);
	SET_POINT(m_ptItems[PLAYER_ITEM_BRACELAT2], 130, 124);
	SET_POINT(m_ptItems[PLAYER_ITEM_RING2], 130, 163);
	SET_POINT(m_ptItems[PLAYER_ITEM_CHARM], 5, 203);
	SET_POINT(m_ptItems[PLAYER_ITEM_BELT], 45, 203);
	SET_POINT(m_ptItems[PLAYER_ITEM_SHOE], 89, 203);
	SET_POINT(m_ptItems[PLAYER_ITEM_GEM], 130, 203);
	SetVisible(false);

	GameInfoManager::GetInstance()->GetItemAttrib(1265, &m_stItems[PLAYER_ITEM_HELMET]);
	GameInfoManager::GetInstance()->GetItemAttrib(1264, &m_stItems[PLAYER_ITEM_BELT]);
	GameInfoManager::GetInstance()->GetItemAttrib(1263, &m_stItems[PLAYER_ITEM_SHOE]);
	GameInfoManager::GetInstance()->GetItemAttrib(1262, &m_stItems[PLAYER_ITEM_NECKLACE]);
	GameInfoManager::GetInstance()->GetItemAttrib(1260, &m_stItems[PLAYER_ITEM_RING1]);
}

GameOtherHumDlg::~GameOtherHumDlg()
{
	SAFE_DELETE(m_pRender);
}


void GameOtherHumDlg::Render()
{
	__super::Render();

	GameTextureManager* pMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	GameTextureManager* pItemsMgr = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	GameTextureManager* pStateMgr = GameResourceManager::GetInstance()->GetTexs(RES_STATE);
	HTEXTURE tex = 0;

	if(m_bSex == 1 ||
		m_bSex == 0)
	{
		tex = pMgr->GetTexture(TEX_HUMPAGE_MALE);
	}
	else if(m_bSex == 2)
	{
		tex = pMgr->GetTexture(TEX_HUMPAGE_FEMALE);
	}

	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 168, 234);
		m_pRender->Render(RELATIVE_X(s_nEquipOftX), RELATIVE_Y(s_nEquipOftY));
	}

	SHORT sOftX = 0;
	SHORT sOftY = 0;
	if(m_bSex == 1 ||
		m_bSex == 0)
	{
		tex = pMgr->GetTexture(TEX_HUMHAIR_MALE);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 16, 14);
			sOftX = pMgr->GetTextureOffsetX(TEX_HUMHAIR_MALE);
			sOftY = pMgr->GetTextureOffsetY(TEX_HUMHAIR_MALE);
			m_pRender->Render(RELATIVE_X(-7 + sOftX + s_nEquipOftX), RELATIVE_Y(45 + sOftY + s_nEquipOftY));
		}
	}
	else if(m_bSex == 2)
	{
		tex = pMgr->GetTexture(TEX_HUMHAIR_FEMALE);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 20, 24);
			sOftX = pMgr->GetTextureOffsetX(TEX_HUMHAIR_FEMALE);
			sOftY = pMgr->GetTextureOffsetY(TEX_HUMHAIR_FEMALE);
			m_pRender->Render(RELATIVE_X(-7 + sOftX + s_nEquipOftX), RELATIVE_Y(45 + sOftY + s_nEquipOftY));
		}
	}

	//	画装备
	const ItemAttrib* pItem = NULL;
	int nTexWidth = 0;
	int nTexHeight = 0;
	int nDrawX = 0;
	int nDrawY = 0;
	int nOftX = 0;
	int nOftY = 0;

	static int s_nXOft = -7;
	static int s_nYOft = 45;

	//	衣服
	pItem = GetItem(PLAYER_ITEM_CLOTH);
	if(pItem->type != ITEM_NO)
	{
		tex = pStateMgr->GetTexture(pItem->tex);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			nTexWidth = pStateMgr->GetTextureWidth(pItem->tex);
			nTexHeight = pStateMgr->GetTextureHeight(pItem->tex);
			m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
			nOftX = pStateMgr->GetTextureOffsetX(pItem->tex);
			nOftY = pStateMgr->GetTextureOffsetY(pItem->tex);
			nDrawX = m_rcClient.left + nOftX + s_nEquipOftX;
			nDrawY = m_rcClient.top + nOftY + s_nEquipOftY;
			m_pRender->Render(nDrawX + s_nXOft, nDrawY + s_nYOft);
		}
	}
	//	头盔
	pItem = GetItem(PLAYER_ITEM_HELMET);
	if(pItem->type != ITEM_NO)
	{
		tex = pStateMgr->GetTexture(pItem->tex);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			nTexWidth = pStateMgr->GetTextureWidth(pItem->tex);
			nTexHeight = pStateMgr->GetTextureHeight(pItem->tex);
			m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
			nOftX = pStateMgr->GetTextureOffsetX(pItem->tex);
			nOftY = pStateMgr->GetTextureOffsetY(pItem->tex);
			nDrawX = m_rcClient.left + nOftX + s_nEquipOftX;
			nDrawY = m_rcClient.top + nOftY + s_nEquipOftY;
			m_pRender->Render(nDrawX + s_nXOft, nDrawY + s_nYOft);
		}
	}
	//	武器
	pItem = GetItem(PLAYER_ITEM_WEAPON);
	if(pItem->type != ITEM_NO)
	{
		tex = pStateMgr->GetTexture(pItem->tex);
		if(tex)
		{
			m_pRender->SetTexture(tex);
			nTexWidth = pStateMgr->GetTextureWidth(pItem->tex);
			nTexHeight = pStateMgr->GetTextureHeight(pItem->tex);
			m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
			nOftX = pStateMgr->GetTextureOffsetX(pItem->tex);
			nOftY = pStateMgr->GetTextureOffsetY(pItem->tex);
			nDrawX = m_rcClient.left + nOftX + s_nEquipOftX;
			nDrawY = m_rcClient.top + nOftY + s_nEquipOftY;
			m_pRender->Render(nDrawX + s_nXOft, nDrawY + s_nYOft);
		}

		/*if(pPlayer->GetPlayerItem(PLAYER_ITEM_WEAPON)->id == 1880)
		{
		//	倚天剑
		static float fFlash = 0.0f;
		fFlash += AfxGetHge()->Timer_GetDelta();
		if(fFlash > 2.0f)
		{
		fFlash = 0.0f
		}

		int nTextIndex = (int)(fFlash / 0.2f);
		if(nTextIndex >= 0 &&
		nTextIndex < 10)
		{
		tex = pStateMgr->GetTexture(1890 + nTextIndex);
		if(tex)
		{
		MagicElement::pEffectRender->SetTexture(tex);
		nTexWidth = pStateMgr->GetTextureWidth(1890 + nTextIndex);
		nTexHeight = pStateMgr->GetTextureHeight(1890 + nTextIndex);
		MagicElement::pEffectRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
		nOftX = pStateMgr->GetTextureOffsetX(1890 + nTextIndex);
		nOftY = pStateMgr->GetTextureOffsetY(1890 + nTextIndex);
		nDrawX = m_rcClient.left + nOftX + s_nEquipOftX;
		nDrawY = m_rcClient.top + nOftY + s_nEquipOftY;
		MagicElement::pEffectRender->Render(nDrawX + s_nXOft, nDrawY + s_nYOft);
		}
		}
		}*/
	}

	for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
	{
		if(m_ptItems[i].x != m_ptItems[i].y)
		{
			pItem = GetItem((PLAYER_ITEM_TYPE)i);
			if(i == PLAYER_ITEM_CLOTH)
			{
				continue;
			}
			if(i == PLAYER_ITEM_HELMET)
			{
				continue;
			}
			if(i == PLAYER_ITEM_WEAPON)
			{
				continue;
			}
			if(pItem->type != ITEM_NO)
			{
				tex = pItemsMgr->GetTexture(pItem->tex);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					nTexWidth = pItemsMgr->GetTextureWidth(pItem->tex);
					nTexHeight = pItemsMgr->GetTextureHeight(pItem->tex);
					m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
					nDrawX = (35 - nTexWidth) / 2 + m_rcClient.left + s_nEquipOftX + m_ptItems[i].x;
					nDrawY = (32 - nTexHeight) / 2 + m_rcClient.top + s_nEquipOftY + m_ptItems[i].y;
					m_pRender->Render(nDrawX, nDrawY);
				}
			}
		}
	}

	GameOtherItemDisplayDlg::GetInstance()->Render();
}

void GameOtherHumDlg::ClearItems()
{
	ZeroMemory(m_stItems, sizeof(m_stItems));
}

void GameOtherHumDlg::SetItem(const ItemAttrib* _pItem, int _nIndex)
{
	if(_nIndex >= 0 &&
		_nIndex < PLAYER_ITEM_TOTAL)
	{
		memcpy(&m_stItems[_nIndex], _pItem, sizeof(ItemAttrib));
	}
}

const ItemAttrib* GameOtherHumDlg::GetItem(int _nIndex)
{
	if(_nIndex >= 0 &&
		_nIndex < PLAYER_ITEM_TOTAL)
	{
		return &m_stItems[_nIndex];
	}
	return NULL;
}

bool GameOtherHumDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	POINT ptDetect = _mp;
	ptDetect.x -= s_nEquipOftX;
	ptDetect.y -= s_nEquipOftY;
	ptDetect.x -= m_rcClient.left;
	ptDetect.y -= m_rcClient.top;

	PLAYER_ITEM_TYPE eItem = PLAYER_ITEM_TOTAL;

	if(ptDetect.x >= 4 &&
		ptDetect.x <= 39 &&
		ptDetect.y >= 124 &&
		ptDetect.y <= 194)
	{
		//	第一列
		if(ptDetect.y >= 124 &&
			ptDetect.y <= 155)
		{
			//	手镯1
			eItem = PLAYER_ITEM_BRACELAT1;
		}
		else if(ptDetect.y >= 163 &&
			ptDetect.y <= 194)
		{
			eItem = PLAYER_ITEM_RING1;
		}
	}
	else if(ptDetect.x >= 130 &&
		ptDetect.x <= 165 &&
		ptDetect.y >= 35 &&
		ptDetect.y <= 194)
	{
		if(ptDetect.y >= 35 &&
			ptDetect.y <= 66)
		{
			eItem = PLAYER_ITEM_NECKLACE;
		}
		else if(ptDetect.y >= 73 &&
			ptDetect.y <= 104)
		{
			eItem = PLAYER_ITEM_MEDAL;
		}
		else if(ptDetect.y >= 124 &&
			ptDetect.y <= 155)
		{
			eItem = PLAYER_ITEM_BRACELAT2;
		}
		else if(ptDetect.y >= 163 &&
			ptDetect.y <= 194)
		{
			eItem = PLAYER_ITEM_RING2;
		}
	}
	else if(ptDetect.x >= 66 &&
		ptDetect.x <= 104 &&
		ptDetect.y >= 64 &&
		ptDetect.y <= 190)
	{
		eItem = PLAYER_ITEM_CLOTH;
	}
	else if(ptDetect.x >= 72 &&
		ptDetect.x <= 93 &&
		ptDetect.y >= 44 &&
		ptDetect.y <= 56)
	{
		eItem = PLAYER_ITEM_HELMET;
	}
	else if(ptDetect.x >= 17 &&
		ptDetect.x <= 41 &&
		ptDetect.y >= 36 &&
		ptDetect.y <= 114)
	{
		eItem = PLAYER_ITEM_WEAPON;
	}
	else if(ptDetect.y >= 203 &&
		ptDetect.y <= 232)
	{
		if(ptDetect.x >= 5 &&
			ptDetect.x <= 39)
		{
			eItem = PLAYER_ITEM_CHARM;
		}
		else if(ptDetect.x >= 47 &&
			ptDetect.x <= 81)
		{
			eItem = PLAYER_ITEM_BELT;
		}
		else if(ptDetect.x >= 89 &&
			ptDetect.x <= 123)
		{
			eItem = PLAYER_ITEM_SHOE;
		}
		else if(ptDetect.x >= 130 &&
			ptDetect.x <= 164)
		{
			eItem = PLAYER_ITEM_GEM;
		}
	}

	if(eItem != PLAYER_ITEM_TOTAL)
	{
		OnItem(eItem);
	}
	else
	{
		GameOtherItemDisplayDlg::GetInstance()->ShowNothing();
	}

	return true;
}

bool GameOtherHumDlg::OnShowDialog()
{
	SetNeedTopLevel(true);
	GameOtherItemDisplayDlg::GetInstance()->ShowNothing();
	return true;
}

bool GameOtherHumDlg::OnCloseDialog()
{
	GameOtherItemDisplayDlg::GetInstance()->ShowNothing();
	return true;
}

void GameOtherHumDlg::OnItem(PLAYER_ITEM_TYPE _type)
{
	PLAYER_ITEM_TYPE _itemrc = _type;
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	const ItemAttrib* pItem = NULL;
	pItem = GetItem((PLAYER_ITEM_TYPE)(_itemrc));
	bool bCanDress = false;

	/*if(pItem->type != ITEM_NO)
	{
		//	鼠标所在格子不为空
		GameOtherItemDisplayDlg::GetInstance()->ShowItemAttrib(pItem);
	}
	else
	{
		GameOtherItemDisplayDlg::GetInstance()->ShowNothing();
	}*/
	if(pItem->type != ITEM_NO)
	{
		GameDisplayDlg* pDisDlg = GameMainOptUI::GetInstance()->GetDisplayDlg();
		pDisDlg->ShowItemAttrib(pItem, 1);
	}
}


//////////////////////////////////////////////////////////////////////////
#define COLOR_SUITNAME			ARGB(255, 178, 234, 97)
#define COLOR_SUITNAMEITEM_ACTIVE	ARGB(255, 152, 199, 146)
#define COLOR_SUITNAMEITEM_UNACTIVE	ARGB(255, 135, 135, 135)
#define COLOR_EXTRAATRRIB_ACTIVE	ARGB(255, 103, 224, 246)
#define COLOR_EXTRAATTRIB_UNACTIVE	ARGB(255, 135, 135, 135)

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
//////////////////////////////////////////////////////////////////////////
GameOtherItemDisplayDlg::GameOtherItemDisplayDlg()
{
	m_pRender = NEW_EMPTY_SPRITE;
	ShowNothing();

	ZeroMemory(&m_rcClient, sizeof(RECT));
	ZeroMemory(&m_oriItem, sizeof(ItemAttrib));
	m_dwItemNameColor = ARGB_YELLOW;
	m_nColorIndex = 0;
}

GameOtherItemDisplayDlg::~GameOtherItemDisplayDlg()
{
	SAFE_DELETE(m_pRender);
}

void GameOtherItemDisplayDlg::ShowItemAttrib(const ItemAttrib* _pItem)
{
	GameOtherHumDlg* pOtherHumDlg = GameMainOptUI::GetInstance()->GetOtherHumDlg();

	if(m_pItemAttrib == _pItem)
	{
		return;
	}
	if(_pItem == NULL)
	{
		m_pItemAttrib = _pItem;
		return;
	}

	m_pItemAttrib = _pItem;
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
				pPlayerItem = pOtherHumDlg->GetItem((PLAYER_ITEM_TYPE)i);

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
			}

			nValue = (int)bLow - nValue;*/
			int nValue = GetItemUpgrade(wLevel);
			if(nValue >= 0 &&
				nValue <= 8)
			{
				m_dwItemNameColor = g_dwItemColor[nValue];
				m_nColorIndex = nValue;
			}
		}
	}
}

void GameOtherItemDisplayDlg::Render()
{
	if(!m_pItemAttrib)
	{
		return;
	}

	RECT rcDrawRect = m_rcItemRect;
	hgeResourceManager* pResManager = pTheGame->GetGameRes();

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
	tex = pResManager->GetTexture("bmcolor");
	if(tex)
	{
		INITSPRITE(m_pRender, tex);
		SETSPRITEWH(tex, m_pRender, m_rcItemRect.right - m_rcItemRect.left, m_rcItemRect.bottom - m_rcItemRect.top);
		m_pRender->Render(rcDrawRect.left, rcDrawRect.top);
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
	nCurX = POS_LEFT;
	nCurY = POS_TOP;
	nCurY += s_nTitleHeight;
	tex = pTexMgr->GetTexture(m_pItemAttrib->tex);
	if(tex)
	{
		nTexWidth = pTexMgr->GetTextureWidth(m_pItemAttrib->tex);
		nTexHeight = pTexMgr->GetTextureHeight(m_pItemAttrib->tex);
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
		m_pRender->Render(nCurX + s_nItemIconWidth / 2 - nTexWidth / 2, nCurY + s_nItemIconHeight / 2 - nTexHeight / 2);
	}
	//	Item boarder
	nCurX = POS_LEFT;
	nCurY = POS_TOP;
	nCurY += s_nTitleHeight;
	AfxGetHge()->Gfx_RenderLine(nCurX, nCurY, nCurX + s_nItemIconWidth, nCurY, ARGB(255, 69, 89, 137));
	AfxGetHge()->Gfx_RenderLine(nCurX, nCurY + s_nItemIconHeight, nCurX + s_nItemIconWidth, nCurY + s_nItemIconHeight, ARGB(255, 69, 89, 137));
	AfxGetHge()->Gfx_RenderLine(nCurX, nCurY, nCurX, nCurY + s_nItemIconHeight, ARGB(255, 69, 89, 137));
	AfxGetHge()->Gfx_RenderLine(nCurX + s_nItemIconWidth, nCurY, nCurX + s_nItemIconWidth, nCurY + s_nItemIconHeight, ARGB(255, 69, 89, 137));

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
		
		/*DWORD dwErr = 0;
		if(!GamePlayer::GetInstance()->CanEquip(m_pItemAttrib, &dwErr))
		{
			AfxGetPrinter()->SetColor(ARGB_RED);
		}*/

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
			m_stSuitAttribRenderInfo.bCanShow &&
			!GamePlayer::GetInstance()->IsSuitIgnored(m_pItemAttrib->atkPalsy))
		{
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
}