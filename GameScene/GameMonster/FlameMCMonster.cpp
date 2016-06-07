#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
FlameMCMonster::FlameMCMonster()
{
	m_nLastAtkTarget = 0;
	m_fLastUpdateSkill = 0.0f;
}

/*void FlameMCMonster::OnPacket(const PacketHeader* _pPkt)
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
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,			PkgObjectActionNot)
		HANDLE_DEFAULT(_pPkt)
		END_HANDLE_PACKET(_pPkt)
}

void FlameMCMonster::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_ATTACK)
	{
		if(not.uUserId != 0)
		{
			m_nLastAtkTarget = not.uUserId;
		}
	}
	else if(not.uAction == ACTION_SPELL)
	{
		m_fLastUpdateSkill = 0.0f;
		SetStatus(PST_SK_LEIDIANSHU);
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
		if(not.uUserId != 0)
		{
			m_nLastAtkTarget = not.uUserId;
		}

		int nDrt = HIWORD(not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
	}

	__super::DoPacket(not);
}*/
void FlameMCMonster::OnObjectAction(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_ATTACK)
	{
		if(not.uUserId != 0)
		{
			m_nLastAtkTarget = not.uUserId;
		}
	}
	else if(not.uAction == ACTION_SPELL)
	{
		m_fLastUpdateSkill = 0.0f;
		SetStatus(PST_SK_LEIDIANSHU);
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
		if(not.uUserId != 0)
		{
			m_nLastAtkTarget = not.uUserId;
		}

		int nDrt = HIWORD(not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
	}

	__super::OnObjectAction(not);
}

void FlameMCMonster::Update(float _dt)
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
						if(pAni->Create(MAKELONG(pObj->GetObjx(), pObj->GetObjy()), this, NULL, 4610, 6, 50))
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
	else if(GetStatus() == PST_SK_LEIDIANSHU)
	{
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

				if(m_nLastAtkTarget != 0)
				{
					PacketHandler* pHandler = SocketDataCenter::GetInstance().GetHandler(m_nLastAtkTarget);
					if(NULL != pHandler)
					{
						GameObject* pObj = static_cast<GameObject*>(pHandler);

						MagicElement* pAni = new MagicMonsPosAni;
						if(pAni->Create(MAKELONG(pObj->GetObjx(), pObj->GetObjy()), this, NULL, 4620, 6, 70))
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
	else
	{
		__super::Update(_dt);
	}
}

void FlameMCMonster::Render()
{
	if(GetStatus() == PST_SK_LEIDIANSHU)
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
			int nTextIndex = 4520 + GetDirection() * 10 + m_bCurFrame;
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

				if(!m_pRender)
				{
					//m_pSpr = new hgeSprite(tex, 0, 0, res->GetTextureWidth(tex), res->GetTextureHeight(tex));
					m_pRender = new hgeSprite(tex, 0, 0, res->GetTextureWidth(nTextIndex),
						res->GetTextureHeight(nTextIndex));
				}
				else
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, res->GetTextureWidth(nTextIndex), 
						res->GetTextureHeight(nTextIndex));
				}
				sOffsetX = res->GetTextureOffsetX(nTextIndex);
				sOffsetY = res->GetTextureOffsetY(nTextIndex);
				//	根据玩家位置 得先算出来正常矩形的位置
				nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
				nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
				m_pRender->Render(nDrawOffsetX + sOffsetX,
					nDrawOffsetY + sOffsetY);
				//	记录当前纹理
				m_nCurrentTextureIndex = nTextIndex;
			}
		}
	}
	else
	{
		__super::Render();
	}
}

bool FlameMCMonster::IsLastFrame()
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
			if(GetCurFrame() == 5)
			{
				return true;
			}
		}break;
	}
	return false;
}