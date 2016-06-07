#include "../Net/PacketQueue.h"

/************************************************************************/
/* 解析数据包
/************************************************************************/
void PacketQueue::PushData(const char *_pData, int _len)
{
	PacketBase* pPacket = NULL;
	const char* pReadData = _pData;

	if(m_dwBufPointer == 0)
	{
		//	缓冲区是空的
		if(_len < 4)
		{
			//	无法读出数据包长度
			return;
		}

		//	先读出数据长度
		DWORD dwSize = ntohl(*(DWORD*)pReadData);
		DWORD dwLeft = _len;
// 		pReadData += 4;
// 		dwLeft -= 4;

		while(dwLeft > 0)
		{
			if(dwSize <= dwLeft)
			{
				//	数据够了
				pPacket = (PacketBase*)malloc(dwSize);
				pPacket->dwSize = dwSize;
				memcpy(pPacket, pReadData, dwSize);
				m_xPackets.push_back(pPacket);
				dwLeft -= dwSize;
				pReadData += dwSize;
			}
			else
			{
				//	数据不够 存入缓冲区
				if(sizeof(m_szBuf) - m_dwBufPointer < dwLeft)
				{
					//	缓冲区不够了 直接清空缓存
					m_dwBufPointer = 0;
					return;
				}
				m_dwBufPointer += dwLeft;
				memcpy(m_szBuf + m_dwBufPointer, pReadData, dwLeft);
				return;
			}

			//	读长度
			if(dwLeft > 4)
			{
				dwSize = ntohl(*(DWORD*)pReadData);
// 				pReadData += 4;
// 				dwLeft -= 4;
			}
			else
			{
				//	不够 存入缓存
				if(sizeof(m_szBuf) - m_dwBufPointer < dwLeft)
				{
					//	缓冲区不够了 直接清空缓存
					m_dwBufPointer = 0;
					return;
				}
				m_dwBufPointer += dwLeft;
				memcpy(m_szBuf + m_dwBufPointer, pReadData, dwLeft);
				return;
			}
		}
	}
	else
	{
		//	缓冲区有数据 先从缓冲区读取长度
		if(m_dwBufPointer < 4)
		{
			return;
		}
		else
		{
			//	先读出数据长度 看是否可以读数据
			DWORD dwSize = ntohl(*(DWORD*)m_szBuf);
			if(dwSize > m_dwBufPointer + _len)
			{
				//	缓冲区加上数据 不够读
				return;
			}
			else
			{
				//	可以读了 先读完缓冲区的数据
				DWORD dwLeft = _len;
				pReadData = _pData;
				DWORD dwNeed = dwSize - m_dwBufPointer;

				pPacket = (PacketBase*)malloc(dwSize);
				pPacket->dwSize = dwSize;
				memcpy(pPacket, m_szBuf, m_dwBufPointer);
				//	在读socket数据
				memcpy(pPacket + m_dwBufPointer, pReadData, dwNeed);
				m_xPackets.push_back(pPacket);

				pReadData += dwNeed;
				dwLeft -= dwNeed;
				m_dwBufPointer = 0;

				//	再读数据
				if(dwLeft > 4)
				{
					dwSize = ntohl(*(DWORD*)pReadData);

					while(dwLeft > 0)
					{
						if(dwSize <= dwLeft)
						{
							//	数据够了
							pPacket = (PacketBase*)malloc(dwSize);
							pPacket->dwSize = dwSize;
							memcpy(pPacket, pReadData, dwSize);
							m_xPackets.push_back(pPacket);
							dwLeft -= dwSize;
							pReadData += dwSize;
						}
						else
						{
							//	数据不够 存入缓冲区
							if(sizeof(m_szBuf) - m_dwBufPointer < dwLeft)
							{
								//	缓冲区不够了 直接清空缓存
								m_dwBufPointer = 0;
								return;
							}
							m_dwBufPointer += dwLeft;
							memcpy(m_szBuf + m_dwBufPointer, pReadData, dwLeft);
							return;
						}
					}
				}
			}
		}
	}
}