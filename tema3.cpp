#include "tema3.h"
#include <vector>
#include <string>
#include <iostream>

#include <map>

#include "Objects.h"

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::loadTextures()
{
	const std::string textureLoc = "Source/tema3/Textures/";

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "grass.png").c_str(), GL_REPEAT);
		mapTextures["grass"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "grass2.png").c_str(), GL_REPEAT);
		mapTextures["grass2"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "bomb.png").c_str(), GL_REPEAT);
		mapTextures["bomb"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "earth.png").c_str(), GL_REPEAT);
		mapTextures["earth"] = texture;
	}

	std::vector<std::string> faces //get the skybox texture
	{
		"Source/tema3/skybox2/right.jpg",
		"Source/tema3/skybox2/left.jpg",
		"Source/tema3/skybox2/top.jpg",
		"Source/tema3/skybox2/bottom.jpg",
		"Source/tema3/skybox2/front.jpg",
		"Source/tema3/skybox2/back.jpg"
	};
	cubeMapTexture = loadCubemap(faces);

	heightmap->loadTerrain(textureLoc + "heightmap.png");
	
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "heightmap.png").c_str(), GL_CLAMP);
		mapTextures["heightmap"] = texture;
	}
}

void Tema3::loadMeshes()
{
	Mesh* terrain = Objects::CreateTerrain("terrain",heightmap->width,heightmap->height,resolution,&terrainResolution,heightmap);
	meshes["terrain"] = terrain;

	Mesh* worm = Objects::CreateWorm("worm");
	meshes["worm"] = worm;

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{

		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		std::vector<glm::vec3> vertices
		{
			glm::vec3(-128.f, 0.f, -128.f),	// Top Right
			glm::vec3(128.f,  0.f, -128.f),	// Bottom Right
			glm::vec3(-128.f, 0.f, 128.f),	// Bottom Left
			glm::vec3(128.f,  0.f, 128.f),	// Top Left
		};

		std::vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0)
		};

		// TODO : Complete texture coordinates for the square
		std::vector<glm::vec2> textureCoords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.f,0.f),
			glm::vec2(0.f,1.f),
			glm::vec2(1.f,1.f)
		};

		std::vector<unsigned short> indices =
		{
			0, 2, 3,
			0, 3, 1
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}
}

