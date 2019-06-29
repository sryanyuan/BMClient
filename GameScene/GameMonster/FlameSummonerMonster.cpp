#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
FlameSummonerMonster::FlameSummonerMonster()
{
	m_fLastUpdateExt1 = m_fLastUpdateExt2 = 0.0f;
	m_nLastAtkTarget = 0;

	m_pRender = NEW_EMPTY_SPRITE;
}

FlameSummonerMonster::~FlameSummonerMonster()
{
	SAFE_DELETE(m_pRender);
}

void FlameSummonerMonster::Render()
{
	int nTextIndex = 0;

	__super::Render();

	//	Render EXT1 EXT2 EXT3
	HTEXTURE tex = 0;
	short sOftX = 0;
	short sOftY = 0;
	int nTexWidth = 0;
	int nTexHeight = 0;
	int nRenderX = 0;
	int nRenderY = 0;

	if(GetStatus() == PST_EXT1 ||
		GetStatus() == PST_EXT2 ||
		GetStatus() == PST_EXT3)
	{
		if(GetStatus() == PST_EXT1)
		{
			nTextIndex = 6240 + 10 * GetDirection() + GetCurFrame();
		}
		else if(GetStatus() == PST_EXT2)
		{
			nTextIndex = 6400 + 10 * GetDirection() + GetCurFrame();
		}

		tex = m_texs->GetTexture(nTextIndex);

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

			m_pRender->SetTexture(tex);
			nTexWidth = m_texs->GetTextureWidth(nTextIndex);
			nTexHeight = m_texs->GetTextureHeight(nTextIndex);
			sOftX = m_texs->GetTextureOffsetX(nTextIndex);
			sOftY = m_texs->GetTextureOffsetY(nTextIndex);
			m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);

			m_nCurrentTextureIndex = nTextIndex;

			nRenderX = GetObjx() - GamePlayer::GetInstance()->GetObjx() + CENTRAL_X + sOftX;
			nRenderY = GetObjy() - GamePlayer::GetInstance()->GetObjy() + CENTRAL_Y + sOftY;

			m_pRender->Render(nRenderX, nRenderY);
		}
	}
	else
	{
		nTextIndex = CalMonIndex();
	}

	//	Render alpha blend layer
	int nAlphaIndex = nTextIndex + 580 + GetCurFrame();
	tex = m_texs->GetTexture(nAlphaIndex);

	if(tex)
	{
		//m_pRender->SetTexture(tex);
		nTexWidth = m_texs->GetTextureWidth(nAlphaIndex);
		nTexHeight = m_texs->GetTextureHeight(nAlphaIndex);
		sOftX = m_texs->GetTextureOffsetX(nAlphaIndex);
		sOftY = m_texs->GetTextureOffsetY(nAlphaIndex);
		//m_pRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);

		nRenderX = GetObjx() - GamePlayer::GetInstance()->GetObjx() + CENTRAL_X + sOftX;
		nRenderY = GetObjy() - GamePlayer::GetInstance()->GetObjy() + CENTRAL_Y + sOftY;

		MagicElement::pEffectRender->SetTexture(tex);
		MagicElement::pEffectRender->SetTextureRect(0, 0, nTexWidth, nTexHeight);
		MagicElement::pEffectRender->Render(nRenderX, nRenderY);
	}
}

void FlameSummonerMonster::Update(float _dt)
{
	if(GetStatus() == PST_EXT1)
	{
		if(GetStatus() == PST_EXT1 &&
			!IsLastFrame())
		{
			m_fLastUpdateExt1 += _dt;
			if(m_fLastUpdateExt1 > /*0.1f*/0.1f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateExt1 = 0.0f;
			}
		}
		else if(GetStatus() == PST_EXT1 &&
			IsLastFrame())
		{
			m_fLastUpdateExt1 += _dt;
			if(m_fLastUpdateExt1 > /*0.2f*/0.1f)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateExt1 = 0.0f;

				if(m_nLastAtkTarget != 0)
				{
					PacketHandler* pHandler = SocketDataCenter::GetInstance().GetHandler(m_nLastAtkTarget);
					if(NULL != pHandler)
					{
						GameObject* pObj = static_cast<GameObject*>(pHandler);

						MagicElement* pAni = new MagicMonsPosAni;
						if(pAni->Create(MAKELONG(pObj->GetObjx(), pObj->GetObjy()), this, NULL, 7060, 17, 70))
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
	else if(GetStatus() == PST_EXT2)
	{
		if(GetStatus() == PST_EXT2 &&
			!IsLastFrame())
		{
			m_fLastUpdateExt2 += _dt;
			if(m_fLastUpdateExt2 > /*0.1f*/0.1f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateExt2 = 0.0f;
			}
		}
		else if(GetStatus() == PST_EXT2 &&
			IsLastFrame())
		{
			m_fLastUpdateExt2 += _dt;
			if(m_fLastUpdateExt2 > /*0.2f*/0.1f)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateExt2 = 0.0f;
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}

void FlameSummonerMonster::OnPacket(const PacketHeader* _pPkt)
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
		//HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,		PkgObjectActionNot)
		HANDLE_DEFAULT(_pPkt)
		END_HANDLE_PACKET(_pPkt)
}


/*void FlameSummonerMonster::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_EXT1)
	{
		if(not.uUserId != 0)
		{
			m_nLastAtkTarget = not.uUserId;
		}
	}

	__super::DoPacket(not);
}*/

void FlameSummonerMonster::OnObjectAction(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_EXT1)
	{
		if(not.uUserId != 0)
		{
			m_nLastAtkTarget = not.uUserId;
		}
	}

	__super::OnObjectAction(not);
}

void FlameSummonerMonster::DoPacket(const PkgPlayerPlayAniAck& ack)
{
	MagicMonsPosAni* pMgc = NULL;
	DWORD dwPos = 0;

	if(!ack.xPos.empty())
	{
		std::list<unsigned int>::const_iterator begiter = ack.xPos.begin();
		for(begiter;
			begiter != ack.xPos.end();
			++begiter)
		{
			pMgc = new MagicMonsPosAni;
			dwPos = *begiter;
			DWORD dwX = LOWORD(dwPos) * UNIT_WIDTH;
			DWORD dwY = HIWORD(dwPos) * UNIT_HEIGHT;

			if(pMgc->Create(MAKELONG(dwX, dwY), this, NULL, 7080, 30, 70))
			{
				pMgc->SetActive();
				GameScene::sThis->InsertNewMagic(pMgc);
			}
			else
			{
				SAFE_DELETE(pMgc);
			}
		}
	}
}

bool FlameSummonerMonster::IsLastFrame()
{
	if(GetStatus() == PST_EXT1)
	{
		return (GetCurFrame() == 6);
	}
	else if(GetStatus() == PST_EXT2)
	{
		return (GetCurFrame() == 6);
	}

	return __super::IsLastFrame();
}