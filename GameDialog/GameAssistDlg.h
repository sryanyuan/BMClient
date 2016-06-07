#ifndef _INC_GAMEASSISTDLG
#define _INC_GAMEASSISTDLG
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include "GameDlgBase.h"
//////////////////////////////////////////////////////////////////////////
class GameAssistDlg : public GameCommonDlg
{
public:
	enum RECT_CHILD
	{
		RC_TRANSFORM,
		RC_SHOWHIDE,
		RC_CLOSE,
		RC_TOTAL
	};

public:
	GameAssistDlg();
	virtual ~GameAssistDlg(){}

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool IsCaptionBar(int _x, int _y);
	virtual void Update(float _dt){}
	virtual void ResetStatus();

public:
	void TransformHorizToVertical();
	void TransformVerticalToHoriz();
	void OnBtn(int _idx);
	void OnAccelerate(int _idx);

protected:
	bool m_bHoriz;
	bool m_bMaxSize;
	BYTE m_bBtnState[RC_TOTAL];
	RECT m_rcBtnRect[RC_TOTAL];
};
//////////////////////////////////////////////////////////////////////////
#endif