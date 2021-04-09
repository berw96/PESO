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
	case SDLK_SPACE:
		keyIndex = Key::SPACE;
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
	for (; std::cin.peek() != EOF &&
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
		}
		else {
			// keep if a digit, sign flag or decimal
			input.push_back(ch);
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
	isNegative = false;
	decimal_index_set = false;
	decimal_index = 0;

	std::cout << "Value entered was determined to be: " << input_value << "\n";

	return input_value;
}

void PESO_Events::PESO_SelectNumberOfObjects(PESO_Physics& physics) {
	int number_of_objects = 0;

	std::cout << "\nSpecify number of objects for simulation:";

	number_of_objects = (int)PESO_CalculateValueFromDigits();

	if (number_of_objects <= 1)
		number_of_objects = 1;

	for (int i = 0; i < number_of_objects; i++) {
		std::cout << "\n[Object " << (i + 1) << " of " << number_of_objects << "]";
		std::shared_ptr<PESO_Object> object{ new PESO_Object(PESO_CreateObjectData()) };
		physics.PESO_RegisterObject(object);
	}
}

PESO_Data PESO_Events::PESO_CreateObjectData() {

	std::string name;

	double mass;
	double xPos, yPos, zPos;
	double roll, yaw, pitch;
	double xThrust, yThrust, zThrust;
	double xTorque, yTorque, zTorque;

	std::cout << "\nName object: ";
	std::cin.ignore();
	std::getline(std::cin, name);
	std::cout << "Set mass: ";
	mass	= (double)PESO_CalculateValueFromDigits();
	std::cout << "Set X position: ";
	xPos	= (double)PESO_CalculateValueFromDigits();
	std::cout << "Set Y position: ";
	yPos	= (double)PESO_CalculateValueFromDigits();
	std::cout << "Set Z position: ";
	zPos	= (double)PESO_CalculateValueFromDigits();
	std::cout << "Set Roll (X angle): ";
	roll	= (double)PESO_CalculateValueFromDigits();
	std::cout << "Set Yaw (Y angle): ";
	yaw		= (double)PESO_CalculateValueFromDigits();
	std::cout << "Set Pitch (Z angle): ";
	pitch	= (double)PESO_CalculateValueFromDigits();
	std::cout << "Set X thrust: ";
	xThrust = (double)PESO_CalculateValueFromDigits();
	std::cout << "Set Y thrust: ";
	yThrust = (double)PESO_CalculateValueFromDigits();
	std::cout << "Set Z thrust: ";
	zThrust = (double)PESO_CalculateValueFromDigits();
	std::cout << "Set X torque: ";
	xTorque = (double)PESO_CalculateValueFromDigits();
	std::cout << "Set Y torque: ";
	yTorque = (double)PESO_CalculateValueFromDigits();
	std::cout << "Set Z torque: ";
	zTorque = (double)PESO_CalculateValueFromDigits();

	return PESO_Data(
		Vector3d(),
		Vector3d(),
		mass,
		PESO_Transform(
			Vector3d(
				xPos + _DEFAULT_WINDOW_WIDTH_ / 2, 
				yPos + _DEFAULT_WINDOW_WIDTH_ / 2,
				zPos + _DEFAULT_WINDOW_WIDTH_ / 2
			),
			Vector3d(roll, yaw, pitch)
		),
		10.0,
		name,
		Vector3d(xThrust, yThrust, zThrust),
		Vector3d(xTorque, yTorque, zTorque)
	);
}
