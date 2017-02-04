#ifndef GAMEMONSTERMANAGER_H_
#define GAMEMONSTERMANAGER_H_
#include "../GameScene/GamePlayer.h"
#include <map>
#include "../Common/GlobalFunction.h"
#include <hgesprite.h>
#include "../Astar/SAstar.h"
#include "../Net/GamePacketHandler.h"
#include "../Common/SelectedTextureManager.h"
#include "../../CommonModule/GDefine.h"

#ifdef WALK_PATH_EX
typedef SASTAR::SAstarElements	WALKPATH;
typedef SASTAR::SAstarElement	WALKELEMENT;
#else
typedef SASTAR::SAstarPoints	WALKPATH;
typedef POINT					WALKELEMENT;
#endif

// Forward declaration
class GameTextureManager;
class GameMonsterManager;
class GameMapManager;
struct CExecuteContext;

class GameMonster : public GameObject
{
	//friend class GameMonsterManager;
	friend class GameMapManager;
public:
	GameMonster();
	~GameMonster();

public:
	void InitOtherRes(GameTextureManager* _res)
	{
		m_texs = _res;
	}
	void UpdateTextureIndex();
	virtual int CalMonIndex();
	int CalAtkEftIndex();
	bool IsInAttackArea(GameObject* _obj);
	bool IsInViewArea(GameObject* _obj);
	bool AttackUser(GameObject* _obj);
	//	attrib.type 0 : 普通攻击  1 ：魔法攻击
	int GetDamage(GameObject* _obj);

	void MoveToTarget(GameObject* _obj);
	void Move();
	void Wonder();
	bool WalkOneStep(int _drt);
	bool MoveMonster(int _dx, int _dy);

	virtual int GetSoundIndex();

	inline const RenderInfo* GetRenderInfo()
	{
		return m_pRenderInfo;
	}
	inline int GetMasterId()
	{
		return m_nMasterId;
	}

	//	召唤兽的等级
	inline int GetSlaveLevel()							{return LOBYTE(m_attrib.maxAC);}

	void SetLastFrame();

public:
	void AddExecuteContext(CExecuteContext* _pCtx);
	int ExecuteContext(CExecuteContext* _pCtx);
	void RenderContext(const CExecuteContext* _pCtx);

public:
	virtual void Update(float _dt);
	virtual bool LoadProperty(int _id);
	virtual void Render();
	virtual void RenderName();
	//virtual bool IsSelected(const POINT& _pt);
	virtual bool IsLastFrame();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void UpdateMove();

	//	动作数据包处理函数
	/*virtual void OnWalk(ActionPacket* _pPacket);
	virtual void OnTurn(ActionPacket* _pPacket);
	virtual void OnRun(ActionPacket* _pPacket);
	virtual void OnAttack(ActionPacket* _pPacket);*/

	virtual void OnPacket(const PacketHeader* _pPkt);

	//void DoPacket(const PkgObjectActionNot& not);
	void DoPacket(const PkgPlayerClickNPCAck& not);
	void DoPacket(const PkgPlayerUpdateAttribNtf& ntf);
	void DoPacket(const PkgPlayerSetEffectAck& ack);
	void DoPacket(const PkgPlayerChangeMonsLookAck& ack);
	void DoPacket(const PkgPlayerPlaySoundNtf& ntf);
	void DoPacket(const PkgChatNot& not);
	/*void DoPacket(const PkgObjectActionTurnNot& _not);
	void DoPacket(const PkgObjectActionWalkNot& _not);
	void DoPacket(const PkgObjectActionRunNot& _not);
	void DoPacket(const PkgObjectActionAttackNot& _not);
	void DoPacket(const PkgObjectActionDeadNot& _not);
	void DoPacket(const PkgObjectActionExtNot& _not);*/

	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	HTEXTURE GetCurTexture();

protected:
	GameTextureManager* m_texs;
	PlayerTexIndexTable m_indexs;
	//	自动寻路路径
	//WALKPATH m_walkPath;
	hgeSprite* m_pSpr;
	//RenderInfo m_renderInfo;
	const RenderInfo* m_pRenderInfo;

	float m_fLastUpdateTime;
	float m_fLastUpdateAttack;
	float m_fLastUpdateDead;
	float m_fLastUpdateNoWeapon;
	float m_fLastAtkTime;

	float m_fMoveOffsetXTotal;
	float m_fMoveOffsetYTotal;

