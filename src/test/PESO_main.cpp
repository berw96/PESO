#include "PESO_demo.h"
#include <map>

class Object;

enum Key {
	T, ESC, QUIT, LAST
};

enum CommandName {
	FOO, EXIT, STOP, START
};

struct Command {
	CommandName name;
	std::string value;

	Command(CommandName name, std::string value) : name(name), value(value) {}
};

class Event {
	std::map<int, std::string> commands;
	bool running;
	SDL_Event event;
	bool keyBooleans[Key::LAST];
	
	void updateKeyBooleans_f(const SDL_Keycode& key, bool keyDown) {
		Key index;

		switch (key) {
		case SDLK_t:
			index = Key::T;
			break;
		case SDLK_ESCAPE:
			index = Key::ESC;
			break;
		case SDL_QUIT:
			index = Key::QUIT;
		default:
			return;
		}
		keyBooleans[index] = keyDown;
	};

public:
	Event() : running(true) {
		for (int i = 0; i < Key::LAST; ++i) {
			//ensure that all keys are by default not triggered on startup
			keyBooleans[i] = false;
		}
	};
	~Event() {};

	void pollEvents_f() {
		while (SDL_PollEvent(&event)) {
			if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
				updateKeyBooleans_f(event.key.keysym.sym, event.type == SDL_KEYDOWN);
			}
		}
	};

	bool isPressed_f(Key key) {
		return keyBooleans[key];
	};

	void setPressed_f(Key key) {
		keyBooleans[key] = true;
	};
};

class Object {
	friend class Physics;

protected:
	double mass;
	Vector3d transform;
	Vector3d acceleration;
	Vector3d speed;
	Vector3d velocity;
	Vector3d netForce;
	Vector3d gravitationalForce;
	Vector3d thrust;

public:
	Object() :
		mass(20),
		transform(Vector3d()),
		acceleration(Vector3d()),
		velocity(Vector3d()),
		gravitationalForce(Vector3d()),
		thrust({0.001, 0.0, 0.0})
	{}
	~Object() {};

	double getObjectMass_f()			const { return mass; }
	Vector3d getObjectTransform_f()		const { return transform; }
	Vector3d getObjectAcceleration_f()	const { return acceleration; }
	Vector3d getObjectNetForce_f()		const { return netForce; }
	Vector3d getObjectGravForce_f()		const { return gravitationalForce; }
	Vector3d getObjectThrust_f()		const { return thrust; }
};

class Physics {
	friend class Object;
	std::vector<std::shared_ptr<Object>> objects;

public:
	Physics() {};
	~Physics() {};

	void registerObject_f(std::shared_ptr<Object> object) {
		objects.push_back(object);
	}

	void mechanics_f(std::vector<std::shared_ptr<Object>> objects) {
		for (auto obj : objects) {
			calculateNetForce_f(*obj);
			calculuateAcceleration_f(*obj);
			accelerate_f(*obj);
			setVelocityMagnitude_f(*obj);
			translateObject_f(*obj);
		}
	}

	void calculateNetForce_f(Object& body) {
		body.netForce.x = body.gravitationalForce.x + body.thrust.x;
		body.netForce.y = body.gravitationalForce.y + body.thrust.y;
		body.netForce.z = body.gravitationalForce.z + body.thrust.z;
	}

	void calculuateAcceleration_f(Object& body) {
		body.acceleration.x = body.netForce.x / body.mass;
		body.acceleration.y = body.netForce.y / body.mass;
		body.acceleration.z = body.netForce.z / body.mass;
	}

	void accelerate_f(Object& body) {
		body.speed.x += body.acceleration.x;
		body.speed.y += body.acceleration.y;
		body.speed.z += body.acceleration.z;
	}

	void setVelocityMagnitude_f(Object& body) {
		body.velocity.x = body.speed.x;
		body.velocity.y = body.speed.y;
		body.velocity.z = body.speed.z;
	}

	void translateObject_f(Object& body) {
		body.transform.x += body.velocity.x;
		body.transform.y += body.velocity.y;
		body.transform.z += body.velocity.z;
	}

	auto getRegisteredObjects_f()		const { return objects; }
};

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
	Event* events = new Event();
	Physics* physics = new Physics();
	std::shared_ptr<Object> object { new Object() };
	std::vector<std::string> inputs;
	std::vector<Command> commands{
		Command(CommandName::FOO, "FOO"),
		Command(CommandName::START, "START")
	};

	physics->registerObject_f(object);

	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_Rect rectangle = {
		(float)(_DEFAULT_WINDOW_WIDTH_/2.f), 
		(float)(_DEFAULT_WINDOW_HEIGHT_/2.f),
		30.f ,
		30.f
	};

	SDL_Window* simulationWindow = SDL_CreateWindow(
		"PESO",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		_DEFAULT_WINDOW_WIDTH_,
		_DEFAULT_WINDOW_HEIGHT_,
		SDL_WINDOW_SHOWN);
	
	SDL_Renderer* simulationRenderer = SDL_CreateRenderer(
		simulationWindow, 
		-1, 
		SDL_RENDERER_ACCELERATED);
	
	std::cout << "Welcome to PESO! Press T to use console input." << std::endl;

	while (running) {
		events->pollEvents_f();

		if (events->isPressed_f(Key::T)) {
			//write directly to input variable with buffered console input
			std::string input;
			std::cout << "Enter command: ";
			std::cin >> input;
			//add input to history
			inputs.push_back(input);
		}
		//check what the user has entered and correlate it with the command name
		for (auto arg : commands) {
			for (auto input : inputs) {
				std::cout << "You typed " << input << "\n";
				//condition preventing negative index
				if (inputs.size() > 0) {
					//compare the most recent input with a valid command
					if (inputs.at(inputs.size() - 1) == arg.value) {
						//check which command has been given
						switch (arg.name) {
						case FOO:
							std::cout << "BAR" << std::endl;
							inputs.clear();
							break;
						case START:
							paused = false;
							inputs.clear();
							break;
						default:
							std::cout << "Invalid command.\n";
							break;
						}
					}
				}
			}
		}
		
		//choose color to clear screen with.
		SDL_SetRenderDrawColor(simulationRenderer, 0, 0, 0, 255);
		//clear screen
		SDL_RenderClear(simulationRenderer);
		//choose color to draw geometry with.
		SDL_SetRenderDrawColor(simulationRenderer, 255, 255, 255, 255);
		
		if (!paused) {
			//apply mechanics of physics engine to all objects registered with it.
			physics->mechanics_f(physics->getRegisteredObjects_f());
			rectangle.x = object->getObjectTransform_f().x;
			rectangle.y = object->getObjectTransform_f().y;
		}
		//draw geometry of provided specs
		SDL_RenderDrawRect(simulationRenderer, &rectangle);
		//show results
		SDL_RenderPresent(simulationRenderer);
	}
	return 0;
}
