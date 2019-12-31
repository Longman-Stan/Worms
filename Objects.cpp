#include "Objects.h"
#include <iostream>

Mesh* Objects::CreateTerrain(std::string terrain_name,int width,int height,float resolution,glm::vec2 *heightMapSize,Terrain* heightmap)
{
	std::vector<VertexFormat> vertices;

	int vertices_width = width/resolution;
	int vertices_height = height/resolution;

	heightMapSize->x = vertices_width;
	heightMapSize->y = vertices_height;

	glm::vec3 base_position = glm::vec3(-(float)width/2,0,-(float)height/2);
	int i, j;
	for (i = 0; i < vertices_height; i++)
	{
		for (j = 0; j < vertices_width; j++)
		{
			vertices.push_back(VertexFormat(base_position + glm::vec3((float)j * resolution, (float)heightmap->getPointHeight(j,i,resolution)*20/255, (float)i * resolution)));
		}
	}

	//for (auto it : vertices)
	//{
	//	std::cout << it.position << '\n';
	//}
	i = 0;
	for (i=0;i<vertices.size();i++)
	{
		glm::vec3 aux = vertices[i].position - base_position;
		vertices[i].text_coord = glm::vec2(((float)aux.x)/width, ((float)aux.z)/height);
		//std::cout << aux << ' ' << vertices[i].text_coord << '\n';;
	}

	std::vector<unsigned short> indices;
	for (i = 0; i < vertices_height - 1; i++)
	{
		for (j = 0; j < vertices_width - 1; j++)
		{
			int base = i * vertices_width + j;
			indices.push_back(base); indices.push_back(base + vertices_width);  indices.push_back(base + vertices_width + 1);
			indices.push_back(base); indices.push_back(base + vertices_width + 1); indices.push_back(base + 1);
		}
		if (i == 250) break;
	}

	Mesh* terrain = new Mesh(terrain_name);
	terrain->InitFromData(vertices, indices);

	return terrain;
}


Mesh* Objects::updateTerrain(Mesh* terrain,float resolution,Terrain* heightmap)
{
	int vertices_width = heightmap->width / resolution;
	int vertices_height = heightmap->height / resolution;
	int i, j;

	Mesh* aux = new Mesh("terrain");

	for (i = 0; i < vertices_height; i++)
	{
		for (j = 0; j < vertices_width; j++)
		{
			//if (terrain->vertices[i*vertices_height+j].position.y != (float)heightmap->getPointHeight(j, i, resolution) * 20 / 255)
			//	std::cout << i << ' ' << j << ' ' << terrain->vertices[(i * vertices_height + j)].position.y<<' '<<(float)heightmap->getPointHeight(j, i, resolution) * 20 / 255 << '\n';
			terrain->vertices[i * vertices_height + j].position.y = (float)heightmap->getPointHeight(j, i, resolution) * 20 / 255;
		}
	}
	aux->InitFromData(terrain->vertices, terrain->indices);
	free(terrain);
	return aux;
}

