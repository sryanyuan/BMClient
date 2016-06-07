#ifndef _INC_CIRCLEBUFFER_
#define _INC_CIRCLEBUFFER_
//////////////////////////////////////////////////////////////////////////
#include <vector>
//////////////////////////////////////////////////////////////////////////
template <class T>
class CircleBuffer
{
public:
	CircleBuffer()
	{
		Clear();
	}
	~CircleBuffer()
	{
		Clear();
	}

public:
	void Init(int _nItemCount)
	{
		Clear();
		m_xItems.resize(_nItemCount);
		m_nBufferSize = _nItemCount;
	}
	void Clear()
	{
		m_xItems.clear();
		m_nTailPtr = 0;
		m_nBufferSize = 0;
		m_nInsertTimes = 0;
	}

	T* AppendItem(T* _pItem)
	{
		int nNextPtr = m_nTailPtr + 1;
		if(nNextPtr >= m_nBufferSize)
		{
			nNextPtr %= m_nBufferSize;
		}

		//	copy the content
		memcpy(&m_xItems[nNextPtr], _pItem, sizeof(T));

		//	update the tail ptr
		SetTailPtrOffset(1);

		return &m_xItems[nNextPtr];
	}

	T* GetNextItem()
	{
		int nNextPtr = m_nTailPtr + 1;
		if(nNextPtr >= m_nBufferSize)
		{
			nNextPtr %= m_nBufferSize;
		}

		return &m_xItems[nNextPtr];
	}

	T* GetCurrentItem()
	{
		return &m_xItems[m_nTailPtr % m_nBufferSize];
	}

	T* GetItemFromTail(int _nTailOffset)
	{
		int nFinalIndex = m_nTailPtr - _nTailOffset;

		if(nFinalIndex >= 0)
		{
			nFinalIndex %= m_nBufferSize;
		}
		else
		{
			int nNegtive = -(nFinalIndex / m_nBufferSize) + 1;
			nFinalIndex += m_nBufferSize * nNegtive;
		}

		return &m_xItems[nFinalIndex];
	}

	int GetInsertTimes()
	{
		return m_nInsertTimes;
	}

	void SetTailPtrOffset(int _nOffset)
	{
		if(_nOffset < 0)
		{
			return;
		}
		m_nTailPtr += _nOffset;
		m_nInsertTimes += _nOffset;
	}

	int GetItemCount()
	{
		if(m_nInsertTimes <= m_nBufferSize)
		{
			return m_nInsertTimes;
		}
		return m_nBufferSize;
	}

protected:
	std::vector<T> m_xItems;
	int m_nTailPtr;
	int m_nBufferSize;
	int m_nInsertTimes;
};
//////////////////////////////////////////////////////////////////////////
#endif