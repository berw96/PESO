#include "../subsystems/PESO.h"

int main(int argc, char* args[]) {
	std::cout << "Welcome to PESO! Press R to play, P to pause or ESCAPE to quit." << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Could not init SDL %s\n", SDL_GetError());
		return 1;
	};

	bool running = true;
	bool paused = true;
	//declare pointers to PESO subsystems
	PESO_Events* events		= new PESO_Events();
	PESO_Graphics* graphics = new PESO_Graphics();
	PESO_Physics* physics	= new PESO_Physics();

#pragma region INPUT_DEMO
	std::string SatelliteName;
	double x;
	double y;
	double z;
	std::cout << "Name the your satellite (no spaces): ";
	std::cin >> SatelliteName;
	std::cout << "Set init x position: ";
	std::cin >> x;
	std::cout << "Set init y position: ";
	std::cin >> y;
	std::cout << "Set init z position: ";
	std::cin >> z;
#pragma endregion

	std::shared_ptr<PESO_Object> Satellite{
		new PESO_Object(Vector3d(),
		Vector3d(),
		10000.0,
		PESO_Transform(Vector3d(x, y, z),
		Vector3d()),
		1.0,
		SatelliteName,
		Vector3d())
	};
	
	std::shared_ptr<PESO_Object> Earth{
		new PESO_Object(Vector3d(),
		Vector3d(),
		200000000.0,
		PESO_Transform(Vector3d(500.0, 500.0, 500.0),
		Vector3d()),
		1.0,
		"Earth",
		Vector3d())
	};
	physics->PESO_RegisterObject(Satellite);
	physics->PESO_RegisterObject(Earth);

	Point2d SatellitePointXY = {
		Satellite->getPosition().x,
		Satellite->getPosition().y
	};
	
	Point2d EarthPointXY = {
		Earth->getPosition().x,
		Earth->getPosition().y
	};

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
			//graphical representation mimics object physics
			SatellitePointXY.x = Satellite->getTransform().position.x;
			SatellitePointXY.y = Satellite->getTransform().position.y;
			
			EarthPointXY.x = Earth->getTransform().position.x;
			EarthPointXY.y = Earth->getTransform().position.y;
		}
		graphics->PESO_DrawSimulationData(Satellite);

		graphics->PESO_DrawEllipse(SatellitePointXY, 10.0, 10.0);
		graphics->PESO_DrawEllipse(EarthPointXY, 15.0, 15.0);

		//show results
		graphics->PESO_ShowScreen();
	}
	SDL_Quit();
	return 0;
}
