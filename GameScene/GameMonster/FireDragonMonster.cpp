#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
FireDragonMonster::FireDragonMonster()
{
	m_nLastAtkTarget = 0;
	m_fLastUpdateSpeAtk1 = 0;
}

void FireDragonMonster::OnPacket(const PacketHeader *_pPkt)
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
		HANDLE_PACKET(PKG_GAME_PLAYER_PLAYANI_ACK,		PkgPlayerPlayAniAck)
		HANDLE_PACKET(PKG_GAME_MONSTER_FIREDRAGON_ATTACK0_NOT,			PkgMonsterFireDragonAttack0Not)
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT, PkgObjectActionNot)
		HANDLE_DEFAULT(_pPkt)
		END_HANDLE_PACKET(_pPkt)
}

void FireDragonMonster::Update(float _dt)
{
	float dt = _dt;
	float fAtkInterval = GetAttackInterval();

	if(GetStatus() == PST_EXT1)
	{
		if(GetStatus() == PST_EXT1 &&
			!IsLastFrame())
		{
			m_fLastUpdateSpeAtk1 += _dt;
			if(m_fLastUpdateSpeAtk1 > /*0.1f*/fAtkInterval)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateSpeAtk1 = 0.0f;
			}
		}
		else if(GetStatus() == PST_EXT1 &&
			IsLastFrame())
		{
			m_fLastUpdateSpeAtk1 += _dt;
			if(m_fLastUpdateSpeAtk1 > /*0.2f*/fAtkInterval)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateSpeAtk1 = 0.0f;

				//	magic
				MagicElement* pAni = new MagicMonsPosAni;
				if(pAni->Create(MAKELONG(GetObjx(), GetObjy()), this, NULL, 68, 14, 50))
				{
					pAni->SetActive();
					GameScene::sThis->InsertNewMagic(pAni);
					//GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 7);
					GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 7);
				}
				else
				{
					SAFE_DELETE(pAni);
				}

				PlayAttack1Effect();
			}
		}
	}
	else if(GetStatus() == PST_ATTACKNOWEAPON)
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

				if(!m_xAttack0List.empty())
				{
					PkgMonsterFireDragonAttack0Not not = m_xAttack0List.front();
					m_xAttack0List.pop_front();

					if(not.uUserId != 0)
					{
						MagicElement* pAni = new MagicMonsPosAni;
						if(pAni->Create(MAKELONG(LOWORD(not.dwExplosionPos) * UNIT_WIDTH, HIWORD(not.dwExplosionPos) * UNIT_HEIGHT), this, NULL, 200, 20, 160))
						{
							pAni->SetActive();
							GameScene::sThis->InsertNewMagic(pAni);

							//	sound
							//GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 6);
						}
						else
						{
							SAFE_DELETE(pAni);
						}
					}
				}
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}

void FireDragonMonster::Render()
{
	HTEXTURE tex = 0;

	if(GetStatus() == PST_EXT1)
	{
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		int nTexIndex = 40 + GetCurFrame();
		HTEXTURE tex = m_texs->GetTexture(nTexIndex);

		if(tex)
		{
			if(GameScene::sThis->GetTarget() == this)
			{
				HTEXTURE texSelected = SelectedTextureManager::GetInstance()->GetSelectedTextureMonster(tex, GetAttrib()->id, nTexIndex);
				if(texSelected != NULL)
				{
					tex = texSelected;
				}
			}

			short sOftX = m_texs->GetTextureOffsetX(nTexIndex);
			short sOftY = m_texs->GetTextureOffsetY(nTexIndex);

			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0,
				m_texs->GetTextureWidth(nTexIndex),
				m_texs->GetTextureHeight(nTexIndex));

			int nDrawOffsetX = GetScreenPosX();
			int nDrawOffsetY = GetScreenPosY();

			m_pRender->Render(nDrawOffsetX + sOftX, nDrawOffsetY + sOftY);
		}
	}
	else
	{
		__super::Render();
	}

	if(GetRenderMode() == ORM_TRANSPARENT)
	{
		if(GetStatus() == PST_ATTACKNOWEAPON)
		{
			//	火龙喷火
			int nTexIndex = CalMonIndex() + 80 + GetCurFrame();

			tex = m_texs->GetTexture(nTexIndex);

			if(tex)
			{
				short sOftX = m_texs->GetTextureOffsetX(nTexIndex);
				short sOftY = m_texs->GetTextureOffsetY(nTexIndex);

				MagicElement::pEffectRender->SetTexture(tex);
				MagicElement::pEffectRender->SetTextureRect(0, 0,
					m_texs->GetTextureWidth(nTexIndex),
					m_texs->GetTextureHeight(nTexIndex));

				int nDrawOffsetX = GetScreenPosX();
				int nDrawOffsetY = GetScreenPosY();

				MagicElement::pEffectRender->Render(nDrawOffsetX + sOftX, nDrawOffsetY + sOftY);
			}
		}
		else if(GetStatus() == PST_EXT1)
		{
			if(GetCurFrame() <= 7)
			{
				int nTexIndex = CalMonIndex() + 20 + GetCurFrame();

				tex = m_texs->GetTexture(nTexIndex);

				if(tex)
				{
					short sOftX = m_texs->GetTextureOffsetX(nTexIndex);
					short sOftY = m_texs->GetTextureOffsetY(nTexIndex);

					MagicElement::pEffectRender->SetTexture(tex);
					MagicElement::pEffectRender->SetTextureRect(0, 0,
						m_texs->GetTextureWidth(nTexIndex),
						m_texs->GetTextureHeight(nTexIndex));

					int nDrawOffsetX = GetScreenPosX();
					int nDrawOffsetY = GetScreenPosY();

					MagicElement::pEffectRender->Render(nDrawOffsetX + sOftX, nDrawOffsetY + sOftY);
				}
			}
		}
	}
}

