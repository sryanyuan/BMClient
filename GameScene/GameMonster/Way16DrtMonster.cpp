#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
Way16DrtMonster::Way16DrtMonster()
{
	m_nTargetID = -1;
}

void Way16DrtMonster::Update(float _dt)
{
	if(GetStatus() == PST_ATTACKNOWEAPON ||
		GetStatus() == PST_ATTACKWEAPON)
	{
		//	π•ª˜update
		float fAtkInterval = GetAttackInterval();
		if(!IsLastFrame() &&
			GetStatus() == PST_ATTACKNOWEAPON)
		{
			m_fLastUpdateNoWeapon += _dt;
			if(m_fLastUpdateNoWeapon > /*0.08f*/fAtkInterval)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateNoWeapon = 0.0f;

				if(GetAttrib()->id == 144 &&
					GetCurFrame() == 4)
				{
					PlayBulletAni();
				}
			}
		}
		else if(IsLastFrame() &&
			GetStatus() == PST_ATTACKNOWEAPON)
		{
			m_fLastUpdateNoWeapon += _dt;
			if(m_fLastUpdateNoWeapon > /*0.08f*/fAtkInterval)
			{
				m_fLastUpdateNoWeapon = 0.0f;
				SetStatus(PST_STAND);

				if(GetAttrib()->id != 144)
				{
					PlayBulletAni();
				}

#ifdef _NET_GAME_
#else
				AfxGetPlayer()->SetHurt();
#endif
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}

void Way16DrtMonster::PlayBulletAni()
{
	if(m_nTargetID != -1)
	{
		PacketHandler* pHandle = SocketDataCenter::GetInstancePtr()->GetHandler(m_nTargetID);
		if(pHandle)
		{
			GameObject* pObj = static_cast<GameObject*>(pHandle);
			Magic16DrtWay* pNewMagic = new Magic16DrtWay;
			if(false == pNewMagic->Create(m_attrib.id, this, pObj, 0, 0, 0))
			{
				delete pNewMagic;
				pNewMagic = NULL;
			}
			else
			{
				pNewMagic->SetActive();
				GameScene::sThis->InsertNewMagic(pNewMagic);
			}
		}
	}
}

/*void Way16DrtMonster::OnPacket(const PacketHeader* _pPkt)
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
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,			PkgObjectActionNot)
		HANDLE_DEFAULT(_pPkt)
		END_HANDLE_PACKET(_pPkt)
}

void Way16DrtMonster::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_ATTACK)
	{
		m_nTargetID = -1;
		if(not.uUserId == 0)
		{
			m_nTargetID = -1;
		}
		else
		{
			m_nTargetID = not.uUserId;
		}
	}

	__super::DoPacket(not);
}*/
void Way16DrtMonster::OnObjectAction(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_ATTACK)
	{
		m_nTargetID = -1;
		if(not.uUserId == 0)
		{
			m_nTargetID = -1;
		}
		else
		{
			m_nTargetID = not.uUserId;
		}
	}

	__super::OnObjectAction(not);
}