#ifndef GAMERENDEROBJECT_H_
#define GAMERENDEROBJECT_H_

#include "../Common/GlobalDefine.h"
#include <Windows.h>
#include <list>
#include <vector>
#include <hge.h>
#include "../GameScene/GameTextureManager.h"
#include "../Common/GlobalFunction.h"
#include <hgesprite.h>
#include "../../CommonModule/ObjectData.h"
#include "../Common/GfxFont.h"
#include <algorithm>
#include "../Common/ResourceID.h"
#include "../Net/Packet.h"
#include "../Common/TextPrinter.h"
#include "../Net/GamePacketHandler.h"
#include <atlbase.h>
#include "../GameScene/GameSoundManager.h"
#include "../../CommonModule/MagicEffectID.h"
//////////////////////////////////////////////////////////////////////////
#define INVALID_TEXTURE_INDEX	-1

#define COLOR_EFF_STONE			ARGB(150, 100, 100, 100)
#define COLOR_EFF_ICE			ARGB(96, 75, 255, 255)

#define OBJECT_CHAT_EACHLINE	(8 * 12)
//////////////////////////////////////////////////////////////////////////
#define MOVE_WALK		0
#define MOVE_RUN		1
//////////////////////////////////////////////////////////////////////////
extern const int g_nMoveOftTable[16];
extern const DWORD g_dwMonsLvColor[8];
extern const DWORD g_dwItemColor[9];
//////////////////////////////////////////////////////////////////////////

class RenderObject;

using std::list;
using std::vector;
typedef list<RenderObject*> OBJECTLIST;
class EffectAttackNumber;
typedef list<EffectAttackNumber*> EffectAttackNumberList;

class RenderObject
{
public:
	RenderObject()
	{
		static UINT id = 0;
		m_id = id++;
		m_bCurFrame = 0;
		m_bVisible = true;
		m_bFocus = false;
		ZeroMemory(&m_rcClient, sizeof(m_rcClient));
		m_pRender = NULL;
		m_bIsStatic = false;
		m_bNeedTopLevel = false;
		m_bSwallowEvent = false;
	}
	virtual ~RenderObject()
	{
		SAFE_DELETE(m_pRender);
	}

public:
	virtual void Render() = 0;
	virtual void Update(float _dt) = 0;
	virtual bool IsLastFrame(){return true;}
	virtual bool ProcUserCmd(const POINT& _mp)
	{
		return true;
	}
public:
	virtual bool LoadProperty(int _id){return true;}
	virtual int OnLButtonDown(){return 0;}
	virtual int OnRButtonDown(){return 0;}
	virtual void ResetTimerCounter(){}
	virtual void ResetStatus(){}

public:
	inline bool IsStatic()
	{
		return m_bIsStatic;
	}
	inline void SetStatic(bool _s)
	{
		m_bIsStatic = _s;
	}
	inline void SetVisible(bool _vb)
	{
		if(!_vb)
		{
			//m_bFocus = false;
		}
		m_bVisible = _vb;
	}
	inline bool IsVisible() const
	{
		return m_bVisible;
	}
	inline void SetRect(const RECT& rc)
	{
		m_rcClient = rc;
	}
	inline const RECT& GetRect()
	{
		return m_rcClient;
	}
	inline BYTE GetCurFrame() const
	{
		return m_bCurFrame;
	}
	inline void SetCurFrame(BYTE _frm)
	{
		m_bCurFrame = _frm;
	}
	inline UINT GetID() const
	{
		return m_id;
	}
	inline bool IsPtInObject(const POINT& _pt)
	{
		bool bInObject = false;
		bInObject = ::PtInRect(&m_rcClient, _pt) ? true : false;
		return bInObject;
	}
	inline bool IsNeedTopLevel()
	{
		return m_bNeedTopLevel;
	}
	inline void SetNeedTopLevel(bool _b)
	{
		m_bNeedTopLevel = _b;
	}
	inline bool IsSwallowEvent()
	{
		return m_bSwallowEvent;
	}
	inline void SetSwallowEvent(bool _b)
	{
		m_bSwallowEvent = _b;
	}
	inline hgeSprite* GetRender()
	{
		return m_pRender;
	}
	inline int GetIDInt()
	{
		return (int)m_id;
	}

protected:
	//	是否可见
	bool m_bVisible;
	//	物体矩形
	RECT m_rcClient;
	//	当前帧
	BYTE m_bCurFrame;
	//RECT m_rcObject;
	//	是否获取焦点
	bool m_bFocus;
	//	绘图精灵
	hgeSprite* m_pRender;
	//	是否静态(对于对话框来说 静态可以阻止用户输入)
	bool m_bIsStatic;
	//	是否需要提到顶层
	bool m_bNeedTopLevel;
	//	是否吞消息(拖拉窗口)
	bool m_bSwallowEvent;
private:
	//	物体ID
	UINT m_id;
};


