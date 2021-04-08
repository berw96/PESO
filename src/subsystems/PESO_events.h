/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>

	Made with C++ and Simple DirectMedia Layer (SDL) Version 2.0, under the
	GNU General Public Licence (GPL) and the ZLib Licence.
*/

#ifndef _EVENTS_
#define _EVENTS_

#define _ASCII_DISPLACEMENT_ (48)

#include <string>
#include <thread>
#include <SDL.h>

#include "PESO_physics.h"
#include "PESO_graphics.h"

enum Key {
	SPACE, ESC, LAST
};

class PESO_Events {
	SDL_Event event;
	bool keyBooleans[Key::LAST];

	std::vector<char> input;
	std::vector<int> digits;

	int decimal_index		= 0;
	bool decimal_index_set	= false;
	bool isNegative			= false;

	void PESO_UpdateKeyBooleans(const SDL_Keycode& key, bool keyDown);

public:
	PESO_Events();
	~PESO_Events();

	void PESO_PollEvents();
	PESO_Data PESO_CreateObjectData();

	bool PESO_KeyIsPressed(Key key)	const { return keyBooleans[key]; }
	void PESO_KeySetPressed(Key key) { keyBooleans[key] = true; }
	void PESO_CheckInput();
	void PESO_ExtractDigitsFromInput();
	double PESO_CalculateValueFromDigits();
	void PESO_SelectNumberOfObjects(PESO_Physics& physics);
};

#endif
