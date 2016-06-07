#ifndef _INC_GAMEROLEINFODLG_
#define _INC_GAMEROLEINFODLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include "GameDlgBase.h"
#include <string>

using std::string;
//////////////////////////////////////////////////////////////////////////
class GameTeamMenu : public GameNewDlg
{
public:
	GameTeamMenu();
	virtual ~GameTeamMenu(){}

public:
	virtual void Render();
	virtual bool IsCaptionBar(int _x, int _y)	{return false;}
	virtual bool OnCommonButtonClick(int _id);

public:
	void SetPlayerID(int _id)
	{
		m_nPlayerID = _id;
	}

protected:
	int m_nPlayerID;
};

//////////////////////////////////////////////////////////////////////////
class GameRoleInfoDlg : public GameNewDlg
{
public:
	GameRoleInfoDlg();
	virtual ~GameRoleInfoDlg(){}

public:
	void ShowWithPlayerID(int _nID);
	void ShowWithPlayer(GameObject* _pObj);

public:
	virtual void Render();
	virtual bool IsCaptionBar(int _x, int _y)	{return false;}
	virtual bool OnCommonButtonClick(int _id);
	virtual void OnMove(int _nOftX, int _nOftY);
	virtual bool OnShowDialog();
	virtual bool OnCloseDialog();

public:
	int GetPlayerID()			{return m_nPlayerID;}

protected:
	int m_nPlayerID;
	string m_xPlayerName;
	int m_nPlayerLevel;
	char m_cPlayerSex;
};
//////////////////////////////////////////////////////////////////////////
#endif