class RenderTarget : public RenderObject
{
public:
	RenderTarget(){}
	~RenderTarget(){}

public:
	virtual void Update(float _dt)
	{
		for(OBJECTLIST::const_iterator iter = m_objList.begin(); iter != m_objList.end(); ++iter)
		{
			(*iter)->Update(_dt);
		}
	}
	virtual void Render()
	{
		for(OBJECTLIST::const_iterator iter = m_objList.begin(); iter != m_objList.end(); ++iter)
		{
			if((*iter)->IsVisible())
			{
				(*iter)->Render();
			}
		}
	}

public:
	virtual void InsertObject(RenderObject* _obj)
	{
		m_objList.push_back(_obj);
	}
	virtual RenderObject* RemoveObject(int _id)
	{
		RenderObject* tobj = NULL;
		for(OBJECTLIST::iterator iter = m_objList.begin(); iter != m_objList.end(); ++iter)
		{
			if((*iter)->GetID() == _id)
			{
				tobj = (*iter);
				m_objList.erase(iter);
				break;
			}
		}
		return tobj;
	}
	void BringToTop(RenderObject* _obj)
	{
		OBJECTLIST::iterator iter = std::find(m_objList.begin(), m_objList.end(), _obj);
		if(iter != m_objList.end())
		{
			m_objList.erase(iter);
		}
		m_objList.push_back(_obj);
	}

protected:
	OBJECTLIST m_objList;
};

//////////////////////////////////////////////////////////////////////////
enum OBJECT_RENDER_MODE
{
	ORM_NONE,
	ORM_NORMAL,
	ORM_TRANSPARENT,
	ORM_HIGHLIGHT
};

