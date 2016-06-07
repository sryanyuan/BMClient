#ifndef GAMEINFOBOARDDLG_H_
#define GAMEINFOBOARDDLG_H_

#include "../Common/GameRenderObject.h"
#include "../Common/GfxFont.h"

#define MAX_BOARDMSG_LENGTH		40
#define MAX_BOARDMSG_NUM		30
#define MSG_DELETE_TIME_MS		5000

#define DRAW_LINE_OFFSET		12

struct BoardMsgNode
{
	char szMsg[MAX_BOARDMSG_LENGTH];
	DWORD dwInsertTime;
	DWORD dwCol;

	BoardMsgNode* pPre;
	BoardMsgNode* pNext;
};

typedef BoardMsgNode* LPBoardMsgNode;
struct ItemAttrib;

class GameInfoBoardDlg : public RenderObject
{
public:
	static GameInfoBoardDlg* GetInstance()
	{
		static GameInfoBoardDlg* pIns = NULL;
		if(!pIns)
		{
			pIns = new GameInfoBoardDlg;
		}
		return pIns;
	}

protected:
	GameInfoBoardDlg();

public:
	~GameInfoBoardDlg();

public:
	//bool InsertBoardMsg(const char* lpszMsg);
	bool InsertBoardMsg(const char* _fmt, ...);
	bool InsertBoardMsg(DWORD _col, const char* _fmt, ...);
	bool InsertEquipErrorMsg(int _err);
	void ShowItemMsg(const ItemAttrib* _pItem);
	void Clear();

public:
	virtual void Update(float _dt);
	virtual void Render();

private:
	LPBoardMsgNode m_pCenterNode;
	LPBoardMsgNode m_pFirstShowNode;
	LPBoardMsgNode m_pLastShowNode;
	GfxFont* m_font;
};

#endif