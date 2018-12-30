#ifndef GAMERESOURCEMANAGER_H_
#define GAMERESOURCEMANAGER_H_
#include "GameTextureManagerWil.h"
#include "GameTextureManagerSib.h"
#include "GameTextureManagerSPB.h"
#include <map>
#include <string>
#include "../Common/GlobalDefine.h"

#define LOADING_TEX_SUM		150
#define EXTRA_OBJECTS_SUM	30


class GameResourceManager
{
public:
	GameResourceManager()
	{
		ZeroMemory(m_texs, sizeof(m_texs));
		ZeroMemory(m_extratexs, sizeof(m_extratexs));
		m_szCustomDir[0] = 0;
	}
	~GameResourceManager()
	{
		ReleaseAllFile();
	}

public:
	int GetResourceIndex(const char* _pszFileName);
	bool LoadFile(const char* _pszFileName);
	bool LoadFileWithFilename(const char* _pszFileName);
	bool LoadFileSibMapPackage(int _index);
	GameTextureManager* GetSibMapPackage(int _index);
	//bool LoadWilFileEx(const char* lpszFileName);

	void SetCustomDir(const char* szPath) {
		if (strlen(szPath) > MAX_PATH - 1) {
			return;
		}
		strcpy(m_szCustomDir, szPath);
	}
	const char* GetCustomDir() {
		if (0 == m_szCustomDir[0]) {
			return NULL;
		}
		return m_szCustomDir;
	}

	void ReleaseAllFile();
	GameTextureManager* GetTexs(int _index)
	{
		if(_index < 0 ||
			_index > sizeof(m_texs) / 4 - 1)
		{
			return NULL;
		}
		return m_texs[_index];
	}
	GameTextureManager* GetTexsEx(int _objs)
	{
		if(_objs >= 10 &&
			_objs < 10 + EXTRA_OBJECTS_SUM)
		{
			return m_extratexs[_objs - 10];
		}

		return NULL;
	}
	inline HTEXTURE GetTexture(int _nResId, int _nTextureId)
	{
		GameTextureManager* pTexMgr = GetTexs(_nResId);
		if(NULL == pTexMgr)
		{
			return NULL;
		}
		return pTexMgr->GetTexture(_nTextureId);
	}
	inline void GetTextureSize(int _nResId, int _nTextureId, SIZE* _pSz)
	{
		GameTextureManager* pTexMgr = GetTexs(_nResId);
		if(NULL == pTexMgr)
		{
			return;
		}

		_pSz->cx = pTexMgr->GetTextureWidth(_nTextureId);
		_pSz->cy = pTexMgr->GetTextureHeight(_nTextureId);
	}
	int GetTexsCount()
	{
		for(int i = 0; i < sizeof(m_texs) / 4; ++i)
		{
			if(m_texs[i] == NULL)
			{
				return i;
			}
		}
		return sizeof(m_texs) / 4;
	}
	inline bool IsExtraObjectsExist(int _idx)
	{
		if(_idx < 20)
		{
			return false;
		}
		return NULL != m_extratexs[_idx - 20];
	}

public:
	static GameResourceManager* GetInstance()
	{
		static GameResourceManager* pIns = NULL;
		if(pIns == NULL)
		{
			pIns = new GameResourceManager;
		}
		return pIns;
	}

protected:
	bool LoadWilFile(const char* lpszFileName);
	bool LoadSibFile(const char* lpszFileName);
	bool LoadSpbFile(const char* lpszFileName);

private:
	GameTextureManager* m_texs[LOADING_TEX_SUM];
	GameTextureManager* m_extratexs[EXTRA_OBJECTS_SUM];
	char m_szCustomDir[MAX_PATH];
};

#endif