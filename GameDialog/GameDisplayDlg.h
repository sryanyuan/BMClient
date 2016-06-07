#ifndef _INC_GAMEDISPLAYDIALOG_
#define _INC_GAMEDISPLAYDIALOG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
#define SHOW_NONE		0
#define SHOW_STRING		1
#define SHOW_TEXTURE	2

struct DisplayItem
{
	int nShowType;
	//	string
	std::string xText;
	DWORD dwColor;
	GfxFont* pFont;
	//	texture
	int nMgrIndex;
	int nTextureIndex;
	//	coordinate
	int nPosX;
	int nPosY;
	//	center
	bool bCenter;
	int nCenterWidth;
	int nCenterHeight;
	//	text linefeed width
	int nTextLineFeedWidth;
	int nTextKerningHeight;

	DisplayItem()
	{
		Reset();
	}

	void Reset()
	{
		nShowType = SHOW_NONE;
		dwColor = 0;
		pFont = NULL;
		nMgrIndex = 0;
		nTextureIndex = 0;
		nPosX = 0;
		nPosY = 0;
		bCenter = false;
		nCenterWidth = 0;
		nCenterHeight = 0;
		xText.clear();
		nTextKerningHeight = 0;
		nTextLineFeedWidth = 0;
	}
};

//////////////////////////////////////////////////////////////////////////
template <typename T>
class DisplayItemFactory
{
public:
	DisplayItemFactory(){}
	~DisplayItemFactory()
	{
		if(!m_xPtrPool.empty())
		{
			std::list<T*>::const_iterator begIter = m_xPtrPool.begin();
			std::list<T*>::const_iterator endIter = m_xPtrPool.end();

			for(begIter;
				begIter != endIter;
				++begIter)
			{
				T* ptr = *begIter;
				SAFE_DELETE(ptr);
			}

			m_xPtrPool.clear();
		}
	}

public:
	T* PopFreePtr()
	{
		T* pPtr = NULL;

		if(m_xPtrPool.empty())
		{
			pPtr = new T;
		}
		else
		{
			pPtr = m_xPtrPool.front();
			m_xPtrPool.pop_front();
			//ZeroMemory(pPtr, sizeof(T));
			pPtr->Reset();
		}

		return pPtr;
	}
	void PushFreePtr(T* _ptr)
	{
		m_xPtrPool.push_back(_ptr);
	}

protected:
	std::list<T*> m_xPtrPool;
};
//////////////////////////////////////////////////////////////////////////
class GameDisplayDlg : public GameNewDlg
{
public:
	GameDisplayDlg();
	virtual ~GameDisplayDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);

public:
	//	player self
	void ShowItemAttrib(const ItemAttrib* _pItem, int _nOwner = -1);
	void SetOwnerEquip(const ItemAttrib* _pItems);
	void SetOwnerEquip(const ItemAttrib* _pItem, int _nIndex);
	void ClearItems();

	void ResetItemAttirb();

	//	raw functions
	DisplayItem* AddStringItem(const char* _pszText, int _nx, int _ny, DWORD _dwColor = 0, GfxFont* _pFont = NULL);
	DisplayItem* AddTextureItem(int _nMgrIndex, int _nTextureIndex, int _nx, int _ny);

protected:
	void GenShowItems();

protected:
	DisplayItemFactory<DisplayItem> m_xItemFactory;

	std::list<DisplayItem*> m_xItemList;

	//	show item attrib
	ItemAttrib m_stItems[PLAYER_ITEM_TOTAL];
	int m_nOwner;
	ItemAttrib m_stShowItem;

	int m_nScreenWidth;
	int m_nScreenHeight;
};
//////////////////////////////////////////////////////////////////////////
#endif