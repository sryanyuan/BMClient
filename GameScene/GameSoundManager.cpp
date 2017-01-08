#include "GameSoundManager.h"
#include "../BackMir/BackMir.h"
#include <hge.h>
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
GameSound::GameSound(const char* _pszFile)
{
	m_pSound = NULL;
	m_pChannel = NULL;
	m_dwID = 0;

	FMOD_RESULT eRet = GameSoundManager::GetInstancePtr()->GetSoundSystem()->createSound(_pszFile, FMOD_DEFAULT, NULL, &m_pSound);
	if(eRet != FMOD_OK)
	{
		m_pSound = NULL;
	}
}

GameSound::~GameSound()
{
	if(m_pSound)
	{
		m_pSound->release();
		m_pSound = NULL;
	}
}

void GameSound::Play()
{
	if(m_pSound == NULL)
	{
		return;
	}
	FMOD_RESULT eRet = GameSoundManager::GetInstancePtr()->GetSoundSystem()->playSound(FMOD_CHANNEL_FREE, m_pSound, false, &m_pChannel);

	if(eRet != FMOD_OK)
	{
		GameSoundManager::GetInstancePtr()->_PrintError(eRet);
	}
}

bool GameSound::IsPlaying()
{
	if(NULL == m_pChannel ||
		NULL == m_pSound)
	{
		return false;
	}

	bool bIsPlaying = false;
	m_pChannel->isPlaying(&bIsPlaying);

	return bIsPlaying;
}


//////////////////////////////////////////////////////////////////////////
GameSoundManager::GameSoundManager()
{
	m_pSoundSystem = NULL;
	m_pBkChannel = NULL;
	m_pBkSound = NULL;
}

GameSoundManager::~GameSoundManager()
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
bool GameSoundManager::Initialize()
{
	FMOD_RESULT eRet = FMOD::System_Create(&m_pSoundSystem);
	if(eRet != FMOD_OK)
	{
		AfxGetHge()->System_Log("Error: %s",
			FMOD_ErrorString(eRet));
		return false;
	}

	eRet = m_pSoundSystem->init(MAX_CHANNELS, FMOD_INIT_NORMAL, NULL);
	if(eRet != FMOD_OK)
	{
		AfxGetHge()->System_Log("Error: %s",
			FMOD_ErrorString(eRet));
		return false;
	}

	return true;
}

void GameSoundManager::Destroy()
{
	if(NULL == m_pSoundSystem)
	{
		return;
	}
	ReleaseCache();
	m_pSoundSystem->release();
	m_pSoundSystem = NULL;
}

void GameSoundManager::UpdateSound()
{
	/*GameSound* pSnd = NULL;

	if(!m_xSoundList.empty())
	{
		SOUNDLIST::iterator begiter = m_xSoundList.begin();
		SOUNDMAP::iterator fnditer;

		for(begiter;
			begiter != m_xSoundList.end();
			++begiter)
		{
			pSnd = *begiter;

			if(!pSnd->IsPlaying())
			{
				fnditer = m_xSoundMap.find(pSnd->GetID());
				if(fnditer == m_xSoundMap.end())
				{
					m_xSoundMap.insert(std::make_pair(pSnd->GetID(), pSnd));
				}
				else
				{
					delete pSnd;
				}
			}
		}
	}*/
	m_pSoundSystem->update();
}

void GameSoundManager::ReleaseCache()
{
	GameSound* pSnd = NULL;
	SOUNDLIST* pSndList = NULL;

	if(!m_xSndCache.empty())
	{
		SOUNDCACHE::const_iterator begiter = m_xSndCache.begin();
		for(begiter;
			begiter != m_xSndCache.end();
			++begiter)
		{
			pSndList = begiter->second;
			if(pSndList != NULL)
			{
				SOUNDLIST::const_iterator sditer = pSndList->begin();
				for(sditer;
					sditer != pSndList->end();
					++sditer)
				{
					pSnd = *sditer;
					delete pSnd;
				}
			}
			delete pSndList;
		}
		m_xSndCache.clear();
	}
}

bool GameSoundManager::PlayObjectSound(DWORD _dwID, DWORD _dwAction)
{
	if(!pTheGame->CanPlaySound())
	{
		return false;
	}
	if(NULL == m_pSoundSystem)
	{
		return false;
	}

	WORD wTag = MAKEWORD(SD_OBJECT, _dwAction);
	DWORD dwTag = MAKELONG(wTag, _dwID);

	char szBuf[MAX_PATH];
	int nSndID = TranslateObjSoundType(_dwAction);
	if(nSndID == -1)
	{
		return false;
	}

	wTag = MAKEWORD(SD_OBJECT, nSndID);
	dwTag = MAKELONG(wTag, _dwID);

	GameSound* pNewSnd = GetCache(dwTag);
	if(NULL == pNewSnd)
	{
		sprintf(szBuf, "%s\\Sound\\%d-%d.wav",
			GetRootPath(), _dwID, nSndID);
		pNewSnd = new GameSound(szBuf);
		InsertCache(dwTag, pNewSnd);
	}
	if(NULL != pNewSnd)
	{
		pNewSnd->Play();
	}

	return true;
}

