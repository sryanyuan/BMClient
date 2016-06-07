#include <hge.h>

class DrawWndBuffer
{
public:
	DrawWndBuffer();
	~DrawWndBuffer();

public:
	void Initialize(int _width, int _height);
	void SetTopLeftOffset(int _left, int _height);
	void Draw(int _x, int _y, int _width, int _height, HTEXTURE _src);
	//void Draw(int _x, int _y, int _width, int _height, BYTE* _src);
	HTEXTURE GetBuffer();
	void Clear();
	void Release();

private:
	HTEXTURE m_hBuffer;
	DWORD m_dwWidth;
	DWORD m_dwHeight;

	DWORD m_dwActWidth;
	DWORD m_dwActHeight;

	DWORD m_dwLeftOffset;
	DWORD m_dwTopOffset;

	HGE* m_HGE;
};