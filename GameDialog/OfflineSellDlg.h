#ifndef _INC_OFFLINESELLDLG_
#define _INC_OFFLINESELLDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include "../../CommonModule/OfflineSellSystem.h"
#include "../Common/GfxEdit.h"
//////////////////////////////////////////////////////////////////////////
typedef std::vector<SellItem> SellItemVector;

struct SellItemRect
{
	//SellItem* pItem;
	int nSellItemIndex;
	RECT rcItem;

	SellItemRect()
	{
		//pItem = NULL;
		nSellItemIndex = 0;
		ZeroMemory(&rcItem, sizeof(RECT));
	}
};

typedef std::vector<SellItemRect> SellItemRectVector;
//////////////////////////////////////////////////////////////////////////
class OfflineSellFilterDlg;

class OfflineSellListDlg : public GameNewDlg
{
public:
	OfflineSellListDlg();
	virtual ~OfflineSellListDlg();

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render();
	virtual bool OnCommonButtonClick(int _id);
	virtual bool OnShowDialog();

public:
	void OnNextPage();
	void OnPrevPage();

	void ResetItems();

	void AddItems(const SellItemVector& _refItems);
	void AddItem(const SellItem& _refItem);

	void FilterItems(int _nFilterType);

protected:
	//	显示的寄售商品
	SellItemVector m_xSellItems;
	SellItemRectVector m_xSellItemsRect;
	//	所有的寄售商品 用于过滤到m_xSellItems中
	SellItemVector m_xAllSellItems;
	int m_nCurPage;
	int m_nShowItemType;
	int m_nFilterType;
	OfflineSellFilterDlg* m_pFilterDlg;
};
//////////////////////////////////////////////////////////////////////////
class OfflineSellDlg : public GameNewDlg
{
public:
	OfflineSellDlg();
	virtual ~OfflineSellDlg()
	{
		SAFE_DELETE(m_pEdit);
	}

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render();
	virtual bool OnCommonButtonClick(int _id);

	virtual bool OnShowDialog();

	inline bool IsEditInFocus()
	{
		if(m_pEdit)
		{
			return m_pEdit->IsFocused();
		}
		return false;
	}

protected:
	static int __stdcall Callback_OnCharInput(char _cKey);

protected:
	ItemAttrib m_stSellItem;
	MyGfxEdit* m_pEdit;
};
//////////////////////////////////////////////////////////////////////////
class OfflineSellFilterDlg : public GameNewDlg
{
public:
	OfflineSellFilterDlg();
	virtual ~OfflineSellFilterDlg();

public:
	virtual bool IsCaptionBar(int _x, int _y)	{return false;}
	virtual void Render();
	virtual bool OnCommonButtonClick(int _id);

public:
	inline void SetListDlg(OfflineSellListDlg* _pDlg)	{m_pListDlg = _pDlg;}
	inline void SetFilterType(int _nType);
	void PullDown(bool _bPull);
	inline bool IsPullDown()
	{
		return m_bPoolDown;
	}

protected:
	int m_nFilterType;
	bool m_bPoolDown;
	OfflineSellListDlg* m_pListDlg;
};
//////////////////////////////////////////////////////////////////////////
#endif