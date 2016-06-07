#include <atlbase.h>
//#include "stdafx.h"
#include "GfxEdit.h"


static MyGfxEdit* g_lpFocusEditPtr = NULL;

LRESULT	CALLBACK MyGfxEdit::MyGfxEditWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_KEYDOWN == nMsg || WM_KEYUP == nMsg)
	{
		MSG aMsg;
		memset(&aMsg,0,sizeof(MSG));

		aMsg.hwnd	= hWnd;
		aMsg.message= nMsg;
		aMsg.wParam	= wParam;
		aMsg.lParam	= lParam;
		aMsg.time	= GetTickCount();

		TranslateMessage(&aMsg);

		if (g_lpFocusEditPtr && WM_KEYDOWN == nMsg)
		{
			switch(wParam)
			{
			case VK_DELETE: //key delete
			case VK_LEFT: //key left
			case VK_RIGHT: //key right
			case VK_HOME: //key home
			case VK_END: //key end
				g_lpFocusEditPtr->OnKey(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
				break;
			}
		}
	}
	else if (WM_CHAR == nMsg || WM_IME_CHAR == nMsg)
	{
		if (g_lpFocusEditPtr)
		{
			g_lpFocusEditPtr->OnChar(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
			return TRUE;
		}
	}

	HANDLE hTmp = GetProp(hWnd, "MyGfxEdit_OldHgeWndProc{7500C348-317D-4ab1-A260-ABB468646F41}");
	LRESULT lRet = FALSE;
	if(hTmp)
	{
		lRet = CallWindowProc((WNDPROC)hTmp, hWnd, nMsg, wParam, lParam);
	}
	return lRet;
}

MyGfxEdit::MyGfxEdit(UINT nEditWidth, DWORD nFontColor, CHAR* lpFontName, UINT nFontSize, BOOL bBold, BOOL bItalic, BOOL bAntialias)
{
	m_pHGE = hgeCreate(HGE_VERSION);
	if (!m_pHGE) throw NULL;

	HWND hWnd = m_pHGE->System_GetState(HGE_HWND);
	if (!hWnd) throw NULL;

	if (NULL == GetProp(hWnd, "MyGfxEdit_OldHgeWndProc{7500C348-317D-4ab1-A260-ABB468646F41}"))
	{
		SetProp(hWnd, "MyGfxEdit_OldHgeWndProc{7500C348-317D-4ab1-A260-ABB468646F41}", (WNDPROC)::GetWindowLong(hWnd,GWL_WNDPROC));
		SetWindowLong(hWnd, GWL_WNDPROC, (LONG)MyGfxEditWndProc);
	}

	nEditWidth = (nEditWidth == 0 ? 1 : nEditWidth);
	nFontSize = (nFontSize == 0 ? 12 : nFontSize);

	m_nEditW = static_cast<FLOAT>(nEditWidth);
	m_nEditH = static_cast<FLOAT>(nFontSize);

	m_pSprite = new hgeSprite(NULL,0,0,1,m_nEditH);
	m_pSprite->SetColor(0xFFFF00FF);
	bAntialias = FALSE;
#ifdef EXTEND_FONT
	m_pFont = new TextPrinter;
	m_pEnterCallback = NULL;
	m_pOnLoseFocus = NULL;
	m_pOnCharInput = NULL;
	m_pCanLoseFocus = NULL;
#else
	m_pFont = new GfxFont(lpFontName,nFontSize,bBold,bItalic,bAntialias);
	if (!m_pFont) throw NULL;
	m_pFont->SetColor(nFontColor);
#endif

	m_nCharPos = 0;

	m_rcWorld.Set(0,0,0,0);

	ClearCookie();

	m_CursorShowCount = 0;

	SetRenderBorder(true);
}

MyGfxEdit::~MyGfxEdit(void)
{
	if (m_pSprite) delete m_pSprite;
	if (m_pFont) delete m_pFont;
	if (m_pHGE) m_pHGE->Release();
}

void MyGfxEdit::SetFocus()
{
	g_lpFocusEditPtr = this;
}

void MyGfxEdit::KillFocus()
{
	g_lpFocusEditPtr = NULL;
}

void MyGfxEdit::ClearCookie()
{
	memset(m_sCookie,0,sizeof(m_sCookie));
	m_nCharPos = 0;
}

bool MyGfxEdit::IsFocused()
{
	return (this == g_lpFocusEditPtr);
}

void MyGfxEdit::InsertCookie(const char* lpCookie)
{
	InsertCookieW(CA2W(lpCookie));
}

const char* MyGfxEdit::GetCookie()
{
	strcpy(m_sBuffer,CW2A(m_sCookie));
	return m_sBuffer;
}

void MyGfxEdit::InsertCookieW(const wchar_t* lpCookie)
{
	if (lpCookie)
	{
		while(*lpCookie)
		{
			InsertChar(*lpCookie++);
		}
	}
}

const wchar_t* MyGfxEdit::GetCookieW()
{
	return m_sCookie;
}

void MyGfxEdit::Updata()
{
	float mouse_x = 0;
	float mouse_y = 0;
	m_pHGE->Input_GetMousePos(&mouse_x,&mouse_y);

	if (m_pHGE->Input_GetKeyState(HGEK_LBUTTON))
	{
		if (m_rcWorld.TestPoint(mouse_x,mouse_y))
		{
			SetFocus();
		}
		else
		{
			if(m_pCanLoseFocus)
			{
				if(0 != m_pCanLoseFocus((void*)this))
				{
					return;
				}
			}
			KillFocus();
			if(m_pOnLoseFocus)
			{
				m_pOnLoseFocus((void*)this);
			}
		}
	}
}

void MyGfxEdit::Render(float x, float y)
{
	float box_x = x - 1;
	float box_y = y - 1;
	float box_w = m_nEditW + 2;
	float box_h = m_nEditH + 2;

	// 外框
	if(m_bRenderBorder)
	{
		m_pHGE->Gfx_RenderLine(box_x, box_y, box_x + box_w, box_y);
		m_pHGE->Gfx_RenderLine(box_x, box_y, box_x, box_y + box_h);
		m_pHGE->Gfx_RenderLine(box_x + box_w, box_y + box_h, box_x + box_w, box_y);
		m_pHGE->Gfx_RenderLine(box_x + box_w, box_y + box_h, box_x, box_y + box_h);
	}

	// 保存编辑框区域
	m_rcWorld.Set(x,y,x+m_nEditW,y+m_nEditH);

	float text_x = x;
	float text_y = y;
	float char_x = x;
	float char_y = y;

	if (m_nCharPos > 0)
	{
		wchar_t sTmp[COOKIE_SIZE];
		memset(sTmp,0,sizeof(sTmp));
		wcsncpy(sTmp,m_sCookie,m_nCharPos);
		SIZE szTmp = m_pFont->GetTextSize(sTmp);
		float tmp_w = static_cast<float>(szTmp.cx);

		if (tmp_w > m_nEditW)
		{
			text_x -= (tmp_w - m_nEditW);
		}

		char_x = text_x + tmp_w;
	}

	int clip_x = static_cast<int>(box_x);
	int clip_y = static_cast<int>(box_y);
	int clip_w = static_cast<int>(box_w);
	int clip_h = static_cast<int>(box_h);

	m_pHGE->Gfx_SetClipping(clip_x, clip_y, clip_w, clip_h);
	m_pFont->Render(text_x,text_y,m_sCookie);

	if (g_lpFocusEditPtr == this)
	{
		m_CursorShowCount++;
		if (m_CursorShowCount <= 20)
		{
			m_pSprite->Render(char_x,char_y);
		}
		if (m_CursorShowCount >= 2*20)
		{
			m_CursorShowCount = 0;
		}
	}

	m_pHGE->Gfx_SetClipping();
}

void MyGfxEdit::OnKey( UINT nKey, UINT nRepCnt, UINT nFlags )
{
	if (VK_DELETE == nKey) // key delete
	{
		DeleteChar(DP_RIGHT);
	}
	else if (VK_HOME == nKey)
	{
		m_nCharPos = 0;
	}
	else if (VK_END == nKey)
	{
		m_nCharPos = (UINT)wcslen(m_sCookie);
	}
	else if (VK_LEFT == nKey) // key left
	{
		if (m_nCharPos > 0)
		{
			--m_nCharPos;
		}
	}
	else if (VK_RIGHT == nKey) // key right
	{
		if (m_nCharPos < wcslen(m_sCookie))
		{
			++m_nCharPos;
		}
	}

	m_CursorShowCount = 0;
}

void MyGfxEdit::OnCtrlPaste()
{
	//	ctrl+v
	if (OpenClipboard(NULL))//打开剪贴板
	{
		if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要
		{
			HANDLE hClip;
			char* pBuf;

			//读取数据
			hClip=GetClipboardData(CF_TEXT);
			pBuf=(char*)GlobalLock(hClip);
			if(NULL != pBuf)
			{
				if(NULL == m_pOnCharInput)
				{
					InsertCookie(pBuf);
				}
				else
				{
					bool bContentValid = true;
					int nStrlen = strlen(pBuf);
					for(int i = 0; i < nStrlen; ++i)
					{
						if(0 != m_pOnCharInput(pBuf[i]))
						{
							bContentValid = false;
							break;
						}
					}
					if(bContentValid)
					{
						InsertCookie(pBuf);
					}
				}
			}
			GlobalUnlock(hClip);
			CloseClipboard();
		}
	}
}

void MyGfxEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if (VK_RETURN == nChar) // key enter
	{
#ifdef EXTEND_FONT
		if(m_pEnterCallback)
		{
			m_pEnterCallback(this);
		}
#endif
	}
	else if (VK_ESCAPE == nChar) // key enter
	{
	}
	else if (VK_TAB == nChar) // key tab
	{
		for (int i=0;i<8;++i) InsertChar(L' ');	// insert 7 space
	}
	else if (VK_BACK == nChar) // key back space
	{
		DeleteChar(DP_LEFT);
	}
	else
	{
		//	ctrl组合键
		if(nChar >= 0 &&
			nChar <= 31)
		{
			if(nChar == 22)
			{
				OnCtrlPaste();
			}
			return;
		}

		if(m_pOnCharInput)
		{
			if(0 != m_pOnCharInput(nChar))
			{
				return;
			}
		}

		if (m_nCharPos < COOKIE_SIZE)
		{
			char szImeChar[3];

			if (nChar < 128)
			{
				szImeChar[0]=(char)nChar;   
				szImeChar[1]='\0';   
			}
			else
			{
				szImeChar[0]=(char)(nChar>>8);   
				szImeChar[1]=(char)nChar;
				szImeChar[2]='\0';
			}

			InsertChar(*CA2W(szImeChar));
		}

	}
	m_CursorShowCount = 0;
}

void MyGfxEdit::InsertChar(wchar_t aChar)
{
	UINT nLen = (UINT)wcslen(m_sCookie);
	if (m_nCharPos < nLen)
	{
		while(m_nCharPos < nLen)
		{
			m_sCookie[nLen] = m_sCookie[nLen-1];
			--nLen;
		}
	}
	m_sCookie[m_nCharPos++] = aChar;
	m_CursorShowCount = 0;
}

void MyGfxEdit::DeleteChar(TDP aPos)
{
	if (DP_LEFT == aPos)
	{
		if (m_nCharPos == 0)
		{
			return;
		}
		--m_nCharPos;
	}
	else if (DP_RIGHT == aPos)
	{
		if (m_nCharPos == (UINT)wcslen(m_sCookie))
		{
			return;
		}
	}
	else
	{
		return;
	}

	UINT nIndex = m_nCharPos;
	while(m_sCookie[nIndex] != 0)
	{
		m_sCookie[nIndex] = m_sCookie[nIndex+1];
		nIndex++;
	}
	m_CursorShowCount = 0;
}