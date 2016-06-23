#ifndef _INC_MAGICEFFECT_
#define _INC_MAGICEFFECT_
//////////////////////////////////////////////////////////////////////////
#include "../GameScene/GameInfoManager.h"
#include "../../CommonModule/MagicEffectID.h"
#include <hgesprite.h>
//////////////////////////////////////////////////////////////////////////
extern const char* g_szMgcDescriptor[MEFF_USERTOTAL];
//////////////////////////////////////////////////////////////////////////
//	不画
#define MAGIC_STAGE_NONE		0
//	在自身的帧
#define MAGIC_STAGE_SELF		1
//	在过程中的帧
#define MAGIC_STAGE_WAY			2
//	在目标的帧
#define MAGIC_STAGE_DEST		3
//////////////////////////////////////////////////////////////////////////
struct MagicRenderInfo
{
	int id;

	BYTE wil;

	UINT selfbegin;
	BYTE selfframe;
	BYTE selftotal;

	UINT waybegin;
	BYTE wayframe;
	BYTE waytotal;

	UINT destbegin;
	BYTE destframe;
	BYTE desttotal;

	WORD speed;

	USHORT sndindex;
};

struct MagicDetail
{
	char cKey;
	char cAccKey;
	const char* szName;
	DWORD dwTrain;
	char cLevel;
	WORD wID;
};

struct EffectHumInfo
{
	int id;

	BYTE wil;

	UINT effectbegin;
	BYTE effectframe;

	WORD speed;
};


/************************************************************************/
/* Magic element
/************************************************************************/
class MagicElement
{
public:
	MagicElement();
	virtual ~MagicElement();

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

public:
	inline BYTE GetStage()					{return m_bStage;}
	inline void SetActive()					{m_bActive = true;}
	inline WORD GetPosX()					{return (WORD)m_fFlyX;}
	inline WORD GetPosY()					{return (WORD)m_fFlyY;}
	inline void SetPosX(float _fX)			{m_fFlyX = _fX;}
	inline void SetPosY(float _fY)			{m_fFlyY = _fY;}
	inline bool IsFixed()					{return m_bFixed;}
	inline void SetFixed(bool _bFix)		{m_bFixed = _bFix;}
	inline bool IsActive()					{return m_bActive;}
	inline bool CanRemove()					{return m_bCanRemove;}
	inline const MagicRenderInfo* GetRenderInfo()		{return m_pRenderInfo;}
	inline DWORD GetLastUpdateTime()		{return m_dwLastUpdateTime;}
	inline void SetLastUpdateTime(DWORD _dwTick)	{m_dwLastUpdateTime = _dwTick;}
	inline void IncFrame()					{++m_bCurFrame;}
	inline void ResetFrame()				{m_bCurFrame = 0;}
	inline BYTE GetCurFrame()				{return m_bCurFrame;}
	inline DWORD GetSelfInterval()			{return m_dwSelfInterval;}
	inline void SetSelfInterval(DWORD _dwInterval)	{m_dwSelfInterval = _dwInterval;}
	inline void SetStage(BYTE _bStg)		{m_bStage = _bStg;}
	inline DWORD GetDestX()					{return m_sDestX;}
	inline DWORD GetDestY()					{return m_sDestY;}
	inline DWORD GetMagicID()				{return m_wMgcID;}
	inline GameObject* GetFire()			{return m_pFire;}
	inline GameObject* GetTarget()			{return m_pTarget;}
	inline bool IsSelfLastFrame()			{return m_bCurFrame == m_pRenderInfo->selfframe - 1;}
	inline void EnableSound()				{m_bSnd = true;}
	inline bool IsSoundEnable()				{return m_bSnd;}
	inline void SetHumEff(bool _b)			{m_bIsHumEff = _b;}
	inline bool IsHumEff()					{return m_bIsHumEff;}
	inline DWORD GetCreateTime()			{return m_dwCreateTime;}
	inline void SetCreateTime(DWORD _dwTime){m_dwCreateTime = _dwTime;}
	inline DWORD GetAniTime()				{return m_dwAniTime;}
	inline void SetAniTime(DWORD _dwTime)	{m_dwAniTime = _dwTime;}
	inline void SetRenderOftX(int _nOft)	{m_nRenderOftX = _nOft;}
	inline void SetRenderOftY(int _nOft)	{m_nRenderOftY = _nOft;}

	bool OverThrough(int _old, int _new);
	bool InPlayRegion();

protected:
	short m_sFireX;
	short m_sFireY;
	short m_sDestX;
	short m_sDestY;
	
	float m_fFlyX;
	float m_fFlyY;

	GameObject* m_pFire;
	GameObject* m_pTarget;

