/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#ifndef _EVENTS_
#define _EVENTS_

#include <string>
#include <thread>
#include <SDL.h>

enum Key {
	P, R, ESC, QUIT, RIGHT_ARROW, LEFT_ARROW, UP_ARROW, DOWN_ARROW, LAST
};

enum CommandName {
	FOO, EXIT, STOP, START, RESET, LOAD, SAVE
};

struct PESO_Command {
	CommandName name;
	std::string value;

	PESO_Command(CommandName name, std::string value);
};

class PESO_Events {
	SDL_Event event;
	bool keyBooleans[Key::LAST];

	void PESO_UpdateKeyBooleans(const SDL_Keycode& key, bool keyDown);

public:
	PESO_Events();
	~PESO_Events();

	void PESO_PollEvents();

	bool PESO_KeyIsPressed(Key key)	const { return keyBooleans[key]; }
	void PESO_KeySetPressed(Key key) { keyBooleans[key] = true; }
};


#endif
