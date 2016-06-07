#include "../Common/PlayerStatusControl.h"
#include "../GameScene/GamePlayer.h"

PlayerStatusControl::PlayerStatusControl()
{
	//m_pPlayer = GamePlayer::GetInstance();
	m_pPlayer = NULL;
	/*
	 *	Note 这里引起死循环了
	 */
}

PlayerStatusControl::~PlayerStatusControl()
{

}


void PlayerStatusControl::AddStatusItem(BYTE _type, int _total, int _step)
{
	if(_total <= 0)
	{
		return;
	}

	StatusItem item;
	item.type = _type;
	item.total = _total;
	item.step = _step;
	item.btime = ::GetTickCount();

	m_list.push_back(item);
}

void PlayerStatusControl::Update()
{
	if(!m_pPlayer)
	{
		return;
	}

	if(m_list.empty())
	{
		return;
	}

	DWORD dwCurTime = ::GetTickCount();

	for(STATUSLIST::iterator iter = m_list.begin();
		iter != m_list.end();
		)
	{
		if(dwCurTime - iter->btime > UPDATE_INTERVAL)
		{
			//	可以更新了
			if(iter->type == STATUS_HP)
			{
				if(iter->total > iter->step)
				{
					//	剩余值够用
					iter->total -= iter->step;
					if(m_pPlayer->AddHP(iter->step))
					{
						iter->btime = dwCurTime;
						++iter;
						//	加了血后 不会叠加加
						return;
					}
					else
					{
						iter = m_list.erase(iter);
					}
				}
				else
				{
					//	剩余不够用了
					m_pPlayer->AddHP(iter->total);
					iter = m_list.erase(iter);
					//	加了血后 不会叠加加
					return;
				}
			}
			else if(iter->type == STATUS_MP)
			{
				if(iter->total > iter->step)
				{
					//	剩余值够用
					iter->total -= iter->step;
					if(m_pPlayer->AddMP(iter->step))
					{
						iter->btime = dwCurTime;
						++iter;
						//	加了血后 不会叠加加
						return;
					}
					else
					{
						iter = m_list.erase(iter);
					}
				}
				else
				{
					//	剩余不够用了
					m_pPlayer->AddMP(iter->total);
					iter = m_list.erase(iter);
					//	加了血后 不会叠加加
					return;
				}
			}
			else
			{
				++iter;
			}
		}
		else
		{
			//	更新时间未到
			++iter;
		}
		
	}
}