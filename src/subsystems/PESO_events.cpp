/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#include "PESO_events.h"

PESO_Events::PESO_Events() {
	for (int i = 0; i < Key::LAST; ++i) {
		keyBooleans[i] = false;
	}
}

PESO_Events::~PESO_Events() {}

void PESO_Events::PESO_PollEvents() {
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) && event.key.repeat == 0) {
			PESO_UpdateKeyBooleans(event.key.keysym.sym, event.type == SDL_KEYDOWN);
		}
	}
}

void PESO_Events::PESO_UpdateKeyBooleans(const SDL_Keycode& key, bool triggered) {
	Key keyIndex;

	switch (key) {
	case SDLK_p:
		keyIndex = Key::P;
		break;
	case SDLK_r:
		keyIndex = Key::R;
		break;
	case SDLK_ESCAPE:
		keyIndex = Key::ESC;
		break;
	default:
		keyIndex = Key::LAST;
		break;
	}
	keyBooleans[keyIndex] = triggered;
}

PESO_Data PESO_Events::PESO_CreateObjectData(std::shared_ptr<PESO_Object> Earth) {
	std::string SatelliteName;
	double xDistance, yDistance, zDistance;
	double roll, yaw, pitch;
	std::cout << "Welcome to PESO!" << std::endl;
	std::cout << "Name your satellite (no spaces): ";
	std::cin >> SatelliteName;
	std::cout << "Set X distance from Earth: ";
	std::cin >> xDistance;
	std::cout << "Set Y distance from Earth: ";
	std::cin >> yDistance;
	std::cout << "Set Z distance from Earth: ";
	std::cin >> zDistance;
	std::cout << "Set Roll (X angle): ";
	std::cin >> roll;
	std::cout << "Set Yaw (Y angle): ";
	std::cin >> yaw;
	std::cout << "Set Pitch (Z angle): ";
	std::cin >> pitch;

	return PESO_Data(
		Vector3d(),
		Vector3d(),
		1000.0,
		PESO_Transform(
			Vector3d(
				Earth->getPosition().x + xDistance,
				Earth->getPosition().y + yDistance,
				Earth->getPosition().z + zDistance
			),
			Vector3d(roll, yaw, pitch)
		),
		5.0,
		SatelliteName
	);
}
