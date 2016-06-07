#ifndef _INC_GAMEDONATEDLG_
#define _INC_GAMEDONATEDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include "../Common/InteractiveContext.h"
//////////////////////////////////////////////////////////////////////////
class GameDonateDlg : public GameNewDlg
{
public:
	GameDonateDlg();
	virtual ~GameDonateDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnShowDialog();

public:
	void CreateDonateDlg(int _nDlgID);
	void OnNativeCommand(DWORD _dwCmd);

private:
	InteractiveContextEx m_xCtx;
	int m_nHelpID;
};
//////////////////////////////////////////////////////////////////////////
#endif