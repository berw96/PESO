/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>

	Made with C++ and Simple DirectMedia Layer (SDL) Version 2.0, under the
	GNU General Public Licence (GPL) and the ZLib Licence.
*/

#include "PESO_events.h"

PESO_Events::PESO_Events() {
	for (int i = 0; i < Key::LAST; ++i) {
		keyBooleans[i] = false;
	}
}

PESO_Events::~PESO_Events() {
	printf("PESO_Events shutting down...\n");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	printf("PESO_Events shut down.\n");
}

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

void PESO_Events::PESO_CheckInput() {
	// get user input from console
	std::cin >> std::ws;

	char ch = '\0';
	// iterate through contents of buffer until EOF and newline
	for (;std::cin.peek() != EOF &&
		std::cin.peek() != '\n';) {
		// if buffer contains newline and is EOF, break from loop
		if (std::cin.peek() == EOF &&
			std::cin.peek() == '\n') {
			break;
		}
		else {
			// assign character from buffer to variable
			ch = std::cin.peek();
		}

		// check if variable is a digit
		if (!std::isdigit(ch) &&
			ch != '-' &&
			ch != '.') {
			// ignore if not a digit, a negative flag, or a decimal
			std::cin.ignore();
			std::cout << "Removed " << ch << " from input\n";
		}
		else {
			// keep if a digit, sign flag or decimal
			input.push_back(ch);
			std::cout << "Found: " << ch << "\n";
			// dispose from buffer after use
			std::cin.ignore();
		}
	}
}

void PESO_Events::PESO_ExtractDigitsFromInput() {
	// scan contents of input for digits
	for (int i = 0; i < input.size(); i++) {
		// if a digit, cast to integer
		// taking into account its ascii value
		if (std::isdigit(input[i])) {
			digits.push_back((int)input[i] - _ASCII_DISPLACEMENT_);
		}
		if (input[i] == '.' && !decimal_index_set) {
			decimal_index = i;
			decimal_index_set = true;
		}
		// set isNegative flag to true if negative sign flag found
		if (input[i] == '-')
			isNegative = true;
	}
	// clear input buffer once valid contents have been registered to digits buffer
	input.clear();
}

double PESO_Events::PESO_CalculateValueFromDigits() {
	PESO_CheckInput();
	PESO_ExtractDigitsFromInput();

	double input_value = 0;

	if (isNegative) {
		int non_digits = 2;
		for (int i = 0; i < digits.size(); i++) {
			input_value += digits[i] * pow(10, decimal_index - (i + non_digits));
		}
		input_value *= -1;
	}
	else {
		int non_digits = 1;
		for (int i = 0; i < digits.size(); i++) {
			input_value += digits[i] * pow(10, decimal_index - (i + non_digits));
		}
	}

	// clear digits buffer once contents have been used to set input_value
	digits.clear();

	// reset flags for next use
	isNegative			= false;
	decimal_index_set	= false;
	decimal_index		= 0;
	
	std::cout << "Value entered was determined to be: " << input_value << "\n";

	return input_value;
}

PESO_Data PESO_Events::PESO_CreateObjectData(std::shared_ptr<PESO_Object> Planet) {

	std::string SatelliteName;

	double mass;
	double xDistance, yDistance, zDistance;
	double roll, yaw, pitch;
	double xThrust, yThrust, zThrust;
	double xTorque, yTorque, zTorque;

	std::cout << "Welcome to PESO!" << std::endl;
	std::cout << "Name your satellite: ";
	std::getline(std::cin, SatelliteName);
	std::cout << "Set mass of satellite: ";
	mass = PESO_CalculateValueFromDigits();
	std::cout << "Set X distance from Planet: ";
	xDistance = PESO_CalculateValueFromDigits();
	std::cout << "Set Y distance from Planet: ";
	yDistance = PESO_CalculateValueFromDigits();
	std::cout << "Set Z distance from Planet: ";
	zDistance = PESO_CalculateValueFromDigits();
	std::cout << "Set Roll (X angle): ";
	roll = PESO_CalculateValueFromDigits();
	std::cout << "Set Yaw (Y angle): ";
	yaw = PESO_CalculateValueFromDigits();
	std::cout << "Set Pitch (Z angle): ";
	pitch = PESO_CalculateValueFromDigits();
	std::cout << "Set X thrust: ";
	xThrust = PESO_CalculateValueFromDigits();
	std::cout << "Set Y thrust: ";
	yThrust = PESO_CalculateValueFromDigits();
	std::cout << "Set Z thrust: ";
	zThrust = PESO_CalculateValueFromDigits();
	std::cout << "Set X torque: ";
	xTorque = PESO_CalculateValueFromDigits();
	std::cout << "Set Y torque: ";
	yTorque = PESO_CalculateValueFromDigits();
	std::cout << "Set Z torque: ";
	zTorque = PESO_CalculateValueFromDigits();

	return PESO_Data(
		Vector3d(),
		Vector3d(),
		mass,
		PESO_Transform(
			Vector3d(
				Planet->getPosition().x + xDistance,
				Planet->getPosition().y + yDistance,
				Planet->getPosition().z + zDistance
			),
			Vector3d(roll, yaw, pitch)
		),
		5.0,
		SatelliteName,
		Vector3d(xThrust, yThrust, zThrust),
		Vector3d(xTorque, yTorque, zTorque)
	);
}