void Tema3::loadShaders()
{
	const std::string sourceLoc = "Source/tema3/Shaders/";
	Shader* shader = new Shader("TerrainShader");
	shader->AddShader(sourceLoc + "TerrainVS.glsl", GL_VERTEX_SHADER);
	shader->AddShader(sourceLoc + "TerrainFS.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	Shader* shader4 = new Shader("ObjectShader");
	shader4->AddShader(sourceLoc + "ObjectVS.glsl", GL_VERTEX_SHADER);
	shader4->AddShader(sourceLoc + "ObjectFS.glsl", GL_FRAGMENT_SHADER);
	shader4->CreateAndLink();
	shaders[shader4->GetName()] = shader4;

	Shader* shader5 = new Shader("SkyBox");
	shader5->AddShader(sourceLoc + "SkyVS.glsl", GL_VERTEX_SHADER);
	shader5->AddShader(sourceLoc + "SkyFS.glsl", GL_FRAGMENT_SHADER);
	shader5->CreateAndLink();
	shaders[shader5->GetName()] = shader5;

}

void Tema3::Init()
{
	//glEnable(GL_CULL_FACE); //enable cull face
	//glCullFace(GL_FRONT);
	//glEnable(GL_BLEND);  //enable alpha blending
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand( time(NULL) );

	renderCameraTarget = false;


	heightmap = new Terrain();
	loadTextures();
	loadMeshes(); //load the meshes
	loadShaders();
	
	
	//setup constant model matrices
	skyMM = glm::mat4(1);
	//skyMM = glm::translate(skyMM, glm::vec3(0, 2.f, 0));
	skyMM = glm::scale(skyMM, glm::vec3(1000));

	terrainMM = glm::mat4(1);
	//terrainMM = glm::scale(terrainMM, glm::vec3(10));

	lightPosition = glm::vec3(0, 75, 0);
	lightDirection = glm::vec3(0, -1, 0);
	materialShininess = 30;
	materialKd = 2000.f;
	materialKs = 2.f;

	spot = 0;
	cutoff = 30;
	globalTime = 0;

	pl1 = new Worm(heightmap,resolution);
	pl2 = new Worm(heightmap, resolution);
	activePlayer = 1;

	cam_pl1 = new Camera(); //set the camera parameters
	camPosT = pl1->position+glm::vec3(0.f, 0.f, -2.f);
	camLookAtT = pl1->position+glm::vec3(0, 0.f, 1.f);
	cam_pl1->Set(camPosT, camLookAtT, glm::vec3(0, 1, 0));

	cam_pl2 = new Camera();
	camPosT = pl2->position + glm::vec3(0.f, 0.f, -2.f);
	camLookAtT = pl2->position + glm::vec3(0, 0.f, 1.f);
	cam_pl2->Set(camPosT, camLookAtT, glm::vec3(0, 1, 0));

	camera = cam_pl1;

	Pfov = 30;
	inaltime = 1;
	latime = 1;
	cameraType = 't';
	projectionMatrix = glm::perspective(RADIANS(30), window->props.aspectRatio, 0.01f, 1100.0f);

	projectile = false;
	projectile_speed = 15.f;

	weapon_type = ROCKET;

	std::cout << pl1->position << '\n';
}




void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.51f, 0.40f, 0.53f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
	RenderSkyBox(skyMM);
	//terrainMM = glm::scale(terrainMM, glm::vec3(0.1f));
	//RenderMesh(meshes["terrain"], shaders["TerrainShader"],terrainMM );
	//RenderSimpleMesh(meshes["terrain"], shaders["simple"], terrainMM, mapTextures["heightmap"]);
	RenderTerrain(meshes["terrain"], shaders["TerrainShader"], mapTextures["heightmap"], mapTextures["grass2"], terrainMM);

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
	}

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pl1->position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
	modelMatrix = glm::rotate(modelMatrix, pl1->rotateY, glm::vec3(0,1,0));
	modelMatrix = glm::rotate(modelMatrix, pl1->rotateX, glm::vec3(1,0,0));
	RenderSimpleMesh(meshes["worm"], shaders["ObjectShader"], modelMatrix,pl1->position);

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pl2->position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
	modelMatrix = glm::rotate(modelMatrix, pl2->rotateY, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, pl2->rotateX, glm::vec3(1, 0, 0));
	RenderSimpleMesh(meshes["worm"], shaders["ObjectShader"], modelMatrix, pl2->position);

	if (projectile)
	{
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, projectile_position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		Texture2D* text;
		if (weapon_type == ROCKET)
			text = mapTextures["bomb"];
		else
			text = mapTextures["grass"];
		RenderSimpleMesh(meshes["sphere"], shaders["ObjectShader"], modelMatrix, projectile_position, text);
	}

	if (renderCameraTarget)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, pl1->position+glm::vec3(0,1,0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
	}
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,const glm::vec3 player_pos,Texture2D* tex)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	{
		glm::vec3 lightPosition = player_pos+glm::vec3(0,1,0);
		glm::vec3  lightDirection = glm::vec3(0,-1,0);
		float materialShininess = 30;
		float materialKd=0.5f;
		float materialKs=0.5f;
		if (tex)
		{
			materialKd = 1.f;
			materialKs = 1.f;
		}
		// Set shader uniforms for light & material properties
		// TODO: Set light position uniform
		int light_position = glGetUniformLocation(shader->program, "light_position");
		glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

		int light_direction = glGetUniformLocation(shader->program, "light_direction");
		glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

		// TODO: Set eye position (camera position) uniform
		glm::vec3 eyePosition = camera->position;
		int eye_position = glGetUniformLocation(shader->program, "eye_position");
		glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

		// TODO: Set material property uniforms (shininess, kd, ks, object color) 
		int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
		glUniform1i(material_shininess, materialShininess);

		int material_kd = glGetUniformLocation(shader->program, "material_kd");
		glUniform1f(material_kd, materialKd);

		int material_ks = glGetUniformLocation(shader->program, "material_ks");
		glUniform1f(material_ks, materialKs);
	}

	int spot_pos = glGetUniformLocation(shader->program, "spot");
	glUniform1i(spot_pos, spot);

	int cutoff_pos = glGetUniformLocation(shader->program, "cut_off");
	glUniform1f(cutoff_pos, RADIANS(cutoff));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (tex)
	{
		int loc_tex_bool = glGetUniformLocation(shader->program, "do_tex");
		glUniform1i(loc_tex_bool, 1);

		glActiveTexture(GL_TEXTURE0); //bind the heightmap
		glBindTexture(GL_TEXTURE_2D, tex->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "textura"), 0);
	}
	else
	{
		int loc_tex_bool = glGetUniformLocation(shader->program, "do_tex");
		glUniform1i(loc_tex_bool, 0);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int isSndTex;

	if (texture1)
	{
		//TODO : activate texture location 0
		//TODO : Bind the texture1 ID
		//TODO : Send texture uniform value
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());

		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}


	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}


