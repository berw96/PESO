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

void PESO_Events::PESO_CheckInput(std::vector<char>& input) {
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

void PESO_Events::PESO_ExtractDigitsFromInput(std::vector<int>& digits, std::vector<char>& input) {
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

double PESO_Events::PESO_CalculateValueFromDigits(std::vector<int>& digits) {
	double input_value = 0;

	for (int i = 0; i < digits.size(); i++) {
		input_value += digits[i] * pow(10, (decimal_index - (i + 1)));
	}
	if (isNegative)
		input_value *= -1;

	// clear digits buffer once contents have been used to set input_value
	digits.clear();
	isNegative = false;

	std::cout << "Value entered was determined to be: " << input_value << "\n";

	return input_value;
}

PESO_Data PESO_Events::PESO_CreateObjectData(std::shared_ptr<PESO_Object> Earth) {
	std::vector<char> input;
	std::vector<int> digits;

	std::string SatelliteName;

	double xDistance, yDistance, zDistance;
	double roll, yaw, pitch;
	double xThrust, yThrust, zThrust;
	double xTorque, yTorque, zTorque;

	std::cout << "Welcome to PESO!" << std::endl;
	std::cout << "Name your satellite: ";
	std::getline(std::cin, SatelliteName);
	std::cout << "Set X distance from Earth: ";
	//std::cin >> xDistance;
	std::cin >> std::ws;
	PESO_CheckInput(input);
	PESO_ExtractDigitsFromInput(digits, input);
	xDistance = PESO_CalculateValueFromDigits(digits);
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
	std::cout << "Set X thrust: ";
	std::cin >> xThrust;
	std::cout << "Set Y thrust: ";
	std::cin >> yThrust;
	std::cout << "Set Z thrust: ";
	std::cin >> zThrust;
	std::cout << "Set X torque: ";
	std::cin >> xTorque;
	std::cout << "Set Y torque: ";
	std::cin >> yTorque;
	std::cout << "Set Z torque: ";
	std::cin >> zTorque;

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
		SatelliteName,
		Vector3d(xThrust, yThrust, zThrust),
		Vector3d(xTorque, yTorque, zTorque)
	);
}
