#include "GameResourceManager.h"
#include "../Common/GlobalFunction.h"

//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
const char* g_szResFile[] = 
{
	"Tiles.sib",
	"SmTiles.sib",
	"Hum.sib",
	"Hum2.sib",
	"Objects.sib",
	"Objects2.sib",
	"Objects3.sib",
	"Objects4.sib",
	"Objects5.sib",
	"Objects6.sib",
	"Objects7.sib",
	"Objects8.sib",
	"Objects9.sib",
	"Objects10.sib",
	"Objects11.sib",
	"Objects12.sib",
	"Objects13.sib",
	"Objects14.sib",
	"Hair.sib",
	"Hair2.sib",
	"Weapon.sib",
	"Weapon2.sib",
	"Weapon_Effect.sib",
	//"Prguse.wil",
	//"Prguse2.wil",
	"OpUI.sib",
	"Items.sib",
	"DnItems.sib",
	"Mon1.sib",
	"Mon2.sib",	//	just for evil dragon
	"Mon3.sib",
	"Mon4.sib",
	"Mon5.sib",
	"Mon6.sib",
	"Mon7.sib",
	"Mon8.sib",
	"Mon9.sib",
	"Mon10.sib",
	"Mon11.sib",
	"Mon12.sib",
	"Mon13.sib",
	"Mon14.sib",
	"Mon15.sib",
	"Mon16.sib",
	"Mon17.sib",
	"Mon18.sib",
	"Mon19.sib",
	"Mon20.sib",
	"Mon21.sib",
	"Mon22.sib",
	"Mon23.sib",
	"Mon24.sib",
	"Mon25.sib",
	"Mon26.sib",
	"Mon27.sib",
	"Mon28.sib",
	"Mon29.sib",
	"Mon30.sib",
	"Mon31.sib",
	"Mon32.sib",
	"Mon33.sib",
	"mmap.sib",
	"bmap.sib",
	"Magic.sib",
	"Magic2.sib",
	"Magic3.sib",
	"Magic4.sib",
	"Magic5.sib",
	"Magic6.sib",
	"Magic7.sib",
	"Magic8.sib",
	"Magic9.sib",
	"Magic10.sib",
	"npc.sib",
	"StateItem.sib",
	"HumEffect.sib",
	"HumEffect2.sib",
	"MagicIcon.sib",
	"Status.sib",
	"Effect.sib",
	"Custom.spb",
	"Ani.sib"
	//"ChrSel.wil"
};
#else
const char* g_szResFile[] = 
{
	"Tiles.sib",
	"SmTiles.sib",
	"Hum.sib",
	"Hum2.sib",
	"Objects.sib",
	"Objects2.sib",
	"Objects3.sib",
	"Objects4.sib",
	"Objects5.sib",
	"Objects6.sib",
	"Objects7.sib",
	"Objects8.sib",
	"Objects9.sib",
	"Objects10.sib",
	"Objects11.sib",
	"Objects12.sib",
	"Objects13.sib",
	"Objects14.sib",
	"Hair.sib",
	"Hair2.sib",
	"Weapon.sib",
	"Weapon2.sib",
	"Weapon_Effect.sib",
	//"Prguse.wil",
	//"Prguse2.wil",
	"OpUI.sib",
	"Items.sib",
	"DnItems.sib",
	"Mon1.sib",
	"Mon2.sib",		//	just for evil dragon
	"Mon3.sib",
	"Mon4.sib",
	"Mon5.sib",
	"Mon6.sib",
	"Mon7.sib",
	"Mon8.sib",
	"Mon9.sib",
	"Mon10.sib",
	"Mon11.sib",
	"Mon12.sib",
	"Mon13.sib",
	"Mon14.sib",
	"Mon15.sib",
	"Mon16.sib",
	"Mon17.sib",
	"Mon18.sib",
	"Mon19.sib",
	"Mon20.sib",
	"Mon21.sib",
	"Mon22.sib",
	"Mon23.sib",
	"Mon24.sib",
	"Mon25.sib",
	"Mon26.sib",
	"Mon27.sib",
	"Mon28.sib",
	"Mon29.sib",
	"Mon30.sib",
	"Mon31.sib",
	"Mon32.sib",
	"Mon33.sib",
	"mmap.sib",
	"bmap.sib",
	"Magic.sib",
	"Magic2.sib",
	"Magic3.sib",
	"Magic4.sib",
	"Magic5.sib",
	"Magic6.sib",
	"Magic7.sib",
	"Magic8.sib",
	"Magic9.sib",
	"Magic10.sib",
	"npc.sib",
	"StateItem.sib",
	"HumEffect.sib",
	"HumEffect2.sib",
	"MagicIcon.sib",
	"Status.sib",
	"Effect.sib",
	"Custom.spb",
	"Ani.sib"
	//"ChrSel.wil"
};
#endif

void GameResourceManager::ReleaseAllFile()
{
	for(int i = 0; i < GetTexsCount(); ++i)
	{
		m_texs[i]->ReleaseAllTexture();
	}

	for(int i = 0; i < EXTRA_OBJECTS_SUM; ++i)
	{
		if(m_extratexs[i])
		{
			m_extratexs[i]->ReleaseAllTexture();
			delete m_extratexs[i];
			m_extratexs[i] = 0;
		}
	}
}