//load the cubemap texture !!not my code, it's from a tutorial!!
unsigned int Tema3::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Tema3::RenderTerrain(Mesh* mesh,Shader* shader,Texture2D* heightmap,Texture2D* terrain_tex, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	shader->Use();
	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	//light uniforms
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	int loc_eyepos = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(loc_eyepos, 1, glm::value_ptr(camera->position));

	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	glm::vec3 lightPosition_sp1 = pl1->position + glm::vec3(0, 1, 0);
	glm::vec3  lightDirection_sp1 = glm::vec3(0,-1,0);

	int sp1_loc = glGetUniformLocation(shader->program, "light_position_sp1");
	glUniform3fv(sp1_loc, 1,glm::value_ptr(lightPosition_sp1));

	sp1_loc = glGetUniformLocation(shader->program, "light_direction_sp1");
	glUniform3fv(sp1_loc, 1, glm::value_ptr(lightDirection_sp1));

	lightPosition_sp1 = pl2->position + glm::vec3(0, 1, 0);
	lightDirection_sp1 = glm::vec3(0, -1, 0);

	sp1_loc = glGetUniformLocation(shader->program, "light_position_sp2");
	glUniform3fv(sp1_loc, 1, glm::value_ptr(lightPosition_sp1));

	sp1_loc = glGetUniformLocation(shader->program, "light_direction_sp2");
	glUniform3fv(sp1_loc, 1, glm::value_ptr(lightDirection_sp1));

	if (projectile)
	{
		sp1_loc = glGetUniformLocation(shader->program, "spot_proj");
		glUniform1i(sp1_loc, 1);
	}
	else
	{
		sp1_loc = glGetUniformLocation(shader->program, "spot_proj");
		glUniform1i(sp1_loc, 0);
	}

	lightPosition_sp1 = projectile_position + glm::vec3(0, 1, 0); 
	lightDirection_sp1 = projectile_fwd;// glm::vec3(0, -1, 0);

	sp1_loc = glGetUniformLocation(shader->program, "light_position_sp3");
	glUniform3fv(sp1_loc, 1, glm::value_ptr(lightPosition_sp1));

	sp1_loc = glGetUniformLocation(shader->program, "light_direction_sp3");
	glUniform3fv(sp1_loc, 1, glm::value_ptr(lightDirection_sp1));


	glActiveTexture(GL_TEXTURE0); //bind the heightmap
	glBindTexture(GL_TEXTURE_2D,heightmap->GetTextureID());
	glUniform1i(glGetUniformLocation(shader->program, "heightmap"), 0);

	glActiveTexture(GL_TEXTURE1); //bind the terrain texture
	glBindTexture(GL_TEXTURE_2D, terrain_tex->GetTextureID());
	glUniform1i(glGetUniformLocation(shader->program, "terrain_tex"), 1);


	int loc_terrain_resolution = glGetUniformLocation(shader->program, "heightmapsize");
	glUniform2f(loc_terrain_resolution,(float)terrainResolution.x,(float)terrainResolution.y);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderSkyBox(const glm::mat4& modelMatrix)
{
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);

	Mesh* mesh = meshes["box"];
	Shader* shader = shaders["SkyBox"];
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	shader->Use();

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glActiveTexture(GL_TEXTURE0); //bind the skybox texture

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

	glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
}

void Tema3::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(),projectionMatrix);
}

void Tema3::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;
	shader->Use();
	int loc_eyepos = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(loc_eyepos, 1, glm::value_ptr(camera->position));


	// render an object using the specified shader and the specified position

	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int loc_time = glGetUniformLocation(shader->program, "Time");
	glUniform1f(loc_time, RADIANS(globalTime ));

	mesh->Render();
}

//Render a mesh which has all the vertices of color "color"
void Tema3::RenderMeshColor(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->program)
		return;
	shader->Use();

	int loc_eyepos = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(loc_eyepos, 1, glm::value_ptr(camera->position));


	// render an object using the specified shader and the specified position

	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int loc_color = glGetUniformLocation(shader->program, "color");
	glUniform3fv(loc_color, 1, glm::value_ptr(color));

	mesh->Render(); 
}

