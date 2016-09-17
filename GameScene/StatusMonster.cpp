#include "StatusMonster.h"
//////////////////////////////////////////////////////////////////////////
StatusMonster::StatusMonster()
{

}

StatusMonster::~StatusMonster()
{
	//	free status item
	MonsterStatusItemMap::iterator it = m_xStatusItemMap.begin();

	for (it; it != m_xStatusItemMap.end(); ++it)
	{
		MonsterStatusItem* pItem = it->second;

		if (NULL != pItem)
		{
			delete pItem;
		}
	}

	m_xStatusItemMap.clear();
}

MonsterStatusItem* StatusMonster::InitStatusItem(int _nStatusId)
{
	//	get render info
	const RenderInfo* pRenderInfo = GetRenderInfo();

	if (NULL == pRenderInfo)
	{
		return NULL;
	}

	//	find the status
	MonsterStatusItem* pStatusItem = new MonsterStatusItem;
	pStatusItem->nStatusId = _nStatusId;

	switch (_nStatusId)
	{
	case PST_STAND:
		{
			pStatusItem->nFrameStart = pRenderInfo->standbegin;
			pStatusItem->nFrameFrames = pRenderInfo->standframe;
			pStatusItem->nFrameTotal = pRenderInfo->standtotal;
		}break;
	case PST_WALK:
		{
			pStatusItem->nFrameStart = pRenderInfo->walkbegin;
			pStatusItem->nFrameFrames = pRenderInfo->walkframe;
			pStatusItem->nFrameTotal = pRenderInfo->walktotal;
		}break;
	case PST_ATTACKNOWEAPON:
		{
			pStatusItem->nFrameStart = pRenderInfo->atkbegin;
			pStatusItem->nFrameFrames = pRenderInfo->atkframe;
			pStatusItem->nFrameTotal = pRenderInfo->atktotal;
		}break;
	case PST_ATTACKED:
		{
			pStatusItem->nFrameStart = pRenderInfo->hurtbegin;
			pStatusItem->nFrameFrames = pRenderInfo->hurtframe;
			pStatusItem->nFrameTotal = pRenderInfo->hurttotal;
		}break;
	case PST_DEAD:
		{
			pStatusItem->nFrameStart = pRenderInfo->deadbegin;
			pStatusItem->nFrameFrames = pRenderInfo->deadframe;
			pStatusItem->nFrameTotal = pRenderInfo->deadtotal;
		}break;
	}

	return pStatusItem;
}

void StatusMonster::Update(float _dt)
{
	//	get the status item
	MonsterStatusItem* pStatusItem;
	MonsterStatusItemMap::iterator it = m_xStatusItemMap.find(int(GetStatus()));

	if (it == m_xStatusItemMap.end())
	{
		//	not found, initialize
		pStatusItem = InitStatusItem(int(GetStatus()));
		m_xStatusItemMap.insert(std::make_pair(int(GetStatus()), pStatusItem));

		if (NULL == pStatusItem)
		{
			AfxGetHge()->System_Log("[ERR] Failed to load monster status [%d]", GetStatus());
		}
	}
	else
	{
		pStatusItem = it->second;
	}

	if (NULL == pStatusItem)
	{
		return;
	}

	//	do update process
	DWORD dwTick = GetTickCount();
	if (dwTick - pStatusItem->dwLastUpdateTime > pStatusItem->dwUpdateInterval)
	{
		//	next frame
		int nNextFrame = pStatusItem->nCurrentFrame + 1;
	}
}

void StatusMonster::Render()
{

}