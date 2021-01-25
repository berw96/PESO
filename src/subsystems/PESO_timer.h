/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#ifndef _TIMER_
#define _TIMER_
#define _MILLISECOND_	(1)
#define _SECOND_		(1000)
#define _MINUTE_		(60 * _SECOND_)
#define _HOUR_			(60 * _MINUTE_)

#include <SDL.h>

class PESO_Timer {
	Uint32 time;
public:
	PESO_Timer();
	void PESO_MeasureSessionTime();
	void PESO_ResetTimer();
	void PESO_ElapseOneMillisecond();
	void PESO_ElapseOneSecond();
	void PESO_ElapseOneMinute();
	void PESO_ElapseOneHour();
	Uint32 PESO_GetTimestamp();
	Uint32 PESO_GetElapsedTime();
};

#endif