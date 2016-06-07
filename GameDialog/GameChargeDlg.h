#ifndef _INC_GAMECHARGEDLG_
#define _INC_GAMECHARGEDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include "../Common/GfxEdit.h"
//////////////////////////////////////////////////////////////////////////
enum ChargePageType
{
	kChargePage_Charge,
	kChargePage_EnterOrderId,
};
//////////////////////////////////////////////////////////////////////////
class GameChargeDlg : public GameNewDlg
{
public:
	GameChargeDlg();
	virtual ~GameChargeDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnCommonButtonClick(int _id);
	virtual bool OnShowDialog();

public:
	void ResetData();

	void RenderPageCharge();
	void ProcPageCharge(const POINT& _mp);

	void RenderPageOrderId();
	void ProcPageOrderId(const POINT& _mp);

protected:
	static int __stdcall Callback_OnCharInput_Number(char _cKey);

protected:
	ChargePageType m_ePage;
	MyGfxEdit* m_pEdit;
};
//////////////////////////////////////////////////////////////////////////
#endif