	DWORD m_dwMoveInterval;
	//DWORD m_dwLastMoveTime;

	DWORD m_dwAttackInterval;
	DWORD m_dwLastAttackTime;
	//	仇恨对象的ID
	int m_nHatred;

	int m_nMasterId;

	// Execute context
	std::map<int, CExecuteContext*> m_xExecuteContextMap;
};

//	触龙神 赤月恶魔 等
class GroundMonster : public GameMonster
{
public:
	GroundMonster();
	virtual ~GroundMonster();

public:
	virtual void Update(float _dt);
	virtual void Render();
/*
	virtual bool IsSelected(const POINT& _pt);
	virtual void RenderHPBar();*/

	virtual bool IsLastFrame();

	virtual int CalMonIndex();

protected:
	float m_fLastUpdateGroundTime;
	float m_fLastUpdateAppearTime;
	bool m_bIsGround;
};

//	神兽
class ShenShouMonster : public GameMonster
{
public:
	ShenShouMonster();
	virtual ~ShenShouMonster();

public:
	virtual void Update(float _dt);

private:
	bool m_bShowEff;
	//virtual void Render();
};

//	骷髅
class KulouMonster : public GameMonster
{
public:
	KulouMonster();
	virtual ~KulouMonster();

public:
	virtual void Update(float _dt);
	virtual bool IsLastFrame();
	virtual void Render();
	virtual int CalMonIndex();

private:
	float m_fLastUpdateShow;
};

//	沃玛教主
class WoMaJiaoZhuMonster : public GameMonster
{
public:
	WoMaJiaoZhuMonster();
	virtual ~WoMaJiaoZhuMonster(){}

public:
	virtual void Render();
	virtual void TransparentRender();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);
};

//	赤月恶魔
class ChiYueEMoMonster : public GameMonster
{
public:
	virtual void OnPacket(const PacketHeader* _pPkt);

	void DoPacket(const PkgPlayerPlayAniAck& ack);

	virtual int CalMonIndex();
};

//	魔龙之柱
class MoLongStone : public ChiYueEMoMonster
{
public:
	virtual void TransparentRender();
	virtual void OnObjectAction(const PkgObjectActionNot& not);
	//virtual void OnPacket(const PacketHeader* _pPkt);

	//void DoPacket(const PkgObjectActionNot& not);
};

//	幻影蜘蛛
class ShadowSpiderMonster : public GameMonster
{
public:
	virtual int CalMonIndex();
};

//	16方向攻击远程怪物 STAGE_SELF
class Self16DrtMonster : public GameMonster
{
public:
	Self16DrtMonster();
	virtual ~Self16DrtMonster(){}

public:
	virtual void Update(float _dt);
	virtual void TransparentRender();

	//virtual void OnPacket(const PacketHeader* _pPkt);

	//void DoPacket(const PkgObjectActionNot& not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

public:
	inline void SetDrt16(int _ndrt)			{m_nDrt16 = _ndrt;}

private:
	int m_nDrt16;
};

//	16方向攻击远程怪物 STAGE_WAY
class Way16DrtMonster : public GameMonster
{
public:
	Way16DrtMonster();
	virtual ~Way16DrtMonster(){}

public:
	virtual void Update(float _dt);

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

	void PlayBulletAni();

private:
	int m_nTargetID;
};

//`月灵
class MoonSpriteMonster : public Way16DrtMonster
{
public:
	virtual void Render();
};

//	魔龙教主
class MoLongBossMonster : public GameMonster
{
public:
	MoLongBossMonster();
	virtual ~MoLongBossMonster(){}

public:
	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

	virtual void Update(float _dt);
	virtual void Render();
	virtual void TransparentRender();
	virtual bool IsLastFrame();

private:
	float m_fLastUpdateSkill;
};

//	赤狐
class ChiHuMonster : public GameMonster
{
public:
	ChiHuMonster();

public:
	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

	virtual void Update(float _dt);

private:
	int m_nLastAtkTarget;
};
//	素狐
class SuHuMonster : public GameMonster
{
public:
	SuHuMonster();

public:
	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

	virtual void Update(float _dt);
	virtual bool IsLastFrame();
	virtual void Render();

private:
	int m_nLastAtkTarget;
	float m_fLastUpdateSkill;
};
//	冰宫侍者
class IceServantMonster : public GameMonster
{
public:
	IceServantMonster();

public:
	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

