/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#include "PESO_timer.h"

PESO_Timer::PESO_Timer() : currentTime(0), previousTime(0) {};

void PESO_Timer::PESO_ResetTimer(){
	previousTime	= 0;
	currentTime		= 0;
}

void PESO_Timer::PESO_SetPreviousTime() {
	previousTime = currentTime;
}

Uint32 PESO_Timer::PESO_GetCurrentTime() {
	return currentTime;
}

Uint32 PESO_Timer::PESO_GetPreviousTime() {
	return previousTime;
}

bool PESO_Timer::PESO_ComparePreviousAndCurrentTime() {
	if (currentTime > previousTime + _SECOND_)
		return true;
	return false;
}

Uint32 PESO_Timer::PESO_GetElapsedTime() {
	return SDL_GetTicks() - currentTime;
}

void PESO_Timer::PESO_MeasureSessionTime() {
	currentTime = SDL_GetTicks();
}

void PESO_Timer::PESO_ElapseOneMillisecond() {
	currentTime += _MILLISECOND_;
}

void PESO_Timer::PESO_ElapseOneSecond() {
	currentTime += _SECOND_;
}

void PESO_Timer::PESO_ElapseOneMinute() {
	currentTime += _MINUTE_;
}

void PESO_Timer::PESO_ElapseOneHour() {
	currentTime += _HOUR_;
}
