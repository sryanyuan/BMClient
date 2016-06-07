#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//	Moon spirte
void MoonSpriteMonster::Render()
{
	//	渲染背后的效果
	if(GetRenderMode() == ORM_NORMAL)
	{
		int nCurrentFrame = m_nCurrentTextureIndex;

		if(0 != nCurrentFrame)
		{
			int nTexIndex = nCurrentFrame - 360;
			HTEXTURE tex = m_texs->GetTexture(nTexIndex);
			if(0 != tex)
			{
				if(GameScene::sThis->GetTarget() == this)
				{
					HTEXTURE texSelected = SelectedTextureManager::GetInstance()->GetSelectedTextureMonster(tex, GetAttrib()->id, nTexIndex);
					if(texSelected != NULL)
					{
						tex = texSelected;
					}
				}

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
		}
	}

	//	正常渲染
	__super::Render();
}