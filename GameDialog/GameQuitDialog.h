#ifndef _INC_GAMEQUITDIALOG
#define _INC_GAMEQUITDIALOG
//////////////////////////////////////////////////////////////////////////
#include "GameDlgBase.h"
//////////////////////////////////////////////////////////////////////////
class GameQuitDlg : public GameNewDlg
{
public:
	enum QUIT_DLG_BTNID
	{
		QDB_QUIT,
		QDB_SAVE,
		QDB_OK
	};

public:
	GameQuitDlg();
	virtual ~GameQuitDlg(){}

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnCommonButtonClick(int _id);

public:
	void CreateSaveDlg();
	void CreateQuitDlg();
	void CreateMsgDlg(const char* _pszMsg);
};
//////////////////////////////////////////////////////////////////////////
#endif