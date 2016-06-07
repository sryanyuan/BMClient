#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
void MoLongStone::TransparentRender()
{
	__super::TransparentRender();

	if(GetStatus() == PST_STAND)
	{
		int nTexIndex = m_pRenderInfo->atkeftbegin + m_bCurFrame;
		HTEXTURE tex = m_texs->GetTexture(nTexIndex);
		SHORT sOftX = m_texs->GetTextureOffsetX(nTexIndex);
		SHORT sOftY = m_texs->GetTextureOffsetY(nTexIndex);
		SHORT sWidth = m_texs->GetTextureWidth(nTexIndex);
		SHORT sHeight = m_texs->GetTextureHeight(nTexIndex);

		int nRenderX = 0;
		int nRenderY = 0;
		GamePlayer* pPlayer = GamePlayer::GetInstance();
		nRenderX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
		nRenderY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

		if(tex)
		{
			MagicElement::pEffectRender->SetTexture(tex);
			MagicElement::pEffectRender->SetTextureRect(0, 0, sWidth, sHeight);
			MagicElement::pEffectRender->Render(nRenderX + sOftX, nRenderY + sOftY);
		}
	}
}
void MoLongStone::OnObjectAction(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_ATTACK)
	{
		MagicElement* pMagic = new MagicMonsAni;
		if(pMagic->Create(0, this, NULL, m_pRenderInfo->standbegin + 60, 26, 30))
		{
			pMagic->SetActive();
			GameScene::sThis->InsertNewMagic(pMagic);
		}
		else
		{
			SAFE_DELETE(pMagic);
		}
	}
	else if(not.uAction == ACTION_DEAD)
	{
		MagicElement* pMagic = new MagicMonsAni;
		if(pMagic->Create(0, this, NULL, m_pRenderInfo->standbegin + 90, 20, 40))
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

/*void MoLongStone::OnPacket(const PacketHeader* _pPkt)
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
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,		PkgObjectActionNot)
		END_HANDLE_PACKET(_pPkt)

		__super::OnPacket(_pPkt);
}

void MoLongStone::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_ATTACK)
	{
		MagicElement* pMagic = new MagicMonsAni;
		if(pMagic->Create(0, this, NULL, m_pRenderInfo->standbegin + 60, 26, 30))
		{
			pMagic->SetActive();
			GameScene::sThis->InsertNewMagic(pMagic);
		}
		else
		{
			SAFE_DELETE(pMagic);
		}
	}
	else if(not.uAction == ACTION_DEAD)
	{
		MagicElement* pMagic = new MagicMonsAni;
		if(pMagic->Create(0, this, NULL, m_pRenderInfo->standbegin + 90, 20, 40))
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