class GameObject : public RenderObject, public PacketHandler
{
	//	基本信息
public:
	GameObject()
	{
		m_fPosx = m_fPosy = 0;
		m_type = OBJ_UNKNOW;
		m_drt = PDT_DOWN;
		m_sex = PSX_MAN;
		m_preStat = m_stat = PST_STAND;
		ZeroMemory(&m_ptPos, sizeof(POINT));
		ZeroMemory(&m_attrib, sizeof(m_attrib));
		m_fLastStandTime = 0;
		m_bRenderName = false;
		m_bHurt = false;
		m_nCurrentTextureIndex = -1;
		m_nCurrentSelectIndex = -1;
		ZeroMemory(&m_rcSelectRect, sizeof(RECT));
		ResetSaying();

		ZeroMemory(m_dwHumEffectTime, sizeof(m_dwHumEffectTime));
		m_dwHumEffectFlag = 0;

		m_pMaster = NULL;
		m_bTransparentMode = false;

		ZeroMemory(m_szMasterName, sizeof(m_szMasterName));

		m_dwLastMoveTime = 0;
		m_dwSayingColor = 0;
	}
	virtual ~GameObject();

public:
	inline float GetObjx() const
	{
		return m_fPosx;
	}
	inline float GetObjy() const
	{
		return m_fPosy;
	}
	inline void SetObjx(float _px)
	{
		m_fPosx = _px;
	}
	inline void SetObjy(float _py)
	{
		m_fPosy = _py;
	}
	inline void SetPosxOffset(float _oft)
	{
		m_fPosx += _oft;
	}
	inline void SetPosyOffset(float _oft)
	{
		m_fPosy += _oft;
	}
	inline int GetShiftX() const
	{
		return (int)m_fPosx % UNIT_WIDTH;
	}
	inline int GetShiftY() const
	{
		return (int)m_fPosy % UNIT_HEIGHT;
	}
	inline void SetDirection(PLAYER_DIRECTION _pd)
	{
		m_drt = _pd;
	}
	inline PLAYER_DIRECTION GetDirection() const 
	{
		return m_drt;
	}
	inline void SetStatus(PLAYER_STATUS _ps)
	{
		m_stat = _ps;
		m_bCurFrame = 0;
		if(_ps == PST_STAND)
		{
			m_fLastStandTime = AfxGetHge()->Timer_GetTime();
		}
		OnSetStatus(_ps);
	}
	inline PLAYER_STATUS GetStatus() const
	{
		return m_stat;
	}
	inline PLAYER_STATUS GetPreStatus() const
	{
		return m_preStat;
	}
	inline void SetOffsetCoordX(int _x)
	{
		m_ptPos.x += _x;
	}
	inline void SetOffsetCoordY(int _y)
	{
		m_ptPos.y += _y;
	}
	inline int GetCoordX() const
	{
		return m_ptPos.x;
	}
	inline int GetCoordY() const 
	{
		return m_ptPos.y;
	}
	inline int GetReallyCoordX() const
	{
		//	获取所在格子实际的坐标
		return (int)m_fPosx / UNIT_WIDTH;
	}
	inline int GetReallyCoordY() const
	{
		return (int)m_fPosy / UNIT_HEIGHT;
	}
	inline void SetReallyCoord(int _x, int _y)
	{
		m_ptPos.x = _x;
		m_ptPos.y = _y;
		m_fPosx = (float)_x * UNIT_WIDTH;
		m_fPosy = (float)_y * UNIT_HEIGHT;
	}
	inline void SetPlayerSex(PLAYER_SEX _ps)
	{
		m_sex = _ps;
	}
	inline PLAYER_SEX GetPlayerSex()
	{
		return m_sex;
	}
	inline ItemAttrib* GetAttrib()
	{
		return &m_attrib;
	}
	bool IsMoving()
	{
		if(GetObjx() == GetCoordX() * UNIT_WIDTH &&
			GetObjy() == GetCoordY() * UNIT_HEIGHT)
		{
			return false;
		}
		return true;
	}
	inline int GetSpeed() const
	{
		return m_attrib.moveSpeed;
	}
	inline void SetSpeed(int _spd)
	{
		m_attrib.moveSpeed = _spd;
	}
	inline float GetLastStandTime() const
	{
		return m_fLastStandTime;
	}
	inline OBJECT_TYPE GetType() const
	{
		return m_type;
	}
	inline void SetType(OBJECT_TYPE _type)
	{
		m_type = _type;
	}
	inline void SetRenderName()
	{
		m_bRenderName = true;
	}
	inline const char* GetName()
	{
		return m_attrib.name;
	}
	inline const POINT& GetPosition()
	{
		return m_ptPos;
	}
	inline void SetHurt()
	{
		m_bHurt = true;
	}
	inline float GetAttackInterval()
	{
		float fAtkInterval = ((float)MAX_ATTACK_INTERVAL - (float)m_attrib.atkSpeed * 5) / 1000;
		if(fAtkInterval < 0.0f)
		{
			fAtkInterval = 0.05f;
		}
		return fAtkInterval;
	}
	inline float GetMovePerSecond()
	{
		float fMoveInterval = ((float)MAX_MOVE_INTERVAL + (float)m_attrib.moveSpeed * 10);

		if(GetType() == OBJ_PLAYER ||
			GetType() == OBJ_OTHERPLAYER)
		{
			if(fMoveInterval > 220.0f)
			{
				fMoveInterval = 220.0f;
			}
		}
		
		return fMoveInterval;
	}
	void Dead()
	{
		SetStatus(PST_DEAD);
	}
	void DecreaseHealth(int _dam)
	{
		if(_dam >= m_attrib.HP)
		{
			m_attrib.HP = 0;
			Dead();
		}
		else
		{
			m_attrib.HP -= _dam;
		}
	}
	void DecreaseSpell(int _dec)
	{
		if(_dec >= m_attrib.MP)
		{
			m_attrib.MP = 0;
			Dead();
		}
		else
		{
			m_attrib.MP -= _dec;
		}
	}
	inline void ResetSaying()
	{
		m_fSayingTime = 0.0f;
		m_szSaying[0] = 0x0000;
	}
	inline DWORD GetSayingColor()
	{
		return m_dwSayingColor;
	}
	inline void SetSayingColor(DWORD _dwColor)
	{
		m_dwSayingColor = _dwColor;
	}

	inline int GetCurrentTextureIndex()
	{
		return m_nCurrentTextureIndex;
	}
 
