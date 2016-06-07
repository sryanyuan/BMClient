#include "TextPrinter.h"
#include <assert.h>
#include <atlbase.h>

//////////////////////////////////////////////////////////////////////////
TextPrinter::TextPrinter(const char* _szFontName/* = "宋体"*/, int _nFontSize/* = 12*/)
: GfxFont(_szFontName, _nFontSize, FALSE, FALSE, FALSE)
{
	//
	memset(m_Stroke,0,sizeof(HTEXTURE)*font_count);
	m_dwStrokeColor = TEXT_ARGB_BLACK;
	m_dwStrokePixel = 1;
}

TextPrinter::~TextPrinter()
{
	//
	for (int nIdx = 0; nIdx < font_count; ++nIdx)
	{ if (m_Stroke[nIdx]) m_pHGE->Texture_Free(m_Stroke[nIdx]); }
}


//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 重写Print
/************************************************************************/
void TextPrinter::Render(float x, float y, const wchar_t* text )
{
	float offsetX = x;
	float offsetY = y;
	DWORD dwOldColor = m_pSprite->GetColor();

	while(*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			offsetX = x;
			offsetY += (m_nFontSize + m_nKerningHeight);
		}
		else
		{
			unsigned int idx = GetGlyphByCharacterStroke(*text);
			if (idx > 0)
			{
				//	先画描边纹理
				m_pSprite->SetColor(m_dwStrokeColor);
				m_pSprite->SetTexture(m_Stroke[idx]);
				m_pSprite->SetTextureRect(0, 0, m_Glyphs[idx].w + 2 * m_dwStrokePixel, m_Glyphs[idx].h + 2 * m_dwStrokePixel);
				m_pSprite->Render(offsetX - m_Glyphs[idx].x - m_dwStrokePixel, offsetY - m_Glyphs[idx].y - m_dwStrokePixel);
				m_pSprite->SetColor(dwOldColor);

				//	正常画文字
				m_pSprite->SetTexture(m_Glyphs[idx].t);
				m_pSprite->SetTextureRect(0, 0, m_Glyphs[idx].w, m_Glyphs[idx].h);
				m_pSprite->Render(offsetX - m_Glyphs[idx].x, offsetY - m_Glyphs[idx].y);
				offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
			}
			else
			{
				offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
			}
		}

		++text;
	}
}

// void TextPrinter::SetStrokeColor(DWORD _dwColor)
// {
// 	m_dwStrokeColor = _dwColor;
// }
// 
// DWORD TextPrinter::GetStrokeColor()
// {
// 	return m_dwStrokeColor;
// }


//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 获取缓存索引
/************************************************************************/
unsigned int TextPrinter::GetGlyphByCharacterStroke( wchar_t c )
{
	unsigned int idx = (unsigned int)c;
	if (NULL == (m_Glyphs[idx].t) ||
		NULL == (m_Stroke[idx]))
	{
		CacheCharacterStroke(idx,c);
	}
	return idx;
}

