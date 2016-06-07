#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
BehemothDevourerMonster::BehemothDevourerMonster()
{
	m_nCurRoundFrame = 0;
	m_dwLastChgRoundTime = 0;
	m_nLastAtkTarget = 0;
	m_fLastUpdateSkill = 0;
	m_fLastUpdateSpeAtk2 = m_fLastUpdateSpeAtk3 = 0;
}

void BehemothDevourerMonster::Render()
{
	__super::Render();

	if(GetStatus() == PST_EXT1)
	{
		if(m_texs)
		{
			GameTextureManager* res = m_texs;
			//	画怪物
			//int nTextIndex = CalMonIndex() + 180 + GetCurFrame();
			int nTextIndex = 3960 + GetDirection() * 10 + m_bCurFrame;

			HTEXTURE tex = res->GetTexture(nTextIndex);
			short sOffsetX = 0;
			short sOffsetY = 0;
			int nDrawOffsetX = -1;
			int nDrawOffsetY = -1;
			if(tex)
			{
				if(GameScene::sThis->GetTarget() == this)
				{
					HTEXTURE texSelected = SelectedTextureManager::GetInstance()->GetSelectedTextureMonster(tex, GetAttrib()->id, nTextIndex);
					if(texSelected != NULL)
					{
						tex = texSelected;
					}
				}

				sOffsetX = res->GetTextureOffsetX(nTextIndex);
				sOffsetY = res->GetTextureOffsetY(nTextIndex);
				//	根据玩家位置 得先算出来正常矩形的位置
				nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
				nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0,
					0,
					m_texs->GetTextureWidth(nTextIndex),
					m_texs->GetTextureHeight(nTextIndex));
				m_pRender->Render(nDrawOffsetX + sOffsetX,
					nDrawOffsetY + sOffsetY);
			}
		}
	}
	else if(GetStatus() == PST_EXT2)
	{
		if(m_texs)
		{
			GameTextureManager* res = m_texs;
			//	画怪物
			//int nTextIndex = CalMonIndex() + 180 + GetCurFrame();
			int nTextIndex = 4120 + GetDirection() * 10 + m_bCurFrame;

			HTEXTURE tex = res->GetTexture(nTextIndex);
			short sOffsetX = 0;
			short sOffsetY = 0;
			int nDrawOffsetX = -1;
			int nDrawOffsetY = -1;
			if(tex)
			{
				if(GameScene::sThis->GetTarget() == this)
				{
					HTEXTURE texSelected = SelectedTextureManager::GetInstance()->GetSelectedTextureMonster(tex, GetAttrib()->id, nTextIndex);
					if(texSelected != NULL)
					{
						tex = texSelected;
					}
				}

				sOffsetX = res->GetTextureOffsetX(nTextIndex);
				sOffsetY = res->GetTextureOffsetY(nTextIndex);
				//	根据玩家位置 得先算出来正常矩形的位置
				nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
				nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0,
					0,
					m_texs->GetTextureWidth(nTextIndex),
					m_texs->GetTextureHeight(nTextIndex));
				m_pRender->Render(nDrawOffsetX + sOffsetX,
					nDrawOffsetY + sOffsetY);
			}
		}
	}
	else if(GetStatus() == PST_EXT3)
	{
		if(m_texs)
		{
			GameTextureManager* res = m_texs;
			//	画怪物
			//int nTextIndex = CalMonIndex() + 180 + GetCurFrame();
			int nTextIndex = 4200 + GetDirection() * 10 + m_bCurFrame;

			HTEXTURE tex = res->GetTexture(nTextIndex);
			short sOffsetX = 0;
			short sOffsetY = 0;
			int nDrawOffsetX = -1;
			int nDrawOffsetY = -1;
			if(tex)
			{
				if(GameScene::sThis->GetTarget() == this)
				{
					HTEXTURE texSelected = SelectedTextureManager::GetInstance()->GetSelectedTextureMonster(tex, GetAttrib()->id, nTextIndex);
					if(texSelected != NULL)
					{
						tex = texSelected;
					}
				}

				sOffsetX = res->GetTextureOffsetX(nTextIndex);
				sOffsetY = res->GetTextureOffsetY(nTextIndex);
				//	根据玩家位置 得先算出来正常矩形的位置
				nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
				nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
				m_pRender->SetTexture(tex);
				m_pRender->SetTextureRect(0,
					0,
					m_texs->GetTextureWidth(nTextIndex),
					m_texs->GetTextureHeight(nTextIndex));
				m_pRender->Render(nDrawOffsetX + sOffsetX,
					nDrawOffsetY + sOffsetY);
			}
		}
	}

	if(GetRenderMode() == ORM_TRANSPARENT)
	{
		//	round
		{
			if(m_texs)
			{
				GameTextureManager* res = m_texs;
				//	画怪物
				int nTextIndex = 4520 + m_nCurRoundFrame;
				//int nTextIndex =  + GetDirection() * 10 + m_bCurFrame;

				HTEXTURE tex = res->GetTexture(nTextIndex);
				short sOffsetX = 0;
				short sOffsetY = 0;
				int nDrawOffsetX = -1;
				int nDrawOffsetY = -1;
				if(tex)
				{
					sOffsetX = res->GetTextureOffsetX(nTextIndex);
					sOffsetY = res->GetTextureOffsetY(nTextIndex);
					//	根据玩家位置 得先算出来正常矩形的位置
					nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
					nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
					MagicElement::pEffectRender->SetTexture(tex);
					MagicElement::pEffectRender->SetTextureRect(0,
						0,
						m_texs->GetTextureWidth(nTextIndex),
						m_texs->GetTextureHeight(nTextIndex));
					MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
						nDrawOffsetY + sOffsetY);
				}
			}
		}

		//
		if(GetStatus() == PST_WALK/* ||
			GetStatus() == PST_ATTACKNOWEAPON ||
			GetStatus() == PST_ATTACKED*/)
		{
			if(m_texs)
			{
				GameTextureManager* res = m_texs;
				//	画怪物
				int nTextIndex = CalMonIndex() + 4280 - 3700 + GetCurFrame();
				//int nTextIndex =  + GetDirection() * 10 + m_bCurFrame;

				HTEXTURE tex = res->GetTexture(nTextIndex);
				short sOffsetX = 0;
				short sOffsetY = 0;
				int nDrawOffsetX = -1;
				int nDrawOffsetY = -1;
				if(tex)
				{
					sOffsetX = res->GetTextureOffsetX(nTextIndex);
					sOffsetY = res->GetTextureOffsetY(nTextIndex);
					//	根据玩家位置 得先算出来正常矩形的位置
					nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
					nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
					MagicElement::pEffectRender->SetTexture(tex);
					MagicElement::pEffectRender->SetTextureRect(0,
						0,
						m_texs->GetTextureWidth(nTextIndex),
						m_texs->GetTextureHeight(nTextIndex));
					MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
						nDrawOffsetY + sOffsetY);
				}
			}
		}
		else if(GetStatus() == PST_ATTACKNOWEAPON)
		{
			if(m_texs)
			{
				GameTextureManager* res = m_texs;
				//	画怪物
				//int nTextIndex = CalMonIndex() + 4280 - 3700 + GetCurFrame();
				int nTextIndex = 4540 + GetDirection() * 10 + m_bCurFrame;

				HTEXTURE tex = res->GetTexture(nTextIndex);
				short sOffsetX = 0;
				short sOffsetY = 0;
				int nDrawOffsetX = -1;
				int nDrawOffsetY = -1;
				if(tex)
				{
					sOffsetX = res->GetTextureOffsetX(nTextIndex);
					sOffsetY = res->GetTextureOffsetY(nTextIndex);
					//	根据玩家位置 得先算出来正常矩形的位置
					nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
					nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
					MagicElement::pEffectRender->SetTexture(tex);
					MagicElement::pEffectRender->SetTextureRect(0,
						0,
						m_texs->GetTextureWidth(nTextIndex),
						m_texs->GetTextureHeight(nTextIndex));
					MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
						nDrawOffsetY + sOffsetY);
				}
			}
		}
		else if(GetStatus() == PST_EXT2)
		{
			if(m_texs)
			{
				GameTextureManager* res = m_texs;
				//	画怪物
				//int nTextIndex = CalMonIndex() + 4280 - 3700 + GetCurFrame();
				int nTextIndex = 4640 + GetDirection() * 10 + m_bCurFrame;

				HTEXTURE tex = res->GetTexture(nTextIndex);
				short sOffsetX = 0;
				short sOffsetY = 0;
				int nDrawOffsetX = -1;
				int nDrawOffsetY = -1;
				if(tex)
				{
					sOffsetX = res->GetTextureOffsetX(nTextIndex);
					sOffsetY = res->GetTextureOffsetY(nTextIndex);
					//	根据玩家位置 得先算出来正常矩形的位置
					nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
					nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
					MagicElement::pEffectRender->SetTexture(tex);
					MagicElement::pEffectRender->SetTextureRect(0,
						0,
						m_texs->GetTextureWidth(nTextIndex),
						m_texs->GetTextureHeight(nTextIndex));
					MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
						nDrawOffsetY + sOffsetY);
				}
			}
		}
		else if(GetStatus() == PST_EXT3)
		{
			if(m_texs)
			{
				GameTextureManager* res = m_texs;
				//	画怪物
				//int nTextIndex = CalMonIndex() + 4280 - 3700 + GetCurFrame();
				int nTextIndex = 4730 + m_bCurFrame;

				HTEXTURE tex = res->GetTexture(nTextIndex);
				short sOffsetX = 0;
				short sOffsetY = 0;
				int nDrawOffsetX = -1;
				int nDrawOffsetY = -1;
				if(tex)
				{
					sOffsetX = res->GetTextureOffsetX(nTextIndex);
					sOffsetY = res->GetTextureOffsetY(nTextIndex);
					//	根据玩家位置 得先算出来正常矩形的位置
					nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
					nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
					MagicElement::pEffectRender->SetTexture(tex);
					MagicElement::pEffectRender->SetTextureRect(0,
						0,
						m_texs->GetTextureWidth(nTextIndex),
						m_texs->GetTextureHeight(nTextIndex));
					MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
						nDrawOffsetY + sOffsetY);
				}
			}
		}
	}
}

