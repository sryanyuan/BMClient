#include "OutlineTextureManager.h"
#include "gfx_utils.h"
//////////////////////////////////////////////////////////////////////////
OutlineTextureManager::OutlineTextureManager()
{

}

OutlineTextureManager::~OutlineTextureManager()
{
	ClearAll();
}

HTEXTURE OutlineTextureManager::GetOutilneTextureMonster(HTEXTURE _texNormal, int _nMonsterId, int _nFrame)
{
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

		//HTEXTURE newTex = Gfx_GetOutlineTexture(m_pHGE, _texNormal, 1, ARGB(255, 255, 0, 0));
		HTEXTURE newTex = Gfx_GetOutlineTexture(m_pHGE, _texNormal, 1, 0xffFFFF00);
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

HTEXTURE OutlineTextureManager::GetOutlineTextureHum(HTEXTURE _texNormal, int _nHumResIndex, int _nFrame)
{
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

		HTEXTURE newTex = Gfx_GetOutlineTexture(m_pHGE, _texNormal, 1, ARGB(255, 0, 255, 0));
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

void OutlineTextureManager::ClearCacheMonster()
{
	m_xCacheMonster.ClearCache();
}

void OutlineTextureManager::ClearCacheHum()
{
	m_xCacheHum.ClearCache();
}

void OutlineTextureManager::ClearAll()
{
	ClearCacheHum();
	ClearCacheMonster();
}