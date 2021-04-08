/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>

	Made with C++ and Simple DirectMedia Layer (SDL) Version 2.0, under the
	GNU General Public Licence (GPL) and the ZLib Licence.
*/

#include "../subsystems/PESO.h"

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Could not init SDL %s\n", SDL_GetError());
		return 1;
	};

	bool running	= true;
	bool paused		= true;

#pragma region OBJECT_DEF
	PESO_Events* events		= new PESO_Events();
	PESO_Physics* physics	= new PESO_Physics();
	PESO_Timer* timer		= new PESO_Timer();
	PESO_FileManager* files	= new PESO_FileManager();

	std::cout << "Welcome to PESO!" << std::endl;

	events->PESO_SelectNumberOfObjects(*physics);
#pragma endregion

#pragma region GRAPHICS
	//init graphics subsystem
	PESO_Graphics* graphics = new PESO_Graphics();
#pragma endregion

	std::cout << "\nPress SPACE to play, or ESCAPE to quit." << std::endl;

	while (running) {
#pragma region KEY_INPUT
		events->PESO_PollEvents();
		if (events->PESO_KeyIsPressed(Key::SPACE)) {
			if (paused) {
				paused = false;
				std::cout << "Playing Simulation...\n";
			}
		}
		if (events->PESO_KeyIsPressed(Key::ESC)) {
			std::cout << "Exiting PESO...\n";
			// quit PESO subsystems using SDL
			graphics->~PESO_Graphics();
			events->~PESO_Events();
			running = false;
		}
#pragma endregion
		//clear screen
		graphics->PESO_ClearScreen();

		if (!paused) {
#pragma region PHYSICS
			//apply mechanics of physics engine to all objects registered with it
			physics->PESO_ApplyLinearMechanics();
			physics->PESO_ApplyRotationMechanics();
#pragma endregion

#pragma region TIMER
			timer->PESO_MeasureSessionTime();
			if (timer->PESO_ComparePreviousAndCurrentTime()) {
				for (auto obj : physics->PESO_GetSessionObjects()) {
					physics->PESO_LogData(obj->getObjectData());
				}
				timer->PESO_SetPreviousTime();
			}
#pragma endregion
		}

#pragma region GEOMETRY
		graphics->PESO_DrawSimulationData(physics->PESO_GetSessionObjects(0));
		graphics->PESO_DrawObjectGraphics(*physics);
#pragma endregion

		//show results
		graphics->PESO_ShowScreen();
	}
#pragma region FILES
	files->PESO_WriteFile(physics->PESO_GetSessionData());
#pragma endregion
	SDL_Quit();	// quit remaining subsystems
	system("PAUSE");
	return 0;
}
