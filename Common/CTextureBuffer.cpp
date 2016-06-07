#include "CTextureBuffer.h"

//////////////////////////////////////////////////////////////////////////
CTextureBuffer::CTextureBuffer()
{
	if(!s_hge)
	{
		s_hge = hgeCreate(HGE_VERSION);
	}
	m_tex = 0;
	ReleaseBuffer();
}

CTextureBuffer::~CTextureBuffer()
{
	ReleaseBuffer();
	if(s_hge)
	{
		s_hge->Release();
		s_hge = NULL;
	}
}

HGE* CTextureBuffer::s_hge = NULL;



//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 初始化纹理
/************************************************************************/
bool CTextureBuffer::InitBuffer(DWORD _dwWidth, DWORD _dwHeight)
{
	if(_dwHeight < 0 ||
		_dwWidth < 0)
	{
		return false;
	}
	ReleaseBuffer();

	m_tex = s_hge->Texture_Create(_dwWidth, _dwHeight);
	if(!m_tex)
	{
		return false;
	}

	m_stRect.x = _dwWidth;
	m_stRect.y = _dwHeight;
	m_dwRealWidth = s_hge->Texture_GetWidth(m_tex);
	m_dwRealHeight = s_hge->Texture_GetHeight(m_tex);
	if(m_dwRealWidth == _dwWidth &&
		m_dwRealHeight == _dwHeight)
	{
		m_bIsStandardRect = true;
	}

	DWORD* pData = s_hge->Texture_Lock(m_tex, false);
	ZeroMemory(pData, m_dwRealHeight * m_dwRealWidth);
	s_hge->Texture_Unlock(m_tex);

	//	调整1个偏移矩形使得可以写入
	m_stRcOffset[1].left = 0;
	m_stRcOffset[1].right = _dwWidth;
	m_stRcOffset[1].top = 0;
	m_stRcOffset[1].bottom = _dwHeight;
	m_bCanDraw = true;
}

/************************************************************************/
/* 释放纹理
/************************************************************************/
void CTextureBuffer::ReleaseBuffer()
{
	if(m_tex)
	{
		s_hge->Texture_Free(m_tex);
		m_tex = 0;
	}

	ZeroMemory(&m_stRect, sizeof(m_stRect));
	//	禁止写入
	m_bCanDraw = false;
	ZeroMemory(m_stRcOffset, sizeof(m_stRcOffset));
	m_bIsStandardRect = false;
	m_dwRealWidth = m_dwRealHeight = 0;
}

/************************************************************************/
/* 绘制纹理(在无效区域矩形内才会绘制 使用前需要调用Offset)
/************************************************************************/
bool CTextureBuffer::DrawTexture(HTEXTURE _tex, int _nDestX, int _nDestY, int _nWidth, int _nHeight)
{
	if(!m_bCanDraw)
	{
		return false;
	}

	RECT stRcDraw;
	stRcDraw.left = _nDestX;
	stRcDraw.right = _nDestX + _nWidth;
	stRcDraw.top = _nDestY;
	stRcDraw.bottom = _nDestY + _nHeight;

	RECT stRcCanDraw;
	POINT stDrawPos;
	DWORD dwRealWidth = s_hge->Texture_GetWidth(_tex);
	int nInsertDest = 0;
	int nFromSrc = 0;
	DWORD* pDest = s_hge->Texture_Lock(m_tex, false);
	DWORD* pSrc = s_hge->Texture_Lock(_tex);
	DWORD dwPixel = 0;
	if(!pDest ||
		!pSrc)
	{
		return false;
	}

	for(int i = 0; i < 2; ++i)
	{
		if(::IntersectRect(&stRcCanDraw, &stRcDraw, &m_stRcOffset[i]))
		{
			//	偏移1 绘制
			for(int j = stRcCanDraw.left; j <= stRcCanDraw.right; ++j)
			{
				for(int k = stRcCanDraw.top; k <= stRcCanDraw.bottom; ++k)
				{
					if(j < 0 ||
						j >= m_stRect.x ||
						k < 0 ||
						k >= m_stRect.y)
					{
						continue;
					}

					nFromSrc = (k - _nDestY) * dwRealWidth + (j - _nDestX);
					stDrawPos.x = j;
					stDrawPos.y = k;
					if(GetRealPoint(&stDrawPos, &nInsertDest))
					{
						dwPixel = pSrc[nFromSrc];
						if(dwPixel & 0xFF000000)
						{
							//	alpha通道不为0 写入
							pDest[nInsertDest] = dwPixel;
						}
					}
				}
			}
		}
	}

	s_hge->Texture_Unlock(m_tex);
	s_hge->Texture_Unlock(_tex);
	return true;
}