bool GameSoundManager::PlayObjectSoundDirect(DWORD _dwID, DWORD _dwSoundIndex)
{
	if(!pTheGame->CanPlaySound())
	{
		return false;
	}
	if(NULL == m_pSoundSystem)
	{
		return false;
	}

	char szBuf[MAX_PATH];

	WORD wTag = MAKEWORD(SD_OBJECT, _dwSoundIndex);
	DWORD dwTag = MAKELONG(wTag, _dwID);

	GameSound* pNewSnd = GetCache(dwTag);
	if(NULL == pNewSnd)
	{
		sprintf(szBuf, "%s\\Sound\\%d-%d.wav",
			GetRootPath(), _dwID, _dwSoundIndex);
		pNewSnd = new GameSound(szBuf);
		InsertCache(dwTag, pNewSnd);
	}
	if(NULL != pNewSnd)
	{
		pNewSnd->Play();
	}

	return true;
}

bool GameSoundManager::PlayGameSound(DWORD _dwID)
{
	if(!pTheGame->CanPlaySound())
	{
		return false;
	}
	if(NULL == m_pSoundSystem)
	{
		return false;
	}

	WORD wTag = MAKEWORD(SD_GAME, 0);
	DWORD dwTag = MAKELONG(wTag, _dwID);

	char szBuf[MAX_PATH];
	GameSound* pNewSnd = GetCache(dwTag);
	if(NULL == pNewSnd)
	{
		sprintf(szBuf, "%s\\Sound\\g-%d.wav",
			GetRootPath(), _dwID);
		pNewSnd = new GameSound(szBuf);
		InsertCache(dwTag, pNewSnd);
	}
	if(NULL != pNewSnd)
	{
		pNewSnd->Play();
	}
	return true;
}

bool GameSoundManager::PlayHumSound(DWORD _dwType)
{
	if(!pTheGame->CanPlaySound())
	{
		return false;
	}
	if(NULL == m_pSoundSystem)
	{
		return false;
	}

	WORD wTag = MAKEWORD(SD_HUM, 0);
	DWORD dwTag = MAKELONG(wTag, _dwType);

	char szBuf[MAX_PATH];
	GameSound* pNewSnd = GetCache(dwTag);
	if(NULL == pNewSnd)
	{
		sprintf(szBuf, "%s\\Sound\\h-%d.wav",
			GetRootPath(), _dwType);
		pNewSnd = new GameSound(szBuf);
		InsertCache(dwTag, pNewSnd);
	}
	if(NULL != pNewSnd)
	{
		pNewSnd->Play();
	}
	return true;
}

bool GameSoundManager::PlayMagicSound(DWORD _dwMgcID, DWORD _dwStage)
{
	if(!pTheGame->CanPlaySound())
	{
		return false;
	}
	if(NULL == m_pSoundSystem)
	{
		return false;
	}

	WORD wTag = MAKEWORD(SD_MAGIC, _dwStage);
	DWORD dwTag = MAKELONG(wTag, _dwMgcID);

	int nSndID = _dwStage;
	DWORD dwSndIndex = _dwMgcID;

	const MagicRenderInfo* pMgcInfo = NULL;
	if(GameInfoManager::GetInstance()->GetMagicRenderInfo(_dwMgcID, &pMgcInfo))
	{
		if(pMgcInfo &&
			pMgcInfo->sndindex != 0)
		{
			dwSndIndex = pMgcInfo->sndindex;
		}
	}

	char szBuf[MAX_PATH];
	GameSound* pNewSnd = GetCache(dwTag);
	if(NULL == pNewSnd)
	{
		sprintf(szBuf, "%s\\Sound\\m%d-%d.wav",
			GetRootPath(), dwSndIndex, nSndID);
		pNewSnd = new GameSound(szBuf);
		InsertCache(dwTag, pNewSnd);
	}
	if(NULL != pNewSnd)
	{
		pNewSnd->Play();
	}
	return true;
}

