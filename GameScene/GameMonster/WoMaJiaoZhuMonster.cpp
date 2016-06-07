#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
WoMaJiaoZhuMonster::WoMaJiaoZhuMonster()
{

}

void WoMaJiaoZhuMonster::Render()
{
	__super::Render();
}

void WoMaJiaoZhuMonster::TransparentRender()
{
	__super::TransparentRender();

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
		int nIndex = m_pRenderInfo->atkeftbegin + GetDirection() * m_pRenderInfo->atkefttotal + GetCurFrame();
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
}

/*void WoMaJiaoZhuMonster::OnPacket(const PacketHeader* _pPkt)
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

void WoMaJiaoZhuMonster::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_DEAD)
	{
		if(m_attrib.id == 94)
		{
			int nIndexBegin = m_pRenderInfo->deadbegin + 80;

			MagicMonsAni* pAni = new MagicMonsAni;
			if(pAni->Create(0, this, NULL, nIndexBegin, 10, 80))
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

	__super::DoPacket(not);
}*/
void WoMaJiaoZhuMonster::OnObjectAction(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_DEAD)
	{
		if(m_attrib.id == 94)
		{
			int nIndexBegin = m_pRenderInfo->deadbegin + 80;

			MagicMonsAni* pAni = new MagicMonsAni;
			if(pAni->Create(0, this, NULL, nIndexBegin, 10, 80))
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

	__super::OnObjectAction(not);
}