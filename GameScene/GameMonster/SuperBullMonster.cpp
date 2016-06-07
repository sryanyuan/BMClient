#include "../GameMonster.h"
#include "../../GameScene/GameScene.h"
//////////////////////////////////////////////////////////////////////////
SuperBullMonster::SuperBullMonster()
{
	m_fLastUpdateSpeAtk1 = 0;
}

bool SuperBullMonster::IsLastFrame()
{
	if(GetStatus() == PST_EXT1)
	{
		return GetCurFrame() == 5;
	}

	return __super::IsLastFrame();
}

void SuperBullMonster::OnObjectAction(const PkgObjectActionNot &not)
{
	if(not.uAction == ACTION_EXT1)
	{
		SetStatus(PST_EXT1);
		m_fLastUpdateSpeAtk1 = 0;
		SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));

		//	放一个魔法效果
		MagicElement* pAni = new MagicMonsPosAni;
		if(pAni->Create(MAKELONG(GetObjx(), GetObjy()), this, NULL, 3580, 20, 60))
		{
			pAni->SetActive();
			GameScene::sThis->InsertNewMagic(pAni);
			//GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 7);
			GameSoundManager::GetInstancePtr()->PlayObjectSoundDirect(GetSoundIndex(), 6);
		}
		else
		{
			SAFE_DELETE(pAni);
		}
	}
	else
	{
		__super::OnObjectAction(not);
	}

	__super::OnObjectAction(not);
}

void SuperBullMonster::Update(float _dt)
{
	float fAtkInterval = GetAttackInterval();

	if(GetStatus() == PST_EXT1)
	{
		if(GetStatus() == PST_EXT1 &&
			!IsLastFrame())
		{
			m_fLastUpdateSpeAtk1 += _dt;
			if(m_fLastUpdateSpeAtk1 > /*0.1f*/fAtkInterval)
			{
				SetCurFrame(GetCurFrame() + 1);
				m_fLastUpdateSpeAtk1 = 0.0f;
			}
		}
		else if(GetStatus() == PST_EXT1 &&
			IsLastFrame())
		{
			m_fLastUpdateSpeAtk1 += _dt;
			if(m_fLastUpdateSpeAtk1 > /*0.2f*/fAtkInterval)
			{
				SetStatus(PST_STAND);
				m_fLastUpdateSpeAtk1 = 0.0f;
			}
		}
	}
	else
	{
		__super::Update(_dt);
	}
}

void SuperBullMonster::Render()
{
	if(GetStatus() == PST_EXT1)
	{
		int nTexIndex = 3400 + GetDirection() * 10 + GetCurFrame();
		HTEXTURE tex = m_texs->GetTexture(nTexIndex);

		if(tex)
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

			m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0,
				m_texs->GetTextureWidth(nTexIndex),
				m_texs->GetTextureHeight(nTexIndex));

			int nDrawOffsetX = GetScreenPosX();
			int nDrawOffsetY = GetScreenPosY();

			m_pRender->Render(nDrawOffsetX + sOftX, nDrawOffsetY + sOftY);
		}
	}
	else
	{
		__super::Render();
	}

	if(GetRenderMode() == ORM_TRANSPARENT)
	{
		if(GetStatus() == PST_ATTACKNOWEAPON)
		{
			int nTexIndex = 3490 + GetDirection() * 10 + GetCurFrame();
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
		}
	}
}