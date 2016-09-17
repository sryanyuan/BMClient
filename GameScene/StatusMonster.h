#ifndef _INC_STATUSMONSTER_
#define _INC_STATUSMONSTER_
//////////////////////////////////////////////////////////////////////////
#include "GameMonster.h"
//////////////////////////////////////////////////////////////////////////
struct MonsterStatusItem
{
	int nStatusId;
	//	frame indexes
	int nFrameStart;
	int nFrameFrames;
	int nFrameTotal;
	//	alpha frame indexes
	int nAlphaStartOffset;
	int nAlphaStart;
	int nAlphaFrames;
	int nAlphaTotal;
	//	time
	DWORD dwUpdateInterval;
	DWORD dwLastUpdateTime;
	int nCurrentFrame;

	MonsterStatusItem()
	{
		memset(this, 0, sizeof(MonsterStatusItem));
	}

	bool IsLastFrame()
	{
		return (nCurrentFrame - 1) == nFrameFrames;
	}
};

typedef std::map<int, MonsterStatusItem*> MonsterStatusItemMap;

class StatusMonster : public GameMonster
{
public:
	StatusMonster();
	virtual ~StatusMonster();

public:
	virtual void Update(float _dt);
	virtual void Render();

	//	add
	virtual MonsterStatusItem* InitStatusItem(int _nStatusId);

protected:
	MonsterStatusItemMap m_xStatusItemMap;
};
//////////////////////////////////////////////////////////////////////////
#endif