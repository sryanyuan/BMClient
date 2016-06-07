#ifndef _INC_OLSHOPDLG_
#define _INC_OLSHOPDLG_
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include "../../CommonModule/OfflineSellSystem.h"
#include "../Common/GfxEdit.h"
//////////////////////////////////////////////////////////////////////////
struct OlShopItem
{
	ItemAttrib stAttrib;
};

typedef std::vector<OlShopItem> OlShopItemVector;

struct OlShopItemRect
{
	int nSellItemIndex;
	RECT rcItem;

	OlShopItemRect()
	{
		//pItem = NULL;
		nSellItemIndex = 0;
		ZeroMemory(&rcItem, sizeof(RECT));
	}
};

typedef std::vector<OlShopItemRect> OlShopItemRectVector;
//////////////////////////////////////////////////////////////////////////
class OlShopDlg : public GameNewDlg
{
public:
	OlShopDlg();
	virtual ~OlShopDlg();

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render();
	virtual bool OnCommonButtonClick(int _id);
	virtual bool OnShowDialog();

public:
	void OnNextPage();
	void OnPrevPage();

	void ResetItems()
	{
		m_xOlShopItems.clear();
		m_xOlShopItemsRect.clear();
	}

	void AddItem(int _nItemID);
	void AddItems(const int* _pItemIDArray, int _nCount);

protected:
	OlShopItemVector m_xOlShopItems;
	OlShopItemRectVector m_xOlShopItemsRect;
	int m_nCurPage;
	int m_nShowItemType;
};
//////////////////////////////////////////////////////////////////////////
#endif