#include "GameTextureManagerWil.h"
#include "../Common/GlobalFunction.h"
#include <assert.h>
//////////////////////////////////////////////////////////////////////////
//	member
GameTextureManagerWil::GameTextureManagerWil()
{
	m_pTexIndexTable = NULL;
	m_pImageInfo = NULL;
}

GameTextureManagerWil::~GameTextureManagerWil()
{
	ReleaseAllTexture();
	delete[] m_pTexIndexTable;
	delete[] m_pImageInfo;
	m_pTexIndexTable = NULL;
	m_pImageInfo = NULL;
}




bool GameTextureManagerWil::LoadPackage(const char* _pszFile)
{
	if(__super::Load(_pszFile))
	{
		//
		if(m_stWilHead.ImageCount > 0)
		{
			m_pTexIndexTable = new DWORD[m_stWilHead.ImageCount];
			ZeroMemory(m_pTexIndexTable, m_stWilHead.ImageCount * sizeof(DWORD));

			m_pImageInfo = new ImageInfo[m_stWilHead.ImageCount];
			return LoadImageInfo();
		}
		else
		{
			AfxGetHge()->System_Log("文件[%s]图片数量为0", _pszFile);
			return false;
		}
	}
	return false;
}

bool GameTextureManagerWil::LoadImageInfo()
{
	for(int i = 0; i < m_stWilHead.ImageCount; ++i)
	{
		if(HFILE_ERROR == ::SetFilePointer(m_hWilFile, m_pIndexTable[i], 0, FILE_BEGIN))
		{
			//::CloseHandle(m_hWilFile);
			//m_hWilFile = NULL;
			return false;
		}
		DWORD nCounter = 0;
		if(!::ReadFile(m_hWilFile, &m_pImageInfo[i].Width, sizeof(WORD), &nCounter, NULL))
		{
			//goto _READ_ERROR_WIL;
			return false;
		}
		if(!::ReadFile(m_hWilFile, &m_pImageInfo[i].Height, sizeof(WORD), &nCounter, NULL))
		{
			//goto _READ_ERROR_WIL;
			return false;
		}
		if(!::ReadFile(m_hWilFile, m_pImageInfo[i].unknow, sizeof(DWORD), &nCounter, NULL))
		{
			//goto _READ_ERROR_WIL;
			return false;
		}
	}

	return true;
}

HTEXTURE GameTextureManagerWil::GetTexture(int _index)
{
	assert(s_HDC != 0 &&
		s_pHGE != NULL);

	HTEXTURE tex = 0;
	if(_index < 0 ||
		_index >= m_stWilHead.ImageCount)
	{
		return 0;
	}
	if(m_pTexIndexTable == NULL)
	{
		return 0;
	}
	if(m_pTexIndexTable[_index] != 0)
	{
		return m_pTexIndexTable[_index];
	}

	HBITMAP bmp = GetBitmap(s_HDC, _index);
	if(bmp)
	{
		BITMAP bm;
		::GetObject(bmp, sizeof(bm), &bm);

		tex = s_pHGE->Texture_Create(bm.bmWidth, bm.bmHeight);
		DWORD dwARGB;
		if(tex)
		{
			//	判断显卡是否支持2次幂的纹理
			int nTextWidth = s_pHGE->Texture_GetWidth(tex);
			int nTextHeight = s_pHGE->Texture_GetHeight(tex);

			if(bm.bmWidth != nTextWidth ||
				bm.bmHeight != nTextHeight)
			{
				//	不支持 解析
				int nWidthOffset = nTextWidth - bm.bmWidth;
				int nHeightOffset = nTextHeight - bm.bmHeight;
				int x = 0;
				int y = 0;

				DWORD* pSrc = new DWORD[bm.bmWidth * bm.bmHeight];
				::GetBitmapBits(bmp, bm.bmWidth * bm.bmHeight * sizeof(DWORD), pSrc);
				DWORD* pColors = s_pHGE->Texture_Lock(tex, false);
				int nInsertIndex = 0;

				for(int i = 0; i < s_pHGE->Texture_GetWidth(tex) * s_pHGE->Texture_GetHeight(tex); ++i)
				{
					x = i % nTextWidth;
					y = i / nTextWidth;

					if(x < bm.bmWidth &&
						y < bm.bmHeight)
					{
						//	在需求范围内
						dwARGB = pSrc[nInsertIndex++];
						if(!dwARGB)
						{
							//	黑色 转为透明
						}
						else
						{
							//	非黑色 alpha通道0xFF
							dwARGB |= 0xFF000000;
						}
						pColors[i] = dwARGB;
					}
					else
					{
						//	在范围外
						dwARGB = 0x00000000;
						pColors[i] = dwARGB;
					}
				}

				//	释放资源
				delete[] pSrc;
				s_pHGE->Texture_Unlock(tex);
				::DeleteObject(bmp);
				m_pTexIndexTable[_index] = tex;
				return tex;
			}
			else
			{
				//	显卡支持
				DWORD* pColors = s_pHGE->Texture_Lock(tex, false);
				DWORD* pSrc = new DWORD[bm.bmWidth * bm.bmHeight];
				::GetBitmapBits(bmp, bm.bmWidth * bm.bmHeight * sizeof(DWORD), pSrc);
				//	拷贝入纹理
				for(int i = 0; i < bm.bmWidth * bm.bmHeight; ++i)
				{
					dwARGB = pSrc[i];
					if(!dwARGB)
					{
						//	dwARGB == 0x00000000 alpha透明
						//dwARGB &= 0xFF000000;
					}
					else
					{
						//	不是黑色 alpha通道FF
						dwARGB |= 0xFF000000;
					}
					pColors[i] = dwARGB;
				}
				s_pHGE->Texture_Unlock(tex);
				m_pTexIndexTable[_index] = tex;
				//	释放资源
				delete[] pSrc;
				::DeleteObject(bmp);
				return tex;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	return 0;
}

int GameTextureManagerWil::GetTextureWidth(int _idx)
{
	int nIndex = _idx;
	if(nIndex >= 0 &&
		nIndex < GetBitmapCount())
	{
		return m_pImageInfo[_idx].Width;
	}

	return 0;
}

int GameTextureManagerWil::GetTextureHeight(int _idx)
{
	int nIndex = _idx;
	if(nIndex >= 0 &&
		nIndex < GetBitmapCount())
	{
		return m_pImageInfo[_idx].Height;
	}
	return 0;
}

short GameTextureManagerWil::GetTextureOffsetX(int _idx)
{
	int nIndex = _idx;
	if(nIndex >= 0 &&
		nIndex < GetBitmapCount())
	{
		return *(WORD*)(m_pImageInfo[_idx].unknow);
	}

	return 0;
}


short GameTextureManagerWil::GetTextureOffsetY(int _idx)
{
	int nIndex = _idx;
	if(nIndex >= 0 &&
		nIndex < GetBitmapCount())
	{
		return *(WORD*)(m_pImageInfo[_idx].unknow + 2);
	}

	return 0;
}

void GameTextureManagerWil::ReleaseAllTexture()
{
	if(m_pTexIndexTable == NULL)
	{
		return;
	}

	for(int i = 0; i < m_stWilHead.ImageCount; ++i)
	{
		if(m_pTexIndexTable[i] != 0)
		{
			s_pHGE->Texture_Free(m_pTexIndexTable[i]);
			m_pTexIndexTable[i] = 0;
		}
	}
}