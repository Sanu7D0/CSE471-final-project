#pragma once
#include <glad/glad.h>

#include <memory>
#include <vector>

#include "ViewController.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "Terrain.hpp"
#include "rendering/Shader.hpp"
#include "rendering/TextRenderer.hpp"
#include "rendering/Light.hpp"
#include "rendering/Image.hpp"
#include "Globals.hpp"

inline glm::mat4 gViewMatrix;
inline glm::mat4 gProjectionMatrix;

enum EGameState
{
	GameIdle,
	GameActive,
	GameEnd
};

class Game
{
public:
	ViewController viewController;

	EGameState state = EGameState::GameIdle;
	bool keys[1024] = {};
	bool keysProcessed[1024] = {};
	bool mouseButtons[8] = {};

	Shader baseShader;
	Shader textShader;
	Shader screenShader;
	TextRenderer textRenderer;


	Game(double firstX, double firstY, int width, int height);
	~Game();

	void init();
	void reset();
	void processInput(const float dt);
	void update(const float dt);
	void render();

private:
	GLuint frameBuffer, textureColorBuffer, RBO;
	GLuint screenQuadVAO, screenQuadVBO;

	std::shared_ptr<Player> _player;
	std::vector<std::shared_ptr<GameObject>> _gameObjects;
	TerrainManager terrainManager = TerrainManager();
	//irrklang::ISoundEngine* soundEngine;
	double lastplayTime = 0.0;

	bool nightVision = false;

	void drawUI();
	void drawDebugInfo();
};
