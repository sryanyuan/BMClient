#include "GameMonster.h"
#include "GameTextureManager.h"
#include "GameMapManager.h"
#include "GamePlayer.h"
#include "GameInfoManager.h"
#include <math.h>
#include <assert.h>
#include "GameResourceManager.h"
#include "GameScene.h"
#include "../BackMir/BackMir.h"
#include "../../CommonModule/SimpleActionHelper.h"
#include "../Common/OutlineTextureManager.h"

// int GameMonster::CalMonWilIndex()
// {
// 	int id = m_attrib.id;
// 	return id;
// }
GameMonster::GameMonster()
{
	SetType(OBJ_MONS);
	ZeroMemory(&m_indexs, sizeof(m_indexs));
	m_pSpr = NULL;
	m_texs = NULL;
	//ZeroMemory(&m_renderInfo, sizeof(RenderInfo));
	m_bRenderName = false;
	m_fLastUpdateTime = 0.0f;
	m_fLastUpdateAttack = 0.0f;
	m_fLastUpdateDead = 0.0f;
	m_fLastUpdateNoWeapon = 0.0f;
	m_fLastAtkTime = 0.0f;
	m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
	m_nHatred = -1;
	m_dwMoveInterval = m_dwLastMoveTime = 0;
	m_dwAttackInterval = m_dwLastAttackTime = 0;
	m_pRenderInfo = NULL;
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_nMasterId = 0;
}

GameMonster::~GameMonster()
{
	SAFE_DELETE(m_pRender);
}

int GameMonster::CalMonIndex()
{
	int nIndex = 0;

	//if(m_renderInfo.drts == 4)
	{
		switch(GetStatus())
		{
		case PST_STAND:
			{
				nIndex = m_pRenderInfo->standbegin + m_drt *m_pRenderInfo->standtotal;
			}break;
		case PST_WALK:
			{
				nIndex = m_pRenderInfo->walkbegin + m_drt * m_pRenderInfo->walktotal;
			}break;
		case PST_ATTACKNOWEAPON:
			{
				nIndex = m_pRenderInfo->atkbegin + m_drt * m_pRenderInfo->atktotal;
			}break;
		case PST_ATTACKED:
			{
				nIndex = m_pRenderInfo->hurtbegin + m_drt * m_pRenderInfo->hurttotal;
			}break;
		case PST_DEAD:
			{
				nIndex = m_pRenderInfo->deadbegin + m_drt * m_pRenderInfo->deadtotal;
			}break;
		}
	}
// 	else if(m_renderInfo.drts == 1)
// 	{
// 		switch(GetStatus())
// 		{
// 		case PST_STAND:
// 			{
// 				nIndex = m_renderInfo.standbegin;
// 			}break;
// 		case PST_WALK:
// 			{
// 				nIndex = m_renderInfo.walkbegin;
// 			}break;
// 		case PST_ATTACKNOWEAPON:
// 			{
// 				nIndex = m_renderInfo.atkbegin;
// 			}break;
// 		case PST_ATTACKED:
// 			{
// 				nIndex = m_renderInfo.hurtbegin;
// 			}break;
// 		case PST_DEAD:
// 			{
// 				nIndex = m_renderInfo.deadbegin;
// 			}break;
// 		}
// 	}
	
	return nIndex;
}

int GameMonster::CalAtkEftIndex()
{
	return m_pRenderInfo->atkeftbegin + m_drt * m_pRenderInfo->atkefttotal;
}

void GameMonster::UpdateTextureIndex()
{
	//	hair当做wil索引
	//m_indexs.wHair = CalMonWilIndex();
	//	hum当做怪物索引
	m_indexs.wHum = CalMonIndex();
}

HTEXTURE GameMonster::GetCurTexture()
{
	if(m_texs)
	{
		return m_texs->GetTexture(CalMonIndex() + m_bCurFrame);
	}
	return 0;
}

// bool GameMonster::IsSelected(const POINT& _pt)
// {
// 	GamePlayer* pPlayer = GamePlayer::GetInstance();
// 	int nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
// 	int nDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
// 	
// 	HTEXTURE tex = GetCurTexture();
// 	if(tex)
// 	{
// 		short sOffsetX = m_texs->GetTextureOffsetXEx(CalMonIndex() + m_bCurFrame);
// 		short sOffsetY = m_texs->GetTextureOffsetYEx(CalMonIndex() + m_bCurFrame);
// 		short sWidth = m_texs->GetTextureWidthEx(CalMonIndex() + m_bCurFrame);
// 		short sHeight = m_texs->GetTextureHeightEx(CalMonIndex() + m_bCurFrame);
// 
// 		//	缩小4像素 生成检测矩形
// 		RECT rcMons;
// 		rcMons.left = nDrawX + sOffsetX + 2;
// 		rcMons.right = rcMons.left + sWidth - 2;
// 		rcMons.top = nDrawY + sOffsetY + 2;
// 		rcMons.bottom = rcMons.top + sHeight - 2;
// 		if(!::PtInRect(&rcMons, _pt))
// 		{
// 			return false;
// 		}
// 
// 		if(rcMons.left > rcMons.right ||
// 			rcMons.top > rcMons.bottom)
// 		{
// 			return false;
// 		}
// 
// 		//	进行像素检测
// 		//	还原相对矩形
// 		POINT ptDetect = _pt;
// 		ptDetect.x = _pt.x - (nDrawX + sOffsetX);
// 		ptDetect.y = _pt.y - (nDrawY + sOffsetY);
// 		if(ptDetect.x < 0 ||
// 			ptDetect.y < 0)
// 		{
// 			return false;
// 		}
// 		DWORD dwActlWidth = AfxGetHge()->Texture_GetWidth(tex);
// 		DWORD dwActlHeight = AfxGetHge()->Texture_GetHeight(tex);
// 		DWORD* pColValue = AfxGetHge()->Texture_Lock(tex);
// 	
// 		if((pColValue[ptDetect.y * dwActlWidth + ptDetect.x] & 0xFF000000) != 0 &&
// 			(pColValue[ptDetect.y * dwActlWidth + ptDetect.x + 1] & 0xFF000000) != 0 &&
// 			(pColValue[ptDetect.y * dwActlWidth + ptDetect.x - 1] & 0xFF000000) != 0 &&
// 			(pColValue[(ptDetect.y + 1) * dwActlWidth + ptDetect.x] & 0xFF000000) != 0 &&
// 			(pColValue[(ptDetect.y - 1) * dwActlWidth + ptDetect.x] & 0xFF000000) != 0)
// 		{
// 			AfxGetHge()->Texture_Unlock(tex);
// 			return true;
// 		}
// 		AfxGetHge()->Texture_Unlock(tex);
// 	}
// 
// 	return false;
// }

