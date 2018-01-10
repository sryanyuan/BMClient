#ifndef _INC_NEWHUMDLG_
#define _INC_NEWHUMDLG_
//////////////////////////////////////////////////////////////////////////
#include "../GameDialog/GameCommonDlg.h"
#include "../Common/GfxEdit.h"
//////////////////////////////////////////////////////////////////////////
class SelChrDlg;
//////////////////////////////////////////////////////////////////////////
class NewHumDlg : public GameNewDlg
{
public:
	NewHumDlg();
	virtual ~NewHumDlg();

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render();
	virtual bool OnShowDialog();
	virtual bool OnCommonButtonClick(int _id);

public:
	inline void SetSelChr(SelChrDlg* _pParent)
	{
		m_pParent = _pParent;
	}
	void ExecuteNewHum();
	void LoginExecuteNewHum();

private:
	SelChrDlg* m_pParent;
	MyGfxEdit* m_pEdit;

	int m_nSelJob;
	int m_nSelSex;

	RECT m_rcJob[4];
	RECT m_rcSex[2];

	DWORD m_dwHumFrame;
	float m_fHumUpdateTime;

};
//////////////////////////////////////////////////////////////////////////
#endif