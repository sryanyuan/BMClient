#include "Widget.h"
#include <algorithm>
#include <atlbase.h>

//////////////////////////////////////////////////////////////////////////

namespace sgui
{
	//////////////////////////////////////////////////////////////////////////
	HGE* Widget::s_pHGE = NULL;
	hgeSprite* Widget::s_pSpr = NULL;
	GfxFont* Widget::s_pFont = NULL;
	WORD Widget::s_wGlobalID = 0xFF00;
	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* Widget
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	Widget::Widget(Widget* _pParent)
	{
		m_wID = s_wGlobalID++;
		ZeroMemory(&m_stClientRect, sizeof(RECT));
		ZeroMemory(&m_stClickPos, sizeof(POINT));
		m_pParent = _pParent;
		m_tex = 0;

		ResetState();

		if(s_pFont == NULL)
		{
			s_pFont = new GfxFont("宋体", 12, FALSE, FALSE, FALSE);
			s_pFont->SetColor(ARGB(255, 255, 255, 255));
		}

		if(s_pHGE == NULL)
		{
			s_pHGE = hgeCreate(HGE_VERSION);
		}
	}

	Widget::~Widget()
	{
		//
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 分派
	/************************************************************************/
	DWORD Widget::Dispatcher(const Event* _pEvt)
	{
		if(_pEvt == NULL)
		{
			return NOTIFY_UNPROCESS;
		}
		else
		{
			BOOL bMouseOver = ::PtInRect(&m_stClientRect, _pEvt->stMouse);

			if(m_bDrag)
			{
				if(m_bLButtonDown)
				{
					//	drag
					if(!_pEvt->bLButtonUp)
					{
						int nOftX = _pEvt->stMouse.x - m_stClickPos.x;
						int nOftY = _pEvt->stMouse.y - m_stClickPos.y;
						MoveWidget(nOftX, nOftY);
						memcpy(&m_stClickPos, &_pEvt->stMouse, sizeof(POINT));
						if(m_pParent &&
							m_bDragNotify)
						{
							WORD wX = nOftX;
							WORD wY = nOftY;
							m_pParent->OnChildNotify(GetID(), NOTIFY_DRAG, MAKELONG(wX, wY));
						}
						return NOTIFY_DRAG;
					}
					else
					{
						m_bLButtonDown = FALSE;
						OnLButtonUp(_pEvt->stMouse);
						return NOTIFY_LBUTTONUP;
					}
				}
			}

			if(_pEvt->bLButtonUp &&
				m_bLButtonDown)
			{
				m_bLButtonDown = FALSE;
			}

			if(m_bFocus &&
				_pEvt->nKey != 0 &&
				_pEvt->nKey != HGEK_LBUTTON &&
				_pEvt->nKey != HGEK_RBUTTON &&
				_pEvt->nKey != HGEK_MBUTTON)
			{
				OnChar(_pEvt->nKey);
				return NOTIFY_KEYDOWN;
			}

			if(bMouseOver)
			{
				if(m_bMouseOver != bMouseOver)
				{
					m_bMouseOver = bMouseOver;
					OnMouseOver(_pEvt->stMouse, TRUE);
					return NOTIFY_MOUSEENTER;
				}

				if(_pEvt->bLButtonDown)
				{
					m_bLButtonDown = TRUE;
					m_bFocus = TRUE;
					if(m_pParent)
					{
						WORD wX = _pEvt->stMouse.x;
						WORD wY = _pEvt->stMouse.y;
						m_pParent->OnChildNotify(GetID(), NOTIFY_LBUTTONDOWN, MAKELONG(wX, wY));
					}
					OnLButtonDown(_pEvt->stMouse);
					m_stClickPos = _pEvt->stMouse;
					return NOTIFY_LBUTTONDOWN;
				}

				if(_pEvt->bLButtonUp)
				{
					m_bLButtonDown = FALSE;
					if(m_pParent)
					{
						WORD wX = _pEvt->stMouse.x;
						WORD wY = _pEvt->stMouse.y;
						m_pParent->OnChildNotify(GetID(), NOTIFY_LBUTTONUP, MAKELONG(wX, wY));
					}
					OnLButtonUp(_pEvt->stMouse);
					return NOTIFY_LBUTTONUP;
				}

				if(_pEvt->bRButtonDown)
				{
					m_bRButtonDown = TRUE;
					m_bFocus = TRUE;
					if(m_pParent)
					{
						WORD wX = _pEvt->stMouse.x;
						WORD wY = _pEvt->stMouse.y;
						m_pParent->OnChildNotify(GetID(), NOTIFY_RBUTTONDOWN, MAKELONG(wX, wY));
					}
					OnRButtonDown(_pEvt->stMouse);
					return NOTIFY_RBUTTONDOWN;
				}

				if(_pEvt->bRButtonUp)
				{
					m_bRButtonDown = FALSE;
					if(m_pParent)
					{
						WORD wX = _pEvt->stMouse.x;
						WORD wY = _pEvt->stMouse.y;
						m_pParent->OnChildNotify(GetID(), NOTIFY_RBUTTONUP, MAKELONG(wX, wY));
					}
					OnRButtonUp(_pEvt->stMouse);
					return NOTIFY_RBUTTONUP;
				}

				if(_pEvt->bMButtonDown)
				{
					m_bMButtonDown = TRUE;
					m_bFocus = TRUE;
					if(m_pParent)
					{
						WORD wX = _pEvt->stMouse.x;
						WORD wY = _pEvt->stMouse.y;
						m_pParent->OnChildNotify(GetID(), NOTIFY_MBUTTONDOWN, MAKELONG(wX, wY));
					}
					OnMButtonDown(_pEvt->stMouse);
					return NOTIFY_MBUTTONDOWN;
				}

				if(_pEvt->bMButtonUp)
				{
					m_bMButtonDown = FALSE;
					if(m_pParent)
					{
						WORD wX = _pEvt->stMouse.x;
						WORD wY = _pEvt->stMouse.y;
						m_pParent->OnChildNotify(GetID(), NOTIFY_MBUTTONUP, MAKELONG(wX, wY));
					}
					OnMButtonUp(_pEvt->stMouse);
					return NOTIFY_MBUTTONUP;
				}

				if(_pEvt->nScroll != 0)
				{
					if(m_pParent)
					{
						m_pParent->OnChildNotify(GetID(), NOTIFY_MBUTTONSCROLL, (DWORD)_pEvt->nScroll);
					}
					OnMButtonScroll(_pEvt->nScroll);
					return NOTIFY_MBUTTONSCROLL;
				}

				OnMouseMove(_pEvt->stMouse);
				return NOTIFY_MOUSEMOVE;
			}
			else
			{
				if(_pEvt->bLButtonDown)
				{
					m_bFocus = FALSE;
				}
				if(m_bMouseOver != bMouseOver)
				{
					m_bMouseOver = bMouseOver;
					OnMouseOver(_pEvt->stMouse, FALSE);
					return NOTIFY_MOUSELEAVE;
				}
			}

			return NOTIFY_UNPROCESS;
		}
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 移动 偏移
	/************************************************************************/
	void Widget::MoveWidget(int _nOftX, int _nOftY)
	{
		m_stClientRect.left += _nOftX;
		m_stClientRect.right += _nOftX;
		m_stClientRect.top += _nOftY;
		m_stClientRect.bottom += _nOftY;
	}

	/************************************************************************/
	/* 移动到目标位置
	/************************************************************************/
	void Widget::MoveWidgetTo(int _nDestX, int _nDestY)
	{
		int nOffsetX = _nDestX - m_stClientRect.left;
		int nOffsetY = _nDestY - m_stClientRect.top;
		MoveWidget(nOffsetX, nOffsetY);
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 重置状态
	/************************************************************************/
	void Widget::ResetState()
	{
		m_bLButtonDown = FALSE;
		m_bMouseOver = FALSE;
		m_bEnable = TRUE;
		m_bVisible = TRUE;
		m_bFocus = FALSE;
		m_bDrag = FALSE;
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 创建事件
	/************************************************************************/
	void Widget::MakeEvent(Event* _pEvt)
	{
		float x, y;
		s_pHGE->Input_GetMousePos(&x, &y);
		POINT stMouse = {x, y};
		ZeroMemory(_pEvt, sizeof(Event));
		_pEvt->stMouse.x = stMouse.x;
		_pEvt->stMouse.y = stMouse.y;
		_pEvt->bLButtonUp = s_pHGE->Input_KeyUp(HGEK_LBUTTON);
		_pEvt->bLButtonDown = s_pHGE->Input_KeyDown(HGEK_LBUTTON);
		_pEvt->bMButtonDown = s_pHGE->Input_KeyDown(HGEK_MBUTTON);
		_pEvt->bMButtonUp = s_pHGE->Input_KeyUp(HGEK_MBUTTON);
		_pEvt->bRButtonDown = s_pHGE->Input_KeyDown(HGEK_RBUTTON);
		_pEvt->bRButtonUp = s_pHGE->Input_KeyUp(HGEK_RBUTTON);
		_pEvt->nScroll = s_pHGE->Input_GetMouseWheel();
		//_pEvt->nKey = s_pHGE->Input_GetKey();
	}

	/************************************************************************/
	/* 释放
	/************************************************************************/
	void Widget::Release()
	{
		if(s_pHGE)
		{
			s_pHGE->Release();
			s_pHGE = NULL;
		}
		SAFE_DELETE(s_pFont);
		SAFE_DELETE(s_pSpr);
	}

	/************************************************************************/
	/* 计算gb2312字符串宽度
	/************************************************************************/
	DWORD Widget::CalcGb2312TextWidth(const char* _pText, DWORD _dwFontSize /* = 12 */)
	{
		DWORD dwTextWidth = 0;

		for(int i = 0; i < strlen(_pText);)
		{
			if(_pText[i] > 0)
			{
				//	非中文
				dwTextWidth += (_dwFontSize / 2);
				++i;
			}
			else
			{
				dwTextWidth += _dwFontSize;
				i += 2;
			}
		}
		return dwTextWidth;
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* WidgetBox
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	WidgetBox::WidgetBox(Widget* _pParent)
		: Widget(_pParent)
	{
		//
	}

	WidgetBox::~WidgetBox()
	{
		//
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 分派
	/************************************************************************/
	DWORD WidgetBox::Dispatcher(const Event* _pEvt)
	{
		if(_pEvt == NULL)
		{
			return NOTIFY_UNPROCESS;
		}
		else
		{
			Widget* pWidget = NULL;
			DWORD dwProcesse = NOTIFY_UNPROCESS;
			DWORD dwProcessed = NOTIFY_UNPROCESS;

			for(WIDGET_LIST::const_iterator iter = m_xWidgets.begin();
				iter != m_xWidgets.end();
				++iter)
			{
				pWidget = *iter;
				if(!pWidget->IsEnable())
				{
					dwProcesse = NOTIFY_UNPROCESS;
					if(dwProcessed == NOTIFY_UNPROCESS)
					{
						dwProcessed = dwProcesse;
					}
					continue;
				}
				if(!pWidget->IsVisible())
				{
					dwProcesse = NOTIFY_UNPROCESS;
					if(dwProcessed == NOTIFY_UNPROCESS)
					{
						dwProcessed = dwProcesse;
					}
					continue;
				}

				dwProcesse = pWidget->Dispatcher(_pEvt);
				if(dwProcessed == NOTIFY_UNPROCESS)
				{
					dwProcessed = dwProcesse;
				}

				//	处理左右中键按下弹起 不派发到下层控件
				if(dwProcesse == NOTIFY_LBUTTONDOWN ||
					/*dwProcesse == NOTIFY_LBUTTONUP ||*/
					dwProcesse == NOTIFY_RBUTTONDOWN ||
					dwProcesse == NOTIFY_RBUTTONUP ||
					dwProcesse == NOTIFY_MBUTTONDOWN ||
					dwProcesse == NOTIFY_MBUTTONUP ||
					dwProcesse == NOTIFY_MBUTTONSCROLL)
				{
					break;
				}
			}

			return dwProcessed;
		}
	}

	/************************************************************************/
	/* 绘制
	/************************************************************************/
	void WidgetBox::Render()
	{
		Widget* pWidget = NULL;

		for(WIDGET_LIST::reverse_iterator iter = m_xWidgets.rbegin();
			iter != m_xWidgets.rend();
			++iter)
		{
			pWidget = *iter;
			if(!pWidget->IsVisible())
			{
				continue;
			}

			(*iter)->Render();
		}
	}

	/************************************************************************/
	/* OnChildNotify
	/************************************************************************/
	BOOL WidgetBox::OnChildNotify(WORD _wID, DWORD _dwNot, DWORD _dwMsg)
	{
		if(_dwNot == NOTIFY_DRAG)
		{
			short sOftX = (short)LOWORD(_dwMsg);
			short sOftY = (short)HIWORD(_dwMsg);

			Widget* pSendWidget = FindChild(_wID);
			Widget* pOtherWidget = NULL;
			for(WIDGET_LIST::const_iterator iter = m_xWidgets.begin();
				iter != m_xWidgets.end();
				++iter)
			{
				pOtherWidget = *iter;
				if(pOtherWidget != pSendWidget)
				{
					pOtherWidget->MoveWidget(sOftX, sOftY);
				}
			}
		}
		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 添加子控件
	/************************************************************************/
	void WidgetBox::AddChild(Widget* _pWidget)
	{
		m_xWidgets.push_back(_pWidget);
		_pWidget->SetParent(this);
	}

	/************************************************************************/
	/* 移动到顶层
	/************************************************************************/
	void WidgetBox::BringWidgetToTop(Widget* _pWidget)
	{
		WIDGET_LIST::iterator iter = std::find(m_xWidgets.begin(), m_xWidgets.end(), _pWidget);
		if(iter != m_xWidgets.end())
		{
			m_xWidgets.erase(iter);
			m_xWidgets.push_front(_pWidget);
		}
	}

	/************************************************************************/
	/* 移动到底层
	/************************************************************************/
	void WidgetBox::BringWidgetToBottom(Widget* _pWidget)
	{
		WIDGET_LIST::iterator iter = std::find(m_xWidgets.begin(), m_xWidgets.end(), _pWidget);
		if(iter != m_xWidgets.end())
		{
			m_xWidgets.erase(iter);
			m_xWidgets.push_back(_pWidget);
		}
	}

	/************************************************************************/
	/* 寻找控件
	/************************************************************************/
	Widget* WidgetBox::FindChild(WORD _wID)
	{
		for(WIDGET_LIST::const_iterator iter = m_xWidgets.begin();
			iter != m_xWidgets.end();
			++iter)
		{
			if((*iter)->GetID() == _wID)
			{
				return *iter;
			}
		}

		return NULL;
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* WidgetLayer
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	WidgetLayer::WidgetLayer()
		: WidgetBox(NULL)
	{
		//
	}

	WidgetLayer::~WidgetLayer()
	{
		//
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* Dispatcher
	/************************************************************************/
	DWORD WidgetLayer::Dispatcher(const Event* _pEvt)
	{
		if(_pEvt == NULL)
		{
			return NOTIFY_UNPROCESS;
		}
		else
		{
			Widget* pWidget = NULL;
			DWORD dwProcesse = NOTIFY_UNPROCESS;
			DWORD dwProcessed = NOTIFY_UNPROCESS;

			for(WIDGET_LIST::const_iterator iter = m_xWidgets.begin();
				iter != m_xWidgets.end();
				++iter)
			{
				pWidget = *iter;
				if(!pWidget->IsEnable())
				{
					dwProcesse = NOTIFY_UNPROCESS;
					if(dwProcessed == NOTIFY_UNPROCESS)
					{
						dwProcessed = dwProcesse;
					}
					continue;
				}
				if(!pWidget->IsVisible())
				{
					dwProcesse = NOTIFY_UNPROCESS;
					if(dwProcessed == NOTIFY_UNPROCESS)
					{
						dwProcessed = dwProcesse;
					}
					continue;
				}

				dwProcesse = pWidget->Dispatcher(_pEvt);
				if(dwProcessed == NOTIFY_UNPROCESS)
				{
					dwProcessed = dwProcesse;
				}

				//	处理左右中键按下弹起 不派发到下层控件
				if(dwProcesse == NOTIFY_LBUTTONDOWN ||
					/*dwProcesse == NOTIFY_LBUTTONUP ||*/
					dwProcesse == NOTIFY_RBUTTONDOWN ||
					dwProcesse == NOTIFY_RBUTTONUP ||
					dwProcesse == NOTIFY_MBUTTONDOWN ||
					dwProcesse == NOTIFY_MBUTTONUP ||
					dwProcesse == NOTIFY_MBUTTONSCROLL)
				{
					break;
				}
			}

			return dwProcessed;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* Button
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////

	Button::Button(Widget* _pParent)
		: Widget(_pParent)
	{
		//
		memset(m_szText, 0, sizeof(m_szText));
		m_dwColor = 0xFF000000;
	}

	Button::~Button()
	{
		//
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 绘制
	/************************************************************************/
	void Button::Render()
	{
		if(m_stClientRect.left == m_stClientRect.right ||
			m_stClientRect.top == m_stClientRect.bottom)
		{
			return;
		}

		if(m_tex == 0)
		{
			return;
		}

		if(!m_bVisible)
		{
			return;
		}

		if(!m_bEnable)
		{
			if(!s_pSpr)
			{
				s_pSpr = new hgeSprite(m_tex,
					0,
					(m_stClientRect.bottom - m_stClientRect.top) * 3,
					m_stClientRect.right - m_stClientRect.left,
					m_stClientRect.bottom - m_stClientRect.top);
			}
			else
			{
				s_pSpr->SetTexture(m_tex);
				s_pSpr->SetTextureRect(0,
					(m_stClientRect.bottom - m_stClientRect.top) * 3,
					m_stClientRect.right - m_stClientRect.left,
					m_stClientRect.bottom - m_stClientRect.top);
			}
			s_pSpr->Render(m_stClientRect.left, m_stClientRect.top);
			goto lbl_printtext;
		}

		if(m_bLButtonDown)
		{
			if(!s_pSpr)
			{
				s_pSpr = new hgeSprite(m_tex,
					0,
					(m_stClientRect.bottom - m_stClientRect.top) * 2,
					m_stClientRect.right - m_stClientRect.left,
					m_stClientRect.bottom - m_stClientRect.top);
			}
			else
			{
				s_pSpr->SetTexture(m_tex);
				s_pSpr->SetTextureRect(0,
					(m_stClientRect.bottom - m_stClientRect.top) * 2,
					m_stClientRect.right - m_stClientRect.left,
					m_stClientRect.bottom - m_stClientRect.top);
			}
			s_pSpr->Render(m_stClientRect.left, m_stClientRect.top);
			goto lbl_printtext;
		}

		if(m_bMouseOver)
		{
			if(!s_pSpr)
			{
				s_pSpr = new hgeSprite(m_tex,
					0,
					(m_stClientRect.bottom - m_stClientRect.top),
					m_stClientRect.right - m_stClientRect.left,
					m_stClientRect.bottom - m_stClientRect.top);
			}
			else
			{
				s_pSpr->SetTexture(m_tex);
				s_pSpr->SetTextureRect(0,
					(m_stClientRect.bottom - m_stClientRect.top),
					m_stClientRect.right - m_stClientRect.left,
					m_stClientRect.bottom - m_stClientRect.top);
			}
			s_pSpr->Render(m_stClientRect.left, m_stClientRect.top);
			goto lbl_printtext;
		}
		else
		{
			if(!s_pSpr)
			{
				s_pSpr = new hgeSprite(m_tex,
					0,
					0,
					m_stClientRect.right - m_stClientRect.left,
					m_stClientRect.bottom - m_stClientRect.top);
			}
			else
			{
				s_pSpr->SetTexture(m_tex);
				s_pSpr->SetTextureRect(0,
					0,
					m_stClientRect.right - m_stClientRect.left,
					m_stClientRect.bottom - m_stClientRect.top);
			}
			s_pSpr->Render(m_stClientRect.left, m_stClientRect.top);
			goto lbl_printtext;
		}

lbl_printtext:
		if(m_szText[0] != 0)
		{
			DWORD dwPreColor = s_pFont->GetColor();
			s_pFont->SetColor(m_dwColor);
			DWORD dwLen = CalcGb2312TextWidth(m_szText);
			DWORD dwRenderX = m_stClientRect.left + GetWidth() / 2 - dwLen / 2;
			DWORD dwRenderY = m_stClientRect.top + GetHeight() / 2 - 6;
			s_pFont->Print(dwRenderX, dwRenderY,
				m_szText);
			s_pFont->SetColor(dwPreColor);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* SetText
	/************************************************************************/
	void Button::SetText(const char *_pText)
	{
		if(strlen(_pText) >= MAX_TEXT - 1)
		{
			return;
		}
		strcpy(m_szText, _pText);
	}

	/************************************************************************/
	/* SetColor
	/************************************************************************/
	void Button::SetColor(DWORD _dwColor)
	{
		m_dwColor = _dwColor;
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* ListBox
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	ListBox::ListBox(Widget* _pParent)
		: Widget(_pParent)
	{
		m_nSel = -1;
		m_wCount = 0;
		m_sLeftOffset = m_sTopOffset = 0;
	}

	ListBox::ListBox(DWORD _dwColShow, Widget* _pParent)
		: Widget(_pParent)
	{
		m_nSel = -1;
		m_nSel = -1;
		m_wCount = 0;
		m_sLeftOffset = m_sTopOffset = 0;

		m_stClientRect.right = 150;
		m_stClientRect.bottom = BORDER_OFFSET * 2 + _dwColShow * 12;
	}

	ListBox::~ListBox()
	{
		//
		if(!m_xContent.empty())
		{
			char* pText = NULL;
			for(std::list<char*>::const_iterator iter = m_xContent.begin();
				iter != m_xContent.end();
				++iter)
			{
				pText = *iter;
				delete[] pText;
			}
			m_xContent.clear();
		}
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 插入字符串
	/************************************************************************/
	BOOL ListBox::InsertString(const char* _szText)
	{
		if(strlen(_szText) > MAX_TEXT)
		{
			return FALSE;
		}

		char* pText = new char[strlen(_szText) + 1];
		strcpy(pText, _szText);
		m_xContent.push_back(pText);
		++m_wCount;

		return TRUE;
	}

	/************************************************************************/
	/* 移除字符串
	/************************************************************************/
	BOOL ListBox::RemoveString(DWORD _dwIndex)
	{
		if(_dwIndex >= m_wCount)
		{
			return FALSE;
		}

		char* pText = NULL;
		DWORD dwCount = 0;

		std::list<char*>::iterator iter = m_xContent.begin();
		for(iter;
			iter != m_xContent.end();
			++iter)
		{
			if(dwCount == _dwIndex)
			{
				break;
			}
			++dwCount;
		}

		//if(iter != m_xContent.end())
		{
			pText = *iter;
			if(pText)
			{
				delete[] pText;
			}
			m_xContent.erase(iter);
			--m_wCount;
			return TRUE;
		}

		return FALSE;
	}

	/************************************************************************/
	/* 获取选择字符串
	/************************************************************************/
	const char* ListBox::GetSelContent()
	{
		if(m_nSel == -1)
		{
			return NULL;
		}

		int nIndex = m_nSel + m_sTopOffset;
		std::list<char*>::const_iterator iter = m_xContent.begin();
		for(int i = 0; i < nIndex; ++i)
		{
			if(i == nIndex)
			{
				return *iter;
			}
			if(iter == m_xContent.end())
			{
				return NULL;
			}
			++iter;
		}
		return NULL;
	}

	/************************************************************************/
	/* 向上滚动
	/************************************************************************/
	void ListBox::ScrollUp()
	{
		if(m_nSel == -1)
		{
			return;
		}

		int nSel = m_nSel + m_sTopOffset;
		--m_nSel;
		if(m_nSel < 0)
		{
			m_nSel = 0;
			--m_sTopOffset;
			if(m_sTopOffset < 0)
			{
				m_sTopOffset = 0;
			}
		}
		int nNowSel = m_nSel + m_sTopOffset;
		if(nNowSel != nSel)
		{
			OnItemClicked(nNowSel);
			if(m_pParent)
			{
				m_pParent->OnChildNotify(GetID(), NOTIFY_LBITEMCLICK, (DWORD)nNowSel);
			}
		}
	}

	/************************************************************************/
	/* 向下滚动
	/************************************************************************/
	void ListBox::ScrollDown()
	{
		if(m_nSel == -1)
		{
			return;
		}
		WORD wCanshow = (m_stClientRect.bottom - m_stClientRect.top - BORDER_OFFSET) / 12;
		int nSel = m_nSel + m_sTopOffset;
		if(m_nSel < wCanshow - 1)
		{
			++m_nSel;
			if(m_sTopOffset + m_nSel >= m_wCount)
			{
				--m_nSel;
				return;
			}
		}
		else
		{
			++m_sTopOffset;
			if(m_sTopOffset + m_nSel >= m_wCount)
			{
				--m_sTopOffset;
				return;
			}
		}
		int nNowSel = m_nSel + m_sTopOffset;
		if(nNowSel != nSel)
		{
			OnItemClicked(nNowSel);
			if(m_pParent)
			{
				m_pParent->OnChildNotify(GetID(), NOTIFY_LBITEMCLICK, (DWORD)nNowSel);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 左键按下
	/************************************************************************/
	BOOL ListBox::OnLButtonDown(const POINT& _ptMouse)
	{
		DWORD dwTopOffset = _ptMouse.y - m_stClientRect.top;
		WORD wCanshow = (m_stClientRect.bottom - m_stClientRect.top - BORDER_OFFSET) / 12;

		if(dwTopOffset > BORDER_OFFSET &&
			dwTopOffset < m_stClientRect.bottom - BORDER_OFFSET * 2)
		{
			dwTopOffset -= BORDER_OFFSET;
			dwTopOffset /= 12;
			if(dwTopOffset < m_wCount &&
				dwTopOffset < wCanshow)
			{
				m_nSel = dwTopOffset;
				OnItemClicked(m_nSel);
				if(m_pParent)
				{
					m_pParent->OnChildNotify(GetID(), NOTIFY_LBITEMCLICK, (DWORD)m_nSel);
				}
			}
		}

		return TRUE;
	}

	/************************************************************************/
	/* 按键消息
	/************************************************************************/
	BOOL ListBox::OnChar(int _nChar)
	{
		switch(_nChar)
		{
		case HGEK_UP:
			{
				ScrollUp();
			}break;
		case HGEK_DOWN:
			{
				ScrollDown();
			}break;
		}
		return TRUE;
	}

	/************************************************************************/
	/* 绘制
	/************************************************************************/
	void ListBox::Render()
	{
		if(m_tex)
		{
			if(!s_pSpr)
			{
				s_pSpr = new hgeSprite(m_tex,
					0,
					0,
					m_stClientRect.right - m_stClientRect.left,
					m_stClientRect.bottom - m_stClientRect.top);
			}
			else
			{
				s_pSpr->SetTexture(m_tex);
				s_pSpr->SetTextureRect(0,
					0,
					m_stClientRect.right - m_stClientRect.left,
					m_stClientRect.bottom - m_stClientRect.top);
			}
			s_pSpr->Render(m_stClientRect.left, m_stClientRect.top);
		}
		else
		{
			s_pHGE->Gfx_RenderLine(m_stClientRect.left, m_stClientRect.top,
				m_stClientRect.right, m_stClientRect.top, 0xFFD2E8FF);
			s_pHGE->Gfx_RenderLine(m_stClientRect.left, m_stClientRect.top,
				m_stClientRect.left, m_stClientRect.bottom, 0xFFD2E8FF);
			s_pHGE->Gfx_RenderLine(m_stClientRect.right, m_stClientRect.top,
				m_stClientRect.right, m_stClientRect.bottom, 0xFFD2E8FF);
			s_pHGE->Gfx_RenderLine(m_stClientRect.left, m_stClientRect.bottom,
				m_stClientRect.right, m_stClientRect.bottom, 0xFFD2E8FF);

			s_pHGE->Gfx_RenderLine(m_stClientRect.left, m_stClientRect.top + 1,
				m_stClientRect.right - 1, m_stClientRect.top + 1, 0xFF9DC4ED);
			s_pHGE->Gfx_RenderLine(m_stClientRect.left + 1, m_stClientRect.top + 1,
				m_stClientRect.left + 1, m_stClientRect.bottom - 1, 0xFF9DC4ED);
			s_pHGE->Gfx_RenderLine(m_stClientRect.right - 1, m_stClientRect.top + 1,
				m_stClientRect.right - 1, m_stClientRect.bottom - 1, 0xFF9DC4ED);
			s_pHGE->Gfx_RenderLine(m_stClientRect.left + 1, m_stClientRect.bottom - 1,
				m_stClientRect.right - 1, m_stClientRect.bottom - 1, 0xFF9DC4ED);

			s_pHGE->Gfx_RenderLine(m_stClientRect.left, m_stClientRect.top + 2,
				m_stClientRect.right - 2, m_stClientRect.top + 2, 0xFF9BC3ED);
			s_pHGE->Gfx_RenderLine(m_stClientRect.left + 2, m_stClientRect.top + 2,
				m_stClientRect.left + 2, m_stClientRect.bottom - 2, 0xFF9BC3ED);
			s_pHGE->Gfx_RenderLine(m_stClientRect.right - 2, m_stClientRect.top + 2,
				m_stClientRect.right - 2, m_stClientRect.bottom - 2, 0xFF9BC3ED);
			s_pHGE->Gfx_RenderLine(m_stClientRect.left + 2, m_stClientRect.bottom - 2,
				m_stClientRect.right - 2, m_stClientRect.bottom - 2, 0xFF9BC3ED);
		}

		if(m_xContent.empty())
		{
			return;
		}

		if(m_nSel != -1)
		{
			DWORD dwPosX = m_stClientRect.left + BORDER_OFFSET;
			DWORD dwPosY = m_stClientRect.top + BORDER_OFFSET + m_nSel * 12;

			for(int i = 0; i < 12; ++i)
			{
				s_pHGE->Gfx_RenderLine(dwPosX, dwPosY + i,
					m_stClientRect.right - BORDER_OFFSET, dwPosY + i,
					ARGB(255, 0, 0, 255));
			}
		}

		WORD wCanShow = 0;
		wCanShow = (m_stClientRect.bottom - m_stClientRect.top - BORDER_OFFSET) / 12;
		char szBuffer[MAX_TEXT + 1];
		WORD wMaxOutputTextLen = (m_stClientRect.right - m_stClientRect.left - BORDER_OFFSET * 2);
		WORD wMaxOutputTexLenCalc = 0;
		WORD wMaxOutputTexLenTest = 0;
		WORD wMaxOutputTexIndex = 0;

		std::list<char*>::iterator iter = m_xContent.begin();
		for(int i = 0; i < m_sTopOffset; ++i)
		{
			++iter;
		}
		for(WORD wBegin = 0; wBegin < wCanShow; ++wBegin)
		{
			if(iter == m_xContent.end())
			{
				break;
			}
			strcpy(szBuffer, *iter);
			for(int i = 0; i < strlen(szBuffer);)
			{
				if(wMaxOutputTexLenCalc <= wMaxOutputTextLen)
				{
					wMaxOutputTexLenTest = wMaxOutputTexLenCalc;
					if(szBuffer[i] > 0)
					{
						//	不是中文
						wMaxOutputTexLenTest += 6;
						++i;
					}
					else
					{
						wMaxOutputTexLenTest += 12;
						i += 2;
					}

					if(wMaxOutputTexLenTest > wMaxOutputTextLen)
					{
						wMaxOutputTexIndex = i;
						break;
					}
					else
					{
						wMaxOutputTexLenCalc = wMaxOutputTexLenTest;
					}
				}
			}

			if(wMaxOutputTexIndex == 0)
			{
				wMaxOutputTexIndex = strlen(szBuffer);
			}
			szBuffer[wMaxOutputTexIndex] = '\0';

			if(wBegin == m_nSel)
			{
				s_pFont->SetColor(0xFFFFFFFF);
			}
			s_pFont->Render(m_stClientRect.left + BORDER_OFFSET,
				m_stClientRect.top + BORDER_OFFSET + wBegin * 12,
				CA2W(szBuffer));
			s_pFont->SetColor(0xFF000000);

			wMaxOutputTexLenCalc = 0;
			wMaxOutputTexLenTest = 0;
			wMaxOutputTexIndex = 0;
			++iter;
		}
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* ListCtrl
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	ListCtrl::ListCtrl(Widget* _pParent)
		: WidgetBox(_pParent)
	{
		m_pxButtonUp = new Button(this);
		m_pxButtonDown = new Button(this);
		m_pxList = new ListBox(this);

		AddChild(m_pxButtonDown);
		AddChild(m_pxButtonUp);
		AddChild(m_pxList);
	}

	ListCtrl::~ListCtrl()
	{
		delete m_pxList;
		delete m_pxButtonUp;
		delete m_pxButtonDown;
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 子控件消息
	/************************************************************************/
	BOOL ListCtrl::OnChildNotify(WORD _wID, DWORD _dwNot)
	{
		if(_wID == m_pxButtonUp->GetID() &&
			_dwNot == NOTIFY_LBUTTONUP)
		{
			m_pxList->OnChar(HGEK_UP);
		}
		else if(_wID == m_pxButtonDown->GetID() &&
			_dwNot == NOTIFY_LBUTTONUP)
		{
			m_pxList->OnChar(HGEK_DOWN);
		}

		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* LineEdit
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	LineEdit::LineEdit(Widget* _pParent)
	: Widget(_pParent)
	{
		ZeroMemory(m_szText, sizeof(m_szText));
	}

	LineEdit::~LineEdit()
	{
		//
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 输入
	/************************************************************************/
	BOOL LineEdit::OnChar(int _nChar)
	{
		DWORD dwInsertPos = strlen(m_szText);
		if(dwInsertPos > MAX_TEXT)
		{
			return TRUE;
		}
		
		m_szText[dwInsertPos] = (char)_nChar;
	}

	/************************************************************************/
	/* 绘制
	/************************************************************************/
	void LineEdit::Render()
	{
		s_pHGE->Gfx_RenderLine(m_stClientRect.left, m_stClientRect.top,
			m_stClientRect.right, m_stClientRect.top, ARGB(255, 225, 225, 225));
		s_pHGE->Gfx_RenderLine(m_stClientRect.left, m_stClientRect.top,
			m_stClientRect.left, m_stClientRect.bottom, ARGB(255, 225, 225, 225));
		s_pHGE->Gfx_RenderLine(m_stClientRect.right, m_stClientRect.top,
			m_stClientRect.right, m_stClientRect.bottom, ARGB(255, 225, 225, 225));
		s_pHGE->Gfx_RenderLine(m_stClientRect.left, m_stClientRect.bottom,
			m_stClientRect.right, m_stClientRect.bottom, ARGB(255, 225, 225, 225));

		s_pHGE->Gfx_RenderLine(m_stClientRect.left + 1, m_stClientRect.top + 1,
			m_stClientRect.right - 1, m_stClientRect.top + 1, ARGB(255, 200, 200, 200));
		s_pHGE->Gfx_RenderLine(m_stClientRect.left + 1, m_stClientRect.top + 1,
			m_stClientRect.left + 1, m_stClientRect.bottom - 1, ARGB(255, 200, 200, 200));
		s_pHGE->Gfx_RenderLine(m_stClientRect.right - 1, m_stClientRect.top + 1,
			m_stClientRect.right - 1, m_stClientRect.bottom - 1, ARGB(255, 200, 200, 200));
		s_pHGE->Gfx_RenderLine(m_stClientRect.left + 1, m_stClientRect.bottom - 1,
			m_stClientRect.right - 1, m_stClientRect.bottom - 1, ARGB(255, 200, 200, 200));

		s_pHGE->Gfx_RenderLine(m_stClientRect.left + 2, m_stClientRect.top + 2,
			m_stClientRect.right - 2, m_stClientRect.top + 2, ARGB(255, 175, 175, 175));
		s_pHGE->Gfx_RenderLine(m_stClientRect.left + 2, m_stClientRect.top + 2,
			m_stClientRect.left + 2, m_stClientRect.bottom - 2, ARGB(255, 175, 175, 175));
		s_pHGE->Gfx_RenderLine(m_stClientRect.right - 2, m_stClientRect.top + 2,
			m_stClientRect.right - 2, m_stClientRect.bottom - 2, ARGB(255, 175, 175, 175));
		s_pHGE->Gfx_RenderLine(m_stClientRect.left + 2, m_stClientRect.bottom - 2,
			m_stClientRect.right - 2, m_stClientRect.bottom - 2, ARGB(255, 175, 175, 175));

		s_pFont->Print(m_stClientRect.left + BORDER_OFFSET,
			m_stClientRect.top + BORDER_OFFSET,
			m_szText);
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* Image
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	Image::Image(Widget* _pParent)
		: Widget(_pParent)
	{
		//
	}

	Image::~Image()
	{
		//
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* Render()绘制
	/************************************************************************/
	void Image::Render()
	{
		if(!m_tex)
		{
			return;
		}

		if(GetWidth() == 0 ||
			GetHeight() == 0)
		{
			return;
		}

		if(!s_pSpr)
		{
			s_pSpr = new hgeSprite(m_tex, 0, 0, GetWidth(), GetHeight());
		}
		else
		{
			s_pSpr->SetTexture(m_tex);
			s_pSpr->SetTextureRect(0, 0, GetWidth(), GetHeight());
		}
		s_pSpr->Render(m_stClientRect.left, m_stClientRect.top);
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* CheckBox
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	CheckBox::CheckBox(const char* _szText, Widget* _pParent)
		: Button(_pParent)
	{
		m_bSelected = FALSE;
		SetHeight(13);
		SetWidth(15 + strlen(_szText) / 2 * 12);
	}

	CheckBox::~CheckBox()
	{
		//
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* OnLButtonUp
	/************************************************************************/
	BOOL CheckBox::OnLButtonUp(const POINT& _ptMouse)
	{
		m_bSelected = (m_bSelected == TRUE ? FALSE : TRUE);
		return TRUE;
	}

	/************************************************************************/
	/* Render
	/************************************************************************/
	void CheckBox::Render()
	{
// 		if(!m_tex)
// 		{
// 			m_tex = s_pHGE->Texture_Create(13, 104);
// 			if(!m_tex)
// 			{
// 				assert("create texture error!" == NULL);
// 			}
// 			DWORD dwWidth = 13;
// 			DWORD dwHeight = 104;
// 
// 			static const DWORD s_dwColorTable[] =
// 			{
// 				0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
// 				0xFFFFFFFF,0xFFDCDCD7,0xFF
// 			};
// 		}
	}


	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* Label
	/************************************************************************/

	//////////////////////////////////////////////////////////////////////////
	Label::Label(Widget* _pParent)
		: Widget(_pParent)
	{
		ZeroMemory(m_szText, sizeof(m_szText));
		m_dwColor = 0xFF000000;
	}

	Label::~Label()
	{
		//
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* SetText
	/************************************************************************/
	void Label::SetText(const char* _pText)
	{
		if(strlen(_pText) > MAX_PATH - 1)
		{
			OutputDebugString("Label文本长度不能超过259");
			return;
		}
		strcpy(m_szText, _pText);
		SetHeight(12);
		SetWidth(CalcGb2312TextWidth(_pText));
	}

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* Render
	/************************************************************************/
	void Label::Render()
	{
		if(strlen(m_szText) == 0)
		{
			return;
		}
		if(GetWidth() == 0 ||
			GetHeight() == 0)
		{
			return;
		}

		if(m_tex)
		{
			if(!s_pSpr)
			{
				s_pSpr = new hgeSprite(m_tex, 0, 0, GetWidth(), GetHeight());
			}
			else
			{
				s_pSpr->SetTexture(m_tex);
				s_pSpr->SetTextureRect(0, 0, GetWidth(), GetHeight());
			}
		}

		DWORD dwColor = s_pFont->GetColor();
		s_pFont->SetColor(m_dwColor);
		s_pFont->Print(m_stClientRect.left, m_stClientRect.top, m_szText);
		s_pFont->SetColor(dwColor);
	}
};