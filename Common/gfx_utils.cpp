#include "gfx_utils.h"
#include "GfxFont.h"
#include <hgesprite.h>
#include <assert.h>
#include <stdio.h>
#include "GlobalFunction.h"
#include "../GameScene/GameResourceManager.h"
//////////////////////////////////////////////////////////////////////////
#define RECT_WIDTH(RC)	(RC.right - RC.left)
#define RECT_HEIGHT(RC)	(RC.bottom - RC.top)
//////////////////////////////////////////////////////////////////////////
//	标题栏拉伸
static bool s_bGfx_9Path_CenterStretch = true;
//////////////////////////////////////////////////////////////////////////
void Gfx_SetRenderState(Gfx_RenderState _eState, int _nParam)
{
	switch(_eState)
	{
	case kGfx_9Path_CenterStretch:
		{
			s_bGfx_9Path_CenterStretch = (_nParam != 0);
		}break;
	}
}

int Gfx_GetRenderState(Gfx_RenderState _eState)
{
	switch(_eState)
	{
	case kGfx_9Path_CenterStretch:
		{
			return s_bGfx_9Path_CenterStretch ? 1 : 0;
		}break;
	}

	return 0;
}

void Gfx_Render9Path(hgeSprite* _pSpr, HTEXTURE _tex, const RECT* _pTexRect, const RECT* _p9Rect, float _fx, float _fy, float _fw, float _fh)
{
	if(NULL == _pTexRect ||
		NULL == _p9Rect)
	{
		assert("null text rect or 9path rect" == NULL);
		return;
	}
	if(NULL == _pSpr)
	{
		assert("null sprite" == NULL);
		return;
	}

	//	计算出九宫格对应的矩形区域
	RECT rc9Path[9] = {0};

	//	left top
	rc9Path[0].left = _pTexRect->left;
	rc9Path[0].top = _pTexRect->top;
	rc9Path[0].right = _pTexRect->left + _p9Rect->left;
	rc9Path[0].bottom = _pTexRect->top + _p9Rect->top;

	//	center top
	rc9Path[1].left = _pTexRect->left + _p9Rect->left;
	rc9Path[1].top = _pTexRect->top;
	rc9Path[1].right = _pTexRect->left + _pTexRect->right - _pTexRect->left - _p9Rect->right;
	rc9Path[1].bottom = _pTexRect->top + _p9Rect->top;

	//	right top
	rc9Path[2].left = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[2].top = _pTexRect->top;
	rc9Path[2].right = _pTexRect->left + _pTexRect->right;
	rc9Path[2].bottom = _pTexRect->top + _p9Rect->top;

	//	center left
	rc9Path[3].left = _pTexRect->left;
	rc9Path[3].top = _pTexRect->top + _p9Rect->top;
	rc9Path[3].right = _pTexRect->left + _p9Rect->left;
	rc9Path[3].bottom = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;

	//	center center
	rc9Path[4].left = _pTexRect->left + _p9Rect->left;
	rc9Path[4].top = _pTexRect->top + _p9Rect->top;
	rc9Path[4].right = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[4].bottom = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;

	//	center right
	rc9Path[5].left = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[5].top = _pTexRect->top + _p9Rect->top;
	rc9Path[5].right = _pTexRect->left + _pTexRect->right;
	rc9Path[5].bottom = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;

	//	bottom left
	rc9Path[6].left = _pTexRect->left;
	rc9Path[6].top = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;
	rc9Path[6].right = _pTexRect->left + _p9Rect->left;
	rc9Path[6].bottom = _pTexRect->top + _pTexRect->bottom;
	
	//	bottom center
	rc9Path[7].left = _pTexRect->left + _p9Rect->left;
	rc9Path[7].top = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;
	rc9Path[7].right = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[7].bottom = _pTexRect->top + _pTexRect->bottom;

	//	bottom right
	rc9Path[8].left = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[8].top = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;
	rc9Path[8].right = _pTexRect->left + _pTexRect->right;
	rc9Path[8].bottom = _pTexRect->top + _pTexRect->bottom;

	//	设置不同的纹理区域来渲染
	_pSpr->SetTexture(_tex);

	//	left top
	_pSpr->SetTextureRect(rc9Path[0].left,
		rc9Path[0].top,
		RECT_WIDTH(rc9Path[0]),
		RECT_HEIGHT(rc9Path[0]));
	_pSpr->Render(_fx, _fy);

	//	center top
	_pSpr->SetTextureRect(rc9Path[1].left,
		rc9Path[1].top,
		RECT_WIDTH(rc9Path[1]),
		RECT_HEIGHT(rc9Path[1]));

	if(s_bGfx_9Path_CenterStretch)
	{
		_pSpr->RenderStretch(_fx + _p9Rect->left,
			_fy,
			_fx + _fw - _p9Rect->right,
			_fy + _p9Rect->top);
	}
	else
	{
		if(RECT_WIDTH(rc9Path[1]) > 0)
		{
			int nRepeatFullTimes = (_fw - _p9Rect->right - _p9Rect->left) / RECT_WIDTH(rc9Path[1]);
			int nRepeatRemainderPixels = (_fw - _p9Rect->right - _p9Rect->left) - nRepeatFullTimes * RECT_WIDTH(rc9Path[1]);

			//	render repeat parts
			float fRenderX = 0;
			for(int i = 0; i < nRepeatFullTimes; ++i)
			{
				fRenderX = _fx + _p9Rect->left + i * RECT_WIDTH(rc9Path[1]);
				_pSpr->Render(fRenderX, _fy);
			}

			//	render left part
			if(0 != nRepeatRemainderPixels)
			{
				fRenderX = _fx + _p9Rect->left + nRepeatFullTimes * RECT_WIDTH(rc9Path[1]);
				_pSpr->SetTextureRect(rc9Path[1].left,
					rc9Path[1].top,
					nRepeatRemainderPixels,
					RECT_HEIGHT(rc9Path[1]));
				_pSpr->Render(fRenderX, _fy);
			}
		}
	}

	//	right top
	_pSpr->SetTextureRect(rc9Path[2].left,
		rc9Path[2].top,
		RECT_WIDTH(rc9Path[2]),
		RECT_HEIGHT(rc9Path[2]));
	_pSpr->Render(_fx + _fw - _p9Rect->right,
		_fy);

	//	left center
	_pSpr->SetTextureRect(rc9Path[3].left,
		rc9Path[3].top,
		RECT_WIDTH(rc9Path[3]),
		RECT_HEIGHT(rc9Path[3]));
	_pSpr->RenderStretch(_fx,
		_fy + _p9Rect->top,
		_fx + _p9Rect->left,
		_fy + _fh - _p9Rect->bottom);

	//	center center
	_pSpr->SetTextureRect(rc9Path[4].left,
		rc9Path[4].top,
		RECT_WIDTH(rc9Path[4]),
		RECT_HEIGHT(rc9Path[4]));
	_pSpr->RenderStretch(_fx + _p9Rect->left,
		_fy + _p9Rect->top,
		_fx + _fw - _p9Rect->right,
		_fy + _fh - _p9Rect->bottom);

	//	right center
	_pSpr->SetTextureRect(rc9Path[5].left,
		rc9Path[5].top,
		RECT_WIDTH(rc9Path[5]),
		RECT_HEIGHT(rc9Path[5]));
	_pSpr->RenderStretch(_fx + _fw - _p9Rect->right,
		_fy + _p9Rect->top,
		_fx + _fw,
		_fy + _fh - _p9Rect->bottom);

	//	left bottom
	_pSpr->SetTextureRect(rc9Path[6].left,
		rc9Path[6].top,
		RECT_WIDTH(rc9Path[6]),
		RECT_HEIGHT(rc9Path[6]));
	_pSpr->Render(_fx,
		_fy + _fh - _p9Rect->bottom);

	//	center bottom
	_pSpr->SetTextureRect(rc9Path[7].left,
		rc9Path[7].top,
		RECT_WIDTH(rc9Path[7]),
		RECT_HEIGHT(rc9Path[7]));
	_pSpr->RenderStretch(_fx + _p9Rect->left,
		_fy + _fh - _p9Rect->bottom,
		_fx + _fw - _p9Rect->right,
		_fy + _fh);

	//	right bottom
	_pSpr->SetTextureRect(rc9Path[8].left,
		rc9Path[8].top,
		RECT_WIDTH(rc9Path[8]),
		RECT_HEIGHT(rc9Path[8]));
	_pSpr->Render(_fx + _fw - _p9Rect->right,
		_fy + _fh - _p9Rect->bottom);
}

