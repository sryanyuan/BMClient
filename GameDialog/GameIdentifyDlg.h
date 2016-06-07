#ifndef _INC_GAMEIDENTIFYDLG_
#define _INC_GAMEIDENTIFYDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
#define GameIdentifyDlgMode_Identify	0
#define GameIdentifyDlgMode_Unbind		1
//////////////////////////////////////////////////////////////////////////
class GameIdentifyDlg : public GameNewDlg
{
public:
	GameIdentifyDlg();
	virtual ~GameIdentifyDlg();

public:
	virtual void Render();
	virtual bool OnShowDialog()
	{
		SetItem(NULL);
		return true;
	}
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnCommonButtonClick(int _id);

public:
	void SetDlgMode(int _nMode);
	void SetItem(ItemAttrib* _pItem);
	void OnItem();

private:
	int m_nDlgMode;
	ItemAttrib* m_pItem;
	int m_nPrice;
};
//////////////////////////////////////////////////////////////////////////
#endif