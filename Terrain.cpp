#include "Terrain.h"

void Terrain::loadTerrain(std::string name)
{
	terrain=stbi_load(name.c_str(), &width, &height, &nrch, STBI_grey);
}

void Terrain::alterPixel(int x, int y, int value)
{
	int val = value + terrain[y * width + x];
	//std::cout << x << ' ' << y << ' ' << (int)terrain[y * width + x]<<' '<< val << '\n';
	if (val < 0)
		val = 0;
	if (val > 255)
		val = 255;
	terrain[y * width + x] = val;
}

unsigned char Terrain::getPointHeight(int x, int y, int resolution)
{	
	return terrain[(y*resolution) * width + x*resolution];
}

float Terrain::getPointHeight(float x, float y)
{
	return terrain[ (int)(y+(float)height/2) * width + (int)(x+float(width/2))];
}

void Terrain::terrainDeformation(int x, int y, int radius,int alter_val)
{
	//std::cout << x << ' ' << y << '\n';
	int i, j,a,b;
	int loX, hiX, loY, hiY;
	loX = x - radius; loX = std::max(0,loX);
	hiX = x + radius; hiX = std::min(width, hiX);
	loY = y - radius; loY = std::max(0, loY);
	hiY = y + radius; hiY = std::min(height, hiY);
	for(i=loY;i<=hiY;i++)
		for (j = loX; j <= hiX; j++)
		{
			a = abs(i - y);
			b = abs(j - x);
			if (a * a + b * b < radius * radius)
				alterPixel(i, j, alter_val);
		}
}