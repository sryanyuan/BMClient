#include "TimeCounter.h"

TimerCounter::TimerCounter()
{
	m_dwCounter = m_dwAlert = 0;
}

TimerCounter::~TimerCounter()
{

}

void TimerCounter::Start()
{
	m_dwCounter = ::GetTickCount();
}

void TimerCounter::SetAlertTime(DWORD _dwMs)
{
	m_dwAlert = _dwMs;
}

DWORD TimerCounter::Stop()
{
	return ::GetTickCount() - m_dwCounter;
}