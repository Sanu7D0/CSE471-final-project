#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include <vector>
#include <iostream>
#include <memory>

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