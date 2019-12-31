#include "worm.h"

Worm::Worm(Terrain* world,float resolution)
{
	int sizeX = world->width / resolution;
	int sizeY = world->height / resolution;
	float x = rand() % sizeX;
	float y = rand() % sizeY;

	float z = (2+((float)(world->getPointHeight(x, y, resolution))))*20/255;
	x = x * resolution -(float)world->width / 2;
	y = y * resolution -(float)world->width / 2;
	position = glm::vec3(x, z, y);
	rotateX = 0;
	rotateY = 0;
	rotMatrix = glm::mat4(1);
}

void Worm::rotateOx(float rotation)
{
	rotateX += rotation;
}

void Worm::rotateOy(float rotation)
{
	rotateY += rotation;
}