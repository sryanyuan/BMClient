#ifndef _INC_EXECUTECONTEXT_
#define _INC_EXECUTECONTEXT_
//////////////////////////////////////////////////////////////////////////
struct CExecuteContext
{
	int nStatus;
	int nResID;

	int nFrmStart;
	int nFrmFrames;
	int nFrmTotal;
	unsigned int uFrmIntervalMS;

	int nEffOffset;
	int nEffIgnore;
	int nEffStart;
	int nEffTotal;

	int nFrmTriggerFrm;
	int nFrmTriggerEffID;
	int nFrmTriggerResID;
	int nFrmTriggerStart;
	int nFrmTriggerFrames;
	int nFrmTriggerTotal;

	// private fields
	int nCurrentFrm;
	unsigned int uLastUpdateTime;
};

#define EXECUTERESULT_DOING	0
#define EXECUTERESULT_DONE	1

//////////////////////////////////////////////////////////////////////////
#endif