	BYTE m_bDrt16;
	BYTE m_bCurFrame;

	bool m_bCanRemove;
	bool m_bActive;
	bool m_bFixed;
	bool m_bSnd;

	DWORD m_dwLastUpdateTime;
	DWORD m_dwLastUpdateWayTime;
	DWORD m_dwAniTime;
	WORD m_wMgcID;
	BYTE m_bStage;

	DWORD m_dwSelfInterval;
	DWORD m_dwCreateTime;

	float m_fDisX;
	float m_fDisY;
	int m_nDisX;
	int m_nDisY;

	int m_nPrevDisX;
	int m_nPrevDisY;

	const MagicRenderInfo* m_pRenderInfo;

	bool m_bIsHumEff;

	int m_nRenderOftX;
	int m_nRenderOftY;


public:
	static hgeSprite* pEffectRender;
	static hgeSprite* pNormalRender;
};

typedef std::list<MagicElement*> MagicElementList;
//////////////////////////////////////////////////////////////////////////
class MagicFireBall : public MagicElement
{
public:
	MagicFireBall(){}
	virtual ~MagicFireBall(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();
};

//////////////////////////////////////////////////////////////////////////
class MagicFireWall : public MagicElement
{
public:
	MagicFireWall(){}
	virtual ~MagicFireWall(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();
};
//////////////////////////////////////////////////////////////////////////
class MagicShield : public MagicElement
{
public:
	MagicShield(){}
	virtual ~MagicShield(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	//virtual void Render();
};
//////////////////////////////////////////////////////////////////////////
class MagicEnergyShield : public MagicShield
{
public:
	virtual void Update();
};
//////////////////////////////////////////////////////////////////////////
class MagicHeal : public MagicElement
{
public:
	MagicHeal(){}
	virtual ~MagicHeal(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();
};
//////////////////////////////////////////////////////////////////////////
class MagicPoison : public MagicHeal
{
public:
	MagicPoison(){}
	virtual ~MagicPoison(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();
};
//////////////////////////////////////////////////////////////////////////
class MagicTempt : public MagicElement
{
public:
	MagicTempt(){}
	virtual ~MagicTempt(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();
};
//////////////////////////////////////////////////////////////////////////
class MagicHide : public MagicElement
{
public:
	MagicHide(){}
	virtual ~MagicHide(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
};
//////////////////////////////////////////////////////////////////////////
class MagicDoor : public MagicElement
{
public:
	MagicDoor();
	virtual ~MagicDoor();

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();
};
//////////////////////////////////////////////////////////////////////////
#define STATICATTACK_SHENSHOU	1
class GameTextureManager;
struct RenderInfo;
class MagicStaticAttackEffect : public MagicElement
{
public:
	MagicStaticAttackEffect();
	virtual ~MagicStaticAttackEffect();

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	GameTextureManager* m_pTexMgr;
	const RenderInfo* m_pEffInfo;
	int m_nDrt;
};
//////////////////////////////////////////////////////////////////////////
class Magic16DrtSelf : public MagicElement
{
public:
	Magic16DrtSelf();
	~Magic16DrtSelf(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	GameTextureManager* m_pTexMgr;
	const RenderInfo* m_pEffInfo;
};
//////////////////////////////////////////////////////////////////////////
class Magic16DrtWay : public MagicElement
{
public:
	Magic16DrtWay();
	virtual ~Magic16DrtWay(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	GameTextureManager* m_pTexMgr;
	const RenderInfo* m_pEffInfo;
};
//////////////////////////////////////////////////////////////////////////
class Magic16DrtExplosion : public MagicElement
{
public:
	Magic16DrtExplosion();

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	GameTextureManager* m_pTexMgr;
	const RenderInfo* m_pEffInfo;
	int m_nMagicIndex;
	int m_nWayFrameBegin;
	int m_nWayFrames;
	int m_nDestFrameBegin;
	int m_nDestFrames;
};
//////////////////////////////////////////////////////////////////////////
class MagicGroundThron : public MagicElement
{
public:
	MagicGroundThron();
	virtual ~MagicGroundThron(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	GameTextureManager* m_pTexMgr;
	const RenderInfo* m_pEffInfo;
};
//////////////////////////////////////////////////////////////////////////
class MagicNormalThron : public MagicElement
{
public:
	MagicNormalThron();
	virtual ~MagicNormalThron(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	GameTextureManager* m_pTexMgr;
	int m_nEffectBegin;
	int m_nEffectFrames;
};
//////////////////////////////////////////////////////////////////////////
class MagicLionRoar : public MagicElement
{
public:
	MagicLionRoar();
	virtual ~MagicLionRoar(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
};
//////////////////////////////////////////////////////////////////////////
class MagicSuperFire : public MagicElement
{
public:
	MagicSuperFire();
	virtual ~MagicSuperFire(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Render();
	virtual void Update();
};
//////////////////////////////////////////////////////////////////////////
class MagicKTSword : public MagicSuperFire
{
public:
	virtual void Render();
};
//////////////////////////////////////////////////////////////////////////
class MagicMonsAni : public MagicElement
{
public:
	MagicMonsAni();
	virtual ~MagicMonsAni(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	GameTextureManager* m_pTexMgr;
	const RenderInfo* m_pMonsRenderInfo;
};
//////////////////////////////////////////////////////////////////////////
class MagicMonsPosAni : public MagicElement
{
public:
	MagicMonsPosAni();
	virtual ~MagicMonsPosAni(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	GameTextureManager* m_pTexMgr;
	const RenderInfo* m_pMonsRenderInfo;
	DWORD m_dwPos;
};
//////////////////////////////////////////////////////////////////////////
class MagicHumEff : public MagicElement
{
public:
	MagicHumEff();
	virtual ~MagicHumEff(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	GameTextureManager* m_pTexMgr;
};
//////////////////////////////////////////////////////////////////////////
class MagicHumState : public MagicElement
{
public:
	MagicHumState();
	virtual ~MagicHumState(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	GameTextureManager* m_pTexMgr;
};
//////////////////////////////////////////////////////////////////////////
class MagicBigPoison : public MagicElement
{
public:
	MagicBigPoison(){}
	virtual ~MagicBigPoison(){}

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY, DWORD _dwAniTime);
	virtual void Update();
	virtual void Render();

private:
	bool m_bFirstLoop;
};
//////////////////////////////////////////////////////////////////////////
class EffectAttackNumber : public MagicElement
{
public:
	EffectAttackNumber()
	{
		m_nNumber = 0;
		m_nOffsetY = 0;
	}
	virtual ~EffectAttackNumber(){}

public:
	bool Init(int _nNumber, DWORD _dwCoordX, DWORD _dwCoordY, bool _bAdd, bool _bCritical);
	virtual void Update();
	virtual void Render();

