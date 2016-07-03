#ifndef _INC_GFXUTILS_
#define _INC_GFXUTILS_
//////////////////////////////////////////////////////////////////////////
//	include
#include <Windows.h>
#include <hge.h>
//////////////////////////////////////////////////////////////////////////
//	forward declaration
class hgeSprite;
class GfxFont;
//////////////////////////////////////////////////////////////////////////
//	statement
enum Gfx_RenderState
{
	kGfx_9Path_CenterStretch,
};

enum TextureEffectType
{
	TexutreEffect_None,
	TextureEffect_Bright,
	TexutreEffect_Grey,
	TextureEffect_Green,
	TextureEffect_Blue,
};

void Gfx_SetRenderState(Gfx_RenderState _eState, int _nParam);
int Gfx_GetRenderState(Gfx_RenderState _eState);
void Gfx_Render9Path(hgeSprite* _pSpr, HTEXTURE _tex, const RECT* _pTexRect, const RECT* _p9Rect, float _fx, float _fy, float _fw, float _fh);
HTEXTURE Gfx_GetBrightTexture(HGE* _pHge, HTEXTURE _hSource, int _nBrightAdd);
HTEXTURE Gfx_GetBrightTextureEx(HGE* _pHge, HTEXTURE _hSource, float _fFactor);
void Gfx_PrintNumberWithComma(GfxFont* _pFont, float _fx, float _fy, int _nNumber);
HTEXTURE Gfx_GetOutlineTexture(HGE* _pHge, HTEXTURE _tex, int _nSize, DWORD _dwOutlineColor);

void Gfx_RenderNameFrame(hgeSprite* _pSpr, int _nTexIndex, float _fx, float _fy, int _nWidth);
void Gfx_RenderChatBack(hgeSprite* _pSpr, int _nTexIndex, int _nFrame, float _fx, float _fy, int _nWidth, int _nHeight);

void Gfx_RenderTextureWithEffect(hgeSprite* _pSpr, int _nObjIndex, int _nTextureIndex, HTEXTURE _tex, float _fx, float _fy, int _nWidth, int _nHeight, TextureEffectType _eType);
//////////////////////////////////////////////////////////////////////////
#endif