#ifndef GAMECOMMONDLG_H_
#define GAMECOMMONDLG_H_

//	ÓÎÏ·¶Ô»°¿ò

#include "../Common/GameRenderObject.h"
#include <vector>
//////////////////////////////////////////////////////////////////////////
struct CommonButton
{
	int nID;
	int nType;
	int nGroup;
	BYTE bState;
	RECT stPos;
	char szText[20];
	int nTexIndex;
	DWORD dwColor;
	DWORD dwOverColor;
	DWORD dwDownColor;
	DWORD dwCheckedColor;
	bool bVisible;
};

struct StaticTexture
{
	int nID;
	int nTextureIndex;
	RECT stPos;
	DWORD dwColor;
	bool bVisible;
};
typedef std::vector<CommonButton> COMMONBUTTONS;
typedef std::vector<StaticTexture> STATICTEXTURES;

#define CBTN_NORMAL		1
#define CBTN_LEFTARROW	2
#define CBTN_RIGHTARROW	3
#define CBTN_UPARROW	4
#define CBTN_DOWNARROW	5
#define CBTN_CHECKBOX	6
#define CBTN_OPTION		7

#define BTN_NORMAL_MASK		0x00
#define BTN_OVER_MASK		0x80
#define BTN_DOWN_MASK		0x40

#define BTN_STATE_NORMAL	0
#define BTN_STATE_OVER		1
#define BTN_STATE_DOWN		2

#define BTN_STATE_UNCHECKED	3
#define BTN_STATE_CHECKED	4

#define BTN_COLOR_NORMAL	ARGB(255, 171, 128, 25)
//////////////////////////////////////////////////////////////////////////
class GameCommonDlg : public RenderObject
{
public:
	GameCommonDlg();
	~GameCommonDlg();

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render();
	virtual void ResetStatus();

public:
	virtual bool IsCaptionBar(int _x, int _y);
	virtual void OnMoveStop(){}
	virtual void OnMove(int _nOftX, int _nOftY){}
	virtual bool OnCommonButtonClick(int _id){return false;}
	virtual void AddCommonButton(int _type, int _id, int _x, int _y, const char* _pszText, int _nTexIndex = 0, DWORD _dwColor = 0);
	virtual void AddStaticTexture(int _id, int _x, int _y, int _nTexIndex, DWORD _dwColor = 0);

public:
	void RemoveCommonButton(int _id);
	void RemoveStaticTexture(int _id);
	void MoveTo(int _x, int _y);
	void Move(int _x, int _y);
	void CenterWindow();
	inline int GetWidth()			{return m_rcClient.right - m_rcClient.left;}
	inline int GetHeight()			{return m_rcClient.bottom - m_rcClient.top;}
	inline void SetHotKey(int _nHotKey)	{m_nHotKey = _nHotKey;}
	inline int GetHotKey()			{return m_nHotKey;}
	inline bool IsESCClose()		{return m_bESCClose;}
	inline void SetESCClose(bool _b){m_bESCClose = _b;}
	inline void SetWndRect(int _nLeft, int _nTop, int _nWidth, int _nHeight)
	{
		m_rcClient.left = _nLeft;
		m_rcClient.top = _nTop;
		m_rcClient.right = m_rcClient.left + _nWidth;
		m_rcClient.bottom = m_rcClient.top + _nHeight;
	}
	inline bool IsCanDrag()			{return m_bCanDrag;}
	inline void SetCanDrag(bool _b)	{m_bCanDrag = _b;}

protected:
	bool m_bLButtonDown;
	POINT m_ptOrigin;
	bool m_bESCClose;

protected:
	RECT m_rcCloseBtn;
	BYTE m_bCloseState;
	COMMONBUTTONS m_xButtons;
	STATICTEXTURES m_xTextures;
	int m_nHotKey;
	bool m_bCanDrag;
};

class GameNewDlg : public GameCommonDlg
{
public:
	GameNewDlg();
	virtual ~GameNewDlg();

public:
	virtual void Render();
	virtual void Update(float _dt){}
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void AddCommonButton(int _type, int _id, int _x, int _y, const char* _pszText, int _nTexIndex = 0, DWORD _dwColor = 0);
	virtual bool OnCloseDialog()			{return true;}
	virtual bool IsCaptionBar(int _x, int _y);
	virtual void CloseDialog();
	virtual void ShowDialog();
	virtual bool OnShowDialog()				{return true;}

public:
	bool CalcCloseButtonPosition();
	void SetCommonButtonTexture(HTEXTURE _tex);
	void SetCommonButtonState(int _id, int _state);
	void RenderBkStyle0();
	void RenderBkStyle1();
	void RenderWindowTip();
	inline void SetWindowTitle(const char* _pszTitle)
	{
		m_xTitle = _pszTitle;
		m_nTitleWidth = GetTextWidth(m_xTitle.c_str(), 12);
	}
	inline void SetWindowInfo(const char* _pszInfo)
	{
		m_xInfo = _pszInfo;
		m_nInfoWidth = GetTextWidth(m_xInfo.c_str(), 12);
	}
	void SetWindowTip(const char* _pszTip);
	inline void SetFixed()
	{
		m_bCanMove = false;
	}
	inline void SetCustomBk()
	{
		m_bCustomBk = true;
	}
	inline void SetAlpha(DWORD _dwAlpha)
	{
		m_dwAlpha = _dwAlpha;
		UpdateAlpha();
	}
	void SetBkStype(int _nStyle);
	inline int GetBkStyle()
	{
		return m_nBkStyle;
	}

	CommonButton* GetCommonButtonData(int _id);

protected:
	void OnOptionClicked(int _nId);

protected:
	void UpdateAlpha();

protected:
	std::string m_xTitle;
	std::string m_xInfo;
	std::string m_xTip;
	int m_nTitleWidth;
	int m_nInfoWidth;
	bool m_bCanMove;
	bool m_bCustomBk;
	DWORD m_dwAlpha;
	int m_nBkStyle;
	int m_nTipLines;
	int m_nTipWidth;
};

#endif