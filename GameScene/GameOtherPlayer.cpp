#include "GameOtherPlayer.h"
#include "GamePlayer.h"
#include "GameResourceManager.h"
#include <math.h>
#include "../BackMir/BackMir.h"
#include "../Common/gfx_utils.h"
#include "../GameScene/MagicSystem.h"
#include "../GameDialog/GameChatDlg.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameDialog/GameInteractiveDlg.h"
#include "../GameDialog/GameOtherHumDlg.h"
#include "../../CommonModule/ShareData.h"
#include "../GameDialog/GameHelpDlg.h"
#include "../GameDialog/GameQuestDlg.h"
#include "../GameDialog/GameStoreDlg2.h"
#include "../GameDialog/OfflineSellDlg.h"
#include "../GameDialog/GameDisplayDlg.h"
#include "../GameDialog/GameRankDlg.h"
#include "../GameDialog/OlShopDlg.h"
#include "../GameDialog/GameStoveDlg.h"
#include "../GameDialog/GameBigStoreDlg.h"
#include "../GameDialog/GameSkillDlg.h"
#include "../../CommonModule/version.h"
#include <zlib.h>
#include "../../CommonModule/cJSON.h"
//////////////////////////////////////////////////////////////////////////
GameOtherPlayer::GameOtherPlayer()
{
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	ZeroMemory(m_equip, sizeof(m_equip));
	m_type = OBJ_OTHERPLAYER;
	m_fDetect = 0.0f;
	m_fMoveOffsetXTotal = 0.0f;
	m_fMoveOffsetYTotal = 0.0f;
	m_fLastUpdateSkill = 0.0f;
	m_fLastUpdateAttack = 0.0f;
	m_fLastUpdateDeath = 0.0f;
	m_fLastUpdateAttackNoWeapon = 0.0f;
	m_fLastUpdateAttackStop = 0.0f;
	m_fLastUpdateAttacked = 0.0f;
	m_fLastUpdateMove = 0.0f;

	m_dwLastAttackTime = 0;
	m_dwLastAttackStopTime = 0;

	m_dwLastAttackMode = 0;

	for(int i = 0; i < MAGICEFFECT_TOTAL; ++i)
	{
		m_eEffect[i] = ME_NONE;
	}
	m_pEffectRender = new hgeSprite(0, 0, 0, 0, 0);
	m_pEffectRender->SetBlendMode(BLEND_ALPHAADD | BLEND_NOZWRITE);

	ZeroMemory(m_dwHumEffectTime, sizeof(m_dwHumEffectTime));
	m_dwHumEffectFlag = 0;
	m_pMagicEffect = NULL;

	m_bPlayOneStep = m_bPlayTwoStep = false;

	m_dwJinGangExpireTime = 0;
	m_nLieHuoSkillLevel = 0;
	m_nVipLevel = 0;
	m_dwLastNameColorChangeTime = 0;
	m_nLastNameColorIndex = 0;

	ZeroMemory(&m_stExtAttrib, sizeof(m_stExtAttrib));

	m_bInRider = 0;
}

GameOtherPlayer::~GameOtherPlayer()
{
	delete m_pRender;
	m_pRender = NULL;
	ClearHumState();
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::RenderHum()
{
	GameTextureManager* ttexs = NULL;
	HGE* hge = AfxGetHge();
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nRenderX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nRenderY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

	if(GetType() == OBJ_PLAYER)
	{
		nRenderX = CENTRAL_X;
		nRenderY = CENTRAL_Y;
	}

	int nResIndex = 0;
	m_nCurrentTextureIndex = CalTextureIndex(NULL, &nResIndex) + m_bCurFrame;

	ttexs = GameResourceManager::GetInstance()->GetTexs(RES_HUM + nResIndex);
	if(!ttexs)
	{
		hge->System_Log("没有载入人物资源");
	}

	HTEXTURE tex = ttexs->GetTexture(m_nCurrentTextureIndex);
	short sOffsetX = 0;
	short sOffsetY = 0;
	if(tex)
	{
		if(GameScene::sThis->GetTarget() == this &&
			GetType() == OBJ_OTHERPLAYER)
		{
			HTEXTURE texSelected = SelectedTextureManager::GetInstance()->GetSelectedTextureHum(tex, nResIndex, m_nCurrentTextureIndex);
			if(texSelected != NULL)
			{
				tex = texSelected;
			}
		}

		if(!m_pRender)
		{
			//m_pSprHum = new hgeSprite(tex, 0, 0, ttexs->GetTextureWidth(tex), ttexs->GetTextureHeight(tex));
			m_pRender = new hgeSprite(tex, 0, 0, ttexs->GetTextureWidth(m_nCurrentTextureIndex),
				ttexs->GetTextureHeight(m_nCurrentTextureIndex));
		}
		else
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, ttexs->GetTextureWidth(m_nCurrentTextureIndex), 
				ttexs->GetTextureHeight(m_nCurrentTextureIndex));
		}
		sOffsetX = ttexs->GetTextureOffsetX(m_nCurrentTextureIndex);
		sOffsetY = ttexs->GetTextureOffsetY(m_nCurrentTextureIndex);
		m_pRender->Render(nRenderX + sOffsetX, nRenderY + sOffsetY);
	}
}

void GameOtherPlayer::RenderWeapon()
{
	GameTextureManager* ttexs = NULL;
	HGE* hge = AfxGetHge();
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nRenderX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nRenderY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

	if(GetType() == OBJ_PLAYER)
	{
		nRenderX = CENTRAL_X;
		nRenderY = CENTRAL_Y;
	}

	int nResIndex = 0;
	int nWeaponIndex = CalWeaponTexIndex(NULL, &nResIndex) + m_bCurFrame;

	ttexs = GameResourceManager::GetInstance()->GetTexs(RES_WEAPON + nResIndex);
	//	画手上武器
	//if(!m_texs[PLAYER_RES_WEAPON])
	if(!ttexs)
	{
		return;
	}

	HTEXTURE tex = ttexs->GetTexture(nWeaponIndex);
	short sOffsetX = 0;
	short sOffsetY = 0;
	if(tex)
	{
		if(!m_pRender)
		{
			m_pRender = new hgeSprite(tex, 0, 0, ttexs->GetTextureWidth(nWeaponIndex),
				ttexs->GetTextureHeight(nWeaponIndex));
		}
		else
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, ttexs->GetTextureWidth(nWeaponIndex),
				ttexs->GetTextureHeight(nWeaponIndex));
		}
		sOffsetX = ttexs->GetTextureOffsetX(nWeaponIndex);
		sOffsetY = ttexs->GetTextureOffsetY(nWeaponIndex);
		m_pRender->Render(nRenderX + sOffsetX, nRenderY + sOffsetY);
	}

	if (GetRenderMode() == ORM_TRANSPARENT)
	{
		ItemAttrib* pWeapon = GetPlayerItem(PLAYER_ITEM_WEAPON);
		if (NULL != pWeapon &&
			GetItemUpgrade(pWeapon->level) > 5)
		{
			ttexs = GameResourceManager::GetInstance()->GetTexs(RES_WEAPONEFFECT);
			if (NULL == ttexs)
			{
				return;
			}
			int nWeaponEffectIndex = nWeaponIndex;
			if (nResIndex == 1)
			{
				if (pWeapon->id >= 1405 &&
					pWeapon->id <= 1407)
				{
					nWeaponEffectIndex += (46800 - 1200);
				}
				else if (pWeapon->id == 1410)
				{

				}
			}

			tex = ttexs->GetTexture(nWeaponEffectIndex);
			if (tex)
			{
				MagicElement::pEffectRender->SetTexture(tex);
				MagicElement::pEffectRender->SetTextureRect(0, 0, ttexs->GetTextureWidth(nWeaponEffectIndex),
					ttexs->GetTextureHeight(nWeaponEffectIndex));
				sOffsetX = ttexs->GetTextureOffsetX(nWeaponEffectIndex);
				sOffsetY = ttexs->GetTextureOffsetY(nWeaponEffectIndex);
				MagicElement::pEffectRender->Render(nRenderX + sOffsetX, nRenderY + sOffsetY);
			}
		}
	}
}

void GameOtherPlayer::RenderHair()
{
	GameTextureManager* ttexs = NULL;
	HGE* hge = AfxGetHge();
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nRenderX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nRenderY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

	if(GetType() == OBJ_PLAYER)
	{
		nRenderX = CENTRAL_X;
		nRenderY = CENTRAL_Y;
	}

	int nHairIndex = CalHairTexIndex() + m_bCurFrame;
	int nHairResIndex = CalHairResIndex();

	ttexs = GameResourceManager::GetInstance()->GetTexs(nHairResIndex);
	if(!ttexs)
	{
		return;
	}


	HTEXTURE tex = ttexs->GetTexture(nHairIndex);
	short sOffsetX = 0;
	short sOffsetY = 0;
	if(tex)
	{
		if(!m_pRender)
		{
			m_pRender = new hgeSprite(tex, 0, 0, ttexs->GetTextureWidth(nHairIndex), 
				ttexs->GetTextureHeight(nHairIndex));
		}
		else
		{
			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, ttexs->GetTextureWidth(nHairIndex),
				ttexs->GetTextureHeight(nHairIndex));
		}
		sOffsetX = ttexs->GetTextureOffsetX(nHairIndex);
		sOffsetY = ttexs->GetTextureOffsetY(nHairIndex);
		m_pRender->Render(nRenderX + sOffsetX, nRenderY + sOffsetY);
	}
}

void GameOtherPlayer::RenderWing()
{
	GameTextureManager* ttexs = NULL;
	HGE* hge = AfxGetHge();
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nRenderX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nRenderY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

	if(GetType() == OBJ_PLAYER)
	{
		nRenderX = CENTRAL_X;
		nRenderY = CENTRAL_Y;
	}

	//	画翅膀
	DWORD dwWingResIndex = CalCustomWingIndexAndRes();

	if(0xffffffff == dwWingResIndex)
	{
		return;
	}

	int nRes = LOWORD(dwWingResIndex);
	int nTextIndex = HIWORD(dwWingResIndex);

	ttexs = GameResourceManager::GetInstance()->GetTexs(nRes);
	if(ttexs &&
		/*!m_bTransparentMode &&*/
		GetRenderMode() == ORM_TRANSPARENT)
	{
		if(nTextIndex != -1)
		{
			nTextIndex += m_bCurFrame;
			HTEXTURE tex = ttexs->GetTexture(nTextIndex);
			short sOffsetX = 0;
			short sOffsetY = 0;
			if(tex)
			{
				MagicElement::pEffectRender->SetTexture(tex);
				MagicElement::pEffectRender->SetTextureRect(0, 0, ttexs->GetTextureWidth(nTextIndex), 
					ttexs->GetTextureHeight(nTextIndex));
				sOffsetX = ttexs->GetTextureOffsetX(nTextIndex);
				sOffsetY = ttexs->GetTextureOffsetY(nTextIndex);
				MagicElement::pEffectRender->Render(nRenderX + sOffsetX, nRenderY + sOffsetY);
			}	
		}
	}
}

static DWORD s_dwChangeNameColor[] = {
	ARGB_WHITE,
	0xffFFBBFF,
	0xffFF00FF,
	ARGB_RED,
	ARGB_YELLOW,
	ARGB_GREEN,
	0xffCAFF70,
	0xffBF3EFF,
	0xff9400D3,
	0xff7CFC00,
	0xff1E90FF
};

void GameOtherPlayer::RenderName()
{
	if(!IsVisible())
	{
		return;
	}
	//	计算中值坐标
	int nDrawX = 0;
	int nDrawY = 0;
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	nDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
	//int nTextWidth = 12 * strlen(m_attrib.name);
	int nTextWidth = GetTextWidth(m_attrib.name, 12);

	if(0 == nTextWidth)
	{
		return;
	}
	nDrawY -= UNIT_HEIGHT / 2;
	nDrawX += UNIT_WIDTH / 2;
	nDrawX -= nTextWidth / 2;

	int nBaseDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx() + UNIT_WIDTH / 2;
	int nBaseDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy() - UNIT_HEIGHT / 2;

	DWORD dwNameColor = ARGB_GREEN;
	int nVipLevel = GetVipLevel();

#ifdef _DEBUG
	//nVipLevel = 1;
#endif

	if(0 == nVipLevel)
	{
		//	不是会员
		if(GetType() == OBJ_OTHERPLAYER)
		{
			dwNameColor = ARGB_WHITE;
		}
	}
	else
	{
		DWORD dwTick = GetTickCount();
		if(dwTick - m_dwLastNameColorChangeTime > 500)
		{
			m_dwLastNameColorChangeTime = dwTick;

			//	change name color index
			m_nLastNameColorIndex++;
			if(m_nLastNameColorIndex >= sizeof(s_dwChangeNameColor) / sizeof(s_dwChangeNameColor[0]))
			{
				m_nLastNameColorIndex = 0;
			}
		}

		dwNameColor = s_dwChangeNameColor[m_nLastNameColorIndex];
	}

	//	先画名字框
	bool bRenderNameFrame = true;
	if(GetType() == OBJ_PLAYER)
	{
		bRenderNameFrame = !pTheGame->Config_DisableRenderNameFrame();
	}
	if(bRenderNameFrame &&
		0 != m_stExtAttrib.uNameFrame)
	{
		m_pRender->SetColor(ARGB(192, 255, 255, 255));
		Gfx_RenderNameFrame(m_pRender, m_stExtAttrib.uNameFrame, nDrawX, nDrawY, nTextWidth);
		m_pRender->SetColor(ARGB_WHITE);
	}

	AfxGetPrinter()->SetColor(dwNameColor);
	AfxGetPrinter()->Print(nDrawX, nDrawY, m_attrib.name);

	//	画游戏捐助者
	if(0 != nVipLevel &&
		GetType() == OBJ_OTHERPLAYER)
	{
		char szDonateInfo[MAX_PATH] = {0};
		sprintf(szDonateInfo, "捐赠者L%d", nVipLevel);

		nTextWidth = GetTextWidth(szDonateInfo, 12);
		nDrawX = nBaseDrawX - nTextWidth / 2;
		nDrawY = nBaseDrawY + 13;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(nDrawX, nDrawY, szDonateInfo);
	}
}

void GameOtherPlayer::RenderSaying()
{
	if(AfxGetHge()->Timer_GetTime() - m_fSayingTime > 8.0f)
	{
		ResetSaying();
	}

	if(m_szSaying[0] == 0 ||
		m_fSayingTime == 0.0f)
	{
		return;
	}
	if(m_nCurrentTextureIndex == INVALID_TEXTURE_INDEX)
	{
		return;
	}

	int nDrawX = 0;
	int nDrawY = 0;
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx() + UNIT_WIDTH / 2;
	if(m_stSayingSize.cx > OBJECT_CHAT_EACHLINE)
	{
		nDrawX -= OBJECT_CHAT_EACHLINE / 2;
	}
	else
	{
		nDrawX -= m_stSayingSize.cx / 2;
	}
	nDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy() - 2 * UNIT_HEIGHT - UNIT_HEIGHT - 12 * m_dwLineNumber;

	//	render bk
	int nChatFrame = m_stExtAttrib.uChatFrame;
	if(0 == nChatFrame)
	{
		nChatFrame = 310;
	}
	/*static int s_nChatFrame = 31;
	if(AfxGetHge()->Input_KeyUp(HGEK_UP))
	{
		++s_nChatFrame;
	}
	else if(AfxGetHge()->Input_KeyUp(HGEK_DOWN))
	{
		--s_nChatFrame;
	}
	nChatFrame = s_nChatFrame * 10;*/

	bool bStroke = true;
	bool bRenderChatFrame = !pTheGame->Config_DisableRenderChatFrame();
	DWORD dwColor = GetSayingColor();
	if(0 == dwColor)
	{
		dwColor = ARGB_WHITE;
		//dwColor = ARGB_BLACK;
		if(nChatFrame == 310)
		{
			bStroke = false;
			dwColor = ARGB_BLACK;
		}
	}

	//	auto switch
	static DWORD s_dwLastSwitchChatFrameTime = GetTickCount();
	static int s_nChatFrameIndex = 0;
	if(nChatFrame == 530)
	{
		if(GetTickCount() - s_dwLastSwitchChatFrameTime > 500)
		{
			//	switch
			++s_nChatFrameIndex;
			if(s_nChatFrameIndex > 1)
			{
				s_nChatFrameIndex = 0;
			}

			s_dwLastSwitchChatFrameTime = GetTickCount();
		}

		nChatFrame += s_nChatFrameIndex * 10;
	}

	if(bRenderChatFrame)
	{
		m_pRender->SetColor(ARGB_WHITE);
		Gfx_RenderChatBack(m_pRender, nChatFrame, 10,
			CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx() + UNIT_WIDTH / 2 - OBJECT_CHAT_EACHLINE / 2,
			nDrawY,
			OBJECT_CHAT_EACHLINE, 13 * m_dwLineNumber);
	}

	AfxGetPrinter()->SetColor(dwColor);
	float fPreKerning = AfxGetPrinter()->GetKerningHeight();
	AfxGetPrinter()->SetKerningHeight(1.0f);
	AfxGetPrinter()->LineFeedRender((float)nDrawX, (float)nDrawY, m_szSaying, OBJECT_CHAT_EACHLINE, bStroke);
	AfxGetPrinter()->SetKerningHeight(fPreKerning);
}

void GameOtherPlayer::Render()
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nRenderX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nRenderY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

	HGE* hge = AfxGetHge();

	GameTextureManager* ttexs = NULL;
	//DWORD dwPreColor = m_pRender->GetColor();

	switch (GetDirection())
	{
	case PDT_LEFT:
	case PDT_LEFTDOWN:
	case PDT_LEFTUP:
		{
			RenderWeapon();
			RenderHum();
			RenderHair();
			RenderWing();
		}break;
	default:
		{
			//	反着
			bool bWingRendered = false;
			if(GetDirection() == PDT_DOWN ||
				GetDirection() == PDT_RIGHTDOWN ||
				GetDirection() == PDT_RIGHT ||
				GetDirection() == PDT_LEFTUP)
			{
				//	画翅膀
				RenderWing();
				bWingRendered = true;
			}
			RenderHum();
			RenderHair();
			RenderWeapon();

			if(!bWingRendered)
			{
				//	画翅膀
				RenderWing();
			}
		}break;
	}	

	//m_pRender->SetColor(dwPreColor);
	//RenderMagic();
	//RenderSelfEffect();
}

