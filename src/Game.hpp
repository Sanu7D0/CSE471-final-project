#pragma once
#include <glad/glad.h>

#include <memory>
#include <vector>

#include "Controller.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Text.hpp"
#include "Globals.hpp"

inline glm::mat4 gViewMatrix;
inline glm::mat4 gProjectionMatrix;

enum EGameState
{
	GameIdle,
	GameActive,
};

class Game
{
public:
	Controller controller;

	EGameState state = GameActive;
	bool keys[1024] = {};
	bool keysProcessed[1024] = {};

	Shader baseShader;
	Shader textShader;
	Text textRenderer;

	Game(double firstX, double firstY, int width, int height);
	~Game();

	void init();
	void processInput(const float dt);
	void update(const float dt);
	void render();

private:
	std::shared_ptr<Player> _player;
	std::vector<std::shared_ptr<GameObject>> _gameObjects;

	void collisions();
	void drawDebugInfo();
};
