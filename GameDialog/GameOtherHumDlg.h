#ifndef _INC_GAMEOTHERHUMDLG_
#define _INC_GAMEOTHERHUMDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameDlgBase.h"
#include "GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
class GameOtherHumDlg : public GameNewDlg
{
public:
	GameOtherHumDlg();
	virtual ~GameOtherHumDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnShowDialog();
	virtual bool OnCloseDialog();

public:
	inline BYTE GetSex()				{return m_bSex;}
	inline void SetSex(BYTE _bSex)		{m_bSex = _bSex;}
	//inline const std::string& GetName()	{return m_xPlayerName;}
	//inline void SetName(const std::string _xName)		{m_xPlayerName = _xName;}

	void OnItem(PLAYER_ITEM_TYPE _type);
	void ClearItems();
	void SetItem(const ItemAttrib* _pItem, int _nIndex);
	const ItemAttrib* GetItem(int _nIndex);

private:
	ItemAttrib m_stItems[PLAYER_ITEM_TOTAL];
	POINT m_ptItems[PLAYER_ITEM_TOTAL];
	BYTE m_bSex;
};

class GameOtherItemDisplayDlg : public GameObject
{
public:
	GameOtherItemDisplayDlg();
	virtual ~GameOtherItemDisplayDlg();

public:
	static GameOtherItemDisplayDlg* GetInstance()
	{
		static GameOtherItemDisplayDlg* s_pIns = NULL;

		if(NULL == s_pIns)
		{
			s_pIns = new GameOtherItemDisplayDlg;
		}

		return s_pIns;
	}

public:
	virtual void Render();

public:
	void ShowItemAttrib(const ItemAttrib* _pItem);
	inline void ShowNothing()			{m_pItemAttrib = NULL;SetVisible(false);}

protected:
	//GameOtherHumDlg* m_pOtherHumDlg;

	const ItemAttrib* m_pItemAttrib;
	SuitAttribRenderInfo m_stSuitAttribRenderInfo;
	std::string m_xSuitItemName[10];
	//RECT m_rcSuit;

	//	Item Rect
	RECT m_rcItemRect;
	int m_nNamePosX;
	ItemAttrib m_oriItem;
	DWORD m_dwItemNameColor;
	int m_nColorIndex;
};

//////////////////////////////////////////////////////////////////////////
#endif