/************************************************************************/
/* 获取描边缓存纹理
/************************************************************************/
inline void TextPrinter::CacheCharacterStroke(unsigned int idx, wchar_t c)
{
	static const int s_nOffsetTable [] = {
		-1,-1,		0,-1,		1,-1,
		-1,0,					1,0,
		-1,1,		0,1,		1,1};
	int nIndex = 0;

	if(m_Glyphs[idx].t != NULL)
	{
		m_pHGE->Texture_Free(m_Glyphs[idx].t);
		m_Glyphs[idx].t = NULL;
	}

	if (idx < font_count && NULL == m_Glyphs[idx].t)
	{
		UINT nChar = (UINT)c;

		MAT2 mat2 = {{0,1},{0,0},{0,0},{0,1}};
		GLYPHMETRICS gm;
		DWORD nLen = ::GetGlyphOutlineW(m_hMemDC,nChar,m_nAntialias,&gm,0,NULL,&mat2);

		HTEXTURE hTex = m_pHGE->Texture_Create(gm.gmBlackBoxX,gm.gmBlackBoxY);
		HTEXTURE hStroke = m_pHGE->Texture_Create(gm.gmBlackBoxX + m_dwStrokePixel * 2,
			gm.gmBlackBoxY + m_dwStrokePixel * 2);
		if (NULL == hTex || NULL == hStroke) return;

		DWORD	dwStkHeight = m_pHGE->Texture_GetHeight(hStroke);
		//DWORD	dwOrgHeight = m_pHGE->Texture_GetHeight(hTex);
		DWORD	dwOrgWidth  = m_pHGE->Texture_GetWidth(hTex);
		DWORD	dwStkWidth	= m_pHGE->Texture_GetWidth(hStroke);

		if((signed)nLen > 0)
		{
			LPBYTE lpBuf = new BYTE[nLen];

			if (nLen == ::GetGlyphOutlineW(m_hMemDC,nChar,m_nAntialias,&gm,nLen,lpBuf,&mat2))
			{
				BYTE*	lpSrc = lpBuf;
				DWORD*	lpDst = m_pHGE->Texture_Lock(hTex,FALSE);
				DWORD*	lpStroke = m_pHGE->Texture_Lock(hStroke, FALSE);
				ZeroMemory(lpStroke, sizeof(DWORD) * dwStkWidth * dwStkHeight);
				DWORD*	lpCopy = lpStroke + m_dwStrokePixel + m_dwStrokePixel * dwStkWidth;	

				if (GGO_BITMAP == m_nAntialias)
				{
					LONG nSrcPitch = (gm.gmBlackBoxX / 32 + (gm.gmBlackBoxX % 32 == 0 ? 0 : 1)) * 4;
					LONG nDstPitch = dwOrgWidth;

					for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
					{
						for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
						{
							for(UINT k = 0; k < 8; ++k)   
							{
								UINT i = 8 * x + k;
								if (i >= gm.gmBlackBoxX)
								{
									x+=7;
									break;
								}
								lpDst[i] = ((lpSrc[x] >> (7 - k)) & 1) ? 0xFFFFFFFF : 0x0;
								assert(&lpSrc[x] - lpBuf < nLen);
							}
						}

						memcpy(lpCopy, lpDst, sizeof(DWORD) * gm.gmBlackBoxX);

						//	扩充周围

						if(y != 0)
						{
							for(int m = 0; m < gm.gmBlackBoxX; ++m)
							{
								if(lpCopy[m - dwStkWidth] == 0xFFFFFFFF)
								{
									for(int n = 0; n < 8; ++n)
									{
										nIndex = m + s_nOffsetTable[2 * n] + s_nOffsetTable[2 * n + 1] * dwStkWidth;
										if(lpCopy[nIndex - dwStkWidth] != 0xFFFFFFFF)
										{
											lpCopy[nIndex - dwStkWidth] = STROKE_COLOR;
										}
									}
								}
							}

							if(y == gm.gmBlackBoxY - 1)
							{
								//	最后一行
								for(int m = 0; m < gm.gmBlackBoxX; ++m)
								{
									if(lpCopy[m] == 0xFFFFFFFF)
									{
										for(int n = 0; n < 8; ++n)
										{
											nIndex = m + s_nOffsetTable[2 * n] + s_nOffsetTable[2 * n + 1] * dwStkWidth;
											if(lpCopy[nIndex] != 0xFFFFFFFF)
											{
												lpCopy[nIndex] = STROKE_COLOR;
											}
										}
									}
								}
							}
						}
						else if(y == 0 &&
							gm.gmBlackBoxY == 1)
						{
							//	只有一行 直接扫
							for(int m = 0; m < gm.gmBlackBoxX; ++m)
							{
								if(lpCopy[m] == 0xFFFFFFFF)
								{
									for(int n = 0; n < 8; ++n)
									{
										nIndex = m + s_nOffsetTable[2 * n] + s_nOffsetTable[2 * n + 1] * dwStkWidth;
										if(lpCopy[nIndex] != 0xFFFFFFFF)
										{
											lpCopy[nIndex] = STROKE_COLOR;
										}
									}
								}
							}
						}
						
						lpSrc += nSrcPitch;
						lpDst += nDstPitch;
						lpCopy += dwStkWidth;
					}
				}

				//	Release
				m_pHGE->Texture_Unlock(hTex);
				m_pHGE->Texture_Unlock(hStroke);
			}
					
			delete[] lpBuf;
		}
		else
		{
			// 非正常显示字符
		}

		m_Glyphs[idx].t = hTex;
		m_Glyphs[idx].w = static_cast<float>(gm.gmBlackBoxX);
		m_Glyphs[idx].h = static_cast<float>(gm.gmBlackBoxY);
		m_Glyphs[idx].x = static_cast<float>(-gm.gmptGlyphOrigin.x);
		m_Glyphs[idx].y = static_cast<float>(-m_nAscent + gm.gmptGlyphOrigin.y);
		m_Glyphs[idx].c = static_cast<float>(gm.gmCellIncX);

		m_Stroke[idx] = hStroke;
	}
}
//////////////////////////////////////////////////////////////////////////
int TextPrinter::LineFeedRenderRetLine(float x, float y, const wchar_t* text, int nFeedNumber, bool _bStroke /* = true */)
{
	int nTextLine = 0;
	float offsetX = x;
	float offsetY = y;
	DWORD dwOldColor = m_pSprite->GetColor();
	UINT nCounter = 0;
	int nWordOffsetX = 0;

	if(text[0] == 0)
	{
		nTextLine = 0;
	}
	else
	{
		nTextLine = 1;
	}

	while(*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			offsetX = x;
			offsetY += (m_nFontSize + m_nKerningHeight);

			++nTextLine;
		}
		else if(nCounter >= nFeedNumber &&
			nFeedNumber != 0)
		{
			offsetX = x;
			offsetY += (m_nFontSize + m_nKerningHeight);
			nCounter = 0;

			++nTextLine;

			continue;
		}
		else
		{
			unsigned int idx = GetGlyphByCharacterStroke(*text);
			if (idx > 0)
			{
				if(_bStroke)
				{
					//	先画描边纹理
					m_pSprite->SetColor(m_dwStrokeColor);
					m_pSprite->SetTexture(m_Stroke[idx]);
					m_pSprite->SetTextureRect(0, 0, m_Glyphs[idx].w + 2 * m_dwStrokePixel, m_Glyphs[idx].h + 2 * m_dwStrokePixel);
					m_pSprite->Render(offsetX - m_Glyphs[idx].x - m_dwStrokePixel, offsetY - m_Glyphs[idx].y - m_dwStrokePixel);
					m_pSprite->SetColor(dwOldColor);
				}

				//	正常画文字
				m_pSprite->SetTexture(m_Glyphs[idx].t);
				m_pSprite->SetTextureRect(0, 0, m_Glyphs[idx].w, m_Glyphs[idx].h);
				m_pSprite->Render(offsetX - m_Glyphs[idx].x, offsetY - m_Glyphs[idx].y);
				//offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
				nWordOffsetX = (GetWidthFromCharacter(*text) + m_nKerningWidth);
				offsetX += nWordOffsetX;
				nCounter += nWordOffsetX;
			}
			else
			{
				offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
			}
		}

		++text;
	}

	return nTextLine;
}