bool FireDragonMonster::IsLastFrame()
{
	if(GetStatus() == PST_EXT1)
	{
		return (9 == GetCurFrame());
	}
	return __super::IsLastFrame();
}

int FireDragonMonster::CalMonIndex()
{
	int nIndex = 0;

	//if(m_renderInfo.drts == 4)
	{
		switch(GetStatus())
		{
		case PST_STAND:
			{
				nIndex = m_pRenderInfo->standbegin + 0 *m_pRenderInfo->standtotal;
			}break;
		case PST_WALK:
			{
				nIndex = m_pRenderInfo->walkbegin + 0 * m_pRenderInfo->walktotal;
			}break;
		case PST_ATTACKNOWEAPON:
			{
				nIndex = m_pRenderInfo->atkbegin + 0 * m_pRenderInfo->atktotal;

				//	根据攻击方向确定帧数
				if(m_drt >= 0 &&
					m_drt <= 4)
				{
					//	nothing
				}
				else if(m_drt == 5)
				{
					nIndex += m_pRenderInfo->atktotal * 1;
				}
				else
				{
					nIndex += m_pRenderInfo->atktotal * 2;
				}
			}break;
		case PST_ATTACKED:
			{
				nIndex = m_pRenderInfo->hurtbegin + 0 * m_pRenderInfo->hurttotal;
			}break;
		case PST_DEAD:
			{
				nIndex = m_pRenderInfo->deadbegin + 0 * m_pRenderInfo->deadtotal;
			}break;
		case PST_EXT1:
			{
				nIndex = 40;
			}break;
		}
	}

	return nIndex;
}

void FireDragonMonster::OnObjectAction(const PkgObjectActionNot &not)
{
	//	无受伤状态
	if(not.uAction == ACTION_STRUCK)
	{
		SetStatus(PST_STAND);
		/*
		GetAttrib()->HP = not.uParam2;
		
				ItemAttrib atr;
				if(GameInfoManager::GetInstance()->GetMonsterAttrib(140, &atr))
				{
					if(TEST_FLAG_BOOL(m_attrib.maxEXPR, MAXEXPR_MASK_ELITE))
					{
						GetAttrib()->maxHP = atr.maxHP * MONS_ELITE_MULTI;
					}
					else if(TEST_FLAG_BOOL(m_attrib.maxEXPR, MAXEXPR_MASK_LEADER))
					{
						GetAttrib()->maxHP = atr.maxHP * MONS_LEADER_MULTI;
					}
					else
					{
						GetAttrib()->maxHP = atr.maxHP;
					}
				}*/
		
	}

	__super::OnObjectAction(not);
}

void FireDragonMonster::DoPacket(const PkgMonsterFireDragonAttack0Not &not)
{
	m_xAttack0List.push_back(not);

	SetReallyCoord(LOWORD(not.dwPos), HIWORD(not.dwPos));
	SetCurFrame(0);
	SetStatus(PST_ATTACKNOWEAPON);
	SetCurFrame(0);
	if(not.nDrt >= 0 &&
		not.nDrt <= 7)
	{
		SetDirection((PLAYER_DIRECTION)not.nDrt);
	}
}

void FireDragonMonster::DoPacket(const PkgPlayerPlayAniAck &ack)
{
	m_xAttack1List.push_back(ack);
}

void FireDragonMonster::PlayAttack1Effect()
{
	if(m_xAttack1List.empty())
	{
		return;
	}

	PkgPlayerPlayAniAck& ack = m_xAttack1List.front();

	MagicMonsPosAni* pMgc = NULL;
	DWORD dwPos = 0;

	if(!ack.xPos.empty())
	{
		std::list<unsigned int>::const_iterator begiter = ack.xPos.begin();
		for(begiter;
			begiter != ack.xPos.end();
			++begiter)
		{
			pMgc = new MagicMonsPosAni;
			dwPos = *begiter;
			DWORD dwX = LOWORD(dwPos) * UNIT_WIDTH;
			DWORD dwY = HIWORD(dwPos) * UNIT_HEIGHT;

			int nAniIndex = 230 + (rand() % 6) * 10;

			if(pMgc->Create(MAKELONG(dwX, dwY), this, NULL, nAniIndex, 5, 80))
			{
				pMgc->SetActive();
				GameScene::sThis->InsertNewMagic(pMgc);
			}
			else
			{
				SAFE_DELETE(pMgc);
			}
		}
	}

	m_xAttack1List.pop_front();
}