//////////////////////////////////////////////////////////////////////////
bool GameOtherPlayer::IsLastFrame()
{
	int nTotal = 0;
	CalTextureIndex(&nTotal);
	return nTotal - 1 == GetCurFrame();
}

void GameOtherPlayer::UpdateMove()
{
	PLAYER_DIRECTION pd = GetDirection();
	DWORD dwMoveCostTime = NORMAL_WALK_COST_TIME - m_attrib.moveSpeed * 50;

	if(GetStatus() == PST_WALK)
	{
		dwMoveCostTime *= 1.2;
	}

	if(!IsMoving())
	{
		return;
	}

	static const float s_fWalkDistance[] = {0, -32.0f,
	48.0f, -32.0f,
	48.0f, 0,
	48.0f, 32.0f,
	0, 32.0f,
	-48.0f, 32.0f,
	-48.0f, 0,
	-48.0f, -32.0f};

	DWORD dwNowTime = GetTickCount();
	DWORD dwMovedTime = dwNowTime - m_dwLastMoveTime;
	float fMovedTime = (float)dwMovedTime / 1000;
	float fMovedRadio = fMovedTime / ((float)dwMoveCostTime / 1000);

	if(fMovedRadio >= 0.98f)
	{
		SetReallyCoord(GetCoordX(), GetCoordY());
		SetCurFrame(5);
	}
	else
	{
		if(fMovedRadio >= 0.99f)
		{
			__asm int 3
		}

		if(fMovedRadio > 0.8f)
		{
			SetCurFrame(5);
		}
		else if(fMovedRadio > 0.64f)
		{
			SetCurFrame(4);
		}
		else if(fMovedRadio > 0.48f)
		{
			SetCurFrame(3);
		}
		else if(fMovedRadio > 0.32f)
		{
			SetCurFrame(2);
		}
		else if(fMovedRadio > 0.16f)
		{
			SetCurFrame(1);
		}
		else
		{
			SetCurFrame(0);
		}
		int nMoveRadio = 1;
		if(GetStatus() == PST_RUN)
		{
			nMoveRadio = 2;
		}

		int nPreCoordX = GetCoordX() - g_nMoveOftTable[pd * 2] * nMoveRadio;
		int nPreCoordY = GetCoordY() - g_nMoveOftTable[pd * 2 + 1] * nMoveRadio;

		SetObjx(nPreCoordX * UNIT_WIDTH + s_fWalkDistance[pd * 2] * nMoveRadio * fMovedRadio);
		SetObjy(nPreCoordY * UNIT_HEIGHT + s_fWalkDistance[pd * 2 + 1] * nMoveRadio * fMovedRadio);
	}
}

void GameOtherPlayer::Update(float _dt)
{
	GameOtherPlayer * m_pPlayer = this;
	PLAYER_DIRECTION pd;
	float dt = _dt;
	int nTotalFrame = 0;
	int nMoveOffset = 0;
	float fMoveOffsetX = 0.0f;
	float fMoveOffsetY = 0.0f;
	int nPreFrame = 0;

	SelfEffectUpdate();

	if(GetStatus() == PST_DEAD &&
		IsLastFrame())
	{
		return;
	}

	//	技能update
	float fAtkInterval = GetAttackInterval();
	fAtkInterval = 0.1f;
	//static float fLastUpdateSk = 0.0f;
	if(GetStatus() == PST_SK_LEIDIANSHU &&
		!IsLastFrame())
	{
		m_fLastUpdateSkill += _dt;
		if(m_fLastUpdateSkill > /*0.1f*/fAtkInterval)
		{
			m_pPlayer->SetCurFrame(m_pPlayer->GetCurFrame() + 1);
			m_fLastUpdateSkill = 0.0f;
		}
	}
	else if(GetStatus() == PST_SK_LEIDIANSHU &&
		IsLastFrame())
	{
		m_fLastUpdateSkill += _dt;
		if(m_fLastUpdateSkill > /*0.2f*/fAtkInterval)
		{
			//SetStatus(PST_STAND);
			SetStatus(PST_ATTACKSTOP);
			SetCurFrame(0);
			m_fLastUpdateSkill = 0.0f;
		}
	}

	//	攻击update
	//static float fLastUpdateAttackTime = 0.0f;
	//float fAtkInterval = GetAttackInterval();
	if(m_pPlayer->GetStatus() == PST_ATTACKWEAPON &&
		!IsLastFrame())
	{
		m_fLastUpdateAttack += _dt;
		if(m_fLastUpdateAttack > /*(1.0f / ((float)m_attrib.atkSpeed + 8.0f))*/fAtkInterval)
		{
			m_pPlayer->SetCurFrame(m_pPlayer->GetCurFrame() + 1);
			m_fLastUpdateAttack = 0.0f;

			nPreFrame = GetCurFrame();
			if(nPreFrame == 3)
			{
				PlayAttackSound();
			}
		}
	}
	if(m_pPlayer->GetStatus() == PST_ATTACKWEAPON &&
		IsLastFrame())
	{
		m_fLastUpdateAttack += _dt;
		if(m_fLastUpdateAttack > /*(1.0f / ((float)m_attrib.atkSpeed + 8.0f)) + 0.2f*/fAtkInterval)
		{
			m_fLastUpdateAttack = 0.0f;
			//SetStatus(PST_STAND);
			SetStatus(PST_ATTACKSTOP);
			SetCurFrame(0);
			if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_SLIEHUO))
			{
				//
				MagicElement* pMagic = new MagicSuperFire;
				if(pMagic->Create(MEFF_SLIEHUO, this, this, GetObjx(), GetObjy(), GetDirection()))
				{
					pMagic->SetActive();
					GameScene::sThis->InsertNewMagic(pMagic);
				}
				else
				{
					SAFE_DELETE(pMagic);
				}
			}
			if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_KTSWORD))
			{
				{
					//
					MagicElement* pMagic = new MagicSuperFire;
					if(pMagic->Create(MEFF_KTSWORD, this, this, GetObjx(), GetObjy(), GetDirection()))
					{
						pMagic->SetActive();
						GameScene::sThis->InsertNewMagic(pMagic);
					}
					else
					{
						SAFE_DELETE(pMagic);
					}
				}
			}
			ClearHitEffect();
		}
	}

	//	死亡update
	if(m_pPlayer->GetStatus() == PST_DEAD &&
		!IsLastFrame())
	{
		//static float fLastUpdateDead = 0.0f;
		m_fLastUpdateDeath += _dt;
		if(m_fLastUpdateDeath > 0.2f)
		{
			m_pPlayer->SetCurFrame(m_pPlayer->GetCurFrame() + 1);
			m_fLastUpdateDeath = 0.0f;
		}
	}
	else if(m_pPlayer->GetStatus() == PST_DEAD &&
		IsLastFrame())
	{
		//	死亡处理
	}


	//	空手攻击update
	//static float fLastUpdateNoWeapon = 0.0f;
	if(m_pPlayer->GetStatus() == PST_ATTACKNOWEAPON &&
		!IsLastFrame())
	{
		m_fLastUpdateAttackNoWeapon += _dt;
		if(m_fLastUpdateAttackNoWeapon > /*(1.0f / ((float)m_attrib.atkSpeed + 10.0f))*/fAtkInterval)
		{
			m_pPlayer->SetCurFrame(m_pPlayer->GetCurFrame() + 1);
			m_fLastUpdateAttackNoWeapon = 0.0f;
			if(m_pPlayer->IsLastFrame())
			{

			}

			nPreFrame = GetCurFrame();
			if(nPreFrame == 3)
			{
				PlayAttackSound();
			}
		}
	}
	else if(m_pPlayer->GetStatus() == PST_ATTACKNOWEAPON &&
		IsLastFrame())
	{
		m_fLastUpdateAttackNoWeapon += _dt;
		if(m_fLastUpdateAttackNoWeapon > /*(1.0f / ((float)m_attrib.atkSpeed + 10.0f)) + 0.2f*/fAtkInterval)
		{
			//SetCurFrame(PST_STAND);
			SetStatus(PST_ATTACKSTOP);
			SetCurFrame(0);
			m_fLastUpdateAttackNoWeapon = 0.0f;
			if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_SLIEHUO))
			{
				//
				MagicElement* pMagic = new MagicSuperFire;
				if(pMagic->Create(MEFF_SLIEHUO, this, this, GetObjx(), GetObjy(), GetDirection()))
				{
					pMagic->SetActive();
					GameScene::sThis->InsertNewMagic(pMagic);
				}
				else
				{
					SAFE_DELETE(pMagic);
				}
			}
			if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_KTSWORD))
			{
				{
					//
					MagicElement* pMagic = new MagicSuperFire;
					if(pMagic->Create(MEFF_KTSWORD, this, this, GetObjx(), GetObjy(), GetDirection()))
					{
						pMagic->SetActive();
						GameScene::sThis->InsertNewMagic(pMagic);
					}
					else
					{
						SAFE_DELETE(pMagic);
					}
				}
			}
			ClearHitEffect();
		}
	}

	//	攻击停止状态update
	//static float fLastUpdateStop = 0.0f;
	if(GetStatus() == PST_ATTACKSTOP &&
		!IsLastFrame())
	{
		m_fLastUpdateAttackStop += dt;
		if(m_fLastUpdateAttackStop >/* 0.1f*//*fAtkInterval + 0.2f*/3.0f)
		{
			SetCurFrame(GetCurFrame() + 1);
			m_fLastUpdateAttackStop = 0.0f;
		}
	}
	else if(GetStatus() == PST_ATTACKSTOP &&
		IsLastFrame())
	{
		m_fLastUpdateAttackStop += _dt;
		if(m_fLastUpdateAttackStop >/* 0.1f*//*fAtkInterval + 0.2f*/3.0f)
		{
			/*if(m_fLastUpdateAttackStop < 3.0f)
			{
				SetStatus(PST_ATTACKSTOP);
			}
			else*/
			{
				SetStatus(PST_STAND);
			}
			
			m_fLastUpdateAttackStop = 0.0f;
			SetCurFrame(0);
		}
	}


	//	受伤update
	//static float fLastUpdateAtked = 0.0f;
	if(GetStatus() == PST_ATTACKED &&
		!IsLastFrame())
	{
		m_fLastUpdateAttacked += dt;
		if(m_fLastUpdateAttacked > 0.08f)
		{
			SetCurFrame(GetCurFrame() + 1);
			m_fLastUpdateAttacked = 0.0f;
		}
	}
	else if(GetStatus() == PST_ATTACKED &&
		IsLastFrame())
	{
		m_fLastUpdateAttacked += _dt;
		if(m_fLastUpdateAttacked > 0.08f)
		{
			//if(m_fLastUpdateAttackStop < 3.0f)
			SetStatus(PST_STAND);
			m_fLastUpdateAttacked = 0.0f;
		}
	}

	/*
	if(m_pPlayer->GetObjx() != m_pPlayer->GetCoordX() * UNIT_WIDTH ||
	m_pPlayer->GetObjy() != m_pPlayer->GetCoordY() * UNIT_HEIGHT)*/
	if(IsMoving())	
	{
		static const int nWalkRunTotalFrames = 6;
		pd = m_pPlayer->GetDirection();
		int nCurFrame = 0;
		float fMovePerSecond = GetMovePerSecond();

		switch (pd)
		{
		case PDT_RIGHT:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*fMoveOffsetX = dt * m_pPlayer->GetSpeed();*/
					fMoveOffsetX = dt * fMovePerSecond;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_fMoveOffsetXTotal += fMoveOffsetX; 
					//	切换跑动帧
					//m_pPlayer->CalTextureIndex(&nTotalFrame);
					nCurFrame = (int)m_fMoveOffsetXTotal / 16;
					if(nCurFrame >= nWalkRunTotalFrames)
					{
						nCurFrame = nWalkRunTotalFrames - 1;
					}
					m_pPlayer->SetCurFrame(nCurFrame);

					if(m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetObjx(m_pPlayer->GetCoordX() * UNIT_WIDTH);
						m_pPlayer->SetCurFrame(nWalkRunTotalFrames - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*fMoveOffsetX = dt * m_pPlayer->GetSpeed() * 0.5f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.5f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_fMoveOffsetXTotal += fMoveOffsetX; 
					//	切换跑动帧
					//m_pPlayer->CalTextureIndex(&nTotalFrame);
					//m_pPlayer->SetCurFrame(m_fMoveOffsetXTotal / (48 /nTotalFrame));
					nCurFrame = (int)m_fMoveOffsetXTotal / (48 / 6);
					if(nCurFrame >= 6)
					{
						nCurFrame = 5;
					}
					m_pPlayer->SetCurFrame(nCurFrame);

					if(m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetObjx(m_pPlayer->GetCoordX() * UNIT_WIDTH);
						m_pPlayer->SetCurFrame(6 - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_LEFT:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*fMoveOffsetX = - dt * m_pPlayer->GetSpeed();*/
					fMoveOffsetX = - dt * fMovePerSecond;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_fMoveOffsetXTotal += fMoveOffsetX; 
					//	切换跑动帧
					//m_pPlayer->CalTextureIndex(&nTotalFrame);
					nCurFrame = (int)(-m_fMoveOffsetXTotal) / (96 / 6);
					if(nCurFrame >= 6)
					{
						nCurFrame = 5;
					}
					//m_pPlayer->SetCurFrame(abs(m_fMoveOffsetXTotal / (96 /nTotalFrame)));
					m_pPlayer->SetCurFrame(nCurFrame);

					if(m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetObjx(m_pPlayer->GetCoordX() * UNIT_WIDTH);
						m_pPlayer->SetCurFrame(6 - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*fMoveOffsetX = -dt * m_pPlayer->GetSpeed() * 0.5f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.5f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_fMoveOffsetXTotal += fMoveOffsetX; 
					//	切换跑动帧
					//m_pPlayer->CalTextureIndex(&nTotalFrame);
					//m_pPlayer->SetCurFrame(abs(m_fMoveOffsetXTotal / (48 /nTotalFrame)));
					nCurFrame = (int)(-m_fMoveOffsetXTotal) / (48 / 6);
					if(nCurFrame >= 6)
					{
						nCurFrame = 5;
					}
					m_pPlayer->SetCurFrame(nCurFrame);

					if(m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetObjx(m_pPlayer->GetCoordX() * UNIT_WIDTH);
						m_pPlayer->SetCurFrame(6 - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_DOWN:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.7f;*/
					fMoveOffsetY = dt * fMovePerSecond * 0.7f;
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					//m_pPlayer->CalTextureIndex(&nTotalFrame);
					//m_pPlayer->SetCurFrame((abs(m_fMoveOffsetYTotal) / (64 /nTotalFrame)) > 5 ? 5 : (abs(m_fMoveOffsetYTotal) / (64 /nTotalFrame)));
					nCurFrame = (int)m_fMoveOffsetYTotal / (64 / 6);
					if(nCurFrame > 5)
					{
						nCurFrame = 5;
					}
					m_pPlayer->SetCurFrame(nCurFrame);

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT)
					{
						m_pPlayer->SetObjy(m_pPlayer->GetCoordY() * UNIT_HEIGHT);
						m_pPlayer->SetCurFrame(6 - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.5f * 0.7f;*/
					fMoveOffsetY = dt * fMovePerSecond * 0.5f * 0.7f;
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					//m_pPlayer->CalTextureIndex(&nTotalFrame);
					//m_pPlayer->SetCurFrame((abs(m_fMoveOffsetYTotal) / (32 /nTotalFrame)) > 5 ? 5 :(abs(m_fMoveOffsetYTotal) / (32 /nTotalFrame)) );
					nCurFrame = (int)m_fMoveOffsetYTotal / (32 / 6);
					if(nCurFrame > 5)
					{
						nCurFrame = 5;
					}
					m_pPlayer->SetCurFrame(nCurFrame);

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT)
					{
						m_pPlayer->SetObjy(m_pPlayer->GetCoordY() * UNIT_HEIGHT);
						m_pPlayer->SetCurFrame(6 - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_UP:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*fMoveOffsetY = - dt * m_pPlayer->GetSpeed() * 0.7f;*/
					fMoveOffsetY = - dt * fMovePerSecond * 0.7f;
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					//m_pPlayer->CalTextureIndex(&nTotalFrame);
					//m_pPlayer->SetCurFrame((abs(m_fMoveOffsetYTotal) / (64 /nTotalFrame)) > 5 ? 5 : (abs(m_fMoveOffsetYTotal) / (64 /nTotalFrame)));
					nCurFrame = (int)(-m_fMoveOffsetYTotal) / (64 / 6);
					if(nCurFrame > 5)
					{
						nCurFrame = 5;
					}
					m_pPlayer->SetCurFrame(nCurFrame);

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT)
					{
						m_pPlayer->SetObjy(m_pPlayer->GetCoordY() * UNIT_HEIGHT);
						m_pPlayer->SetCurFrame(6 - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*fMoveOffsetY = -dt * m_pPlayer->GetSpeed() * 0.5f * 0.7f;*/
					fMoveOffsetY = -dt * fMovePerSecond * 0.5f * 0.7f;
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					//m_pPlayer->CalTextureIndex(&nTotalFrame);
					//m_pPlayer->SetCurFrame((abs(m_fMoveOffsetYTotal) / (32 /nTotalFrame)) > 5 ? 5 : (abs(m_fMoveOffsetYTotal) / (32 /nTotalFrame)));
					nCurFrame = (int)(-m_fMoveOffsetYTotal) / (32 / 6);
					if(nCurFrame > 5)
					{
						nCurFrame = 5;
					}
					m_pPlayer->SetCurFrame(nCurFrame);

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT)
					{
						m_pPlayer->SetObjy(m_pPlayer->GetCoordY() * UNIT_HEIGHT);
						m_pPlayer->SetCurFrame(6 - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_RIGHTUP:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*
					fMoveOffsetX = dt * m_pPlayer->GetSpeed() * 0.6f * 1.4f;
										fMoveOffsetY = - dt * m_pPlayer->GetSpeed() * 0.4f * 1.4f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.6f * 1.4f;
					fMoveOffsetY = - dt * fMovePerSecond * 0.4f * 1.4f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)));

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = dt * m_pPlayer->GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = - dt * m_pPlayer->GetSpeed() * 0.4f * 0.7f;*/
					
					fMoveOffsetX = dt * fMovePerSecond * 0.6f * 0.7f;
					fMoveOffsetY = - dt * fMovePerSecond * 0.4f * 0.7f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_RIGHTDOWN:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*
					fMoveOffsetX = dt * m_pPlayer->GetSpeed() * 0.6f * 1.4f;
										fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.4f * 1.4f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.6f * 1.4f;
					fMoveOffsetY = dt * fMovePerSecond * 0.4f * 1.4f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)));

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = dt * m_pPlayer->GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.6f * 0.7f;
					fMoveOffsetY = dt * fMovePerSecond * 0.4f * 0.7f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_LEFTUP:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*
					fMoveOffsetX = -dt * m_pPlayer->GetSpeed() * 0.6f * 1.4f;
										fMoveOffsetY = -dt * m_pPlayer->GetSpeed() * 0.4f * 1.4f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.6f * 1.4f;
					fMoveOffsetY = -dt * fMovePerSecond * 0.4f * 1.4f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)));

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = -dt * m_pPlayer->GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = -dt * m_pPlayer->GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.6f * 0.7f;
					fMoveOffsetY = -dt * fMovePerSecond * 0.4f * 0.7f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_LEFTDOWN:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*
					fMoveOffsetX = -dt * m_pPlayer->GetSpeed() * 0.6f * 1.4f;
										fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.4f * 1.4f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.6f * 1.4f;
					fMoveOffsetY = dt * fMovePerSecond * 0.4f * 1.4f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)));

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = -dt * m_pPlayer->GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.6f * 0.7f;
					fMoveOffsetY = dt * fMovePerSecond * 0.4f * 0.7f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		}
	}

	if(m_bHurt &&
		!IsMoving() &&
		(GetStatus() != PST_ATTACKNOWEAPON && GetStatus() != PST_ATTACKWEAPON && GetStatus() != PST_ATTACKSTOP && GetStatus() != PST_ATTACKSTOP))
	{
		if(GetStatus() == PST_DEAD)
		{
			return;
		}
		SetStatus(PST_ATTACKED);
		SetCurFrame(0);
		m_bHurt = false;
	}


	//static float tDetected = 0;
	if((m_pPlayer->GetStatus() == PST_WALK ||
		m_pPlayer->GetStatus() == PST_RUN) &&
		!m_pPlayer->IsMoving() &&
		IsLastFrame())
	{
		//	走动状态
		if(abs(m_fDetect - 0.0f) < 0.00001f)
		{
			m_fDetect = AfxGetHge()->Timer_GetTime();
		}
		else
		{
			if(abs(m_fDetect - AfxGetHge()->Timer_GetTime()) > 0.1f)
			{
				m_pPlayer->SetStatus(PST_STAND);
				m_pPlayer->SetCurFrame(0);
				m_fDetect = 0.0f;
			}
		}
	}
	else
	{
		m_fDetect = 0.0f;
	}

	//	动态
	if(m_pPlayer->GetStatus() == PST_STAND)
	{
		if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_STONE))
		{
			SetCurFrame(0);
		}
		else
		{
			int nFrm = ((int)((AfxGetHge()->Timer_GetTime() - m_pPlayer->GetLastStandTime()) / 0.5)) % 4;
			m_pPlayer->SetCurFrame(nFrm);
		}
	}

	//	人物效果
	UpdateHumState();
}