void BehemothDevourerMonster::Update(float _dt)
{
	float dt = _dt;

	if(GetStatus() == PST_ATTACKNOWEAPON)
	{
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

				if(m_nLastAtkTarget != 0)
				{
					PacketHandler* pHandler = SocketDataCenter::GetInstance().GetHandler(m_nLastAtkTarget);
					if(NULL != pHandler)
					{
						GameObject* pObj = static_cast<GameObject*>(pHandler);

						MagicElement* pAni = new MagicMonsPosAni;
						if(pAni->Create(MAKELONG(pObj->GetObjx(), pObj->GetObjy()), this, NULL, 4620, 5, 90))
						{
							pAni->SetActive();
							GameScene::sThis->InsertNewMagic(pAni);
						}
						else
						{
							SAFE_DELETE(pAni);
						}
					}
				}

				m_nLastAtkTarget = 0;
			}
		}
	}
	else if(GetStatus() == PST_EXT1)
	{
		if(GetStatus() == PST_EXT1 &&
			!IsLastFrame())
		{
			m_fLastUpdateSkill += _dt;
			if(m_fLastUpdateSkill > /*0.1f*/0.1f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateSkill = 0.0f;
			}
		}
		else if(GetStatus() == PST_EXT1 &&
			IsLastFrame())
		{
			m_fLastUpdateSkill += _dt;
			if(m_fLastUpdateSkill > /*0.2f*/0.1f)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateSkill = 0.0f;
			}
		}
	}
	else if(GetStatus() == PST_EXT2)
	{
		if(GetStatus() == PST_EXT2 &&
			!IsLastFrame())
		{
			m_fLastUpdateSpeAtk2 += _dt;
			if(m_fLastUpdateSpeAtk2 > /*0.1f*/0.1f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateSpeAtk2 = 0.0f;
			}
		}
		else if(GetStatus() == PST_EXT2 &&
			IsLastFrame())
		{
			m_fLastUpdateSpeAtk2 += _dt;
			if(m_fLastUpdateSpeAtk2 > /*0.2f*/0.1f)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateSpeAtk2 = 0.0f;

				MagicElement* pAni = new MagicMonsPosAni;
				if(pAni->Create(MAKELONG(GetObjx(), GetObjy()), this, NULL, 4530, 9, 90))
				{
					pAni->SetActive();
					GameScene::sThis->InsertNewMagic(pAni);
				}
				else
				{
					SAFE_DELETE(pAni);
				}

				m_nLastAtkTarget = 0;
			}
		}
	}
	else if(GetStatus() == PST_EXT3)
	{
		if(GetStatus() == PST_EXT3 &&
			!IsLastFrame())
		{
			m_fLastUpdateSpeAtk3 += _dt;
			if(m_fLastUpdateSpeAtk3 > /*0.1f*/0.1f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateSpeAtk3 = 0.0f;
			}
		}
		else if(GetStatus() == PST_EXT3 &&
			IsLastFrame())
		{
			m_fLastUpdateSpeAtk3 += _dt;
			if(m_fLastUpdateSpeAtk3 > /*0.2f*/0.1f)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateSpeAtk3 = 0.0f;

				MagicElement* pAni = new MagicMonsPosAni;
				if(pAni->Create(MAKELONG(GetObjx(), GetObjy()), this, NULL, 4740, 9, 90))
				{
					pAni->SetActive();
					GameScene::sThis->InsertNewMagic(pAni);
				}
				else
				{
					SAFE_DELETE(pAni);
				}

				m_nLastAtkTarget = 0;
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}

	if(GetTickCount() - m_dwLastChgRoundTime > 500)
	{
		m_dwLastChgRoundTime = GetTickCount();

		if(m_nCurRoundFrame >= 9)
		{
			m_nCurRoundFrame = 0;
		}
		else
		{
			++m_nCurRoundFrame;
		}
	}
}

