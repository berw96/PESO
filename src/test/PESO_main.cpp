#include "PESO_demo.h"
#include <map>

class Object;

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

	bool running = true;
	bool paused = true;
	PESO_Events* events = new PESO_Events();
	PESO_Physics* physics = new PESO_Physics();
	std::shared_ptr<PESO_Object> object{ new PESO_Object(Vector3d(), Vector3d(), 1.0, PESO_Transform(), 1.0, "Object 1", Vector3d(0.0001, 0.0, 0.0001)) };

	physics->PESO_RegisterObject(object);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Could not init SDL %s\n", SDL_GetError());
		return 1;
	};
	
	SDL_Rect XYRectangle = {
		(float)(_DEFAULT_WINDOW_WIDTH_/2.f), 
		(float)(_DEFAULT_WINDOW_HEIGHT_/2.f),
		30.f ,
		30.f
	};

	SDL_Rect XZRectangle = {
		(float)(_DEFAULT_WINDOW_WIDTH_ / 2.f),
		(float)(_DEFAULT_WINDOW_HEIGHT_ / 2.f),
		30.f ,
		30.f
	};
	
	SDL_Rect YZRectangle = {
		(float)(_DEFAULT_WINDOW_WIDTH_ / 2.f),
		(float)(_DEFAULT_WINDOW_HEIGHT_ / 2.f),
		30.f ,
		30.f
	};

	SDL_Window* XYViewport = SDL_CreateWindow(
		"XY Viewport",
		_DEFAULT_WINDOW_POS_X_,
		_DEFAULT_WINDOW_POS_Y_,
		_DEFAULT_WINDOW_WIDTH_ / 6,
		_DEFAULT_WINDOW_HEIGHT_ / 6,
		SDL_WINDOW_SHOWN
		);

	SDL_Window* XZViewport = SDL_CreateWindow(
		"XZ Viewport",
		_DEFAULT_WINDOW_POS_X_,
		_DEFAULT_WINDOW_POS_Y_ + _DEFAULT_WINDOW_HEIGHT_ / 6,
		_DEFAULT_WINDOW_WIDTH_ / 6,
		_DEFAULT_WINDOW_HEIGHT_ / 6,
		SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_GRABBED
	);

	SDL_Window* YZViewport = SDL_CreateWindow(
		"YZ Viewport",
		_DEFAULT_WINDOW_POS_X_,
		_DEFAULT_WINDOW_POS_Y_ + 2 * (_DEFAULT_WINDOW_HEIGHT_ / 6),
		_DEFAULT_WINDOW_WIDTH_ / 6,
		_DEFAULT_WINDOW_HEIGHT_ / 6,
		SDL_WINDOW_SHOWN
	);

	SDL_Renderer* XYRenderer = SDL_CreateRenderer(
		XYViewport, 
		-1, 
		SDL_RENDERER_ACCELERATED);

	SDL_Renderer* XZRenderer = SDL_CreateRenderer(
		XZViewport,
		-1,
		SDL_RENDERER_ACCELERATED
	);
	
	SDL_Renderer* YZRenderer = SDL_CreateRenderer(
		YZViewport,
		-1,
		SDL_RENDERER_ACCELERATED
	);
	
	std::cout << "Welcome to PESO! Press T to use console input." << std::endl;

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
		
		//choose color to clear screen with.
		SDL_SetRenderDrawColor(XYRenderer, 0, 0, 0, 255);
		SDL_SetRenderDrawColor(XZRenderer, 0, 0, 0, 255);
		SDL_SetRenderDrawColor(YZRenderer, 0, 0, 0, 255);
		//clear screen
		SDL_RenderClear(XYRenderer);
		SDL_RenderClear(XZRenderer);
		SDL_RenderClear(YZRenderer);
		//choose color to draw geometry with.
		SDL_SetRenderDrawColor(XYRenderer, 255, 255, 255, 255);
		SDL_SetRenderDrawColor(XZRenderer, 255, 255, 255, 255);
		SDL_SetRenderDrawColor(YZRenderer, 255, 255, 255, 255);
		
		if (!paused) {
			//apply mechanics of physics engine to all objects registered with it.
			physics->PESO_ApplyLinearMechanics();
			XYRectangle.x = object->getTransform().position.x;
			XYRectangle.y = object->getTransform().position.y;
			XZRectangle.x = object->getTransform().position.x;
			XZRectangle.y = object->getTransform().position.z;
			YZRectangle.x = object->getTransform().position.y;
			YZRectangle.y = object->getTransform().position.z;
		}
		//draw geometry of provided specs
		SDL_RenderDrawRect(XYRenderer, &XYRectangle);
		SDL_RenderDrawRect(XZRenderer, &XZRectangle);
		SDL_RenderDrawRect(YZRenderer, &YZRectangle);
		//show results
		SDL_RenderPresent(XYRenderer);
		SDL_RenderPresent(XZRenderer);
		SDL_RenderPresent(YZRenderer);
	}
	SDL_DestroyRenderer(XYRenderer);
	SDL_DestroyRenderer(XZRenderer);
	SDL_DestroyRenderer(YZRenderer);
	SDL_DestroyWindow(XYViewport);
	SDL_DestroyWindow(XZViewport);
	SDL_DestroyWindow(YZViewport);
	SDL_Quit();
	return 0;
}