void GameMonster::Render()
{
	HGE* hge = AfxGetHge();
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	if(!IsVisible())
	{
		return;
	}

	if(m_texs)
	{
		GameTextureManager* res = m_texs;
		//	画怪物
		int nTextureIndex = CalMonIndex() + m_bCurFrame;
		HTEXTURE tex = res->GetTexture(nTextureIndex);
		short sOffsetX = 0;
		short sOffsetY = 0;
		int nDrawOffsetX = -1;
		int nDrawOffsetY = -1;
		int nTextureWidth = 0;
		int nTextureHeight = 0;

		if(tex)
		{
			nTextureWidth = res->GetTextureWidth(nTextureIndex);
			nTextureHeight = res->GetTextureHeight(nTextureIndex);
			sOffsetX = res->GetTextureOffsetX(nTextureIndex);
			sOffsetY = res->GetTextureOffsetY(nTextureIndex);

			//	是否是目标
			if(this == GameScene::sThis->GetTarget())
			{
				HTEXTURE texSelected = SelectedTextureManager::GetInstance()->GetSelectedTextureMonster(tex, GetAttrib()->id, nTextureIndex);
				if(NULL != texSelected)
				{
					tex = texSelected;
				}
			}

			if(!m_pRender)
			{
				//m_pSpr = new hgeSprite(tex, 0, 0, res->GetTextureWidth(tex), res->GetTextureHeight(tex));
				m_pRender = new hgeSprite(tex, 0, 0, nTextureWidth,
					nTextureHeight);
			}
			else
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, nTextureWidth, 
					nTextureHeight);
			}
			
			//	根据玩家位置 得先算出来正常矩形的位置
			//nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
			//nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
			nDrawOffsetX = GetScreenPosX();
			nDrawOffsetY = GetScreenPosY();

			//
			m_pRender->Render(nDrawOffsetX + sOffsetX,
				nDrawOffsetY + sOffsetY);

			//	记录当前纹理
			m_nCurrentTextureIndex = nTextureIndex;
		}

		//	攻击效果

// 		if(m_renderInfo.atkeftframe &&
// 			GetStatus() == PST_ATTACKNOWEAPON)
// 		{
// 			tex = res->GetTexture(CalAtkEftIndex() + m_bCurFrame);
// 			if(tex)
// 			{
// 				m_pSpr->SetTexture(tex);
// 				m_pSpr->SetTextureRect(0, 0, res->GetTextureWidth(tex), res->GetTextureHeight(tex));
// 				//m_pSpr->SetBlendMode(BLEND_COLORADD | BLEND_COLORMUL | BLEND_NOZWRITE);
// 				m_pSpr->SetBlendMode(BLEND_ALPHAADD | BLEND_NOZWRITE);
// 				sOffsetX = res->GetTextureOffsetX(tex);
// 				sOffsetY = res->GetTextureOffsetY(tex);
// 				m_pSpr->Render(x + sOffsetX - (GetCoordX() * UNIT_WIDTH - GetObjx())
// 					, y + sOffsetY - (GetCoordY() * UNIT_HEIGHT - GetObjy()));
// 				m_pSpr->SetBlendMode(BLEND_DEFAULT);
// 			}
// 		}


		//	名字
// 		if(m_bRenderName)
// 		{
// 			AfxGetFont()->Print(nDrawOffsetX,
// 				nDrawOffsetY -16, m_attrib.name);
// 			m_bRenderName = false;
// 		}

		//	血条
// 		GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_PRGUSE2);
// 		if(!ttex)
// 		{
// 			return;
// 		}
// 		float fScale = 0.0f;
// 		fScale = ((float)GetAttrib()->HP / (float)GetAttrib()->maxHP);
// 		//tex = ttex->GetTexture(0);
// 		tex = ttex->GetTexture(TEX_HUM_HPTUBEBLANK);
// 		if(tex &&
// 			GetStatus() != PST_DEAD)
// 		{
// 			m_pSpr->SetTexture(tex);
// 			m_pSpr->SetTextureRect(0, 0, ttex->GetTextureWidthEx(TEX_HUM_HPTUBEBLANK),
// 				ttex->GetTextureHeightEx(TEX_HUM_HPTUBEBLANK));
// 			sOffsetX = ttex->GetTextureOffsetXEx(TEX_HUM_HPTUBEBLANK);
// 			sOffsetY = ttex->GetTextureOffsetYEx(TEX_HUM_HPTUBEBLANK);
// 
// 			m_pSpr->Render(nDrawOffsetX + sOffsetX
// 				, nDrawOffsetY - 15 + sOffsetY);
// 			AfxGetFont()->Print(nDrawOffsetX + sOffsetX
// 				, nDrawOffsetY + sOffsetY - 25,
// 				"%d/%d", m_attrib.HP, m_attrib.maxHP);
// 		}
// 		//tex = ttex->GetTexture(1);
//  		tex = ttex->GetTexture(TEX_HUM_HPTUBEFULL);
//  		if(tex &&
//  			GetStatus() != PST_DEAD)
//  		{
//  			m_pSpr->SetTexture(tex);
//  			m_pSpr->SetTextureRect(0, 0, fScale * ttex->GetTextureWidthEx(TEX_HUM_HPTUBEFULL), 
// 				ttex->GetTextureHeightEx(TEX_HUM_HPTUBEFULL));
//  			sOffsetX = ttex->GetTextureOffsetXEx(TEX_HUM_HPTUBEFULL);
//  			sOffsetY = ttex->GetTextureOffsetYEx(TEX_HUM_HPTUBEFULL);
// 			m_pSpr->Render(nDrawOffsetX + sOffsetX
// 				, nDrawOffsetY - 15 + sOffsetY);
//  		}
 	}
}

