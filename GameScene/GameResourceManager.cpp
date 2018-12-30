#include "GameResourceManager.h"
#include "../Common/GlobalFunction.h"
#include "../GameScene/GameResourceUtil.h"

//////////////////////////////////////////////////////////////////////////
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

bool GameResourceManager::LoadFileWithFilename(const char* _pszFileName)
{
	char szFilePath[MAX_PATH];
	
	if (m_szCustomDir[0] != 0) {
		sprintf(szFilePath, "%s/%s\\Data\\%s", GetGameResourceDir(), m_szCustomDir, _pszFileName);
	} else {
		sprintf(szFilePath, "%s\\Data\\%s", GetGameResourceDir(), _pszFileName);
	}
	return LoadFile(szFilePath);
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
		GetGameResourceDir(), _index - 20);
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