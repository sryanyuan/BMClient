#include "MagicEffect.h"
#include "../GameScene/GameInfoManager.h"
#include "../Common/GameRenderObject.h"
#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameResourceManager.h"
#include "../Common/GlobalFunction.h"
#include "../GameScene/GameMonster.h"
#include <algorithm>
#include <hgeresource.h>
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
//	Global text
const char* g_szMgcDescriptor[MEFF_USERTOTAL] =
{
	//	0
	"",
	//	1
	"",
	//	2
	"基本剑术:提高剑术的熟练度和准确率",
	//	3
	"刺杀剑术:运用刀光攻击敌人",
	//	4
	"攻杀剑术:使用聚集的内力给敌人致命一击",
	//	5
	"半月弯刀:运用刀光攻击身前所有敌人",
	//	6
	"烈火剑法:凝聚火精灵的力量给敌人致命一击",
	//	7
	"稳如泰山:减小被打中后的后仰几率",
	//	8
	"狂战士:提高攻击速度",
	//	9
	"狮子吼:采用内功吼住周围一定范围敌人，使敌人麻痹",
	//	10
	"炙炎剑法:凝聚火精灵的力量给身前直线上的敌人致命一击",
	//	11
	"开天斩:运用刀光攻击身前直线上的所有敌人",
	//	12
	"血龙剑法:一定几率出现暴击",
	//	13
	"金刚不坏:运用真气保护肉身，一定几率吸收伤害与反弹伤害",
	//	14
	"",
	//	15
	"",
	//	16
	"",
	//	17
	"",
	//	18
	"",
	//	19
	"",
	//	20
	"",
	//	21
	"",
	//	22
	"",
	//	23
	"",
	//	24
	"",
	//	25
	"",
	//	26
	"",
	//	27
	"",


	//	MC
	//	28
	"",
	//	29
	"小火球",
	//	30
	"大火球",
	//	31
	"爆裂火焰",
	//	32
	"圣言术",
	//	33
	"冰咆哮",
	//	34
	"地狱雷光",
	//	35
	"雷电术",
	//	36
	"火墙",
	//	37
	"魔法盾: 制造一个魔法盾来保护施法者，持续时间和强度随着技能等级的提高而提高。进阶技能护体神盾可限制受到的最大伤害，当血量低于一定比例后，一定几率无视伤害",
	//	38
	"诱惑之光: 诱惑怪物作为自己的奴仆，可以召唤不高于法师等级3级的非不死系怪物",
	//	39
	"寒冰掌: 扔出一个冰球，随机造成怪物减速，在冰冻状态下可提升火系魔法伤害",
	//	40
	"灭天火: 召唤火龙攻击单个怪物，忽视部分魔法防御",
	//	41
	"火流星: 召唤炙热的流星雨连续攻击处于锁定魔法阵中的所有怪物",
	//	42
	"冰焰术: 通过制造冰柱来攻击敌人.",
	//	43
	"火龙气焰: 在一定时间内灼伤周围的所有敌人",
	//	44
	"",
	//	45
	"",
	//	46
	"",
	//	47
	"",
	//	48
	"",
	//	49
	"",
	//	50
	"",
	//	51
	"",
	//	52
	"",
	//	53
	"",
	//	54
	"",



	//	SC
	//	55
	"",
	//	56
	"治愈术",
	//	57
	"灵魂火符",
	//	58
	"神圣战甲术",
	//	59
	"幽灵盾",
	//	60
	"隐身术: 可以在一定时间内隐藏自己不被怪物发现,隐身时间随着技能等级和道术的提高而提高。",
	//	61
	"精神力战法: 通过精神力提高物理攻击的专注度，提高准确率",
	//	62
	"召唤术",
	//	63
	"施毒术",
	//	64
	"召唤骷髅",
	//	65
	"进阶召唤",
	//	66
	"神兽天御",
	//	67
	"先天气功",
	//	68
	"召唤白虎",
	//	69
	"群体治愈术",
	//	70
	"索命毒雾: 通过驱使护身符散播毒雾,使一定范围内怪物承受持续性攻击,一定几率使怪物中毒。",
	//	71
	"召唤月灵",
	//	END
	""
};
//////////////////////////////////////////////////////////////////////////
hgeSprite* MagicElement::pEffectRender = new hgeSprite(0, 0, 0, 0, 0);
hgeSprite* MagicElement::pNormalRender = new hgeSprite(0, 0, 0, 0, 0);
hgeSprite* EffectAttackNumber::s_pSharedEffectRender = new hgeSprite(0, 0, 0, 0, 0);
HTEXTURE EffectAttackNumber::s_pSharedNumberTexture = 0;
//////////////////////////////////////////////////////////////////////////
MagicElement::MagicElement()
{
	m_fFlyX = m_fFlyY = 0.0f;
	m_sDestX = m_sDestY = 0;
	m_sFireX = m_sFireY = 0;
	m_dwLastUpdateTime = 0;
	m_pFire = NULL;
	m_pTarget = NULL;
	m_bDrt16 = 0;
	m_bCanRemove = false;
	m_bFixed = false;
	m_wMgcID = MEFF_NONE;
	m_bStage = MAGIC_STAGE_NONE;
	m_pRenderInfo = NULL;
	m_bActive = false;
	m_bCurFrame = 0;
	m_dwSelfInterval = 0;
	m_nPrevDisX = m_nPrevDisY = 999999;
	m_bSnd = false;
	m_bIsHumEff = false;
	m_nRenderOftX = m_nRenderOftY = 0;
}

MagicElement::~MagicElement()
{
	
}
//////////////////////////////////////////////////////////////////////////
bool MagicElement::OverThrough(int _old, int _new)
{
	bool bRet = false;
	if(abs(_old - _new) >= 2)
	{
		bRet = true;
		if((_old == 0 && _new == 15) ||
			(_old == 15 && _new == 0))
		{
			bRet = false;
		}
		if(bRet)
		{
			AfxGetHge()->System_Log("over through %d %d",
				_old, _new);
		}
	}
	return bRet;
}
//////////////////////////////////////////////////////////////////////////
void MagicElement::Update()
{
	DWORD dwCurTick = GetTickCount();
	if(m_bStage == MAGIC_STAGE_SELF)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->selfframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_dwSelfInterval)
			{
				//	last frame
				m_bCurFrame = 0;
				if(m_pRenderInfo->wayframe == 0)
				{
					if(m_pRenderInfo->destframe == 0)
					{
						m_bCanRemove = true;
					}
					else
					{
						m_bStage = MAGIC_STAGE_DEST;
						m_dwLastUpdateTime = dwCurTick;

						if(m_pRenderInfo->destframe != 0 &&
							IsSoundEnable() &&
							InPlayRegion())
						{
							GameSoundManager::GetInstancePtr()->PlayMagicSound(m_wMgcID, MAGIC_STAGE_DEST);
						}
					}
				}
				else
				{
					m_bStage = MAGIC_STAGE_WAY;
					m_dwLastUpdateTime = dwCurTick;
					m_dwLastUpdateWayTime = dwCurTick;

					if(m_pRenderInfo->wayframe != 0 &&
						IsSoundEnable() &&
						InPlayRegion())
					{
						GameSoundManager::GetInstancePtr()->PlayMagicSound(m_wMgcID, MAGIC_STAGE_WAY);
					}
				}
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_dwSelfInterval)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
	else if(m_bStage == MAGIC_STAGE_WAY)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->wayframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;

				/*if(m_pRenderInfo->destframe == 0)
				{
					m_bCanRemove = true;
				}
				else
				{
					m_bStage = MAGIC_STAGE_WAY;
					m_dwLastUpdateTime = dwCurTick;
				}*/
				m_dwLastUpdateTime = dwCurTick;

				//	update the position
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}

		if(m_pTarget == NULL)
		{
			if(m_wMgcID == MEFF_CHARMAC ||
				m_wMgcID == MEFF_CHARMMC)
			{
				bool bCrash = false;
				DWORD dwDelta = dwCurTick - m_dwLastUpdateWayTime;

				//m_sDestX = m_pTarget->GetObjx();
				//m_sDestY = m_pTarget->GetObjy();
				int nTex = 0;
				int nTey = 0;

				if((int)m_fFlyX != m_sDestX)
				{
					nTex = abs(m_sDestX - (int)m_fFlyX);
				}
				else
				{
					nTex = 1;
				}
				if((int)m_fFlyY != m_sDestY)
				{
					nTey = abs(m_sDestY - (int)m_fFlyY);
				}
				else
				{
					nTey = 1;
				}

				int nNewDisX = 0;
				int nNewDisY = 0;
				if(nTex > nTey)
				{
					nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTex);
					nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTex);
				}
				else
				{
					nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTey);
					nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTey);
				}

				if(m_nDisX < nNewDisX)
				{
					m_nDisX += max(1.0f, ((float)nNewDisX - (float)m_nDisX) / 10.0f);
				}
				else if(m_nDisX > nNewDisX)
				{
					m_nDisX -= max(1.0f, ((float)m_nDisX - (float)nNewDisX) / 10.0f);
				}
				if(m_nDisY < nNewDisY)
				{
					m_nDisY += max(1.0f, ((float)nNewDisY - (float)m_nDisY) / 10.0f);
				}
				else if(m_nDisY > nNewDisY)
				{
					m_nDisY -= max(1.0f, ((float)m_nDisY - (float)nNewDisY) / 10.0f);
				}

				float fStepX = ((float)m_nDisX / 700) * dwDelta;
				float fStepY = ((float)m_nDisY / 700) * dwDelta;
				m_fFlyX += fStepX;
				m_fFlyY += fStepY;
				m_dwLastUpdateWayTime = dwCurTick;

				int nDrt = GetDirection16(m_fFlyX, m_fFlyY, m_sDestX, m_sDestY);
				int nNowDisX = abs(m_sDestX - m_fFlyX);
				int nNowDisY = abs(m_sDestY - m_fFlyY);
				if((nNowDisX <= 15 &&  nNowDisY <= 15) ||
					(nNowDisX >= m_nPrevDisX && nNowDisY >= m_nPrevDisY) ||
					OverThrough(m_bDrt16, nDrt))
				{
					m_dwLastUpdateTime = dwCurTick;
					if(m_pRenderInfo->destframe != 0)
					{
						m_bStage = MAGIC_STAGE_DEST;
						m_bCurFrame = 0;

						if(m_pRenderInfo->destframe != 0 &&
							IsSoundEnable() &&
							InPlayRegion())
						{
							GameSoundManager::GetInstancePtr()->PlayMagicSound(m_wMgcID, MAGIC_STAGE_DEST);
						}
					}
					else
					{
						m_bCanRemove = true;
					}
				}
				else
				{
					m_nPrevDisX = nNowDisX;
					m_nPrevDisY = nNowDisY;
				}
			}
			else
			{
				DWORD dwDelta = dwCurTick - m_dwLastUpdateWayTime;
				float fStepX = ((float)m_nDisX / 900) * dwDelta;
				float fStepY = ((float)m_nDisY / 900) * dwDelta;
				m_fFlyX += fStepX;
				m_fFlyY += fStepY;
				m_dwLastUpdateWayTime = dwCurTick;
			}
		}
		else
		{
			bool bCrash = false;
			DWORD dwDelta = dwCurTick - m_dwLastUpdateWayTime;

			m_sDestX = m_pTarget->GetObjx();
			m_sDestY = m_pTarget->GetObjy();
			int nTex = 0;
			int nTey = 0;

			if((int)m_fFlyX != m_sDestX)
			{
				nTex = abs(m_sDestX - (int)m_fFlyX);
			}
			else
			{
				nTex = 1;
			}
			if((int)m_fFlyY != m_sDestY)
			{
				nTey = abs(m_sDestY - (int)m_fFlyY);
			}
			else
			{
				nTey = 1;
			}

			int nNewDisX = 0;
			int nNewDisY = 0;
			if(nTex > nTey)
			{
				nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTex);
				nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTex);
			}
			else
			{
				nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTey);
				nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTey);
			}

			if(m_nDisX < nNewDisX)
			{
				m_nDisX += max(1.0f, ((float)nNewDisX - (float)m_nDisX) / 10.0f);
			}
			else if(m_nDisX > nNewDisX)
			{
				m_nDisX -= max(1.0f, ((float)m_nDisX - (float)nNewDisX) / 10.0f);
			}
			if(m_nDisY < nNewDisY)
			{
				m_nDisY += max(1.0f, ((float)nNewDisY - (float)m_nDisY) / 10.0f);
			}
			else if(m_nDisY > nNewDisY)
			{
				m_nDisY -= max(1.0f, ((float)m_nDisY - (float)nNewDisY) / 10.0f);
			}

			float fStepX = ((float)m_nDisX / 700) * dwDelta;
			float fStepY = ((float)m_nDisY / 700) * dwDelta;
			m_fFlyX += fStepX;
			m_fFlyY += fStepY;
			m_dwLastUpdateWayTime = dwCurTick;

			int nDrt = GetDirection16(m_fFlyX, m_fFlyY, m_sDestX, m_sDestY);
			int nNowDisX = abs(m_sDestX - m_fFlyX);
			int nNowDisY = abs(m_sDestY - m_fFlyY);
			if((nNowDisX <= 15 &&  nNowDisY <= 15) ||
				(nNowDisX > m_nPrevDisX && nNowDisY > m_nPrevDisY) ||
				OverThrough(m_bDrt16, nDrt))
			{
				m_dwLastUpdateTime = dwCurTick;
				if(m_pRenderInfo->destframe != 0)
				{
					m_bStage = MAGIC_STAGE_DEST;
					m_bCurFrame = 0;

					if(m_pRenderInfo->destframe != 0 &&
						IsSoundEnable() &&
						InPlayRegion())
					{
						GameSoundManager::GetInstancePtr()->PlayMagicSound(m_wMgcID, MAGIC_STAGE_DEST);
					}
				}
				else
				{
					m_bCanRemove = true;
				}
			}
			else
			{
				m_nPrevDisX = nNowDisX;
				m_nPrevDisY = nNowDisY;
			}
		}
	}
	else if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->destframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;
				m_bCanRemove = true;
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
}

