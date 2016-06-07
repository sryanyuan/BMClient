#ifndef _INC_DELETECONFIRM_
#define _INC_DELETECONFIRM_
//////////////////////////////////////////////////////////////////////////
#include "../GameDialog/GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
class SelChrDlg;
//////////////////////////////////////////////////////////////////////////
class DeleteConfirmDlg : public GameNewDlg
{
public:
	DeleteConfirmDlg();
	virtual ~DeleteConfirmDlg();

public:
	virtual bool OnCommonButtonClick(int _id);

public:
	void SetCurSel(int _idx)
	{
		m_nCurSel = _idx;
	}
	void ExecuteDelete();
	void LoginExecuteDelete();
	inline void SetSelChrDlg(SelChrDlg* _pParent)
	{
		m_pParent = _pParent;
	}

private:
	int m_nCurSel;
	SelChrDlg* m_pParent;
};
//////////////////////////////////////////////////////////////////////////
#endif