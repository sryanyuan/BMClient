#ifndef PLAYERSTATUSCONTROL
#define PLAYERSTATUSCONTROL
#include <list>
#include <Windows.h>

class GamePlayer;
#define STATUS_HP	0
#define STATUS_MP	1

#define UPDATE_INTERVAL		1000

struct StatusItem
{
	BYTE type;
	int total;
	int step;
	DWORD btime;
};

typedef std::list<StatusItem>	STATUSLIST;

class PlayerStatusControl
{
public:
	PlayerStatusControl();
	~PlayerStatusControl();

public:
	void AddStatusItem(BYTE _type, int _total, int _step);

public:
	void Update();
	void Init(GamePlayer* _player)
	{
		m_pPlayer = _player;
	}

private:
	STATUSLIST m_list;
	GamePlayer* m_pPlayer;
};

#endif