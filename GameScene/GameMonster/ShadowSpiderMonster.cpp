#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
int ShadowSpiderMonster::CalMonIndex()
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