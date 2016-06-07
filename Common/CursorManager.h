#ifndef _INC_CURSORMANAGER_
#define _INC_CURSORMANAGER_
//////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include "..\resource.h"
//////////////////////////////////////////////////////////////////////////
enum CursorType
{
	kCursor_System,
	kCursor_Default,
	kCursor_Attack,
	kCursor_SysWait,
};
//////////////////////////////////////////////////////////////////////////
class CursorManager
{
public:
	static CursorManager* GetInstance()
	{
		static CursorManager s_nIns;
		return &s_nIns;
	}

	~CursorManager()
	{

	}

protected:
	CursorManager()
	{
		m_eCursorType = kCursor_System;
		m_hCursor = NULL;
	}

public:
	bool UpdateCursor(HWND _hwnd, CursorType _eType)
	{
		if(m_eCursorType == _eType)
		{
			return true;
		}

		HCURSOR hCursor = NULL;

		if(_eType == kCursor_Default)
		{
			hCursor = ::LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR_NORMAL));

		}
		else if(_eType == kCursor_Attack)
		{
			hCursor = ::LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR_ATTACK));
		}
		else if(_eType == kCursor_SysWait)
		{
			hCursor = ::LoadCursor(NULL, IDC_WAIT);
		}
		else
		{
			hCursor = ::LoadCursor(NULL, IDC_ARROW);
		}

		if(hCursor != NULL)
		{
			::SetCursor(hCursor);
			m_eCursorType = _eType;
			m_hCursor = hCursor;

			SetClassLong(_hwnd,
				GCL_HCURSOR ,
				LONG(m_hCursor));

			return true;
		}

		return false;
	}
	bool UpdateCursor(CursorType _eType)
	{
		if(m_eCursorType == _eType)
		{
			return true;
		}

		HCURSOR hCursor = NULL;

		if(_eType == kCursor_Default)
		{
			hCursor = ::LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR_NORMAL));
			
		}
		else if(_eType == kCursor_Attack)
		{
			hCursor = ::LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR_ATTACK));
		}
		else if(_eType == kCursor_SysWait)
		{
			hCursor = ::LoadCursor(NULL, IDC_WAIT);
		}
		else
		{
			hCursor = ::LoadCursor(NULL, IDC_ARROW);
		}

		if(hCursor != NULL)
		{
			HCURSOR hRet = ::SetCursor(hCursor);
			if(NULL == hRet)
			{
				DWORD dwLastErr = GetLastError();
				int a = 0;
			}
			m_eCursorType = _eType;
			m_hCursor = hCursor;
			return true;
		}

		return false;
	}

	HCURSOR GetTheCursor()
	{
		return m_hCursor;
	}

private:
	CursorType m_eCursorType;
	HCURSOR m_hCursor;
};
//////////////////////////////////////////////////////////////////////////
#endif