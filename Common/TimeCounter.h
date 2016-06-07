#ifndef TIMERCOUNTER_H_
#define TIMERCOUNTER_H_

#include "GlobalFunction.h"
#include <Windows.h>

class TimerCounter
{
public:
	TimerCounter();
	~TimerCounter();

public:
	void Start();
	DWORD Stop();
	void SetAlertTime(DWORD _dwMs);

private:
	DWORD m_dwCounter;
	DWORD m_dwAlert;
};

#endif