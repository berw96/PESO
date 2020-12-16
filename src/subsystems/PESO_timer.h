/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#ifndef _TIMER_
#define _TIMER_
#define _SECOND_	(1000)
#define _MINUTE_	(60 * _SECOND_)
#define _HOUR_		(60 * _MINUTE_)
#define _DAY_		(24 * _HOUR_)
#define _SOL_		(0.9723435226 * _DAY_)

#include <SDL.h>

class PESO_Timer {
	Uint32 time;
public:
	PESO_Timer();
	void PESO_MeasureCurrentTime();
	void PESO_ResetTimer();
	Uint32 PESO_GetTimestamp();
	Uint32 PESO_GetElapsedTime();
};


#endif