	virtual void Update(float _dt);
	virtual bool IsLastFrame();
	virtual void Render();

private:
	float m_fLastUpdateSkill;
};
//	天界雕像
class FlyStatue : public GameMonster
{
public:
	FlyStatue();

public:
	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

	virtual void Update(float _dt);
	virtual bool IsLastFrame();
	virtual void Render();

private:
	float m_fLastUpdateSkill;
	int m_nLastAtkTarget;
};
//	Flame summoner
class FlameSummonerMonster : public GameMonster
{
public:
	FlameSummonerMonster();
	virtual ~FlameSummonerMonster();

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& not);
	void DoPacket(const PkgPlayerPlayAniAck& ack);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	float m_fLastUpdateExt1;
	float m_fLastUpdateExt2;
	int m_nLastAtkTarget;
};
//	Flame DC
class FlameDCMonster : public GameMonster
{
public:
	FlameDCMonster();
	virtual ~FlameDCMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	float m_fLastUpdateSkill;
};
//	Flame MC
class FlameMCMonster : public GameMonster
{
public:
	FlameMCMonster();
	virtual ~FlameMCMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	float m_fLastUpdateSkill;
	int m_nLastAtkTarget;
};
//	Ice soldier
class IceSoldierMonster : public GameMonster
{
public:
	IceSoldierMonster();
	virtual ~IceSoldierMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	float m_fLastUpdateSkill;
};
//	Ice guard
class IceGuardMonster : public GameMonster
{
public:
	IceGuardMonster();
	virtual ~IceGuardMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	float m_fLastUpdateSkill;
};

//	Ice king of war
class IceKingOfWarMonster : public GameMonster
{
public:
	IceKingOfWarMonster();
	virtual ~IceKingOfWarMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	float m_fLastUpdateSkill;
	float m_fLastUpdateSkill2;
};

//	Ice king
class IceKingMonster : public GameMonster
{
public:
	IceKingMonster();
	virtual ~IceKingMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	float m_fLastUpdateSkill;
	float m_fLastUpdateSkill2;
};

//	Ice Savage
class IceSavageMonster : public GameMonster
{
public:
	IceSavageMonster();
	virtual ~IceSavageMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	float m_fLastUpdateSkill;
	float m_fLastUpdateSkill2;
};

//	Ice Defender
class IceDefenderMonster : public GameMonster
{
public:
	IceDefenderMonster();
	virtual ~IceDefenderMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	float m_fLastUpdateSkill;
};

//	Yama watcher
class YamaWatcherMonster : public GameMonster
{
public:
	YamaWatcherMonster();
	virtual ~YamaWatcherMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	virtual void OnPacket(const PacketHeader* _pPkt);
	virtual void OnObjectAction(const PkgObjectActionNot& not);
	//void DoPacket(const PkgObjectActionNot& _not);
	void DoPacket(const PkgPlayerPlayAniAck& ack);

private:
	float m_fLastUpdateSkill;
	float m_fLastUpdateSkill2;
};

//	blue ghast
class BlueGhastMonster : public GameMonster
{
public:
	BlueGhastMonster();
	virtual ~BlueGhastMonster(){}

public:
	virtual void Render();
	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);
};

//	manworm
class ManWormMonster : public GameMonster
{
public:
	ManWormMonster();
	virtual ~ManWormMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

protected:
	float m_fLastUpdateSkill;
};

//	Dream Devourer
class DreamDevourerMonster : public GameMonster
{
public:
	DreamDevourerMonster();
	virtual ~DreamDevourerMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();

	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

protected:
	float m_fLastUpdateSkill;
};

//	Mutated Face Grabber
class MutatedFaceGrabberMonster : public GameMonster
{
public:
	MutatedFaceGrabberMonster();
	virtual ~MutatedFaceGrabberMonster(){}

public:
	virtual void Render();
	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);
};

//	Behemoth Devourer
class BehemothDevourerMonster : public GameMonster
{
public:
	BehemothDevourerMonster();
	virtual ~BehemothDevourerMonster(){}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool IsLastFrame();
	//virtual void OnPacket(const PacketHeader* _pPkt);
	//void DoPacket(const PkgObjectActionNot& _not);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

protected:
	DWORD m_dwLastChgRoundTime;
	int m_nCurRoundFrame;
	int m_nLastAtkTarget;
	float m_fLastUpdateSkill;
	float m_fLastUpdateSpeAtk2;
	float m_fLastUpdateSpeAtk3;
};