void MagicElement::Render()
{
	if(m_bCanRemove)
	{
		return;
	}
	
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(m_pRenderInfo->wil + RES_MAGIC);
	if(NULL == pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;
	if(m_bStage == MAGIC_STAGE_SELF)
	{
		nTexIndex = m_pRenderInfo->selfbegin + m_bCurFrame;
		if(m_pRenderInfo->selftotal == 0)
		{
			return;
		}
	}
	else if(m_bStage == MAGIC_STAGE_WAY)
	{
		nTexIndex = m_pRenderInfo->waybegin + m_bCurFrame + m_bDrt16 * m_pRenderInfo->waytotal;
		if(m_pRenderInfo->waytotal == 0)
		{
			return;
		}
		//assert(nTexIndex < 16);
	}
	else if(m_bStage == MAGIC_STAGE_DEST)
	{
		nTexIndex = m_pRenderInfo->destbegin + m_bCurFrame;
		if(m_pRenderInfo->desttotal == 0)
		{
			return;
		}
	}
	tex = pTexMgr->GetTexture(nTexIndex);
	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_fFlyX, m_fFlyY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;
			if(m_bStage == MAGIC_STAGE_DEST)
			{
				nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();
			}
			else
			{
				nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();
			}
			sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
			pEffectRender->SetTexture(tex);
			pEffectRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(nTexIndex),
				pTexMgr->GetTextureHeight(nTexIndex));
			if(m_bStage != MAGIC_STAGE_SELF)
			{
				//nRenderX -= UNIT_WIDTH / 2;
				//nRenderY -= UNIT_HEIGHT / 2;
			}
			pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool MagicElement::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	if(NULL == _pFire)
	{
		return false;
	}
	if(false == GameInfoManager::GetInstance()->GetMagicRenderInfo(_nMgcID, &m_pRenderInfo))
	{
		return false;
	}
	m_wMgcID = (WORD)_nMgcID;
	m_pFire = _pFire;
	m_pTarget = _pTarget;
	m_sDestX = _sDestX;
	m_sDestY = _sDestY;
	if(_pTarget)
	{
		m_sDestX = _pTarget->GetObjx();
		m_sDestY = _pTarget->GetObjy();
	}
	m_sFireX = _pFire->GetObjx();
	m_sFireY = _pFire->GetObjy();
	m_fFlyX = _pFire->GetObjx();
	m_fFlyY = _pFire->GetObjy();
	m_dwAniTime = _dwAniTime;
	m_dwCreateTime = GetTickCount();

	float fAtkInterval = _pFire->GetAttackInterval();;
	DWORD dwSelfFrame = 6;
	if(m_pRenderInfo->selfframe > 0)
	{
		dwSelfFrame = m_pRenderInfo->selfframe;
	}
	float fMgcInterval = /*fAtkInterval * 6*/0.9f / (float)dwSelfFrame;
	m_dwSelfInterval = fMgcInterval * 1000;

	m_bFixed = false;

	int nTax = 0;
	int nTay = 0;
	if(m_sDestX != m_sFireX)
	{
		nTax = abs(m_sDestX - m_sFireX);
	}
	else
	{
		nTax = 1;
	}
	if(m_sDestY != m_sFireY)
	{
		nTay = abs(m_sDestY - m_sFireY);
	}
	else
	{
		nTay = 1;
	}
	if(nTax > nTay)
	{
		m_nDisX = (m_sDestX - m_sFireX) * (500.0f / nTax);
		m_nDisY = (m_sDestY - m_sFireY) * (500.0f / nTax);
	}
	else
	{
		m_nDisX = (m_sDestX - m_sFireX) * (500.0f / nTay);
		m_nDisY = (m_sDestY - m_sFireY) * (500.0f / nTay);
	}
	m_bDrt16 = GetDirection16(m_sFireX, m_sFireY, m_sDestX, m_sDestY);

	return true;
}
//////////////////////////////////////////////////////////////////////////
bool MagicElement::InPlayRegion()
{
	RECT rcView;
	rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
	rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
	rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
	rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

	POINT ptDraw = {m_fFlyX, m_fFlyY};
	if(PtInRect(&rcView, ptDraw))
	{
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool MagicFireBall::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	m_bStage = MAGIC_STAGE_SELF;
	m_bFixed = false;
	return __super::Create(_nMgcID, _pFire, _pTarget, _sDestX, _sDestY, _dwAniTime);
}

void MagicFireBall::Render()
{
	//__super::Render();
	if(m_bCanRemove)
	{
		return;
	}

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(m_pRenderInfo->wil + RES_MAGIC);
	if(NULL == pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;
	if(m_bStage == MAGIC_STAGE_SELF)
	{
		nTexIndex = m_pRenderInfo->selfbegin + m_bCurFrame;
		if(m_pRenderInfo->selftotal == 0)
		{
			return;
		}
	}
	else if(m_bStage == MAGIC_STAGE_WAY)
	{
		nTexIndex = m_pRenderInfo->waybegin + m_bCurFrame + m_bDrt16 * m_pRenderInfo->waytotal;
		if(m_pRenderInfo->waytotal == 0)
		{
			return;
		}
		//assert(nTexIndex < 16);
	}
	else if(m_bStage == MAGIC_STAGE_DEST)
	{
		nTexIndex = m_pRenderInfo->destbegin + m_bCurFrame;
		if(m_pRenderInfo->desttotal == 0)
		{
			return;
		}
	}
	tex = pTexMgr->GetTexture(nTexIndex);
	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_fFlyX, m_fFlyY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;
			if(m_bStage == MAGIC_STAGE_DEST)
			{
				nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();

				if(m_wMgcID == MEFF_ICETHRUST)
				{
					nRenderY += 2 * UNIT_HEIGHT;
				}
			}
			else
			{
				nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();
			}
			sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
			pEffectRender->SetTexture(tex);
			pEffectRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(nTexIndex),
				pTexMgr->GetTextureHeight(nTexIndex));
			if(m_bStage != MAGIC_STAGE_SELF)
			{
				//nRenderX -= UNIT_WIDTH / 2;
				//nRenderY -= UNIT_HEIGHT / 2;
			}
			pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}

void MagicFireBall::Update()
{
	__super::Update();
	if(GetTickCount() - m_dwCreateTime > 10000)
	{
		//	10s auto delete
		m_bCanRemove = true;
	}
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
bool MagicFireWall::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	m_bStage = MAGIC_STAGE_SELF;
	m_bFixed = false;
	return __super::Create(_nMgcID, _pFire, _pTarget, _sDestX, _sDestY, _dwAniTime);
}

void MagicFireWall::Update()
{
	DWORD dwCurTick = GetTickCount();
	if(m_bStage == MAGIC_STAGE_SELF)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->selfframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_dwSelfInterval)
			{
				//	last frame
				m_bCurFrame = 0;
				if(m_pRenderInfo->wayframe == 0)
				{
					if(m_pRenderInfo->destframe == 0)
					{
						m_bCanRemove = true;
					}
					else
					{
						m_bStage = MAGIC_STAGE_DEST;
						m_dwLastUpdateTime = dwCurTick;
						m_bFixed = true;
						if(m_dwAniTime == 0)
						{
							m_bCanRemove = true;
						}
						else
						{
							m_dwAniTime += dwCurTick;
						}
					}
				}
				else
				{
					m_bStage = MAGIC_STAGE_WAY;
					m_dwLastUpdateTime = dwCurTick;
					m_dwLastUpdateWayTime = dwCurTick;
				}
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_dwSelfInterval)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
	else if(m_bStage == MAGIC_STAGE_WAY)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->wayframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;

				/*if(m_pRenderInfo->destframe == 0)
				{
					m_bCanRemove = true;
				}
				else
				{
					m_bStage = MAGIC_STAGE_WAY;
					m_dwLastUpdateTime = dwCurTick;
				}*/
				m_dwLastUpdateTime = dwCurTick;

				//	update the position
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
	else if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->destframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;
				m_dwLastUpdateTime = dwCurTick;

				if(m_dwLastUpdateTime > m_dwAniTime)
				{
					m_bCanRemove = true;
				}
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;

				if(m_dwLastUpdateTime > m_dwAniTime)
				{
					m_bCanRemove = true;
				}
			}
		}
	}
}