Mesh* Objects::CreateWorm(std::string worm_name)
{
	glm::vec3 wormColor = glm::vec3(0.93f, 0.84f, 0.82f);
	glm::vec3 white = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 black = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 green = glm::vec3(0.25f, 0.41f, 0.15f);
	std::vector<VertexFormat> vertices =
	{
		//dreapta
		VertexFormat(glm::vec3(-1.5f, 2.f,-2.f),wormColor,glm::vec3(1,0,0)), //0
		VertexFormat(glm::vec3(-1.5f, 4.f,-2.f),wormColor,glm::vec3(1,0,0)), //1
		VertexFormat(glm::vec3(-1.5f, 4.f, 1.f),wormColor,glm::vec3(1,0,0)), //2
		VertexFormat(glm::vec3(-1.5f, 0.f, 1.f),wormColor,glm::vec3(1,0,0)), //3
		VertexFormat(glm::vec3(-1.5f,-2.f,-1.f),wormColor,glm::vec3(1,0,0)), //4
		VertexFormat(glm::vec3(-1.5f,-2.f,-6.f),wormColor,glm::vec3(1,0,0)), //5
		VertexFormat(glm::vec3(-1.5f, 0.f,-3.f),wormColor,glm::vec3(1,0,0)), //6

		//stanga
		VertexFormat(glm::vec3(1.5f, 2.f,-2.f),wormColor,glm::vec3(-1,0,0)), //7
		VertexFormat(glm::vec3(1.5f, 4.f,-2.f),wormColor,glm::vec3(-1,0,0)), //8
		VertexFormat(glm::vec3(1.5f, 4.f, 1.f),wormColor,glm::vec3(-1,0,0)), //9
		VertexFormat(glm::vec3(1.5f, 0.f, 1.f),wormColor,glm::vec3(-1,0,0)), //10
		VertexFormat(glm::vec3(1.5f,-2.f,-1.f),wormColor,glm::vec3(-1,0,0)), //11
		VertexFormat(glm::vec3(1.5f,-2.f,-6.f),wormColor,glm::vec3(-1,0,0)), //12
		VertexFormat(glm::vec3(1.5f, 0.f,-3.f),wormColor,glm::vec3(-1,0,0)), //13

		//sus
		VertexFormat(glm::vec3(-1.5f, 4.f,-2.f),wormColor,glm::vec3(0,1,0)), //1 -14
		VertexFormat(glm::vec3(-1.5f, 4.f, 1.f),wormColor,glm::vec3(0,1,0)), //2 -15
		VertexFormat(glm::vec3(1.5f, 4.f,-2.f),wormColor,glm::vec3(0,1,0)), //8 -16
		VertexFormat(glm::vec3(1.5f, 4.f, 1.f),wormColor,glm::vec3(0,1,0)), //9 -17

		//jos
		VertexFormat(glm::vec3(-1.5f,-2.f,-1.f),wormColor,glm::vec3(0,-1,0)), //4 -18
		VertexFormat(glm::vec3(-1.5f,-2.f,-6.f),wormColor,glm::vec3(0,-1,0)), //5 -19
		VertexFormat(glm::vec3(1.5f,-2.f,-1.f),wormColor,glm::vec3(0,-1,0)), //11 -20
		VertexFormat(glm::vec3(1.5f,-2.f,-6.f),wormColor,glm::vec3(0,-1,0)), //12 -21

		//fata
		VertexFormat(glm::vec3(-1.5f, 4.f, 1.f),wormColor,glm::vec3(0,0,1)), //2 -22
		VertexFormat(glm::vec3(-1.5f, 0.f, 1.f),wormColor,glm::vec3(0,0,1)), //3 -23
		VertexFormat(glm::vec3(1.5f, 4.f, 1.f),wormColor,glm::vec3(0,0,1)), //9 -24
		VertexFormat(glm::vec3(1.5f, 0.f, 1.f),wormColor,glm::vec3(0,0,1)), //10 -25
		VertexFormat(glm::vec3(-1.5f, 0.f, 1.f),wormColor,glm::normalize(glm::vec3(0,-1,1))), //3 -26
		VertexFormat(glm::vec3(-1.5f,-2.f,-1.f),wormColor,glm::normalize(glm::vec3(0,-1,1))), //4 -27
		VertexFormat(glm::vec3(1.5f, 0.f, 1.f),wormColor,glm::normalize(glm::vec3(0,-1,1))), //10 -28
		VertexFormat(glm::vec3(1.5f,-2.f,-1.f),wormColor,glm::normalize(glm::vec3(0,-1,1))), //11 -29

		//spate
		VertexFormat(glm::vec3(-1.5f, 2.f,-2.f),wormColor,glm::vec3(0,0,-1)), //0 -30
		VertexFormat(glm::vec3(-1.5f, 4.f,-2.f),wormColor,glm::vec3(0,0,-1)), //1 -31
		VertexFormat(glm::vec3(1.5f, 2.f,-2.f),wormColor,glm::vec3(0,0,-1)), //7 -32
		VertexFormat(glm::vec3(1.5f, 4.f,-2.f),wormColor,glm::vec3(0,0,-1)), //8 -33

		VertexFormat(glm::vec3(-1.5f, 2.f,-2.f),wormColor,glm::normalize(glm::vec3(0,3,-6))), //0 -34
		VertexFormat(glm::vec3(-1.5f, 0.f,-3.f),wormColor,glm::normalize(glm::vec3(0,3,-6))), //6 -35
		VertexFormat(glm::vec3(1.5f, 2.f,-2.f),wormColor,glm::normalize(glm::vec3(0,3,-6))), //7 -36
		VertexFormat(glm::vec3(1.5f, 0.f,-3.f),wormColor,glm::normalize(glm::vec3(0,3,-6))), //13-37

		VertexFormat(glm::vec3(-1.5f,-2.f,-6.f),wormColor,glm::normalize(glm::vec3(0,9,-6))), //5-38
		VertexFormat(glm::vec3(-1.5f, 0.f,-3.f),wormColor,glm::normalize(glm::vec3(0,9,-6))), //6-39
		VertexFormat(glm::vec3(1.5f,-2.f,-6.f),wormColor,glm::normalize(glm::vec3(0,9,-6))), //12-40
		VertexFormat(glm::vec3(1.5f, 0.f,-3.f),wormColor,glm::normalize(glm::vec3(0,9,-6))), //13-41

		//ochi
		VertexFormat(glm::vec3(-1.f, 3.f,1.1f),white,glm::vec3(0,0,1)), //42
		VertexFormat(glm::vec3(-0.5f, 3.f,1.1f),white,glm::vec3(0,0,1)), //43
		VertexFormat(glm::vec3(-0.5f, 1.f,1.1f),white,glm::vec3(0,0,1)), //44
		VertexFormat(glm::vec3(-1.f, 1.f,1.1f),white,glm::vec3(0,0,1)), //45

		VertexFormat(glm::vec3(1.f, 3.f,1.1f),white,glm::vec3(0,0,1)), //46
		VertexFormat(glm::vec3(0.5f, 3.f,1.1f),white,glm::vec3(0,0,1)), //47
		VertexFormat(glm::vec3(0.5f, 1.f,1.1f),white,glm::vec3(0,0,1)), //48
		VertexFormat(glm::vec3(1.f, 1.f,1.1f),white,glm::vec3(0,0,1)), //49

		VertexFormat(glm::vec3(-0.875f, 2.75f,1.2f),black,glm::vec3(0,0,1)), //50
		VertexFormat(glm::vec3(-0.625f, 2.75f,1.2f),black,glm::vec3(0,0,1)), //51
		VertexFormat(glm::vec3(-0.625f, 1.25f,1.2f),black,glm::vec3(0,0,1)), //52
		VertexFormat(glm::vec3(-0.875f, 1.25f,1.2f),black,glm::vec3(0,0,1)), //53

		VertexFormat(glm::vec3(0.875f, 2.75f,1.2f),black,glm::vec3(0,0,1)), //54
		VertexFormat(glm::vec3(0.625f, 2.75f,1.2f),black,glm::vec3(0,0,1)), //55
		VertexFormat(glm::vec3(0.625f, 1.25f,1.2f),black,glm::vec3(0,0,1)), //56
		VertexFormat(glm::vec3(0.875f, 1.25f,1.2f),black,glm::vec3(0,0,1)), //57

		VertexFormat(glm::vec3(-1.25f, 3.5f,1.3f),black,glm::vec3(0,0,1)), //58
		VertexFormat(glm::vec3(-0.2f, 3.2f,1.3f),black,glm::vec3(0,0,1)), //59
		VertexFormat(glm::vec3(-0.2f, 3.f,1.3f),black,glm::vec3(0,0,1)), //60

		VertexFormat(glm::vec3(0.2f, 3.f,1.1f),black,glm::vec3(0,0,1)), //61
		VertexFormat(glm::vec3(0.2f, 3.2f,1.1f),black,glm::vec3(0,0,1)), //62
		VertexFormat(glm::vec3(1.25f, 3.5f,1.1f),black,glm::vec3(0,0,1)), //63

		//bazooka
		VertexFormat(glm::vec3(2.5f, 0.f,-4.f),green,glm::vec3(1,0,0)), //64
		VertexFormat(glm::vec3(2.5f, 2.f,-4.f),green,glm::vec3(1,0,0)), //65
		VertexFormat(glm::vec3(2.5f, 2.f, 3.f),green,glm::vec3(1,0,0)), //66
		VertexFormat(glm::vec3(2.5f, 0.f, 3.f),green,glm::vec3(1,0,0)), //67
		VertexFormat(glm::vec3(1.51f, 0.f,-4.f),green,glm::vec3(-1,0,0)), //68
		VertexFormat(glm::vec3(1.51f, 2.f,-4.f),green,glm::vec3(-1,0,0)), //69
		VertexFormat(glm::vec3(1.51f, 2.f, 3.f),green,glm::vec3(-1,0,0)), //70
		VertexFormat(glm::vec3(1.51f, 0.f, 3.f),green,glm::vec3(-1,0,0)), //71

		VertexFormat(glm::vec3(2.5f, 2.f,-4.f),green,glm::vec3(0,1,0)), //72
		VertexFormat(glm::vec3(2.5f, 2.f, 3.f),green,glm::vec3(0,1,0)), //73
		VertexFormat(glm::vec3(1.51f, 2.f,-4.f),green,glm::vec3(0,1,0)), //74
		VertexFormat(glm::vec3(1.51f, 2.f, 3.f),green,glm::vec3(0,1,0)), //75

		VertexFormat(glm::vec3(2.5f, 0.f, -4.f), green, glm::vec3(0, -1, 0)), //76
		VertexFormat(glm::vec3(2.5f, 0.f, 3.f), green, glm::vec3(0, -1, 0)), //77
		VertexFormat(glm::vec3(1.51f, 0.f, -4.f), green, glm::vec3(0, -1, 0)), //78
		VertexFormat(glm::vec3(1.51f, 0.f, 3.f), green, glm::vec3(0, -1, 0)), //79

		VertexFormat(glm::vec3(2.5f, 2.f, 3.f), green, glm::vec3(0, 0, 1)), //80
		VertexFormat(glm::vec3(2.5f, 0.f, 3.f), green, glm::vec3(0, 0, 1)), //81
		VertexFormat(glm::vec3(1.51f, 2.f, 3.f), green, glm::vec3(0, 0, 1)), //82
		VertexFormat(glm::vec3(1.51f, 0.f, 3.f), green, glm::vec3(0, 0, 1)), //83

		VertexFormat(glm::vec3(2.5f, 0.f, -4.f), green, glm::vec3(0, 0,-1)), //84
		VertexFormat(glm::vec3(2.5f, 2.f, -4.f), green, glm::vec3(0, 0,-1)), //85
		VertexFormat(glm::vec3(1.51f, 0.f, -4.f), green, glm::vec3(0, 0,-1)), //86
		VertexFormat(glm::vec3(1.51f, 2.f, -4.f), green, glm::vec3(0, 0,-1)), //87

		//manuta
		VertexFormat(glm::vec3(2.52f, 2.f, 0.f), wormColor, glm::vec3(1, 0, 0)), //88
		VertexFormat(glm::vec3(2.52f, 2.f, -1.f), wormColor, glm::vec3(1, 0, 0)), //89
		VertexFormat(glm::vec3(2.52f, 0.f, -1.f), wormColor, glm::vec3(1, 0, 0)), //90
		VertexFormat(glm::vec3(2.52f, 0.f, 0.f), wormColor, glm::vec3(1, 0, 0)), //91

		VertexFormat(glm::vec3(1.48f, 1.f, 2.5f), wormColor, glm::vec3(-1, 0, 0)), //92
		VertexFormat(glm::vec3(1.48f, 1.f, 1.5f), wormColor, glm::vec3(-1, 0, 0)), //93
		VertexFormat(glm::vec3(1.48f, 0.f, 1.5f), wormColor, glm::vec3(-1, 0, 0)), //94
		VertexFormat(glm::vec3(1.48f, 0.f, 2.5f), wormColor, glm::vec3(-1, 0, 0)), //95
	};

	std::vector<unsigned short> indices = {
		1,0,2,
		2,0,3,
		0,4,3,
		0,6,4,
		6,5,4,
		8,9,10,
		8,10,7,
		7,10,11,
		7,11,13,
		13,11,12,

		//sus
		17,14,15,
		17,16,14,

		//jos
		19,21,20,
		19,20,18,

		//fata
		22,23,24,
		24,23,25,
		26,27,28,
		27,29,28,

		//spate
		31,32,30,
		31,33,32,
		34,37,35,
		34,36,37,
		39,40,38,
		39,41,40,

		//ochi
		42,44,43,
		42,45,44,
		46,47,48,
		46,48,49,
		50,52,51,
		50,53,52,
		54,55,56,
		54,56,57,

		//sprancene
		58,60,59,
		61,63,62,

		//bazooka
		66,64,65,
		66,67,64,
		70,69,68,
		70,68,71,
		
		75,72,74,
		75,73,72,
		79,78,76,
		79,76,77,

		82,81,80,
		82,83,81,
		87,85,84,
		87,84,86,

		//manuta
		88,90,89,
		88,91,90,
		92,93,94,
		92,94,95
	};

	Mesh* worm = new Mesh(worm_name);
	worm->InitFromData(vertices, indices);

	return worm;
}