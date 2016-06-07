#ifndef _INC_GAMEPACKETHANDLER_
#define _INC_GAMEPACKETHANDLER_
//////////////////////////////////////////////////////////////////////////
#include "PacketHandler.h"
#include <assert.h>
//////////////////////////////////////////////////////////////////////////
class GameObject;
//////////////////////////////////////////////////////////////////////////
#define ALERT_MSGBOX(T) ::MessageBox(NULL, T, "ERROR", MB_ICONERROR | MB_TASKMODAL)
#define DO_PACKET(PKT)	PKT pkt;try{g_xBuffer >> pkt;}BUFFER_EXCEPTION_CATCH_RETURN_VOID;\
						DoPacket(pkt);
#define BEGIN_HANDLE_PACKET(pkt)	switch(pkt->uOp){
#define HANDLE_PACKET(op,pkt)		case op:{DO_PACKET(pkt);}break;
#define HANDLE_DEFAULT(pkt)			default:{__super::OnPacket(pkt);}break;
#define END_HANDLE_PACKET(pkt)		}
//////////////////////////////////////////////////////////////////////////
extern ByteBuffer g_xBuffer;
//////////////////////////////////////////////////////////////////////////
//	base packet handler
/*
class BasePacketHandler : public PacketHandler
{
public:
	BasePacketHandler();
	virtual ~BasePacketHandler();

public:
	virtual void OnPacket(const PacketHeader* _pPkt);

public:
	void DoPacket(const PkgObjectActionNot& _not);
	void DoPacket(const PkgPlayerSayReq& req);
};

//////////////////////////////////////////////////////////////////////////
//	other players
class HeroPacketHandler : public BasePacketHandler
{
public:
	HeroPacketHandler();
	virtual ~HeroPacketHandler();

public:
	virtual void OnPacket(const PacketHeader* _pPkt);
	
public:
	void DoPacket(const PkgChatNot& not);
	void DoPacket(const PkgObjectActionNot& not);
	void DoPacket(const PkgPlayerChangeEquipNot& not);
	void DoPacket(const PkgPlayerGainItemNtf& ntf);
	void DoPacket(const PkgPlayerDropItemNot& not);
	void DoPacket(const PkgPlayerLostItemAck& ack);
	void DoPacket(const PkgPlayerClearItemNtf& ntf);
	void DoPacket(const PkgPlayerSyncAttribNtf& ntf);
	void DoPacket(const PkgPlayerUndressItemAck& ack);
	void DoPacket(const PkgPlayerDressItemAck& ack);
	void DoPacket(const PkgPlayerUseItemAck& ack);
	void DoPacket(const PkgPlayerUpdateAttribNtf& ntf);
	void DoPacket(const PkgPlayerStruckAck& ack);
	void DoPacket(const PkgPlayerForceActionAck& ack);
	void DoPacket(const PkgPlayerClearBagNtf& ntf);
};

//////////////////////////////////////////////////////////////////////////
//	this class is for user
class MyHeroPacketHandler : public HeroPacketHandler
{
public:
	MyHeroPacketHandler(GameObject* _pObj);
	virtual ~MyHeroPacketHandler();

public:
	virtual void OnPacket(const PacketHeader* _pPkt);
};

//////////////////////////////////////////////////////////////////////////
//	base class for monster,other players and npc
class AIPacketHandler : public BasePacketHandler
{
public:
	AIPacketHandler();
	virtual ~AIPacketHandler();

public:
	virtual void OnPacket(const PacketHeader* _pPkt);

public:
	void DoPacket(const PkgChatNot& not);
	void DoPacket(const PkgObjectActionNot& not);
	void DoPacket(const PkgPlayerClickNPCAck& not);
};*/


//////////////////////////////////////////////////////////////////////////
#endif