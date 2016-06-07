#include "DrawWndBuffer.h"

DrawWndBuffer::DrawWndBuffer()
{
	m_hBuffer = NULL;
	m_HGE = hgeCreate(HGE_VERSION);
	m_dwActWidth = m_dwActHeight = 0;
	m_dwHeight = m_dwWidth = 0;
	m_dwLeftOffset = m_dwTopOffset = 0;
}

DrawWndBuffer::~DrawWndBuffer()
{
	Release();
	m_HGE->Release();
}

void DrawWndBuffer::Release()
{
	if(m_hBuffer)
	{
		m_HGE->Texture_Free(m_hBuffer);
		m_hBuffer = 0;
	}
}

void DrawWndBuffer::Initialize(int _width, int _height)
{
	m_hBuffer = m_HGE->Texture_Create(_width + m_dwLeftOffset, _height + m_dwTopOffset);
	m_dwActWidth = m_HGE->Texture_GetWidth(m_hBuffer);
	m_dwActHeight = m_HGE->Texture_GetHeight(m_hBuffer);
	m_dwWidth = _width;
	m_dwHeight = _height;
}

void DrawWndBuffer::Clear()
{
	if(!m_hBuffer)
	{
		return;
	}

	DWORD* pBuffer = m_HGE->Texture_Lock(m_hBuffer, false);
	memset(pBuffer, ARGB(255, 0, 0, 0), m_dwActWidth * m_dwHeight);
	m_HGE->Texture_Unlock(m_hBuffer);
}

HTEXTURE DrawWndBuffer::GetBuffer()
{
	return m_hBuffer;
}

void DrawWndBuffer::SetTopLeftOffset(int _left, int _height)
{
	m_dwLeftOffset = _left;
	m_dwTopOffset = _height;
}





//	DRAW
void DrawWndBuffer::Draw(int _x, int _y, int _width, int _height, HTEXTURE _src)
{
	DWORD dwActSrcWidth = m_HGE->Texture_GetWidth(_src);
	DWORD dwActSrcHeight = m_HGE->Texture_GetHeight(m_hBuffer);

	DWORD* pSrcBuffer = m_HGE->Texture_Lock(_src);
	DWORD* pDestBuffer = m_HGE->Texture_Lock(m_hBuffer, false);

	DWORD dwSrcIdx = 0;
	DWORD dwDestIdx = 0;

	for(int i = _y; i < _y + _height; ++i)
	{
		for(int j = _x; j < _x + _width; ++j)
		{
			dwDestIdx = (i + m_dwTopOffset) * (m_dwWidth + m_dwLeftOffset) + (j + m_dwLeftOffset);
			dwSrcIdx = (i - _y) * dwActSrcWidth + (j - _x);

			if(dwDestIdx < m_dwActHeight * m_dwActWidth &&
				dwDestIdx >= 0 &&
				dwSrcIdx < dwActSrcWidth * dwActSrcHeight &&
				dwSrcIdx >= 0)
			{
				if(pSrcBuffer[dwSrcIdx] & 8000 == 0)
				{
					continue;
				}
				pDestBuffer[dwDestIdx] = pSrcBuffer[dwSrcIdx];
			}
		}
	}

	m_HGE->Texture_Unlock(m_hBuffer);
	m_HGE->Texture_Unlock(_src);
}