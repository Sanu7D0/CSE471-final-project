#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include <vector>
#include <iostream>
#include <memory>
#include <list>

#include "GameObject.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Model.hpp"
#include "physics/Collider.hpp"

class Terrain : public GameObject
{
public:
	BoxCollider collider;

	Terrain(Transform transform, Model model, BoxCollider collider);

	void draw(const Shader& shader) const override;
};

class TerrainManager
{
public:
	//Shader skyboxShader;
	std::vector<std::shared_ptr<Terrain>> terrains;

	TerrainManager();

	void init();
	void draw(const Shader& shader) const;

private:
	//GLuint skyboxID, skyboxVAO, skyboxVBO;

	void loadTerrain();
	//void loadCubemap();
	//void drawCubemap() const;
};

class TerrainContainer
{
private:
	static TerrainContainer* _instance;
	std::list<Terrain*> enemies;

	TerrainContainer() {};
	~TerrainContainer() {};
	TerrainContainer(TerrainContainer const&) = delete;
	TerrainContainer& operator=(TerrainContainer const&) = delete;

public:
	static TerrainContainer* Instance()
	{
		if (_instance == nullptr) _instance = new TerrainContainer();
		return _instance;
	}

	const std::list<Terrain*>& getContainer()
	{
		return enemies;
	}

	void addEnemy(Terrain* enemy)
	{
		enemies.push_back(enemy);
	}

	void removeEnemy(Terrain* enemy)
	{
		enemies.remove(enemy);
		delete enemy;
	}
};