HTEXTURE Gfx_GetBrightTexture(HGE* _pHge, HTEXTURE _hSource, int _nBrightAdd)
{
	if(NULL == _hSource)
	{
		return NULL;
	}

	int nTextureWidth = _pHge->Texture_GetWidth(_hSource);
	int nTextureHeight = _pHge->Texture_GetHeight(_hSource);

	if(0 == nTextureHeight ||
		0 == nTextureWidth)
	{
		return NULL;
	}

	HTEXTURE texNew = _pHge->Texture_Create(nTextureWidth, nTextureHeight);
	DWORD* pDestData = _pHge->Texture_Lock(texNew, false);
	DWORD* pSrcData = _pHge->Texture_Lock(_hSource);

	for(int nRow = 0; nRow < nTextureHeight; ++nRow)
	{
		for(int nCol = 0; nCol < nTextureWidth; ++nCol)
		{
			DWORD dwPixelIndex = nRow * nTextureWidth + nCol;
			DWORD dwPixel = pSrcData[dwPixelIndex];
			BYTE bAlpha = (dwPixel & 0xff000000) >> 24;

			if(bAlpha)
			{
				//	取出RGB值
				BYTE bR = (dwPixel & 0x00ff0000) >> 16;
				BYTE bG = (dwPixel & 0x0000ff00) >> 8;
				BYTE bB = dwPixel & 0x000000ff;

				if(0 == bR &&
					0 == bG &&
					0 == bB)
				{
					continue;
				}
				
				//	转为YUV
				float fY = 0.299f * (float)bR + 0.587f * (float)bG + 0.114f * (float)bB;
				float fU = 0.565f * ((float)bB - fY);
				float fV = 0.713f * ((float)bR - fY);

				//	提升Y值 提高亮度
				if(fY + _nBrightAdd > 0xff)
				{
					fY = 0xff;
				}
				else
				{
					fY = fY + _nBrightAdd;
				}

				//	转换为RGB
				int nR = fY + 1.403f * fV;
				if(nR > 0xff)
				{
					bR = 0xff;
				}
				else
				{
					bR = nR;
				}
				int nG = fY - 0.344f * fU - 0.714f * fV;
				if(nG > 0xff)
				{
					bG = 0xff;
				}
				else
				{
					bG = nG;
				}
				int nB = fY + 1.77f * fU;
				if(nB > 0xff)
				{
					bB = 0xff;
				}
				else
				{
					bB = nB;
				}
				pDestData[dwPixelIndex] = ARGB(bAlpha, bR, bG, bB);
			}
		}
	}

	_pHge->Texture_Unlock(_hSource);
	_pHge->Texture_Unlock(texNew);

	return texNew;
}

