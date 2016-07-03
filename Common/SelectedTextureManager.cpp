#include "SelectedTextureManager.h"
#include "gfx_utils.h"
//////////////////////////////////////////////////////////////////////////
SelectedTextureManager::SelectedTextureManager()
{

}

SelectedTextureManager::~SelectedTextureManager()
{
	ClearAll();
}


HTEXTURE SelectedTextureManager::GetSelectedTextureMonster(HTEXTURE _texNormal, int _nMonsterId, int _nFrame)
{
	return _texNormal;

	//	make key
	DWORD dwKey = MAKELONG(_nMonsterId, _nFrame);
	HTEXTURE tex = m_xCacheMonster.GetCache(dwKey);

	if(NULL == tex)
	{
		//	new texture
		if(NULL == _texNormal)
		{
			return NULL;
		}

		if(NULL == m_pHGE)
		{
			return NULL;
		}

		//HTEXTURE newTex = Gfx_GetBrightTexture(m_pHGE, _texNormal, 30);
		HTEXTURE newTex = Gfx_GetBrightTextureEx(m_pHGE, _texNormal, 1.3);
		if(NULL != newTex)
		{
			m_xCacheMonster.SetCache(dwKey, newTex);
		}
		return newTex;
	}
	else
	{
		return tex;
	}
}

HTEXTURE SelectedTextureManager::GetSelectedTextureHum(HTEXTURE _texNormal, int _nHumResIndex, int _nFrame)
{
	return _texNormal;

	//	make key
	DWORD dwKey = MAKELONG(_nHumResIndex, _nFrame);
	HTEXTURE tex = m_xCacheHum.GetCache(dwKey);

	if(NULL == tex)
	{
		//	new texture
		if(NULL == _texNormal)
		{
			return NULL;
		}

		if(NULL == m_pHGE)
		{
			return NULL;
		}

		//HTEXTURE newTex = Gfx_GetBrightTexture(m_pHGE, _texNormal, 20);
		HTEXTURE newTex = Gfx_GetBrightTextureEx(m_pHGE, _texNormal, 1.3);
		if(NULL != newTex)
		{
			m_xCacheHum.SetCache(dwKey, newTex);
		}
		return newTex;
	}
	else
	{
		return tex;
	}
}

void SelectedTextureManager::ClearCacheMonster()
{
	m_xCacheMonster.ClearCache();
}

void SelectedTextureManager::ClearCacheHum()
{
	m_xCacheHum.ClearCache();
}

void SelectedTextureManager::ClearAll()
{
	ClearCacheHum();
	ClearCacheMonster();
}