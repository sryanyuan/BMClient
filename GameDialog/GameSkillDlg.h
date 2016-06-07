#ifndef _INC_GAMESKILLDLG_
#define _INC_GAMESKILLDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameDlgBase.h"
//////////////////////////////////////////////////////////////////////////
class GameSkillDlg : public GameNewDlg
{
public:
	GameSkillDlg();
	virtual ~GameSkillDlg();

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render();
	virtual void Update(float _dt){}
	virtual bool IsCaptionBar(int _x, int _y);
	virtual bool OnCommonButtonClick(int _id);
	virtual bool OnShowDialog();

public:
	inline void Init(GameMainOptUI* _pParent)
	{
		m_pParent = _pParent;
	}
	inline void Reset()
	{
		m_nPage = 0;
	}

private:
	//
	GameMainOptUI* m_pParent;
	int m_nSkillSum;
	int m_nPage;
};
//////////////////////////////////////////////////////////////////////////
#endif