void MagicFireWall::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	static const int s_nAroundOft[] =
	{
		0,-1,
		0,1,
		1,0,
		-1,0
	};

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(m_pRenderInfo->wil + RES_MAGIC);
	if(NULL == pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;
	if(m_bStage == MAGIC_STAGE_SELF)
	{
		nTexIndex = m_pRenderInfo->selfbegin + m_bCurFrame;
		if(m_pRenderInfo->selftotal == 0)
		{
			return;
		}
	}
	else if(m_bStage == MAGIC_STAGE_WAY)
	{
		nTexIndex = m_pRenderInfo->waybegin + m_bCurFrame + m_bDrt16 * m_pRenderInfo->waytotal;
		if(m_pRenderInfo->waytotal == 0)
		{
			return;
		}
		//assert(nTexIndex < 16);
	}
	else if(m_bStage == MAGIC_STAGE_DEST)
	{
		nTexIndex = m_pRenderInfo->destbegin + m_bCurFrame;
		if(m_pRenderInfo->desttotal == 0)
		{
			return;
		}
	}
	tex = pTexMgr->GetTexture(nTexIndex);
	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_fFlyX, m_fFlyY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;
			if(m_bStage == MAGIC_STAGE_DEST)
			{
				nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();

				sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
				sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
				pEffectRender->SetTexture(tex);
				pEffectRender->SetTextureRect(0, 0,
					pTexMgr->GetTextureWidth(nTexIndex),
					pTexMgr->GetTextureHeight(nTexIndex));
				if(m_bStage != MAGIC_STAGE_SELF)
				{
					//nRenderX -= UNIT_WIDTH / 2;
					//nRenderY -= UNIT_HEIGHT / 2;
				}
				pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);

				for(int i = 0; i < 4; ++i)
				{
					nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH + (s_nAroundOft[i * 2] * UNIT_WIDTH) - (int)GamePlayer::GetInstance()->GetObjx();
					nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT + (s_nAroundOft[i * 2 + 1] * UNIT_HEIGHT) - (int)GamePlayer::GetInstance()->GetObjy();

					sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
					sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
					pEffectRender->SetTexture(tex);
					pEffectRender->SetTextureRect(0, 0,
						pTexMgr->GetTextureWidth(nTexIndex),
						pTexMgr->GetTextureHeight(nTexIndex));
					if(m_bStage != MAGIC_STAGE_SELF)
					{
						//nRenderX -= UNIT_WIDTH / 2;
						//nRenderY -= UNIT_HEIGHT / 2;
					}
					pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
				}
			}
			else
			{
				nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();

				sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
				sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
				pEffectRender->SetTexture(tex);
				pEffectRender->SetTextureRect(0, 0,
					pTexMgr->GetTextureWidth(nTexIndex),
					pTexMgr->GetTextureHeight(nTexIndex));
				if(m_bStage != MAGIC_STAGE_SELF)
				{
					//nRenderX -= UNIT_WIDTH / 2;
					//nRenderY -= UNIT_HEIGHT / 2;
				}
				pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
bool MagicShield::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	m_bStage = MAGIC_STAGE_SELF;
	m_bFixed = false;
	return __super::Create(_nMgcID, _pFire, _pTarget, _sDestX, _sDestY, _dwAniTime);
}

void MagicShield::Update()
{
	DWORD dwCurTick = GetTickCount();
	if(m_bStage == MAGIC_STAGE_SELF)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->selfframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_dwSelfInterval)
			{
				//	last frame
				m_bCurFrame = 0;
				m_bCanRemove = true;
				if(m_pFire)
				{
					if(m_pFire->GetType() == OBJ_PLAYER ||
						m_pFire->GetType() == OBJ_OTHERPLAYER)
					{
						if(m_dwAniTime == 0)
						{
							//	Nothing
						}
						else
						{
							static_cast<GameOtherPlayer*>(m_pFire)->SetHumEffect(MMASK_SHIELD,m_dwAniTime/* + dwCurTick*/);
						}
					}
				}
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_dwSelfInterval)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
	else if(m_bStage == MAGIC_STAGE_WAY)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->wayframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;

				/*if(m_pRenderInfo->destframe == 0)
				{
					m_bCanRemove = true;
				}
				else
				{
					m_bStage = MAGIC_STAGE_WAY;
					m_dwLastUpdateTime = dwCurTick;
				}*/
				m_dwLastUpdateTime = dwCurTick;

				//	update the position
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
	else if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->destframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;
				m_dwLastUpdateTime = dwCurTick;

				if(m_dwLastUpdateTime > m_dwAniTime)
				{
					m_bCanRemove = true;
				}
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;

				if(m_dwLastUpdateTime > m_dwAniTime)
				{
					m_bCanRemove = true;
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void MagicEnergyShield::Update()
{
	DWORD dwCurTick = GetTickCount();
	if(m_bStage == MAGIC_STAGE_SELF)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->selfframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_dwSelfInterval)
			{
				//	last frame
				m_bCurFrame = 0;
				m_bCanRemove = true;
				if(m_pFire)
				{
					if(m_pFire->GetType() == OBJ_PLAYER ||
						m_pFire->GetType() == OBJ_OTHERPLAYER)
					{
						static_cast<GameOtherPlayer*>(m_pFire)->SetHumEffect(MMASK_ENERGYSHIELD,m_dwAniTime/* + dwCurTick*/);
					}
				}
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_dwSelfInterval)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
	else if(m_bStage == MAGIC_STAGE_WAY)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->wayframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;

				/*if(m_pRenderInfo->destframe == 0)
				{
					m_bCanRemove = true;
				}
				else
				{
					m_bStage = MAGIC_STAGE_WAY;
					m_dwLastUpdateTime = dwCurTick;
				}*/
				m_dwLastUpdateTime = dwCurTick;

				//	update the position
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
	else if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->destframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;
				m_dwLastUpdateTime = dwCurTick;

				if(m_dwLastUpdateTime > m_dwAniTime)
				{
					m_bCanRemove = true;
				}
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;

				if(m_dwLastUpdateTime > m_dwAniTime)
				{
					m_bCanRemove = true;
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool MagicHeal::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	m_bStage = MAGIC_STAGE_SELF;
	m_bFixed = false;
	if(_pTarget == NULL)
	{
		_pTarget = GamePlayer::GetInstance();
	}
	return __super::Create(_nMgcID, _pFire, _pTarget, _sDestX, _sDestY, _dwAniTime);
}

void MagicHeal::Update()
{
	__super::Update();

	if(GetTickCount() - m_dwCreateTime > 10*1000)
	{
		m_bCanRemove = true;
	}
}

void MagicHeal::Render()
{
	if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCanRemove)
		{
			return;
		}

		GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(m_pRenderInfo->wil + RES_MAGIC);
		if(NULL == pTexMgr)
		{
			return;
		}
		if(NULL == m_pTarget)
		{
			return;
		}
		HTEXTURE tex = 0;
		int nTexIndex = 0;
		short sOftX = 0;
		short sOftY = 0;
		if(m_bStage == MAGIC_STAGE_DEST)
		{
			nTexIndex = m_pRenderInfo->destbegin + m_bCurFrame;
			if(m_pRenderInfo->desttotal == 0)
			{
				return;
			}
		}
		tex = pTexMgr->GetTexture(nTexIndex);
		if(tex)
		{
			RECT rcView;
			rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
			rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
			rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
			rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

			POINT ptDraw = {m_pTarget->GetObjx(), m_pTarget->GetObjy()};
			if(PtInRect(&rcView, ptDraw))
			{
				int nRenderX = 0;
				int nRenderY = 0;
				/*if(m_bStage == MAGIC_STAGE_DEST)
				{
					nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
					nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();
				}
				else
				{
					nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
					nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();
				}*/
				nRenderX = ptDraw.x + CENTRAL_X - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = ptDraw.y + CENTRAL_Y - (int)GamePlayer::GetInstance()->GetObjy();
				sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
				sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
				pEffectRender->SetTexture(tex);
				pEffectRender->SetTextureRect(0, 0,
					pTexMgr->GetTextureWidth(nTexIndex),
					pTexMgr->GetTextureHeight(nTexIndex));
				if(m_bStage != MAGIC_STAGE_SELF)
				{
					//nRenderX -= UNIT_WIDTH / 2;
					//nRenderY -= UNIT_HEIGHT / 2;
				}
				pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
			}
		}
	}
	else
	{
		__super::Render();
	}
}

//////////////////////////////////////////////////////////////////////////
bool MagicPoison::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	m_bStage = MAGIC_STAGE_SELF;
	m_bFixed = false;
	
	bool bRet = __super::Create(_nMgcID, _pFire, _pTarget, _sDestX, _sDestY, _dwAniTime);
	if(NULL == _pTarget)
	{
		m_pTarget = NULL;
	}
	return bRet;
}

void MagicPoison::Update()
{
	if(m_bCanRemove)
	{
		return;
	}
	if(MAGIC_STAGE_DEST == m_bStage &&
		NULL == m_pTarget)
	{
		m_bCanRemove = true;
		return;
	}
	
	__super::Update();

	if(MAGIC_STAGE_DEST == m_bStage &&
		NULL != m_pTarget)
	{
		if(m_bCurFrame == m_pRenderInfo->destframe - 1)
		{
			m_bCanRemove = true;
			m_pTarget->SetHumEffect(MMASK_LVDU, m_dwAniTime/* + GetTickCount()*/);
		}
	}

	if(GetTickCount() - m_dwCreateTime > 15*1000)
	{
		m_bCanRemove = true;
	}
}

void MagicPoison::Render()
{
	if(m_bCanRemove)
	{
		return;
	}
	__super::Render();
}
//////////////////////////////////////////////////////////////////////////
bool MagicHide::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	m_bStage = MAGIC_STAGE_SELF;
	m_bFixed = false;

	bool bRet = __super::Create(_nMgcID, _pFire, _pTarget, _sDestX, _sDestY, _dwAniTime);
	return bRet;
}

void MagicHide::Update()
{
	if(m_bCanRemove)
	{
		return;
	}

	__super::Update();

	if(m_bStage == MAGIC_STAGE_SELF &&
		m_bCurFrame == m_pRenderInfo->selfframe - 1)
	{
		m_bCanRemove = true;
		m_pFire->SetHumEffect(MMASK_HIDE, m_dwAniTime/* + GetTickCount()*/);
	}
	if(GetTickCount() - m_dwCreateTime > 15*1000)
	{
		m_bCanRemove = true;
	}
}
//////////////////////////////////////////////////////////////////////////
bool MagicTempt::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	m_bStage = MAGIC_STAGE_SELF;
	m_bFixed = false;
	/*if(_pTarget == NULL)
	{
		_pTarget = GamePlayer::GetInstance();
	}*/
	return __super::Create(_nMgcID, _pFire, _pTarget, _sDestX, _sDestY, _dwAniTime);
}

void MagicTempt::Update()
{
	__super::Update();

	if(GetTickCount() - m_dwCreateTime > 10*1000)
	{
		m_bCanRemove = true;
	}
}

void MagicTempt::Render()
{
	if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCanRemove)
		{
			return;
		}

		GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(m_pRenderInfo->wil + RES_MAGIC);
		if(NULL == pTexMgr)
		{
			return;
		}
		/*if(NULL == m_pTarget)
		{
			return;
		}*/
		HTEXTURE tex = 0;
		int nTexIndex = 0;
		short sOftX = 0;
		short sOftY = 0;
		if(m_bStage == MAGIC_STAGE_DEST)
		{
			nTexIndex = m_pRenderInfo->destbegin + m_bCurFrame;
			if(m_pRenderInfo->desttotal == 0)
			{
				return;
			}
		}
		tex = pTexMgr->GetTexture(nTexIndex);
		if(tex)
		{
			RECT rcView;
			rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
			rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
			rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
			rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

			POINT ptDraw/* = {m_pTarget->GetObjx(), m_pTarget->GetObjy()};*/;
			if(m_pTarget)
			{
				ptDraw.x = m_pTarget->GetObjx();
				ptDraw.y = m_pTarget->GetObjy();
			}
			else
			{
				ptDraw.x = m_sDestX;
				ptDraw.y = m_sDestY;
			}
			if(PtInRect(&rcView, ptDraw))
			{
				int nRenderX = 0;
				int nRenderY = 0;
				/*if(m_bStage == MAGIC_STAGE_DEST)
				{
					nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
					nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();
				}
				else
				{
					nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
					nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();
				}*/
				nRenderX = ptDraw.x + CENTRAL_X - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = ptDraw.y + CENTRAL_Y - (int)GamePlayer::GetInstance()->GetObjy();
				sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
				sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
				pEffectRender->SetTexture(tex);
				pEffectRender->SetTextureRect(0, 0,
					pTexMgr->GetTextureWidth(nTexIndex),
					pTexMgr->GetTextureHeight(nTexIndex));
				if(m_bStage != MAGIC_STAGE_SELF)
				{
					//nRenderX -= UNIT_WIDTH / 2;
					//nRenderY -= UNIT_HEIGHT / 2;
				}
				pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
			}
		}
	}
	else
	{
		__super::Render();
	}
}
//////////////////////////////////////////////////////////////////////////
MagicDoor::MagicDoor()
{
	
}

