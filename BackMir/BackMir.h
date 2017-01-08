#ifndef _BACKMIR_H_
#define _BACKMIR_H_
#include "SGameBase.h"
#include <Windows.h>
#include "../Common/MirMap.h"
#include "../GameScene/GameTextureManager.h"
#include "../GameScene/GameMapManager.h"
#include "../GameScene/GameScene.h"
#include "../LoginScene/LoginScene.h"
//#include "../LoginScene/SelChrScene.h"
//#include "../Net/PacketQueue.h"
//#include "../../CommonModule/ScriptEngine.h"
#include "LuaClientEngine.h"
#include "../../CommonModule/SaveFile.h"
#include "../../CommonModule/SettingLoader.h"
#include "../../CommonModule/DataParser.h"
#include "../DuiWnd/AssistPaneWnd.h"
#include "../Common/SelectedTextureManager.h"
#include <google/protobuf/message.h>
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class MirGame;
class AssistPaneWnd;

extern MirGame* pTheGame;

extern ByteBuffer g_xBuffer;
//////////////////////////////////////////////////////////////////////////
#define DO_LOGIN_PACKET(op, pkg) case op:{pkg p;g_xBuffer >> p;DoLoginPacket(p);}break;

#define MAP_LAYER_NUM 3
#define MAP_WIDTH_PIX 48
#define MAP_HEIGHT_PIX 32
//
#define SMSG_ERROR(str); ::MessageBox(NULL, str, "Error!", MB_OK | MB_ICONERROR);
#define INC_ELE(e, n) (e += n)

unsigned int SendBufferToGS(ByteBuffer* _pBuf);
unsigned int SendBufferToLS(ByteBuffer* _pBuf);
unsigned int SendBufferToGS(ByteBuffer& _xBuf);
unsigned int SendBufferToLS(ByteBuffer& _xBuf);

unsigned int SendProto(int _nOpcode, google::protobuf::Message& _refMsg);
unsigned int SendProto2(int _nOpcode, google::protobuf::Message& _refMsg);
//////////////////////////////////////////////////////////////////////////
class MirGame : public SGameBase
{
public:
	MirGame();
	~MirGame();
	static MirGame* GetInstance();

public:
	virtual bool UserInitial();
	virtual void UserUninitial();
	virtual void UserFrameFunc();
	virtual void UserRenderFunc();

	virtual LRESULT                 WinEventFilter(HWND h, UINT u, WPARAM w, LPARAM l); 

	virtual bool IsWindowMode();

public:
	inline GameScene* GetGameScene()
	{
		return m_pGameScene;
	}
	inline BYTE GetCurStage()
	{
		return m_bScene;
	}
	inline void SetCurState(BYTE _bStage)
	{
		m_bScene = _bStage;
	}
	inline LoginScene* GetLoginScene()
	{
		return m_pxLoginScene;
	}
	inline lua_State* GetLuaState()
	{
		return m_xScript.GetVM();
	}
	inline LuaClientEngine* GetScriptEngine()
	{
		return &m_xScript;
	}
	/*inline ScriptEngine* GetQuestScript()
	{
		return &m_xQuestScript;
	}*/
	/*inline lua_State* GetQuestLuaState()
	{
		return m_xQuestScript.GetVM();
	}*/
	inline std::map<int, int>& GetClothTable()
	{
		return m_xClothTable;
	}
	inline std::map<int, int>& GetWeaponTable()
	{
		return m_xWeaponTable;
	}
	inline bool IsSoundEnabled()
	{
		return m_bSoundEnable;
	}
	inline void SetSoundEnabled(bool _b)
	{
		m_bSoundEnable = _b;
	}
	inline bool IsSoundForbid()
	{
		return false;
		return m_bSoundForbid;
	}
	inline bool CanPlaySound()
	{
		return Config_EnableSound();
		return (m_xSettings.GetIntValue("SOUNDENABLE") != 0);
		return (m_bSoundEnable && !m_bSoundForbid);
	}
	inline const char* GetRunMap(int _id)
	{
		if(_id < 0 ||
			_id >= m_xRunMapData.size())
		{
			return NULL;
		}
		return m_xRunMapData[_id].c_str();
	}
	inline const char* GetInstanceMap(int _id)
	{
		INSTANCEMAPDATA::const_iterator fndIter = m_xInsMapData.find(_id);
		if(fndIter != m_xInsMapData.end())
		{
			return fndIter->second.c_str();
		}

		return NULL;
	}
	inline hgeResourceManager* GetGameRes()
	{
		return m_pxGameRes;
	}
	inline hgeResourceManager* GetLoginRes()
	{
		return m_pxResMgr;
	}
	inline SettingLoader* GetGameSettings()
	{
		return &m_xSettings;
	}
	inline void ReleaseLoginRes()
	{
		delete m_pxLoginScene;
		m_pxLoginScene = NULL;
	}
	inline bool IsConnect()
	{
		return m_bConnect;
	}
	inline void SetConnect()
	{
		m_bConnect = true;
	}
	inline AssistPaneWnd* GetAssistPaneWnd()
	{
		return m_pAssistPaneWnd;
	}

	bool SetGameCursor(int _nResourceId);

	inline int GetDarkMode()						{return m_nDarkMode;}
	inline void SetDarkMode(int _nMode)				{m_nDarkMode = _nMode;}
	HTEXTURE GetDarkModeTexture();

