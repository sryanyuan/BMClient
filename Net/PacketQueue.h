#ifndef PACKET_QUEUE
#define PACKET_QUEUE
#include <list>
#include "../Net/Packet.h"

typedef std::list<PacketBase*> PACKETLIST;
class PacketQueue
{
public:
	PacketQueue()
	{
		memset(m_szBuf, 0, sizeof(m_szBuf));
		m_dwBufPointer = 0;
	}
	~PacketQueue()
	{
		//
		for(PACKETLIST::const_iterator iter = m_xPackets.begin();
			iter != m_xPackets.end();
			++iter)
		{
			delete *iter;
		}
		m_xPackets.clear();
	}

public:
	void PushData(const char* _pData, int _len);
	PacketBase* GetPacket()
	{
		if(m_xPackets.empty())
		{
			return NULL;
		}

		PacketBase* pRet = m_xPackets.front();
		m_xPackets.pop_front();
		return pRet;
	}
	void Reset()
	{
		m_dwBufPointer = 0;
	}

private:
	char m_szBuf[8192 * 4];
	DWORD m_dwBufPointer;
	PACKETLIST m_xPackets;
};

#endif