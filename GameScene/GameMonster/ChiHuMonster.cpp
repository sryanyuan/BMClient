#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
ChiHuMonster::ChiHuMonster()
{
	m_nLastAtkTarget = 0;
}

/*void ChiHuMonster::OnPacket(const PacketHeader* _pPkt)
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

void ChiHuMonster::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_ATTACK)
	{
		MagicElement* pAni = new MagicMonsAni;
		if(pAni->Create(0, this, NULL, 800, 20, 20))
		{
			pAni->SetActive();
			GameScene::sThis->InsertNewMagic(pAni);
			GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 7);
		}
		else
		{
			SAFE_DELETE(pAni);
		}

		if(not.uUserId != 0)
		{
			m_nLastAtkTarget = not.uUserId;
		}
	}

	__super::DoPacket(not);
}*/
void ChiHuMonster::OnObjectAction(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_ATTACK)
	{
		MagicElement* pAni = new MagicMonsAni;
		if(pAni->Create(0, this, NULL, 800, 20, 20))
		{
			pAni->SetActive();
			GameScene::sThis->InsertNewMagic(pAni);
			GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 7);
		}
		else
		{
			SAFE_DELETE(pAni);
		}

		if(not.uUserId != 0)
		{
			m_nLastAtkTarget = not.uUserId;
		}
	}

	__super::OnObjectAction(not);
}

void ChiHuMonster::Update(float _dt)
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
						if(pAni->Create(MAKELONG(pObj->GetObjx(), pObj->GetObjy()), this, NULL, 780, 9, 50))
						{
							pAni->SetActive();
							GameScene::sThis->InsertNewMagic(pAni);

							GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 6);
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
	else
	{
		__super::Update(_dt);
	}
}