bool BehemothDevourerMonster::IsLastFrame()
{
	if(GetStatus() == PST_EXT1)
	{
		return (GetCurFrame() == (6 - 1));
	}
	else if(GetStatus() == PST_EXT2)
	{
		return (GetCurFrame() == (7 - 1));
	}
	else if(GetStatus() == PST_EXT3)
	{
		return (GetCurFrame() == (7 - 1));
	}
	else
	{
		return __super::IsLastFrame();
	}
}

/*void BehemothDevourerMonster::OnPacket(const PacketHeader* _pPkt)
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
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,		PkgObjectActionNot)
		END_HANDLE_PACKET(_pPkt)

	__super::OnPacket(_pPkt);
}

void BehemothDevourerMonster::DoPacket(const PkgObjectActionNot& _not)
{
	__super::DoPacket(_not);

	if(_not.uAction == ACTION_ATTACK)
	{
		if(_not.uUserId == 0)
		{
			m_nLastAtkTarget = 0;
		}
		else
		{
			m_nLastAtkTarget = _not.uUserId;
		}
	}
	else if(_not.uAction == ACTION_EXT1)
	{
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 6);

		m_fLastUpdateSkill = 0.0f;
		SetStatus(PST_EXT1);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		int nDrt = HIWORD(_not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
		m_nLastAtkTarget = _not.uUserId;

		if(m_nLastAtkTarget != 0)
		{
			PacketHandler* pHandler = SocketDataCenter::GetInstance().GetHandler(m_nLastAtkTarget);
			if(NULL != pHandler)
			{
				GameObject* pObj = static_cast<GameObject*>(pHandler);

				MagicElement* pAni = new MagicMonsPosAni;
				if(pAni->Create(MAKELONG(pObj->GetObjx(), pObj->GetObjy()), this, NULL, 4630, 9, 90))
				{
					pAni->SetActive();
					GameScene::sThis->InsertNewMagic(pAni);
				}
				else
				{
					SAFE_DELETE(pAni);
				}
			}
		}

		m_nLastAtkTarget = 0;
	}
	else if(_not.uAction == ACTION_EXT2)
	{
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 7);

		m_fLastUpdateSpeAtk2 = 0.0f;
		SetStatus(PST_EXT2);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		int nDrt = HIWORD(_not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
		m_nLastAtkTarget = _not.uUserId;
	}
	else if(_not.uAction == ACTION_EXT3)
	{
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 8);

		m_fLastUpdateSpeAtk2 = 0.0f;
		SetStatus(PST_EXT3);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		int nDrt = HIWORD(_not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
		m_nLastAtkTarget = _not.uUserId;
	}
}*/