/************************************************************************/
/* 直接绘制（任何区域都能绘制）
/************************************************************************/
bool CTextureBuffer::DrawTextureDirect(HTEXTURE _tex, int _nDestX, int _nDestY, int _nWidth, int _nHeight)
{
	DWORD* pDest = s_hge->Texture_Lock(m_tex, false);
	DWORD* pSrc = s_hge->Texture_Lock(_tex);
	DWORD dwPixel = 0;
	if(!pDest ||
		!pSrc)
	{
		return false;
	}

	RECT stRcCanDraw;
	stRcCanDraw.left = _nDestX;
	stRcCanDraw.right = _nDestX + _nWidth;
	stRcCanDraw.top = _nDestY;
	stRcCanDraw.bottom = _nDestY + _nHeight;

	DWORD dwRealWidth = s_hge->Texture_GetWidth(_tex);
	int nInsertDest = 0;
	int nFromSrc = 0;
	POINT stDrawPos;

	for(int j = stRcCanDraw.left; j <= stRcCanDraw.right; ++j)
	{
		for(int k = stRcCanDraw.top; k <= stRcCanDraw.bottom; ++k)
		{
			if(j < 0 ||
				j >= m_stRect.x ||
				k < 0 ||
				k >= m_stRect.y)
			{
				continue;
			}

			nFromSrc = (k - _nDestY) * dwRealWidth + (j - _nDestX);
			stDrawPos.x = j;
			stDrawPos.y = k;
			if(GetRealPoint(&stDrawPos, &nInsertDest))
			{
				dwPixel = pSrc[nFromSrc];
				if(dwPixel & 0xFF000000)
				{
					//	alpha通道不为0 写入
					pDest[nInsertDest] = dwPixel;
				}
			}
		}
	}

	s_hge->Texture_Unlock(m_tex);
	s_hge->Texture_Unlock(_tex);
	return true;
}

/************************************************************************/
/* 结束绘制
/************************************************************************/
void CTextureBuffer::EndDraw()
{
	ZeroMemory(m_stRcOffset, sizeof(m_stRcOffset));
}

