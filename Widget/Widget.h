#ifndef S__WIDGET_H
#define S__WIDGET_H

#include <hgesprite.h>
#include <Windows.h>
#include <list>
#include <assert.h>

#include "../Common/GfxFont.h"
/************************************************************************/
/* Simple GUI
/* 简单的GUI控件 适用于游戏窗口 可以实现窗口消息遮挡效果
/* 2013.01.18
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
#define NOTIFY_UNPROCESS		0x00000000
#define NOTIFY_LBUTTONDOWN		0x00000001
#define NOTIFY_LBUTTONUP		0x00000002

#define NOTIFY_MOUSEENTER		0x00000003
#define NOTIFY_MOUSELEAVE		0x00000004
#define NOTIFY_MOUSEMOVE		0x00000005

#define NOTIFY_DRAG				0x00000006

#define NOTIFY_RBUTTONDOWN		0x00000007
#define NOTIFY_RBUTTONUP		0x00000008

#define NOTIFY_MBUTTONDOWN		0x00000009
#define NOTIFY_MBUTTONUP		0x0000000A
#define NOTIFY_MBUTTONSCROLL	0x0000000B

#define NOTIFY_KEYDOWN			0x0000000C

#define NOTIFY_LBITEMCLICK		0x00008000

#define BORDER_OFFSET			3

#define SAFE_DELETE(p)			{if(p){delete p;p = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{if(p){delete[] p;p = NULL;}}

#define MAX_TEXT			50
#define MAX_LINE			50
//////////////////////////////////////////////////////////////////////////

namespace sgui
{
	/************************************************************************/
	/* 消息结构
	/************************************************************************/
	struct Event
	{
		POINT stMouse;
		BOOL bLButtonDown;
		BOOL bLButtonUp;
		BOOL bRButtonDown;
		BOOL bRButtonUp;
		BOOL bMButtonDown;
		BOOL bMButtonUp;
		int nScroll;
		int nKey;
	};
	/************************************************************************/
	/* 抽象控件
	/************************************************************************/
	class Widget
	{
	public:
		Widget(Widget* _pParent);
		virtual ~Widget();

	public:
		virtual DWORD Dispatcher(const Event* _pEvt);
		virtual BOOL LoadScript()							{return FALSE;}
		virtual void Render()								= 0;

	public:
		/*
		*	消息处理
		*/
		virtual BOOL OnMouseOver(const POINT& _ptMouse, BOOL _bIsOver)		{return TRUE;}
		virtual BOOL OnMouseMove(const POINT& _ptMouse)						{return TRUE;}
		virtual BOOL OnLButtonDown(const POINT& _ptMouse)					{return TRUE;}
		virtual BOOL OnLButtonUp(const POINT& _ptMouse)						{return TRUE;}
		virtual BOOL OnRButtonDown(const POINT& _ptMouse)					{return TRUE;}
		virtual BOOL OnRButtonUp(const POINT& _ptMouse)						{return TRUE;}
		virtual BOOL OnMButtonDown(const POINT& _ptMouse)					{return TRUE;}
		virtual BOOL OnMButtonUp(const POINT& _ptMouse)						{return TRUE;}
		virtual BOOL OnMButtonScroll(int _nScroll)							{return TRUE;}
		virtual BOOL OnChar(int _nChar)										{return TRUE;}

		virtual BOOL OnChildNotify(WORD _wID, DWORD _dwNot, DWORD _dwMsg)	{return TRUE;}

	public:
		inline void SetTexture(HTEXTURE _tex)				{m_tex = _tex;}
		inline void SetRect(const RECT& _rcRect)			{m_stClientRect = _rcRect;}

		inline void SetID(WORD _wID)						{if(_wID >= 0xFF00) assert("User id must small than 0xFF00" == NULL);m_wID = _wID;}
		inline WORD GetID()									{return m_wID;}

		inline void SetEnable(BOOL _bEnable)				{m_bEnable = _bEnable;}
		inline BOOL IsEnable()								{return m_bEnable;}

		inline void SetVisible(BOOL _bVis)					{m_bVisible = _bVis;}
		inline BOOL IsVisible()								{return m_bVisible;}

		inline void SetParent(Widget* _pWidget)				{m_pParent = _pWidget;}
		inline Widget* GetParent()							{return m_pParent;}

		inline void SetFocus(BOOL _bFocus)					{m_bFocus = _bFocus;}
		inline BOOL GetFocus()								{return m_bFocus;}

		inline DWORD GetWidth()								{return m_stClientRect.right - m_stClientRect.left;}
		inline void SetWidth(DWORD _dwWidth)				{m_stClientRect.right += (_dwWidth - GetWidth());}

		inline DWORD GetHeight()							{return m_stClientRect.bottom - m_stClientRect.top;}
		inline void SetHeight(DWORD _dwHeight)				{m_stClientRect.bottom += (_dwHeight - GetHeight());}

		inline BOOL CanDrag()								{return m_bDrag;}
		inline void SetCanDrag(BOOL _bDrag, BOOL _bNotify = FALSE)					{m_bDrag = _bDrag;m_bDragNotify = _bNotify;}

		void		MoveWidget(int _nOftX, int _nOftY);
		void		MoveWidgetTo(int _nDestX, int _nDestY);

	private:
		void ResetState();

	public:
		static void MakeEvent(Event* _pEvt);
		static DWORD CalcGb2312TextWidth(const char* _pText, DWORD _dwFontSize = 12);
		static void Release();

	protected:
		RECT	m_stClientRect;
		POINT	m_stClickPos;
		Widget* m_pParent;
		HTEXTURE m_tex;
		//	状态
		BOOL	m_bMouseOver;
		BOOL	m_bLButtonDown;
		BOOL	m_bRButtonDown;
		BOOL	m_bMButtonDown;
		BOOL	m_bFocus;
		BOOL	m_bEnable;
		BOOL	m_bVisible;
		BOOL	m_bDrag;
		BOOL	m_bDragNotify;

	protected:
		static HGE* s_pHGE;
		static hgeSprite* s_pSpr;
		static GfxFont* s_pFont;
		static WORD s_wGlobalID;

	private:
		WORD	m_wID;
	};

	typedef std::list<Widget*> WIDGET_LIST;

	/************************************************************************/
	/* 控件盒子
	/************************************************************************/
	class WidgetBox : public Widget
	{
	public:
		WidgetBox(Widget* _pParent);
		virtual ~WidgetBox();

	public:
		virtual DWORD Dispatcher(const Event* _pEvt);
		virtual void Render();

		virtual BOOL OnChildNotify(WORD _wID, DWORD _dwNot, DWORD _dwMsg);

	public:
		void AddChild(Widget* _pWidget);
		void BringWidgetToTop(Widget* _pWidget);
		void BringWidgetToBottom(Widget* _pWidget);
		Widget* FindChild(WORD _wID);

	protected:
		WIDGET_LIST m_xWidgets;
	};

	/************************************************************************/
	/* 控件层
	/************************************************************************/
	class WidgetLayer : public WidgetBox
	{
	public:
		WidgetLayer();
		virtual ~WidgetLayer();

	public:
		virtual DWORD Dispatcher(const Event* _pEvt);
	};

	/************************************************************************/
	/* 按钮
	/************************************************************************/
	class Button : public Widget
	{
	public:
		Button(Widget* _pParent);
		virtual ~Button();

	public:
		virtual void Render();

	public:
		void SetText(const char* _pText);
		inline const char* GetText()				{return m_szText;}

		void SetColor(DWORD _dwColor);
		inline DWORD GetColor()						{return m_dwColor;}

	protected:
		char m_szText[MAX_TEXT];
		DWORD m_dwColor;
	};

	/************************************************************************/
	/* 列表框
	/************************************************************************/

	class ListBox : public Widget
	{
	public:
		ListBox(Widget* _pParent);
		ListBox(DWORD _dwColShow, Widget* _pParent);
		virtual ~ListBox();

	public:
		virtual void Render();

	public:
		virtual BOOL OnLButtonDown(const POINT& _ptMouse);
		virtual BOOL OnChar(int _nChar);

		virtual BOOL OnItemClicked(int _nItemIndex)				{return TRUE;}

	public:
		inline WORD GetCount()									{return m_wCount;}
		inline int GetCurSel()									{if(m_nSel == -1) return -1;return m_nSel + m_sTopOffset;}

	public:
		BOOL InsertString(const char* _szText);
		BOOL RemoveString(DWORD _dwIndex);
		const char* GetSelContent();
		void ScrollUp();
		void ScrollDown();

	protected:
		int m_nSel;
		WORD m_wCount;
		std::list<char*> m_xContent;

		short m_sTopOffset;
		short m_sLeftOffset;
	};


	/************************************************************************/
	/* 增强列表框
	/************************************************************************/
	class ListCtrl : public WidgetBox
	{
	public:
		ListCtrl(Widget* _pParent);
		virtual ~ListCtrl();

	public:
		virtual BOOL OnChildNotify(WORD _wID, DWORD _dwNot);

	public:
		inline ListBox* GetListBox()										{return m_pxList;}
		inline Button* GetUpButton()										{return m_pxButtonUp;}
		inline Button* GetDownButton()										{return m_pxButtonDown;}

	private:
		Button* m_pxButtonUp;
		Button* m_pxButtonDown;
		ListBox* m_pxList;
	};


	/************************************************************************/
	/* 复选框
	/************************************************************************/
	 class CheckBox : public Button
	 {
	 public:
	 	CheckBox(const char* _szText, Widget* _pParent);
	 	virtual ~CheckBox();
	 
	 public:
	 	virtual void Render();

	 public:
		virtual BOOL OnLButtonUp(const POINT& _ptMouse);

	 protected:
		 BOOL m_bSelected;
	 };

	/************************************************************************/
	/* 输入框
	/************************************************************************/
	class LineEdit : public Widget
	{
	public:
		LineEdit(Widget* _pParent);
		virtual ~LineEdit();

	public:
		virtual void Render();
		virtual BOOL OnChar(int _nChar);

	private:
		char m_szText[MAX_TEXT + 1];
	};


	/************************************************************************/
	/* 图片
	/************************************************************************/
	class Image : public Widget
	{
	public:
		Image(Widget* _pParent);
		virtual ~Image();

	public:
		virtual void Render();
	};

	/************************************************************************/
	/* 标签
	/************************************************************************/
	class Label : public Widget
	{
	public:
		Label(Widget* _pParent);
		virtual ~Label();

	public:
		virtual void Render();

	public:
		void SetText(const char* _pText);
		inline const char* GetText()			{return m_szText;}
		void SetColor(DWORD _dwCol)				{m_dwColor = _dwCol;}
		inline DWORD GetColor()					{return m_dwColor;}

	protected:
		char m_szText[MAX_PATH];
		DWORD m_dwColor;
	};

};

#endif