MagicDoor::~MagicDoor()
{

}
//////////////////////////////////////////////////////////////////////////
bool MagicDoor::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	if(false == GameInfoManager::GetInstance()->GetMagicRenderInfo(_nMgcID, &m_pRenderInfo))
	{
		return false;
	}
	m_wMgcID = (WORD)_nMgcID;

	m_sDestX = _sDestX;
	m_sDestY = _sDestY;
	m_fFlyX = m_sDestX;
	m_fFlyY = m_sDestY;

	m_bCanRemove = false;

	if(_dwAniTime != 0)
	{
		m_dwAniTime = GetTickCount() + _dwAniTime;
	}
	else
	{
		m_dwAniTime = 0;
	}
	
	m_dwCreateTime = GetTickCount();
	SetActive();
	m_bStage = MAGIC_STAGE_DEST;

	return true;
}
void MagicDoor::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	__super::Render();
}
void MagicDoor::Update()
{
	if(m_bCanRemove)
	{
		return;
	}

	if(GetTickCount() - m_dwLastUpdateTime > m_pRenderInfo->speed)
	{
		if(m_dwAniTime != 0)
		{
			if(GetTickCount() > m_dwAniTime)
			{
				m_bCanRemove = true;
				return;
			}
		}

		if(m_bCurFrame >= m_pRenderInfo->destframe - 1)
		{
			m_bCurFrame = 0;
		}
		else
		{
			++m_bCurFrame;
		}
		m_dwLastUpdateTime = GetTickCount();
	}
}
//////////////////////////////////////////////////////////////////////////
MagicStaticAttackEffect::MagicStaticAttackEffect()
{
	m_pTexMgr = NULL;
	m_pEffInfo = NULL;
	m_nDrt = 0;
}

MagicStaticAttackEffect::~MagicStaticAttackEffect()
{

}


bool MagicStaticAttackEffect::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	m_fFlyX = _sDestX;
	m_fFlyY = _sDestY;
	bool bLoadOk = false;

	if(_nMgcID == 14)
	{
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON18);
		bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);
	}
	else if(_nMgcID == MONSTER_HUOYANWOMA)
	{
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON4);
		bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);
	}
	else if(_nMgcID == MONSTER_CHULONGSHEN ||
		_nMgcID == 131)
	{
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON15);
		bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);
	}
	else if(_nMgcID == 62 ||
		_nMgcID == 82)
	{
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(/*RES_MON18*/RES_MAGIC8);
		bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);
	}
	if(NULL == m_pTexMgr ||
		false == bLoadOk)
	{
		return false;
	}

	m_bFixed = false;
	m_bCurFrame = 0;
	m_dwLastUpdateTime = 0;
	m_bCanRemove = false;
	m_nDrt = _dwAniTime;
	return true;
}

void MagicStaticAttackEffect::Update()
{
	DWORD dwCurTick = GetTickCount();
	if(m_bCanRemove)
	{
		return;
	}

	if(dwCurTick - m_dwLastUpdateTime > 100)
	{
		if(m_bCurFrame == (m_pEffInfo->atkeftframe - 1))
		{
			//	Last frame
			m_bCanRemove = true;
		}
		else
		{
			++m_bCurFrame;
		}
		m_dwLastUpdateTime = dwCurTick;
	}
}

void MagicStaticAttackEffect::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nRenderX = 0;
	int nRenderY = 0;

	nRenderX = m_fFlyX + CENTRAL_X - (int)pPlayer->GetObjx();
	nRenderY = m_fFlyY + CENTRAL_Y - (int)pPlayer->GetObjy();

	int nTextIndex = 0;
	if(m_wMgcID == MONSTER_CHULONGSHEN ||
		m_wMgcID == 131)
	{
		nTextIndex = m_pEffInfo->atkeftbegin + m_bCurFrame/* + m_nDrt * m_pEffInfo->atkefttotal;*/;
	}
	else
	{
		nTextIndex = m_pEffInfo->atkeftbegin + m_bCurFrame + m_nDrt * m_pEffInfo->atkefttotal;
	}
	int nOftX = m_pTexMgr->GetTextureOffsetX(nTextIndex);
	int nOftY = m_pTexMgr->GetTextureOffsetY(nTextIndex);
	int nTexWidth = m_pTexMgr->GetTextureWidth(nTextIndex);
	int nTexHeight = m_pTexMgr->GetTextureHeight(nTextIndex);
	HTEXTURE tex = m_pTexMgr->GetTexture(nTextIndex);

	if(tex)
	{
		pEffectRender->SetTexture(tex);
		pEffectRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
		pEffectRender->Render(nRenderX + nOftX,
			nRenderY + nOftY);
	}
}

//////////////////////////////////////////////////////////////////////////
Magic16DrtSelf::Magic16DrtSelf()
{
	m_pTexMgr = NULL;
	m_pEffInfo = NULL;
}

bool Magic16DrtSelf::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	if(_pFire == NULL ||
		_pTarget == NULL)
	{
		return false;
	}

	bool bLoadOk = true;
	if(_nMgcID == 45)
	{
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON5);
		bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);
	}

	if(NULL == m_pEffInfo ||
		false == bLoadOk)
	{
		return false;
	}

	return true;
}

void Magic16DrtSelf::Render()
{

}

void Magic16DrtSelf::Update()
{

}
//////////////////////////////////////////////////////////////////////////
Magic16DrtWay::Magic16DrtWay()
{
	m_pTexMgr = NULL;
	m_pEffInfo = NULL;
}

bool Magic16DrtWay::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	if(_pFire == NULL ||
		_pTarget == NULL)
	{
		return false;
	}

	bool bLoadOk = true;
	if(_nMgcID == 46 ||
		_nMgcID == 47)
	{
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON5);
		bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);
	}
	else if(_nMgcID == 85)
	{
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON22);
		bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);
	}
	else if(_nMgcID == 144)
	{
		//m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON20);
		//bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MAGIC6);
		bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);
	}

	if(NULL == m_pEffInfo ||
		false == bLoadOk)
	{
		return false;
	}

	if(NULL == _pFire)
	{
		return false;
	}

	m_wMgcID = (WORD)_nMgcID;
	m_pFire = _pFire;
	m_pTarget = _pTarget;
	m_sDestX = _sDestX;
	m_sDestY = _sDestY;
	if(_pTarget)
	{
		m_sDestX = _pTarget->GetObjx();
		m_sDestY = _pTarget->GetObjy();
	}
	m_sFireX = _pFire->GetObjx();
	m_sFireY = _pFire->GetObjy();
	m_fFlyX = _pFire->GetObjx();
	m_fFlyY = _pFire->GetObjy();
	m_dwAniTime = _dwAniTime;
	m_dwCreateTime = GetTickCount();

	float fAtkInterval = _pFire->GetAttackInterval();
	DWORD dwSelfFrame = 6;
	/*if(m_pRenderInfo->selfframe > 0)
	{
		dwSelfFrame = m_pRenderInfo->selfframe;
	}*/
	float fMgcInterval = fAtkInterval * 6 / (float)dwSelfFrame;
	m_dwSelfInterval = fMgcInterval * 1000;

	m_bFixed = false;

	int nTax = 0;
	int nTay = 0;
	if(m_sDestX != m_sFireX)
	{
		nTax = abs(m_sDestX - m_sFireX);
	}
	else
	{
		nTax = 1;
	}
	if(m_sDestY != m_sFireY)
	{
		nTay = abs(m_sDestY - m_sFireY);
	}
	else
	{
		nTay = 1;
	}
	if(nTax > nTay)
	{
		m_nDisX = (m_sDestX - m_sFireX) * (500.0f / nTax);
		m_nDisY = (m_sDestY - m_sFireY) * (500.0f / nTax);
	}
	else
	{
		m_nDisX = (m_sDestX - m_sFireX) * (500.0f / nTay);
		m_nDisY = (m_sDestY - m_sFireY) * (500.0f / nTay);
	}
	m_bDrt16 = GetDirection16(m_sFireX, m_sFireY, m_sDestX, m_sDestY);
	m_bStage = MAGIC_STAGE_WAY;
	m_dwLastUpdateWayTime = m_dwCreateTime;

	return true;
}

void Magic16DrtWay::Update()
{
	if(CanRemove())
	{
		return;
	}
	DWORD dwCurTick = GetTickCount();

	if(m_bStage == MAGIC_STAGE_WAY)
	{
		if(m_bCurFrame + 1 >= m_pEffInfo->atkeftframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > 40)
			{
				//	last frame
				m_bCurFrame = 0;

				/*if(m_pRenderInfo->destframe == 0)
				{
					m_bCanRemove = true;
				}
				else
				{
					m_bStage = MAGIC_STAGE_WAY;
					m_dwLastUpdateTime = dwCurTick;
				}*/
				m_dwLastUpdateTime = dwCurTick;

				//	update the position
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > 40)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}

		if(m_pTarget == NULL)
		{
			if(m_wMgcID == MEFF_CHARMAC ||
				m_wMgcID == MEFF_CHARMMC)
			{
				bool bCrash = false;
				DWORD dwDelta = dwCurTick - m_dwLastUpdateWayTime;

				//m_sDestX = m_pTarget->GetObjx();
				//m_sDestY = m_pTarget->GetObjy();
				int nTex = 0;
				int nTey = 0;

				if((int)m_fFlyX != m_sDestX)
				{
					nTex = abs(m_sDestX - (int)m_fFlyX);
				}
				else
				{
					nTex = 1;
				}
				if((int)m_fFlyY != m_sDestY)
				{
					nTey = abs(m_sDestY - (int)m_fFlyY);
				}
				else
				{
					nTey = 1;
				}

				int nNewDisX = 0;
				int nNewDisY = 0;
				if(nTex > nTey)
				{
					nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTex);
					nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTex);
				}
				else
				{
					nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTey);
					nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTey);
				}

				if(m_nDisX < nNewDisX)
				{
					m_nDisX += max(1.0f, ((float)nNewDisX - (float)m_nDisX) / 10.0f);
				}
				else if(m_nDisX > nNewDisX)
				{
					m_nDisX -= max(1.0f, ((float)m_nDisX - (float)nNewDisX) / 10.0f);
				}
				if(m_nDisY < nNewDisY)
				{
					m_nDisY += max(1.0f, ((float)nNewDisY - (float)m_nDisY) / 10.0f);
				}
				else if(m_nDisY > nNewDisY)
				{
					m_nDisY -= max(1.0f, ((float)m_nDisY - (float)nNewDisY) / 10.0f);
				}

				float fStepX = ((float)m_nDisX / 700) * dwDelta;
				float fStepY = ((float)m_nDisY / 700) * dwDelta;
				m_fFlyX += fStepX;
				m_fFlyY += fStepY;
				m_dwLastUpdateWayTime = dwCurTick;

				int nDrt = GetDirection16(m_fFlyX, m_fFlyY, m_sDestX, m_sDestY);
				int nNowDisX = abs(m_sDestX - m_fFlyX);
				int nNowDisY = abs(m_sDestY - m_fFlyY);
				if((nNowDisX <= 15 &&  nNowDisY <= 15) ||
					(nNowDisX >= m_nPrevDisX && nNowDisY >= m_nPrevDisY) ||
					OverThrough(m_bDrt16, nDrt))
				{
					m_dwLastUpdateTime = dwCurTick;
					if(m_pRenderInfo->destframe != 0)
					{
						m_bStage = MAGIC_STAGE_DEST;
						m_bCurFrame = 0;
					}
					else
					{
						m_bCanRemove = true;
					}
				}
				else
				{
					m_nPrevDisX = nNowDisX;
					m_nPrevDisY = nNowDisY;
				}
			}
			else
			{
				DWORD dwDelta = dwCurTick - m_dwLastUpdateWayTime;
				float fStepX = ((float)m_nDisX / 900) * dwDelta;
				float fStepY = ((float)m_nDisY / 900) * dwDelta;
				m_fFlyX += fStepX;
				m_fFlyY += fStepY;
				m_dwLastUpdateWayTime = dwCurTick;
			}
		}
		else
		{
			bool bCrash = false;
			DWORD dwDelta = dwCurTick - m_dwLastUpdateWayTime;

			m_sDestX = m_pTarget->GetObjx();
			m_sDestY = m_pTarget->GetObjy();
			int nTex = 0;
			int nTey = 0;

			if((int)m_fFlyX != m_sDestX)
			{
				nTex = abs(m_sDestX - (int)m_fFlyX);
			}
			else
			{
				nTex = 1;
			}
			if((int)m_fFlyY != m_sDestY)
			{
				nTey = abs(m_sDestY - (int)m_fFlyY);
			}
			else
			{
				nTey = 1;
			}

			int nNewDisX = 0;
			int nNewDisY = 0;
			if(nTex > nTey)
			{
				nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTex);
				nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTex);
			}
			else
			{
				nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTey);
				nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTey);
			}

			if(m_nDisX < nNewDisX)
			{
				m_nDisX += max(1.0f, ((float)nNewDisX - (float)m_nDisX) / 10.0f);
			}
			else if(m_nDisX > nNewDisX)
			{
				m_nDisX -= max(1.0f, ((float)m_nDisX - (float)nNewDisX) / 10.0f);
			}
			if(m_nDisY < nNewDisY)
			{
				m_nDisY += max(1.0f, ((float)nNewDisY - (float)m_nDisY) / 10.0f);
			}
			else if(m_nDisY > nNewDisY)
			{
				m_nDisY -= max(1.0f, ((float)m_nDisY - (float)nNewDisY) / 10.0f);
			}

			float fStepX = ((float)m_nDisX / 700) * dwDelta;
			float fStepY = ((float)m_nDisY / 700) * dwDelta;
			m_fFlyX += fStepX;
			m_fFlyY += fStepY;
			m_dwLastUpdateWayTime = dwCurTick;

			int nDrt = GetDirection16(m_fFlyX, m_fFlyY, m_sDestX, m_sDestY);
			int nNowDisX = abs(m_sDestX - m_fFlyX);
			int nNowDisY = abs(m_sDestY - m_fFlyY);
			if((nNowDisX <= 15 &&  nNowDisY <= 15) ||
				(nNowDisX > m_nPrevDisX && nNowDisY > m_nPrevDisY) ||
				OverThrough(m_bDrt16, nDrt))
			{
				m_dwLastUpdateTime = dwCurTick;
				/*if(m_pRenderInfo->destframe != 0)
				{
					m_bStage = MAGIC_STAGE_DEST;
					m_bCurFrame = 0;
				}
				else*/
				{
					m_bCanRemove = true;
				}
			}
			else
			{
				m_nPrevDisX = nNowDisX;
				m_nPrevDisY = nNowDisY;
			}
		}
	}
}

