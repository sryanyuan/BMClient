#ifndef TEXT_PRINTER_H_
#define TEXT_PRINTER_H_

#include "GfxFont.h"

#define STROKE_COLOR		0xFFFFFFFE
#define TEXT_ARGB_WHITE		0xFFFFFFFF
#define TEXT_ARGB_BLACK		0xFF000000
#define TEXT_ARGB_YELLOW	0xFFFFFF00
#define TEXT_ARGB_RED		0xFFFF0000
#define TEXT_ARGB_GREEN		0xFF00FF00
#define TEXT_ARGB_BLUE		0xFF0000FF
/************************************************************************/
/* 支持字体描边
/************************************************************************/
class TextPrinter : public GfxFont
{
public:
	TextPrinter(const char* _szFontName = "宋体", int _nFontSize = 12);
	virtual ~TextPrinter();

public:
	virtual void Render(float x, float y, const wchar_t* text );
	int LineFeedRender(float x, float y, const wchar_t* text, int nFeedNumber, bool _bStroke = true);
	int LineFeedRender(float x, float y, const char* text, int nFeedNumber, bool _bStroke = true);
	int LineFeedRenderRetLine(float x, float y, const wchar_t* text, int nFeedNumber, bool _bStroke = true);
	int LineFeedRenderRetLine(float x, float y, const char* text, int nFeedNumber, bool _bStroke = true);
	void PrintWithoutStroke( float x, float y, const char *format, ... );

	inline DWORD GetStrokeColor() {return m_dwStrokeColor;}
	inline void  SetStrokeColor(DWORD _dwColor)	{m_dwStrokeColor = _dwColor;}

	int GetLineFeedLines(const char* _pszStr, int _nFeedNumber);

private:
	inline void		CacheCharacterStroke(unsigned int idx, wchar_t c);
	unsigned int	GetGlyphByCharacterStroke( wchar_t c );

private:
	HTEXTURE			m_Stroke[font_count];
	DWORD				m_dwStrokeColor;
	DWORD				m_dwStrokePixel;
};

#endif