//	qi e
class QieMonster : public GameMonster
{
public:
	virtual void Render();
};

//	white tiger
class WhiteTigerMonster : public GameMonster
{
public:
	virtual void Render();
};

class MoonLizardMonster : public GameMonster
{
public:
	MoonLizardMonster();
	
public:
	virtual void OnObjectAction(const PkgObjectActionNot& not);
	virtual void Update(float _dt);
	virtual void Render();
	virtual bool IsLastFrame();

protected:
	float m_fLastUpdateSpeAtk1;
};

class MoonStatueMonster : public GameMonster
{
public:
	MoonStatueMonster();

public:
	virtual void OnObjectAction(const PkgObjectActionNot& not);
	virtual void Update(float _dt);
	virtual void Render();
	virtual bool IsLastFrame();

protected:
	float m_fLastUpdateSpeAtk1;
};

class MoonSpiderMonster : public GameMonster
{
public:
	MoonSpiderMonster();

public:
	virtual void OnObjectAction(const PkgObjectActionNot& not);
	virtual void Update(float _dt);
	virtual void Render();
	virtual bool IsLastFrame();

protected:
	float m_fLastUpdateSpeAtk1;
};

class MoonBeastMonster : public GameMonster
{
public:
	MoonBeastMonster();

public:
	virtual void OnObjectAction(const PkgObjectActionNot& not);
	virtual void Update(float _dt);
	virtual void Render();
	virtual bool IsLastFrame();
	virtual void OnPacket(const PacketHeader* _pPkt);

public:
	void DoPacket(const PkgPlayerPlayAniAck& ack);

protected:
	float m_fLastUpdateSpeAtk1;
	float m_fLastUpdateSpeAtk2;
	float m_fLastUpdateSpeAtk3;
	float m_fLastUpdateSpeAtk4;
	float m_fLastUpdateSpeAtk5;
};

class MoonWarLordMonster : public GameMonster
{
public:
	MoonWarLordMonster();

public:
	virtual void OnObjectAction(const PkgObjectActionNot& not);
	virtual void Update(float _dt);
	virtual void Render();
	virtual bool IsLastFrame();
	virtual void OnPacket(const PacketHeader* _pPkt);

public:
	void DoPacket(const PkgPlayerPlayAniAck& ack);

protected:
	float m_fLastUpdateSpeAtk1;
	float m_fLastUpdateSpeAtk2;
	float m_fLastUpdateSpeAtk3;
	float m_fLastUpdateSpeAtk4;
	float m_fLastUpdateSpeAtk5;
	DWORD m_dwDefenceEffExpireTime;
	DWORD m_dwLastUpdateDefenceEffTime;
};

class YamaKingMonster : public GameMonster
{
public:
	YamaKingMonster();

public:
	virtual void OnPacket(const PacketHeader* _pPkt);
	virtual bool IsLastFrame();
	virtual void Render();
	virtual void Update(float _dt);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

public:
	void DoPacket(const PkgPlayerPlayAniAck& ack);

protected:
	float m_fLastUpdateSpeAtk1;
	float m_fLastUpdateSpeAtk2;
	float m_fLastUpdateSpeAtk3;
	float m_fLastUpdateSpeAtk4;
	PLAYER_STATUS m_eEndStatus;
	int m_nLastAtkTarget;
};

class FireDragonMonster : public GameMonster
{
public:
	FireDragonMonster();

public:
	virtual void OnPacket(const PacketHeader* _pPkt);
	virtual void Render();
	virtual void Update(float _dt);
	virtual void OnObjectAction(const PkgObjectActionNot& not);
	virtual bool IsLastFrame();
	virtual int CalMonIndex();

public:
	void DoPacket(const PkgMonsterFireDragonAttack0Not& not);
	void DoPacket(const PkgPlayerPlayAniAck& ack);

	void PlayAttack1Effect();

protected:
	float m_fLastUpdateSpeAtk1;
	int m_nLastAtkTarget;
	list<PkgMonsterFireDragonAttack0Not> m_xAttack0List;
	list<PkgPlayerPlayAniAck> m_xAttack1List;
};

class SuperBullMonster : public GameMonster
{
public:
	SuperBullMonster();

public:
	virtual bool IsLastFrame();
	virtual void Render();
	virtual void Update(float _dt);
	virtual void OnObjectAction(const PkgObjectActionNot& not);

private:
	float m_fLastUpdateSpeAtk1;
};
//////////////////////////////////////////////////////////////////////////
#endif