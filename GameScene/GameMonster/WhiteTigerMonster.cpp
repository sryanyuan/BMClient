#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
void WhiteTigerMonster::Render()
{
	__super::Render();

	/*GamePlayer* pPlayer = GamePlayer::GetInstance();

	if(GetStatus() == PST_DEAD)
	{
		int nTexIndex = CalMonIndex() + 190 + GetCurFrame();
		HTEXTURE tex = m_texs->GetTexture(nTexIndex);

		if(tex)
		{
			short sOftX = m_texs->GetTextureOffsetX(nTexIndex);
			short sOftY = m_texs->GetTextureOffsetY(nTexIndex);

			MagicElement::pEffectRender->SetTexture(tex);
			MagicElement::pEffectRender->SetTextureRect(0, 0,
				m_texs->GetTextureWidth(nTexIndex),
				m_texs->GetTextureHeight(nTexIndex));

			int nDrawOffsetX = GetScreenPosX();
			int nDrawOffsetY = GetScreenPosY();

			MagicElement::pEffectRender->Render(nDrawOffsetX + sOftX, nDrawOffsetY + sOftY);
		}
	}*/
}