bool GameOtherPlayer::ProcUserCmd(const POINT& _mp)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
int GameOtherPlayer::CalWeaponTexIndex(int* _ptexsum /* = NULL */, int* _pResIndex /* = NULL */)
{
#ifdef _NET_GAME_
	ItemAttrib* pWeapon = &m_equip[PLAYER_ITEM_WEAPON];
	int nIndex = 600;
	int nOffset = 0;
	int nResIndex = 0;

	int tex = pWeapon->tex;
	if(GetExtendAttrib()->uWeaponLook != 0)
	{
		tex = GetExtendAttrib()->uWeaponLook;
	}

	if(pWeapon->type == ITEM_NO)
	{
		nOffset = 0;
		nIndex = 0;
	}
	else
	{
		std::map<int, int>::const_iterator iter = pTheGame->GetWeaponTable().find(tex);
		if(iter == pTheGame->GetWeaponTable().end())
		{
			return 0;
		}
		else
		{
			//nIndex = LOWORD(iter->second);
			nResIndex = HIWORD(iter->second);

			//nOffset = iter->second;
			nOffset = LOWORD(iter->second);
		}
	}

	if(0 == nResIndex)
	{
		//	weapon0的起始索引为600
	}
	else
	{
		nIndex = 0;
	}
#else
	static const int indexTable[] = {30,43,31,36,32,67,33,33,34,34,35,35,40,40,39,39,
		38,38,37,37,41,41,42,42,45,45,47,47,44,44,46,46,48,48,49,49,50,50,51,51,52,52,53,53,54,54,
		55,55,56,5,57,57,58,58,59,59,65,65,66,66,69,69,70,70,71,71};
	bool bFind = false;

	int nIndex = 1200;
	int nOffset = 0;
	if(m_equip[PLAYER_ITEM_WEAPON].type == ITEM_NO)
	{
		nOffset = 0;
		nIndex = 0;
		bFind = true;
	}
	else
	{
		for(int i = 0; i < sizeof(indexTable) / 4; ++i)
		{
			if(indexTable[i] == m_equip[PLAYER_ITEM_WEAPON].tex)
			{
				nOffset = i;
				bFind = true;
				break;
			}
		}
	}

	if(!bFind)
	{
		return 0;
	}
#endif

	nIndex += (nOffset * 600);

	switch(m_stat)
	{
	case PST_STAND:
		{
			if(_ptexsum)
			{
				*_ptexsum = 4;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_WALK:
		{
			nIndex += 64;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_RUN:
		{
			nIndex += 128;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_ATTACKSTOP:
		{
			nIndex += 192;
			if(_ptexsum)
			{
				*_ptexsum = 1;
			}
			nIndex += m_drt;
		}break;
	case PST_ATTACKNOWEAPON:
		{
			nIndex += 200;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_ATTACKWEAPON:
		{
			nIndex += 264;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_FORCEATTACK:
		{
			nIndex += 328;
			if(_ptexsum)
			{
				*_ptexsum = 8;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_SK_LEIDIANSHU:
		{
			nIndex += 392;
			if(_ptexsum)
			{
				*_ptexsum = 8;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_WAROU:
		{
			nIndex += 456;
			if(_ptexsum)
			{
				*_ptexsum = 2;
			}
			nIndex += 2 * m_drt;
		}break;
	case PST_ATTACKED:
		{
			nIndex += 472;
			if(_ptexsum)
			{
				*_ptexsum = 3;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_DEAD:
		{
			nIndex += 536;
			if(_ptexsum)
			{
				*_ptexsum = 4;
			}
			nIndex += 8 * m_drt;
		}break;
	}

	if(NULL != _pResIndex)
	{
		*_pResIndex = nResIndex;
	}
	return nIndex;
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::OnSetStatus(PLAYER_STATUS _ps)
{
	if(_ps == PST_ATTACKSTOP)
	{
		m_dwLastAttackStopTime = GetTickCount();
	}

	if(_ps == PST_ATTACKNOWEAPON ||
		_ps == PST_ATTACKWEAPON)
	{
		m_dwLastAttackTime = GetTickCount();
	}
}
//////////////////////////////////////////////////////////////////////////
int GameOtherPlayer::CalTextureIndex(int* _ptexsum /* = NULL */, int* _pResIndex /* = NULL */)
{
#ifdef _NET_GAME_
	int tex = m_equip[PLAYER_ITEM_CLOTH].tex;

	if(0 != GetExtendAttrib()->uClothLook)
	{
		tex = GetExtendAttrib()->uClothLook;
	}

	int nIndex = 0;
	int nResIndex = 0;

	std::map<int, int>::const_iterator iter = pTheGame->GetClothTable().find(tex);
	if(iter != pTheGame->GetClothTable().end())
	{
		nIndex = LOWORD(iter->second);
		nResIndex = HIWORD(iter->second);
		//nIndex = nIndex / 2 + 1;
	}
	else if(tex == 0)
	{
		if(m_attrib.sex == 2)
		{
			nIndex = 1;
		}
	}
#else
	static const int indexTable[] = {60,80,61,81,62,82,63,83,64,84,85,88,86,89,87,90};
	int tex = m_equip[PLAYER_ITEM_CLOTH].tex;
	//int nIndex = m_equip[PLAYER_ITEM_CLOTH].id;
	bool bFind = false;
	for(int i = 0; i < sizeof(indexTable) / 4; ++i)
	{
		if(indexTable[i] == tex)
		{
			tex = i;
			bFind = true;
			break;
		}
	}

	int nIndex = 0;
	if(bFind)
	{
		nIndex = tex / 2 + 1;
	}
	else
	{
		nIndex = 0;
	}
#endif
	nIndex *= 600;
	/*if(m_sex == PSX_WOMAN)
	{
		nIndex += 600;
	}*/
	/*if(m_attrib.sex == 2)
	{
		nIndex += 600;
	}*/

	switch(m_stat)
	{
	case PST_STAND:
		{
			if(_ptexsum)
			{
				*_ptexsum = 4;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_WALK:
		{
			nIndex += 64;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_RUN:
		{
			nIndex += 128;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_ATTACKSTOP:
		{
			nIndex += 192;
			if(_ptexsum)
			{
				*_ptexsum = 1;
			}
			nIndex += m_drt;
		}break;
	case PST_ATTACKNOWEAPON:
		{
			nIndex += 200;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_ATTACKWEAPON:
		{
			nIndex += 264;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_FORCEATTACK:
		{
			nIndex += 328;
			if(_ptexsum)
			{
				*_ptexsum = 8;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_SK_LEIDIANSHU:
		{
			nIndex += 392;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_WAROU:
		{
			nIndex += 456;
			if(_ptexsum)
			{
				*_ptexsum = 2;
			}
			nIndex += 2 * m_drt;
		}break;
	case PST_ATTACKED:
		{
			nIndex += 472;
			if(_ptexsum)
			{
				*_ptexsum = 3;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_DEAD:
		{
			nIndex += 536;
			if(_ptexsum)
			{
				*_ptexsum = 4;
			}
			nIndex += 8 * m_drt;
		}break;
	}

	if(_pResIndex)
	{
		*_pResIndex = nResIndex;
	}
	return nIndex;
}
//////////////////////////////////////////////////////////////////////////
DWORD GameOtherPlayer::CalCustomWingIndexAndRes()
{
	int nRes = RES_HUMEFFECT;

	if(m_stExtAttrib.uWing != 0)
	{
		int nIndex = -1;

		//	humeffect1 1-10 humeffect2 13 14 15 17

		//	自定义翅膀 humeffect total 1200frames
		if(m_stExtAttrib.uWing <= 10)
		{
			nIndex = (m_stExtAttrib.uWing - 1) * 1200;
		}
		else
		{
			//	> 10的索引 需要-11计算索引
			static int s_nHumEffect2Map[] = 
			{
				-1,
				-1,
				2400,
				3600,
				4800,
				-1,
				7200,
				-1,
				-1,
				-1,
			};

			int nEffect2Index = m_stExtAttrib.uWing - 11;
			if(nEffect2Index >= 0 &&
				nEffect2Index < sizeof(s_nHumEffect2Map) / sizeof(s_nHumEffect2Map[0]))
			{
				nIndex = s_nHumEffect2Map[nEffect2Index];
				nRes = RES_HUMEFFECT2;
			}
		}

		if(-1 != nIndex)
		{
			if(m_attrib.sex == 2)
			{
				nIndex += 600;
			}

			switch(m_stat)
			{
			case PST_STAND:
				{
					nIndex += 8 * m_drt;
				}break;
			case PST_WALK:
				{
					nIndex += 64;
					nIndex += 8 * m_drt;
				}break;
			case PST_RUN:
				{
					nIndex += 128;
					nIndex += 8 * m_drt;
				}break;
			case PST_ATTACKSTOP:
				{
					nIndex += 192;
					nIndex += m_drt;
				}break;
			case PST_ATTACKNOWEAPON:
				{
					nIndex += 200;
					nIndex += 8 * m_drt;
				}break;
			case PST_ATTACKWEAPON:
				{
					nIndex += 264;
					nIndex += 8 * m_drt;
				}break;
			case PST_FORCEATTACK:
				{
					nIndex += 328;
					nIndex += 8 * m_drt;
				}break;
			case PST_SK_LEIDIANSHU:
				{
					nIndex += 392;
					nIndex += 8 * m_drt;
				}break;
			case PST_WAROU:
				{
					nIndex += 456;
					nIndex += 2 * m_drt;
				}break;
			case PST_ATTACKED:
				{
					nIndex += 472;
					nIndex += 8 * m_drt;
				}break;
			case PST_DEAD:
				{
					nIndex += 536;
					nIndex += 8 * m_drt;
				}break;
			}

			return MAKELONG(nRes, nIndex);
		}
	}


	int nWingIndex = CalWingIndex();
	if(-1 == nWingIndex)
	{
		return 0xffffffff;
	}

	return MAKELONG(RES_HUMEFFECT, nWingIndex);
}
//////////////////////////////////////////////////////////////////////////
int GameOtherPlayer::CalWingIndex(int* _ptexsum /* = NULL */)
{
	int nIndex = 0;
	if(m_equip[PLAYER_ITEM_CLOTH].accuracy != 0)
	{
		nIndex = m_equip[PLAYER_ITEM_CLOTH].accuracy - 1;

		if(0 != GetExtendAttrib()->uClothLook)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	nIndex *= 1200;
	/*if(m_sex == PSX_WOMAN)
	{
		nIndex += 600;
	}*/
	if(m_attrib.sex == 2)
	{
		nIndex += 600;
	}

	switch(m_stat)
	{
	case PST_STAND:
		{
			if(_ptexsum)
			{
				*_ptexsum = 4;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_WALK:
		{
			nIndex += 64;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_RUN:
		{
			nIndex += 128;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_ATTACKSTOP:
		{
			nIndex += 192;
			if(_ptexsum)
			{
				*_ptexsum = 1;
			}
			nIndex += m_drt;
		}break;
	case PST_ATTACKNOWEAPON:
		{
			nIndex += 200;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_ATTACKWEAPON:
		{
			nIndex += 264;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_FORCEATTACK:
		{
			nIndex += 328;
			if(_ptexsum)
			{
				*_ptexsum = 8;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_SK_LEIDIANSHU:
		{
			nIndex += 392;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_WAROU:
		{
			nIndex += 456;
			if(_ptexsum)
			{
				*_ptexsum = 2;
			}
			nIndex += 2 * m_drt;
		}break;
	case PST_ATTACKED:
		{
			nIndex += 472;
			if(_ptexsum)
			{
				*_ptexsum = 3;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_DEAD:
		{
			nIndex += 536;
			if(_ptexsum)
			{
				*_ptexsum = 4;
			}
			nIndex += 8 * m_drt;
		}break;
	}
	return nIndex;
}
//////////////////////////////////////////////////////////////////////////
int GameOtherPlayer::CalHairResIndex()
{
	if(m_stExtAttrib.uHair == 0)
	{
		return RES_HAIR;
	}

	if(m_stExtAttrib.uHair > 0 &&
		m_stExtAttrib.uHair <= 4)
	{
		return RES_HAIR;
	}

	return RES_HAIR2;
}
//////////////////////////////////////////////////////////////////////////
int GameOtherPlayer::CalHairTexIndex(int* _ptexsum /* = NULL */)
{
	int nIndex = 600;
	int nHairStyle = 0;

	if(m_stExtAttrib.uHair == 0)
	{
		if(m_attrib.sex == 1)
		{
			nHairStyle = 3;
		}
		else
		{
			nHairStyle = 2;
		}
	}
	else
	{
		nHairStyle = m_stExtAttrib.uHair;
	}
	nIndex += nHairStyle * 600;

	switch(m_stat)
	{
	case PST_STAND:
		{
			if(_ptexsum)
			{
				*_ptexsum = 4;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_WALK:
		{
			nIndex += 64;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_RUN:
		{
			nIndex += 128;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_ATTACKSTOP:
		{
			nIndex += 192;
			if(_ptexsum)
			{
				*_ptexsum = 1;
			}
			nIndex += m_drt;
		}break;
	case PST_ATTACKNOWEAPON:
		{
			nIndex += 200;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_ATTACKWEAPON:
		{
			nIndex += 264;
			if(_ptexsum)
			{
				*_ptexsum = 6;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_FORCEATTACK:
		{
			nIndex += 328;
			if(_ptexsum)
			{
				*_ptexsum = 8;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_SK_LEIDIANSHU:
		{
			nIndex += 392;
			if(_ptexsum)
			{
				*_ptexsum = 8;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_WAROU:
		{
			nIndex += 456;
			if(_ptexsum)
			{
				*_ptexsum = 2;
			}
			nIndex += 2 * m_drt;
		}break;
	case PST_ATTACKED:
		{
			nIndex += 472;
			if(_ptexsum)
			{
				*_ptexsum = 3;
			}
			nIndex += 8 * m_drt;
		}break;
	case PST_DEAD:
		{
			nIndex += 536;
			if(_ptexsum)
			{
				*_ptexsum = 4;
			}
			nIndex += 8 * m_drt;
		}break;
	}
	return nIndex;
}
//////////////////////////////////////////////////////////////////////////
//	For magic
void GameOtherPlayer::SetMagicEffect(MAGICEFFECT_TYPE _mgc)
{
	for(int i = 0; i < MAGICEFFECT_TOTAL; ++i)
	{
		if(m_eEffect[i] == ME_NONE)
		{
			m_eEffect[i] = _mgc;
			break;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::ResetMagicEffect(MAGICEFFECT_TYPE _mgc)
{
	if(_mgc == ME_NONE)
	{
		for(int i = 0; i < MAGICEFFECT_TOTAL; ++i)
		{
			m_eEffect[i] = ME_NONE;
		}
	}
	else
	{
		for(int i = 0; i < MAGICEFFECT_TOTAL; ++i)
		{
			if(m_eEffect[i] == _mgc)
			{
				m_eEffect[i] = ME_NONE;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::RenderMagic()
{
	const MagicRenderInfo* pInfo = NULL;
	GameTextureManager* pTexs = NULL;
	HTEXTURE tex = 0;
	short sDrawOftX = 0;
	short sDrawOftY = 0;
	int nDrawX = 0;
	int nDrawY = 0;
	int nTexWidth = 0;
	int nTexHeight = 0;
	int nTexIndex = 0;
	
	for(int i = 0; i < MAGICEFFECT_TOTAL; ++i)
	{
		switch(m_eEffect[i])
		{
		case ME_BANYUE:
		case ME_LIEHUO:
		case ME_CISHA:
		case ME_GONGSHA:
			{
				if(GameInfoManager::GetInstance()->GetMagicRenderInfo(m_eEffect[i], &pInfo))
				{
					pTexs = GameResourceManager::GetInstance()->GetTexs(RES_MAGIC + pInfo->wil - 1);
					if(pTexs)
					{
						if(GetStatus() == PST_ATTACKWEAPON ||
							GetStatus() == PST_ATTACKNOWEAPON)
						{
							nTexIndex = pInfo->selfbegin + pInfo->selftotal * m_drt + GetCurFrame();
							tex = pTexs->GetTexture(nTexIndex);
							if(tex)
							{
								sDrawOftX = pTexs->GetTextureOffsetX(nTexIndex);
								sDrawOftY = pTexs->GetTextureOffsetY(nTexIndex);
								nTexWidth = pTexs->GetTextureWidth(nTexIndex);
								nTexHeight = pTexs->GetTextureHeight(nTexIndex);
								m_pEffectRender->SetTexture(tex);
								m_pEffectRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
								nDrawX = CENTRAL_X + m_fPosx - GamePlayer::GetInstance()->GetObjx();
								nDrawY = CENTRAL_Y + m_fPosy - GamePlayer::GetInstance()->GetObjy();
								m_pEffectRender->Render(nDrawX + sDrawOftX,
									nDrawY + sDrawOftY);
							}
						}
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//	For Magic
bool GameOtherPlayer::PlayMagic(int _nMgcID, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	bool bRet = false;
	//if(m_pMagicEffect)
	{
		//delete m_pMagicEffect;
		//m_pMagicEffect = NULL;
	}

	MagicElement* pMagicEffect = NULL;
	if((_nMgcID >= MEFF_SFIREBALL && _nMgcID <= MEFF_THUNDER) ||
		(_nMgcID >= MEFF_FIRECHARM && _nMgcID < MEFF_HIDE) ||
		_nMgcID == MEFF_SUMMON ||
		_nMgcID == MEFF_SUPERSUMMON ||
		_nMgcID == MEFF_ICEPALM ||
		_nMgcID == MEFF_SKYFIRE ||
		_nMgcID == MEFF_FIRESHOWER ||
		_nMgcID == MEFF_ICETHRUST ||
		_nMgcID == MEFF_DRAGONBLUSTER)
	{
		int nMagicID = _nMgcID;
		if(_nMgcID == MEFF_FIRECHARM ||
			_nMgcID == MEFF_THUNDER)
		{
			//	判断用户的技能等级
			int nMagicLevel = LOWORD(_dwAniTime);
			if(nMagicLevel > 3)
			{
				nMagicID = nMagicID + 500;
			}
		}

		pMagicEffect = new MagicFireBall;
		bRet = pMagicEffect->Create(nMagicID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			//	Remove
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			if(_nMgcID == MEFF_DRAGONBLUSTER)
			{
				pMagicEffect->SetSelfInterval(180);
			}
			//
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_HIDE)
	{
		pMagicEffect = new MagicHide;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			//	Remove
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			//
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID >= MEFF_FIREWALL &&
		_nMgcID <= MEFF_FIREWALL)
	{
		pMagicEffect = new MagicFireWall;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			//	Remove
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			//
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_ENERGYSHIELD)
	{
		pMagicEffect = new MagicEnergyShield;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			//	Remove
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			//
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID >= MEFF_SHIELD &&
		_nMgcID <= MEFF_SHIELD)
	{
		pMagicEffect = new MagicShield;
		DWORD dwAniTime = _dwAniTime;
		int nMgcOffset = 0;
		int nMgcLevel = GetSkillLevel(MEFF_SHIELD);
		if(nMgcLevel > 3)
		{
			nMgcOffset = 500;
		}
		bRet = pMagicEffect->Create(_nMgcID + nMgcOffset, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			//	Remove
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			//
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_HEAL)
	{
		pMagicEffect = new MagicHeal;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_POISON)
	{
		pMagicEffect = new MagicPoison;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_KULOU)
	{
		pMagicEffect = new MagicElement;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			pMagicEffect->SetStage(MAGIC_STAGE_SELF);
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_TEMPT)
	{
		pMagicEffect = new MagicTempt;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_LIONROAR)
	{
		pMagicEffect = new MagicLionRoar;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_SUMMONAC)
	{
		pMagicEffect = new MagicLionRoar;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_SUMMONTIGER ||
		_nMgcID == MEFF_SUMMONBOWMAN)
	{
		pMagicEffect = new MagicLionRoar;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_BLOODDRAGON)
	{
		pMagicEffect = new MagicHumEff;
		const MagicRenderInfo* pMgcRenderInfo = NULL;
		if(GameInfoManager::GetInstance()->GetMagicRenderInfo(_nMgcID, &pMgcRenderInfo))
		{
			bRet = pMagicEffect->Create(RES_MAGIC + pMgcRenderInfo->wil, this, (GameObject*)_nMgcID, pMgcRenderInfo->selfbegin, pMgcRenderInfo->selfframe, _dwAniTime);

			if(!bRet)
			{
				SAFE_DELETE(pMagicEffect);
			}
			else
			{
				pMagicEffect->SetActive();
				pMagicEffect->EnableSound();
				pMagicEffect->SetHumEff(true);
				if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
					abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
				{
					GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
				}
				GameScene::sThis->InsertNewMagic(pMagicEffect);
			}
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_JINGANG)
	{
		/*pMagicEffect = new MagicHumEff;
		const MagicRenderInfo* pMgcRenderInfo = NULL;
		if(GameInfoManager::GetInstance()->GetMagicRenderInfo(_nMgcID, &pMgcRenderInfo))
		{
			bRet = pMagicEffect->Create(RES_MAGIC + pMgcRenderInfo->wil, this, (GameObject*)_nMgcID, pMgcRenderInfo->selfbegin, pMgcRenderInfo->selfframe, _dwAniTime);

			if(!bRet)
			{
				SAFE_DELETE(pMagicEffect);
			}
			else
			{
				pMagicEffect->SetActive();
				pMagicEffect->EnableSound();
				pMagicEffect->SetHumEff(true);
				if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
					abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
				{
					GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
				}
				GameScene::sThis->InsertNewMagic(pMagicEffect);
			}
		}*/

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_SUPERHEAL)
	{
		pMagicEffect = new MagicFireBall;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			//	Remove
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			//
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}
	else if(_nMgcID == MEFF_BIGPOISON)
	{
		pMagicEffect = new MagicBigPoison;
		bRet = pMagicEffect->Create(_nMgcID, this, _pTarget, _sDestX, _sDestY, _dwAniTime);

		if(false == bRet)
		{
			//	Remove
			delete pMagicEffect;
			pMagicEffect = NULL;
		}
		else
		{
			//
			pMagicEffect->SetActive();
			pMagicEffect->EnableSound();
			if(abs(GetCoordX() - GamePlayer::GetInstance()->GetCoordX()) < MAGIC_PLAY_OFTX ||
				abs(GetCoordY() - GamePlayer::GetInstance()->GetCoordY()) < MAGIC_PLAY_OFTY)
			{
				GameSoundManager::GetInstancePtr()->PlayMagicSound(_nMgcID, MAGIC_STAGE_SELF);
			}
			GameScene::sThis->InsertNewMagic(pMagicEffect);
		}

		SetStatus(PST_SK_LEIDIANSHU);
		SetCurFrame(0);
	}

	return bRet;
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::RenderSelfEffect()
{
	if(NULL == m_pMagicEffect)
	{
		return;
	}
	//if(GetStatus() == PST_SK_LEIDIANSHU)
	{
		DWORD dwCurTick = GetTickCount();
		const MagicRenderInfo* pInfo = m_pMagicEffect->GetRenderInfo();

		BYTE bFrameTotal = pInfo->selfframe;
		if(m_pMagicEffect->GetCurFrame() + 1 >= bFrameTotal)
		{
			//	Last frame
			if(dwCurTick - m_pMagicEffect->GetLastUpdateTime() > m_pMagicEffect->GetSelfInterval())
			{
				m_pMagicEffect->ResetFrame();
				m_pMagicEffect->SetActive();
				GameScene::sThis->InsertNewMagic(m_pMagicEffect);
				m_pMagicEffect = NULL;
			}
		}
		else
		{
			if(dwCurTick - m_pMagicEffect->GetLastUpdateTime() > m_pMagicEffect->GetSelfInterval())
			{
				m_pMagicEffect->IncFrame();
				m_pMagicEffect->SetLastUpdateTime(dwCurTick);
			}
		}

		if(m_pMagicEffect)
		{
			GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MAGIC + pInfo->wil);
			if(pTexMgr)
			{
				int nRenderX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
				int nRenderY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
				int nTexIndex = pInfo->selfbegin + m_pMagicEffect->GetCurFrame();
				HTEXTURE tex = pTexMgr->GetTexture(nTexIndex);
				if(tex)
				{
					WORD wTexWidth = pTexMgr->GetTextureWidth(nTexIndex);
					WORD wTexHeight = pTexMgr->GetTextureHeight(nTexIndex);
					short sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
					short sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
					m_pEffectRender->SetTexture(tex);
					m_pEffectRender->SetTextureRect(0, 0, wTexWidth, wTexHeight);
					m_pEffectRender->Render(nRenderX + sOftX,
						nRenderY + sOftY);
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
/*
void GameOtherPlayer::SetHumEffect(DWORD _dwMask, DWORD _dwTime)
{
	m_dwHumEffectFlag |= _dwMask;

	if(_dwMask == MMASK_LVDU)
	{
		m_dwHumEffectTime[MMASK_C_LVDU_INDEX] = _dwTime;
	}
	else if(_dwMask == MMASK_HONGDU)
	{
		m_dwHumEffectTime[MMASK_C_HONGDU_INDEX] = _dwTime;
	}
	else if(_dwMask == MMASK_SHIELD)
	{
		m_dwHumEffectTime[MMASK_C_SHEILD_INDEX] = _dwTime;
	}
}*/
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::TransparentRender()
{
	if(!m_pRender)
	{
		return;
	}

	SelfEffectRender();

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nRenderX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nRenderY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
	DWORD dwPreCol = m_pRender->GetColor();
	DWORD dwCurTick = GetTickCount();
	bool bChangedColor = false;

	if(TEST_FLAG(m_dwHumEffectFlag, MMASK_HONGDU))
	{
		if(m_dwHumEffectTime[MMASK_C_HONGDU_INDEX] < dwCurTick)
		{
			m_dwHumEffectTime[MMASK_C_HONGDU_INDEX] = 0;
			RESET_FLAG(m_dwHumEffectFlag, MMASK_HONGDU);
		}
		else
		{
			m_pRender->SetColor(ARGB(96, 0xFF, 0, 0));
			bChangedColor = true;
		}
	}

	if(TEST_FLAG(m_dwHumEffectFlag, MMASK_LVDU))
	{
		if(m_dwHumEffectTime[MMASK_C_LVDU_INDEX] < dwCurTick)
		{
			m_dwHumEffectTime[MMASK_C_LVDU_INDEX] = 0;
			RESET_FLAG(m_dwHumEffectFlag, MMASK_LVDU);
		}
		else
		{
			m_pRender->SetColor(ARGB(48, 0, 0xFF, 0));
			bChangedColor = true;
		}
	}

	bool bIsStone = false;
	if(TEST_FLAG(m_dwHumEffectFlag, MMASK_STONE))
	{
		if(m_dwHumEffectTime[MMASK_C_STONE_INDEX] < dwCurTick)
		{
			m_dwHumEffectTime[MMASK_C_STONE_INDEX] = 0;
			RESET_FLAG(m_dwHumEffectFlag, MMASK_STONE);
		}
		else
		{
			m_pRender->SetColor(COLOR_EFF_STONE);
			bChangedColor = true;
			bIsStone = true;
		}
	}

	if(!bChangedColor)
	{
		m_pRender->SetColor(ARGB(96, 255, 255, 255));
	}
	/*if(bIsStone &&
		GetStatus() != PST_DEAD)
	{
		SetStatus(PST_STAND);
		SetCurFrame(0);
	}*/
	m_bTransparentMode = true;
	Render();
	m_bTransparentMode = false;

	m_pRender->SetColor(ARGB(96, 255, 255, 255));

	//if(m_dwHumEffectFlag & MMASK_SHIELD)
	if(TEST_FLAG(m_dwHumEffectFlag, MMASK_SHIELD))
	{
		HTEXTURE tex = 0;
		int nTexIndex = 0;
		short sOftX = 0;
		short sOftY = 0;

		if(m_dwHumEffectTime[MMASK_C_SHEILD_INDEX] < dwCurTick)
		{
			m_dwHumEffectTime[MMASK_C_SHEILD_INDEX] = 0;
			//m_dwHumEffectFlag &= (~MMASK_SHIELD);
			RESET_FLAG(m_dwHumEffectFlag, MMASK_SHIELD);
		}
		else
		{
			const MagicRenderInfo* pInfo = NULL;
			int nMagicOffset = 0;
			int nShieldLevel = GetSkillLevel(MEFF_SHIELD);
			if(nShieldLevel > 3)
			{
				nMagicOffset = 500;
			}

			if(GameInfoManager::GetInstance()->GetMagicRenderInfo(MEFF_SHIELD + nMagicOffset, &pInfo))
			{
				GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(pInfo->wil + RES_MAGIC);

				if(pTexMgr)
				{
					if(GetStatus() != PST_ATTACKED)
					{
						nTexIndex = pInfo->waybegin + m_bCurFrame % 3;
						tex = pTexMgr->GetTexture(nTexIndex);
						if(tex)
						{
							MagicElement::pEffectRender->SetTexture(tex);
							MagicElement::pEffectRender->SetTextureRect(0, 0, pTexMgr->GetTextureWidth(nTexIndex),
								pTexMgr->GetTextureHeight(nTexIndex));
							sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
							sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
							MagicElement::pEffectRender->Render(nRenderX + sOftX,
								nRenderY + sOftY);
						}
					}
					else
					{
						nTexIndex = pInfo->destbegin + m_bCurFrame;
						tex = pTexMgr->GetTexture(nTexIndex);
						if(tex)
						{
							MagicElement::pEffectRender->SetTexture(tex);
							MagicElement::pEffectRender->SetTextureRect(0, 0, pTexMgr->GetTextureWidth(nTexIndex),
								pTexMgr->GetTextureHeight(nTexIndex));
							sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
							sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
							MagicElement::pEffectRender->Render(nRenderX + sOftX,
								nRenderY + sOftY);
						}
					}
				}
			}
		}
	}

	if(TEST_FLAG(m_dwHumEffectFlag, MMASK_ENERGYSHIELD))
	{
		HTEXTURE tex = 0;
		int nTexIndex = 0;
		short sOftX = 0;
		short sOftY = 0;

		if(m_dwHumEffectTime[MMASK_C_ENERGYSHIELD_INDEX] < dwCurTick)
		{
			m_dwHumEffectTime[MMASK_C_ENERGYSHIELD_INDEX] = 0;
			RESET_FLAG(m_dwHumEffectFlag, MMASK_ENERGYSHIELD);
		}
		else
		{
			const MagicRenderInfo* pInfo = NULL;
			if(GameInfoManager::GetInstance()->GetMagicRenderInfo(MEFF_ENERGYSHIELD, &pInfo))
			{
				GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(pInfo->wil + RES_MAGIC);
				if(pTexMgr)
				{
					if(GetStatus() != PST_ATTACKED)
					{
						nTexIndex = pInfo->waybegin + m_bCurFrame % 3;
						tex = pTexMgr->GetTexture(nTexIndex);
						if(tex)
						{
							MagicElement::pEffectRender->SetTexture(tex);
							MagicElement::pEffectRender->SetTextureRect(0, 0, pTexMgr->GetTextureWidth(nTexIndex),
								pTexMgr->GetTextureHeight(nTexIndex));
							sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
							sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
							MagicElement::pEffectRender->Render(nRenderX + sOftX,
								nRenderY + sOftY);
						}
					}
					else
					{
						nTexIndex = pInfo->destbegin + m_bCurFrame;
						tex = pTexMgr->GetTexture(nTexIndex);
						if(tex)
						{
							MagicElement::pEffectRender->SetTexture(tex);
							MagicElement::pEffectRender->SetTextureRect(0, 0, pTexMgr->GetTextureWidth(nTexIndex),
								pTexMgr->GetTextureHeight(nTexIndex));
							sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
							sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
							MagicElement::pEffectRender->Render(nRenderX + sOftX,
								nRenderY + sOftY);
						}
					}
				}
			}
		}
	}
	RenderHitEffect();
	RenderSkillEffect();

	m_pRender->SetColor(dwPreCol);
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::RenderHitEffect()
{
	GameTextureManager* pTexMgr = NULL;
	const MagicRenderInfo* pInfo = NULL;
	HTEXTURE tex = 0;
	short sOftX = 0;
	short sOftY = 0;
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nRenderX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nRenderY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

	if(m_dwHumEffectFlag & MMASK_GONGSHA)
	{
		GameInfoManager::GetInstance()->GetMagicRenderInfo(MEFF_GONGSHA, &pInfo);
	}
	else if(m_dwHumEffectFlag & MMASK_CISHA)
	{
		GameInfoManager::GetInstance()->GetMagicRenderInfo(MEFF_CISHA, &pInfo);
	}
	else if(m_dwHumEffectFlag & MMASK_BANYEU)
	{
		GameInfoManager::GetInstance()->GetMagicRenderInfo(MEFF_BANYUE, &pInfo);
	}
	else if(m_dwHumEffectFlag & MMASK_LIEHUO)
	{
		GameInfoManager::GetInstance()->GetMagicRenderInfo(MEFF_LIEHUO, &pInfo);
	}
	else if(m_dwHumEffectFlag & MMASK_SLIEHUO)
	{
		GameInfoManager::GetInstance()->GetMagicRenderInfo(MEFF_SLIEHUO, &pInfo);
	}
	else if(m_dwHumEffectFlag & MMASK_KTSWORD)
	{
		GameInfoManager::GetInstance()->GetMagicRenderInfo(MEFF_KTSWORD, &pInfo);
	}

	if(NULL == pInfo)
	{
		return;
	}

	pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MAGIC);
	if(m_dwHumEffectFlag & MMASK_SLIEHUO)
	{
		pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MAGIC4);
	}
	if(m_dwHumEffectFlag & MMASK_KTSWORD)
	{
		pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MAGIC5);
	}
	if(NULL == pTexMgr)
	{
		return;
	}

	if(GetStatus() == PST_ATTACKNOWEAPON ||
		GetStatus() == PST_ATTACKWEAPON)
	{
		int nTexIndex = pInfo->selfbegin + m_bCurFrame + GetDirection() * pInfo->selftotal;
		sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
		sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
		tex = pTexMgr->GetTexture(nTexIndex);
		if(tex)
		{
			MagicElement::pEffectRender->SetTexture(tex);
			MagicElement::pEffectRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(nTexIndex), pTexMgr->GetTextureHeight(nTexIndex));
			MagicElement::pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::ClearHitEffect()
{
	m_dwHumEffectFlag &= (~MMASK_CISHA);
	m_dwHumEffectFlag &= (~MMASK_GONGSHA);
	m_dwHumEffectFlag &= (~MMASK_BANYEU);
	m_dwHumEffectFlag &= (~MMASK_LIEHUO);
	m_dwHumEffectFlag &= (~MMASK_SLIEHUO);
	m_dwHumEffectFlag &= (~MMASK_KTSWORD);
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::SelfEffectUpdate()
{
	if(m_pMagicEffect)
	{
		m_pMagicEffect->Update();
		if(m_pMagicEffect->CanRemove())
		{
			delete m_pMagicEffect;
			m_pMagicEffect = NULL;
		}
		else
		{
			if(m_pMagicEffect->GetStage() != MAGIC_STAGE_SELF ||
				(m_pMagicEffect->GetStage() == MAGIC_STAGE_SELF && m_pMagicEffect->IsSelfLastFrame()))
			{
				GameScene::sThis->InsertNewMagic(m_pMagicEffect);
				m_pMagicEffect = NULL;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::SelfEffectRender()
{
	if(m_pMagicEffect)
	{
		m_pMagicEffect->Render();
	}
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::SetSelfEffect(MagicElement* _pEle)
{
	if(m_pMagicEffect != NULL)
	{
		GameScene::sThis->InsertNewMagic(m_pMagicEffect);
		m_pMagicEffect = NULL;
	}

	m_pMagicEffect = _pEle;
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::PlayAttackSound()
{
	int nOftX = abs((int)GetCoordX() - (int)GamePlayer::GetInstance()->GetCoordX());
	int nOftY = abs((int)GetCoordY() - (int)GamePlayer::GetInstance()->GetCoordY());
	bool bCanPlay = false;

	if(GetType() == OBJ_OTHERPLAYER)
	{
		if(nOftX <= 8 &&
			nOftY <= 8)
		{
			bCanPlay = true;
		}
	}
	else if(GetType() == OBJ_PLAYER)
	{
		bCanPlay = true;
	}

	if(bCanPlay)
	{
		const ItemAttrib* pWeapon = &m_equip[PLAYER_ITEM_WEAPON];
		if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_GONGSHA))
		{
			if(GetAttrib()->sex == 2)
			{
				GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_FEMALEGONGSHA);
			}
			else
			{
				GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_MALEGONGSHA);
			}
		}
		else if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_LIEHUO)/* ||
			TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_SLIEHUO)*/)
		{
			GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_LIEHUO);
		}
		else if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_SLIEHUO))
		{
			if(GetAttrib()->sex == 2)
			{
				GameSoundManager::GetInstancePtr()->PlayHumSound(201);
			}
			else
			{
				GameSoundManager::GetInstancePtr()->PlayHumSound(200);
			}
		}
		else if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_CISHA))
		{
			GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_CISHA);
		}
		if(pWeapon->type == ITEM_NO)
		{
			GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_FISTATTACK);
		}
		else
		{
			GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_SWORDATTACK);
		}
	}
}

void GameOtherPlayer::ClearHumState()
{
	if(!m_xHumStates.empty())
	{
		MagicElementList::const_iterator begIter = m_xHumStates.begin();
		MagicElementList::const_iterator endIter = m_xHumStates.end();
		MagicElement* pMagicElement = NULL;

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			pMagicElement = *begIter;
			SAFE_DELETE(pMagicElement);
		}

		m_xHumStates.clear();
	}
}

bool GameOtherPlayer::InsertHumState(MagicElement* _pMgc)
{
	//	MagicID不能相同
	if(_pMgc == NULL)
	{
		return false;
	}

	if(!m_xHumStates.empty())
	{
		MagicElementList::const_iterator begIter = m_xHumStates.begin();
		MagicElementList::const_iterator endIter = m_xHumStates.end();
		MagicElement* pMagicElement = NULL;

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			pMagicElement = *begIter;
			if(pMagicElement->GetMagicID() == _pMgc->GetMagicID())
			{
				return false;
			}
		}
	}

	m_xHumStates.push_back(_pMgc);
	return true;
}

void GameOtherPlayer::UpdateHumState()
{
	if(!m_xHumStates.empty())
	{
		MagicElementList::const_iterator begIter = m_xHumStates.begin();
		MagicElementList::const_iterator endIter = m_xHumStates.end();
		MagicElement* pMagicElement = NULL;

		for(begIter;
			begIter != endIter;
			)
		{
			pMagicElement = *begIter;
			pMagicElement->Update();
			if(pMagicElement->CanRemove())
			{
				SAFE_DELETE(pMagicElement);
				begIter = m_xHumStates.erase(begIter);
			}
			else
			{
				++begIter;
			}
		}
	}
}

void GameOtherPlayer::RenderHumState()
{
	if(!m_xHumStates.empty())
	{
		MagicElementList::const_iterator begIter = m_xHumStates.begin();
		MagicElementList::const_iterator endIter = m_xHumStates.end();
		MagicElement* pMagicElement = NULL;

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			pMagicElement = *begIter;
			pMagicElement->Render();
		}
	}
}

void GameOtherPlayer::RefleashHumState()
{
	ClearHumState();

	for(int i = 0;
		i < PLAYER_ITEM_TOTAL;
		++i)
	{
		ItemAttrib* pItem = GetPlayerItem((PLAYER_ITEM_TYPE)i);

		if(pItem)
		{
			EffectHumCloth* pMagic = new EffectHumCloth;
			pMagic->SetInterval(2000);
			//if(false == pMagic->Create(pItem->HP, this, NULL, 0, 0, 0xFFFFFFFF))
			if(false == pMagic->Init(this, pItem->id))
			{
				SAFE_DELETE(pMagic);
			}
			else
			{
				if(!InsertHumState(pMagic))
				{
					SAFE_DELETE(pMagic);
				}
			}
		}
	}
}

int GameOtherPlayer::GetMinEquipGrade()
{
	return 0;
}

void GameOtherPlayer::RenderSkillEffect()
{
	if(GetRenderMode() != ORM_TRANSPARENT)
	{
		return;
	}
	if(GetTickCount() > m_dwJinGangExpireTime)
	{
		m_dwJinGangExpireTime = 0;
	}

	/*if(m_dwJinGangExpireTime != 0)
	{
		int nFrame = ((m_dwJinGangExpireTime - (GetTickCount() - m_dwJinGangExpireTime)) / 100) % 18;

		HTEXTURE tex = 0;
		GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_EFFECT);

		tex = pTexMgr->GetTexture(nFrame + 80);
		if(tex)
		{
			MagicElement::pEffectRender->SetTexture(tex);
			MagicElement::pEffectRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(80 + nFrame),
				pTexMgr->GetTextureHeight(nFrame + 80));
			int nRenderX = GetScreenPosX() + pTexMgr->GetTextureOffsetX(80 + nFrame);
			int nRenderY = GetScreenPosY() + pTexMgr->GetTextureOffsetY(80 + nFrame);
			MagicElement::pEffectRender->Render(nRenderX, nRenderY);
		}
	}*/

	if(m_dwJinGangExpireTime != 0)
	{
		int nFrame = ((m_dwJinGangExpireTime - (GetTickCount() - m_dwJinGangExpireTime)) / 150) % 10 + 790;

		if(nFrame < 790 + 10)
		{
			HTEXTURE tex = 0;
			GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MAGIC5);

			tex = pTexMgr->GetTexture(nFrame);
			if(tex)
			{
				MagicElement::pEffectRender->SetColor(ARGB(100, 255, 255, 255));
				MagicElement::pEffectRender->SetTexture(tex);
				MagicElement::pEffectRender->SetTextureRect(0, 0,
					pTexMgr->GetTextureWidth(nFrame),
					pTexMgr->GetTextureHeight(nFrame));
				int nRenderX = GetScreenPosX() + pTexMgr->GetTextureOffsetX(nFrame);
				int nRenderY = GetScreenPosY() + pTexMgr->GetTextureOffsetY(nFrame);
				MagicElement::pEffectRender->Render(nRenderX, nRenderY);
				MagicElement::pEffectRender->SetColor(ARGB_WHITE);
			}
		}	
	}
}

int GameOtherPlayer::GetSkillLevel(DWORD _dwMgcID)
{
	map<DWORD, DWORD>::const_iterator fndIter = m_xSkillInfo.find(_dwMgcID);
	if(fndIter != m_xSkillInfo.end())
	{
		return fndIter->second;
	}
	return 0;
}

void GameOtherPlayer::AddSkillLevelInfo(DWORD _dwMgcID, DWORD _dwLevel)
{
	map<DWORD, DWORD>::iterator fndIter = m_xSkillInfo.find(_dwMgcID);
	if(fndIter != m_xSkillInfo.end())
	{
		fndIter->second = _dwLevel;
	}
	else
	{
		m_xSkillInfo.insert(std::make_pair(_dwMgcID, _dwLevel));
	}
}

void GameOtherPlayer::ClearSkillLevel()
{
	m_xSkillInfo.clear();
}















//////////////////////////////////////////////////////////////////////////
//	数据包处理
void GameOtherPlayer::OnPacket(const PacketHeader* _pPkt)
{
	g_xBuffer.Reset();
	unsigned int uWriteLen = g_xBuffer.Write(_pPkt, _pPkt->uLen);
	if(uWriteLen != _pPkt->uLen)
	{
		ALERT_MSGBOX("写入全局Buffer失败");
		::PostQuitMessage(0);
		return;
	}

	BEGIN_HANDLE_PACKET(_pPkt)
		HANDLE_PACKET(PKG_GAME_CHAT_NOT,					PkgChatNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_CHANGEEQUIPMENT_NOT,	PkgPlayerChangeEquipNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_GAINITEM_NTF,			PkgPlayerGainItemNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_DROPITEM_NOT,			PkgPlayerDropItemNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_LOSTITEM_ACK,			PkgPlayerLostItemAck)
		//HANDLE_PACKET(PKG_GAME_PLAYER_CLEARITEM_NTF,		PkgPlayerClearItemNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_SYNCATTRIB_NTF,		PkgPlayerSyncAttribNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_UNDRESSITEM_ACK,		PkgPlayerUndressItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_DRESSITEM_ACK,		PkgPlayerDressItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_USEITEM_ACK,			PkgPlayerUseItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_UPDATEATTRIB_NTF,		PkgPlayerUpdateAttribNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_STRUCK_ACK,			PkgPlayerStruckAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_FORCEACTION_ACK,		PkgPlayerForceActionAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_CLEARBAG_NTF,			PkgPlayerClearBagNtf)
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,			PkgObjectActionNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_CHANGEMAP_ACK,		PkgPlayerChangeMapAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_SHOWDOORANI_ACK,		PkgPlayerShowDoorAniAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_HIDEQUESTDLG_ACK,		PkgPlayerHideQuestDlgAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_GETBALEITEM_ACK,		PkgPlayerGetBaleItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_SETEFFECT_ACK,		PkgPlayerSetEffectAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_REVIVE_NOT,			PkgPlayerReviveNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_STOREDATA_ACK,		PkgPlayerStoreDataAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_STORESYNC_ACK,		PkgPlayerStoreSyncAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_SHOWHELP_ACK,			PkgPlayerShowHelpAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_SHOWDLG_ACK,			PkgPlayerShowDlgAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_UPDATECOSTITEM_NTF,	PkgPlayerUpdateCostNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_NETDELAY_ACK,			PkgPlayerNetDelayAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_CLEARALLITEM_ACK,		PkgPlayerClearAllItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_UPDATEITEM_NTF,		PkgPlayerUpdateItemNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_QUICKMSG_NTF,			PkgPlayerQuickMsgNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_VERSIONVERIFY_NTF,	PkgPlayerVersionVerifyNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_QUESTDATA_NTF,		PkgPlayerQuestDataNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_SYNCQUEST_NTF,		PkgPlayerSyncQuestNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_OTHERPLAYEREQUIP_NTF,	PkgPlayerOtherPlayerEquipNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_GSTATUS_NTF,			PkgPlayerGStatusNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_PLAYSOUND_NTF,		PkgPlayerPlaySoundNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_OFFSELLLIST_NTF,		PkgPlayerOffSellListNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_OFFSOLDITEM_ACK,		PkgPlayerOffSoldItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_UID_NTF,				PkgPlayerUIDNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_UPDATEWEAPONGROW_NTF, PkgPlayerUpdateWeaponGrowNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_PRIVATECHAT_ACK,		PkgPlayerPrivateChatAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_PRIVATECHAT_NTF,		PkgPlayerPrivateChatNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_SERVERDELAY_REQ,		PkgPlayerServerDelayReq)
		HANDLE_PACKET(PKG_GAME_PLAYER_RANKLIST_NOT,			PkgPlayerRankListNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_BUYOLSHOPITEM_ACK,	PkgPlayerBuyOlShopItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_GETOLSHOPLIST_ACK,	PkgPlayerGetOlShopListAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_EXTENDATTRIB_NOT,		PkgPlayerExtendAttribNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_SMELTMATERIALS_RSP,	PkgPlayerSmeltMaterialsRsp)
		HANDLE_PACKET(PKG_GAME_PLAYER_OPEN_POTENTIAL_RSP,	PkgPlayerOpenPotentialRsp)
		HANDLE_PACKET(PKG_GAME_PLAYER_SAY_NOT,				PkgPlayerSayNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_ENABLESKILL_NOT,		PkgPlayerEnableSkillNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_UPDATE_RANDSEED_NOT,	PkgPlayerUpdateRandSeedNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_PLAYANI_ACK,			PkgPlayerPlayAniAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_DIALOGDATA_ACK,		PkgPlayerDialogDataAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_DIFFICULTYLEVEL_RSP,	PkgPlayerDifficultyLevelRsp)
		HANDLE_PACKET(PKG_GAME_PLAYER_QUITSELCHR_RSP,		PkgPlayerQuitSelChrRsp)
		HANDLE_DEFAULT(_pPkt)
	END_HANDLE_PACKET(_pPkt)
}

//////////////////////////////////////////////////////////////////////////

void GameOtherPlayer::OnObjectAction(const PkgObjectActionNot& not)
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	bool bIsPlayerSelf = false;
	if(GetType() == OBJ_PLAYER)
	{
		bIsPlayerSelf = true;
	}
	bool bCanPlaySnd = false;
	if(abs((int)GetCoordX() - (int)pPlayer->GetCoordX()) <= 8 &&
		abs((int)GetCoordY() - (int)pPlayer->GetCoordY()) <= 8)
	{
		bCanPlaySnd = true;
	}

	if(not.uAction == ACTION_SPELL)
	{
		if(pPlayer->GetLockType() == PST_SK_LEIDIANSHU)
		{
			pPlayer->Unlock();
			pPlayer->UpdateLastAttackTime();
			m_dwLastAttackMode = 1;
		}
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
		if(HIWORD(not.uParam1) >= 0 &&
			HIWORD(not.uParam1) <= 7)
		{
			SetDirection((PLAYER_DIRECTION)HIWORD(not.uParam1));
		}
		if(not.uUserId == 0)
		{
			if(LOWORD(not.uParam1) == MEFF_HEAL)
			{
				PlayMagic(LOWORD(not.uParam1), this, LOWORD(not.uParam2), HIWORD(not.uParam2), not.uParam3);
			}
			else
			{
				PlayMagic(LOWORD(not.uParam1), NULL, LOWORD(not.uParam2), HIWORD(not.uParam2), not.uParam3);
			}
		}
		else
		{
			GameObject* pObj = static_cast<GameObject*>(SocketDataCenter::GetInstance().GetHandler(not.uUserId));
			if(NULL == pObj)
			{
				return;
			}
			else
			{
				PlayMagic(LOWORD(not.uParam1), pObj, 0, 0, not.uParam3);
			}
		}
	}
	else if(not.uAction == ACTION_ATTACK)
	{
		if(GetType() == OBJ_PLAYER)
		{
			if(pPlayer->IsUsingPreLock())
			{
				if(pPlayer->GetLockType() == PST_ATTACKWEAPON ||
					pPlayer->GetLockType() == PST_ATTACKNOWEAPON)
				{
					pPlayer->Unlock();
					pPlayer->UpdateLastAttackTime();
					m_dwLastAttackMode = 0;
				}

				if(not.uParam2 == MEFF_CISHA)
				{
					m_dwHumEffectFlag |= MMASK_CISHA;
				}
				else if(not.uParam2 == MEFF_GONGSHA)
				{
					m_dwHumEffectFlag |= MMASK_GONGSHA;
				}
				else if(not.uParam2 == MEFF_BANYUE)
				{
					m_dwHumEffectFlag |= MMASK_BANYEU;
				}
				else if(not.uParam2 == MEFF_LIEHUO)
				{
					m_dwHumEffectFlag |= MMASK_LIEHUO;
					m_nLieHuoSkillLevel = LOWORD(not.uParam3);
				}
				else if(not.uParam2 == MEFF_SLIEHUO)
				{
					m_dwHumEffectFlag |= MMASK_SLIEHUO;
				}
				else if(not.uParam2 == MEFF_KTSWORD)
				{
					m_dwHumEffectFlag |= MMASK_KTSWORD;
				}
			}
			else
			{
				//	just unlock server wait
				if(pPlayer->GetWaitServerResponseType() == WAITSERVERRSP_NORMALATK)
				{
					pPlayer->SetWaitServerResponse(0, 0);
				}
			}
		}
		else if(GetType() == OBJ_OTHERPLAYER)
		{
			//	just set the hit effect
			if(not.uParam2 == MEFF_CISHA)
			{
				m_dwHumEffectFlag |= MMASK_CISHA;
			}
			else if(not.uParam2 == MEFF_GONGSHA)
			{
				m_dwHumEffectFlag |= MMASK_GONGSHA;
			}
			else if(not.uParam2 == MEFF_BANYUE)
			{
				m_dwHumEffectFlag |= MMASK_BANYEU;
			}
			else if(not.uParam2 == MEFF_LIEHUO)
			{
				m_dwHumEffectFlag |= MMASK_LIEHUO;
				m_nLieHuoSkillLevel = LOWORD(not.uParam3);
			}
			else if(not.uParam2 == MEFF_SLIEHUO)
			{
				m_dwHumEffectFlag |= MMASK_SLIEHUO;
			}
			else if(not.uParam2 == MEFF_KTSWORD)
			{
				m_dwHumEffectFlag |= MMASK_KTSWORD;
			}
		}
	}
	else if(not.uAction == ACTION_STRUCK)
	{
		if(bCanPlaySnd)
		{
			if(GetAttrib()->sex == 2)
			{
				GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_FEMALESTRUCK);
			}
			else
			{
				GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_MALESTRUCK);
			}
		}
		//__super::DoPacket(not);
	}
	else if(not.uAction == ACTION_DEAD)
	{
		if(bCanPlaySnd)
		{
			if(GetAttrib()->sex == 2)
			{
				GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_FEMALEDEAD);
			}
			else
			{
				GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_MALEDEAD);
			}
		}
		//__super::DoPacket(not);
	}
	else
	{
		//__super::DoPacket(not);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerChangeEquipNot& not)
{
	if(GetType() == OBJ_OTHERPLAYER)
	{
		if(not.stItem.type == ITEM_WEAPON)
		{
			memcpy(GetPlayerItem(PLAYER_ITEM_WEAPON),
				&not.stItem, sizeof(ItemAttrib));
		}
		else if(not.stItem.type == ITEM_CLOTH)
		{
			memcpy(GetPlayerItem(PLAYER_ITEM_CLOTH),
				&not.stItem, sizeof(ItemAttrib));
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgChatNot& not)
{
	std::string xSay;
	if(not.uExtra == 0)
	{
		xSay += GetAttrib()->name;
		xSay += "说:";
		xSay += not.xMsg;
		SetSaying(not.xMsg.c_str());
	}
	DWORD dwColor = ARGB_WHITE;
	if(not.uExtra == 1)
	{
		dwColor = COLOR_SYSTEM;
		xSay = not.xMsg;
	}
	//GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(xSay.c_str(), dwColor);
	//GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertPublicChatMessage(GetAttrib()->name, not.xMsg.c_str());

	if(not.uExtra == 1)
	{
		//	system message
		GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(xSay.c_str(), dwColor);
	}
	else if(not.uExtra == 0)
	{
		//	chat message
		xSay = "[地图]";
		xSay += GetAttrib()->name;
		xSay += "说:";
		xSay += not.xMsg;
		SetSaying(not.xMsg.c_str());
		GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertPublicChatMessage(GetAttrib()->name, xSay.c_str(), ARGB_WHITE);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerSayNot& not)
{
	DWORD dwColor = ARGB_WHITE;
	if(not.uColor != 0)
	{
		dwColor = not.uColor;
	}

	std::string xSay;
	xSay = "[地图]";
	xSay += GetAttrib()->name;
	xSay += "说:";
	xSay += not.xMsg;
	SetSaying(not.xMsg.c_str());
	SetSayingColor(dwColor);
	GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertPublicChatMessage(GetAttrib()->name, xSay.c_str(), ARGB_WHITE);
}

void GameOtherPlayer::DoPacket(const PkgPlayerWorldSayNot& not)
{
	std::string xSay;
	xSay = "[世界]";
	xSay += GetAttrib()->name;
	xSay += "说:";
	xSay += not.xMsg;
	GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertPublicChatMessage(GetAttrib()->name, xSay.c_str(), 0xffFF00FF);
}

void GameOtherPlayer::DoPacket(const PkgPlayerGainItemNtf& ntf)
{
	if(GetType() == OBJ_PLAYER)
	{
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		pPlayer->GetPlayerBag()->AddItem(&ntf.stItem);

		if(ntf.stItem.type == ITEM_COST)
		{
			int nAttribID = ntf.stItem.id;

			for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
			{
				ItemList& assistItems = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList();

				if(assistItems[i].id == nAttribID)
				{
					GamePlayer::GetInstance()->UpdateAccelerate(i);
				}
			}
		}
		/*int nBagLeft = pPlayer->GetBagItemsEmptySum();
		int nAssisLeft = pPlayer->GetAssistItemsEmptySum();
		if(nBagLeft > 0)
		{
			pPlayer->GetPlayerBag()->AddItem(&ntf.stItem);
		}
		else
		{
			if(nAssisLeft > 0)
			{
				pPlayer->GetPlayerBag()->AddAssistItem(&ntf.stItem);
				PkgPlayerSyncAssistReq req;
				req.bNum = nAssisLeft - 1;
				req.uUserId = pPlayer->GetHandlerID();
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBuffer(&g_xBuffer);
			}
			else
			{
				AfxGetHge()->System_Log("Error: No more bag space!");
			}
		}*/
		GameInfoBoardDlg::GetInstance()->ShowItemMsg(&ntf.stItem);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerDropItemNot& not)
{
	//if(m_pObject->GetType() == OBJ_PLAYER ||
		//m_pObject->GetType() == OBJ_OTHERPLAYER)
	{
		//GamePlayer* pPlayer = (GamePlayer*)m_pObject;
		GroundItem item;
		item.bVisible = true;
		item.stAttrib.id = not.dwID;
		GameInfoManager::GetInstance()->GetItemAttrib(not.dwID, &item.stAttrib);
		item.stAttrib.tag = not.dwTag;
		item.wPosX = not.wCoordX;
		item.wPosY = not.wCoordY;

		if(!pTheGame->GetAssistPaneWnd()->CheckItemVisible(item.stAttrib.name) &&
			0 == GetItemUpgrade(item.stAttrib.level))
		{
			item.bVisible = false;
		}
		GameScene::sThis->InsertNewGroundItem(&item);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerLostItemAck& ack)
{
	if(GetType() == OBJ_PLAYER)
	{
		ItemAttrib* pItem = GamePlayer::GetInstance()->GetPlayerBag()->GetItemByTag(ack.dwTag);
		if (NULL == pItem)
		{
			for (int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
			{
				if (GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->tag == ack.dwTag)
				{
					pItem = GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i);
					break;
				}
			}
		}
		if(pItem)
		{
			if(pItem->type == ITEM_COST)
			{
				pItem->atkSpeed = 0;

				for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
				{
					if(GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList()[i].id == pItem->id)
					{
						GamePlayer::GetInstance()->UpdateAccelerate(i);
					}
				}
			}
			ZeroMemory(pItem, sizeof(ItemAttrib));
		}
	}
}

/*
void GameOtherPlayer::DoPacket(const PkgPlayerClearItemNtf& ntf)
{
	GameScene::sThis->RemoveGroundItem(ntf.dwTag);
}*/

void GameOtherPlayer::DoPacket(const PkgPlayerSyncAttribNtf& ntf)
{
	memcpy(GetAttrib(), &ntf.stAttrib, sizeof(ItemAttrib));
}

void GameOtherPlayer::DoPacket(const PkgPlayerUndressItemAck& ack)
{
	if(GetType() == OBJ_PLAYER)
	{
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		ItemAttrib* pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)ack.bPos);
		if(pItem->tag == ack.dwTag)
		{
			pPlayer->GetPlayerBag()->AddItem(pItem);
			ZeroMemory(pItem, sizeof(ItemAttrib));
		}
		pPlayer->UpdatePlayerAttrib();
		//pPlayer->UpdateHumState();
	}
	else
	{
		ItemAttrib* pItem = GetPlayerItem((PLAYER_ITEM_TYPE)ack.bPos);
		//if(pItem->tag == ack.dwTag)
		{
			ZeroMemory(pItem, sizeof(ItemAttrib));
			//UpdateHumState();
		}
	}

	RefleashHumState();
}

void GameOtherPlayer::DoPacket(const PkgPlayerDressItemAck& ack)
{
	if(0 != ack.bRet)
	{
		GameInfoBoardDlg::GetInstance()->InsertEquipErrorMsg(ack.bRet);
	}
	else
	{
		if(GetType() == OBJ_PLAYER)
		{
			//	self
			GamePlayer* pPlayer = (GamePlayer*)GamePlayer::GetInstance();
			ItemAttrib* pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)ack.bPos);
			ItemAttrib* pWantDressItem = pPlayer->GetPlayerBag()->GetItemByTag(ack.dwTag);
			if(pItem &&
				pWantDressItem)
			{
				if(pItem->type == ITEM_NO)
				{
					memcpy(pItem, pWantDressItem, sizeof(ItemAttrib));
					memset(pWantDressItem, 0, sizeof(ItemAttrib));
				}
				else
				{
					ItemAttrib item;
					memcpy(&item, pItem, sizeof(ItemAttrib));
					memcpy(pItem, pWantDressItem, sizeof(ItemAttrib));
					memcpy(pWantDressItem, &item, sizeof(ItemAttrib));
				}
			}
			pPlayer->UpdatePlayerAttrib();
			//pPlayer->UpdateHumState();
			pTheGame->GetGameScene()->GetMainOpt()->GetBagDlg2()->CancelSel();
		}
		else
		{
			//	other players
			ItemAttrib* pItem = GetPlayerItem((PLAYER_ITEM_TYPE)ack.bPos);
			if(pItem)
			{
				if (ack.bPos == PLAYER_ITEM_WEAPON)
				{
					pItem->type = ITEM_WEAPON;

					if ((ack.dwFlag & 0x1) != 0)
					{
						pItem->level = MakeItemUpgrade(0, 6);
					}
					else
					{
						pItem->level = 0;
					}
				}
				else if (ack.bPos == PLAYER_ITEM_CLOTH)
				{
					pItem->type = ITEM_CLOTH;
				}
				pItem->tex = ack.dwTex;
				pItem->id = ack.uUserId;

				//RefleashHumState();
			}
		}

		RefleashHumState();
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerUseItemAck& ack)
{
	if(GetType() == OBJ_PLAYER)
	{
		//	self
		bool bInAssist = false;
		GamePlayer* pPlayer = (GamePlayer*)GamePlayer::GetInstance();
		ItemAttrib* pItem = pPlayer->GetPlayerBag()->GetItemByTag(ack.dwTag);
		if(pItem == NULL)
		{
			pItem = pPlayer->GetPlayerBag()->GetAssitItemByTag(ack.dwTag);
			if(pItem)
			{
				bInAssist = true;
			}
		}

		/*switch(ack.dwUsage)
		{
		case USE_RANDOM_POS:
			{
				pPlayer->SetReallyCoord(LOWORD(ack.dwParam0), HIWORD(ack.dwParam0));
				pPlayer->SetCurFrame(0);
				pPlayer->SetStatus(PST_STAND);
			}break;
		}

		if(pItem)
		{
			if(pPlayer->IsCostItem(*pItem))
			{
				if(bInAssist)
				{
					int nUseID = pItem->id;
					ZeroMemory(pItem, sizeof(ItemAttrib));
					pPlayer->GetPlayerBag()->MoveBagItemToAssistItem(nUseID);
				}

				if(!bInAssist)
				{
					memset(pItem, 0, sizeof(ItemAttrib));
				}
			}
			else
			{
				memset(pItem, 0, sizeof(ItemAttrib));
			}
		}*/

		if(pItem)
		{
			if(pPlayer->IsCostItem(*pItem))
			{
				return;

				if(bInAssist)
				{
					int nUseID = pItem->id;
					ZeroMemory(pItem, sizeof(ItemAttrib));
					pPlayer->GetPlayerBag()->MoveBagItemToAssistItem(nUseID);
				}

				if(!bInAssist)
				{
					memset(pItem, 0, sizeof(ItemAttrib));
				}
			}
			else
			{
				memset(pItem, 0, sizeof(ItemAttrib));
			}
		}

		/*PkgPlayerSyncAssistReq req;
		req.uUserId = pPlayer->GetHandlerID();
		req.bNum = pPlayer->GetAssistItemsEmptySum();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBuffer(&g_xBuffer);*/
	}
	else
	{
		//	other players
		/*GameOtherPlayer* pPlayer = (GameOtherPlayer*)m_pObject;
		pPlayer->SetReallyCoord(LOWORD(ack.dwParam0), HIWORD(ack.dwParam0));
		pPlayer->SetCurFrame(0);
		pPlayer->SetStatus(PST_STAND);*/
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerUpdateAttribNtf& ntf)
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	//switch(LOWORD(ntf.bType))
	switch(ntf.bType)
	{
	case UPDATE_HP:
		{
			int nCurHP = GetAttrib()->HP;
			GetAttrib()->HP = (ntf.dwParam);
			if(ntf.dwExtra != 0)
			{
				GetAttrib()->maxHP = ntf.dwExtra;
			}

			int nHPChange = GetAttrib()->HP - nCurHP;
			//if(nHPChange < 0)
			{
				GameScene::sThis->InsertNewAttackNumber(abs(nHPChange), nHPChange > 0, this);
			}
		}break;
	case UPDATE_MP:
		{
			GetAttrib()->MP = (ntf.dwParam);
		}break;
	case UPDATE_EXP:
		{
			char szMsg[50];
			sprintf(szMsg, "[提示]获得 %d 经验值",
				ntf.dwParam - GetAttrib()->EXPR);
			GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(szMsg, ARGB_RED);
			GetAttrib()->EXPR = ntf.dwParam;
		}break;
	case UPDATE_MONEY:
		{
			if(GetType() == OBJ_PLAYER)
			{
				int nMoney = GamePlayer::GetInstance()->GetPlayerBag()->GetMoney();
				GamePlayer::GetInstance()->GetPlayerBag()->SetMoney((int)ntf.dwParam);

				nMoney = (int)ntf.dwParam - nMoney;
				if(nMoney > 0)
				{
					GameInfoBoardDlg::GetInstance()->InsertBoardMsg("发现 %d 金币", nMoney);
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_FETCHMONEY);
				}
			}
		}break;
	case UPDATE_LEVEL:
		{
			if(GetType() == OBJ_PLAYER)
			{
				pPlayer->GetAttrib()->level = ntf.dwParam;
				pPlayer->GetAttrib()->EXPR = ntf.dwExtra;
				pPlayer->GetAttrib()->HP = pPlayer->GetAttrib()->maxHP = g_nHPTable[pPlayer->GetAttrib()->level - 1][pPlayer->GetHeroJob()];
				pPlayer->GetAttrib()->MP = pPlayer->GetAttrib()->maxMP = g_nMPTable[pPlayer->GetAttrib()->level - 1][pPlayer->GetHeroJob()];
				pPlayer->GetAttrib()->maxEXPR = g_nExprTable[pPlayer->GetAttrib()->level - 1];
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg("升级了!");
			}
		}break;
	case UPDATE_MAGIC:
		{
			if(GetType() == OBJ_PLAYER)
			{
				if(pPlayer->IsMagicExist(ntf.dwParam))
				{
					pPlayer->SetMagicLevel(ntf.dwParam, ntf.dwExtra);
				}
				else
				{
					pPlayer->AddToMagicTree(ntf.dwParam);
					pPlayer->SetMagicLevel(ntf.dwParam, ntf.dwExtra);
				}
				GamePlayer::GetInstance()->UpdatePlayerAttrib();
			}
		}break;
	case UPDATE_DONATELFET:
		{
			if(GetType() == OBJ_PLAYER)
			{
				GamePlayer::GetInstance()->SetDonateLeft(ntf.dwParam);
			}
		}break;
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerForceActionAck& ntf)
{
	if(ntf.bType == FORCE_POSITION)
	{
		SetReallyCoord(LOWORD(ntf.dwData), HIWORD(ntf.dwData));
		SetStatus(PST_STAND);
		SetCurFrame(0);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerClearBagNtf& ntf)
{
	if(GetType() == OBJ_PLAYER)
	{
		GamePlayer* pPlayer = (GamePlayer*)GamePlayer::GetInstance();
		ItemAttrib* pItem = pPlayer->GetPlayerBag()->GetItemByTag(ntf.dwTag);
		if(pItem)
		{
			if(pItem->type == ITEM_COST)
			{
				for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
				{
					if(GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList()[i].id == pItem->id)
					{
						GamePlayer::GetInstance()->UpdateAccelerate(i);
					}
				}
			}
			ZeroMemory(pItem, sizeof(ItemAttrib));
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerChangeMapAck& ack)
{
	if(GetType() == OBJ_PLAYER)
	{
		GamePlayer* pPlayer = (GamePlayer*)GamePlayer::GetInstance();
		pPlayer->SetReallyCoord(ack.wPosX, ack.wPosY);
		pPlayer->SetStatus(PST_STAND);
		pPlayer->SetCurFrame(0);
		GameScene::sThis->SwitchScene(ack.wMapID);
		pPlayer->SetWalkLocked();
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerShowDoorAniAck& ack)
{
	if(GetType() == OBJ_PLAYER)
	{
		MagicDoor* pDoorAni = new MagicDoor;
		int nLastTime = ack.uUserId;

		if(pDoorAni->Create(MEFF_DOOR, NULL, NULL, ack.wPosX * UNIT_WIDTH, ack.wPosY * UNIT_HEIGHT, nLastTime))
		{
			GameScene::sThis->InsertNewMagic(pDoorAni);
		}
		else
		{
			delete pDoorAni;
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerGetBaleItemAck& ack)
{
	/*ItemAttrib item;
	item.id = ack.dwID;
	int nFailCounter = 0;

	if(GameInfoManager::GetInstance()->GetItemAttrib(ack.dwID, &item))
	{
		for(int i = 0; i < 6; ++i)
		{
			item.tag = ack.dwTagbeg + i;
			if(GamePlayer::GetInstance()->GetPlayerBag()->AddItem(&item))
			{
				GameInfoBoardDlg::GetInstance()->ShowItemMsg(&item);
			}
			else
			{
				++nFailCounter;
			}
		}

		for(int i = 0; i < nFailCounter; ++i)
		{
			if(GamePlayer::GetInstance()->GetPlayerBag()->AddAssistItem(&item))
			{
				GameInfoBoardDlg::GetInstance()->ShowItemMsg(&item);
			}
		}
	}
	PkgPlayerSyncAssistReq req;
	req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
	req.bNum = GamePlayer::GetInstance()->GetAssistItemsEmptySum();
	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBuffer(&g_xBuffer);*/
}

void GameOtherPlayer::DoPacket(const PkgPlayerReviveNot& not)
{
	if(not.bMode != 0)
	{
		SetStatus(PST_STAND);
		m_attrib.HP = not.uHp;
		GameScene::sThis->GetMainOpt()->GetStaticDlg()->SetVisible(false);
		m_bHurt = false;
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerHideQuestDlgAck& ack)
{
	GameScene::sThis->GetMainOpt()->GetActiveDlg()->SetVisible(false);
}

void GameOtherPlayer::DoPacket(const PkgPlayerSetEffectAck& ack)
{
	if(ack.bInt == 0)
	{
		if(!ack.bShow)
		{
			if(ack.dwMgcID == MMASK_SHIELD)
			{
				RESET_FLAG(m_dwHumEffectFlag, MMASK_SHIELD);
				m_dwHumEffectTime[MMASK_C_SHEILD_INDEX] = 0;
			}
			else if(ack.dwMgcID == MMASK_HIDE)
			{
				RESET_FLAG(m_dwHumEffectFlag, MMASK_HIDE);
				m_dwHumEffectTime[MMASK_C_HIDE_INDEX] = 0;
			}
			else if(ack.dwMgcID == MMASK_STONE)
			{
				RESET_FLAG(m_dwHumEffectFlag, MMASK_STONE);
				m_dwHumEffectTime[MMASK_C_STONE_INDEX] = 0;
			}
			else if(ack.dwMgcID == MMASK_LVDU)
			{
				RESET_FLAG(m_dwHumEffectFlag, MMASK_LVDU);
				m_dwHumEffectTime[MMASK_C_LVDU_INDEX] = 0;
			}
		}
		else
		{
			if(ack.dwMgcID == MMASK_LVDU)
			{
				SetHumEffect(MMASK_LVDU, ack.dwTime);
			}
			else if(ack.dwMgcID == MMASK_STONE)
			{
				SetHumEffect(MMASK_STONE, ack.dwTime);
//				SetReallyCoord(LOWORD(ack.dwPos), HIWORD(ack.dwPos));
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameOtherPlayer::DoPacket(const PkgPlayerStruckAck& ack)
{
	if(GetStatus() != PST_ATTACKED)
	{
		SetHurt();
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerDialogDataAck& ack)
{
#define MAX_SHOP_BUFFER_SIZE	(8 * 1024)
	if(GetType() == OBJ_PLAYER)
	{
		static char* s_pBuffer = new char[MAX_SHOP_BUFFER_SIZE];
		//char szBuf[MAX_STORE_NUMBER * sizeof(ItemAttrib)];
		const char* pData = &ack.xData[0];
		uLongf srclen = (uLongf)ack.xData.size();
		//uLongf buflen = (uLongf)sizeof(szBuf);
		uLongf buflen = (uLongf)MAX_SHOP_BUFFER_SIZE;

		int nRet = uncompress((Bytef*)s_pBuffer, &buflen, (const Bytef*)pData, srclen);
		if(nRet == Z_OK)
		{
			g_xBuffer.Reset();
			g_xBuffer.Write(s_pBuffer, buflen);

			BYTE bSize = 0;
			g_xBuffer >> bSize;

			//GameStoreDlg* pStoreDlg = GameScene::sThis->GetMainOpt()->GetStoreDlg();
			GameBigStoreDlg* pStoreDlg = GameScene::sThis->GetMainOpt()->GetBigStoreDlg();
			GameBagDlg2* pBagDlg = GameScene::sThis->GetMainOpt()->GetBagDlg2();
			pStoreDlg->ClearItem();

			if(bSize != 0)
			{
				ItemAttrib item;

				for(int i = 0; i < bSize; ++i)
				{
					g_xBuffer >> item;
					pStoreDlg->AddItem(&item);
				}
			}
			//pStoreDlg->SetVisible(true);
			pStoreDlg->ShowDialog();
			pStoreDlg->MoveTo(0, 0);

			int nPosX = WINDOW_WIDTH - pBagDlg->GetWidth();
			pBagDlg->MoveTo(nPosX, 0);
			pBagDlg->ShowDialog();
		}
		else
		{
			AfxGetHge()->System_Log("无法解压缩仓库数据");
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerStoreDataAck& ack)
{
	if(GetType() == OBJ_PLAYER)
	{
		char szBuf[MAX_STORE_NUMBER * sizeof(ItemAttrib)];
		const char* pData = &ack.xData[0];
		uLongf srclen = (uLongf)ack.xData.size();
		uLongf buflen = (uLongf)sizeof(szBuf);

		int nRet = uncompress((Bytef*)szBuf, &buflen, (const Bytef*)pData, srclen);
		if(nRet == Z_OK)
		{
			g_xBuffer.Reset();
			g_xBuffer.Write(szBuf, buflen);

			BYTE bSize = 0;
			g_xBuffer >> bSize;

			//GameStoreDlg* pStoreDlg = GameScene::sThis->GetMainOpt()->GetStoreDlg();
			GameStoreDlg2* pStoreDlg = GameScene::sThis->GetMainOpt()->GetStoreDlg2();
			GameBagDlg2* pBagDlg = GameScene::sThis->GetMainOpt()->GetBagDlg2();
			pStoreDlg->ClearItem();

			if(bSize != 0)
			{
				ItemAttrib item;

				for(int i = 0; i < bSize; ++i)
				{
					g_xBuffer >> item;
					pStoreDlg->AddItem(&item);
				}
			}
			//pStoreDlg->SetVisible(true);
			pStoreDlg->ShowDialog();
			pStoreDlg->MoveTo(0, 0);

			int nPosX = WINDOW_WIDTH - pBagDlg->GetWidth();
			pBagDlg->MoveTo(nPosX, 0);
			pBagDlg->ShowDialog();
		}
		else
		{
			AfxGetHge()->System_Log("无法解压缩仓库数据");
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerStoreSyncAck& ack)
{
	//GameStoreDlg* pStoreDlg = GameScene::sThis->GetMainOpt()->GetStoreDlg();
	GameStoreDlg2* pStoreDlg = GameScene::sThis->GetMainOpt()->GetStoreDlg2();
	GameBigStoreDlg* pBigStoreDlg = GameMainOptUI::GetInstance()->GetBigStoreDlg();
	bool bVisible = pStoreDlg->IsVisible();
	ItemAttrib* pItem = NULL;
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	if(GetType() != OBJ_PLAYER)
	{
		return;
	}

	if(ack.bType == STORESYNC_ADDSTORE)
	{
		//	添加装备到仓库
		pItem = pPlayer->GetPlayerBag()->GetItemByTag(ack.dwTag);
		if(pItem)
		{
			pStoreDlg->AddItem(pItem);
			pItem->type = ITEM_NO;
			pItem->tag = 0;
		}
	}
	else if(ack.bType == STORESYNC_REMOVESTORE)
	{
		//	添加仓库装备到背包
		pItem = pStoreDlg->GetItem(ack.dwTag);
		if(pItem)
		{
			pPlayer->GetPlayerBag()->AddItem(pItem);
			GameInfoBoardDlg::GetInstance()->ShowItemMsg(pItem);
			pItem->type = ITEM_NO;
			pItem->tag = 0;
		}
	}
	else if(ack.bType == STORESYNC_BIG_ADDSTORE)
	{
		//	添加装备到大仓库
		pItem = pPlayer->GetPlayerBag()->GetItemByTag(ack.dwTag);
		if(pItem)
		{
			pBigStoreDlg->AddItem(pItem);
			pItem->type = ITEM_NO;
			pItem->tag = 0;
		}
	}
	else if(ack.bType == STORESYNC_BIG_REMOVESTORE)
	{
		//	添加大仓库装备到背包
		pItem = pBigStoreDlg->GetItem(ack.dwTag);
		if(pItem)
		{
			pPlayer->GetPlayerBag()->AddItem(pItem);
			GameInfoBoardDlg::GetInstance()->ShowItemMsg(pItem);
			pItem->type = ITEM_NO;
			pItem->tag = 0;
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerShowHelpAck& ack)
{
	GameScene::sThis->GetMainOpt()->GetHelpDlg()->CreateHelpDlg(ack.nHelpID);
}

void GameOtherPlayer::DoPacket(const PkgPlayerShowDlgAck& ack)
{
	if(ack.bType == DLG_REVIVE)
	{
		GameScene::sThis->GetMainOpt()->GetStaticDlg()->CreateDeadDlg();
	}
	else if(ack.bType == DLG_CUBE)
	{
		GameScene::sThis->GetMainOpt()->GetCubeDlg()->ShowDialog();
	}
	else if(ack.bType == DLG_WORLDCHAT)
	{
		//	世界聊天栏
		GameLineEdit* pEdit = GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit();
		pEdit->GainFocus();
		pEdit->ClearText();
		pEdit->SetText("~");
		pEdit->SetVisible(true);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerNetDelayAck& ack)
{
	if(GetType() == OBJ_PLAYER)
	{
		GameScene::sThis->OnReceiveNetDelayReply(ack);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerUpdateCostNtf& ntf)
{
	if(GetType() == OBJ_PLAYER)
	{
		ItemAttrib* pItem = GamePlayer::GetInstance()->GetPlayerBag()->GetItemByTag(ntf.dwTag);
		if(NULL != pItem)
		{
			int nAttribID = pItem->id;

			if(ntf.nNumber > pItem->atkSpeed)
			{
				GameInfoBoardDlg::GetInstance()->ShowItemMsg(pItem);
			}
			else
			{
				//	Update cool down status
				if(pItem->hide != 0 &&
					ntf.bSelfUse)
				{
					GamePlayer::GetInstance()->GetDrugCoolDownController()->PushItem(nAttribID, pItem->hide * 1000);
				}
			}

			pItem->atkSpeed = ntf.nNumber;
			if(ntf.bBind)
			{
				SET_FLAG(pItem->atkPois, POIS_MASK_BIND);
			}

			for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
			{
				ItemList& assistItems = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList();

				if(assistItems[i].id == nAttribID)
				{
					GamePlayer::GetInstance()->UpdateAccelerate(i);
				}
			}

			if(ntf.nNumber == 0)
			{
				ZeroMemory(pItem, sizeof(ItemAttrib));
			}
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerClearAllItemAck& ack)
{
	if(GetType() == OBJ_PLAYER)
	{
		ZeroMemory(m_equip, sizeof(m_equip));

		GamePlayer::GetInstance()->GetPlayerBag()->Clear();
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerUpdateItemNtf& ntf)
{
	if(GetType() == OBJ_PLAYER)
	{
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		ItemAttrib* pItem = pPlayer->GetPlayerBag()->GetItemByTag(ntf.stItem.tag);
		if(NULL == pItem)
		{
			for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
			{
				if(pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)i)->tag == ntf.stItem.tag)
				{
					pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)i);
					break;
				}
			}
		}

		if(NULL != pItem)
		{
			ItemAttrib prevItem = *pItem;
			memcpy(pItem, &ntf.stItem, sizeof(ItemAttrib));
			GameMainOptUI::GetInstance()->GetDisplayDlg()->ResetItemAttirb();

			if (pTheGame->IsEquipItem(prevItem))
			{
				WORD wPrevDuraMax = LOWORD(prevItem.maxHP);
				WORD wPrevDura = HIWORD(prevItem.maxHP);
				WORD wCurDuraMax = LOWORD(pItem->maxHP);
				WORD wCurDura = HIWORD(pItem->maxHP);

				if (wCurDura != wPrevDura ||
					wPrevDuraMax != wCurDuraMax)
				{
					// durability changed
					if (wCurDura < wCurDuraMax / 4 ||
						wCurDuraMax <= 2)
					{
						GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "[%s]持久过低", pItem->name);
					}
				}
			}
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerQuickMsgNtf& ntf)
{
	char szBuf[MAX_PATH];

	if(GetType() == OBJ_PLAYER)
	{
		switch(ntf.nMsgID)
		{
		case QUICKMSG_MULTI:
			{
				float fDropMulti = (float)LOWORD(ntf.nParam) / 10;
				float fExprMulti = (float)HIWORD(ntf.nParam) / 10;
				if(0 != fDropMulti ||
					0 != fExprMulti)
				{
					sprintf(szBuf, "[公告]当前服务器爆率为%.1f倍，经验为%.1f倍",
						fDropMulti, fExprMulti);
					GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(szBuf,ARGB_YELLOW);
				}
			}break;
		case QMSG_CANNOTTRANSFER:
			{
				GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[提示]该地图无法传送", COLOR_SYSTEM);
			}break;
		case QMSG_TRANSFERINTERVAL:
			{
				sprintf(szBuf, "[提示]传送时间间隔限制: %d 秒",
					ntf.nParam);
				GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(szBuf,COLOR_SYSTEM);
			}break;
		case QMSG_REVIVEINTERVAL:
			{
				sprintf(szBuf, "[提示]原地复活时间间隔: %d 分",
					ntf.nParam);
				GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(szBuf,COLOR_SYSTEM);
			}break;
		case QMSG_DOUBLEDROPINFO:
			{
				sprintf(szBuf, "[提示]获得双倍祝福，效果持续: %d 分",
					ntf.nParam);
				GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(szBuf,COLOR_SYSTEM);
			}break;
		case QMSG_CANNOTSELLQUEST:
			{
				GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[提示]无法出售任务物品", COLOR_SYSTEM);
			}break;
		case QMSG_OFFSELLOK:
			{
				if(ntf.nParam == 0)
				{
					GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[提示]寄售失败！可能是已达寄售物品上限（8个）", COLOR_SYSTEM);
				}
				else
				{
					GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[提示]寄售成功！", COLOR_SYSTEM);
					if(GameMainOptUI::GetInstance()->GetOfflineSellDlg()->IsVisible())
					{
						GameMainOptUI::GetInstance()->GetOfflineSellDlg()->OnShowDialog();
					}
				}
			}break;
		case QMSG_NOBAGROOM:
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "没有足够的背包空间");
			}break;
		case QMSG_NOENOUGHMONEY:
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "没有足够的金币");
			}break;
		case QMSG_OFFNOTSOLD:
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "您没有寄售出任何物品");
			}break;
		case QMSG_NOIDENTIFYSCROLL:
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "没有足够的辨识卷轴");
			}break;
		case QMSG_IDENTIFYFULL:
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "辨识属性已满");
			}break;
		case QMSG_IDENTIFYOK:
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "辨识属性成功!");
			}break;
		case QMSG_UNBINDOK:
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "解绑物品成功!");
			}break;
		case QMSG_UNBINDFAILED:
			{
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "解绑物品失败!");
			}break;
		case QMSG_NOENOUGHITEM:
			{
				ItemAttrib item = {0};
				if(GameInfoManager::GetInstance()->GetItemAttrib(ntf.nParam, &item))
				{
					GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "您没有足够的[%s]!", item.name);
				}
			}break;
		case QMSG_SKILLLEVELLOW:
			{
				WORD wMgcID = LOWORD(ntf.nParam);
				WORD wNeedLevel = HIWORD(ntf.nParam);

				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "你要学习的技能[%s]需要等级[%d]", GetMagicNameByID(wMgcID), wNeedLevel);
			}break;
		case QMSG_DIFFICULTYLEVEL:
			{
				char szMsg[MAX_PATH];
				sprintf(szMsg, "[提示]当前游戏难度为：%s", pTheGame->GetDifficultyLevelName(ntf.nParam));
				GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(szMsg, ARGB_YELLOW);
				pTheGame->UpdateWindowTitle(ntf.nParam);
			}break;
		case QMSG_ATTACKMODE:
			{
				static const char* s_szAttackModeName[] =
				{
					"[提示]当前模式 ：和平模式",
					"[提示]当前模式 ：杀戮模式",
					"[提示]当前模式 ：组队模式",
					"[提示]当前模式 ：行会模式"
				};

				if(ntf.nParam < 0 ||
					ntf.nParam >= kHeroPkType_Total)
				{
					return;
				}

				GameMainOptUI::GetInstance()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(s_szAttackModeName[ntf.nParam], 0xff63B8FF);
			}break;
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerVersionVerifyNtf& ntf)
{
	if(ntf.xVersion != BACKMIR_CURVERSION)
	{
		std::string xMsg;
		xMsg = "服务器版本:" + ntf.xVersion;
		xMsg += " 客户端版本:";
		xMsg += BACKMIR_CURVERSION;

#ifdef _DEBUG
#else
		::MessageBox(NULL, xMsg.c_str(), "Error", MB_ICONERROR | MB_TASKMODAL);
		::PostQuitMessage(0);
#endif
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerQuestDataNtf& ntf)
{
	if(GetType() == OBJ_PLAYER)
	{
		char szBuf[500];
		const char* pData = &ntf.xData[0];
		uLongf srclen = (uLongf)ntf.xData.size();
		uLongf buflen = (uLongf)sizeof(szBuf);

		int nRet = uncompress((Bytef*)szBuf, &buflen, (const Bytef*)pData, srclen);
		if(nRet == Z_OK)
		{
			g_xBuffer.Reset();
			g_xBuffer.Write(szBuf, buflen);
#ifdef _DEBUG
			//AfxGetHge()->System_Log(g_xBuffer.ToHexString().c_str());
#endif
			g_xBuffer >> *GamePlayer::GetInstance()->GetQuest();
		}
		else
		{
			AfxGetHge()->System_Log("无法解压缩任务数据");
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerSyncQuestNtf& ntf)
{
	if(ntf.bStage < 0 ||
		ntf.bStage >= MAX_QUEST_NUMBER)
	{
		return;
	}
	if(GetType() != OBJ_PLAYER)
	{
		return;
	}

	QuestContext* pQuest = GamePlayer::GetInstance()->GetQuest();
	pQuest->SetQuestStep(ntf.bStage, ntf.bStep);
	pQuest->SetQuestCounter(ntf.bStage, ntf.bCounter);

	GameQuestDlg* pQuestDlg = GameScene::sThis->GetMainOpt()->GetQuestDlg();
	if(pQuestDlg->IsVisible())
	{
		pQuestDlg->ShowDialog();
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerOtherPlayerEquipNtf& ntf)
{
	if(GetType() == OBJ_PLAYER)
	{
#define MAX_DATA_SIZE 1024*2
		char* pBuf = new char[MAX_DATA_SIZE];
		//char szBuf[500];
		const char* pData = &ntf.xData[0];
		uLongf srclen = (uLongf)ntf.xData.size();
		uLongf buflen = (uLongf)MAX_DATA_SIZE;

		int nRet = uncompress((Bytef*)pBuf, &buflen, (const Bytef*)pData, srclen);
		if(nRet == Z_OK)
		{
			g_xBuffer.Reset();
			g_xBuffer.Write(pBuf, buflen);
#ifdef _DEBUG
			//AfxGetHge()->System_Log(g_xBuffer.ToHexString().c_str());
#endif
			//	name
			std::string xName;
			BYTE bSex = 0;
			ItemAttrib stItem;
			ZeroMemory(&stItem, sizeof(ItemAttrib));

			g_xBuffer >> xName
				>> bSex;

			GameOtherHumDlg* pDlg = GameMainOptUI::GetInstance()->GetOtherHumDlg();
			pDlg->ClearItems();
			pDlg->SetWindowInfo(xName.c_str());
			pDlg->SetSex(bSex);

			GameDisplayDlg* pDisDlg = GameMainOptUI::GetInstance()->GetDisplayDlg();
			pDisDlg->ClearItems();

			for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
			{
				g_xBuffer >> stItem;
				pDlg->SetItem(&stItem, i);
				pDisDlg->SetOwnerEquip(&stItem, i);
			}

			pDlg->ShowDialog();
		}
		else
		{
			AfxGetHge()->System_Log("无法解压缩玩家装备数据");
		}
		SAFE_DELETE_ARRAY(pBuf);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerGStatusNtf& ntf)
{
	if(GetType() != OBJ_PLAYER &&
		GetType() != OBJ_OTHERPLAYER)
	{
		return;
	}
	bool bNeedDlgShow = false;
	if(GetType() == OBJ_PLAYER)
	{
		bNeedDlgShow = true;
	}

	if(ntf.xStatus.size() != ntf.xTimes.size())
	{
		return;
	}

	GameStatusDlg* pDlg = GameScene::sThis->GetStatusDlg();

	for(int i = 0; i < ntf.xStatus.size(); ++i)
	{
		int nStatusID = ntf.xStatus[i];
		DWORD dwTime = ntf.xTimes[i];
		//pDlg->AddStatus(nStatusID, dwTime);

		if(nStatusID == GSTATUS_ENERGYSHIELD)
		{
			SetHumEffect(MMASK_ENERGYSHIELD, dwTime/* + dwCurTick*/);
			if(bNeedDlgShow)
			{
				pDlg->AddStatus(nStatusID, dwTime);
			}
		}
		else if(nStatusID == GSTATUS_POISON)
		{
			SetHumEffect(MMASK_LVDU, dwTime);
			if(bNeedDlgShow)
			{
				pDlg->AddStatus(nStatusID, dwTime);
			}
		}
		else if(nStatusID == GSTATUS_SHIELD)
		{
			SetHumEffect(MMASK_SHIELD, dwTime);
		}
		else if(nStatusID == GSTATUS_JINGANG)
		{
			m_dwJinGangExpireTime = GetTickCount() + dwTime;
			if(bNeedDlgShow)
			{
				pDlg->AddStatus(nStatusID, dwTime);
			}
		}
		else
		{
			if(bNeedDlgShow)
			{
				pDlg->AddStatus(nStatusID, dwTime);
			}
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerPlaySoundNtf& ntf)
{
	/*if(ntf.bType == PLAYSOUND_OBJECTSOUND)
	{
		WORD wObj = LOWORD(ntf.dwData);
		WORD wIdx = HIWORD(ntf.dwData);

		const RenderInfo* pRenderInfo = NULL;
		if(GameInfoManager::GetInstance()->GetMonsterRenderInfo(wObj, &pRenderInfo))
		{
			if(NULL != pRenderInfo)
			{
				int nIndex = 0;
				nIndex = pRenderInfo->sndindex;
				if(0 != nIndex)
				{
					GameSoundManager::GetInstancePtr()
				}
			}
		}
	}*/
}

void GameOtherPlayer::DoPacket(const PkgPlayerOffSoldItemAck& ack)
{
	char szMsg[MAX_PATH];
	sprintf(szMsg, "您寄售的 [%s] 已被玩家 [%s] 购买，获得收入 %d",
		ack.xItemName.c_str(), ack.xBuyerName.c_str(), ack.dwMoney);
	GameMainOptUI::GetInstance()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(szMsg, ARGB_YELLOW);
}

void GameOtherPlayer::DoPacket(const PkgPlayerOffSellListNtf& ntf)
{
	OfflineSellListDlg* pOffSellListDlg = GameMainOptUI::GetInstance()->GetOfflineSellListDlg();
	if(ntf.bPage == 0)
	{
		//	clear
		pOffSellListDlg->ResetItems();
	}

	//pOffSellListDlg->AddItems(ntf.xItems);
	for(int i = 0; i < ntf.xItems.size(); ++i)
	{
		pOffSellListDlg->AddItem(ntf.xItems[i]);
	}
	pOffSellListDlg->FilterItems(0);
}

void GameOtherPlayer::DoPacket(const PkgPlayerUIDNtf& ntf)
{
	if(GetType() == OBJ_PLAYER)
	{
		GamePlayer::GetInstance()->SetUID(ntf.uUID);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerUpdateWeaponGrowNtf& ntf)
{
	ItemAttrib* pItem = GetPlayerItem(PLAYER_ITEM_WEAPON);
	if(NULL != pItem &&
		pItem->tag == ntf.dwTag &&
		pItem->type == ITEM_WEAPON)
	{
		pItem->HP = ntf.dwData;

		if(GetType() == OBJ_PLAYER)
		{
			GamePlayer::GetInstance()->UpdatePlayerAttrib();
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_YELLOW, "恭喜! 您的武器已成长");
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerPrivateChatNtf& ntf)
{
	if(GetType() == OBJ_PLAYER)
	{
		char szMsg[MAX_PATH];
		sprintf(szMsg, "[私聊]%s => %s",
			ntf.xSender.c_str(),
			ntf.xMsg.c_str());
		GameMainOptUI::GetInstance()->GetChatDlg()->GetHistoryDlg()->InsertPrivateChatMessage(ntf.xSender.c_str(), szMsg);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerPrivateChatAck& ack)
{
	if(GetType() == OBJ_PLAYER)
	{
		char szMsg[MAX_PATH];
		sprintf(szMsg, "[提示]%s 不在线",
			ack.xPeerName.c_str());
		GameMainOptUI::GetInstance()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(szMsg, 0xffFF0000);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerServerDelayReq& req)
{
	if(GetType() != OBJ_PLAYER)
	{
		return;
	}

	PkgPlayerServerDelayAck ack;
	ack.nSeq = req.nSeq;
	ack.dwTimeStamp = req.dwTimeStamp;

	g_xBuffer.Reset();
	g_xBuffer << ack;
	SendBufferToGS(&g_xBuffer);
}

void GameOtherPlayer::DoPacket(const PkgPlayerUpdateRandSeedNot &not)
{
	if(GetType() != OBJ_PLAYER)
	{
		return;
	}

	if(RANDSEED_NORMALATK == not.nType)
	{
		GamePlayer::GetInstance()->SetRandSeedNormalAtk(not.dwSeed);
#ifdef _DEBUG
		GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_WHITE, "同步攻击种子：%d", not.dwSeed);
#endif
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerRankListNot& not)
{
	if(GetType() != OBJ_PLAYER)
	{
		return;
	}

	if(not.xData.empty())
	{
		return;
	}

	GameRankDlg* pRankDlg = GameMainOptUI::GetInstance()->GetRankDlg();

	//	解析json数组
	cJSON* pRoot = cJSON_Parse(not.xData.c_str());
	if(NULL == pRoot)
	{
		return;
	}

	static const char* s_pszRankNodeName[] = {
		"rank_level",
		"rank_zhanshi",
		"rank_fashi",
		"rank_daoshi"
	};

	do 
	{
		for(int i = 0; i < sizeof(s_pszRankNodeName) / sizeof(s_pszRankNodeName[0]); ++i)
		{
			const char* pNodeName = s_pszRankNodeName[i];
			cJSON* pRankLevel = cJSON_GetObjectItem(pRoot, pNodeName);

			if(NULL != pRankLevel)
			{
				if(pRankLevel->type == cJSON_NULL)
				{
					continue;
				}

				int nArraySize = cJSON_GetArraySize(pRankLevel);
				if(0 != nArraySize)
				{
					//	清空对应排行榜数据
					pRankDlg->ClearRankItem(i);

					cJSON* pArrayItem = pRankLevel->child;
					while(NULL != pArrayItem)
					{
						RankItem item = {0};

						cJSON* pNode = cJSON_GetObjectItem(pArrayItem, "uid");
						if(NULL != pNode)
						{
							item.nUid = pNode->valueint;
						}
						pNode = cJSON_GetObjectItem(pArrayItem, "name");
						if(NULL != pNode &&
							pNode->type == cJSON_String &&
							NULL != pNode->valuestring)
						{
							//	从utf转gbk
							string xGbk = UTF8ToGBK(pNode->valuestring);
							strcpy(item.szName, xGbk.c_str());
						}
						pNode = cJSON_GetObjectItem(pArrayItem, "level");
						if(NULL != pNode)
						{
							item.nLevel = pNode->valueint;
						}
						pNode = cJSON_GetObjectItem(pArrayItem, "job");
						if(NULL != pNode)
						{
							item.nJob = pNode->valueint;
						}
						pNode = cJSON_GetObjectItem(pArrayItem, "power");
						if(NULL != pNode)
						{
							item.nPower = pNode->valueint;
						}

						//	insert
						pRankDlg->AddRankItem(i, &item);

						//	next item
						pArrayItem = pArrayItem->next;
					}
				}
			}
		}
	} while (0);

	cJSON_Delete(pRoot);
}

void GameOtherPlayer::DoPacket(const PkgPlayerBuyOlShopItemAck& ack)
{
	if(ack.nRet < 0)
	{
		ItemAttrib item;
		item.id = ack.nItemId;

		if(GameInfoManager::GetInstance()->GetItemAttrib(ack.nItemId, &item))
		{
			GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "购买商城物品[%s]失败", item.name);
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerGetOlShopListAck& ack)
{
	if(GetType() == OBJ_PLAYER)
	{
		OlShopDlg* pShopDlg = GameMainOptUI::GetInstance()->GetOlShopDlg();
		pShopDlg->ResetItems();
		pShopDlg->AddItems(&ack.xItemsId[0], ack.xItemsId.size());
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerExtendAttribNot& not)
{
	int nCount = not.xAttrib.size();

	//ZeroMemory(&m_stExtAttrib, sizeof(m_stExtAttrib));

	//	read specified attribute
	for(int i = 0; i < nCount; ++i)
	{
		const ExtendAttribItem& item = not.xAttrib[i];

		switch(item.uType)
		{
		case kExtendAttrib_Hair:
			{
				m_stExtAttrib.uHair = item.nValue;
			}break;
		case kExtendAttrib_Wing:
			{
				m_stExtAttrib.uWing = item.nValue;
			}break;
		case kExtendAttrib_SmeltOreLevel:
			{
				m_stExtAttrib.uSmeltOreLevel = item.nValue;
			}break;
		case kExtendAttrib_SmeltOreExp:
			{
				m_stExtAttrib.uSmeltOreExp = item.nValue;
			}break;
		case kExtendAttrib_SmeltWoodLevel:
			{
				m_stExtAttrib.uSmeltWoodLevel = item.nValue;
			}break;
		case kExtendAttrib_SmeltWoodExp:
			{
				m_stExtAttrib.uSmeltWoodExp = item.nValue;
			}break;
		case kExtendAttrib_SmeltClothLevel:
			{
				m_stExtAttrib.uSmeltClothLevel = item.nValue;
			}break;
		case kExtendAttrib_SmeltClothExp:
			{
				m_stExtAttrib.uSmeltClothExp = item.nValue;
			}break;
		case kExtendAttrib_SmeltGemLevel:
			{
				m_stExtAttrib.uSmeltGemLevel = item.nValue;
			}break;
		case kExtendAttrib_SmeltGemExp:
			{
				m_stExtAttrib.uSmeltGemExp = item.nValue;
			}break;
		case kExtendAttrib_MakeEquipLevel:
			{
				m_stExtAttrib.uMakeEquipLevel = item.nValue;
			}break;
		case kExtendAttrib_MakeEquipExp:
			{
				m_stExtAttrib.uMakeEquipExp = item.nValue;
			}break;
		case kExtendAttrib_ChatColor:
			{
				m_stExtAttrib.uChatColor = item.nValue;
			}break;
		case kExtendAttrib_ClothLook:
			{
				m_stExtAttrib.uClothLook = item.nValue;
			}break;
		case kExtendAttrib_WeaponLook:
			{
				m_stExtAttrib.uWeaponLook = item.nValue;
			}break;
		case kExtendAttrib_NameFrame:
			{
				m_stExtAttrib.uNameFrame = item.nValue;
			}break;
		case kExtendAttrib_ChatFrame:
			{
				m_stExtAttrib.uChatFrame = item.nValue;
			}break;
		default:
			{
				
			}break;
		}
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerSmeltMaterialsRsp& rsp)
{
	if(rsp.nCode == 0)
	{
		GameStoveDlg* pDlg = GameMainOptUI::GetInstance()->GetStoveDlg();
		pDlg->ResetData();
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerOpenPotentialRsp& rsp)
{
	if(rsp.nCode == 0)
	{
		GameStoveDlg* pDlg = GameMainOptUI::GetInstance()->GetStoveDlg();
		pDlg->ResetData();
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerEnableSkillNot& not)
{
	if(GetType() != OBJ_PLAYER)
	{
		return;
	}

	GamePlayer::GetInstance()->UpdateEnableSkill(not.nSkillId, not.bEnabled);
}

void GameOtherPlayer::DoPacket(const PkgPlayerPlayAniAck& ack)
{
	if(ack.wAniID == 1000 ||
		ack.wAniID == 1001 ||
		ack.wAniID == 1002)
	{
		std::list<DWORD>::const_iterator begIter = ack.xPos.begin();
		std::list<DWORD>::const_iterator endIter = ack.xPos.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			DWORD dwPos = *begIter;

			WORD wX = LOWORD(dwPos);
			WORD wY = HIWORD(dwPos);

			const MagicRenderInfo* pRenderInfo = NULL;
			if(!GameInfoManager::GetInstance()->GetMagicRenderInfo(ack.wAniID, &pRenderInfo))
			{
				continue;
			}

			SequenceEffect* pMgc = new SequenceEffect;
			pMgc->SetPosX(wX);
			pMgc->SetPosY(wY);
			if(!pMgc->Init(RES_ANI, pRenderInfo->destbegin, pRenderInfo->destframe, pRenderInfo->speed, pRenderInfo->desttotal))
			{
				SAFE_DELETE(pMgc);
				continue;
			}
			else
			{
				pMgc->SetActive();
				GameScene::sThis->InsertNewMagic(pMgc);
			}
		}
	}
	else if(ack.wAniID == 1006 ||
		ack.wAniID == 1007)
	{
		if(ack.xPos.empty())
		{
			return;
		}

		DWORD dwCoord = ack.xPos.front();
		WORD wX = LOWORD(dwCoord);
		WORD wY = HIWORD(dwCoord);

		GameScene::sThis->PlayTransAnimation(ack.wAniID, wX, wY);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerDifficultyLevelRsp& rsp)
{
	if(rsp.nRet < 0)
	{
		GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage("[提示]难度调整失败，可能已调整难度，或者处于战网模式", COLOR_SYSTEM);
	}
}

void GameOtherPlayer::DoPacket(const PkgPlayerQuitSelChrRsp& rsp)
{
	//	小退成功 切换场景
	GameMainOptUI::GetInstance()->GetSkillDlg()->Reset();
	GamePlayer::GetInstance()->Reset();
	::PostMessage(AfxGetHge()->System_GetState(HGE_HWND), WM_BACK_SELCHR, 0, 0);
}