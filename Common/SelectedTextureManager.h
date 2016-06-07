#ifndef _INC_SELECTED_TEXTURE_MANAGER_
#define _INC_SELECTED_TEXTURE_MANAGER_
//////////////////////////////////////////////////////////////////////////
#include "TextureCache.h"
//////////////////////////////////////////////////////////////////////////
class SelectedTextureManager
{
public:
	~SelectedTextureManager();

protected:
	//	single instance
	SelectedTextureManager();

public:
	static SelectedTextureManager* GetInstance()
	{
		static SelectedTextureManager s_ins;
		return &s_ins;
	}

public:
	inline void SetHGE(HGE* _pHge)
	{
		m_pHGE = _pHge;
		m_xCacheHum.SetHGE(_pHge);
		m_xCacheMonster.SetHGE(_pHge);
	}
	HTEXTURE GetSelectedTextureMonster(HTEXTURE _texNormal, int _nMonsterId, int _nFrame);
	HTEXTURE GetSelectedTextureHum(HTEXTURE _texNormal, int _nHumResIndex, int _nFrame);

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