	bool SetSaying(const char* _pszWords);
	bool MoveToDestination(WORD _wCoordX, WORD _wCoordY, BYTE _bWay);

	bool UpdateHide();
	//	获得声音索引
	virtual int GetSoundIndex();

	virtual bool IsSelectedEx(const POINT& _pt);

	inline OBJECT_RENDER_MODE GetRenderMode()			{return m_eRenderMode;}
	inline void SetRenderMode(OBJECT_RENDER_MODE _eMode)		{m_eRenderMode = _eMode;}

	void GetMinAroundRect(RECT& _stOut);
	void GetMaxAroundRect(RECT& _stOut);

	int GetScreenPosX();
	int GetScreenPosY();

	void DrawTexture(int _nResID, int _nIndex, int _nPosX, int _nPosY, bool _bAlpha);

	//	网络包处理
public:
	virtual void OnPacket(const PacketHeader* _pPkt);

	void DoPacket(const PkgObjectActionNot& _not);
	void DoPacket(const PkgPlayerSayReq& req);
	void DoPacket(const PkgObjectActionTurnNot& _not);
	void DoPacket(const PkgObjectActionWalkNot& _not);
	void DoPacket(const PkgObjectActionRunNot& _not);
	void DoPacket(const PkgObjectActionAttackNot& _not);
	void DoPacket(const PkgObjectActionDeadNot& _not);
	void DoPacket(const PkgObjectActionExtNot& _not);

public:
	virtual void Render(){}
	virtual void TransparentRender();
	virtual void HighlightRender();
	virtual void Update(float _dt){}
	virtual bool IsSelected(const POINT& _pt);
	virtual void OnSetStatus(PLAYER_STATUS _ps){}
	virtual void UpdateMove(){}
	virtual void UpdateSelectRect(int _idx);
	virtual void OnObjectAction(const PkgObjectActionNot& not){}

	inline void SetMaster(GameObject* _pObj)
	{
		m_pMaster = _pObj;
		strcpy(m_szMasterName, m_pMaster->GetAttrib()->name);
	}
	inline GameObject* GetMaster()
	{
		return m_pMaster;
	}


public:
	//	绘制名字
	virtual void RenderName();
	//	绘制血条
	virtual void RenderHPBar();
	//	绘制发言
	virtual void RenderSaying();
	//	获得资源索引
	int GetTextureMgrIndex();
	//	绘制攻击数字
	virtual void RenderAttackNumber();

	void SetHumEffect(DWORD _dwMask, DWORD _dwTime);
	void ResetHumEffect(DWORD _dwMask);

	void AddAttackNumber(int _nNumber, bool _bCritical);
	void ClearAttackNumberList();

protected:
	//	玩家所在坐标(实时) 以1*1 像素为单位
	float m_fPosx;
	float m_fPosy;
	//	玩家所在的坐标（移动过后）以48*32为单位
	POINT m_ptPos;
	OBJECT_TYPE m_type;
	PLAYER_DIRECTION m_drt;
	PLAYER_SEX m_sex;
	PLAYER_STATUS m_stat;
	PLAYER_STATUS m_preStat;
	//	人物属性
	ItemAttrib m_attrib;
	//	上一次站姿势的时间
	float m_fLastStandTime;
	//	是否绘制名字
	bool m_bRenderName;
	//	是否受伤
	bool m_bHurt;

	//	当前纹理索引
	int m_nCurrentTextureIndex;
	//	当前选取矩形索引
	int m_nCurrentSelectIndex;
	RECT m_rcSelectRect;

	//	动作数据包
	//ACTIONLIST m_lstActPacket;

	//	发言
	wchar_t m_szSaying[MAX_SAY_LENGTH];
	float m_fSayingTime;
	SIZE m_stSayingSize;
	DWORD m_dwLineNumber;
	DWORD m_dwSayingColor;

	//	魔法效果
	DWORD m_dwHumEffectFlag;
	DWORD m_dwHumEffectTime[MMASK_TOTAL];

	//	主人
	GameObject* m_pMaster;
	char m_szMasterName[20];

	//	透明绘制模式
	bool m_bTransparentMode;
	OBJECT_RENDER_MODE m_eRenderMode;

	DWORD m_dwLastMoveTime;

	//	攻击数字列表
	EffectAttackNumberList m_xAttackNumberList;
};

typedef std::list<GameObject*> PLAYERLIST;

#endif