int TextPrinter::GetLineFeedLines(const char* _pszStr, int _nFeedNumber)
{
	int nLines = 1;

	float x = 0;
	float y = 0;
	float offsetX = x;
	float offsetY = y;
	UINT nCounter = 0;
	int nWordOffsetX = 0;

	while(*_pszStr)
	{
		if (*_pszStr == L'\n' || *_pszStr == L'\r')
		{
			offsetX = x;
			offsetY += (m_nFontSize + m_nKerningHeight);
			++nLines;
		}
		else if(nCounter >= _nFeedNumber &&
			_nFeedNumber != 0)
		{
			offsetX = x;
			offsetY += (m_nFontSize + m_nKerningHeight);
			nCounter = 0;
			++nLines;
			continue;
		}
		else
		{
			unsigned int idx = GetGlyphByCharacterStroke(*_pszStr);
			if (idx > 0)
			{
				nWordOffsetX = (GetWidthFromCharacter(*_pszStr) + m_nKerningWidth);
				offsetX += nWordOffsetX;
				nCounter += nWordOffsetX;
			}
			else
			{
				offsetX += (GetWidthFromCharacter(*_pszStr) + m_nKerningWidth);
			}
		}

		++_pszStr;
	}

	return nLines;
}

//////////////////////////////////////////////////////////////////////////
int TextPrinter::LineFeedRender(float x, float y, const wchar_t* text, int nFeedNumber, bool _bStroke /* = true */)
{
	float offsetX = x;
	float offsetY = y;
	DWORD dwOldColor = m_pSprite->GetColor();
	UINT nCounter = 0;
	int nWordOffsetX = 0;
	int nLines = 1;

	while(*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			offsetX = x;
			offsetY += (m_nFontSize + m_nKerningHeight);
			nCounter = 0;
			++nLines;
		}
		/*else if(nCounter >= nFeedNumber &&
			nFeedNumber != 0)
		{
			offsetX = x;
			offsetY += (m_nFontSize + m_nKerningHeight);
			nCounter = 0;
			++nLines;
			continue;
		}*/
		else
		{
			unsigned int idx = GetGlyphByCharacterStroke(*text);
			float fTextWidth = GetWidthFromCharacter(*text);

			if (idx > 0)
			{
				if(nFeedNumber != 0)
				{
					//	检查是否要换行了
					if(nCounter + fTextWidth > nFeedNumber)
					{
						offsetX = x;
						offsetY += (m_nFontSize + m_nKerningHeight);
						nCounter = 0;
						++nLines;
						continue;
					}
				}

				if(_bStroke)
				{
					//	先画描边纹理
					m_pSprite->SetColor(m_dwStrokeColor);
					m_pSprite->SetTexture(m_Stroke[idx]);
					m_pSprite->SetTextureRect(0, 0, m_Glyphs[idx].w + 2 * m_dwStrokePixel, m_Glyphs[idx].h + 2 * m_dwStrokePixel);
					m_pSprite->Render(offsetX - m_Glyphs[idx].x - m_dwStrokePixel, offsetY - m_Glyphs[idx].y - m_dwStrokePixel);
					m_pSprite->SetColor(dwOldColor);
				}

				//	正常画文字
				m_pSprite->SetTexture(m_Glyphs[idx].t);
				m_pSprite->SetTextureRect(0, 0, m_Glyphs[idx].w, m_Glyphs[idx].h);
				m_pSprite->Render(offsetX - m_Glyphs[idx].x, offsetY - m_Glyphs[idx].y);
				//offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
				nWordOffsetX = (fTextWidth + m_nKerningWidth);
				offsetX += nWordOffsetX;
				nCounter += nWordOffsetX;
			}
			else
			{
				offsetX += (fTextWidth + m_nKerningWidth);
			}
		}

		++text;
	}

	return nLines;
}

int TextPrinter::LineFeedRender(float x, float y, const char* text, int nFeedNumber, bool _bStroke /* = true */)
{
	return LineFeedRender(x, y, CA2W(text), nFeedNumber, _bStroke);
}

int TextPrinter::LineFeedRenderRetLine(float x, float y, const char* text, int nFeedNumber, bool _bStroke /* = true */)
{
	return LineFeedRenderRetLine(x, y, CA2W(text), nFeedNumber, _bStroke);
}

void TextPrinter::PrintWithoutStroke( float x, float y, const char *format, ... )
{
	char sBuffer[10240] = {0};
	char *lpsArg=(char*)&format+sizeof(format);
	vsprintf(sBuffer, format, lpsArg);

	GfxFont::Render(x,y,CA2W(sBuffer));
}