void Magic16DrtWay::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	GameTextureManager* pTexMgr = m_pTexMgr;
	if(NULL == pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;
	if(m_bStage == MAGIC_STAGE_WAY)
	{
		nTexIndex = m_pEffInfo->atkeftbegin + m_bDrt16 * m_pEffInfo->atkefttotal + m_bCurFrame;
		//assert(nTexIndex < 16);
	}
	
	tex = pTexMgr->GetTexture(nTexIndex);
	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_fFlyX, m_fFlyY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;
			/*if(m_bStage == MAGIC_STAGE_DEST)
			{
				nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();
			}
			else*/
			{
				nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();
			}
			sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
			pEffectRender->SetTexture(tex);
			pEffectRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(nTexIndex),
				pTexMgr->GetTextureHeight(nTexIndex));
			if(m_bStage != MAGIC_STAGE_SELF)
			{
				//nRenderX -= UNIT_WIDTH / 2;
				//nRenderY -= UNIT_HEIGHT / 2;
			}
			if(m_wMgcID == 85 ||
				m_wMgcID == 144)
			{
				pEffectRender->Render(nRenderX + sOftX + m_nRenderOftX, nRenderY + sOftY + m_nRenderOftY);
			}
			else
			{
				pEffectRender->Render(nRenderX + sOftX + m_nRenderOftY, nRenderY + sOftY - UNIT_HEIGHT + m_nRenderOftY);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
Magic16DrtExplosion::Magic16DrtExplosion()
{
	m_pTexMgr = NULL;
	m_pEffInfo = NULL;
	m_nMagicIndex = -1;
	m_nWayFrameBegin = m_nWayFrames = 0;
	m_nDestFrameBegin = m_nDestFrames = 0;
}

bool Magic16DrtExplosion::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	if(_pFire == NULL ||
		_pTarget == NULL)
	{
		return false;
	}
	if(0 == _sDestX)
	{
		return false;
	}

	m_nMagicIndex = _sDestX;

	bool bLoadOk = true;

	if(_nMgcID == 138)
	{
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON31);
		bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);

		if(0 == m_nMagicIndex)
		{
			m_nWayFrameBegin = 3560;
			m_nWayFrames = 9;

			m_nDestFrameBegin = 3570;
			m_nWayFrames = 10;
		}
		else if(1 == m_nMagicIndex)
		{
			m_nWayFrameBegin = 3580;
			m_nWayFrames = 8;

			m_nDestFrameBegin = 3590;
			m_nWayFrames = 10;
		}
		else if(2 == m_nMagicIndex)
		{
			m_nWayFrameBegin = 3600;
			m_nWayFrames = 9;

			m_nDestFrameBegin = 3610;
			m_nWayFrames = 8;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	if(NULL == m_pEffInfo ||
		false == bLoadOk)
	{
		return false;
	}

	if(NULL == _pFire)
	{
		return false;
	}

	m_wMgcID = (WORD)_nMgcID;
	m_pFire = _pFire;
	m_pTarget = _pTarget;

	if(_pTarget)
	{
		m_sDestX = _pTarget->GetObjx();
		m_sDestY = _pTarget->GetObjy();
	}
	m_sFireX = _pFire->GetObjx();
	m_sFireY = _pFire->GetObjy();
	m_fFlyX = _pFire->GetObjx();
	m_fFlyY = _pFire->GetObjy();
	m_dwAniTime = _dwAniTime;
	m_dwCreateTime = GetTickCount();

	float fAtkInterval = _pFire->GetAttackInterval();
	DWORD dwSelfFrame = 6;
	/*if(m_pRenderInfo->selfframe > 0)
	{
		dwSelfFrame = m_pRenderInfo->selfframe;
	}*/
	float fMgcInterval = fAtkInterval * 6 / (float)dwSelfFrame;
	m_dwSelfInterval = fMgcInterval * 1000;

	m_bFixed = false;

	int nTax = 0;
	int nTay = 0;
	if(m_sDestX != m_sFireX)
	{
		nTax = abs(m_sDestX - m_sFireX);
	}
	else
	{
		nTax = 1;
	}
	if(m_sDestY != m_sFireY)
	{
		nTay = abs(m_sDestY - m_sFireY);
	}
	else
	{
		nTay = 1;
	}
	if(nTax > nTay)
	{
		m_nDisX = (m_sDestX - m_sFireX) * (500.0f / nTax);
		m_nDisY = (m_sDestY - m_sFireY) * (500.0f / nTax);
	}
	else
	{
		m_nDisX = (m_sDestX - m_sFireX) * (500.0f / nTay);
		m_nDisY = (m_sDestY - m_sFireY) * (500.0f / nTay);
	}
	m_bDrt16 = GetDirection16(m_sFireX, m_sFireY, m_sDestX, m_sDestY);
	m_bStage = MAGIC_STAGE_WAY;
	m_dwLastUpdateWayTime = m_dwCreateTime;

	return true;
}

void Magic16DrtExplosion::Update()
{
	if(CanRemove())
	{
		return;
	}
	DWORD dwCurTick = GetTickCount();

	if(m_bStage == MAGIC_STAGE_WAY)
	{
		if(m_bCurFrame + 1 >= m_nWayFrames)
		{
			if(dwCurTick - m_dwLastUpdateTime > 40)
			{
				//	last frame
				m_bCurFrame = 0;
				m_dwLastUpdateTime = dwCurTick;
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > 40)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}

		{
			bool bCrash = false;
			DWORD dwDelta = dwCurTick - m_dwLastUpdateWayTime;

			m_sDestX = m_pTarget->GetObjx();
			m_sDestY = m_pTarget->GetObjy();
			int nTex = 0;
			int nTey = 0;

			if((int)m_fFlyX != m_sDestX)
			{
				nTex = abs(m_sDestX - (int)m_fFlyX);
			}
			else
			{
				nTex = 1;
			}
			if((int)m_fFlyY != m_sDestY)
			{
				nTey = abs(m_sDestY - (int)m_fFlyY);
			}
			else
			{
				nTey = 1;
			}

			int nNewDisX = 0;
			int nNewDisY = 0;
			if(nTex > nTey)
			{
				nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTex);
				nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTex);
			}
			else
			{
				nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTey);
				nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTey);
			}

			if(m_nDisX < nNewDisX)
			{
				m_nDisX += max(1.0f, ((float)nNewDisX - (float)m_nDisX) / 10.0f);
			}
			else if(m_nDisX > nNewDisX)
			{
				m_nDisX -= max(1.0f, ((float)m_nDisX - (float)nNewDisX) / 10.0f);
			}
			if(m_nDisY < nNewDisY)
			{
				m_nDisY += max(1.0f, ((float)nNewDisY - (float)m_nDisY) / 10.0f);
			}
			else if(m_nDisY > nNewDisY)
			{
				m_nDisY -= max(1.0f, ((float)m_nDisY - (float)nNewDisY) / 10.0f);
			}

			float fStepX = ((float)m_nDisX / 700) * dwDelta;
			float fStepY = ((float)m_nDisY / 700) * dwDelta;
			m_fFlyX += fStepX;
			m_fFlyY += fStepY;
			m_dwLastUpdateWayTime = dwCurTick;

			int nDrt = GetDirection16(m_fFlyX, m_fFlyY, m_sDestX, m_sDestY);
			int nNowDisX = abs(m_sDestX - m_fFlyX);
			int nNowDisY = abs(m_sDestY - m_fFlyY);
			if((nNowDisX <= 15 &&  nNowDisY <= 15) ||
				(nNowDisX > m_nPrevDisX && nNowDisY > m_nPrevDisY) ||
				OverThrough(m_bDrt16, nDrt))
			{
				m_dwLastUpdateTime = dwCurTick;
				m_bStage = MAGIC_STAGE_DEST;
				m_bCurFrame = 0;
			}
			else
			{
				m_nPrevDisX = nNowDisX;
				m_nPrevDisY = nNowDisY;
			}
		}
	}
	else if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCurFrame + 1 >= m_nDestFrames)
		{
			if(dwCurTick - m_dwLastUpdateTime > 40)
			{
				//	last frame
				m_bCurFrame = 0;
				m_dwLastUpdateTime = dwCurTick;

				//	remove
				m_bCanRemove = true;
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > 40)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
}

void Magic16DrtExplosion::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	GameTextureManager* pTexMgr = m_pTexMgr;
	if(NULL == pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;

	if(m_bStage == MAGIC_STAGE_WAY)
	{
		nTexIndex = m_nWayFrameBegin + m_bCurFrame;
	}
	else if(m_bStage == MAGIC_STAGE_DEST)
	{
		nTexIndex = m_nDestFrameBegin + m_bCurFrame;
	}

	tex = pTexMgr->GetTexture(nTexIndex);
	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_fFlyX, m_fFlyY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;
			if(m_bStage == MAGIC_STAGE_DEST)
			{
				nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();
			}
			else
			{
				nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();
			}
			sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
			pEffectRender->SetTexture(tex);
			pEffectRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(nTexIndex),
				pTexMgr->GetTextureHeight(nTexIndex));

			pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY - UNIT_HEIGHT);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
MagicGroundThron::MagicGroundThron()
{
	m_pRenderInfo = NULL;
	m_pEffInfo = NULL;
}