	inline int GetShowMapSnap()						{return m_bShowMapSnap;}
	inline void SetShowMapSnap(bool _bShow)			{m_bShowMapSnap = _bShow;}

	inline GAME_MODE GetGameMode()					{return m_eGameMode;}
	inline void SetGameMode(GAME_MODE _eMode)		{m_eGameMode = _eMode;}
	void GetSnapshotFile(std::string& _file);
	bool CheckItemValid(const ItemAttrib* _pChecked);
	bool IsPassiveSkill(int _nMgcID);
	static bool IsEquipItem(const ItemAttrib& _item)
	{
		if(_item.type == ITEM_WEAPON ||
			_item.type == ITEM_CLOTH ||
			_item.type == ITEM_NECKLACE ||
			_item.type == ITEM_RING ||
			_item.type == ITEM_BRACELAT ||
			_item.type == ITEM_HELMET ||
			_item.type == ITEM_MEDAL ||
			_item.type == ITEM_SHOE ||
			_item.type == ITEM_BELT ||
			_item.type == ITEM_GEM ||
			_item.type == ITEM_CHARM)
		{
			return true;
		}

		return false;
	}
	void PlayItemPutDownSound(const ItemAttrib* _pItem);
	void PlayItemClickUpSound(const ItemAttrib* _pItem);

	bool IsInPlayerViewRange(int _nCoordX, int _nCoordY);

	const char* GetDifficultyLevelName(int _nDifficultyLevel);
	void UpdateWindowTitle(int _nDifficultyLevel);

	void RequestSmallQuit();

	//////////////////////////////////////////////////////////////////////////
	//	Quick config helper
	void LoadGameSetting();
	void WriteDefaultSetting();
	void WriteSettingsToFile();
	void UpdateConfigCache();
	bool Config_RenderHPBar();
	bool Config_ItemFlash();
	bool Config_EnableSound();
	bool Config_WindowMode();
	bool Config_SelfName();
	bool Config_EnhanceSel();
	int  Config_GetFPS();
	bool Config_JpgSnap();
	bool Config_HPAnimation();
	bool Config_TileSurface();
	bool Config_MousePickUp();
	bool Config_DisableRenderNameFrame();
	bool Config_DisableRenderChatFrame();

public:
	void Log(const char* _plog);
	bool LoadScript(int _nMapID);
	bool LoadQuestScript();
	//	是否开启了变速精灵
	bool IsTimerFunctionRepalaced();
	//	获得地图名称
	bool GetMapName(int _id, std::string& _xName);

private:
	void InitLogfile();
	bool CheckParam();
	void OnSocketMessage(SOCKET _s, LPARAM lParam);
	void OnGameSvrMsg(SOCKET _s, LPARAM lParam);
	void OnLoginSvrMsg(SOCKET _s, LPARAM lParam);
	void LoadIndexInfo();

	static void STDCALL _HandleLoginMsg(const void* _pData, unsigned int _len);
	void HandleLoginMsg(const void* _pData, unsigned int _len);
	void LoginAddGameRoleAck(const char* _pData, unsigned int _len);
	void LoginDelGameRoleAck(const char* _pData, unsigned int _len);
	void DoLoginPacket(const PkgLoginQuickMsgNot& not);
	void LoginCreateHumRsp(const char* _pData, unsigned int _len);
	void LoginDelHumRsp(const char* _pData, unsigned int _len);
	void LoginQuickMessage(const char* _pData, unsigned int _len);

	void RenderCursor(float _fx, float _fy);

private:
	//	游戏场景
	GameScene* m_pGameScene;
	//	登陆场景
	LoginScene* m_pxLoginScene;
	//	所处场景
	BYTE m_bScene;
	//	是否初始化完毕
	bool m_bInitialized;

	hgeResourceManager* m_pxResMgr;
	hgeResourceManager* m_pxGameRes;

	char m_szIP[4 * 3 + 3 + 1];
	WORD m_wPort;
	char m_szHero[20];

	//	脚本引擎(对话脚本)
	LuaClientEngine m_xScript;
	//	脚本引擎(任务提示脚本)
	//LuaClientEngine m_xQuestScript;

	//	服装 武器 纹理索引
	std::map<int, int> m_xClothTable;
	std::map<int, int> m_xWeaponTable;

	bool m_bSoundForbid;
	bool m_bSoundEnable;

	//	运行地图
	RUNMAPDATA m_xRunMapData;
	INSTANCEMAPDATA m_xInsMapData;
	//	是否已连接
	bool m_bConnect;
	//	游戏设置
	SettingLoader m_xSettings;
	bool m_bConfigCache[12];
	int m_nFps;
	//	游戏 模式
	GAME_MODE m_eGameMode;

	DataParser m_xLoginMsgParser;

	//	辅助窗口
	AssistPaneWnd* m_pAssistPaneWnd;

	//	内存检测释放计时
	DWORD m_dwLastCheckMemoryStatus;

	//	黑夜效果
	int m_nDarkMode;
	HTEXTURE m_texDarkMode;

	//	是否显示小地图和大地图
	bool m_bShowMapSnap;

	//	鼠标
	int m_nCursorResourceId;
};

//////////////////////////////////////////////////////////////////////////
#endif