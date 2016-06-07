#ifndef _INC_OUTLINETEXTUREMANAGER_
#define _INC_OUTLINETEXTUREMANAGER_
//////////////////////////////////////////////////////////////////////////
#include "TextureCache.h"
//////////////////////////////////////////////////////////////////////////
class OutlineTextureManager
{
public:
	~OutlineTextureManager();
	static OutlineTextureManager* GetInstance()
	{
		static OutlineTextureManager ins;
		return &ins;
	}

protected:
	OutlineTextureManager();

public:
	inline void SetHGE(HGE* _pHge)
	{
		m_pHGE = _pHge;
		m_xCacheHum.SetHGE(_pHge);
		m_xCacheMonster.SetHGE(_pHge);
	}
	HTEXTURE GetOutilneTextureMonster(HTEXTURE _texNormal, int _nMonsterId, int _nFrame);
	HTEXTURE GetOutlineTextureHum(HTEXTURE _texNormal, int _nHumResIndex, int _nFrame);

	void ClearCacheMonster();
	void ClearCacheHum();
	void ClearAll();

protected:
	TextureCache m_xCacheMonster;
	TextureCache m_xCacheHum;
	HGE* m_pHGE;
};
//////////////////////////////////////////////////////////////////////////
#endif