#ifndef GAMENPC_H_
#define GAMENPC_H_
#include "../Common/GameRenderObject.h"
#include "../Net/GamePacketHandler.h"

class GameNPC : public GameObject
{
public:
	GameNPC();
	virtual ~GameNPC();

public:
	virtual void Update(float _dt);
	virtual void Render();
	virtual int OnLButtonDown();
	virtual bool IsLastFrame();
	virtual bool LoadProperty(int _id);
	virtual void RenderName();
	//virtual bool IsSelected(const POINT& _pt);

public:
	const RenderInfo* GetRenderInfo()	{return m_pRenderInfo;}

public:
	virtual void OnPacket(const PacketHeader* _pPkt);

	void DoPacket(const PkgObjectActionNot& not);
	void DoPacket(const PkgPlayerClickNPCAck& not);
	void DoPacket(const PkgPlayerShowShopAck& ack);
	void DoPacket(const PkgPlayerInteractiveDialogContentNot& not);

protected:
	int CalRenderIndex();
	HTEXTURE GetCurTexture();

protected:
	//RenderInfo m_renderInfo;
	const RenderInfo* m_pRenderInfo;
	float m_fUpdateTime;
	hgeSprite* m_pSpr;
};

class MagicDoorNPC : public GameNPC
{
public:
	MagicDoorNPC();
	virtual ~MagicDoorNPC(){}

public:
	virtual void Update(float _dt);
	virtual void Render();
};

class GameNPCAltar : public GameNPC
{
public:
	GameNPCAltar();
	virtual ~GameNPCAltar(){}

public:
	virtual void Update(float _dt);
	virtual void Render();

protected:
	int m_nAlphaFrame;
	DWORD m_dwLastSwitchAlphaTime;
};

//////////////////////////////////////////////////////////////////////////

#endif