bool MagicGroundThron::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	bool bLoadOk = true;
	m_bStage = MAGIC_STAGE_DEST;
	m_dwAniTime = _dwAniTime;

	if(_nMgcID == 51 ||
		_nMgcID == 74 ||
		_nMgcID == 92)
	{
		m_pFire = _pFire;
		m_sDestX = _sDestX;
		m_sDestY = _sDestY;

		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON14);
		bLoadOk = GameInfoManager::GetInstance()->GetMonsterRenderInfo(_nMgcID, &m_pEffInfo);

		if(NULL == m_pEffInfo ||
			false == bLoadOk)
		{
			return false;
		}
		return true;
	}

	return false;
}

void MagicGroundThron::Update()
{
	DWORD dwCurTick = GetTickCount();

	if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCurFrame + 1 >= m_pEffInfo->atkeftframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > 100)
			{
				//	last frame
				m_bCurFrame = 0;
				m_bCanRemove = true;
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > 100)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
}

void MagicGroundThron::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	GameTextureManager* pTexMgr = m_pTexMgr;
	if(NULL == pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;

	nTexIndex = m_pEffInfo->atkeftbegin + m_bCurFrame;
	tex = pTexMgr->GetTexture(nTexIndex);

	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_sDestX, m_sDestY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;
			/*if(m_bStage == MAGIC_STAGE_DEST)
			{
				nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();
			}
			else*/
			//{
// 				nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
// 				nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();
			//}
			nRenderX = CENTRAL_X + (int)m_sDestX - (int)GamePlayer::GetInstance()->GetObjx();
			nRenderY = CENTRAL_Y + (int)m_sDestY - (int)GamePlayer::GetInstance()->GetObjy();

			sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
			pNormalRender->SetTexture(tex);
			pNormalRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(nTexIndex),
				pTexMgr->GetTextureHeight(nTexIndex));
			if(m_bStage != MAGIC_STAGE_SELF)
			{
				//nRenderX -= UNIT_WIDTH / 2;
				//nRenderY -= UNIT_HEIGHT / 2;
			}
			pNormalRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
MagicNormalThron::MagicNormalThron()
{
	m_pRenderInfo = NULL;
	m_nEffectBegin = 0;
	m_nEffectFrames = 0;
}

bool MagicNormalThron::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	bool bLoadOk = true;
	m_bStage = MAGIC_STAGE_DEST;
	m_dwAniTime = _dwAniTime;

	if(_nMgcID == 138)
	{
		m_pFire = _pFire;
		m_sDestX = _sDestX;
		m_sDestY = _sDestY;

		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON31);

		if(0 == m_dwAniTime)
		{
			m_nEffectBegin = 3520;
			m_nEffectFrames = 12;
		}
		else if(1 == m_dwAniTime)
		{
			m_nEffectBegin = 3540;
			m_nEffectFrames = 9;
		}
		else
		{
			return false;
		}
		return true;
	}

	return false;
}

void MagicNormalThron::Update()
{
	DWORD dwCurTick = GetTickCount();

	if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCurFrame + 1 >= m_nEffectFrames)
		{
			if(dwCurTick - m_dwLastUpdateTime > 100)
			{
				//	last frame
				m_bCurFrame = 0;
				m_bCanRemove = true;
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > 100)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
}

void MagicNormalThron::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	GameTextureManager* pTexMgr = m_pTexMgr;
	if(NULL == pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;

	nTexIndex = m_nEffectBegin + m_bCurFrame;
	tex = pTexMgr->GetTexture(nTexIndex);

	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_sDestX, m_sDestY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;
			/*if(m_bStage == MAGIC_STAGE_DEST)
			{
				nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();
			}
			else*/
			//{
// 				nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
// 				nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();
			//}
			nRenderX = CENTRAL_X + (int)m_sDestX - (int)GamePlayer::GetInstance()->GetObjx();
			nRenderY = CENTRAL_Y + (int)m_sDestY - (int)GamePlayer::GetInstance()->GetObjy();

			sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
			pNormalRender->SetTexture(tex);
			pNormalRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(nTexIndex),
				pTexMgr->GetTextureHeight(nTexIndex));
			if(m_bStage != MAGIC_STAGE_SELF)
			{
				//nRenderX -= UNIT_WIDTH / 2;
				//nRenderY -= UNIT_HEIGHT / 2;
			}
			pNormalRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
MagicLionRoar::MagicLionRoar()
{

}

bool MagicLionRoar::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	if(true == __super::Create(_nMgcID, _pFire, _pTarget, _sDestX, _sDestY, _dwAniTime))
	{
		m_bStage = MAGIC_STAGE_SELF;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
MagicSuperFire::MagicSuperFire()
{

}

bool MagicSuperFire::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	if(NULL == _pTarget)
	{
		return false;
	}
	if(true == __super::Create(_nMgcID, _pFire, _pTarget, _sDestX, _sDestY, _dwAniTime))
	{
		m_bStage = MAGIC_STAGE_DEST;
		return true;
	}
	return false;
}

void MagicSuperFire::Update()
{
	DWORD dwCurTick = GetTickCount();
	if(CanRemove())
	{
		return;
	}

	if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->destframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;
				m_bCanRemove = true;
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}
	}
	else
	{
		m_bCanRemove = true;
	}
}

void MagicSuperFire::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(m_pRenderInfo->wil + RES_MAGIC);
	if(NULL == pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;
	
	if(m_bStage == MAGIC_STAGE_DEST)
	{
		nTexIndex = m_pRenderInfo->destbegin + m_bCurFrame + m_pRenderInfo->desttotal * m_dwAniTime;
		if(m_pRenderInfo->desttotal == 0)
		{
			return;
		}
	}
	tex = pTexMgr->GetTexture(nTexIndex);
	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_fFlyX, m_fFlyY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;
			if(m_bStage == MAGIC_STAGE_DEST)
			{
				nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();
			}
			else
			{
				nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();
			}
			sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
			pEffectRender->SetTexture(tex);
			pEffectRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(nTexIndex),
				pTexMgr->GetTextureHeight(nTexIndex));
			if(m_bStage != MAGIC_STAGE_SELF)
			{
				//nRenderX -= UNIT_WIDTH / 2;
				//nRenderY -= UNIT_HEIGHT / 2;
			}
			pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void MagicKTSword::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(m_pRenderInfo->wil + RES_MAGIC);
	if(NULL == pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;

	if(m_bStage == MAGIC_STAGE_DEST)
	{
		nTexIndex = m_pRenderInfo->destbegin + m_bCurFrame + m_pRenderInfo->desttotal * m_dwAniTime;
		if(m_pRenderInfo->desttotal == 0)
		{
			return;
		}
	}
	tex = pTexMgr->GetTexture(nTexIndex);
	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_fFlyX, m_fFlyY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;
			if(m_bStage == MAGIC_STAGE_DEST)
			{
				nRenderX = CENTRAL_X + (int)m_sDestX / UNIT_WIDTH * UNIT_WIDTH - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_sDestY / UNIT_HEIGHT * UNIT_HEIGHT - (int)GamePlayer::GetInstance()->GetObjy();
			}
			else
			{
				nRenderX = CENTRAL_X + (int)m_fFlyX - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_fFlyY - (int)GamePlayer::GetInstance()->GetObjy();
			}
			sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
			pEffectRender->SetTexture(tex);
			pEffectRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(nTexIndex),
				pTexMgr->GetTextureHeight(nTexIndex));
			if(m_bStage != MAGIC_STAGE_SELF)
			{
				//nRenderX -= UNIT_WIDTH / 2;
				//nRenderY -= UNIT_HEIGHT / 2;
			}
			pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
MagicMonsPosAni::MagicMonsPosAni()
{
	m_pMonsRenderInfo = NULL;
	m_pTexMgr = NULL;
	m_dwPos = 0;
}

bool MagicMonsPosAni::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	//	note: _sDestX: begin frame _sDestY total frame _dwAniTime: speed nMacID: position
	if(NULL == _pFire)
	{
		return false;
	}
	if(_pFire->GetType() == OBJ_MONS)
	{
		GameMonster* pMonster = static_cast<GameMonster*>(_pFire);
		m_pMonsRenderInfo = pMonster->GetRenderInfo();
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON1 + m_pMonsRenderInfo->wil);

		if(m_pMonsRenderInfo != NULL &&
			m_pTexMgr != NULL)
		{
			m_sDestX = _sDestX;
			m_sDestY = _sDestY;
			m_dwAniTime = _dwAniTime;
			m_sFireX = LOWORD(_nMgcID);
			m_sFireY = HIWORD(_nMgcID);
			m_dwCreateTime = GetTickCount();
			m_dwPos = _nMgcID;
			return true;
		}
	}
	return false;
}

void MagicMonsPosAni::Update()
{
	if(GetTickCount() - m_dwCreateTime > 10 * 1000)
	{
		m_bCanRemove = true;
	}
	if(m_bCanRemove)
	{
		return;
	}
	if(m_bCurFrame == m_sDestY - 1)
	{
		//	last frame
		if(GetTickCount() - m_dwLastUpdateTime > m_dwAniTime)
		{
			m_bCanRemove = true;
			return;
		}
	}
	else
	{
		if(GetTickCount() - m_dwLastUpdateTime > m_dwAniTime)
		{
			++m_bCurFrame;
			m_dwLastUpdateTime = GetTickCount();
		}
	}
}

void MagicMonsPosAni::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	if(NULL == m_pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;

	nTexIndex = m_bCurFrame + m_sDestX;
	tex = m_pTexMgr->GetTexture(nTexIndex);
	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_sFireX, m_sFireY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;

			nRenderX = CENTRAL_X + (int)LOWORD(m_dwPos) - (int)GamePlayer::GetInstance()->GetObjx();
			nRenderY = CENTRAL_Y + (int)HIWORD(m_dwPos) - (int)GamePlayer::GetInstance()->GetObjy();

			sOftX = m_pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = m_pTexMgr->GetTextureOffsetY(nTexIndex);
			pEffectRender->SetTexture(tex);
			pEffectRender->SetTextureRect(0, 0,
				m_pTexMgr->GetTextureWidth(nTexIndex),
				m_pTexMgr->GetTextureHeight(nTexIndex));
			if(m_bStage != MAGIC_STAGE_SELF)
			{
				//nRenderX -= UNIT_WIDTH / 2;
				//nRenderY -= UNIT_HEIGHT / 2;
			}
			pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
MagicMonsAni::MagicMonsAni()
{
	m_pTexMgr = NULL;
	m_pMonsRenderInfo = NULL;
}

bool MagicMonsAni::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	//	note: _sDestX: begin frame _sDestY total frame _dwAniTime: speed
	if(NULL == _pFire)
	{
		return false;
	}
	if(_pFire->GetType() == OBJ_MONS)
	{
		GameMonster* pMonster = static_cast<GameMonster*>(_pFire);
		m_pMonsRenderInfo = pMonster->GetRenderInfo();
		m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON1 + m_pMonsRenderInfo->wil);

		if(m_pMonsRenderInfo != NULL &&
			m_pTexMgr != NULL)
		{
			m_sDestX = _sDestX;
			m_sDestY = _sDestY;
			m_dwAniTime = _dwAniTime;
			m_sFireX = _pFire->GetObjx();
			m_sFireY = _pFire->GetObjy();
			m_dwCreateTime = GetTickCount();
			m_fFlyX = m_sFireX;
			m_fFlyY = m_sFireY;
			return true;
		}
	}
	return false;
}

void MagicMonsAni::Update()
{
	if(GetTickCount() - m_dwCreateTime > 10 * 1000)
	{
		m_bCanRemove = true;
	}
	if(m_bCanRemove)
	{
		return;
	}
	if(m_bCurFrame == m_sDestY - 1)
	{
		//	last frame
		if(GetTickCount() - m_dwLastUpdateTime > m_dwAniTime)
		{
			m_bCanRemove = true;
			return;
		}
	}
	else
	{
		if(GetTickCount() - m_dwLastUpdateTime > m_dwAniTime)
		{
			++m_bCurFrame;
			m_dwLastUpdateTime = GetTickCount();
		}
	}
}

