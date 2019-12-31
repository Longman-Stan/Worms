#pragma once
#include <Core/Engine.h>
#include "Terrain.h"

class Worm
{
public:
	glm::vec3 position;
	float rotateX;
	float rotateY;
	glm::mat4 rotMatrix;

	Worm(Terrain* world,float resolution);
	void rotateOx(float rotation);
	void rotateOy(float rotation);
};