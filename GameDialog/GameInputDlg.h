#ifndef _INC_GAMEINPUTDLG_
#define _INC_GAMEINPUTDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameDlgBase.h"
#include "GameCommonDlg.h"
#include "../Common/GfxEdit.h"
//////////////////////////////////////////////////////////////////////////
enum InputDlg_Type
{
	InputDlg_None,
	InputDlg_QueryBuySum,
	InputDlg_QueryDropSum,
	InputDlg_QuerySplitSum,
	InputDlg_QueryOffBuy,
	InputDlg_QueryOffTackBack,
	InputDlg_QueryOlShopBuy
};
//////////////////////////////////////////////////////////////////////////
class GameInputDlg : public GameNewDlg
{
public:
	GameInputDlg();
	virtual ~GameInputDlg();

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render();
	virtual bool OnCommonButtonClick(int _id);
	virtual bool OnShowDialog();
	virtual bool OnCloseDialog();

public:
	void SetEditVisible(bool _b)
	{
		m_bEditVisible = _b;
	}
	void SetTextVisible(bool _b)
	{
		m_bTextVisible = _b;
	}
	void SetText(const char* _pszText)
	{
		m_xText = _pszText;
	}
	inline bool IsEditInFocus()
	{
		if(m_pEdit)
		{
			return m_pEdit->IsFocused();
		}
		return false;
	}

	void ShowAsMode(InputDlg_Type _eType, int _nParam0 = 0, int _nParam1 = 0, int _nParam2 = 0, int _nParam3 = 0);

	void OnQueryBuySumConfirm();
	void OnQueryDropSumConfirm();
	void OnQuerySplitSumConfirm();
	void OnQueryOffBuyConfirm();
	void OnQueryOffTackOffConfirm();
	void OnQueryOlShopBuyConfirm();

protected:
	static int __stdcall Callback_OnCharInput_Number(char _cKey);

protected:
	MyGfxEdit* m_pEdit;
	bool m_bEditVisible;
	bool m_bTextVisible;
	std::string m_xText;
	InputDlg_Type m_eDlgType;

	//	for buy mode
	int m_nParam0;
	int m_nParam1;
	int m_nParam2;
	int m_nParam3;
};
//////////////////////////////////////////////////////////////////////////
#endif