#ifndef PACKET_H_
#define PACKET_H_

#include <Windows.h>
#include <list>

enum PACKET_COMMAND
{
	PC_BEGIN = 0,

	//	系统数据包
	PC_SYSTEM_LOGIN_REQ = 1,
	PC_SYSTEM_LOGIN_ACK,

	//	数据包命令开始

	//	人物动作
	PC_ACTION_BEGIN = 0x00FF,
	//	走动
	PC_ACTION_WALK,
	//	跑动
	PC_ACTION_RUN,
	//	转身
	PC_ACTION_TURN,
	//	攻击
	PC_ACTION_ATTACK,

	PC_ACTION_END,

	//	命令结束
	PC_END = 0xFFFF,
};

struct PacketBase
{
	//	长度
	DWORD dwSize;
	//	命令
	WORD wCmd;
	//	物体ID
	DWORD dwObjID;
	//	目标ID
	DWORD dwTarget;
};

struct ActionPacket : public PacketBase
{
	//	参数
	WORD wParam0;
	WORD wParam1;
	WORD wParam2;
	WORD wParam3;
	WORD wParam4;
};

struct UserLoginPacket : public PacketBase
{
	//
	char szName[20];
};

struct UserLoginPacketAck : public PacketBase
{
	//
	BYTE bRet;
};

typedef std::list<ActionPacket*> ACTIONLIST;

// class PacketReader
// {
// public:
// 	BOOL Read(const char* _pData, DWORD _dwLen,)
// };


#endif