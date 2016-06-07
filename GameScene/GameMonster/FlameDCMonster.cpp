#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
FlameDCMonster::FlameDCMonster()
{
	m_fLastUpdateSkill = 0.0f;
}

bool FlameDCMonster::IsLastFrame()
{
	if(GetStatus() == PST_SK_LEIDIANSHU)
	{
		return (GetCurFrame() == 6);
	}

	return __super::IsLastFrame();
}

/*void FlameDCMonster::OnPacket(const PacketHeader* _pPkt)
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
		HANDLE_DEFAULT(_pPkt)
		END_HANDLE_PACKET(_pPkt)
}

void FlameDCMonster::DoPacket(const PkgObjectActionNot& _not)
{
	if(_not.uAction == ACTION_SPELL)
	{
		m_fLastUpdateSkill = 0.0f;
		SetStatus(PST_SK_LEIDIANSHU);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));

		int nDrt = HIWORD(_not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
	}

	__super::DoPacket(_not);
}*/
void FlameDCMonster::OnObjectAction(const PkgObjectActionNot& _not)
{
	if(_not.uAction == ACTION_SPELL)
	{
		m_fLastUpdateSkill = 0.0f;
		SetStatus(PST_SK_LEIDIANSHU);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));

		int nDrt = HIWORD(_not.uParam1);
		if(nDrt >= 0 &&
			nDrt < 8)
		{
			SetDirection((PLAYER_DIRECTION)nDrt);
		}
	}

	__super::OnObjectAction(_not);
}

void FlameDCMonster::Update(float _dt)
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
			if(m_fLastUpdateSkill > /*0.2f*/0.1f)
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

void FlameDCMonster::Render()
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
			int nTextIndex = 4010 + GetDirection() * 10 + m_bCurFrame;
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

			if(GetCurFrame() >= 3 &&
				GetCurFrame() <= 5)
			{
				nTextIndex = 4090 + GetDirection() * 10 + GetCurFrame();
				tex = m_texs->GetTexture(nTextIndex);

				if(tex)
				{
					sOffsetX = res->GetTextureOffsetX(nTextIndex);
					sOffsetY = res->GetTextureOffsetY(nTextIndex);
					//	根据玩家位置 得先算出来正常矩形的位置
					nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)GamePlayer::GetInstance()->GetObjx();
					nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)GamePlayer::GetInstance()->GetObjy();

					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, m_texs->GetTextureWidth(nTextIndex), m_texs->GetTextureHeight(nTextIndex));
					m_pRender->Render(nDrawOffsetX + sOffsetX, nDrawOffsetY + sOffsetY);
				}
			}
		}
	}
	else
	{
		__super::Render();
	}
}