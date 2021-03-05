/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>

	Made with C++ and Simple DirectMedia Layer (SDL) Version 2.0, under the
	GNU General Public Licence (GPL) and the ZLib Licence.
*/

#ifndef _FILE_SYSTEM_
#define _FILE_SYSTEM_

#include <stdio.h>
#include <iostream>
#include "../subsystems/PESO_physics.h"

class PESO_FileManager {
	FILE* output_file = nullptr;
	const char* directory = "../out/session_data.peso";
	const char* mode = "w+";

public:
	PESO_FileManager();
	~PESO_FileManager();

	void PESO_WriteFile(std::vector<PESO_Data>& sessionData, PESO_Object& satellite);
};

#endif