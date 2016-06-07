#ifndef _INC_GAMESTOREDLG2_
#define _INC_GAMESTOREDLG2_
//////////////////////////////////////////////////////////////////////////
#include "GameDlgBase.h"
#include "GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
class GameStoreDlg2 : public GameNewDlg
{
public:
	GameStoreDlg2();
	virtual ~GameStoreDlg2();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnShowDialog();
	virtual bool OnCloseDialog();

public:
	void AddItem(int _id);
	void AddItem(const ItemAttrib* _pItem);
	ItemAttrib* GetItem(DWORD _dwTag);
	void ClearItem();
	void BindAllItem();

private:
	void OnItem(int _idx);
	void OnPrev();
	void OnNext();

protected:
	int m_nPageIndex;
	int m_nItemCounter;
	int m_nStoreID;
	ItemAttrib m_stItems[36];
};
//////////////////////////////////////////////////////////////////////////
#endif