/************************************************************************/
/* 偏移进行写入 >0 右下偏移（更新可绘制矩形）
/************************************************************************/
void CTextureBuffer::Offset(int _nX, int _nY)
{
	//	0
	m_stRcOffset[0].top = 0;
	m_stRcOffset[0].bottom = m_stRect.y;
	m_stRcOffset[0].right = _nX;
	m_stRcOffset[0].left = 0;

	if(m_stRcOffset[0].right < m_stRcOffset->left)
	{
		int nTmp = 0;
		m_stRcOffset[0].right += m_stRect.x;
		m_stRcOffset[0].left += m_stRect.x;
		nTmp = m_stRcOffset[0].left;
		m_stRcOffset[0].left = m_stRcOffset[0].right;
		m_stRcOffset[0].right = nTmp;
	}

	//	1
	m_stRcOffset[1].left = 0;
	m_stRcOffset[1].right = m_stRect.x;
	m_stRcOffset[1].top = 0;
	m_stRcOffset[1].bottom = _nY;

	if(m_stRcOffset[1].bottom < m_stRcOffset[1].top)
	{
		int nTmp = 0;
		m_stRcOffset[1].bottom += m_stRect.x;
		m_stRcOffset[1].top += m_stRect.x;
		nTmp = m_stRcOffset[1].top;
		m_stRcOffset[1].top = m_stRcOffset[1].bottom;
		m_stRcOffset[1].bottom = nTmp;
	}

	//	偏移矩形完成 偏移数据
	DWORD* pData = s_hge->Texture_Lock(m_tex, false);
	int nMoveIndexStart = 0;
	POINT stStart;
	int nMoveIndexNext = 0;
	POINT stNext;

	if(_nX > 0)
	{
		for(int i = 0; i < m_stRect.y; ++i)
		{
			//	每行向右移
			stStart.x = 0;
			stStart.y = i;
			if(GetRealPoint(&stStart, &nMoveIndexStart))
			{
				memmove(&pData[nMoveIndexStart + _nX], &pData[nMoveIndexStart], (m_stRect.x - _nX) * sizeof(DWORD));
				memset(&pData[nMoveIndexStart], 0xFF000000, _nX * sizeof(DWORD));
			}
		}
	}
	else if(_nX < 0)
	{
		//	左移
		_nX = -_nX;
		for(int i = 0; i < m_stRect.y; ++i)
		{
			stStart.x = _nX;
			stStart.y = i;
			if(GetRealPoint(&stStart, &nMoveIndexStart))
			{
				memmove(&pData[nMoveIndexStart - _nX], &pData[nMoveIndexStart], (m_stRect.x - _nX) * sizeof(DWORD));
				memset(&pData[nMoveIndexStart - _nX + m_stRect.x - _nX], 0xFF000000, _nX * sizeof(DWORD));
			}
		}
	}

	if(_nY > 0)
	{
		//	下移
		for(int i = m_stRect.y - 1 - _nY; i >= 0; --i)
		{
			stStart.x = 0;
			stStart.y = i;
			if(GetRealPoint(&stStart, &nMoveIndexStart))
			{
				stNext.x = 0;
				stNext.y = stStart.y + _nY;
				if(GetRealPoint(&stNext, &nMoveIndexNext))
				{
					memmove(&pData[nMoveIndexNext], &pData[nMoveIndexStart], m_stRect.x * sizeof(DWORD));
				}
			}
		}
		//	清除区域
		for(int i = 0; i < _nY; ++i)
		{
			stStart.x = 0;
			stStart.y = i;
			if(GetRealPoint(&stStart, &nMoveIndexStart))
			{
				memset(&pData[nMoveIndexStart], 0xFF000000, m_stRect.x * sizeof(DWORD));
			}
		}
	}
	else if(_nY < 0)
	{
		//	上移
		_nY = -_nY;
		for(int i = _nY; i < m_stRect.y; ++i)
		{
			stStart.x = 0;
			stStart.y = i;
			if(GetRealPoint(&stStart, &nMoveIndexStart))
			{
				stNext.x = 0;
				stNext.y = stStart.y - _nY;
				if(GetRealPoint(&stNext, &nMoveIndexNext))
				{
					memmove(&pData[nMoveIndexNext], &pData[nMoveIndexStart], m_stRect.x * sizeof(DWORD));
				}
			}
		}
		//	清除区域
		for(int i = m_stRect.y - _nY; i < m_stRect.y; ++i)
		{
			stStart.x = 0;
			stStart.y = i;
			if(GetRealPoint(&stStart, &nMoveIndexStart))
			{
				memset(&pData[nMoveIndexStart], 0xFF000000, m_stRect.x * sizeof(DWORD));
			}
		}
	}

	s_hge->Texture_Unlock(m_tex);
	m_bCanDraw = true;
}

/************************************************************************/
/* 直接偏移 不更新矩形
/************************************************************************/
void CTextureBuffer::OffsetDirect(int _nX, int _nY)
{

}

/************************************************************************/
/* 返回纹理
/************************************************************************/
// HTEXTURE CTextureBuffer::GetBuffer()
// {
// 	return m_tex;
// }


//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 获取实际像素点位置(获取缓存纹理实际写入点)
/************************************************************************/
bool CTextureBuffer::GetRealPoint(const POINT* _pstSrc, int* _pnDest)
{
	if(!_pstSrc ||
		!_pnDest)
	{
		return false;
	}

	if(_pstSrc->x < 0 ||
		_pstSrc->x >= m_stRect.x ||
		_pstSrc->y < 0 ||
		_pstSrc->y >= m_stRect.y)
	{
		return false;
	}

	if(m_bIsStandardRect)
	{
		*_pnDest = _pstSrc->y * m_stRect.x + _pstSrc->x;
		return true;
	}
	else
	{
		//	不相等 显卡不支持非32倍数纹理
		if(m_dwRealHeight == 0 ||
			m_dwRealWidth == 0)
		{
			return false;
		}

		*_pnDest = _pstSrc->y * m_dwRealWidth + _pstSrc->x;
	}

	return true;
}

/************************************************************************/
/* 获取实际像素点位置(任意纹理写入点)
/************************************************************************/
bool CTextureBuffer::GetRealPoint(HTEXTURE _tex, const POINT* _pstSrc, const POINT* _pstSrcRect, int* _pnDest)
{
	if(!_pstSrc ||
		!_pnDest ||
		!_tex ||
		!_pstSrcRect)
	{
		return false;
	}

	if(_pstSrc->x < 0 ||
		_pstSrc->x >= _pstSrcRect->x ||
		_pstSrc->y < 0 ||
		_pstSrc->y >= _pstSrcRect->y)
	{
		return false;
	}

	DWORD dwRealWidth = s_hge->Texture_GetWidth(_tex);
	*_pnDest = _pstSrc->y * dwRealWidth + _pstSrc->x;
	return true;
}