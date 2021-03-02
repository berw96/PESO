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

	std::shared_ptr<PESO_Object> Earth{
		new PESO_Object(PESO_Data(
			Vector3d(),
			Vector3d(),
			100000.0,
			PESO_Transform(
				Vector3d(225.0, 225.0, 225.0),
				Vector3d()
			),
			50.0,
			"Earth"
		))
	};

	std::shared_ptr<PESO_Object> Satellite{ new PESO_Object(events->PESO_CreateObjectData(Earth)) };

	physics->PESO_RegisterObject(Satellite);
	physics->PESO_RegisterObject(Earth);
#pragma endregion

#pragma region GRAPHICAL_REP
	Point2d SatellitePointXY = {
		Satellite->getPosition().x,
		Satellite->getPosition().y
	};

	Point2d SatellitePointXZ = {
		Satellite->getPosition().x,
		Satellite->getPosition().z
	};

	Point2d SatellitePointYZ = {
		Satellite->getPosition().z,
		Satellite->getPosition().y
	};
	
	Point2d EarthPointXY = {
		Earth->getPosition().x,
		Earth->getPosition().y
	};

	Point2d EarthPointXZ = {
		Earth->getPosition().x,
		Earth->getPosition().z
	};

	Point2d EarthPointYZ = {
		Earth->getPosition().z,
		Earth->getPosition().y
	};

	//init graphics subsystem
	PESO_Graphics* graphics = new PESO_Graphics();
#pragma endregion

	std::cout << "Press R to play, P to pause or ESCAPE to quit." << std::endl;

	while (running) {
#pragma region KEY_INPUT
		events->PESO_PollEvents();

		if (events->PESO_KeyIsPressed(Key::P)) {
			if (!paused) {
				paused = true;
				std::cout << "PAUSED\n";
			}
		}
		if (events->PESO_KeyIsPressed(Key::R)) {
			if (paused) {
				paused = false;
				std::cout << "PLAYING...\n";
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

			//graphical representation mimics object physics
			SatellitePointXY.horizontal = Satellite->getTransform().position.x;
			SatellitePointXY.vertical	= Satellite->getTransform().position.y;
			SatellitePointXZ.horizontal = Satellite->getTransform().position.x;
			SatellitePointXZ.vertical	= Satellite->getTransform().position.z;
			SatellitePointYZ.horizontal = Satellite->getTransform().position.z;
			SatellitePointYZ.vertical	= Satellite->getTransform().position.y;
			
			EarthPointXY.horizontal		= Earth->getTransform().position.x;
			EarthPointXY.vertical		= Earth->getTransform().position.y;
			EarthPointXZ.horizontal		= Earth->getTransform().position.x;
			EarthPointXZ.vertical		= Earth->getTransform().position.z;
			EarthPointYZ.horizontal		= Earth->getTransform().position.z;
			EarthPointYZ.vertical		= Earth->getTransform().position.y;
#pragma endregion

#pragma region TIMER
			timer->PESO_MeasureSessionTime();
			if (timer->PESO_ComparePreviousAndCurrentTime()) {
				physics->PESO_LogData(Satellite->getObjectData());
				timer->PESO_SetPreviousTime();
			}
#pragma endregion
		}

#pragma region GRAPHICS
		graphics->PESO_DrawSimulationData(Satellite);
		graphics->PESO_DrawTag(Satellite);
		graphics->PESO_DrawTag(Earth);

		graphics->PESO_DrawEllipseXY(SatellitePointXY, Satellite->getRadius(), Satellite->getRadius());
		graphics->PESO_DrawEllipseXZ(SatellitePointXZ, Satellite->getRadius(), Satellite->getRadius());
		graphics->PESO_DrawEllipseYZ(SatellitePointYZ, Satellite->getRadius(), Satellite->getRadius());

		graphics->PESO_DrawEllipseXY(EarthPointXY, Earth->getRadius(), Earth->getRadius());
		graphics->PESO_DrawEllipseXZ(EarthPointXZ, Earth->getRadius(), Earth->getRadius());
		graphics->PESO_DrawEllipseYZ(EarthPointYZ, Earth->getRadius(), Earth->getRadius());
#pragma endregion

		//show results
		graphics->PESO_ShowScreen();
	}
#pragma region FILES
	files->PESO_WriteFile(physics->PESO_GetSessionData(), *Satellite);
#pragma endregion
	SDL_Quit();	// quit remaining subsystems
	system("PAUSE");
	return 0;
}
