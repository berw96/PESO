/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>

	Made with C++ and Simple DirectMedia Layer (SDL) Version 2.0, under the
	GNU General Public Licence (GPL) and the ZLib Licence.
*/

#include "PESO_filesys.h"

PESO_FileManager::PESO_FileManager() {};
PESO_FileManager::~PESO_FileManager() {};

void PESO_FileManager::PESO_WriteFile(std::vector<PESO_Data>& sessionData) {
	if (sessionData.size() > 0) {
		if ((output_file = fopen(directory, mode)) != NULL) {
			fprintf(stdout, "Opened %s" " successfully.\n", directory);
			
			// iterate through contents of session data and print to output file.
			for (auto data_set : sessionData) {
				if ((fprintf(output_file, "\n" "%s", ctime(&data_set.timestamp))) == -1) {
					fprintf(stderr, "Error writing timestamp to file.\n");
				}
#pragma region MISC
				if ((fprintf(output_file, "Tag: %s" "\n", data_set.tag.c_str())) == -1) {
					fprintf(stderr, "Error writing Tag to file.\n");
				}
				if ((fprintf(output_file, "Mass: %Ls" "\n", std::to_string(data_set.mass).c_str())) == -1) {
					fprintf(stderr, "Error writing Mass to file.\n");
				}
				if ((fprintf(output_file, "Inertia: %Ls" "\n", std::to_string(data_set.inertia).c_str())) == -1) {
					fprintf(stderr, "Error writing Inertia to file.\n");
				}
#pragma endregion
#pragma region TRANSFORM
				if ((fprintf(output_file, "x-pos: %+Ls" "\n", std::to_string(data_set.transform.position.x).c_str())) == -1) {
					fprintf(stderr, "Error writing x-pos to file.\n");
				}
				if ((fprintf(output_file, "y-pos: %+Ls" "\n", std::to_string(data_set.transform.position.y).c_str())) == -1) {
					fprintf(stderr, "Error writing y-pos to file.\n");
				}
				if ((fprintf(output_file, "z-pos: %+Ls" "\n", std::to_string(data_set.transform.position.z).c_str())) == -1) {
					fprintf(stderr, "Error writing z-pos to file.\n");
				}

				if ((fprintf(output_file, "x-rot: %+Ls" "\n", std::to_string(data_set.transform.rotation.x).c_str())) == -1) {
					fprintf(stderr, "Error writing x-rot to file.\n");
				}
				if ((fprintf(output_file, "y-rot: %+Ls" "\n", std::to_string(data_set.transform.rotation.y).c_str())) == -1) {
					fprintf(stderr, "Error writing y-rot to file.\n");
				}
				if ((fprintf(output_file, "z-rot: %+Ls" "\n", std::to_string(data_set.transform.rotation.z).c_str())) == -1) {
					fprintf(stderr, "Error writing z-rot to file.\n");
				}
#pragma endregion
#pragma region LINEAR
				if ((fprintf(output_file, "Net Linear Force (x): %+Ls" "\n", std::to_string(data_set.netLinForce.x).c_str())) == -1) {
					fprintf(stderr, "Error writing Net Linear Force (x) to file.\n");
				}
				if ((fprintf(output_file, "Net Linear Force (y): %+Ls" "\n", std::to_string(data_set.netLinForce.y).c_str())) == -1) {
					fprintf(stderr, "Error writing Net Linear Force (y) to file.\n");
				}
				if ((fprintf(output_file, "Net Linear Force (z): %+Ls" "\n", std::to_string(data_set.netLinForce.z).c_str())) == -1) {
					fprintf(stderr, "Error writing Net Linear Force (z) to file.\n");
				}

				if ((fprintf(output_file, "Gravitational Force (x): %+Ls" "\n", std::to_string(data_set.gravForce.x).c_str())) == -1) {
					fprintf(stderr, "Error writing Gravitational Force (x) to file.\n");
				}
				if ((fprintf(output_file, "Gravitational Force (y): %+Ls" "\n", std::to_string(data_set.gravForce.y).c_str())) == -1) {
					fprintf(stderr, "Error writing Gravitational Force (y) to file.\n");
				}
				if ((fprintf(output_file, "Gravitational Force (z): %+Ls" "\n", std::to_string(data_set.gravForce.z).c_str())) == -1) {
					fprintf(stderr, "Error writing Gravitational Force (z) to file.\n");
				}

				if ((fprintf(output_file, "Thrust (x): %+Ls" "\n", std::to_string(data_set.thrust.x).c_str())) == -1) {
					fprintf(stderr, "Error writing Thrust (x) to file.\n");
				}
				if ((fprintf(output_file, "Thrust (y): %+Ls" "\n", std::to_string(data_set.thrust.y).c_str())) == -1) {
					fprintf(stderr, "Error writing Thrust (y) to file.\n");
				}
				if ((fprintf(output_file, "Thrust (z): %+Ls" "\n", std::to_string(data_set.thrust.z).c_str())) == -1) {
					fprintf(stderr, "Error writing Thrust (z) to file.\n");
				}

				if ((fprintf(output_file, "Linear Velocity (x): %+Ls" "\n", std::to_string(data_set.linVelocity.x).c_str())) == -1) {
					fprintf(stderr, "Error writing Linear Speed (x) to file.\n");
				}
				if ((fprintf(output_file, "Linear Velocity (y): %+Ls" "\n", std::to_string(data_set.linVelocity.y).c_str())) == -1) {
					fprintf(stderr, "Error writing Linear Speed (y) to file.\n");
				}
				if ((fprintf(output_file, "Linear Velocity (z): %+Ls" "\n", std::to_string(data_set.linVelocity.z).c_str())) == -1) {
					fprintf(stderr, "Error writing Linear Speed (z) to file.\n");
				}

				if ((fprintf(output_file, "Linear Acceleration (x): %+Ls" "\n", std::to_string(data_set.linAcceleration.x).c_str())) == -1) {
					fprintf(stderr, "Error writing Linear Acceleration (x) to file.\n");
				}
				if ((fprintf(output_file, "Linear Acceleration (y): %+Ls" "\n", std::to_string(data_set.linAcceleration.y).c_str())) == -1) {
					fprintf(stderr, "Error writing Linear Acceleration (y) to file.\n");
				}
				if ((fprintf(output_file, "Linear Acceleration (z): %+Ls" "\n", std::to_string(data_set.linAcceleration.z).c_str())) == -1) {
					fprintf(stderr, "Error writing Linear Acceleration (z) to file.\n");
				}

				if ((fprintf(output_file, "Linear Momentum (x): %+Ls" "\n", std::to_string(data_set.linMomentum.x).c_str())) == -1) {
					fprintf(stderr, "Error writing Linear Momentum (x) to file.\n");
				}
				if ((fprintf(output_file, "Linear Momentum (y): %+Ls" "\n", std::to_string(data_set.linMomentum.y).c_str())) == -1) {
					fprintf(stderr, "Error writing Linear Momentum (y) to file.\n");
				}
				if ((fprintf(output_file, "Linear Momentum (z): %+Ls" "\n", std::to_string(data_set.linMomentum.z).c_str())) == -1) {
					fprintf(stderr, "Error writing Linear Momentum (z) to file.\n");
				}
#pragma endregion
#pragma region ANGULAR
				if ((fprintf(output_file, "Torque (x): %+Ls" "\n", std::to_string(data_set.torque.x).c_str())) == -1) {
					fprintf(stderr, "Error writing Torque (x) to file.\n");
				}
				if ((fprintf(output_file, "Torque (y): %+Ls" "\n", std::to_string(data_set.torque.y).c_str())) == -1) {
					fprintf(stderr, "Error writing Torque (y) to file.\n");
				}
				if ((fprintf(output_file, "Torque (z): %+Ls" "\n", std::to_string(data_set.torque.z).c_str())) == -1) {
					fprintf(stderr, "Error writing Torque (z) to file.\n");
				}

				if ((fprintf(output_file, "Angular Velocity (x): %+Ls" "\n", std::to_string(data_set.angVelocity.x).c_str())) == -1) {
					fprintf(stderr, "Error writing Angular Velocity (x) to file.\n");
				}
				if ((fprintf(output_file, "Angular Velocity (y): %+Ls" "\n", std::to_string(data_set.angVelocity.y).c_str())) == -1) {
					fprintf(stderr, "Error writing Angular Velocity (y) to file.\n");
				}
				if ((fprintf(output_file, "Angular Velocity (z): %+Ls" "\n", std::to_string(data_set.angVelocity.z).c_str())) == -1) {
					fprintf(stderr, "Error writing Angular Velocity (z) to file.\n");
				}

				if ((fprintf(output_file, "Angular Acceleration (x): %+Ls" "\n", std::to_string(data_set.angAcceleration.x).c_str())) == -1) {
					fprintf(stderr, "Error writing Angular Acceleration (x) to file.\n");
				}
				if ((fprintf(output_file, "Angular Acceleration (y): %+Ls" "\n", std::to_string(data_set.angAcceleration.y).c_str())) == -1) {
					fprintf(stderr, "Error writing Angular Acceleration (y) to file.\n");
				}
				if ((fprintf(output_file, "Angular Acceleration (z): %+Ls" "\n", std::to_string(data_set.angAcceleration.z).c_str())) == -1) {
					fprintf(stderr, "Error writing Angular Acceleration (z) to file.\n");
				}

				if ((fprintf(output_file, "Angular Momentum (x): %+Ls" "\n", std::to_string(data_set.angMomentum.x).c_str())) == -1) {
					fprintf(stderr, "Error writing Angular Momentum (x) to file.\n");
				}
				if ((fprintf(output_file, "Angular Momentum (y): %+Ls" "\n", std::to_string(data_set.angMomentum.y).c_str())) == -1) {
					fprintf(stderr, "Error writing Angular Momentum (y) to file.\n");
				}
				if ((fprintf(output_file, "Angular Momentum (z): %+Ls" "\n", std::to_string(data_set.angMomentum.z).c_str())) == -1) {
					fprintf(stderr, "Error writing Angular Momentum (z) to file.\n");
				}
#pragma endregion
			}
			fprintf(stdout, "Simulation data logged to %s" "\n", directory);
			SDL_Delay(_SECOND_);
		}
		else {
			fprintf(stderr, "Error opening %s" "\n", directory);
			SDL_Delay(_SECOND_);
			exit(EXIT_FAILURE);
		}
		if ((fclose(output_file)) != EOF) {
			fprintf(stdout, "Closed file successfully.\n");
			SDL_Delay(_SECOND_);
		}
		else {
			fprintf(stderr, "Error closing file.\n");
			SDL_Delay(_SECOND_);
			exit(EXIT_FAILURE);
		}
	}
}
