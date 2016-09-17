#ifndef _INC_STATUSDEFINE_
#define _INC_STATUSDEFINE_
//////////////////////////////////////////////////////////////////////////
struct StatusInfo
{
	int nStatusID;
	int nParam;
	DWORD dwExpireTime;

	StatusInfo()
	{
		nStatusID = 0;
		dwExpireTime = 0;
		nParam = 0;
	}
};

#define GSTATUS_DOUBLEDROP		(200)
#define GSTATUS_POISON			201
#define GSTATUS_CHARMAC			202
#define GSTATUS_SHIELD			203
#define GSTATUS_ENERGYSHIELD	204
#define GSTATUS_JINGANG			205
#define GSTATUS_SUITSAMELEVEL	206
//////////////////////////////////////////////////////////////////////////
#endif