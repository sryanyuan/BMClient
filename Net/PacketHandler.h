#ifndef _INC_PACKET_HANDLER_
#define _INC_PACKET_HANDLER_
//////////////////////////////////////////////////////////////////////////
#include "../../CommonModule/ByteBuffer.h"
#include "../../CommonModule/DataParser.h"
#include "../../CommonModule/GamePacket.h"
#include <map>

//////////////////////////////////////////////////////////////////////////
#define UNDEFINE_OBJECT_ID		-1
#define DO_SYSTEM_PACKET(op, pkg)	case op:{pkg p;g_xBuffer >> p;DoPacket(p);}break;
//////////////////////////////////////////////////////////////////////////
class PacketHandler;
typedef std::map<int, PacketHandler*> HandlerMap;

class SocketDataCenter : public DataParser
{
	//	singleton
public:
	SocketDataCenter();
	virtual ~SocketDataCenter();

public:
	static SocketDataCenter* GetInstancePtr()
	{
		static SocketDataCenter* pIns = NULL;
		if(NULL == pIns)
		{
			pIns = new SocketDataCenter;
		}
		return pIns;
	}
	static SocketDataCenter& GetInstance()
	{
		return *GetInstancePtr();
	}
	static void __stdcall ProcessData(const void* _pData, unsigned int _nLen);

public:
	bool RegisterObject(PacketHandler* _pHandler);
	void UnregisterObject(int _nId);
	PacketHandler* GetHandler(int _nId);
	HandlerMap& GetHandlers()			{return m_xHandlers;}

	void PushData(const void* _pData, unsigned int _nLen);

	virtual void ProcessSystemPacket(const PacketHeader* _pPkt);
	virtual void ProcessGamePacket(const PacketHeader* _pPkt);

	bool SaveHumData_SaveFile(const char* _pszFile, const PkgSystemUserDataAck* _pPkt);
	bool SaveHumData_ZipArchive(const char* _pszFile, const PkgSystemUserDataAck* _pPkt);
	bool SaveHumData_ZipArchiveMulti(const PkgSystemUserDataAck* _pPkt);
	bool SaveHumData_ZipArchiveMultiLogin(const PkgSystemUserDataAck* _pPkt);
	bool SaveHumConfig(const char* _pszFile);

	bool SaveExtHumData_ZipArchiveMulti(const PkgSystemExtUserDataAck& ack);

public:
	//	Packet processor
	//	System
	void DoPacket_SystemUserLoginAck(const PkgUserLoginAck* _pPkt);
	void DoPacket_SystemForceActionAck(const PkgForceActionAck* _pPkt);
	void DoPacket_SystemNewPlayerNot(const PkgNewPlayerNot* _pPkt);
	void DoPacket_SystemNewNPCNot(const PkgNewNPCNot* _pPkt);
	void DoPacket_SystemUserDataAck(const PkgSystemUserDataAck* _pPkt);
	//	Game
	void DoPacket_GameObjectActionNot(const PkgObjectActionNot* _pPkt);
	void DoPacket_GameSyncDataNot(const PkgGameSyncDataNot& not);

	void DoPacket(const PkgSystemNewItemNot& not);
	void DoPacket(const PkgSystemDelItemNot& not);
	void DoPacket(const PkgDelNPCNot& not);
	void DoPacket(const PkgSystemClearGroundItemNtf& ntf);
	void DoPacket(const PkgGmNotificationNot& not);
	void DoPacket(const PkgSystemHideDoorNtf& not);
	void DoPacket(const PkgSystemClientVersionErrNtf& not);
	void DoPacket(const PkgSystemNotifyNot& not);
	void DoPacket(const PkgSystemWorldSayNot& not);
	void DoPacket(const PkgSystemExtUserDataAck& ack);

protected:
	HandlerMap m_xHandlers;
};
//////////////////////////////////////////////////////////////////////////

class PacketHandler
{
	friend class SocketDataCenter;
public:
	PacketHandler();
	virtual ~PacketHandler();

public:
	virtual void OnPacket(const PacketHeader* _pPkt);

public:
	inline void	SetHandlerID(int _nId)			{m_nUniqueID = _nId;}
	inline int	GetHandlerID() const			{return m_nUniqueID;}

protected:
	int m_nUniqueID;
};

//////////////////////////////////////////////////////////////////////////
#endif