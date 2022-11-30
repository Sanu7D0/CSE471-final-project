#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include <vector>
#include <iostream>

#include "GameObject.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Model.hpp"

class Terrain : public GameObject
{
	
};

class TerrainManager
{
public:
	Shader skyboxShader;

	TerrainManager();

	void draw(const Shader& shader) const;

private:
	GLuint skyboxID, skyboxVAO, skyboxVBO;

	void loadTerrain();
	void loadCubemap();
	void drawCubemap() const;
};