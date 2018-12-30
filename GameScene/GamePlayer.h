#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_
#include "../Common/GameRenderObject.h"
#include "../Common/PlayerBag.h"
//#include "../GameScene/GameMagicManager.h"
#include "../Common/GlobalDefine.h"
#include "../Common/PlayerStatusControl.h"
#include "../Net/GamePacketHandler.h"
#include "GameOtherPlayer.h"
#include "../../CommonModule/ShareData.h"
#include "../../CommonModule/QuestContext.h"
#include "../../CommonModule/RandGenerator.h"
//////////////////////////////////////////////////////////////////////////
#define WAITSERVERRSP_NONE			0
#define WAITSERVERRSP_NORMALATK		1
//////////////////////////////////////////////////////////////////////////

class GameMonster;

typedef std::map<DWORD, DWORD> MGCHISTORYMAP;

class GamePlayer : public GameOtherPlayer/*, public MyHeroPacketHandler*/
{
	//SINGLETON_DECLARE(GamePlayer);
public:
	static GamePlayer* GetInstance()
	{
		static GamePlayer* pIns = NULL;
		if(!pIns)
		{
			pIns = new GamePlayer;
		}
		return pIns;
	}
protected:
	GamePlayer();
public:
	virtual ~GamePlayer();

public:
	virtual bool LoadProperty(int _id);
	virtual void Render();
	virtual int OnLButtonDown(){return 0;}
	virtual int OnRButtonDown(){return 0;}
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual int GetSkillLevel(DWORD _dwMgcID);

public:
	bool Save();
	void Reset();

public:
	//	画血条
	void RenderHP();
	bool Run(PLAYER_DIRECTION _pd);
	bool Walk(PLAYER_DIRECTION _pd);
	bool AttackMonster(GameObject* _mons);
	bool NPCClick(GameObject* _pObj);
	bool OtherPlayerClick(GameObject* _pObj);
	bool HandleMagicInput(const POINT& _mp);
	bool HandleNoneSpellMagic(const POINT& _mp);
	bool HandleSpellMagic(const POINT& _mp);

public:
// 	int CalTextureIndex(int* _ptexsum = NULL);
// 	int CalHairTexIndex(int* _ptexsum = NULL);
// 	int CalWeaponTexIndex(int* _ptexsum = NULL);
	int GetScreenOffsetX();
	int GetScreenOffsetY();

	PlayerEquipAttrib* GetEquip()
	{
		return &m_equipAttrib;
	}
	PlayerBag* GetPlayerBag()
	{
		return &m_bag;
	}
	inline void Unlock()
	{
		m_bIsLocked = false;
		m_dwLockType = PST_NONE;
	}
	inline bool IsUsingPreLock()
	{
		return m_bUsingPreLock;
	}
	inline void Lock(DWORD _dwType)
	{
		m_dwLockType = _dwType;
		m_bIsLocked = true;
		m_dwLastLockedTime = GetTickCount();
	}
	inline DWORD GetLockType()
	{
		return m_dwLockType;
	}
	inline bool IsLocked()
	{
		return m_bIsLocked;
	}
	//	获取职业字符串
	const char* GetHref()
	{
		return "";
	}
	int GetMapID()
	{
		return m_nMapID;
	}
	void SetMapID(int _map)
	{
		m_nMapID = _map;
	}
	inline void SetWalkLocked()
	{
		m_bIsWalkLocked = true;
		m_dwLastLockedWalkTime = GetTickCount();
	}
	int GetMagicCost(const MagicDetail* _pDetail);
	int GetMagicDelay(const MagicDetail* _pDetail);
	inline int GetWanLi()
	{
		if(m_attrib.level > MAX_LEVEL)
		{
			return 0;
		}
		if(GetHeroJob() < 0 ||
			GetHeroJob() > 2)
		{
			return 0;
		}
		return g_nWanLiTable[m_attrib.level - 1][GetHeroJob()];
	}
	inline int GetDonateLeft()			{return m_nDonateLeft;}
	inline void SetDonateLeft(int _nLeft){m_nDonateLeft = _nLeft;}

	inline bool IsRequestSmallQuit()	{return m_bRequestSmallQuit;}
	inline void SetRequestSmallQuit(bool _b)
	{
		m_bRequestSmallQuit = _b;
	}

