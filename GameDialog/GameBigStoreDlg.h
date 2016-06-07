#ifndef _INC_GAMEBIGSTOREDLG_
#define _INC_GAMEBIGSTOREDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameDlgBase.h"
#include "GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
class GameBigStoreDlg : public GameNewDlg
{
public:
	GameBigStoreDlg();
	virtual ~GameBigStoreDlg();

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
	ItemAttrib m_stItems[MAX_BIGSTORE_NUMBER];
};
//////////////////////////////////////////////////////////////////////////
#endif