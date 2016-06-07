#ifndef _INC_GAMESTATUSDLG_
#define _INC_GAMESTATUSDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include "GameDlgBase.h"
#include <list>
#include "../Common/StatusDefine.h"
//////////////////////////////////////////////////////////////////////////

struct StatusInfoItem
{
	StatusInfo stInfo;
	RECT stRcItem;

	DWORD dwAlphaValue;
	bool bAlphaActivated;
	bool bAlphaDown;
	DWORD dwAlphaActivateTime;

	StatusInfoItem()
	{
		ZeroMemory(&stRcItem, sizeof(RECT));
		dwAlphaActivateTime = 0;
		dwAlphaValue = 255;
		bAlphaActivated = false;
		bAlphaDown = true;
	}
};

typedef std::list<StatusInfoItem*> StatusInfoItemList;
//////////////////////////////////////////////////////////////////////////
class GameStatusDlg : public GameCommonDlg
{
public:
	GameStatusDlg();
	virtual ~GameStatusDlg(){}

public:
	virtual void Update(float _dt);
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);

public:
	void AddStatus(int _nStatusID, DWORD _dwLastTime);

protected:
	void AlignItems();

public:
	static const char* GetStatusString(int _nStatusID);

protected:
	StatusInfoItemList m_xStatusList;
};
//////////////////////////////////////////////////////////////////////////
#endif