	bool CanAttack();

public:
	void SetPlayerItem(PLAYER_ITEM_TYPE _type, const ItemAttrib* _item);
	bool CanEquip(const ItemAttrib* _item, DWORD* _err = NULL);
	bool DressEquip(int _bagidx, DWORD* _err = NULL);
	bool DressEquip(int _bagidx, PLAYER_ITEM_TYPE _type, DWORD* _err = NULL);
	bool UnDressEquip(PLAYER_ITEM_TYPE _type);

	PLAYER_ITEM_TYPE Item2PlayerItem(ItemAttrib* _item);
	BYTE PlayerItem2Item(PLAYER_ITEM_TYPE _type);

	bool MoveBagItemToAssistItem(int _bagidx, int _astidx);
	bool IsCostItem(const ItemAttrib& _item);
	bool IsEquipItem(const ItemAttrib& _item);
	void UpdateAccelerate(int _idx = -1);

	bool UseItemFromBag(int _bagidx);
	bool UseItemFromAssist(int _astidx);
	bool UseItem(ItemAttrib* _item);

	int GetAssistItemsEmptySum();
	int GetBagItemsEmptySum();

	void WriteBagItemOrder();
	void ReadBagItemOrder();

public:
	//	true : 还没加满 false : 加满了
	bool AddHP(int _hp, int _way = ADD_NORMAL, int _step = 10);
	bool AddMP(int _mp, int _way = ADD_NORMAL, int _step = 10);
	void DecHP(int _hp);
	void DecMP(int _mp);
	void AddExp(int _exp);
	void RandomPos();

private:
	void LevelUp();
	bool LoadSaveData_SaveFile(const char* _pszFile);
	bool LoadSaveData_ZipArchive(const char* _pszFile);

public:
	//	魔法相关

public:
	//	根据装备更新玩家属性值
	void UpdatePlayerAttrib();
	//	更新套装属性信息
	void UpdateSuitAttrib();
	//	更新相同品质的加成信息
	void UpdateSuitSameLevel();
	//void UpdatePlayerTexIndex();
	//	是否是最后一帧
	bool IsLastFrame()
	{
		int nTotal = 0;
		CalTextureIndex(&nTotal);
		return nTotal - 1 == GetCurFrame();
	}
public:
	void SetAtkMonster(GameObject* _mons)
	{
		m_atkTarget = _mons;
	}
	void SetMgcTarget(GameObject* _pMons)
	{
		m_pMgcTarget = _pMons;
	}
	const GameObject* GetAtkMonster() const
	{
		return m_atkTarget;
	}
	GameObject* GetMgcTarget() const
	{
		return m_pMgcTarget;
	}

public:
	//GameMagicManager m_mgcs;
	int GetMagicCount();
	bool AddToMagicTree(DWORD _dwMgcID);
	bool IsMagicExist(DWORD _dwMgcID);
	bool UpgradeMagicLevel(DWORD _dwMgcID, char _c = 1);
	bool SetMagicLevel(DWORD _dwMgcID, char _level);
	bool SetMagicKey(DWORD _dwMgcID, char _cKey);
	bool SetMagicAccKey(DWORD _dwMgcID, char _cKey);
	const MagicDetail* GetMagicByKey(char _cKey);
	const MagicDetail* GetMagicByAccKey(char _cKey);
	inline const MagicDetail* GetMagicByIndex(int _index)
	{
		if(_index < 0 ||
			_index >= USER_MAGIC_NUM)
		{
			return NULL;
		}
		return &m_stMagicDetail[_index];
	}
	const MagicDetail* GetMagicByID(DWORD _dwMgcID);
	inline void WriteMagicKeyCfg(ByteBuffer* _pBuf)
	{
		std::map<char, MagicDetail*>::const_iterator begiter = m_xInputMap.begin();

		for(begiter; begiter != m_xInputMap.end();
			++begiter)
		{
			*_pBuf << begiter->first << begiter->second->wID;
		}
	}
	void WriteAccMagicKeyCfg();
	void LoadAccMagicKeyCfg();
	inline int GetAccMagicID()			{return m_nAccMagicID;}
	inline void SetAccMagicID(int _nID)	{m_nAccMagicID = _nID;}
	inline void SetSaveIndex(int _idx)
	{
		m_nSaveIndex = _idx;
	}
	inline int GetSaveIndex()
	{
		return m_nSaveIndex;
	}
	inline const char* GetSaveName()
	{
		return m_szSaveName;
	}
	inline void SetSaveName(const char* _pszSaveName)
	{
		strcpy(m_szSaveName, _pszSaveName);
	}
	inline CoolDownController* GetDrugCoolDownController()
	{
		return &m_xDrugCoolDown;
	}
	inline QuestContext* GetQuest()
	{
		return &m_xQuest;
	}
	inline void SetHeroNameInMask(const char* _pszName)
	{
		//	Copy to name mask buffer
		if(_pszName[0] == 0 ||
			strlen(_pszName) > 19)
		{
			return;
		}

		ZeroMemory(m_szNameCopy, sizeof(m_szNameCopy));

		for(int i = 0; i < 20/*strlen(_pszName)*/; ++i)
		{
			char cVar = _pszName[i];
			cVar ^= g_szNameMask;
			m_szNameCopy[i] = cVar;
		}
	}
	inline void GetHeroNameInMask(char* _pszNameBuf)
	{
		_pszNameBuf[0] = 0;

		/*if(m_szNameCopy[0] == 0 ||
			strlen(m_szNameCopy) > 19)
		{
			return;
		}*/

		for(int i = 0; i < 20/*strlen(m_szNameCopy)*/; ++i)
		{
			_pszNameBuf[i] = m_szNameCopy[i] ^ g_szNameMask;

			/*if(i == strlen(m_szNameCopy) - 1)
			{
				_pszNameBuf[i + 1] = 0;
			}*/
		}
	}
	inline void SetUID(UINT _uUID)				{m_uUID = _uUID;}
	inline UINT GetUID()						{return m_uUID;}

public:
	void SetRandSeedNormalAtk(DWORD _dwSeed)	{m_xNormalAtkRand.SetSeed(_dwSeed);}

