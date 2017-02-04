#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameTextureManager.h"
#include "../GameScene/GameMapManager.h"
#include <hge.h>
#include "../Common/GfxFont.h"
#include "../GameDialog/GameDlgBase.h"
#include "../GameScene/GameMonster.h"
#include "../GameDialog/GameMiniMapDlg.h"
#include "../GameDialog/GameStatusDlg.h"
#include "../GameScene/MagicEffect.h"
#include "../GameDialog/GameFloatDlg.h"
#include <assert.h>

class GameMonster;
class GameInfoBoardDlg;
struct PacketBase;
typedef std::list<MagicElement*> MAGICLIST;

#define MAPPED_OBJECT_X		(VIEW_WIDTH / UNIT_WIDTH + 2)
#define MAPPED_OBJECT_Y		(VIEW_HEIGHT / UNIT_HEIGHT + 2)

#define CHAT_BTNS			4
#define CHAT_RECORD_NUMBER	15

//	GameScene 为游戏场景 管理游戏资源 渲染 接受用户输入 更新部件
class GameScene : public RenderTarget
{
public:
	GameScene();
	~GameScene();

public:
	bool Init(HGE* _hge);
	bool SwitchScene(DWORD _id);
	bool SwitchScene(const char* lpszmapname);
	virtual bool LoadProperty(int _id);
	//	处理界面的点击之类的
	void ProcKeyCmd();
	void ProcKeyCmd_ChatRecord();
	void ProcMagicCmd(GameObject* _obj);

	bool ProcItemClick(const POINT& _ptMouse);
	bool ProcChatClick(const POINT& _ptMouse);
	GameObject* FindGameMonsterByArea(float _mx, float _my);
	inline GameMainOptUI* GetMainOpt()
	{
		return m_pMainOpt;
	}

	bool LoadMagicCost();

	inline int GetMouseCoordX()					{return m_nMouseCoordX;}
	inline int GetMouseCoordY()					{return m_nMouseCoordY;}

	void BringDlgToTop(RenderObject* _obj);
	void UpdateMapData(float _dt);
	void UpdateMouseCoord();
	bool CleanMapDataCell(int _x, int _y, GameObject* _pObj);

	//	绝对位置
	inline const MemMapObject* GetMemMapObject(int _x, int _y)
	{
		if(_x < 0 ||
			_x >= MAPPED_OBJECT_X ||
			_y < 0 ||
			_y >= MAPPED_OBJECT_Y)
		{
			return NULL;
		}
		return &m_stMapObjects[_x][_y];
	}

	bool CreateMonster(int _x, int _y, OBJECT_TYPE _type, int _id);
	bool InsertNewObject(const PkgNewPlayerNot& not);
	bool InsertNewObject(const PkgNewNPCNot& not);
	bool InsertNewGroundItem(const GroundItem* _pItem);
	bool InsertNewMagic(MagicElement* _pMgc);
	bool InsertNewAttackNumber(int _nNumber, bool _bAdd, GameObject* _pObj, bool _bCritical=false);
	bool RemoveGroundItem(DWORD _dwID);
	bool RemoveNPC(DWORD _uHandleID);

	GameMonster* NewMonsterByID(int _nID);

	GameObject* GetMonster(int _x, int _y);
	//	相对位置
	const MemMapObject* GetMappedObjectsRelative(int _x, int _y);
	inline GameObject* GetTarget()
	{
		return m_pTarget;
	}
	bool SendChatMessage();
	void ShowGmNotification(const std::string& _xMsg);
	void ShowSystemNotify(const std::string& _xMsg, DWORD dwColor, DWORD _dwTimes);
	static int __stdcall OnEnterPressed(void* _pEdit);
	static int __stdcall OnLoseFocus(void* _pEdit);

	inline PLAYERLIST& GetNPCList()
	{
		return m_lstPlayers;
	}
	GameObject* GetPlayerByName(const char* _pszName);

	inline GROUNDITEMS& GetGroundItems()
	{
		return m_lstGroundItems;
	}

	inline DWORD GetNetDelayMillSeconds()
	{
		return m_dwNetDelayMillSeconds;
	}
	inline void SetNetDelayMillSeconds(DWORD _dwDelay)
	{
		m_dwNetDelayMillSeconds = _dwDelay;
	}
	inline DWORD GetLastSendSequence()
	{
		return m_dwLastSendSequence;
	}
	inline void SetNetDelayReceived()
	{
		m_bReceiveReply = true;
	}
	void OnReceiveNetDelayReply(const PkgPlayerNetDelayAck& ack);

	void PlayTransAnimation(int _nId, int _nX, int _nY);

public:
	void OnMessage(PacketBase* _pPacket);
	void RequestCloseGame();
	void RenderMagic();
	void RenderAttackNumber();
	void RemoveMagicFireAndTarget(GameObject* _pObj);

public:
	void MousePtToScreenPt(const POINT& _src, POINT& _dest);
	inline GameMiniMapDlg* GetMiniMapDlg()
	{
		return m_pMiniMap;
	}
	inline GameStatusDlg* GetStatusDlg()
	{
		return m_pStatusDlg;
	}
	inline GameFloatDlg* GetFloatDlg()
	{
		return m_pFloatDlg;
	}

private:
	/*
	 *	TEST
	 */
#ifdef _DEBUG
	void InsertBoardMsg();
#endif

private:
	bool IsInClient();
	void ProcGameCmd(const POINT& _mp);

private:
	int ProcExcept(int nExpCode);

public:
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void Render();
	virtual void Update(float _dt);

private:
	//	游戏玩家
	GamePlayer* m_pPlayer;
	//	主操作界面
	GameMainOptUI* m_pMainOpt;
	//	小地图
	GameMiniMapDlg* m_pMiniMap;
	//	信息输出窗口
	GameInfoBoardDlg* m_pMsgBoard;
	//	悬浮消息窗口
	GameFloatDlg* m_pFloatDlg;

	//	游戏物体列表
	PLAYERLIST m_lstPlayers;
	//	游戏地面物品列表
	GROUNDITEMS m_lstGroundItems;
	//	物体装备映射表
	MemMapObject m_stMapObjects[MAPPED_OBJECT_X][MAPPED_OBJECT_Y];
	//	游戏魔法列表
	MAGICLIST m_xMagics;
	//	选中怪物
	GameObject* m_pTarget;
	//	攻击数字列表
	MAGICLIST m_xAttackNumber;
	//	玩家状态窗口
	GameStatusDlg* m_pStatusDlg;


	//	网络延迟
	//	上次发送的序列号
	DWORD m_dwLastSendSequence;
	//	网络延迟时间
	DWORD m_dwNetDelayMillSeconds;
	//	上次发送的时间
	DWORD m_dwDelaySendTime;
	//	是否收到了回应
	bool m_bReceiveReply;

	//	GM公告显示
	std::string m_xGmMsg;
	DWORD m_dwLastShowGmMsgTime;
	RECT m_rcGmMsg;

	//	最近的几条聊天记录
	StringList m_xRecentChat;
	StringList::iterator m_xRecentChatIterator;

	bool m_bInit;

	bool m_bShowDonate;

private:
	static HGE* s_hge;
	char m_szCurMapName[15];

	DWORD m_dwLastDetectTime;

public:
	static GameScene* sThis;
	//	魔法消耗表
	DWORD m_dwMgcCostTable[MEFF_USERTOTAL];
	//	魔法延迟表
	DWORD m_dwMgcDelayTable[MEFF_USERTOTAL];

	int m_nMouseCoordX;
	int m_nMouseCoordY;
};


//////////////////////////////////////////////////////////////////////////
#endif