	void SetHero(bool _bHero)
	{
		m_bIsHero = _bHero;
	}
	inline void SetOffsetY(int _nOft)
	{
		m_nOffsetY = _nOft;
	}
	inline int GetOffsetY()
	{
		return m_nOffsetY;
	}
	int GetRenderPosY();

private:
	int m_nNumber;
	DWORD m_dwCoordX;
	DWORD m_dwCoordY;
	std::list<int> m_xNumbers;
	bool m_bAdd;
	bool m_bCritical;
	bool m_bIsHero;
	int m_nBits;
	DWORD m_dwAlpha;
	int m_nOffsetY;

	static hgeSprite* s_pSharedEffectRender;
	static HTEXTURE s_pSharedNumberTexture;
};
//////////////////////////////////////////////////////////////////////////
class EffectHumCloth : public MagicElement
{
public:
	EffectHumCloth()
	{
		m_nClothID = 0;
		m_pEffectHumInfo = NULL;
		m_dwAniInterval = 0;
		m_dwNextExecuteTime = 0;
	}
	virtual ~EffectHumCloth(){}

public:
	bool Init(GameObject* _pAttachObj, int _nClothID);
	inline void SetInterval(DWORD _dwInterval)
	{
		m_dwAniInterval = _dwInterval;
	}
	virtual void Update();
	virtual void Render();

protected:
	int m_nClothID;
	EffectHumInfo* m_pEffectHumInfo;
	DWORD m_dwAniInterval;
	DWORD m_dwNextExecuteTime;
};
//////////////////////////////////////////////////////////////////////////
class SequenceEffect : public MagicElement
{
public:
	SequenceEffect()
	{
		m_nResID = 0;
		m_nTextStartID = 0;
		m_nFrames = 0;
		m_nSpeed = 0;
		m_nLoopTimes = 0;
		m_pResPack = NULL;
		m_nCurrentLoopTimes = 0;
	}
	virtual ~SequenceEffect(){}

public:
	bool Init(int _nResID, int _nTextStartID, int _nFrames, int _nSpeed, int _nLoopTimes);
	virtual void Update();
	virtual void Render();

protected:
	int m_nResID;
	int m_nTextStartID;
	int m_nFrames;
	int m_nSpeed;
	int m_nLoopTimes;
	int m_nCurrentLoopTimes;
	GameTextureManager* m_pResPack;
};
//////////////////////////////////////////////////////////////////////////
#endif