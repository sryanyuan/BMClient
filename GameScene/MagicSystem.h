#ifndef _INC_MAGICSYSTEM_
#define _INC_MAGICSYSTEM_
//////////////////////////////////////////////////////////////////////////
/*
#include "../Common/GameRenderObject.h"
#include "GamePlayer.h"
#include <list>
//////////////////////////////////////////////////////////////////////////
//	不画
#define MAGIC_STAGE_NONE		0
//	在自身的帧
#define MAGIC_STAGE_SELF		1
//	在过程中的帧
#define MAGIC_STAGE_WAY			2
//	在目标的帧
#define MAGIC_STAGE_DEST		3

#define MAGIC_MOVE_OFFSET		20.0f
#define RENDER_OFFSET			128

class MagicObject;
typedef void (*LASTFRAME_FUNC)(MagicObject*);
//////////////////////////////////////////////////////////////////////////

/ *
 *	魔法分类:
 *	1.有咏唱帧 有行进帧 有爆炸帧 : MagicFireball
 *	2.有咏唱帧 无行进帧 有爆炸帧 : MagicIceRoar
 *	3.有咏唱帧 无进行帧 无爆炸帧 : MagicHeal
 *	4.状态魔法 即战士的攻击效果 法师的魔法盾
 *	1,2,3类加入魔法系统控制 4封装入游戏物体内
 *	_nMgcId为魔法ID _nId为标识符
 *	魔法产生后，关键帧会回调LASTFRAME_FUNC，用于请求服务器数据
 * /

//////////////////////////////////////////////////////////////////////////
#define MAGIC_LOW_GONGSHA	0x00000001
#define MAGIC_LOW_CISHA		0x00000002
#define MAGIC_LOW_BANYUE	0x00000004
#define MAGIC_LOW_LIEHUO	0x00000008*/

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Render define
/************************************************************************/
/*
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
};
struct MagicInfo
{
	MagicRenderInfo* prenderinfo;

	GameObject* powner;
	GameObject* ptarget;
	WORD srctilex;
	WORD srctiley;
	WORD desttilex;
	WORD desttiley;
	float curx;
	float cury;

	BYTE curframe;
	BYTE stage;
	BYTE atkstage;

	BYTE drt;
	DWORD looptime;
	DWORD updatetotaltime;
	DWORD lastupdatetime;
	DWORD createtime;

	DOUBLE angle;
	BYTE end;
};*/


/************************************************************************/
/* Magic element
/************************************************************************/
/*
class MagicElement
{
public:
	MagicElement();
	~MagicElement();

public:
	virtual bool Create(int _nMgcID, GameObject* _pFire, GameObject* _pTarget, short _sDestX, short _sDestY);
	virtual bool Update();

protected:
	short m_sFireX;
	short m_sFireY;
	short m_sDestX;
	short m_sDestY;
	short m_sFlyX;
	short m_sFlyY;

	GameObject* m_pFire;
	GameObject* m_pTarget;

	BYTE m_bDrt16;

	bool m_bCanRemove;
	bool m_bFixed;

	DWORD m_dwLastUpdateTime;
};*/


