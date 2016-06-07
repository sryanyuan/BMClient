#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////

KulouMonster::KulouMonster()
{
	//
	m_fLastUpdateShow = 0;
	SetStatus(PST_SHOW);
}

KulouMonster::~KulouMonster()
{

}

void KulouMonster::Render()
{
	HGE* hge = AfxGetHge();
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	if(!IsVisible())
	{
		return;
	}

	if(GetStatus() == PST_SHOW)
	{
		if(m_texs)
		{
			GameTextureManager* res = m_texs;
			//	画怪物
			HTEXTURE tex = res->GetTexture(CalMonIndex() + m_bCurFrame);
			short sOffsetX = 0;
			short sOffsetY = 0;
			int nDrawOffsetX = -1;
			int nDrawOffsetY = -1;
			if(tex)
			{
				/*if(!m_pRender)
				{
					//m_pSpr = new hgeSprite(tex, 0, 0, res->GetTextureWidth(tex), res->GetTextureHeight(tex));
					m_pRender = new hgeSprite(tex, 0, 0, res->GetTextureWidth(CalMonIndex() + m_bCurFrame),
						res->GetTextureHeight(CalMonIndex() + m_bCurFrame));
				}
				else*/
				{
					/*m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, res->GetTextureWidth(CalMonIndex() + m_bCurFrame), 
						res->GetTextureHeight(CalMonIndex() + m_bCurFrame));*/
					MagicElement::pEffectRender->SetTexture(tex);
					MagicElement::pEffectRender->SetTextureRect(0, 0, res->GetTextureWidth(CalMonIndex() + m_bCurFrame), 
						res->GetTextureHeight(CalMonIndex() + m_bCurFrame));
				}
				sOffsetX = res->GetTextureOffsetX(CalMonIndex() + m_bCurFrame);
				sOffsetY = res->GetTextureOffsetY(CalMonIndex() + m_bCurFrame);
				//	根据玩家位置 得先算出来正常矩形的位置
				nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
				nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
				MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
					nDrawOffsetY + sOffsetY);
				//	记录当前纹理
				m_nCurrentTextureIndex = CalMonIndex() + m_bCurFrame;
			}
		}
	}
	else
	{
		__super::Render();
	}
}

void KulouMonster::Update(float _dt)
{
	if(GetStatus() == PST_SHOW)
	{
		//	Show update
		if(!IsLastFrame() &&
			GetStatus() == PST_SHOW)
		{
			m_fLastUpdateShow += _dt;
			if(m_fLastUpdateShow > 0.1f)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateShow = 0.0f;
			}
		}
		else if(IsLastFrame() &&
			GetStatus() == PST_SHOW)
		{
			m_fLastUpdateShow += _dt;
			if(m_fLastUpdateShow > 0.1f)
			{
				m_fLastUpdateShow = 0.0f;
				SetStatus(PST_STAND);
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}

int KulouMonster::CalMonIndex()
{
	if(GetStatus() == PST_SHOW)
	{
		return m_pRenderInfo->showbegin;
	}
	else
	{
		return __super::CalMonIndex();
	}
}

bool KulouMonster::IsLastFrame()
{
	if(GetStatus() == PST_SHOW)
	{
		return m_bCurFrame == (m_pRenderInfo->showframe - 1);
	}
	else
	{
		return __super::IsLastFrame();
	}
}