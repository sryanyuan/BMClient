#include "TextureCache.h"
//////////////////////////////////////////////////////////////////////////
TextureCache::TextureCache()
{
	m_pHGE = NULL;
}

TextureCache::~TextureCache()
{
	ClearCache(true);
}


HTEXTURE TextureCache::GetCache(DWORD _dwKey)
{
	TextureCacheMap::const_iterator fndIter = m_xCache.find(_dwKey);
	if(fndIter == m_xCache.end())
	{
		return NULL;
	}

	return fndIter->second;
}

void TextureCache::SetCache(DWORD _dwKey, HTEXTURE _tex)
{
	TextureCacheMap::iterator fndIter = m_xCache.find(_dwKey);
	if(fndIter != m_xCache.end())
	{
		//	warning?
		fndIter->second = _tex;
	}
	else
	{
		m_xCache.insert(std::make_pair(_dwKey, _tex));
	}
}

void TextureCache::RemoveCache(DWORD _dwKey, bool _bRelease /* = true */)
{
	TextureCacheMap::const_iterator fndIter = m_xCache.find(_dwKey);
	if(fndIter != m_xCache.end())
	{
		HTEXTURE tex = fndIter->second;

		if(_bRelease &&
			NULL != m_pHGE)
		{
			m_pHGE->Texture_Free(tex);
		}

		m_xCache.erase(fndIter);
	}
}

void TextureCache::ClearCache(bool _bRelease /* = true */)
{
	TextureCacheMap::const_iterator begIter = m_xCache.begin();
	TextureCacheMap::const_iterator endIter = m_xCache.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		HTEXTURE tex = begIter->second;

		if(_bRelease &&
			NULL != m_pHGE)
		{
			m_pHGE->Texture_Free(tex);
		}
	}

	m_xCache.clear();
}