	//	等待服务器同步函数
	void SetWaitServerResponse(int _nWaitType, int _nTimeout);
	bool IsWaitServerResponse();
	void UpdateWaitServerResponse();
	inline int GetWaitServerResponseType()
	{
		return m_nWaitServerRspType;
	}

	void UpdateEnableSkill(int _nSkillType, bool _bEnabled);

private:
	void AutoAttackTarget();
	void PreNormalAttackTarget(GameObject* _pTarget);

private:
	//	玩家装备
	PlayerEquipAttrib m_equipAttrib;
	hgeSprite* m_pSprHum;
	hgeSprite* m_pSprHair;
	hgeSprite* m_pSprWeapon;
	hgeSprite* m_pSprState;
	hgeSprite* m_pSprMgc;
	//	玩家背包
	PlayerBag m_bag;
	//	玩家装备
	//ItemAttrib m_equip[PLAYER_ITEM_TOTAL];
	//	索引信息
	PlayerTexIndexTable m_indexs;
	//	攻击相关
	GameObject* m_atkTarget;
	//	玩家的MapID
	int m_nMapID;
	//	玩家血量魔法控制
	//PlayerStatusControl m_statusControl;
	MagicDetail m_stMagicDetail[USER_MAGIC_NUM];
	std::map<char, MagicDetail*> m_xInputMap;
	std::map<char, MagicDetail*> m_xAccInputMap;

	bool m_bIsLocked;
	DWORD m_dwLockType;
	DWORD m_dwLastLockedTime;
	bool m_bIsWalkLocked;
	DWORD m_dwLastLockedWalkTime;
	//	魔法锁定
	GameObject* m_pMgcTarget;
	DWORD m_dwLastUseMagicTime;
	//	玩家存档索引
	int m_nSaveIndex;
	//	玩家存档名字
	char m_szSaveName[20];
	//	魔法使用历史 用于实现魔法延时 冷却效果
	MGCHISTORYMAP m_xMgcHistory;

	//	药品冷却
	CoolDownController m_xDrugCoolDown;
	//	任务数据
	QuestContext m_xQuest;

	//	人物名字备份
	char m_szNameCopy[20];

	UINT m_uUID;

	//	当前快捷魔法ID
	int m_nAccMagicID;

	//	是否使用预加锁
	bool m_bUsingPreLock;

	//	随机数生成器 与服务器同步
	RandGenerator m_xNormalAtkRand;
	//	预先判断攻击效果的攻击掩码
	DWORD m_dwPreCalcNormalAttackEffMask;
	bool m_bEnableLieHuo;
	bool m_bEnableSuperLieHuo;
	bool m_bEnableBanYue;
	bool m_bEnableCiSha;
	bool m_bEnableKtSword;

	//	服务器同步数据
	int m_nWaitServerRspType;
	DWORD m_dwWaitServerRspTimeoutTime;

	//	贡献币
	int m_nDonateLeft;

	//	小退中
	bool m_bRequestSmallQuit;
};

//////////////////////////////////////////////////////////////////////////
#endif