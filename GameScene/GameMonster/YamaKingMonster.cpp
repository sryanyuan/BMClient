#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
YamaKingMonster::YamaKingMonster()
{
	m_fLastUpdateSpeAtk1 = 0.0f;
	m_fLastUpdateSpeAtk2 = 0.0f;
	m_fLastUpdateSpeAtk3 = 0.0f;
	m_fLastUpdateSpeAtk4 = 0.0f;
	m_eEndStatus = PST_ATTACKNOWEAPON;
	m_nLastAtkTarget = 0;
}

void YamaKingMonster::OnPacket(const PacketHeader* _pPkt)
{
	g_xBuffer.Reset();
	unsigned int uWriteLen = g_xBuffer.Write(_pPkt, _pPkt->uLen);
	if(uWriteLen != _pPkt->uLen)
	{
		ALERT_MSGBOX("Ð´ÈëÈ«¾ÖBufferÊ§°Ü");
		::PostQuitMessage(0);
		return;
	}

	BEGIN_HANDLE_PACKET(_pPkt)
		HANDLE_PACKET(PKG_GAME_PLAYER_PLAYANI_ACK,		PkgPlayerPlayAniAck)
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,		PkgObjectActionNot)
		HANDLE_DEFAULT(_pPkt)
		END_HANDLE_PACKET(_pPkt)
}

void YamaKingMonster::OnObjectAction(const PkgObjectActionNot& not)
{
	/*if(not.uAction == ACTION_EXT1)
	{
		m_fLastUpdateSpeAtk1 = 0.0f;
		SetStatus(PST_EXT1);
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
		int nDrt = HIWORD(not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 2);
	}*/

	if(not.uAction == ACTION_EXT2)
	{
		SetStatus(PST_ATTACKNOWEAPON);
		m_eEndStatus = PST_EXT2;
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
		m_nLastAtkTarget = not.uUserId;
	}
	else if(not.uAction == ACTION_EXT3)
	{
		SetStatus(PST_ATTACKNOWEAPON);
		m_eEndStatus = PST_EXT3;
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
		m_nLastAtkTarget = not.uUserId;
	}

	__super::OnObjectAction(not);
}

void YamaKingMonster::Update(float _dt)
{
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
				if(pAni->Create(MAKELONG(GetObjx(), GetObjy()), this, NULL, 1500 + 10 * GetDirection(), 7, 50))
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
			}
		}
	}
	else if(GetStatus() == PST_ATTACKNOWEAPON)
	{
		if(!IsLastFrame())
		{
			m_fLastUpdateNoWeapon += _dt;
			if(m_fLastUpdateNoWeapon > fAtkInterval)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateNoWeapon = 0.0f;
			}
		}
		else
		{
			m_fLastUpdateNoWeapon += _dt;
			if(m_fLastUpdateNoWeapon > fAtkInterval)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateNoWeapon = 0.0f;

				//	magic
				if(m_eEndStatus == PST_EXT2)
				{
					if(m_nLastAtkTarget != 0)
					{
						PacketHandler* pHandler = SocketDataCenter::GetInstance().GetHandler(m_nLastAtkTarget);
						if(NULL != pHandler)
						{
							GameObject* pObj = static_cast<GameObject*>(pHandler);

							MagicElement* pAni = new MagicMonsPosAni;
							if(pAni->Create(MAKELONG(pObj->GetObjx(), pObj->GetObjy()), this, NULL, 3570, 10, 50))
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
					m_nLastAtkTarget = 0;
				}
				else if(m_eEndStatus == PST_EXT3)
				{
					if(m_nLastAtkTarget != 0)
					{
						PacketHandler* pHandler = SocketDataCenter::GetInstance().GetHandler(m_nLastAtkTarget);
						if(NULL != pHandler)
						{
							GameObject* pObj = static_cast<GameObject*>(pHandler);

							MagicElement* pAni = new MagicMonsPosAni;
							if(pAni->Create(MAKELONG(pObj->GetObjx(), pObj->GetObjy()), this, NULL, 3590, 10, 50))
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
					m_nLastAtkTarget = 0;
				}
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}

bool YamaKingMonster::IsLastFrame()
{
	if(GetStatus() == PST_EXT1)
	{
		return (7 == GetCurFrame());
	}
	else if(GetStatus() == PST_EXT2)
	{
		return (7 == GetCurFrame());
	}
	else if(GetStatus() == PST_EXT3)
	{
		return (7 == GetCurFrame());
	}

	return __super::IsLastFrame();
}

void YamaKingMonster::Render()
{
	if(GetStatus() == PST_EXT1)
	{
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		int nTexIndex = 1100 + GetDirection() * 10 + GetCurFrame();
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
	else if(GetStatus() == PST_EXT2)
	{
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		int nTexIndex = 1180 + GetDirection() * 10 + GetCurFrame();
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
	else if(GetStatus() == PST_EXT3)
	{
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		int nTexIndex = 1180 + GetDirection() * 10 + GetCurFrame();
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

	if(GetStatus() == PST_DEAD)
	{
		int nTexIndex = 3500 + GetCurFrame();
		HTEXTURE tex = m_texs->GetTexture(nTexIndex);

		if(tex)
		{
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
		int nTexIndex = 3490;
		HTEXTURE tex = m_texs->GetTexture(nTexIndex);

		if(tex)
		{
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

	if(GetRenderMode() == ORM_TRANSPARENT)
	{
		int nFrame = (GetTickCount() / 50) % 6;
		int nTexIndex = 3510 + nFrame;

		HTEXTURE tex = m_texs->GetTexture(nTexIndex);

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

void YamaKingMonster::DoPacket(const PkgPlayerPlayAniAck& ack)
{
	MagicNormalThron* pNewMagic = NULL;

	if(!ack.xPos.empty())
	{
		std::list<DWORD>::const_iterator begiter = ack.xPos.begin();
		for(begiter;
			begiter != ack.xPos.end();
			++begiter)
		{
			pNewMagic = new MagicNormalThron;

			if(pNewMagic->Create(m_attrib.id, this, NULL, LOWORD(*begiter) * UNIT_WIDTH, HIWORD(*begiter) * UNIT_HEIGHT, ack.uUserId))
			{
				pNewMagic->SetActive();
				GameScene::sThis->InsertNewMagic(pNewMagic);
			}
			else
			{
				SAFE_DELETE(pNewMagic);
			}
		}
	}
}