bool Tema3::collision(glm::vec3 pos)
{
	if (pos.y <= (float)heightmap->getPointHeight(pos.x, pos.z)*20/255 )
	{
		std::cout << pos << ' ' << heightmap->getPointHeight(pos.x, pos.z) << '\n';
		return true;
	}
	return false;
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	globalTime += deltaTime;

	if (projectile)
	{
		camera->TranslateForward(projectile_speed*deltaTime);
		projectile_position += projectile_fwd * projectile_speed*deltaTime;
		if (projectile_position.x > heightmap->width / 2 || projectile_position.x < -heightmap->width / 2 ||
			projectile_position.z> heightmap->height / 2 || projectile_position.z < -heightmap->height / 2)
		{
			projectile = false;
			*camera = cam_proj;
			switch_players();
		}
		if (collision(projectile_position))
		{
			std::cout << "collision " << projectile_position <<"\n";
			projectile = false;
			*camera = cam_proj;
			switch_players();
			std::cout << projectile_position.x + (float)heightmap->width / 2<<' '<< projectile_position.z + (float)heightmap->height / 2 << '\n';
			int deform_value;
			if (weapon_type == ROCKET)
			{
				deform_value = -50;
			}
			else
			{
				deform_value = 30;
			}
			heightmap->terrainDeformation(projectile_position.z+(float)heightmap->width/2, projectile_position.x+ (float)heightmap->height / 2, 5, deform_value);
			meshes["terrain"] = Objects::updateTerrain(meshes["terrain"],resolution,heightmap);
			mapTextures["heightmap"]->UploadNewData(heightmap->terrain);
			
		}
	}


	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = camera->right;
		glm::vec3 forward = camera->forward;
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

		float speed = 2;

		// Control light position using on W, A, S, D, E, Q
		if (window->KeyHold(GLFW_KEY_W)) lightPosition -= forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_A)) lightPosition -= right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_S)) lightPosition += forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_D)) lightPosition += right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_E)) lightPosition += up * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_Q)) lightPosition -= up * deltaTime * speed;
	}

	
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 10.f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_L)) {
			Pfov++;
		}

		if (window->KeyHold(GLFW_KEY_K)) {
			Pfov--;
		}

		if (window->KeyHold(GLFW_KEY_M)) {
			latime += 0.01f;
			pl1->rotateOx(0.01);
		}

		if (window->KeyHold(GLFW_KEY_N)) {
			latime -= 0.01f;
			pl1->rotateOy(0.01);
		}

		if (window->KeyHold(GLFW_KEY_V)) {
			inaltime += 0.01f;
		}

		if (window->KeyHold(GLFW_KEY_B)) {
			inaltime -= 0.01f;
		}


	}

	if (window->KeyHold(GLFW_KEY_M)) {
		pl1->rotateOx(0.01);
	}

	if (window->KeyHold(GLFW_KEY_N)) {
		pl1->rotateOy(0.01);
	}

	/*if (cameraType == 't')
		projectionMatrix = glm::perspective(RADIANS(Pfov), window->props.aspectRatio, 0.01f, 200.0f);
	if (cameraType == 'f')
		projectionMatrix = glm::perspective(RADIANS(90), window->props.aspectRatio, 0.01f, 300.0f);*/
}

void Tema3::switch_players()
{
	activePlayer = 3 - activePlayer;
	if (activePlayer == 1)
	{
		camera = cam_pl1;
	}
	else
	{
		camera = cam_pl2;
	}
}


void Tema3::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_X)
	{
		heightmap->terrainDeformation(15, 129, 5, 120);
		mapTextures["heightmap"]->UploadNewData(heightmap->terrain);
	}
	if (key == GLFW_KEY_Z)
	{
		std::cout << "change weapon" << ' ' << weapon_type << '\n';
		weapon_type = weapon_type + 1;
		if (weapon_type > DIRT)
			weapon_type = ROCKET;
	}
	if (key == GLFW_KEY_SPACE)
	{
		switch_players();
	}

}

void Tema3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !projectile)
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(-(float)deltaY * sensivityOX);
			camera->RotateFirstPerson_OY(-(float)deltaX * sensivityOY);

		}

		if (window->GetSpecialKeyState() ==0) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OX(-(float)deltaY * sensivityOY);
			camera->RotateThirdPerson_OY(-(float)deltaX * sensivityOX);
			if (activePlayer == 1)
			{
				pl1->rotateOx((float)deltaY * sensivityOY);
				pl1->rotateOy(-(float)deltaX * sensivityOX);
			}
			else
			{
				pl2->rotateOx((float)deltaY * sensivityOY);
				pl2->rotateOy(-(float)deltaX * sensivityOX);
			}
		}

	}
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	clickedX = mouseX;
	clickedY = mouseY;
	if (button == GLFW_MOUSE_BUTTON_2 && !projectile)
	{
		std::cout << "Projectile launched\n";
		projectile = true;
		if (activePlayer == 1)
		{
			projectile_position = pl1->position;
			projectile_fwd = cam_pl1->forward;
		}
		else
		{
			projectile_position = pl2->position;
			projectile_fwd = cam_pl2->forward;
		}
		cam_proj = *camera;
	}
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
	clickedX = -1;
	clickedY = -1;
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