HTEXTURE Gfx_GetBrightTextureEx(HGE* _pHge, HTEXTURE _hSource, float _fFactor)
{
	if(NULL == _hSource)
	{
		return NULL;
	}

	int nTextureWidth = _pHge->Texture_GetWidth(_hSource);
	int nTextureHeight = _pHge->Texture_GetHeight(_hSource);

	if(0 == nTextureHeight ||
		0 == nTextureWidth)
	{
		return NULL;
	}

	HTEXTURE texNew = _pHge->Texture_Create(nTextureWidth, nTextureHeight);
	DWORD* pDestData = _pHge->Texture_Lock(texNew, false);
	DWORD* pSrcData = _pHge->Texture_Lock(_hSource);

	for(int nRow = 0; nRow < nTextureHeight; ++nRow)
	{
		for(int nCol = 0; nCol < nTextureWidth; ++nCol)
		{
			DWORD dwPixelIndex = nRow * nTextureWidth + nCol;
			DWORD dwPixel = pSrcData[dwPixelIndex];
			BYTE bAlpha = (dwPixel & 0xff000000) >> 24;

			if(bAlpha)
			{
				//	取出RGB值
				BYTE bR = (dwPixel & 0x00ff0000) >> 16;
				BYTE bG = (dwPixel & 0x0000ff00) >> 8;
				BYTE bB = dwPixel & 0x000000ff;

				if(0 == bR &&
					0 == bG &&
					0 == bB)
				{
					continue;
				}

				bR = bR * 1.3 > 0xff ? 0xff : bR * 1.3;
				bG = bG * 1.3 > 0xff ? 0xff : bG * 1.3;
				bB = bB * 1.3 > 0xff ? 0xff : bB * 1.3;

				pDestData[dwPixelIndex] = ARGB(bAlpha, bR, bG, bB);
			}
		}
	}

	_pHge->Texture_Unlock(_hSource);
	_pHge->Texture_Unlock(texNew);

	return texNew;
}

