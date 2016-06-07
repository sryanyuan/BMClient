#ifndef _INC_GAMESOUNDMANAGER_
#define _INC_GAMESOUNDMANAGER_
//////////////////////////////////////////////////////////////////////////
#include <fmod.hpp>
#include <fmod_errors.h>
#include <map>
#include <list>
#include "../Common/GlobalFunction.h"
#include "../../CommonModule/ByteBuffer.h"
//////////////////////////////////////////////////////////////////////////
#define MAX_CHANNELS	100

#define SD_OBJECT		1
#define SD_MAGIC		2
#define SD_GAME			3
#define SD_STREAM		4
#define SD_HUM			5

#define SDOBJ_SHOW		0
#define SDOBJ_BELLOW	1
#define SDOBJ_ATTACK	2
#define SDOBJ_STRUCK	4
#define SDOBJ_DEAD		5
#define SDOBJ_ATT_1		6
#define SDOBJ_ATT_2		7
#define SDOBJ_ATT_3		8
#define SDOBJ_ATT_4		9
#define SDOBJ_ATT_5		10

#define SDHUM_WALK		3
#define SDHUM_SWORDATTACK	54
#define SDHUM_MALESTRUCK	138
#define SDHUM_FEMALESTRUCK	139
#define SDHUM_MALEGONGSHA	4
#define SDHUM_FEMALEGONGSHA	5
#define SDHUM_MALEDEAD		144
#define SDHUM_FEMALEDEAD	145
#define SDHUM_ATTACKSTICK	8
#define SDHUM_FISTATTACK	57
#define SDHUM_LIEHUO		55
#define SDHUM_CISHA			6

#define SDGAME_CLICKDRUG	0
#define SDGAME_CLICKDOWN	1
#define SDGAME_CLICKBUTTON	105
#define SDGAME_FETCHMONEY	106
#define SDGAME_ATTACKEDNOW	72
#define SDGAME_ATTACKEDW	81
#define SDGAME_ITEMDRUGC	108
#define SDGAME_OBJECTFLY	110
#define SDGAME_ITEMDRUGD	113
#define SDGAME_ITEMWEAPONC	111
#define SDGAME_ITEMWEAPOND	116
#define SDGAME_ITEMCLOTHC	112
#define SDGAME_ITEMCLOTHD	117
#define SDGAME_ITEMRINGC	114
#define SDGAME_ITEMRINGD	118
#define SDGAME_ITEMDEFAULTC	125
#define SDGAME_ITEMDEFAULTD	126
#define SDGAME_ITEMSCROLLC	113
#define SDGAME_ITEMSCROLLD	118
//////////////////////////////////////////////////////////////////////////
class GameSoundManager;

class GameSound
{
public:
	GameSound(const char* _pszFile);
	~GameSound();

public:
	bool IsPlaying();
	void Play();

	inline DWORD GetID()				{return m_dwID;}
	inline void SetID(DWORD _dwID)		{m_dwID = _dwID;}

private:
	FMOD::Channel* m_pChannel;
	FMOD::Sound* m_pSound;
	DWORD m_dwID;
};
//////////////////////////////////////////////////////////////////////////
typedef std::list<GameSound*> SOUNDLIST;
typedef std::map<DWORD, SOUNDLIST*> SOUNDCACHE;
//////////////////////////////////////////////////////////////////////////

class GameSoundManager
{
public:
	static GameSoundManager* GetInstancePtr()
	{
		static GameSoundManager* pIns = NULL;
		if(NULL == pIns)
		{
			pIns = new GameSoundManager;
		}
		return pIns;
	}

	~GameSoundManager();

protected:
	GameSoundManager();

public:
	//	Beginning or end of the game
	bool Initialize();
	void Destroy();
	void UpdateSound();

	//	
	void ReleaseCache();

	bool PlayBkSound(DWORD _dwID, bool _bLoop = true);
	void StopBkSound();

	bool PlayObjectSound(DWORD _dwID, DWORD _dwAction);
	bool PlayObjectSoundDirect(DWORD _dwID, DWORD _dwSoundIndex);
	bool PlayHumSound(DWORD _dwType);
	bool PlayMagicSound(DWORD _dwMgcID, DWORD _dwStage);
	bool PlayGameSound(DWORD _dwID);

	inline FMOD::System* GetSoundSystem()			{return m_pSoundSystem;}
	void PauseBkSound();
	void ResumeBkSound();

private:
	void _PrintError(FMOD_RESULT _eRet);
	bool InsertCache(DWORD _dwTag, GameSound* _pSnd);
	GameSound* GetCache(DWORD _dwTag);
	int TranslateObjSoundType(int _type);

protected:
	FMOD::System* m_pSoundSystem;
	FMOD::Sound* m_pBkSound;
	FMOD::Channel* m_pBkChannel;
	//	Cache
	SOUNDCACHE m_xSndCache;

	//	Declaration of the friend class 'GameSound'..
	friend class GameSound;
};
//////////////////////////////////////////////////////////////////////////
#endif