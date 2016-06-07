#ifndef _INC_GAMESETTINGDLG_
#define _INC_GAMESETTINGDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameDlgBase.h"
#include "GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
class GameSettingDlg : public GameNewDlg
{
public:
	GameSettingDlg();
	virtual ~GameSettingDlg();

public:
	virtual bool OnCommonButtonClick(int _id);
	virtual bool OnShowDialog();
	virtual bool OnCloseDialog();
	virtual void Render();

public:
	void SwitchPage(int _nPage);
	void SavePageData(int _nPage);
	void LoadPageData(int _nPage);

public:
	void LoadGameSettings();
	void SaveGameSettings();

private:
	int m_nCurPage;
};
//////////////////////////////////////////////////////////////////////////
#endif