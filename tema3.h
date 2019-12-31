#pragma once
#include <Component/SimpleScene.h>
#include "camera.h"
#include <map>
#include <Core/Engine.h>
#include <stb/stb_image.h>
#include <time.h>

#include "Terrain.h"
#include "worm.h"

enum Weapons
{
	ROCKET,
	DIRT
};

class Tema3 : public SimpleScene
{

public:
	Tema3();
	~Tema3();
	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
	void RenderMeshColor(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
	void RenderSkyBox(const glm::mat4& modelMatrix);
	void RenderTerrain(Mesh* mesh,Shader* shader,Texture2D* texture,Texture2D* terrain_tex,const glm::mat4& modelMatrix);
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1);
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,const glm::vec3 player_pos,Texture2D* tex=NULL);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	
	void loadMeshes();
	void loadTextures();
	void loadShaders();

	void switch_players();
	bool collision(glm::vec3 obj_position);

	unsigned int loadCubemap(std::vector< std::string > faces);

protected:
	Camera* camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;

	float Pfov;
	float latime, inaltime;
	char cameraType;

	int clickedX, clickedY;
	float globalTime;

	glm::vec3 camPosT;
	glm::vec3 camLookAtT;

	Camera* cam_pl1,*cam_pl2,cam_proj;
	
	unsigned int cubeMapTexture;
	unsigned int heightmapTexture;

	glm::vec3 lightPosition;
	glm::vec3 lightDirection;
	unsigned int materialShininess;
	float materialKd;
	float materialKs;

	float cutoff;
	int spot;

	//----------------

	float resolution = 2.f;

	std::unordered_map<std::string, Texture2D*> mapTextures;

	glm::mat4 skyMM, terrainMM;

	glm::vec2 terrainResolution;

	Terrain* heightmap;
	
	Worm *pl1, *pl2;
	int activePlayer;

	bool projectile;
	glm::vec3 projectile_position;
	glm::vec3 projectile_fwd;

	float projectile_speed;

	int weapon_type;
};