void GameMonster::UpdateMove()
{
	PLAYER_DIRECTION pd = GetDirection();
	int nMoveSpeed = m_attrib.moveSpeed;
	if(nMoveSpeed > 10)
	{
		nMoveSpeed = 10;
	}
	DWORD dwMoveCostTime = NORMAL_WALK_COST_TIME - nMoveSpeed * 50;

	if(GetStatus() == PST_WALK)
	{
		dwMoveCostTime *= 1.2;
	}

	bool bIsFrozen = TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_ICE);
	if(bIsFrozen)
	{
		dwMoveCostTime *= 2.0f;
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

	int nWalkMax = m_pRenderInfo->walkframe;

	if(fMovedRadio >= 0.98f)
	{
		SetReallyCoord(GetCoordX(), GetCoordY());
		SetCurFrame(nWalkMax - 1);
	}
	else
	{
		if(fMovedRadio >= 0.99f)
		{
			//__asm int 3
		}

		float fPerFrame = (float)1 / nWalkMax;
		for(int i = 0; i < nWalkMax; ++i)
		{
			if(fMovedRadio > i * fPerFrame &&
				fMovedRadio < (i + 1) * fPerFrame)
			{
				SetCurFrame(i);
				break;
			}
		}

		/*if(fMovedRadio > 0.8f)
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
		}*/
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

void GameMonster::Update(float dt)
{
	float fMoveOffsetX = 0.0f;
	float fMoveOffsetY = 0.0f;
	int nTotalFrame = 0;
	int nMoveOffset = 0;

	//	受伤状态update
	if(!IsLastFrame() &&
		GetStatus() == PST_ATTACKED)
	{
		m_fLastUpdateAttack += dt;
		if(m_fLastUpdateAttack > 0.1f)
		{
			SetCurFrame(GetCurFrame() + 1);
			m_fLastUpdateAttack = 0.0f;
		}
	}
	else if(IsLastFrame() &&
		GetStatus() == PST_ATTACKED)
	{
		m_fLastUpdateAttack += dt;
		if(m_fLastUpdateAttack > 0.1f)
		{
			m_fLastUpdateAttack = 0.0f;
			SetStatus(PST_STAND);
		}
	}

	//	死亡update
	if(!IsLastFrame() &&
		GetStatus() == PST_DEAD)
	{
		//static float fLastUpdateDead = 0.0f;
		m_fLastUpdateDead += dt;
		if(m_fLastUpdateDead > 0.1f)
		{
			SetCurFrame(GetCurFrame() + 1);
			m_fLastUpdateDead = 0.0f;
		}
		if(IsLastFrame())
		{
#ifdef _NET_GAME_
			if(TEST_FLAG_BOOL(m_attrib.maxEXPR, MAXEXPR_MASK_DEADHIDE))
			{
				SetVisible(false);
			}
#else
			ItemAttrib* pItems = NULL;
			int nSum = 0;
			GameInfoManager::GetInstance()->GetMonsterItems(m_attrib.id, &nSum, &pItems);
			if(nSum > 0)
			{
				for(int i = 0; i < nSum; ++i)
				{
					GameMapManager::GetInstance()->AddFloorItem(GetCoordX(), GetCoordY(), &pItems[i]);
				}
				delete [] pItems;
			}
			//m_walkPath.clear();
#endif
		}
	}


	//	攻击update
	float fAtkInterval = GetAttackInterval();
	if(!IsLastFrame() &&
		GetStatus() == PST_ATTACKNOWEAPON)
	{
		m_fLastUpdateNoWeapon += dt;
		if(m_fLastUpdateNoWeapon > /*0.08f*/fAtkInterval)
		{
			SetCurFrame(GetCurFrame() + 1);
			m_fLastUpdateNoWeapon = 0.0f;
		}
	}
	else if(IsLastFrame() &&
		GetStatus() == PST_ATTACKNOWEAPON)
	{
		m_fLastUpdateNoWeapon += dt;
		if(m_fLastUpdateNoWeapon > /*0.08f*/fAtkInterval)
		{
			m_fLastUpdateNoWeapon = 0.0f;
			SetStatus(PST_STAND);
#ifdef _NET_GAME_
#else
			AfxGetPlayer()->SetHurt();
#endif
		}
	}

#ifdef _NORMAL_MOVE_
	if(IsMoving())
	{
		//	更新移动信息
		float fMovePerSecond = GetMovePerSecond();
		switch(GetDirection())
		{
		case PDT_RIGHT:
			{
				if(GetStatus() == PST_WALK)
				{
					/*fMoveOffsetX = dt * GetSpeed() * 0.5f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.5f;
					SetPosxOffset(fMoveOffsetX);
					m_fMoveOffsetXTotal += fMoveOffsetX; 
					//	切换跑动帧
					nTotalFrame = m_pRenderInfo->walkframe;
					SetCurFrame(m_fMoveOffsetXTotal / (48 /nTotalFrame));

					if(GetObjx() >= GetCoordX() * UNIT_WIDTH)
					{
						SetObjx(GetCoordX() * UNIT_WIDTH);
						SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_LEFT:
			{
				if(GetStatus() == PST_WALK)
				{
					/*fMoveOffsetX = -dt * GetSpeed() * 0.5f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.5f;
					SetPosxOffset(fMoveOffsetX);
					m_fMoveOffsetXTotal += fMoveOffsetX; 
					//	切换跑动帧
					nTotalFrame = m_pRenderInfo->walkframe;
					SetCurFrame(abs(m_fMoveOffsetXTotal / (48 /nTotalFrame)));

					if(GetObjx() <= GetCoordX() * UNIT_WIDTH)
					{
						SetObjx(GetCoordX() * UNIT_WIDTH);
						SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_DOWN:
			{
				if(GetStatus() == PST_WALK)
				{
					/*fMoveOffsetY = dt * GetSpeed() * 0.5f * 0.7f;*/
					fMoveOffsetY = dt * fMovePerSecond * 0.5f * 0.7f;
					SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					nTotalFrame = m_pRenderInfo->walkframe;
					SetCurFrame((abs(m_fMoveOffsetYTotal) / (32 /nTotalFrame)) > (m_pRenderInfo->walkframe - 1) ? (m_pRenderInfo->walkframe - 1) :(abs(m_fMoveOffsetYTotal) / (32 /nTotalFrame)) );

					if(GetObjy() >= GetCoordY() * UNIT_HEIGHT)
					{
						SetObjy(GetCoordY() * UNIT_HEIGHT);
						SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_UP:
			{
				if(GetStatus() == PST_WALK)
				{
					/*fMoveOffsetY = -dt * GetSpeed() * 0.5f * 0.7f;*/
					fMoveOffsetY = -dt * fMovePerSecond * 0.5f * 0.7f;
					SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					nTotalFrame = m_pRenderInfo->walkframe;
					SetCurFrame((abs(m_fMoveOffsetYTotal) / (32 /nTotalFrame)) > 5 ? 5 : (abs(m_fMoveOffsetYTotal) / (32 /nTotalFrame)));

					if(GetObjy() <= GetCoordY() * UNIT_HEIGHT)
					{
						SetObjy(GetCoordY() * UNIT_HEIGHT);
						SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_RIGHTUP:
			{
				if(GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = dt * GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = - dt * GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.6f * 0.7f;
					fMoveOffsetY = - dt * fMovePerSecond * 0.4f * 0.7f;
					SetPosxOffset(fMoveOffsetX);
					SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					nTotalFrame = m_pRenderInfo->walkframe;
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					int nCurFrame = (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame));
					SetCurFrame(nCurFrame);
					//AfxGetHge()->System_Log("帧%d %d", nCurFrame, nMoveOffset);

					if(GetObjy() <= GetCoordY() * UNIT_HEIGHT ||
						GetObjx() >= GetCoordX() * UNIT_WIDTH)
					{
						SetReallyCoord(GetCoordX(), GetCoordY());
						SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_RIGHTDOWN:
			{
				if(GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = dt * GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = dt * GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.6f * 0.7f;
					fMoveOffsetY = dt * fMovePerSecond * 0.4f * 0.7f;
					SetPosxOffset(fMoveOffsetX);
					SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					nTotalFrame =m_pRenderInfo->walkframe;
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(GetObjy() >= GetCoordY() * UNIT_HEIGHT ||
						GetObjx() >= GetCoordX() * UNIT_WIDTH)
					{
						SetReallyCoord(GetCoordX(), GetCoordY());
						SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_LEFTUP:
			{
				if(GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = -dt * GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = -dt * GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.6f * 0.7f;
					fMoveOffsetY = -dt * fMovePerSecond * 0.4f * 0.7f;
					SetPosxOffset(fMoveOffsetX);
					SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					nTotalFrame = m_pRenderInfo->walkframe;
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(GetObjy() <= GetCoordY() * UNIT_HEIGHT ||
						GetObjx() <= GetCoordX() * UNIT_WIDTH)
					{
						SetReallyCoord(GetCoordX(), GetCoordY());
						SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_LEFTDOWN:
			{
				if(GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = -dt * GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = dt * GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.6f * 0.7f;
					fMoveOffsetY = dt * fMovePerSecond * 0.4f * 0.7f;
					SetPosxOffset(fMoveOffsetX);
					SetPosyOffset(fMoveOffsetY);
					m_fMoveOffsetXTotal += fMoveOffsetX;
					m_fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					nTotalFrame = m_pRenderInfo->walkframe;
					nMoveOffset = sqrt(pow((float)m_fMoveOffsetXTotal, 2) + pow((float)m_fMoveOffsetYTotal, 2));
					SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(GetObjy() >= GetCoordY() * UNIT_HEIGHT ||
						GetObjx() <= GetCoordX() * UNIT_WIDTH)
					{
						SetReallyCoord(GetCoordX(), GetCoordY());
						SetCurFrame(nTotalFrame - 1);
						m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		}
	}
#else
	if(IsMoving())
	{
		UpdateMove();
	}
#endif

	//	假如不在移动 并且有受伤状态 并且不再攻击 就受伤
	if(!IsMoving() &&
		m_bHurt &&
		(GetStatus() != PST_ATTACKNOWEAPON && GetStatus() != PST_ATTACKWEAPON && GetStatus() != PST_ATTACKSTOP))
	{
		SetStatus(PST_ATTACKED);
		SetCurFrame(0);
		m_bHurt = false;
		//m_fLastAtkTime = AfxGetHge()->Timer_GetTime();
	}

	//static float tDetected = 0;
	if((GetStatus() == PST_WALK ||
		GetStatus() == PST_RUN) &&
		!IsMoving() &&
		IsLastFrame())
	{
		//	检测到不在站立状态
		if(abs(m_fLastUpdateTime - 0.0f) < 0.00001f)
		{
			m_fLastUpdateTime = AfxGetHge()->Timer_GetTime();
		}
		else
		{
			if(abs(m_fLastUpdateTime - AfxGetHge()->Timer_GetTime()) > 0.1f)
			{
				SetStatus(PST_STAND);
				SetCurFrame(0);
				m_fLastUpdateTime = 0.0f;
			}
		}
	}
// 	else if(GetStatus() == PST_ATTACKED &&
// 		!IsMoving() &&
// 		IsLastFrame())
// 	{
// 		//	检测到不在站立状态
// 		if(abs(m_fLastUpdateTime - 0.0f) < 0.00001f)
// 		{
// 			m_fLastUpdateTime = AfxGetHge()->Timer_GetTime();
// 		}
// 		else
// 		{
// 			if(abs(m_fLastUpdateTime - AfxGetHge()->Timer_GetTime()) > 0.2f)
// 			{
// 				SetStatus(PST_STAND);
// 				SetCurFrame(0);
// 				m_fLastUpdateTime = 0.0f;
// 			}
// 		}
// 	}
// 	else if(GetStatus() == PST_ATTACKWEAPON &&
// 		!IsMoving() &&
// 		IsLastFrame())
// 	{
// 		//	检测到不在站立状态
// 		if(abs(m_fLastUpdateTime - 0.0f) < 0.00001f)
// 		{
// 			m_fLastUpdateTime = AfxGetHge()->Timer_GetTime();
// 		}
// 		else
// 		{
// 			if(abs(m_fLastUpdateTime - AfxGetHge()->Timer_GetTime()) > 0.2f)
// 			{
// 				SetStatus(PST_STAND);
// 				SetCurFrame(0);
// 				m_fLastUpdateTime = 0.0f;
// 			}
// 		}
// 	}
// 	else if(GetStatus() == PST_ATTACKNOWEAPON &&
// 		!IsMoving() &&
// 		IsLastFrame())
// 	{
// 		//	检测到不在站立状态
// 		if(abs(m_fLastUpdateTime - 0.0f) < 0.00001f)
// 		{
// 			m_fLastUpdateTime = AfxGetHge()->Timer_GetTime();
// 		}
// 		else
// 		{
// 			if(abs(m_fLastUpdateTime - AfxGetHge()->Timer_GetTime()) > 0.2f)
// 			{
// 				SetStatus(PST_STAND);
// 				SetCurFrame(0);
// 				m_fLastUpdateTime = 0.0f;
// 			}
// 		}
// 	}
	else
	{
		m_fLastUpdateTime = 0.0f;
	}

	//	动态
	if(GetStatus() == PST_STAND)
	{
		//	石化
		if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_STONE))
		{
			SetCurFrame(0);
		}
		else
		{
			int nFrm = 0;

			if(m_pRenderInfo->standframe == 0)
			{
				nFrm = ((int)((AfxGetHge()->Timer_GetTime() - GetLastStandTime()) / 0.5)) % 4;
			}
			else
			{
				nFrm = ((int)((AfxGetHge()->Timer_GetTime() - GetLastStandTime()) / 0.5)) % m_pRenderInfo->standframe;
			}
			SetCurFrame(nFrm);
		}
	}
}

bool GameMonster::ProcUserCmd(const POINT& _mp)
{
#ifndef NETWORK_MODE
// 	GameObject* pObj = GamePlayer::GetInstance();
// 
// 	if(!IsMoving() &&
// 		(GetStatus() == PST_WALK ||
// 		GetStatus() == PST_STAND))
// 	{
// 		//	不在移动 处于走路结束和站立的状态 接受自动AI输入
// 		//	取消仇恨
// 		//if(m_attrib.maxMC)
// 		//	首先 处理攻击
// 		DWORD dwTimeStamp = ::GetTickCount();
// 
// 		if(IsInAttackArea(pObj) &&
// 			pObj->GetStatus() != PST_DEAD)
// 		{
// 			if(dwTimeStamp - m_dwLastAttackTime <= m_dwAttackInterval)
// 			{
// 				return true;
// 			}
// 
// 			m_dwLastAttackTime = dwTimeStamp;
// 			if(!AttackUser(pObj))
// 			{
// 				MoveToTarget(pObj);
// 			}
// 			return true;
// 		}
// 
// 		//	处理在视野内的跟踪
// 		if(IsInViewArea(pObj) &&
// 			m_attrib.moveSpeed != 0 &&
// 			pObj->GetStatus() != PST_DEAD)
// 		{
// 			if(dwTimeStamp - m_dwLastMoveTime <= m_dwMoveInterval)
// 			{
// 				return true;
// 			}
// 
// 			m_dwLastMoveTime = dwTimeStamp;
// 			MoveToTarget(pObj);
// 			return true;
// 		}
// 
// 		//	不攻击也不跟踪 自动走动
// 		if(m_attrib.moveSpeed != 0 &&
// 			GetStatus() != PST_DEAD)
// 		{
// 			Wonder();
// 		}
// 		
// 	}
#endif
	return true;
}

int GameMonster::GetSoundIndex()
{
	if(NULL == m_pRenderInfo)
	{
		return 0;
	}

	int nIndex = 0;
	nIndex = m_pRenderInfo->sndindex;
	if(0 != nIndex)
	{
		return nIndex;
	}

	nIndex = m_pRenderInfo->wil * 10 + m_pRenderInfo->index;
	nIndex = nIndex * 10 + 200;
	return nIndex;
}

bool GameMonster::IsInAttackArea(GameObject* _obj)
{
	if(abs(_obj->GetObjx() - m_fPosx) / UNIT_WIDTH <= m_attrib.maxSC &&
		abs(_obj->GetObjy() - m_fPosy) / UNIT_HEIGHT <= m_attrib.maxSC)
	{
		return true;
	}
	return false;
}

bool GameMonster::IsInViewArea(GameObject* _obj)
{
	if(abs(_obj->GetObjx() - m_fPosx) / UNIT_WIDTH < m_attrib.SC &&
		abs(_obj->GetObjy() - m_fPosy) / UNIT_HEIGHT < m_attrib.SC)
	{
		return true;
	}
	return false;
}

bool GameMonster::AttackUser(GameObject* _obj)
{
	if(!IsInAttackArea(_obj))
	{
		return false;
	}

	int nDir = GetDirectionOf2(m_ptPos, _obj->GetPosition(), m_attrib.maxSC);
	if(nDir <= PDT_LEFTUP)
	{
		//SetDirection((PLAYER_DIRECTION)nDir);
		//SetStatus(PST_ATTACKNOWEAPON);
		/*ActionPacket* pPacket = new ActionPacket;
		ZeroMemory(pPacket, sizeof(ActionPacket));
		pPacket->dwTarget = GamePlayer::GetInstance()->GetID();
		pPacket->wCmd = PC_ACTION_ATTACK;
		pPacket->wParam0 = (WORD)nDir;
		m_lstActPacket.push_back(pPacket);

		int nDamage = GetDamage(_obj);
		if(nDamage > 0)
		{
			if(_obj->GetStatus() == PST_ATTACKNOWEAPON ||
				_obj->GetStatus() == PST_ATTACKWEAPON ||
				_obj->GetStatus() == PST_SK_LEIDIANSHU ||
				_obj->GetStatus() == PST_ATTACKED)
			{
				//	攻击时候不后仰
			}
			else
			{
				//_obj->SetHurt();
			}
			_obj->DecreaseHealth(nDamage);
		}
		return true;*/
	}

	return false;
}



int GameMonster::GetDamage(GameObject* _obj)
{
	int nRandom = AfxGetHge()->Random_Int(0, 50);
	if(nRandom < _obj->GetAttrib()->hide)
	{
		//	躲避属性
		return 0;
	}

	nRandom = AfxGetHge()->Random_Int(m_attrib.DC, m_attrib.maxDC);
	if(m_attrib.type == 0)
	{
		nRandom -= AfxGetHge()->Random_Int(min(_obj->GetAttrib()->AC, _obj->GetAttrib()->maxAC),
			max(_obj->GetAttrib()->AC, _obj->GetAttrib()->maxAC));
		return nRandom;
	}
	else if(m_attrib.type == 1)
	{
		nRandom -= AfxGetHge()->Random_Int(min(_obj->GetAttrib()->MAC, _obj->GetAttrib()->maxMAC),
			max(_obj->GetAttrib()->MAC, _obj->GetAttrib()->maxMAC));
		return nRandom;
	}

	return 0;
}

bool GameMonster::LoadProperty(int _id)
{
	bool bSuc = true;

	bSuc =  bSuc && GameInfoManager::GetInstance()->GetMonsterAttrib(_id, &m_attrib);
	bSuc = bSuc && GameInfoManager::GetInstance()->GetMonsterRenderInfo(_id, &m_pRenderInfo);

	if(bSuc)
	{
		InitOtherRes(GameResourceManager::GetInstance()->GetTexs((GAME_RES)(RES_MON1 + m_pRenderInfo->wil)));
	}
	

	m_dwMoveInterval = (300 - m_attrib.moveSpeed) * 10;
	m_dwAttackInterval = 2000;
	return bSuc;
}

bool GameMonster::IsLastFrame()
{
	switch (GetStatus())
	{
	case PST_ATTACKED:
		{
			if(GetCurFrame() == m_pRenderInfo->hurtframe - 1)
			{
				return true;
			}
		}break;
	case PST_WALK:
		{
			if(GetCurFrame() == m_pRenderInfo->walkframe - 1)
			{
				return true;
			}
		}break;
	case PST_ATTACKNOWEAPON:
		{
			if(GetCurFrame() == m_pRenderInfo->atkframe - 1)
			{
				return true;
			}
		}break;
	case PST_DEAD:
		{
			if(GetCurFrame() ==m_pRenderInfo->deadframe - 1)
			{
				return true;
			}
		}
	}
	return false;
}

void GameMonster::Move()
{

}

void GameMonster::Wonder()
{
	/*if ((rand() % 20) == 0)
	{
		if ((rand() % 4) == 1)
		{
			//SetDirection((PLAYER_DIRECTION)(rand() % 8));
			ActionPacket* pPacket = new ActionPacket;
			pPacket->wCmd = PC_ACTION_TURN;
			pPacket->wParam0 = (WORD)(rand() % 8);
			m_lstActPacket.push_back(pPacket);
		}
		else
		{
			WalkOneStep(GetDirection());
		}
	}*/
}

void GameMonster::MoveToTarget(GameObject *_obj)
{
	int nDrt = 0;

	while(1)
	{
		if(m_ptPos.x < _obj->GetCoordX())
		{
			nDrt = PDT_RIGHT;

			if(m_ptPos.y < _obj->GetCoordY())
			{
				nDrt = PDT_RIGHTDOWN;
				break;
			}

			if(m_ptPos.y > _obj->GetCoordY())
			{
				nDrt = PDT_RIGHTUP;
				break;
			}
			break;
		}
		else if(m_ptPos.x > _obj->GetCoordX())
		{
			nDrt = PDT_LEFT;

			if(m_ptPos.y < _obj->GetCoordY())
			{
				nDrt = PDT_LEFTDOWN;
				break;
			}

			if(m_ptPos.y > _obj->GetCoordY())
			{
				nDrt = PDT_LEFTUP;
				break;
			}
			break;
		}
		
		if(m_ptPos.y > _obj->GetCoordY())
		{
			nDrt = PDT_UP;
			break;
		}

		if(m_ptPos.y < _obj->GetCoordY())
		{
			nDrt = PDT_DOWN;
			break;
		}
	}

	bool bMoveSuc = false;

	bMoveSuc = WalkOneStep(nDrt);
	if(!bMoveSuc)
	{
		int nRandom = 0;
		for(int i = 0; i < 7; ++i)
		{
			nRandom = AfxGetHge()->Random_Int(0, 2);
			if(nRandom)
			{
				++nDrt;
			}
			else if(nDrt > 0)
			{
				--nDrt;
			}
			else
			{
				nDrt = 7;
			}

			if(nDrt > 7)
			{
				nDrt = 7;
			}

			if(WalkOneStep(nDrt))
			{
				break;
			}
		}
	}
}

bool GameMonster::WalkOneStep(int _drt)
{
	int nWalkOffsetX = 0;
	int nWalkOffsetY = 0;

	switch(_drt)
	{
	case PDT_UP:
		{
			nWalkOffsetY = -1;
		}break;
	case PDT_DOWN:
		{
			nWalkOffsetY = 1;
		}break;
	case PDT_LEFTUP:
		{
			nWalkOffsetY = -1;
			nWalkOffsetX = -1;
		}break;
	case PDT_LEFTDOWN:
		{
			nWalkOffsetY = 1;
			nWalkOffsetX = -1;
		}break;
	case PDT_RIGHTUP:
		{
			nWalkOffsetY = -1;
			nWalkOffsetX = 1;
		}break;
	case PDT_RIGHTDOWN:
		{
			nWalkOffsetX = 1;
			nWalkOffsetY = 1;
		}break;
	case PDT_LEFT:
		{
			nWalkOffsetX = -1;
		}break;
	case PDT_RIGHT:
		{
			nWalkOffsetX = 1;
		}break;
	}

	return MoveMonster(nWalkOffsetX, nWalkOffsetY);
}

bool GameMonster::MoveMonster(int _dx, int _dy)
{
	/*GameMapManager* pMap = GameMapManager::GetInstance();
	int nWalkX = m_ptPos.x + _dx;
	int nWalkY = m_ptPos.y + _dy;

	if(!pMap->CanThrough(nWalkX, nWalkY))
	{
		return false;
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	if(pPlayer->GetCoordX() == nWalkX &&
		pPlayer->GetCoordY() == nWalkY)
	{
		return false;
	}

	//pMap->MoveMonster(_dx, _dy, this);
	POINT ptTo = {nWalkX, nWalkY};
	BYTE bDrt = GetDirectionOf2(GetPosition(),ptTo);
	if(bDrt < 8)
	{
		ActionPacket* pPakcet = new ActionPacket;
		pPakcet->wCmd = PC_ACTION_WALK;
		pPakcet->wParam0 = (WORD)bDrt;
		pPakcet->wParam1 = (WORD)GetCoordX();
		pPakcet->wParam2 = (WORD)GetCoordY();
		pPakcet->wParam3 = (WORD)_dx;
		pPakcet->wParam4 = (WORD)_dy;
		PushActionPacket(pPakcet);
// 		SetOffsetCoordX(_dx);
// 		SetOffsetCoordY(_dy);
// 		SetStatus(PST_WALK);
// 		SetDirection((PLAYER_DIRECTION)bDrt);
		return true;
	}*/
	return false;
}




/************************************************************************/
/* 动作数据包处理
/************************************************************************/
/*
void GameMonster::OnAttack(ActionPacket* _pPacket)
{
	//	攻击状态
	SetStatus(PST_ATTACKNOWEAPON);
	SetDirection((PLAYER_DIRECTION)_pPacket->wParam0);
}

void GameMonster::OnWalk(ActionPacket *_pPacket)
{
	//	行走
	SetDirection((PLAYER_DIRECTION)_pPacket->wParam0);
	SetReallyCoord(_pPacket->wParam1, _pPacket->wParam2);
	SetStatus((PLAYER_STATUS)PST_WALK);
	SetOffsetCoordX((short)_pPacket->wParam3);
	SetOffsetCoordY((short)_pPacket->wParam4);
}

void GameMonster::OnRun(ActionPacket *_pPacket)
{

}

void GameMonster::OnTurn(ActionPacket *_pPacket)
{
	SetStatus(PST_STAND);
	SetDirection((PLAYER_DIRECTION)(_pPacket->wParam0));
}*/
//////////////////////////////////////////////////////////////////////////
void GameMonster::SetLastFrame()
{
	switch(GetStatus())
	{
	case PST_DEAD:
		{
			SetCurFrame(m_pRenderInfo->deadframe - 1);
		}break;
	}
}









/************************************************************************/
/* GroundMonster
/************************************************************************/
GroundMonster::GroundMonster()
{
	m_fLastUpdateAppearTime = 0.0f;
	m_fLastUpdateGroundTime = 0.0f;
	SetVisible(false);
}

GroundMonster::~GroundMonster()
{

}
//////////////////////////////////////////////////////////////////////////
void GroundMonster::Update(float _dt)
{
	if(!IsVisible())
	{
		return;
	}

	if(NULL == m_pRenderInfo)
	{
		return;
	}
	

	float dt = _dt;

	if(GetStatus() == PST_APPEAR ||
		GetStatus() == PST_GROUND ||
		GetStatus() == PST_ATTACKNOWEAPON)
	{
		//	出地面Update
		if(!IsLastFrame() &&
			GetStatus() == PST_APPEAR)
		{
			m_fLastUpdateAppearTime += dt;
			if(m_fLastUpdateAppearTime > 0.15f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateAppearTime = 0.0f;
			}
		}
		else if (IsLastFrame() &&
			GetStatus() == PST_APPEAR)
		{
			m_fLastUpdateAppearTime += dt;
			if(m_fLastUpdateAppearTime > 0.15f)
			{
				m_fLastUpdateAppearTime = 0.0f;
				SetStatus(PST_STAND);
			}
		}

		//	入地Update
		if(!IsLastFrame() &&
			GetStatus() == PST_GROUND)
		{
			m_fLastUpdateGroundTime += _dt;
			if(m_fLastUpdateGroundTime > 0.15f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateGroundTime = 0.0f;
			}
		}
		else if(IsLastFrame() &&
			GetStatus() == PST_GROUND)
		{
			m_fLastUpdateGroundTime += dt;
			if(m_fLastUpdateGroundTime > 0.15f)
			{
				m_fLastUpdateGroundTime = 0.0f;
				//SetStatus(PST_STAND);
				//GetAttrib()->curse = 0;
				SetVisible(false);
			}
		}

		//	攻击update
		float fAtkInterval = GetAttackInterval();
		if(!IsLastFrame() &&
			GetStatus() == PST_ATTACKNOWEAPON)
		{
			m_fLastUpdateNoWeapon += dt;
			if(m_fLastUpdateNoWeapon > /*0.08f*/fAtkInterval)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateNoWeapon = 0.0f;
			}
		}
		else if(IsLastFrame() &&
			GetStatus() == PST_ATTACKNOWEAPON)
		{
			m_fLastUpdateNoWeapon += dt;
			if(m_fLastUpdateNoWeapon > /*0.08f*/fAtkInterval)
			{
				m_fLastUpdateNoWeapon = 0.0f;
				SetStatus(PST_STAND);
				
				MagicStaticAttackEffect* pEff = new MagicStaticAttackEffect;
				if(pEff->Create(m_attrib.id, NULL, NULL, GetObjx(), GetObjy(), GetDirection()))
				{
					pEff->SetActive();
					GameScene::sThis->InsertNewMagic(pEff);
				}
				else
				{
					SAFE_DELETE(pEff);
				}
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}
//////////////////////////////////////////////////////////////////////////
void GroundMonster::Render()
{
	HGE* hge = AfxGetHge();
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	if(NULL == m_pRenderInfo)
	{
		return;
	}

	if(!IsVisible())
	{
		return;
	}
	/*
	if(GetAttrib()->curse == 0)
		{
			//	In the ground
			return;
		}*/
	

	if(m_texs)
	{
		GameTextureManager* res = m_texs;
		//	画怪物
		HTEXTURE tex = res->GetTexture(CalMonIndex() + m_bCurFrame);
		short sOffsetX = 0;
		short sOffsetY = 0;
		int nDrawOffsetX = -1;
		int nDrawOffsetY = -1;
		int nTextureIndex = CalMonIndex() + m_bCurFrame;
		if(tex)
		{
			if(GameScene::sThis->GetTarget() == this)
			{
				HTEXTURE texSelected = SelectedTextureManager::GetInstance()->GetSelectedTextureMonster(tex, GetAttrib()->id, nTextureIndex);
				if(texSelected)
				{
					tex = texSelected;
				}
			}
			if(!m_pRender)
			{
				//m_pSpr = new hgeSprite(tex, 0, 0, res->GetTextureWidth(tex), res->GetTextureHeight(tex));
				m_pRender = new hgeSprite(tex, 0, 0, res->GetTextureWidth(nTextureIndex),
					res->GetTextureHeight(nTextureIndex));
			}
			else
			{
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0, 0, res->GetTextureWidth(nTextureIndex), 
					res->GetTextureHeight(nTextureIndex));
			}
			sOffsetX = res->GetTextureOffsetX(nTextureIndex);
			sOffsetY = res->GetTextureOffsetY(nTextureIndex);
			//	根据玩家位置 得先算出来正常矩形的位置
			nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
			nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
			m_pRender->Render(nDrawOffsetX + sOffsetX,
				nDrawOffsetY + sOffsetY);
			//	记录当前纹理
			m_nCurrentTextureIndex = CalMonIndex() + m_bCurFrame;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
/*
bool GroundMonster::IsSelected(const POINT &_pt)
{
	if(GetAttrib()->curse == 0)
	{
		return false;
	}
	return __super::IsSelected(_pt);
}*/

//////////////////////////////////////////////////////////////////////////
int GroundMonster::CalMonIndex()
{
	int nIndex = 0;

	//if(m_renderInfo.drts == 4)
	{
		switch(GetStatus())
		{
		case PST_STAND:
			{
				nIndex = m_pRenderInfo->standbegin + 0 * m_pRenderInfo->standtotal;
			}break;
// 		case PST_WALK:
// 			{
// 				nIndex = m_renderInfo.walkbegin + m_drt * m_renderInfo.walktotal;
// 			}break;
		case PST_ATTACKNOWEAPON:
			{
				nIndex = m_pRenderInfo->atkbegin + 0 *m_pRenderInfo->atktotal;
			}break;
		case PST_ATTACKED:
			{
				nIndex = m_pRenderInfo->hurtbegin + 0 * m_pRenderInfo->hurttotal;
			}break;
		case PST_DEAD:
			{
				nIndex = m_pRenderInfo->deadbegin + 0 * m_pRenderInfo->deadtotal;
			}break;
		case PST_APPEAR:
			{
				nIndex = m_pRenderInfo->walkbegin + 0;
			}break;
		case PST_GROUND:
			{
				nIndex =m_pRenderInfo->walkbegin + 1 * m_pRenderInfo->walktotal;
			}break;
		}
	}

	return nIndex;
}
//////////////////////////////////////////////////////////////////////////
/*
void GroundMonster::RenderHPBar()
{
	if(GetAttrib()->curse == 0)
	{
		return;
	}
	else
	{
		__super::RenderHPBar();
	}
}*/

//////////////////////////////////////////////////////////////////////////
bool GroundMonster::IsLastFrame()
{
	switch (GetStatus())
	{
	case PST_ATTACKED:
		{
			if(GetCurFrame() == m_pRenderInfo->hurtframe - 1)
			{
				return true;
			}
		}break;
	/*
	case PST_WALK:
			{
				if(GetCurFrame() == m_renderInfo.walkframe - 1)
				{
					return true;
				}
			}break;*/
	
	case PST_ATTACKNOWEAPON:
		{
			if(GetCurFrame() == m_pRenderInfo->atkframe - 1)
			{
				return true;
			}
		}break;
	case PST_DEAD:
		{
			if(GetCurFrame() == m_pRenderInfo->deadframe - 1)
			{
				return true;
			}
		}break;
	case PST_APPEAR:
	case PST_GROUND:
		{
			if(GetCurFrame() == m_pRenderInfo->walkframe - 1)
			{
				return true;
			}
		}break;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
void GameMonster::RenderName()
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

	bool bRenderMaster = false;
	BYTE bMonsterType = 0;

	if(m_szMasterName[0] != 0)
	{
		bRenderMaster = true;
		nTextWidth += GetTextWidth(m_szMasterName, 12) + 12;
	}	
	else if(TEST_FLAG_BOOL(m_attrib.maxEXPR, MAXEXPR_MASK_ELITE))
	{
		bMonsterType = 1;
		nTextWidth += 12 + 24;
	}
	else if(TEST_FLAG_BOOL(m_attrib.maxEXPR, MAXEXPR_MASK_LEADER))
	{
		bMonsterType = 2;
		nTextWidth += 12 + 24;
	}

	if(0 == nTextWidth)
	{
		return;
	}

	if(bRenderMaster)
	{
		nDrawY -= UNIT_HEIGHT / 2;
		nDrawX += UNIT_WIDTH / 2;
		nDrawX -= nTextWidth / 2;

		char szBuf[40];
		sprintf(szBuf, "%s(%s)",
			m_attrib.name, m_szMasterName);

		int nCurLevel = GetSlaveLevel();
		if(nCurLevel < 0 ||
			nCurLevel >= 8)
		{
			nCurLevel = 0;
		}
		DWORD dwUseColor = g_dwMonsLvColor[nCurLevel];

		AfxGetPrinter()->SetColor(dwUseColor);
		AfxGetPrinter()->Print(nDrawX, nDrawY, szBuf);
	}
	else if(bMonsterType != 0)
	{
		char szBuf[40];

		if(bMonsterType == 1)
		{
			sprintf(szBuf, "%s[精英]",
				m_attrib.name);
			AfxGetPrinter()->SetColor(ARGB(255, 240, 20, 240));
		}
		else if(bMonsterType == 2)
		{
			sprintf(szBuf, "%s[首领]",
				m_attrib.name);
			AfxGetPrinter()->SetColor(ARGB(255, 230, 30, 80));
		}
		
		nDrawY -= UNIT_HEIGHT / 2;
		nDrawX += UNIT_WIDTH / 2;
		nDrawX -= nTextWidth / 2;

		AfxGetPrinter()->Print(nDrawX, nDrawY, szBuf);
	}
	else
	{
		nDrawY -= UNIT_HEIGHT / 2;
		nDrawX += UNIT_WIDTH / 2;
		nDrawX -= nTextWidth / 2;

		AfxGetPrinter()->SetColor(TEXT_ARGB_WHITE);
		AfxGetPrinter()->Print(nDrawX, nDrawY, m_attrib.name);
	}
	/*nDrawY -= UNIT_HEIGHT / 2;
	nDrawX += UNIT_WIDTH / 2;
	nDrawX -= nTextWidth / 2;

	char szBuf[40];
	if(bRenderMaster)
	{
		sprintf(szBuf, "%s(%s)",
			m_attrib.name, m_szMasterName);
	}

	int nCurLevel = GetSlaveLevel();
	if(nCurLevel < 0 ||
		nCurLevel >= 8)
	{
		nCurLevel = 0;
	}
	DWORD dwUseColor = g_dwMonsLvColor[nCurLevel];

	if(bRenderMaster)
	{
		//AfxGetPrinter()->SetColor(TEXT_ARGB_WHITE);
		AfxGetPrinter()->SetColor(dwUseColor);
		AfxGetPrinter()->Print(nDrawX, nDrawY, szBuf);
	}
	else
	{
		AfxGetPrinter()->SetColor(TEXT_ARGB_WHITE);
		AfxGetPrinter()->Print(nDrawX, nDrawY, m_attrib.name);
	}*/
}


//////////////////////////////////////////////////////////////////////////
void GameMonster::OnPacket(const PacketHeader *_pPkt)
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
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,			PkgObjectActionNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_CLICKNPC_ACK,			PkgPlayerClickNPCAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_UPDATEATTRIB_NTF,		PkgPlayerUpdateAttribNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_SETEFFECT_ACK,		PkgPlayerSetEffectAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_CHANGEMONSLOOK_ACK,	PkgPlayerChangeMonsLookAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_PLAYSOUND_NTF,		PkgPlayerPlaySoundNtf)
		HANDLE_PACKET(PKG_GAME_CHAT_NOT,					PkgChatNot)
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_TURN_NOT,		PkgObjectActionTurnNot)
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_WALK_NOT,		PkgObjectActionWalkNot)
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_RUN_NOT,		PkgObjectActionRunNot)
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_ATTACK_NOT,	PkgObjectActionAttackNot)
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_DEAD_NOT,		PkgObjectActionDeadNot)
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_EXT_NOT,		PkgObjectActionExtNot)
		HANDLE_DEFAULT(_pPkt)
	END_HANDLE_PACKET(_pPkt)
}

/*void GameMonster::DoPacket(const PkgObjectActionTurnNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = ACTION_TURN;

	not.uParam1 = SimpleActionHelper::GetDirection(_not);
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}

void GameMonster::DoPacket(const PkgObjectActionWalkNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = ACTION_WALK;

	not.uParam1 = _not.dwPos;
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}

void GameMonster::DoPacket(const PkgObjectActionRunNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = ACTION_RUN;

	not.uParam1 = _not.dwPos;
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}

void GameMonster::DoPacket(const PkgObjectActionAttackNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = ACTION_ATTACK;

	not.uParam1 = SimpleActionHelper::GetDirection(_not);
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}

void GameMonster::DoPacket(const PkgObjectActionDeadNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = ACTION_DEAD;

	not.uParam1 = SimpleActionHelper::GetDirection(_not);
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}

void GameMonster::DoPacket(const PkgObjectActionExtNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = _not.cExtAction;

	not.uParam1 = SimpleActionHelper::GetDirection(_not);
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}*/

void GameMonster::DoPacket(const PkgChatNot& not)
{
	std::string xSay;

	xSay += GetAttrib()->name;
	xSay += "说:";
	xSay += not.xMsg;
	SetSaying(not.xMsg.c_str());
}

void GameMonster::DoPacket(const PkgPlayerChangeMonsLookAck& ack)
{
	bool bSuc = GameInfoManager::GetInstance()->GetMonsterRenderInfo(ack.dwChg, &m_pRenderInfo);

	if(bSuc)
	{
		InitOtherRes(GameResourceManager::GetInstance()->GetTexs((GAME_RES)(RES_MON1 + m_pRenderInfo->wil)));

		if(ack.dwChg == 82)
		{
			strcpy(m_attrib.name, "圣兽");
		}
	}
}

void GameMonster::OnObjectAction(const PkgObjectActionNot& not)
{
	//	Play sound
	int nPlayerX = GamePlayer::GetInstance()->GetCoordX();
	int nPlayerY = GamePlayer::GetInstance()->GetCoordY();
	int nSelfX = GetCoordX();
	int nSelfY = GetCoordY();
	int nOftX = abs(nPlayerX - nSelfX);
	int nOftY = abs(nPlayerY - nSelfY);
	bool bCanPlay = false;

	if(nOftX < 8 &&
		nOftY < 8)
	{
		bCanPlay = true;
	}

	if(not.uAction == ACTION_STRUCK ||
		not.uAction == ACTION_ATTACK ||
		not.uAction == ACTION_DEAD)
	{
		if(bCanPlay)
		{
			GameSoundManager::GetInstancePtr()->PlayObjectSound(GetSoundIndex(), not.uAction);

			if(not.uAction == ACTION_STRUCK)
			{
				if(TEST_FLAG_BOOL(not.uParam3, STRUCK_MASK_WEAPON))
				//if(not.uParam3 == 1)
				{
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ATTACKEDW);
				}
				else
				{
					GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_ATTACKEDNOW);
				}
			}
		}
	}
	else if(not.uAction == ACTION_TURN ||
		not.uAction == ACTION_WALK)
	{
		int nRandom = rand() % 20;
		if(nRandom == 0 &&
			bCanPlay)
		{
			GameSoundManager::GetInstancePtr()->PlayObjectSound(GetSoundIndex(), ACTION_TURN);
		}

		if(not.uAction == ACTION_WALK)
		{
			m_fMoveOffsetXTotal = m_fMoveOffsetYTotal = 0.0f;
		}
	}

	if(not.uAction == ACTION_APPEAR ||
		not.uAction == ACTION_GROUND)
	{
		if(not.uAction == ACTION_APPEAR)
		{
			SetStatus(PST_APPEAR);
			GameSoundManager::GetInstancePtr()->PlayObjectSound(GetSoundIndex(), ACTION_APPEAR);
			//m_pObject->GetAttrib()->curse = 1;
			SetCurFrame(0);
			SetVisible(true);
		}
		else
		{
			SetStatus(PST_GROUND);
			SetCurFrame(0);
		}
	}
	else
	{
		SetVisible(true);
	}
}
//////////////////////////////////////////////////////////////////////////
void GameMonster::DoPacket(const PkgPlayerClickNPCAck& not)
{
	
}
//////////////////////////////////////////////////////////////////////////
void GameMonster::DoPacket(const PkgPlayerUpdateAttribNtf& ntf)
{
	//if(LOWORD(ntf.bType) == UPDATE_HP)
	if(ntf.bType == UPDATE_HP)
	{
		int nHPChange = ntf.dwParam - m_attrib.HP;
		m_attrib.HP = ntf.dwParam;
		if(ntf.dwExtra != 0)
		{
			//m_attrib.maxHP = ntf.dwExtra;
		}

		//if(nHPChange < 0)
		{
			GameScene::sThis->InsertNewAttackNumber(abs(nHPChange), nHPChange > 0, this);
		}
	}
	else if(ntf.bType == UPDATE_MASTER)
	{
		m_nMasterId = ntf.dwParam;

		if(ntf.dwParam == 0)
		{
			m_szMasterName[0] = 0;
		}
		else
		{
			PacketHandler* pHandler = SocketDataCenter::GetInstance().GetHandler(ntf.dwParam);
			if(pHandler)
			{
				GameObject* pObj = static_cast<GameObject*>(pHandler);
				if(pObj->GetType() == OBJ_PLAYER ||
					pObj->GetType() == OBJ_OTHERPLAYER)
				{
					strcpy(m_szMasterName, pObj->GetAttrib()->name);
				}
			}
		}
	}
	else if(ntf.bType == UPDATE_SLAVELV)
	{
		m_attrib.maxAC = ntf.dwParam;
	}
	else if(ntf.bType == UPDATE_MOVESPD)
	{
		m_attrib.moveSpeed = ntf.dwParam;
	}
}

void GameMonster::DoPacket(const PkgPlayerSetEffectAck& ack)
{
	if(ack.bShow)
	{
		if(ack.dwMgcID == MMASK_STONE)
		{
			SetHumEffect(ack.dwMgcID, ack.dwTime);
		}
		else if(ack.dwMgcID == MMASK_ICE)
		{
			SetHumEffect(ack.dwMgcID, ack.dwTime);
		}
		else if(ack.dwMgcID == MMASK_LVDU)
		{
			SetHumEffect(ack.dwMgcID, ack.dwTime);
		}
	}
	else if(!ack.bShow)
	{
		if(ack.dwMgcID == MMASK_STONE)
		{
			ResetHumEffect(MMASK_STONE);
		}
		else if(ack.dwMgcID == MMASK_ICE)
		{
			ResetHumEffect(MMASK_ICE);
		}
		else if(ack.dwMgcID == MMASK_LVDU)
		{
			ResetHumEffect(MMASK_LVDU);
		}
	}
}

void GameMonster::DoPacket(const PkgPlayerPlaySoundNtf& ntf)
{
	if(ntf.bType == PLAYSOUND_OBJECTSOUND)
	{
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), ntf.dwData);
	}
}
