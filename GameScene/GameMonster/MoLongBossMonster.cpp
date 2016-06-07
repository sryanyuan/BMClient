#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
MoLongBossMonster::MoLongBossMonster()
{
	m_fLastUpdateSkill = 0.0f;
}

/*void MoLongBossMonster::OnPacket(const PacketHeader* _pPkt)
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

void MoLongBossMonster::DoPacket(const PkgObjectActionNot& not)
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
	else if(not.uAction == ACTION_DEAD)
	{
		MagicElement* pMagic = new MagicMonsAni;
		if(pMagic->Create(0, this, NULL, 3120 + 20 * GetDirection(), 20, 45))
		{
			pMagic->SetActive();
			GameScene::sThis->InsertNewMagic(pMagic);
		}
		else
		{
			SAFE_DELETE(pMagic);
		}
	}
}*/
void MoLongBossMonster::OnObjectAction(const PkgObjectActionNot& not)
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
	else if(not.uAction == ACTION_DEAD)
	{
		MagicElement* pMagic = new MagicMonsAni;
		if(pMagic->Create(0, this, NULL, 3120 + 20 * GetDirection(), 20, 45))
		{
			pMagic->SetActive();
			GameScene::sThis->InsertNewMagic(pMagic);
		}
		else
		{
			SAFE_DELETE(pMagic);
		}
	}

	__super::OnObjectAction(not);
}

void MoLongBossMonster::Update(float _dt)
{
	if(GetStatus() == PST_SK_LEIDIANSHU)
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
			if(m_fLastUpdateSkill > /*0.2f*/0.2f)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateSkill = 0.0f;
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}

void MoLongBossMonster::Render()
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
			int nTextIndex = 3280 + GetDirection() * 20 + m_bCurFrame;
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
					if(texSelected)
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

void MoLongBossMonster::TransparentRender()
{
	if(GetStatus() == PST_SK_LEIDIANSHU)
	{
		GameTextureManager* res = m_texs;
		int nTextIndex = 3520 + m_bCurFrame;
		HTEXTURE tex = res->GetTexture(nTextIndex);
		short sOffsetX = 0;
		short sOffsetY = 0;
		int nDrawOffsetX = -1;
		int nDrawOffsetY = -1;
		if(tex)
		{
			MagicElement::pEffectRender->SetTexture(tex);
			MagicElement::pEffectRender->SetTextureRect(0, 0, res->GetTextureWidth(nTextIndex), 
				res->GetTextureHeight(nTextIndex));
			sOffsetX = res->GetTextureOffsetX(nTextIndex);
			sOffsetY = res->GetTextureOffsetY(nTextIndex);
			//	根据玩家位置 得先算出来正常矩形的位置
			nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
			nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
			MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
				nDrawOffsetY + sOffsetY);
		}
	}

	__super::TransparentRender();

	if(GetStatus() == PST_ATTACKNOWEAPON)
	{
		GameTextureManager* res = m_texs;
		int nTextIndex = 3440 + 10 * GetDirection() + m_bCurFrame;
		HTEXTURE tex = res->GetTexture(nTextIndex);
		short sOffsetX = 0;
		short sOffsetY = 0;
		int nDrawOffsetX = -1;
		int nDrawOffsetY = -1;
		if(tex)
		{
			MagicElement::pEffectRender->SetTexture(tex);
			MagicElement::pEffectRender->SetTextureRect(0, 0, res->GetTextureWidth(nTextIndex), 
				res->GetTextureHeight(nTextIndex));
			sOffsetX = res->GetTextureOffsetX(nTextIndex);
			sOffsetY = res->GetTextureOffsetY(nTextIndex);
			//	根据玩家位置 得先算出来正常矩形的位置
			nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
			nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();
			MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
				nDrawOffsetY + sOffsetY);
		}
	}
}

bool MoLongBossMonster::IsLastFrame()
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
			if(GetCurFrame() == 19)
			{
				return true;
			}
		}break;
	}
	return false;
}