#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
void ChiYueEMoMonster::OnPacket(const PacketHeader* _pPkt)
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
		HANDLE_DEFAULT(_pPkt)
		END_HANDLE_PACKET(_pPkt)
}

void ChiYueEMoMonster::DoPacket(const PkgPlayerPlayAniAck& ack)
{
	MagicGroundThron* pNewMagic = NULL;

	if(!ack.xPos.empty())
	{
		std::list<DWORD>::const_iterator begiter = ack.xPos.begin();
		for(begiter;
			begiter != ack.xPos.end();
			++begiter)
		{
			pNewMagic = new MagicGroundThron;
			if(pNewMagic->Create(m_attrib.id, this, NULL, LOWORD(*begiter) * UNIT_WIDTH, HIWORD(*begiter) * UNIT_HEIGHT, 0))
			{
				pNewMagic->SetActive();
				GameScene::sThis->InsertNewMagic(pNewMagic);
			}
			else
			{
				SAFE_DELETE(pNewMagic);
			}
		}
	}
}

int ChiYueEMoMonster::CalMonIndex()
{
	int nIndex = 0;

	//if(m_renderInfo.drts == 4)
	{
		switch(GetStatus())
		{
		case PST_STAND:
			{
				nIndex = m_pRenderInfo->standbegin + 0 *m_pRenderInfo->standtotal;
			}break;
		case PST_WALK:
			{
				nIndex = m_pRenderInfo->walkbegin + 0 * m_pRenderInfo->walktotal;
			}break;
		case PST_ATTACKNOWEAPON:
			{
				nIndex = m_pRenderInfo->atkbegin + 0 * m_pRenderInfo->atktotal;
			}break;
		case PST_ATTACKED:
			{
				nIndex = m_pRenderInfo->hurtbegin + 0 * m_pRenderInfo->hurttotal;
			}break;
		case PST_DEAD:
			{
				nIndex = m_pRenderInfo->deadbegin + 0 * m_pRenderInfo->deadtotal;
			}break;
		}
	}

	return nIndex;
}