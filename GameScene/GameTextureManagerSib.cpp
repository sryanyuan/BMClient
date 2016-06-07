#include "GameTextureManagerSib.h"
//////////////////////////////////////////////////////////////////////////
#include "../Common/GlobalFunction.h"
#include <assert.h>
//#include <d3dx8.h>
//#include <d3d8.h>
//////////////////////////////////////////////////////////////////////////
//	member
GameTextureManagerSib::GameTextureManagerSib()
{
	m_pTexIndexTable = NULL;
	m_pImageInfo = NULL;
}

GameTextureManagerSib::~GameTextureManagerSib()
{
	ReleaseAllTexture();
	delete[] m_pTexIndexTable;
	m_pTexIndexTable = NULL;
	m_pImageInfo = NULL;
}




bool GameTextureManagerSib::LoadPackage(const char* _pszFile)
{
	if(Load(_pszFile))
	{
		//
		if(m_stHeader.uImgCount > 0)
		{
			m_pTexIndexTable = new DWORD[m_stHeader.uImgCount];
			ZeroMemory(m_pTexIndexTable, m_stHeader.uImgCount * sizeof(DWORD));
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

HTEXTURE GameTextureManagerSib::GetTexture(int _index)
{
	assert(s_HDC != 0);

	HTEXTURE tex = 0;
	if(_index < 0 ||
		_index >= m_stHeader.uImgCount)
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

int GameTextureManagerSib::GetTextureWidth(int _idx)
{
	int nIndex = _idx;
	if(nIndex >= 0 &&
		nIndex < m_stHeader.uImgCount)
	{
		return m_pInfo[_idx].uImgWidth;
	}

	return 0;
}

int GameTextureManagerSib::GetTextureHeight(int _idx)
{
	int nIndex = _idx;
	if(nIndex >= 0 &&
		nIndex < m_stHeader.uImgCount)
	{
		return m_pInfo[_idx].uImgHeight;
	}
	return 0;
}

short GameTextureManagerSib::GetTextureOffsetX(int _idx)
{
	int nIndex = _idx;
	if(nIndex >= 0 &&
		nIndex < m_stHeader.uImgCount)
	{
		return m_pInfo[_idx].sImgOftX;
	}

	return 0;
}


short GameTextureManagerSib::GetTextureOffsetY(int _idx)
{
	int nIndex = _idx;
	if(nIndex >= 0 &&
		nIndex < m_stHeader.uImgCount)
	{
		return m_pInfo[_idx].sImgOftY;
	}

	return 0;
}

void GameTextureManagerSib::ReleaseAllTexture()
{
	if(m_pTexIndexTable == NULL)
	{
		return;
	}

	for(int i = 0; i < m_stHeader.uImgCount; ++i)
	{
		if(m_pTexIndexTable[i] != 0)
		{
			s_pHGE->Texture_Free(m_pTexIndexTable[i]);
			m_pTexIndexTable[i] = 0;
		}
	}
}