void MagicMonsAni::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	if(NULL == m_pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;
	
	nTexIndex = m_bCurFrame + m_sDestX;
	tex = m_pTexMgr->GetTexture(nTexIndex);
	if(tex)
	{
		RECT rcView;
		rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
		rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
		rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
		rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

		POINT ptDraw = {m_sFireX, m_sFireY};
		if(PtInRect(&rcView, ptDraw))
		{
			int nRenderX = 0;
			int nRenderY = 0;

			nRenderX = CENTRAL_X + (int)m_sFireX - (int)GamePlayer::GetInstance()->GetObjx();
			nRenderY = CENTRAL_Y + (int)m_sFireY - (int)GamePlayer::GetInstance()->GetObjy();

			sOftX = m_pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = m_pTexMgr->GetTextureOffsetY(nTexIndex);
			pEffectRender->SetTexture(tex);
			pEffectRender->SetTextureRect(0, 0,
				m_pTexMgr->GetTextureWidth(nTexIndex),
				m_pTexMgr->GetTextureHeight(nTexIndex));
			if(m_bStage != MAGIC_STAGE_SELF)
			{
				//nRenderX -= UNIT_WIDTH / 2;
				//nRenderY -= UNIT_HEIGHT / 2;
			}
			pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
MagicHumEff::MagicHumEff()
{
	
}

bool MagicHumEff::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	//	note: _nMgcID:sib id, _sDestX:begin frame _sDestY:frame count _pTarget:MagicID
	if(NULL == _pFire)
	{
		return false;
	}

	m_pTexMgr = GameResourceManager::GetInstance()->GetTexs(_nMgcID);
	m_sDestX = _sDestX;
	m_sDestY = _sDestY;
	m_pFire = _pFire;
	m_dwAniTime = _dwAniTime;
	m_dwCreateTime = GetTickCount();
	m_wMgcID = (WORD)_pTarget;

	return true;
}

void MagicHumEff::Update()
{
	if(GetTickCount() - m_dwLastUpdateTime > 50)
	{
		if(GetTickCount() - m_dwCreateTime > m_dwAniTime)
		{
			//	Remove
			m_bCanRemove = true;
		}
		else
		{
			if(m_bCurFrame == m_sDestY - 1)
			{
				m_bCurFrame = 0;
			}
			else
			{
				++m_bCurFrame;
			}
			m_dwLastUpdateTime = GetTickCount();
		}
	}
}

void MagicHumEff::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	if(NULL == m_pFire)
	{
		return;
	}

	if(NULL == m_pTexMgr)
	{
		return;
	}

	HTEXTURE tex = 0;

	int nTexIdx = m_sDestX + m_bCurFrame;
	tex = m_pTexMgr->GetTexture(nTexIdx);
	int nRenderX = 0;
	int nRenderY = 0;

	if(tex)
	{
		pEffectRender->SetTexture(tex);
		pEffectRender->SetTextureRect(0, 0, m_pTexMgr->GetTextureWidth(nTexIdx), m_pTexMgr->GetTextureHeight(nTexIdx));

		nRenderX = m_pFire->GetObjx() + m_pTexMgr->GetTextureOffsetX(nTexIdx) + CENTRAL_X - (int)GamePlayer::GetInstance()->GetObjx();;
		nRenderY = m_pFire->GetObjy() + m_pTexMgr->GetTextureOffsetY(nTexIdx) + CENTRAL_Y - (int)GamePlayer::GetInstance()->GetObjy();

		pEffectRender->Render(nRenderX, nRenderY);
	}
}
//////////////////////////////////////////////////////////////////////////
MagicHumState::MagicHumState()
{

}

bool MagicHumState::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	if(NULL == _pFire)
	{
		return false;
	}

	if(false == GameInfoManager::GetInstance()->GetMagicRenderInfo(_nMgcID, &m_pRenderInfo))
	{
		return false;
	}

	if(NULL == _pTarget)
	{
		return false;
	}

	m_pTexMgr = GameResourceManager::GetInstance()->GetTexs((int)_pTarget);
	m_sDestX = _sDestX;
	m_sDestY = _sDestY;
	m_pFire = _pFire;
	m_dwAniTime = _dwAniTime;
	m_dwCreateTime = GetTickCount();
	m_wMgcID = _nMgcID;

	return true;
}

void MagicHumState::Update()
{
	if(GetTickCount() - m_dwLastUpdateTime > 50)
	{
		if(GetTickCount() - m_dwCreateTime > m_dwAniTime)
		{
			//	Remove
			m_bCanRemove = true;
		}
		else
		{
			if(m_bCurFrame == m_sDestY - 1)
			{
				m_bCurFrame = 0;
			}
			else
			{
				++m_bCurFrame;
			}
			m_dwLastUpdateTime = GetTickCount();
		}
	}
}

void MagicHumState::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	if(NULL == m_pFire)
	{
		return;
	}

	if(NULL == m_pTexMgr)
	{
		return;
	}

	HTEXTURE tex = 0;

	int nTexIdx = m_sDestX + m_bCurFrame;
	tex = m_pTexMgr->GetTexture(nTexIdx);
	int nRenderX = 0;
	int nRenderY = 0;

	if(tex)
	{
		pEffectRender->SetTexture(tex);
		pEffectRender->SetTextureRect(0, 0, m_pTexMgr->GetTextureWidth(nTexIdx), m_pTexMgr->GetTextureHeight(nTexIdx));

		nRenderX = m_pFire->GetObjx() + m_pTexMgr->GetTextureOffsetX(nTexIdx) + CENTRAL_X - (int)GamePlayer::GetInstance()->GetObjx();;
		nRenderY = m_pFire->GetObjy() + m_pTexMgr->GetTextureOffsetY(nTexIdx) + CENTRAL_Y - (int)GamePlayer::GetInstance()->GetObjy();

		pEffectRender->Render(nRenderX, nRenderY);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool MagicBigPoison::Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime)
{
	m_bStage = MAGIC_STAGE_SELF;
	m_bFixed = false;
	m_bFirstLoop = true;
	return __super::Create(_nMgcID, _pFire, _pTarget, _sDestX, _sDestY, _dwAniTime);
}

void MagicBigPoison::Update()
{
	DWORD dwCurTick = GetTickCount();

	if(m_bStage == MAGIC_STAGE_DEST)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->destframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;
				m_dwLastUpdateTime = dwCurTick;

				if(m_dwLastUpdateTime > m_dwAniTime)
				{
					m_bCanRemove = true;
				}
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;

				if(m_dwLastUpdateTime > m_dwAniTime)
				{
					m_bCanRemove = true;
				}
			}
		}
	}
	else if(m_bStage == MAGIC_STAGE_WAY)
	{
		if(m_bCurFrame + 1 >= m_pRenderInfo->wayframe)
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				//	last frame
				m_bCurFrame = 0;

				/*if(m_pRenderInfo->destframe == 0)
				{
					m_bCanRemove = true;
				}
				else
				{
					m_bStage = MAGIC_STAGE_WAY;
					m_dwLastUpdateTime = dwCurTick;
				}*/
				m_dwLastUpdateTime = dwCurTick;

				//	update the position
			}
		}
		else
		{
			if(dwCurTick - m_dwLastUpdateTime > m_pRenderInfo->speed)
			{
				IncFrame();
				m_dwLastUpdateTime = dwCurTick;
			}
		}

		if(m_pTarget == NULL)
		{
			if(m_wMgcID == MEFF_CHARMAC ||
				m_wMgcID == MEFF_CHARMMC)
			{
				bool bCrash = false;
				DWORD dwDelta = dwCurTick - m_dwLastUpdateWayTime;

				//m_sDestX = m_pTarget->GetObjx();
				//m_sDestY = m_pTarget->GetObjy();
				int nTex = 0;
				int nTey = 0;

				if((int)m_fFlyX != m_sDestX)
				{
					nTex = abs(m_sDestX - (int)m_fFlyX);
				}
				else
				{
					nTex = 1;
				}
				if((int)m_fFlyY != m_sDestY)
				{
					nTey = abs(m_sDestY - (int)m_fFlyY);
				}
				else
				{
					nTey = 1;
				}

				int nNewDisX = 0;
				int nNewDisY = 0;
				if(nTex > nTey)
				{
					nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTex);
					nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTex);
				}
				else
				{
					nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTey);
					nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTey);
				}

				if(m_nDisX < nNewDisX)
				{
					m_nDisX += max(1.0f, ((float)nNewDisX - (float)m_nDisX) / 10.0f);
				}
				else if(m_nDisX > nNewDisX)
				{
					m_nDisX -= max(1.0f, ((float)m_nDisX - (float)nNewDisX) / 10.0f);
				}
				if(m_nDisY < nNewDisY)
				{
					m_nDisY += max(1.0f, ((float)nNewDisY - (float)m_nDisY) / 10.0f);
				}
				else if(m_nDisY > nNewDisY)
				{
					m_nDisY -= max(1.0f, ((float)m_nDisY - (float)nNewDisY) / 10.0f);
				}

				float fStepX = ((float)m_nDisX / 700) * dwDelta;
				float fStepY = ((float)m_nDisY / 700) * dwDelta;
				m_fFlyX += fStepX;
				m_fFlyY += fStepY;
				m_dwLastUpdateWayTime = dwCurTick;

				int nDrt = GetDirection16(m_fFlyX, m_fFlyY, m_sDestX, m_sDestY);
				int nNowDisX = abs(m_sDestX - m_fFlyX);
				int nNowDisY = abs(m_sDestY - m_fFlyY);
				if((nNowDisX <= 15 &&  nNowDisY <= 15) ||
					(nNowDisX >= m_nPrevDisX && nNowDisY >= m_nPrevDisY) ||
					OverThrough(m_bDrt16, nDrt))
				{
					m_dwLastUpdateTime = dwCurTick;
					if(m_pRenderInfo->destframe != 0)
					{
						m_bStage = MAGIC_STAGE_DEST;
						m_bCurFrame = 0;
						m_dwAniTime = dwCurTick + m_dwAniTime;

						if(m_pRenderInfo->destframe != 0 &&
							IsSoundEnable() &&
							InPlayRegion())
						{
							GameSoundManager::GetInstancePtr()->PlayMagicSound(m_wMgcID, MAGIC_STAGE_DEST);
						}
					}
					else
					{
						m_bCanRemove = true;
					}
				}
				else
				{
					m_nPrevDisX = nNowDisX;
					m_nPrevDisY = nNowDisY;
				}
			}
			else
			{
				DWORD dwDelta = dwCurTick - m_dwLastUpdateWayTime;
				float fStepX = ((float)m_nDisX / 900) * dwDelta;
				float fStepY = ((float)m_nDisY / 900) * dwDelta;
				m_fFlyX += fStepX;
				m_fFlyY += fStepY;
				m_dwLastUpdateWayTime = dwCurTick;
			}
		}
		else
		{
			bool bCrash = false;
			DWORD dwDelta = dwCurTick - m_dwLastUpdateWayTime;

			m_sDestX = m_pTarget->GetObjx();
			m_sDestY = m_pTarget->GetObjy();
			int nTex = 0;
			int nTey = 0;

			if((int)m_fFlyX != m_sDestX)
			{
				nTex = abs(m_sDestX - (int)m_fFlyX);
			}
			else
			{
				nTex = 1;
			}
			if((int)m_fFlyY != m_sDestY)
			{
				nTey = abs(m_sDestY - (int)m_fFlyY);
			}
			else
			{
				nTey = 1;
			}

			int nNewDisX = 0;
			int nNewDisY = 0;
			if(nTex > nTey)
			{
				nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTex);
				nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTex);
			}
			else
			{
				nNewDisX = ((float)m_sDestX - m_fFlyX) * (500.0f / nTey);
				nNewDisY = ((float)m_sDestY - m_fFlyY) * (500.0f / nTey);
			}

			if(m_nDisX < nNewDisX)
			{
				m_nDisX += max(1.0f, ((float)nNewDisX - (float)m_nDisX) / 10.0f);
			}
			else if(m_nDisX > nNewDisX)
			{
				m_nDisX -= max(1.0f, ((float)m_nDisX - (float)nNewDisX) / 10.0f);
			}
			if(m_nDisY < nNewDisY)
			{
				m_nDisY += max(1.0f, ((float)nNewDisY - (float)m_nDisY) / 10.0f);
			}
			else if(m_nDisY > nNewDisY)
			{
				m_nDisY -= max(1.0f, ((float)m_nDisY - (float)nNewDisY) / 10.0f);
			}

			float fStepX = ((float)m_nDisX / 700) * dwDelta;
			float fStepY = ((float)m_nDisY / 700) * dwDelta;
			m_fFlyX += fStepX;
			m_fFlyY += fStepY;
			m_dwLastUpdateWayTime = dwCurTick;

			int nDrt = GetDirection16(m_fFlyX, m_fFlyY, m_sDestX, m_sDestY);
			int nNowDisX = abs(m_sDestX - m_fFlyX);
			int nNowDisY = abs(m_sDestY - m_fFlyY);
			if((nNowDisX <= 15 &&  nNowDisY <= 15) ||
				(nNowDisX > m_nPrevDisX && nNowDisY > m_nPrevDisY) ||
				OverThrough(m_bDrt16, nDrt))
			{
				m_dwLastUpdateTime = dwCurTick;
				if(m_pRenderInfo->destframe != 0)
				{
					m_bStage = MAGIC_STAGE_DEST;
					m_bCurFrame = 0;
					m_dwAniTime = m_dwAniTime + dwCurTick;

					if(m_pRenderInfo->destframe != 0 &&
						IsSoundEnable() &&
						InPlayRegion())
					{
						GameSoundManager::GetInstancePtr()->PlayMagicSound(m_wMgcID, MAGIC_STAGE_DEST);
					}
				}
				else
				{
					m_bCanRemove = true;
				}
			}
			else
			{
				m_nPrevDisX = nNowDisX;
				m_nPrevDisY = nNowDisY;
			}
		}
	}
	else
	{
		__super::Update();
	}
}

