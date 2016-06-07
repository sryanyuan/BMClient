#ifndef _INC_GAMESHOPDLG2_
#define _INC_GAMESHOPDLG2_
//////////////////////////////////////////////////////////////////////////
#include "GameDlgBase.h"
#include "GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
class GameShopDlg2 : public GameNewDlg
{
public:
	GameShopDlg2();
	virtual ~GameShopDlg2();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnShowDialog();
	virtual bool OnCloseDialog();

public:
	void CreateByID(GameNPC* _pNPC);
	void AddItem(int _id);
	void ClearItem();

private:
	void OnItem(int _idx);

public:
	ItemAttrib m_stItems[36];
	int m_nPrice;
	int m_nHandleID;
	int m_nAttribID;
	int m_nItemCounter;
};
//////////////////////////////////////////////////////////////////////////
#endif