void Gfx_PrintNumberWithComma(GfxFont* _pFont, float _fx, float _fy, int _nNumber)
{
	static char s_szNumberCache[20] = {0};
	static char s_szTransformedCache[20] = {0};
	sprintf(s_szNumberCache, "%d", _nNumber);

	int nNumberLength = strlen(s_szNumberCache);
	int nNumberCount = 0;
	ZeroMemory(s_szTransformedCache, sizeof(s_szTransformedCache));
	int nInsertIndex = 0;

	if(nNumberLength == 0)
	{
		return;
	}

	for(int i = nNumberLength - 1; i >= 0; --i)
	{
		s_szTransformedCache[nInsertIndex++] = s_szNumberCache[i];
		if(s_szNumberCache[i] != '-')
		{
			++nNumberCount;
		}
		if(nNumberCount >= 3)
		{
			//	需要判定是否还有下一个字符了
			if(i > 0 &&
				s_szNumberCache[i - 1] != '-')
			{
				nNumberCount = 0;
				s_szTransformedCache[nInsertIndex++] = ',';
			}
		}
	}

	//	reverse the transformed array
	for(int i = 0; i < nInsertIndex / 2; ++i)
	{
		char cTmp = s_szTransformedCache[i];
		s_szTransformedCache[i] = s_szTransformedCache[nInsertIndex - 1 - i];
		s_szTransformedCache[nInsertIndex - 1 - i] = cTmp;
	}

	//	done
	_pFont->Print(_fx, _fy, "%s", s_szTransformedCache);
}

