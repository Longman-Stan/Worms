#pragma once
#include <stb/stb_image.h>
#include <string>
#include <algorithm>
#include <iostream>

class Terrain
{
public:
	void loadTerrain(std::string name);
	void terrainDeformation(int x, int y,int radius,int alter_val);
	void alterPixel(int x, int y, int value);
	unsigned char getPointHeight(int x, int y, int resolution);
	float getPointHeight(float x, float y);
	unsigned char* terrain;
	int width, height, nrch;
};