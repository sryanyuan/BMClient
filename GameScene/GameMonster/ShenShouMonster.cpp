#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
ShenShouMonster::ShenShouMonster()
{
	m_bShowEff = false;
}

ShenShouMonster::~ShenShouMonster()
{

}

void ShenShouMonster::Update(float _dt)
{
	if(GetStatus() == PST_ATTACKNOWEAPON)
	{
		//	¹¥»÷update
		float fAtkInterval = GetAttackInterval();
		if(!IsLastFrame() &&
			GetStatus() == PST_ATTACKNOWEAPON)
		{
			m_fLastUpdateNoWeapon += _dt;
			if(m_fLastUpdateNoWeapon > /*0.08f*/fAtkInterval)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateNoWeapon = 0.0f;
				/*if(GetCurFrame() == 2 &&
					!m_bShowEff)
				{
					MagicStaticAttackEffect* pEff = new MagicStaticAttackEffect;
					if(pEff->Create(m_attrib.id, NULL, NULL, GetObjx(), GetObjy(), GetDirection()))
					{
						GameScene::sThis->InsertNewMagic(pEff);
					}
					else
					{
						SAFE_DELETE(pEff);
					}
					m_bShowEff = true;
				}*/
			}
		}
		else if(IsLastFrame() &&
			GetStatus() == PST_ATTACKNOWEAPON)
		{
			m_fLastUpdateNoWeapon += _dt;
			if(m_fLastUpdateNoWeapon > /*0.08f*/fAtkInterval)
			{
				m_fLastUpdateNoWeapon = 0.0f;
				SetStatus(PST_STAND);

				MagicStaticAttackEffect* pEff = new MagicStaticAttackEffect;
				if(pEff->Create(m_attrib.id, NULL, NULL, GetObjx(), GetObjy(), GetDirection()))
				{
					pEff->SetActive();
					GameScene::sThis->InsertNewMagic(pEff);
				}
				else
				{
					SAFE_DELETE(pEff);
				}
				//m_bShowEff = false;
#ifdef _NET_GAME_
#else
				AfxGetPlayer()->SetHurt();
#endif
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
	if(GetStatus() == PST_DEAD &&
		IsLastFrame())
	{
		SetVisible(false);
	}
}