void BehemothDevourerMonster::OnObjectAction(const PkgObjectActionNot& _not)
{
	if(_not.uAction == ACTION_ATTACK)
	{
		if(_not.uUserId == 0)
		{
			m_nLastAtkTarget = 0;
		}
		else
		{
			m_nLastAtkTarget = _not.uUserId;
		}
	}
	else if(_not.uAction == ACTION_EXT1)
	{
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 6);

		m_fLastUpdateSkill = 0.0f;
		SetStatus(PST_EXT1);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		int nDrt = HIWORD(_not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
		m_nLastAtkTarget = _not.uUserId;

		if(m_nLastAtkTarget != 0)
		{
			PacketHandler* pHandler = SocketDataCenter::GetInstance().GetHandler(m_nLastAtkTarget);
			if(NULL != pHandler)
			{
				GameObject* pObj = static_cast<GameObject*>(pHandler);

				MagicElement* pAni = new MagicMonsPosAni;
				if(pAni->Create(MAKELONG(pObj->GetObjx(), pObj->GetObjy()), this, NULL, 4630, 9, 90))
				{
					pAni->SetActive();
					GameScene::sThis->InsertNewMagic(pAni);
				}
				else
				{
					SAFE_DELETE(pAni);
				}
			}
		}

		m_nLastAtkTarget = 0;
	}
	else if(_not.uAction == ACTION_EXT2)
	{
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 7);

		m_fLastUpdateSpeAtk2 = 0.0f;
		SetStatus(PST_EXT2);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		int nDrt = HIWORD(_not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
		m_nLastAtkTarget = _not.uUserId;
	}
	else if(_not.uAction == ACTION_EXT3)
	{
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 8);

		m_fLastUpdateSpeAtk2 = 0.0f;
		SetStatus(PST_EXT3);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		int nDrt = HIWORD(_not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
		m_nLastAtkTarget = _not.uUserId;
	}

	__super::OnObjectAction(_not);
}