#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
MoonWarLordMonster::MoonWarLordMonster()
{
	m_fLastUpdateSpeAtk1 = 0.0f;
	m_fLastUpdateSpeAtk2 = 0.0f;
	m_fLastUpdateSpeAtk3 = 0.0f;
	m_fLastUpdateSpeAtk4 = 0.0f;
	m_fLastUpdateSpeAtk5 = 0.0f;
	m_dwDefenceEffExpireTime = 0;
	m_dwLastUpdateDefenceEffTime = 0;
}

void MoonWarLordMonster::OnPacket(const PacketHeader* _pPkt)
{
	g_xBuffer.Reset();
	unsigned int uWriteLen = g_xBuffer.Write(_pPkt, _pPkt->uLen);
	if(uWriteLen != _pPkt->uLen)
	{
		ALERT_MSGBOX("–¥»Î»´æ÷Buffer ß∞‹");
		::PostQuitMessage(0);
		return;
	}

	BEGIN_HANDLE_PACKET(_pPkt)
		HANDLE_PACKET(PKG_GAME_PLAYER_PLAYANI_ACK,		PkgPlayerPlayAniAck)
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,		PkgObjectActionNot)
		HANDLE_DEFAULT(_pPkt)
		END_HANDLE_PACKET(_pPkt)
}

void MoonWarLordMonster::DoPacket(const PkgPlayerPlayAniAck& ack)
{
	MagicMonsPosAni* pMgc = NULL;
	DWORD dwPos = 0;

	if(!ack.xPos.empty())
	{
		if(ack.wAniID == 0)
		{
			std::list<DWORD>::const_iterator begiter = ack.xPos.begin();
			for(begiter;
				begiter != ack.xPos.end();
				++begiter)
			{
				pMgc = new MagicMonsPosAni;
				dwPos = *begiter;
				DWORD dwX = LOWORD(dwPos) * UNIT_WIDTH;
				DWORD dwY = HIWORD(dwPos) * UNIT_HEIGHT;

				if(pMgc->Create(MAKELONG(dwX, dwY), this, NULL, 1590, 7, 70))
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
		else if(ack.wAniID == 1)
		{
			DWORD dwLastTime = ack.xPos.front();
			m_dwDefenceEffExpireTime = GetTickCount() + dwLastTime;
		}
	}
}

void MoonWarLordMonster::OnObjectAction(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_EXT1)
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
	}
	else if(not.uAction == ACTION_EXT2)
	{
		m_fLastUpdateSpeAtk2 = 0.0f;
		SetStatus(PST_EXT2);
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
		int nDrt = HIWORD(not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
		//	magic
		MagicElement* pAni = new MagicMonsPosAni;
		if(pAni->Create(MAKELONG(GetObjx(), GetObjy()), this, NULL, 1580, 10, 50))
		{
			pAni->SetActive();
			GameScene::sThis->InsertNewMagic(pAni);
			GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 7);
		}
		else
		{
			SAFE_DELETE(pAni);
		}
	}
	else if(not.uAction == ACTION_EXT3)
	{
		m_fLastUpdateSpeAtk3 = 0.0f;
		SetStatus(PST_EXT3);
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
		int nDrt = HIWORD(not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 9);
		m_dwLastUpdateDefenceEffTime = GetTickCount();
	}

	__super::OnObjectAction(not);
}

void MoonWarLordMonster::Update(float _dt)
{
	if(m_dwDefenceEffExpireTime != 0)
	{
		if(GetTickCount() > m_dwDefenceEffExpireTime)
		{
			m_dwDefenceEffExpireTime = 0;
			m_dwLastUpdateDefenceEffTime = 0;
		}
	}
	else
	{
		m_dwLastUpdateDefenceEffTime = 0;
	}

	if(GetStatus() == PST_EXT1)
	{
		if(GetStatus() == PST_EXT1 &&
			!IsLastFrame())
		{
			m_fLastUpdateSpeAtk1 += _dt;
			if(m_fLastUpdateSpeAtk1 > /*0.1f*/0.1f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateSpeAtk1 = 0.0f;
			}
		}
		else if(GetStatus() == PST_EXT1 &&
			IsLastFrame())
		{
			m_fLastUpdateSpeAtk1 += _dt;
			if(m_fLastUpdateSpeAtk1 > /*0.2f*/0.1f)
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
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}

bool MoonWarLordMonster::IsLastFrame()
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

void MoonWarLordMonster::Render()
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

	if(GetRenderMode() == ORM_TRANSPARENT)
	{
		if(GetStatus() == PST_ATTACKNOWEAPON)
		{
			int nTexIndex = 1420 + 10 * GetDirection() + GetCurFrame();

			//	effect
			GamePlayer* pPlayer = GamePlayer::GetInstance();
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
		else if(GetStatus() == PST_EXT1)
		{
			int nTexIndex = 5290 + 10 * GetDirection() + GetCurFrame();

			//	effect
			GamePlayer* pPlayer = GamePlayer::GetInstance();
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
		else if(GetStatus() == PST_EXT2)
		{
			int nTexIndex = 5370 + 10 * GetDirection() + GetCurFrame();

			//	effect
			GamePlayer* pPlayer = GamePlayer::GetInstance();
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

		if(m_dwDefenceEffExpireTime != 0)
		{
			int nTexIndex = ((GetTickCount() - m_dwLastUpdateDefenceEffTime) / 60) % 7 + 1600;

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
}