HTEXTURE Gfx_GetOutlineTexture(HGE* _pHge, HTEXTURE _tex, int _nSize, DWORD _dwOutlineColor)
{
	int nTextureWidth = _pHge->Texture_GetWidth(_tex);
	int nTextureHeight = _pHge->Texture_GetHeight(_tex);

	if(0 == nTextureHeight ||
		0 == nTextureWidth)
	{
		return NULL;
	}

	HTEXTURE texNew = _pHge->Texture_Create(nTextureWidth, nTextureHeight);
	DWORD* pDestData = _pHge->Texture_Lock(texNew, false);
	DWORD* pSrcData = _pHge->Texture_Lock(_tex);

	static int s_nAroundOffset[] = {
		-1,-1,
		-1,0,
		-1,1,
		0,-1,
		0,1,
		1,-1,
		1,0,
		1,1
	};
	static int s_nUpDownOffset[] = {
		-1,0,
		1,0,
		0,-1,
		0,1
	};

	for(int nRow = 0; nRow < nTextureHeight; ++nRow)
	{
		for(int nCol = 0; nCol < nTextureWidth; ++nCol)
		{
			DWORD dwPixelIndex = nRow * nTextureWidth + nCol;
			DWORD dwPixel = pSrcData[dwPixelIndex];
			BYTE bAlpha = (dwPixel & 0xff000000) >> 24;

			if(bAlpha)
			{
				//	取出RGB值
				BYTE bR = (dwPixel & 0x00ff0000) >> 16;
				BYTE bG = (dwPixel & 0x0000ff00) >> 8;
				BYTE bB = dwPixel & 0x000000ff;

				if(0 == bR &&
					0 == bG &&
					0 == bB)
				{
					continue;
				}

				int nPredX = 0;
				int nPredY = 0;
				DWORD dwPredIndex = 0;
				bool bOutlinePixel = false;

				//	是否是影子像素
				//if(bR == 16 && bG == 8 && bB == 8)
				{
					bool bShadowPixel = true;

					for(int i = 0; i < sizeof(s_nUpDownOffset) / sizeof(s_nUpDownOffset[0]) / 2; ++i)
					{
						nPredX = nCol + s_nUpDownOffset[i * 2] * _nSize;
						nPredY = nRow + s_nUpDownOffset[i * 2 + 1] * _nSize;
						dwPredIndex = nPredY * nTextureWidth + nPredX;

						if(nPredX < 0 ||
							nPredY < 0 ||
							nPredX >= nTextureWidth ||
							nPredY >= nTextureHeight)
						{
							continue;
						}

						//	not out of boundry
						if(pSrcData[dwPredIndex] != ARGB(0, 0, 0, 0))
						{
							bShadowPixel = false;
							break;
						}
					}

					if(bShadowPixel)
					{
						continue;
					}
				}

				for(int i = 0; i < sizeof(s_nAroundOffset) / sizeof(s_nAroundOffset[0]) / 2; ++i)
				{
					nPredX = nCol + s_nAroundOffset[i * 2] * _nSize;
					nPredY = nRow + s_nAroundOffset[i * 2 + 1] * _nSize;
					dwPredIndex = nPredY * nTextureWidth + nPredX;

					if(nPredX < 0 ||
						nPredY < 0 ||
						nPredX >= nTextureWidth ||
						nPredY >= nTextureHeight)
					{
						bOutlinePixel = true;
						break;
					}

					//	not out of boundry
					//if(pSrcData[dwPredIndex] == ARGB(255, 0, 0, 0))
					if(pSrcData[dwPredIndex] == ARGB(0, 0, 0, 0))
					{
						bOutlinePixel = true;
						break;
					}
				}

				if(bOutlinePixel)
				{
					pDestData[dwPixelIndex] = _dwOutlineColor;
					//AfxGetHge()->System_Log("write pixel:%d %d,%d", _tex, nCol, nRow);
				}
			}
		}
	}

	_pHge->Texture_Unlock(_tex);
	_pHge->Texture_Unlock(texNew);

	return texNew;
}

//	动态调整的
void Gfx_RenderNameFrame(hgeSprite* _pSpr, int _nTexIndex, float _fx, float _fy, int _nWidth)
{
	HTEXTURE texs[3] = {0};
	SIZE szs[3] = {0};

	for(int i = 0; i < 3; ++i)
	{
		texs[i] = GameResourceManager::GetInstance()->GetTexture(RES_CUSTOM, _nTexIndex + i);
		if(NULL == texs[i])
		{
			return;
		}
		GameResourceManager::GetInstance()->GetTextureSize(RES_CUSTOM, _nTexIndex + i, &szs[i]);
	}
	
	//	render left
	_pSpr->SetTexture(texs[0]);
	_pSpr->SetTextureRect(0, 0, szs[0].cx, szs[0].cy);
	_pSpr->Render(_fx - szs[0].cx - 2, _fy - (szs[1].cy - 12) / 2 - (szs[0].cy - szs[1].cy));

	//	center
	_pSpr->SetTexture(texs[1]);
	_pSpr->SetTextureRect(0, 0, _nWidth + 4, szs[1].cy);
	_pSpr->Render(_fx - 2, _fy - (szs[1].cy - 12) / 2);

	_pSpr->SetTexture(texs[2]);
	_pSpr->SetTextureRect(0, 0, szs[2].cx, szs[2].cy);
	_pSpr->Render(_fx + _nWidth + 2, _fy - (szs[1].cy - 12) / 2 /*+ (szs[2].cy - szs[1].cy)*/);
}

