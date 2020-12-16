/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#include "PESO_timer.h"

PESO_Timer::PESO_Timer() : time(0) {};

void PESO_Timer::PESO_ResetTimer(){
	time = 0;
}

Uint32 PESO_Timer::PESO_GetTimestamp() {
	return time;
}

Uint32 PESO_Timer::PESO_GetElapsedTime() {
	return SDL_GetTicks() - time;
}

void PESO_Timer::PESO_MeasureCurrentTime() {
	time = SDL_GetTicks();
}
