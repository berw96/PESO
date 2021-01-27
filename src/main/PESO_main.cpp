#include "../subsystems/PESO.h"

int main(int argc, char* args[]) {
	std::cout << "Welcome to PESO! Press R to play, P to pause or ESCAPE to quit." << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Could not init SDL %s\n", SDL_GetError());
		return 1;
	};

	bool running	= true;
	bool paused		= true;

#pragma region INPUT_DEMO
	std::string SatelliteName;
	double xDistance, yDistance, zDistance;
	double roll, yaw, pitch;
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
#pragma endregion

#pragma region INIT_SUBSYSTEMS
	//declare pointers to PESO subsystems
	PESO_Events* events		= new PESO_Events();
	PESO_Graphics* graphics = new PESO_Graphics();
	PESO_Physics* physics	= new PESO_Physics();
	PESO_Timer* timer		= new PESO_Timer();
#pragma endregion

#pragma region OBJECT_DEF
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
			"Planet"
		))
	};

	std::shared_ptr<PESO_Object> Satellite{
		new PESO_Object(PESO_Data(
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
		))
	};


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
#pragma endregion

#pragma region MAIN_LOOP
	while (running) {
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
			std::cout << "Exiting PESO...";
			//should clear resources before exiting.
			exit(0);
		}
		
		//clear screen
		graphics->PESO_ClearScreen();
		
		if (!paused) {
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

			timer->PESO_MeasureSessionTime();
			if (timer->PESO_ComparePreviousAndCurrentTime()) {
				physics->PESO_LogData(Satellite->getObjectData());
				timer->PESO_SetPreviousTime();
			}
		}
		graphics->PESO_DrawSimulationData(Satellite);

		graphics->PESO_DrawEllipseXY(SatellitePointXY, Satellite->getRadius(), Satellite->getRadius());
		graphics->PESO_DrawEllipseXZ(SatellitePointXZ, Satellite->getRadius(), Satellite->getRadius());
		graphics->PESO_DrawEllipseYZ(SatellitePointYZ, Satellite->getRadius(), Satellite->getRadius());

		graphics->PESO_DrawEllipseXY(EarthPointXY, Earth->getRadius(), Earth->getRadius());
		graphics->PESO_DrawEllipseXZ(EarthPointXZ, Earth->getRadius(), Earth->getRadius());
		graphics->PESO_DrawEllipseYZ(EarthPointYZ, Earth->getRadius(), Earth->getRadius());

		//show results
		graphics->PESO_ShowScreen();
	}
#pragma endregion
	SDL_Quit();
	return 0;
}