void Gfx_RenderChatBack(hgeSprite* _pSpr, int _nTexIndex, int _nFrame, float _fx, float _fy, int _nWidth, int _nHeight)
{
	HTEXTURE texs[10] = {0};
	SIZE szs[10] = {0};

	for(int i = 0; i < 10; ++i)
	{
		texs[i] = GameResourceManager::GetInstance()->GetTexture(RES_CUSTOM, _nTexIndex + i);
		if(NULL == texs[i])
		{
			return;
		}
		GameResourceManager::GetInstance()->GetTextureSize(RES_CUSTOM, _nTexIndex + i, &szs[i]);
	}

	static const int s_nHoriOffset = 0;
	static const int s_nVeriOffset = 0;

	static const int s_nArrowIndex = 0;
	static const int s_nCenterIndex = 1;
	static const int s_nRightIndex = 2;
	static const int s_nTopIndex = 3;
	static const int s_nRightTopIndex = 4;
	static const int s_nLeftTopIndex = 5;
	static const int s_nBottomIndex = 6;
	static const int s_nRightBottomIndex = 7;
	static const int s_nLeftBottomIndex = 8;
	static const int s_nLeftIndex = 9;

	//	render back
	_pSpr->SetTexture(texs[s_nCenterIndex]);
	_pSpr->SetTextureRect(0, 0,
		_nWidth + s_nHoriOffset * 2, 
		_nHeight + s_nVeriOffset * 2);
	_pSpr->Render(_fx - s_nHoriOffset,
		_fy - s_nVeriOffset);

	//	render left
	_pSpr->SetTexture(texs[s_nLeftIndex]);
	_pSpr->SetTextureRect(0, 0,
		szs[s_nLeftIndex].cx,
		_nHeight);
	_pSpr->Render(_fx - s_nHoriOffset - szs[s_nLeftIndex].cx,
		_fy - s_nVeriOffset);

	//	render right
	_pSpr->SetTexture(texs[s_nRightIndex]);
	_pSpr->SetTextureRect(0, 0,
		szs[s_nRightIndex].cx,
		_nHeight);
	_pSpr->Render(_fx + _nWidth,
		_fy - s_nVeriOffset);

	//	render top
	_pSpr->SetTexture(texs[s_nTopIndex]);
	_pSpr->SetTextureRect(0, 0,
		_nWidth,
		szs[s_nTopIndex].cy);
	_pSpr->Render(_fx,
		_fy - s_nVeriOffset - szs[s_nTopIndex].cy);

	//	render bottom
	_pSpr->SetTexture(texs[s_nBottomIndex]);
	_pSpr->SetTextureRect(0, 0,
		_nWidth,
		szs[s_nBottomIndex].cy);
	_pSpr->Render(_fx,
		_fy + _nHeight + s_nVeriOffset);

	//	render left top corner
	_pSpr->SetTexture(texs[s_nLeftTopIndex]);
	_pSpr->SetTextureRect(0, 0, szs[s_nLeftTopIndex].cx, szs[s_nLeftTopIndex].cy);
	_pSpr->Render(_fx - s_nHoriOffset - szs[s_nLeftTopIndex].cx,
		_fy - s_nVeriOffset - szs[s_nLeftTopIndex].cy);

	//	render left bottom
	_pSpr->SetTexture(texs[s_nLeftBottomIndex]);
	_pSpr->SetTextureRect(0, 0, szs[s_nLeftBottomIndex].cx, szs[s_nLeftBottomIndex].cy);
	_pSpr->Render(_fx - s_nHoriOffset - szs[s_nLeftBottomIndex].cx,
		_fy + _nHeight + s_nVeriOffset);

	//	render right top
	_pSpr->SetTexture(texs[s_nRightTopIndex]);
	_pSpr->SetTextureRect(0, 0, szs[s_nRightTopIndex].cx, szs[s_nRightTopIndex].cy);
	_pSpr->Render(_fx + _nWidth + s_nHoriOffset,
		_fy - s_nVeriOffset - szs[s_nRightTopIndex].cy);

	//	render right bottom
	_pSpr->SetTexture(texs[s_nRightBottomIndex]);
	_pSpr->SetTextureRect(0, 0, szs[s_nRightBottomIndex].cx, szs[s_nRightBottomIndex].cy);
	_pSpr->Render(_fx + _nWidth + s_nHoriOffset,
		_fy + _nHeight + s_nVeriOffset);

	//	render arrow, cover bottom
	_pSpr->SetTexture(texs[s_nArrowIndex]);
	_pSpr->SetTextureRect(0, 0, szs[s_nArrowIndex].cx, szs[s_nArrowIndex].cy);
	_pSpr->Render(_fx + _nWidth / 2 - szs[s_nArrowIndex].cx / 2,
		_fy + _nHeight + s_nVeriOffset);

	/*//	render back
	_pSpr->SetTexture(texs[s_nCenterIndex]);
	_pSpr->SetTextureRect(0, 0,
		_nWidth + s_nHoriOffset * 2 - szs[s_nLeftIndex].cx - szs[s_nRightIndex].cx, 
		_nHeight + s_nVeriOffset * 2 - szs[s_nTopIndex].cy - szs[s_nBottomIndex].cy);
	_pSpr->Render(_fx - s_nHoriOffset + szs[s_nLeftIndex].cx,
		_fy - s_nVeriOffset + szs[s_nTopIndex].cy);

	//	render left
	_pSpr->SetTexture(texs[s_nLeftIndex]);
	_pSpr->SetTextureRect(0, 0,
		szs[s_nLeftIndex].cx,
		_nHeight + s_nVeriOffset * 2 - szs[s_nTopIndex].cy - szs[s_nBottomIndex].cy);
	_pSpr->Render(_fx - s_nHoriOffset,
		_fy - s_nVeriOffset + szs[s_nTopIndex].cy);

	//	render right
	_pSpr->SetTexture(texs[s_nRightIndex]);
	_pSpr->SetTextureRect(0, 0,
		szs[s_nRightIndex].cx,
		_nHeight + s_nVeriOffset * 2 - szs[s_nTopIndex].cy - szs[s_nBottomIndex].cy);
	_pSpr->Render(_fx + _nWidth + s_nHoriOffset - szs[s_nRightIndex].cx,
		_fy - s_nVeriOffset + szs[s_nTopIndex].cy);

	//	render top
	_pSpr->SetTexture(texs[s_nTopIndex]);
	_pSpr->SetTextureRect(0, 0,
		_nWidth + s_nHoriOffset * 2 - szs[s_nLeftIndex].cx - szs[s_nRightIndex].cx,
		szs[s_nTopIndex].cy);
	_pSpr->Render(_fx - s_nHoriOffset + szs[s_nLeftIndex].cx,
		_fy - s_nVeriOffset);

	//	render bottom
	_pSpr->SetTexture(texs[s_nBottomIndex]);
	_pSpr->SetTextureRect(0, 0,
		_nWidth + s_nHoriOffset * 2 - szs[s_nLeftIndex].cx - szs[s_nRightIndex].cx,
		szs[s_nBottomIndex].cy);
	_pSpr->Render(_fx - s_nHoriOffset + szs[s_nLeftIndex].cx,
		_fy + _nHeight + s_nVeriOffset - szs[s_nBottomIndex].cy);

	//	render left top corner
	_pSpr->SetTexture(texs[s_nLeftTopIndex]);
	_pSpr->SetTextureRect(0, 0, szs[s_nLeftTopIndex].cx, szs[s_nLeftTopIndex].cy);
	_pSpr->Render(_fx - s_nHoriOffset, _fy - s_nVeriOffset);

	//	render left bottom
	_pSpr->SetTexture(texs[s_nLeftBottomIndex]);
	_pSpr->SetTextureRect(0, 0, szs[s_nLeftBottomIndex].cx, szs[s_nLeftBottomIndex].cy);
	_pSpr->Render(_fx - s_nHoriOffset, _fy + _nHeight + s_nVeriOffset - szs[s_nLeftBottomIndex].cy);

	//	render right top
	_pSpr->SetTexture(texs[s_nRightTopIndex]);
	_pSpr->SetTextureRect(0, 0, szs[s_nRightTopIndex].cx, szs[s_nRightTopIndex].cy);
	_pSpr->Render(_fx + _nWidth + s_nHoriOffset - szs[s_nRightTopIndex].cx, _fy - s_nVeriOffset);

	//	render right bottom
	_pSpr->SetTexture(texs[s_nRightBottomIndex]);
	_pSpr->SetTextureRect(0, 0, szs[s_nRightBottomIndex].cx, szs[s_nRightBottomIndex].cy);
	_pSpr->Render(_fx + _nWidth + s_nHoriOffset - szs[s_nRightBottomIndex].cx, _fy + _nHeight + s_nVeriOffset - szs[s_nRightBottomIndex].cy);

	//	render arrow, cover bottom
	_pSpr->SetTexture(texs[s_nArrowIndex]);
	_pSpr->SetTextureRect(0, 0, szs[s_nArrowIndex].cx, szs[s_nArrowIndex].cy);
	_pSpr->Render(_fx + _nWidth / 2 - szs[s_nArrowIndex].cx / 2, _fy + _nHeight + s_nVeriOffset - szs[s_nBottomIndex].cy);*/
}

