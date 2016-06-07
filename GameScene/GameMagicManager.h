#ifndef GAMEMAGICMANAGER_H_
#define GAMEMAGICMANAGER_H_
#include "../Common/GameRenderObject.h"
#include <hgesprite.h>
#include <list>
#include <map>
#define MAGIC_SELF 0
#define MAGIC_WAY  1
#define MAGIC_DEST 2


struct MagicInfo
{
	GameObject* sender;
	GameObject* receiver;
	int id;
	int spd;
	int curframe;
	int stage;
	float time;
	float endtime;
	int selfbegin;
	int selftotal;
	int waybegin;
	int waytotal;
	int destbegin;
	int desttotal;
	BYTE mgcfileindex;
	BYTE dist;
	int extrax;
	int extray;
	int mgcx;
	int mgcy;
	BYTE lock;
	BYTE loop;
};

struct UserMgcInput
{
	BYTE prikey;
	BYTE hlpkey;
	BYTE mgcidx;
};

typedef UserMgcInput UserMgcTable[MAX_MGC_NUM];

typedef std::list<MagicInfo> MagicList;
typedef std::map<BYTE,BYTE> MgcIptTable;

class GameMagicManager : public RenderObject
{
public:
	GameMagicManager()
	{
		m_pSprMgc = NULL;
	}
	~GameMagicManager()
	{
		delete m_pSprMgc;
		m_mgc.clear();
	}

public:
	bool InsertMagic(GameObject* _from, GameObject* _to, MagicInfo* _mgc, int _spd);

public:
	virtual void Render();
	virtual void Update(float _dt);

private:
	hgeSprite* m_pSprMgc;
	MagicList m_mgc;
};


#endif