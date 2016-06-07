#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
IceServantMonster::IceServantMonster()
{
	m_fLastUpdateSkill = 0.0f;
}

/*void IceServantMonster::OnPacket(const PacketHeader* _pPkt)
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
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,			PkgObjectActionNot)
		HANDLE_DEFAULT(_pPkt)
		END_HANDLE_PACKET(_pPkt)
}

void IceServantMonster::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_SPELL)
	{
		m_fLastUpdateSkill = 0.0f;
		SetStatus(PST_SK_LEIDIANSHU);
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));

		int nDrt = HIWORD(not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
	}

	__super::DoPacket(not);
}*/
void IceServantMonster::OnObjectAction(const PkgObjectActionNot &not)
{
	if(not.uAction == ACTION_SPELL)
	{
		m_fLastUpdateSkill = 0.0f;
		SetStatus(PST_SK_LEIDIANSHU);
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));

		int nDrt = HIWORD(not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
	}

	__super::OnObjectAction(not);
}

void IceServantMonster::Update(float _dt)
{
	float dt = _dt;

	if(GetStatus() == PST_SK_LEIDIANSHU)
	{
		int nPreFrame = GetCurFrame();

		if(GetStatus() == PST_SK_LEIDIANSHU &&
			!IsLastFrame())
		{
			m_fLastUpdateSkill += _dt;
			if(m_fLastUpdateSkill > /*0.1f*/0.1f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateSkill = 0.0f;
			}
		}
		else if(GetStatus() == PST_SK_LEIDIANSHU &&
			IsLastFrame())
		{
			m_fLastUpdateSkill += _dt;
			if(m_fLastUpdateSkill > /*0.2f*/0.1f)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateSkill = 0.0f;
			}
		}

		if(nPreFrame == 3 &&
			GetCurFrame() == 4)
		{
			MagicElement* pAni = new MagicMonsPosAni;
			if(pAni->Create(MAKELONG(GetObjx(), GetObjy()), this, NULL, 420 + GetDirection() * 10, 20, 40))
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
	else
	{
		__super::Update(_dt);
	}
}

bool IceServantMonster::IsLastFrame()
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
		}break;
	case PST_SK_LEIDIANSHU:
		{
			if(GetCurFrame() == 9)
			{
				return true;
			}
		}break;
	}
	return false;
}

void IceServantMonster::Render()
{
	__super::Render();
}