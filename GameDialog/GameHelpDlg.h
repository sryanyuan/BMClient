#ifndef _INC_GAMEHELPDLG_
#define _INC_GAMEHELPDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include "../Common/InteractiveContext.h"
//////////////////////////////////////////////////////////////////////////
class GameHelpDlg : public GameNewDlg
{
public:
	GameHelpDlg();
	virtual ~GameHelpDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnShowDialog();

public:
	void CreateHelpDlg(int _nHelpID);
	void OnNativeCommand(DWORD _dwCmd);

private:
	InteractiveContextEx m_xCtx;
	int m_nHelpID;
};
//////////////////////////////////////////////////////////////////////////
#endif