/************************************************************************/
/* Magic element
/************************************************************************/
/*
class GameTextureManager;
class MagicObject : public RenderObject
{
	friend class MagicSystem;
public:
	MagicObject(int _nId, int _nMgcId);
	~MagicObject();

public:
	virtual void Update(float _dt);
	virtual void Render();

	virtual bool Init();

public:
	inline MagicInfo* GetRenderInfo()				{return &m_stInfo;}
	inline BYTE CanRemove()								{return m_stInfo.end;}
	inline void SetDamageHook(LASTFRAME_FUNC _func)		{m_pFunc = _func;}

	bool CheckValid()
	{
		if(!m_stInfo.psrc &&
			!m_stInfo.pdest &&
			m_stInfo.destx == 0 &&
			m_stInfo.desty == 0 &&
			m_stInfo.srcx == 0 &&
			m_stInfo.srcy == 0)
		{
			return false;
		}
		return true;
	}
	void UpdateValid()
	{
		m_bValid = CheckValid();
		if(m_bValid)
		{
			GamePlayer* pPlayer = GamePlayer::GetInstance();
			if(m_stInfo.psrc)
			{
				m_stInfo.srcx = m_stInfo.psrc->GetObjx();
				m_stInfo.srcy = m_stInfo.psrc->GetObjy();
			}
			if(m_stInfo.pdest)
			{
				m_stInfo.destx = m_stInfo.pdest->GetObjx() - pPlayer->GetObjx() + CENTRAL_X;
				m_stInfo.desty = m_stInfo.pdest->GetObjy() - pPlayer->GetObjy() + CENTRAL_Y;
			}
			else
			{
				m_stInfo.destx = m_stInfo.destx + pPlayer->GetObjx() - CENTRAL_X;
				m_stInfo.desty = m_stInfo.desty + pPlayer->GetObjy() - CENTRAL_Y;
			}
		}
	}
	float MapPositionToScreenPositionX(float _fIn)
	{
		float fOut = 0.0f;
		fOut = _fIn - GamePlayer::GetInstance()->GetObjx() + CENTRAL_X;
		return fOut;
	}
	float MapPositionToScreenPositionY(float _fIn)
	{
		float fOut = 0.0f;
		fOut = _fIn - GamePlayer::GetInstance()->GetObjy() + CENTRAL_Y;
		return fOut;
	}
	bool InScreen(float _fX, float _fY)
	{
		if(_fX >= 0 - RENDER_OFFSET &&
			_fX <= VIEW_WIDTH + RENDER_OFFSET &&
			_fY >= 0 - RENDER_OFFSET &&
			_fY <= VIEW_HEIGHT + VIEW_OPERATE + RENDER_OFFSET)
		{
			return true;
		}
		return false;
	}

protected:
	MagicInfo m_stInfo;
	//	for distinguish
	int m_nId;
	GameTextureManager* m_pTextureMgr;
	bool m_bValid;
	LASTFRAME_FUNC m_pFunc;

	//	static
	static hgeSprite* s_pSpr;
};

/ ************************************************************************ /
/ * inherit element
/ ************************************************************************ /
//	火球系列 相同的行为抽象 具有魔法移动动作
class MagicFireball : public MagicObject
{
public:
	MagicFireball(int _nId, int _nMgcId)
		: MagicObject(_nId, _nMgcId)
	{
		m_bDownMove = m_bRightMove = true;
		m_fDistance = 0.0f;
	}
public:
	virtual void Update(float _dt);

	virtual bool Init();

protected:
	bool m_bDownMove;
	bool m_bRightMove;
	float m_fDistance;
};

//	辅助系列
class MagicHeal : public MagicObject
{
public:
	MagicHeal(int _nId, int _nMgcId)
		: MagicObject(_nId, _nMgcId){}

public:
	virtual void Update(float _dt);
};

//	冰咆哮系列 无魔法移动动作
class MagicIceRoar : public MagicObject
{
public:
	MagicIceRoar(int _nId, int _nMgcId)
		: MagicObject(_nId, _nMgcId){}

public:
	virtual void Update(float _dt);

	virtual bool Init();
};

//	疾光电影系列
class MagicChainThunder : public MagicObject
{
public:
	MagicChainThunder(int _nId, int _nMgcId)
		: MagicObject(_nId, _nMgcId){}

public:
	virtual void Update(float _dt);
};

//	火墙系列 持续性魔法
class MagicFireWall : public MagicObject
{
public:
	MagicFireWall(int _nId, int _nMgcId)
		: MagicObject(_nId, _nMgcId){}

public:
	virtual void Update(float _dt);
	virtual void Render();
};


//////////////////////////////////////////////////////////////////////////
//	状态魔法系列
class MagicSwordKee : public MagicObject
{
public:
	MagicSwordKee(int _nMgcId, GameObject* _pObj)
		: MagicObject(-1, _nMgcId)
	{
		m_stInfo.psrc = _pObj;
	}

public:
	virtual void Update(float _dt);
	virtual void Render();

	virtual bool Init();

protected:
	GameObject* m_pObj;
};

//	魔法盾
class MagicShield : public MagicObject
{
public:
	MagicShield(int _nMgcId, GameObject* _pObj)
		: MagicObject(-1, _nMgcId)
	{
		m_stInfo.psrc = _pObj;
	}

public:
	virtual void Update(float _dt);

protected:
	GameObject* m_pObj;
};

/ ************************************************************************ /
/ * Magic factory
/ ************************************************************************ /
typedef std::list<MagicObject*> MagicObjectList;
//typedef std::map<MagicObject*> MagicObjectMap;
class MagicSystem : public RenderObject
{
protected:
	MagicSystem();
public:
	~MagicSystem();

public:
	//	static
	static MagicSystem* GetInstance()
	{
		static MagicSystem* s_pIns = NULL;
		if(s_pIns == NULL)
		{
			s_pIns = new MagicSystem;
			MagicObject::s_pSpr->SetBlendMode(BLEND_ALPHAADD | BLEND_NOZWRITE);
		}
		return s_pIns;
	}

public:
	//	创建普通魔法
	MagicObject* CreateMagic(int _nId, int _nMgcId);
	//	创建状态魔法 人物初始化的时候获取
	MagicObject* CreateStaticMagic(int _nMgcId, GameObject* _pObj);
	//	状态魔法不要加入魔法系统控制
	void PushMagic(MagicObject* _pMgc);
	//	移除非状态魔法
	void RemoveMagic(int _nId);

public:
	virtual void Render();
	virtual void Update(float _dt);

protected:
	MagicObjectList m_xObjects;
};*/


#endif