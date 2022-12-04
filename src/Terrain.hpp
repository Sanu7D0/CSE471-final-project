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
	std::list<std::shared_ptr<Terrain>> terrains; // no collision

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
	std::list<std::shared_ptr<Terrain>> terrains;

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

	const std::list<std::shared_ptr<Terrain>>& getContainer()
	{
		return terrains;
	}

	void addTerrain(const std::shared_ptr<Terrain>& terrain)
	{
		terrains.push_back(terrain);
	}

	void removeTerrain(const std::shared_ptr<Terrain>& terrain)
	{
		terrains.remove(terrain);
	}
};