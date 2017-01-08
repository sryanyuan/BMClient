#include "GameTextureManagerSPB.h"
#include "../Common/GlobalFunction.h"
#include <d3dx8.h>
#include <d3d8.h>
//////////////////////////////////////////////////////////////////////////
/*
d3d8.lib
d3dx8.lib
*/
//////////////////////////////////////////////////////////////////////////
static LPDIRECT3DDEVICE8 s_pD3DDev = NULL;
//////////////////////////////////////////////////////////////////////////
HTEXTURE RawLoadTexture(const char* _pData, DWORD _dwSize)
{
	if(NULL == s_pD3DDev)
	{
		return NULL;
	}
	void *data;
	DWORD _size;
	D3DFORMAT fmt1, fmt2;
	LPDIRECT3DTEXTURE8 pTex;
	D3DXIMAGE_INFO info;

	data = (void*)_pData;
	_size = _dwSize;

	if(*(DWORD*)data == 0x20534444) // Compressed DDS format magic number
	{
		fmt1=D3DFMT_UNKNOWN;
		fmt2=D3DFMT_A8R8G8B8;
	}
	else
	{
		fmt1=D3DFMT_A8R8G8B8;
		fmt2=D3DFMT_UNKNOWN;
	}

	//	if( FAILED( D3DXCreateTextureFromFileInMemory( pD3DDevice, data, _size, &pTex ) ) ) pTex=NULL;
	if( FAILED( D3DXCreateTextureFromFileInMemoryEx( s_pD3DDev, data, _size,
		D3DX_DEFAULT, D3DX_DEFAULT,
		false ? 0:1,		// Mip levels
		0,					// Usage
		fmt1,				// Format
		D3DPOOL_MANAGED,	// Memory pool
		D3DX_FILTER_NONE,	// Filter
		D3DX_DEFAULT,		// Mip filter
		0,					// Color key
		&info, NULL,
		&pTex ) ) )

		if( FAILED( D3DXCreateTextureFromFileInMemoryEx( s_pD3DDev, data, _size,
			D3DX_DEFAULT, D3DX_DEFAULT,
			false ? 0:1,		// Mip levels
			0,					// Usage
			fmt2,				// Format
			D3DPOOL_MANAGED,	// Memory pool
			D3DX_FILTER_NONE,	// Filter
			D3DX_DEFAULT,		// Mip filter
			0,					// Color key
			&info, NULL,
			&pTex ) ) )

		{	
			return NULL;
		}

		return (HTEXTURE)pTex;
}
//////////////////////////////////////////////////////////////////////////
GameTextureManagerSpb::GameTextureManagerSpb()
{
	m_pTexIndexTable = NULL;
}

GameTextureManagerSpb::~GameTextureManagerSpb()
{
	ReleaseAllTexture();
	delete[] m_pTexIndexTable;
	m_pTexIndexTable = NULL;
}


bool GameTextureManagerSpb::LoadPackage(const char* _pszFile)
{
	if(Load(_pszFile))
	{
		//
		if(m_stHeader.nImgCount > 0)
		{
			m_pTexIndexTable = new DWORD[m_stHeader.nImgCount];
			ZeroMemory(m_pTexIndexTable, m_stHeader.nImgCount * sizeof(DWORD));
			return true;
		}
		else
		{
			AfxGetHge()->System_Log("文件[%s]图片数量为0", _pszFile);
			return false;
		}
	}
	return false;
}

HTEXTURE GameTextureManagerSpb::GetTexture(int _index)
{
	if(NULL == s_pD3DDev)
	{
		HTEXTURE tex = AfxGetHge()->Texture_Create(1, 1);
		if(tex)
		{
			LPDIRECT3DTEXTURE8 pTexture = (LPDIRECT3DTEXTURE8)tex;
			HRESULT hr = pTexture->GetDevice(&s_pD3DDev);
			if(!SUCCEEDED(hr))
			{
				s_pD3DDev = NULL;
				return NULL;
			}
		}
	}

	HTEXTURE tex = 0;
	if(_index < 0 ||
		_index >= m_stHeader.nImgCount)
	{
		return 0;
	}

	if(m_pTexIndexTable[_index] != 0)
	{
		return m_pTexIndexTable[_index];
	}

	const SPBFileInfo* pInfo = GetImgInfo(_index);
	if(NULL == pInfo ||
		pInfo->dwOffset == 0 ||
		pInfo->uSize == 0)
	{
		return NULL;
	}

	const char* pData = GetImgData(_index, NULL);
	tex = RawLoadTexture(pData, pInfo->uSize);
	delete[] pData;

	m_pTexIndexTable[_index] = tex;

	return tex;
}

int GameTextureManagerSpb::GetTextureWidth(int _idx)
{
	const SPBFileInfo* pInfo = GetImgInfo(_idx);

	if(pInfo)
	{
		return pInfo->nWidth;
	}

	return 0;
}

int GameTextureManagerSpb::GetTextureHeight(int _idx)
{
	int nIndex = _idx;
	const SPBFileInfo* pInfo = GetImgInfo(_idx);

	if(pInfo)
	{
		return pInfo->nHeight;
	}
	return 0;
}

short GameTextureManagerSpb::GetTextureOffsetX(int _idx)
{
	int nIndex = _idx;
	const SPBFileInfo* pInfo = GetImgInfo(_idx);

	if(pInfo)
	{
		return pInfo->sOftX;
	}

	return 0;
}


short GameTextureManagerSpb::GetTextureOffsetY(int _idx)
{
	int nIndex = _idx;
	const SPBFileInfo* pInfo = GetImgInfo(_idx);

	if(pInfo)
	{
		return pInfo->sOftY;
	}

	return 0;
}

void GameTextureManagerSpb::ReleaseAllTexture()
{
	if(m_pTexIndexTable == NULL)
	{
		return;
	}

	for(int i = 0; i < m_stHeader.nImgCount; ++i)
	{
		if(m_pTexIndexTable[i] != 0)
		{
			AfxGetHge()->Texture_Free(m_pTexIndexTable[i]);
			m_pTexIndexTable[i] = 0;
		}
	}
}