static void gfx_RenderNone(hgeSprite* _pSpr, int _nObjIndex, int _nTextureIndex, HTEXTURE _tex, float _fx, float _fy, int _nWidth, int _nHeight)
{
	_pSpr->SetTexture(_tex);
	_pSpr->SetTextureRect(0, 0, _nWidth, _nHeight);
	_pSpr->Render(_fx, _fy);
}

static void gfx_RenderBright(hgeSprite* _pSpr, int _nObjIndex, int _nTextureIndex, HTEXTURE _tex, float _fx, float _fy, int _nWidth, int _nHeight)
{
	static int s_nPrevObjIndex = 0;
	static int s_nPrevTextureIndex = 0;
	static HTEXTURE texBright = 0;

	if(s_nPrevObjIndex == _nObjIndex &&
		s_nPrevTextureIndex == _nTextureIndex)
	{
		//	nothing
	}
	else
	{
		int nTexWidth = AfxGetHge()->Texture_GetWidth(_tex);
		int nTexHeight = AfxGetHge()->Texture_GetHeight(_tex);
		texBright = AfxGetHge()->Texture_Create(nTexHeight, nTexWidth);
		DWORD* _pSrcData = AfxGetHge()->Texture_Lock(_tex);
		DWORD* _pDestData = AfxGetHge()->Texture_Lock(texBright, false);

		for(int i = 0; i < nTexHeight; ++i)
		{
			for(int j = 0; j < nTexWidth; ++j)
			{
				DWORD dwColor = _pSrcData[i * nTexWidth + j];

				BYTE bA = dwColor & 0xff000000;
				BYTE bR = dwColor & 0x00ff0000;
				BYTE bG = dwColor & 0x0000ff00;
				BYTE bB = dwColor & 0x000000ff;

				bR = bR * 1.3 > 0xff ? 0xff : bR * 1.3;
				bG = bG * 1.3 > 0xff ? 0xff : bG * 1.3;
				bB = bB * 1.3 > 0xff ? 0xff : bB * 1.3;

				dwColor = ARGB(bA, bR, bG, bB);
				_pDestData[i * nTexWidth + j] = dwColor;
			}
		}

		AfxGetHge()->Texture_Unlock(texBright);
		AfxGetHge()->Texture_Unlock(_tex);
	}

	//	render the bright texture
	_pSpr->SetTexture(texBright);
	_pSpr->SetTextureRect(0, 0, _nWidth, _nHeight);
	_pSpr->Render(_fx, _fy);
}

void Gfx_RenderTextureWithEffect(hgeSprite* _pSpr, int _nObjIndex, int _nTextureIndex, HTEXTURE _tex, float _fx, float _fy, int _nWidth, int _nHeight, TextureEffectType _eType)
{
	switch(_eType)
	{
	case TextureEffect_Bright:
		{
			gfx_RenderBright(_pSpr, _nObjIndex, _nTextureIndex, _tex, _fx, _fy, _nWidth, _nHeight);
		}break;
	default:
		{
			gfx_RenderNone(_pSpr, _nObjIndex, _nTextureIndex, _tex, _fx, _fy, _nWidth, _nHeight);
		}break;
	}
}