bool GameSoundManager::PlayBkSound(DWORD _dwID, bool _bLoop /* = true */)
{
	if(!pTheGame->CanPlaySound())
	{
		return false;
	}
	if(NULL == m_pSoundSystem)
	{
		return false;
	}

	if(NULL != m_pBkSound)
	{
		m_pBkSound->release();
		m_pBkSound = NULL;
	}

	if(NULL == m_pSoundSystem)
	{
		return false;
	}

	char szBuf[MAX_PATH];
	sprintf(szBuf, "%s\\Sound\\b-%d.mp3",
		GetRootPath(), _dwID);
	FMOD_RESULT eRet = m_pSoundSystem->createStream(szBuf, FMOD_DEFAULT, NULL, &m_pBkSound);
	if(eRet != FMOD_OK)
	{
		m_pBkSound = NULL;
		return false;
	}
	if(_bLoop)
	{
		m_pBkSound->setMode(FMOD_LOOP_NORMAL | FMOD_HARDWARE | FMOD_2D);
	}
	eRet = m_pSoundSystem->playSound(FMOD_CHANNEL_FREE, m_pBkSound, false, &m_pBkChannel);
	if(eRet != FMOD_OK)
	{
		m_pBkSound->release();
		m_pBkSound = NULL;
		return false;
	}
	return true;
}

void GameSoundManager::StopBkSound()
{
	if(NULL == m_pSoundSystem)
	{
		return;
	}
	if(m_pBkChannel)
	{
		//m_pBkChannel->stop();
		if(m_pBkSound)
		{
			m_pBkSound->release();
			m_pBkSound = NULL;
		}
		m_pBkChannel = NULL;
	}
}

void GameSoundManager::PauseBkSound()
{
	if(NULL == m_pSoundSystem)
	{
		return;
	}
	if(m_pBkChannel)
	{
		if(m_pBkSound)
		{
			bool bPlaying = false;
			if(FMOD_OK == m_pBkChannel->isPlaying(&bPlaying))
			{
				if(bPlaying)
				{
					m_pBkChannel->setPaused(true);
				}
			}
		}
	}
}

void GameSoundManager::ResumeBkSound()
{
	if(NULL == m_pSoundSystem)
	{
		return;
	}
	if(m_pBkChannel)
	{
		if(m_pBkSound)
		{
			bool bPaused = false;
			if(FMOD_OK == m_pBkChannel->getPaused(&bPaused))
			{
				if(bPaused)
				{
					m_pBkChannel->setPaused(false);
				}
			}

			if(!bPaused)
			{
				//	重新播放
				PlayBkSound(2, true);
			}
		}
	}
	else
	{
		//	重新播放
		PlayBkSound(2, true);
	}
}

//////////////////////////////////////////////////////////////////////////
void GameSoundManager::_PrintError(FMOD_RESULT _eRet)
{
	if(_eRet != FMOD_OK)
	{
		AfxGetHge()->System_Log("Error: %s",
			FMOD_ErrorString(_eRet));
	}
}

bool GameSoundManager::InsertCache(DWORD _dwTag, GameSound* _pSnd)
{
	SOUNDCACHE::const_iterator fnditer = m_xSndCache.find(_dwTag);
	SOUNDLIST* pSndList = NULL;

	if(m_xSndCache.end() == fnditer)
	{
		pSndList = new SOUNDLIST;
		pSndList->push_back(_pSnd);

		m_xSndCache.insert(std::make_pair(_dwTag, pSndList));
		return true;
	}
	else
	{
		pSndList = fnditer->second;
		pSndList->push_back(_pSnd);
		return true;
	}
}

GameSound* GameSoundManager::GetCache(DWORD _dwTag)
{
	SOUNDCACHE::const_iterator fnditer = m_xSndCache.find(_dwTag);
	GameSound* pSnd = NULL;
	SOUNDLIST* pSndList = NULL;

	if(m_xSndCache.end() == fnditer)
	{
		return NULL;
	}
	else
	{
		pSndList = fnditer->second;

		SOUNDLIST::const_iterator begiter = pSndList->begin();
		for(begiter;
			begiter != pSndList->end();
			++begiter)
		{
			pSnd = *begiter;
			if(!pSnd->IsPlaying())
			{
				return pSnd;
			}
		}
		return NULL;
	}
}

int GameSoundManager::TranslateObjSoundType(int _type)
{
	int nType = -1;

	if(_type == ACTION_TURN)
	{
		nType = SDOBJ_BELLOW;
	}
	else if(_type == ACTION_ATTACK)
	{
		nType = SDOBJ_ATTACK;
	}
	else if(_type == ACTION_STRUCK)
	{
		nType = SDOBJ_STRUCK;
	}
	else if(_type == ACTION_DEAD)
	{
		nType = SDOBJ_DEAD;
	}
	else if(_type == ACTION_APPEAR)
	{
		nType = SDOBJ_SHOW;
	}

	if(nType == -1)
	{
		return _type;
	}
	return nType;
}