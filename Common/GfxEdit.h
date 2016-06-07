//////////////////////////////////////////////////////////////////////////
//
// 编辑输入框类
//
// 作者：微妙的平衡(BOGY)
//
// 版本：For HGE v1.6
//
//////////////////////////////////////////////////////////////////////////
#ifndef MY_HGE_GFXEDIT_H
#define MY_HGE_GFXEDIT_H

class MyGfxEdit;
typedef int(_stdcall *PressEnterCallback)(void* _pEdit);
typedef int(_stdcall *OnLoseFocusCallback)(void* _pEdit);
typedef int(_stdcall *OnCharInput)(char _cKey);
typedef int(_stdcall *CanLoseFocus)(void* _pEdit);

//#include "GfxFont.h"
#include "../Common/TextPrinter.h"

#define EXTEND_FONT

class MyGfxEdit
{
public:

	///
	/// nEditWidth		编辑框宽度
	///	nFontColor		字体颜色
	/// lpFontName		字体名称
	///	nFontSize		字体大小
	/// bBold			是否粗体
	/// bItalic			是否斜体
	/// bAntialias		是否平滑
	///
	MyGfxEdit(UINT nEditWidth, DWORD nFontColor, CHAR* lpFontName = "宋体", UINT nFontSize = 12, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bAntialias = TRUE);
	~MyGfxEdit(void);

	typedef enum tagDeletePos
	{
		DP_LEFT = 0,
		DP_RIGHT
	}TDP;

public:
	void			SetFocus(); // 设置编辑框焦点，只有拥有编辑框焦点才能响应输入
	void			KillFocus(); // 清除编辑框焦点，失去编辑框焦点后将不再响应输入
	bool			IsFocused(); // 判断编辑框是否拥有焦点

	void			ClearCookie(); // 清除编辑框内容

	void			InsertCookie(const char* lpCookie); // 插入字符串到编辑框当前光标处
	const char*		GetCookie(); // 获取编辑框当前内容

	void			InsertCookieW(const wchar_t* lpCookie); // 插入字符串到编辑框当前光标处(宽字符模式)
	const wchar_t*	GetCookieW(); // 获取编辑框当前内容(宽字符模式)

	void			Updata();
	void			Render(float x, float y); //

	inline void		SetRenderBorder(bool _br)		{m_bRenderBorder = _br;}

private:
	void			OnKey( UINT nKey, UINT nRepCnt, UINT nFlags );
	void			OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	void			InsertChar(wchar_t aChar);
	void			DeleteChar(TDP aPos);
	void			OnCtrlPaste();

private:
	static const int COOKIE_SIZE = 1024;
	wchar_t			m_sCookie[COOKIE_SIZE];
	char			m_sBuffer[COOKIE_SIZE];

	FLOAT			m_nEditW;
	FLOAT			m_nEditH;

	UINT			m_nCharPos;		// 字符位置

	hgeRect			m_rcWorld;
    
	HGE*			m_pHGE;			// HGE指针
	hgeSprite*		m_pSprite;		// 光标精灵
#ifdef EXTEND_FONT
public:
	TextPrinter*    m_pFont;
	PressEnterCallback m_pEnterCallback;
	inline PressEnterCallback SetEnterKeyCallback(PressEnterCallback _pFunc)
	{
		PressEnterCallback pPre = m_pEnterCallback;
		m_pEnterCallback = _pFunc;
		return pPre;
	}
	OnLoseFocusCallback m_pOnLoseFocus;
	inline OnLoseFocusCallback SetOnLoseFocusCallback(OnLoseFocusCallback _pFunc)
	{
		OnLoseFocusCallback pPre = m_pOnLoseFocus;
		m_pOnLoseFocus = _pFunc;
		return pPre;
	}
	OnCharInput m_pOnCharInput;
	inline OnCharInput SetOnCharInputCallback(OnCharInput _pFunc)
	{
		OnCharInput pPre = m_pOnCharInput;
		m_pOnCharInput = _pFunc;
		return pPre;
	}
	CanLoseFocus m_pCanLoseFocus;
	inline CanLoseFocus SetCanLoseFocus(CanLoseFocus _pFunc)
	{
		CanLoseFocus pPre = m_pCanLoseFocus;
		m_pCanLoseFocus = _pFunc;
		return pPre;
	}
#else
	GfxFont*		m_pFont;		// 字体精灵
#endif

	static LRESULT CALLBACK MyGfxEditWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	int         m_CursorShowCount;

	//////////////////////////////////////////////////////////////////////////
	bool m_bRenderBorder;
};
#endif//MY_HGE_GFXEDIT_H
