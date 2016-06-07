#ifndef _INC_GAMEQUESTDLG_
#define _INC_GAMEQUESTDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include "GameDlgBase.h"
//////////////////////////////////////////////////////////////////////////
struct QuestSummary
{
	std::string xQuestName;
	int nType;
};

struct QuestTip
{
	std::string xDescript;
	std::string xRequire;
	std::string xReward;
};

struct QuestInfo
{
	QuestSummary stSummary;
	QuestTip stTip;
};

typedef std::list<QuestInfo> QuestInfoList;
//////////////////////////////////////////////////////////////////////////
class GameQuestDlg : public GameNewDlg
{
public:
	GameQuestDlg();
	virtual ~GameQuestDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnCommonButtonClick(int _id);
	virtual bool OnShowDialog();

public:
	//	Export to lua
	QuestInfo* PushQuestInfo();

	bool CursorScrollUp();
	bool CursorScrollDown();
	bool CursorScroll(int _nTimes);

private:
	QuestInfoList m_xQuestInfoList;
	//int m_nCurSelLine;
	int m_nShowIndex;
	int m_nCursorIndex;
};
//////////////////////////////////////////////////////////////////////////
#endif