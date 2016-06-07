#ifndef _INC_TEXTURE_CACHE_
#define _INC_TEXTURE_CACHE_
//////////////////////////////////////////////////////////////////////////
#include <map>
#include <hge.h>
//////////////////////////////////////////////////////////////////////////
typedef std::map<DWORD, HTEXTURE> TextureCacheMap;
//////////////////////////////////////////////////////////////////////////
class TextureCache
{
public:
	TextureCache();
	~TextureCache();

public:
	inline void SetHGE(HGE* _pHge)
	{
		m_pHGE = _pHge;
	}
	HTEXTURE GetCache(DWORD _dwKey);
	void SetCache(DWORD _dwKey, HTEXTURE _tex);

	void RemoveCache(DWORD _dwKey, bool _bRelease = true);
	void ClearCache(bool _bRelease = true);

private:
	TextureCacheMap m_xCache;
	HGE* m_pHGE;
};
//////////////////////////////////////////////////////////////////////////
#endif