void MagicBigPoison::Render()
{
	__super::Render();
}

//////////////////////////////////////////////////////////////////////////
bool EffectAttackNumber::Init(int _nNumber, DWORD _dwCoordX, DWORD _dwCoordY, bool _bAdd, bool _bCritical)
{
	m_bCanRemove = false;
	int nNumber = _nNumber;
	m_nNumber = _nNumber;
	m_dwCoordX = _dwCoordX;
	m_dwCoordY = _dwCoordY;
	m_bAdd = _bAdd;
	m_dwCreateTime = GetTickCount();
	m_nBits = 0;
	m_dwAlpha = 255;
	m_bCritical = _bCritical;
	m_bIsHero = false;

	if(0 == s_pSharedNumberTexture)
	{
		hgeResourceManager* pResMgr = pTheGame->GetGameRes();
		s_pSharedNumberTexture = pResMgr->GetTexture("attacknum");
		if(s_pSharedNumberTexture)
		{
			s_pSharedEffectRender->SetTexture(s_pSharedNumberTexture);
			s_pSharedEffectRender->SetTextureRect(0, 0, 16, 16);
		}
		else
		{
			return false;
		}
	}

	while(nNumber)
	{
		int nBit = nNumber % 10;
		m_xNumbers.push_back(nBit);
		nNumber /= 10;
	}

	std::reverse(m_xNumbers.begin(), m_xNumbers.end());
	m_nBits = m_xNumbers.size();
	if(m_nBits == 0)
	{
		return false;
	}

	return true;
}

void EffectAttackNumber::Update()
{
	if(m_bCanRemove)
	{
		return;
	}

	//DWORD dwTick = GetTickCount();
}

void EffectAttackNumber::Render()
{
	if(m_bCanRemove)
	{
		return;
	}

	RECT rcView;
	rcView.left = (int)GamePlayer::GetInstance()->GetObjx() - VIEW_WIDTH / 2 - UNIT_WIDTH;
	rcView.right = (int)GamePlayer::GetInstance()->GetObjx() + VIEW_WIDTH / 2 + UNIT_WIDTH;
	rcView.top = (int)GamePlayer::GetInstance()->GetObjy() - VIEW_HEIGHT / 2 - UNIT_HEIGHT;
	rcView.bottom = (int)GamePlayer::GetInstance()->GetObjy() + VIEW_HEIGHT / 2 + UNIT_HEIGHT;

	int nPosX = m_dwCoordX * UNIT_WIDTH + UNIT_WIDTH / 2;
	int nPosY = m_dwCoordY * UNIT_HEIGHT - UNIT_HEIGHT * 2;

	DWORD dwLastTime = GetTickCount() - m_dwCreateTime;
	if(dwLastTime > 10 * 1000)
	{
		m_bCanRemove = true;
		return;
	}

	nPosY -= dwLastTime / 8;
	m_dwAlpha = 255 - dwLastTime / 8;
	if(m_dwAlpha > 255)
	{
		m_dwAlpha = 0;
		m_bCanRemove = true;
		return;
	}

	POINT ptDraw = {nPosX, nPosY};
	if(PtInRect(&rcView, ptDraw))
	{
		int nRenderX = 0;
		int nRenderY = 0;

		nRenderX = CENTRAL_X + (int)ptDraw.x - (int)GamePlayer::GetInstance()->GetObjx() - 6 * m_nBits;
		nRenderY = CENTRAL_Y + (int)ptDraw.y - (int)GamePlayer::GetInstance()->GetObjy();

		if(m_bCritical)
		{
			nRenderX = CENTRAL_X + (int)ptDraw.x - (int)GamePlayer::GetInstance()->GetObjx() - 16 * m_nBits;
		}

		std::list<int>::const_iterator begIter = m_xNumbers.begin();
		std::list<int>::const_iterator endIter = m_xNumbers.end();

		if(!m_bAdd)
		{
			if(m_bCritical)
			{
				s_pSharedEffectRender->SetColor(ARGB(m_dwAlpha, 255, 255, 0));
			}
			else
			{
				if(!m_bIsHero)
				{
					s_pSharedEffectRender->SetColor(ARGB(m_dwAlpha, 255, 0, 0));
				}
				else
				{
					s_pSharedEffectRender->SetColor(ARGB(m_dwAlpha, 0xcd, 00, 0xcd));
				}
			}
		}
		else
		{
			s_pSharedEffectRender->SetColor(ARGB(m_dwAlpha, 0, 255, 0));
		}

		int nBitSeq = 0;
		for(begIter;
			begIter != endIter;
			++begIter)
		{
			if(!m_bCritical)
			{
				int nBit = *begIter;
				int nTextX = nBit * 16;
				s_pSharedEffectRender->SetTextureRect(nTextX, 0, 16, 16);
				s_pSharedEffectRender->Render(nRenderX + nBitSeq * 12, nRenderY);
			}
			else
			{
				int nBit = *begIter;
				int nTextX = nBit * 16;
				s_pSharedEffectRender->SetTextureRect(nTextX, 0, 16, 16);
				s_pSharedEffectRender->RenderStretch(nRenderX + nBitSeq * 26, nRenderY, nRenderX + nBitSeq * 26 + 32, nRenderY + 32);
			}
			
			++nBitSeq;
		}
	}
	else
	{
		m_bCanRemove = true;
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
bool EffectHumCloth::Init(GameObject* _pAttachObj, int _nClothID)
{
	m_nClothID = _nClothID;
	m_wMgcID = _nClothID;
	m_bCanRemove = false;

	if(!GameInfoManager::GetInstance()->GetEffectHumInfo(m_nClothID, &m_pEffectHumInfo))
	{
		return false;
	}

	m_pTarget = _pAttachObj;
	m_dwLastUpdateTime = 0;

	

	return true;
}

void EffectHumCloth::Render()
{
	if(GetTickCount() < m_dwNextExecuteTime)
	{
		return;
	}

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(m_pEffectHumInfo->wil + RES_EFFECT);
	if(NULL == pTexMgr)
	{
		return;
	}
	HTEXTURE tex = 0;
	int nTexIndex = 0;
	short sOftX = 0;
	short sOftY = 0;
	nTexIndex = m_pEffectHumInfo->effectbegin + m_bCurFrame;
	tex = pTexMgr->GetTexture(nTexIndex);
	if(tex)
	{
		{
			int nRenderX = 0;
			int nRenderY = 0;

			{
				nRenderX = CENTRAL_X + (int)m_pTarget->GetObjx() - (int)GamePlayer::GetInstance()->GetObjx();
				nRenderY = CENTRAL_Y + (int)m_pTarget->GetObjy() - (int)GamePlayer::GetInstance()->GetObjy();
			}
			sOftX = pTexMgr->GetTextureOffsetX(nTexIndex);
			sOftY = pTexMgr->GetTextureOffsetY(nTexIndex);
			pEffectRender->SetTexture(tex);
			pEffectRender->SetTextureRect(0, 0,
				pTexMgr->GetTextureWidth(nTexIndex),
				pTexMgr->GetTextureHeight(nTexIndex));

			pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}

void EffectHumCloth::Update()
{
	if(GetTickCount() < m_dwNextExecuteTime)
	{
		return;
	}

	if(GetTickCount() - m_dwLastUpdateTime > m_pEffectHumInfo->speed)
	{
		m_dwLastUpdateTime = GetTickCount();

		++m_bCurFrame;

		if(m_bCurFrame >= m_pEffectHumInfo->effectframe)
		{
			m_bCurFrame = 0;

			if(m_dwAniInterval != 0)
			{
				m_dwNextExecuteTime = GetTickCount() + m_dwAniInterval;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool SequenceEffect::Init(int _nResID, int _nTextStartID, int _nFrames, int _nSpeed, int _nLoopTimes)
{
	m_pResPack = GameResourceManager::GetInstance()->GetTexs(_nResID);
	if(NULL == m_pResPack)
	{
		return false;
	}

	m_nTextStartID = _nTextStartID;
	m_nFrames = _nFrames;
	m_nSpeed = _nSpeed;
	m_nLoopTimes = _nLoopTimes;

	m_dwLastUpdateTime = GetTickCount();
	return true;
}

void SequenceEffect::Update()
{
	if(m_nCurrentLoopTimes >= m_nLoopTimes)
	{
		m_bCanRemove = true;
		return;
	}

	if(GetTickCount() - m_dwLastUpdateTime > m_nSpeed)
	{
		m_dwLastUpdateTime = GetTickCount();
		++m_bCurFrame;
		if(m_bCurFrame >= m_nFrames)
		{
			++m_nCurrentLoopTimes;
			m_bCurFrame = 0;
		}

		if(m_nCurrentLoopTimes >= m_nLoopTimes)
		{
			return;
		}
	}
}

void SequenceEffect::Render()
{
	if(m_nCurrentLoopTimes >= m_nLoopTimes)
	{
		m_bCanRemove = true;
		return;
	}

	int nIndex = m_nTextStartID + m_bCurFrame;
	HTEXTURE tex = m_pResPack->GetTexture(nIndex);

	if(tex)
	{
		int nRenderX = m_fFlyX * UNIT_WIDTH + m_pResPack->GetTextureOffsetX(nIndex) + CENTRAL_X - (int)GamePlayer::GetInstance()->GetObjx();
		int nRenderY = m_fFlyY * UNIT_HEIGHT + m_pResPack->GetTextureOffsetY(nIndex) + CENTRAL_Y - (int)GamePlayer::GetInstance()->GetObjy();
		pEffectRender->SetTexture(tex);
		pEffectRender->SetTextureRect(0, 0,
			m_pResPack->GetTextureWidth(nIndex),
			m_pResPack->GetTextureHeight(nIndex));

		pEffectRender->Render(nRenderX, nRenderY);
	}
}