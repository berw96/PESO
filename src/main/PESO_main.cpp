#include "../subsystems/PESO.h"
#include <map>

int main(int argc, char* args[]) {
	/*std::string in_file_name = "../saves/testProject.peso";
	std::ifstream in_file {in_file_name, std::ios::in | std::ios::binary};
	std::string s;
	int i;
	char c;

	if (in_file.is_open()) {
		std::cout << "Opened file: " << in_file_name << std::endl;;
	}
	while (!in_file.eof()) {
		in_file >> s;
		in_file >> i;
		in_file >> c;
	}
	in_file.close();
	std::cout << "s = " << s << std::endl;
	std::cout << "i = " << i << std::endl;
	std::cout << "c = " << c << std::endl;
	std::ofstream out_file{ in_file_name, std::ios::app | std::ios::binary };
	if (out_file.is_open()) {
		std::cout << "Opened file: " << in_file_name << std::endl;
		out_file << s;
	}
	out_file.close();*/
	std::cout << "Welcome to PESO! Press R to play, P to pause or ESCAPE to quit." << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Could not init SDL %s\n", SDL_GetError());
		return 1;
	};

	bool running = true;
	bool paused = true;
	PESO_Events* events		= new PESO_Events();
	PESO_Graphics* graphics = new PESO_Graphics();
	PESO_Physics* physics	= new PESO_Physics();

	std::shared_ptr<PESO_Object> object1{
		new PESO_Object(Vector3d(),
		Vector3d(),
		10000.0,
		PESO_Transform(Vector3d(100.0, 500.0, 20.0),
		Vector3d()),
		1.0,
		"Object 1",
		Vector3d(10.0, 0.0, 0.0))
	};
	
	std::shared_ptr<PESO_Object> object2{
		new PESO_Object(Vector3d(),
		Vector3d(),
		200000.0,
		PESO_Transform(Vector3d(500.0, 30.0, 15.0),
		Vector3d()),
		1.0,
		"Object 2",
		Vector3d())
	};
	physics->PESO_RegisterObject(object1);
	physics->PESO_RegisterObject(object2);

	Point2d object1Point = {
		object1->getPosition().x,
		object1->getPosition().y
	};
	
	Point2d object2Point = {
		object2->getPosition().x,
		object2->getPosition().y
	};

	Line2i objectLine = {
		object1Point,
		object2Point
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
		if (events->PESO_KeyIsPressed(Key::RIGHT_ARROW)) {
			std::cout << "right arrow pressed\n";
			SDL_Delay(500);
		}
		
		//clear screen
		graphics->PESO_ClearScreen();
		
		if (!paused) {
			//apply mechanics of physics engine to all objects registered with it.
			physics->PESO_ApplyLinearMechanics();
			//graphical representation mimics object physics
			object1Point.x = object1->getTransform().position.x;
			object1Point.y = object1->getTransform().position.y;
			objectLine.startPoint.x = object1Point.x;
			objectLine.startPoint.y = object1Point.y;
			
			object2Point.x = object2->getTransform().position.x;
			object2Point.y = object2->getTransform().position.y;
			objectLine.endPoint.x = object2Point.x;
			objectLine.endPoint.y = object2Point.y;
		}
		//param should be getter for highlighted object
		graphics->PESO_DrawSimulationData(object1);

		graphics->PESO_DrawEllipse(object1Point, 10.0, 10.0);
		graphics->PESO_DrawEllipse(object2Point, 15.0, 15.0);
		graphics->PESO_DrawLineSegment(objectLine);

		//show results
		graphics->PESO_ShowScreen();
	}
	SDL_Quit();
	return 0;
}