int GameResourceManager::GetResourceIndex(const char* _pszFileName)
{
	for(int i = 0; i < sizeof(g_szResFile) / sizeof(g_szResFile[0]); ++i)
	{
		if(0 == strcmp(g_szResFile[i], _pszFileName))
		{
			return i;
		}
	}

	return -1;
}

bool GameResourceManager::LoadFile(const char* _pszFileName)
{
	size_t nLen = strlen(_pszFileName);
	size_t nExtPtr = 0;

	for(nLen; nLen > 0; --nLen)
	{
		if(_pszFileName[nLen] == '.')
		{
			nExtPtr = nLen;
			++nExtPtr;
			break;
		}
	}

	char szExt[10];
	strcpy(szExt, _pszFileName + nExtPtr);
	nLen = strlen(szExt);

	for(size_t i = 0; i < nLen; ++i)
	{
		szExt[i] = tolower(szExt[i]);
	}

	if(0 == strcmp(szExt, "wil"))
	{
		return LoadWilFile(_pszFileName);
	}
	else if(0 == strcmp(szExt, "sib"))
	{
		return LoadSibFile(_pszFileName);
	}
	else if(0 == strcmp(szExt, "spb"))
	{
		return LoadSpbFile(_pszFileName);
	}
	
	return false;
}

bool GameResourceManager::LoadWilFile(const char* lpszFileName)
{
	int nAvailable = GetTexsCount();
	if(nAvailable < sizeof(m_texs) / 4)
	{
		m_texs[nAvailable] = new GameTextureManagerWil();
		return m_texs[nAvailable]->LoadPackage(lpszFileName);
	}
	return false;
}

bool GameResourceManager::LoadSibFile(const char* lpszFileName)
{
	int nAvailable = GetTexsCount();
	if(nAvailable < sizeof(m_texs) / 4)
	{
		m_texs[nAvailable] = new GameTextureManagerSib();
		return m_texs[nAvailable]->LoadPackage(lpszFileName);
	}
	return false;
}

bool GameResourceManager::LoadSpbFile(const char* lpszFileName)
{
	int nAvailable = GetTexsCount();
	if(nAvailable < sizeof(m_texs) / 4)
	{
		m_texs[nAvailable] = new GameTextureManagerSpb();
		return m_texs[nAvailable]->LoadPackage(lpszFileName);
	}
	return false;
}

bool GameResourceManager::LoadFileSibMapPackage(int _index)
{
	if(_index < 20)
	{
		return false;
	}

	int nExtraIndex = _index - 20;
	if(nExtraIndex >= EXTRA_OBJECTS_SUM)
	{
		return false;
	}

	if(m_extratexs[_index - 20] != 0)
	{
		return true;
	}

	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\Data\\Map\\map%03d.sib",
		GetRootPath(), _index - 20);
	m_extratexs[_index - 20] = new GameTextureManagerSib;
	if(!m_extratexs[_index - 20]->LoadPackage(szPath))
	{
		AfxGetHge()->System_Log("∂¡»°[%s] ß∞‹",
			szPath);
		return false;
	}
	return true;
}

GameTextureManager* GameResourceManager::GetSibMapPackage(int _index)
{
	if(_index < 20)
	{
		return NULL;
	}

	int nExtraIndex = _index - 20;
	if(nExtraIndex >= EXTRA_OBJECTS_SUM)
	{
		return NULL;
	}

	return m_extratexs[_index - 20];
}

// bool GameResourceManager::LoadWilFileEx(const char* lpszFileName)
// {
// 	char szObjIdx[5];
// 	ZeroMemory(szObjIdx, sizeof(szObjIdx));
// 
// 	int nCounter = 0;
// 	for(int i = 0; i < strlen(lpszFileName); ++i)
// 	{
// 		if(toupper(lpszFileName[i]) == 'T' &&
// 			toupper(lpszFileName[i + 1]) == 'S')
// 		{
// 			for(int j = i + 2; j < strlen(lpszFileName); ++j)
// 			{
// 				if(lpszFileName[j] >= '0' &&
// 					lpszFileName[j] <= '9')
// 				{
// 					szObjIdx[nCounter++] = lpszFileName[j];
// 				}
// 				else
// 				{
// 					szObjIdx[nCounter] = '\0';
// 					break;
// 				}
// 			}
// 			break;
// 		}
// 	}
// 
// 	if(szObjIdx[0] == '\0')
// 	{
// 		return false;
// 	}
// 
// 	int nKey = atoi(szObjIdx);
// 	if(nKey >= 10 &&
// 		nKey < 10 + EXTRA_OBJECTS_SUM)
// 	{
// 		if(m_extratexs[nKey - 10] == NULL)
// 		{
// 			m_extratexs[nKey - 10] = new GameTextureManager;
// 			if(m_extratexs[nKey - 10]->Load(lpszFileName))
// 			{
// 				return true;
// 			}
// 		}
// 		return true;
// 	}
// 	return false;
// }