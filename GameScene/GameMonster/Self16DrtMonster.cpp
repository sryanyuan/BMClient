#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
#include "../../GameScene/GameResourceManager.h"
//////////////////////////////////////////////////////////////////////////
Self16DrtMonster::Self16DrtMonster()
{
	m_nDrt16 = -1;
}

void Self16DrtMonster::Update(float _dt)
{
	__super::Update(_dt);
}

void Self16DrtMonster::TransparentRender()
{
	if(!IsVisible())
	{
		return;
	}
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nTexWidth = 0;
	int nTexHeight = 0;
	HTEXTURE tex = 0;
	if(GetStatus() == PST_ATTACKNOWEAPON ||
		GetStatus() == PST_ATTACKWEAPON)
	{
		int nDrawDrt16 = m_nDrt16;
		if(nDrawDrt16 < 0 ||
			nDrawDrt16 >= 16)
		{
			nDrawDrt16 = GetDirection() * 2;
		}

		int nIndex = 0;
		if(m_attrib.id == 45)
		{
			nIndex = m_pRenderInfo->atkeftbegin + m_pRenderInfo->atkefttotal * nDrawDrt16 + GetCurFrame();
			tex = m_texs->GetTexture(nIndex);
			if(tex)
			{
				MagicElement::pEffectRender->SetTexture(tex);
				nTexWidth = m_texs->GetTextureWidth(nIndex);
				nTexHeight = m_texs->GetTextureHeight(nIndex);
				MagicElement::pEffectRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);

				short sOffsetX = 0;
				short sOffsetY = 0;
				int nDrawOffsetX = -1;
				int nDrawOffsetY = -1;

				sOffsetX = m_texs->GetTextureOffsetX(nIndex);
				sOffsetY = m_texs->GetTextureOffsetY(nIndex);
				//	根据玩家位置 得先算出来正常矩形的位置
				nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
				nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
				MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
					nDrawOffsetY + sOffsetY);
			}
		}
		else
		{
			const RenderInfo* pThunderInfo = NULL;
			if(GameInfoManager::GetInstance()->GetMonsterRenderInfo(45, &pThunderInfo))
			{
				nIndex = pThunderInfo->atkeftbegin + pThunderInfo->atkefttotal * nDrawDrt16 + GetCurFrame();
				GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_MON1 + pThunderInfo->wil);
				if(pTexMgr)
				{
					tex = pTexMgr->GetTexture(nIndex);

					if(tex)
					{
						MagicElement::pEffectRender->SetTexture(tex);
						nTexWidth = pTexMgr->GetTextureWidth(nIndex);
						nTexHeight = pTexMgr->GetTextureHeight(nIndex);
						MagicElement::pEffectRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);

						short sOffsetX = 0;
						short sOffsetY = 0;
						int nDrawOffsetX = -1;
						int nDrawOffsetY = -1;

						sOffsetX = pTexMgr->GetTextureOffsetX(nIndex);
						sOffsetY = pTexMgr->GetTextureOffsetY(nIndex);
						//	根据玩家位置 得先算出来正常矩形的位置
						nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
						nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
						MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
							nDrawOffsetY + sOffsetY);
					}
				}
			}
		}
	}

	__super::TransparentRender();
}

/*void Self16DrtMonster::OnPacket(const PacketHeader* _pPkt)
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

void Self16DrtMonster::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_ATTACK)
	{
		m_nDrt16 = -1;
		if(not.uUserId == 0)
		{
			m_nDrt16 = -1;
		}
		else
		{
			PacketHandler* pHandler = SocketDataCenter::GetInstancePtr()->GetHandler(not.uUserId);
			if(pHandler != NULL)
			{
				GameObject* pObj = static_cast<GameObject*>(pHandler);
				m_nDrt16 = GetDirection16(GetCoordX(), GetCoordY(), pObj->GetCoordX(), pObj->GetCoordY());
			}
		}
	}

	__super::DoPacket(not);
}*/
void Self16DrtMonster::OnObjectAction(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_ATTACK)
	{
		m_nDrt16 = -1;
		if(not.uUserId == 0)
		{
			m_nDrt16 = -1;
		}
		else
		{
			PacketHandler* pHandler = SocketDataCenter::GetInstancePtr()->GetHandler(not.uUserId);
			if(pHandler != NULL)
			{
				GameObject* pObj = static_cast<GameObject*>(pHandler);
				m_nDrt16 = GetDirection16(GetCoordX(), GetCoordY(), pObj->GetCoordX(), pObj->GetCoordY());
			}
		}
	}

	__super::OnObjectAction(not);
}