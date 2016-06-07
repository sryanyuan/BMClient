#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
MoonBeastMonster::MoonBeastMonster()
{
	m_fLastUpdateSpeAtk1 = 0.0f;
	m_fLastUpdateSpeAtk2 = 0.0f;
	m_fLastUpdateSpeAtk3 = 0.0f;
	m_fLastUpdateSpeAtk4 = 0.0f;
	m_fLastUpdateSpeAtk5 = 0.0f;
}

void MoonBeastMonster::OnPacket(const PacketHeader* _pPkt)
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

void MoonBeastMonster::DoPacket(const PkgPlayerPlayAniAck& ack)
{
	MagicMonsPosAni* pMgc = NULL;
	DWORD dwPos = 0;

	if(!ack.xPos.empty())
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

			if(pMgc->Create(MAKELONG(dwX, dwY), this, NULL, 5470, 10, 70))
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
}

void MoonBeastMonster::OnObjectAction(const PkgObjectActionNot& not)
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
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 7);
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
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 7);
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
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 8);
	}
	else if(not.uAction == ACTION_EXT4)
	{
		m_fLastUpdateSpeAtk4 = 0.0f;
		SetStatus(PST_EXT4);
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
		int nDrt = HIWORD(not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
		GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 9);
	}

	__super::OnObjectAction(not);
}

void MoonBeastMonster::Update(float _dt)
{
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
	else if(GetStatus() == PST_EXT4)
	{
		if(GetStatus() == PST_EXT4 &&
			!IsLastFrame())
		{
			m_fLastUpdateSpeAtk4 += _dt;
			if(m_fLastUpdateSpeAtk4 > /*0.1f*/0.1f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateSpeAtk4 = 0.0f;
			}
		}
		else if(GetStatus() == PST_EXT4 &&
			IsLastFrame())
		{
			m_fLastUpdateSpeAtk4 += _dt;
			if(m_fLastUpdateSpeAtk4 > /*0.2f*/0.1f)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateSpeAtk4 = 0.0f;
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}

bool MoonBeastMonster::IsLastFrame()
{
	if(GetStatus() == PST_EXT1)
	{
		return (4 == GetCurFrame());
	}
	else if(GetStatus() == PST_EXT2)
	{
		return (7 == GetCurFrame());
	}
	else if(GetStatus() == PST_EXT3)
	{
		return (4 == GetCurFrame());
	}
	else if(GetStatus() == PST_EXT4)
	{
		return (7 == GetCurFrame());
	}

	return __super::IsLastFrame();
}

void MoonBeastMonster::Render()
{
	if(GetRenderMode() == ORM_TRANSPARENT)
	{
		if(GetStatus() == PST_EXT3)
		{
			int nTexIndex = 5450 + GetCurFrame();

			//	effect
			GamePlayer* pPlayer = GamePlayer::GetInstance();
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

	if(GetStatus() == PST_EXT1)
	{
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		int nTexIndex = 4890 + GetDirection() * 10 + GetCurFrame();
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

		int nTexIndex = 4970 + GetDirection() * 10 + GetCurFrame();
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

		int nTexIndex = 5050 + GetDirection() * 10 + GetCurFrame();
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
	else if(GetStatus() == PST_EXT4)
	{
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		int nTexIndex = 5130 + GetDirection() * 10 + GetCurFrame();
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
		if(GetStatus() == PST_EXT1)
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
	}
}