#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include "../include/math.h"
#include <time.h>
#include "Terrain.h"

namespace Objects
{
	// Creates the meshes I'll use in the program
	Mesh* CreateTerrain(std::string terrain_name, int width, int height, float resolution, glm::vec2* heightMapSize,Terrain* heightmap);
	
	Mesh* CreateWorm(std::string worm_name);

	Mesh* updateTerrain(Mesh* terrain, float resolution, Terrain* heightmap);
}

