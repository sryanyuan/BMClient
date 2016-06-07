#ifndef CTEXTUREBUFFER_H_
#define CTEXTUREBUFFER_H_

#include <hge.h>

/************************************************************************/
/* CTextureBuffer 缓存纹理
/************************************************************************/

class CTextureBuffer
{
public:
	CTextureBuffer();
	virtual ~CTextureBuffer();

public:
	//	初始化纹理
	bool InitBuffer(DWORD _dwWidth, DWORD _dwHeight);
	//	释放纹理
	void ReleaseBuffer();
	//	绘制
	bool DrawTexture(HTEXTURE _tex, int _nDestX, int _nDestY, int _nWidth, int _nHeight);
	//	结束绘制
	void EndDraw();
	//	直接绘制
	bool DrawTextureDirect(HTEXTURE _tex, int _nDestX, int _nDestY, int _nWidth, int _nHeight);
	//	直接偏移
	void OffsetDirect(int _nX, int _nY);
	//	得到缓存纹理
	inline HTEXTURE GetBuffer()	{return m_tex;}
	//	进行偏移
	void Offset(int _nX, int _nY);
	//	获得缓存尺寸
	inline DWORD GetBufferWidth() {return m_stRect.x;}
	inline DWORD GetBufferHeight() {return m_stRect.y;}

protected:
	//	存取纹理坐标转为实际坐标
	bool GetRealPoint(const POINT* _pstSrc, int* _pnDest);
	bool GetRealPoint(HTEXTURE _tex, const POINT* _pstSrc, const POINT* _pstSrcRect, int* _pnDest);

protected:
	HTEXTURE m_tex;
	POINT m_stRect;
	bool m_bIsStandardRect;
	DWORD m_dwRealWidth;
	DWORD m_dwRealHeight;
	RECT m_stRcOffset[3];
	bool m_bCanDraw;

private:
	static HGE* s_hge;
};

#endif