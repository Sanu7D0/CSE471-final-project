#pragma once
#include <glad/glad.h>

#include <memory>
#include <vector>

#include "Controller.hpp"
#include "Player.hpp"
#include "rendering/Shader.hpp"

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

	Game(double firstX, double firstY, int width, int height);
	~Game();

	void init();
	void processInput(const float dt);
	void update(const float dt);
	void render();
	void collisions();

private:
	std::unique_ptr<Player> _player;
	std::vector<std::shared_ptr<GameObject>> _gameObjects;
	GLuint VBO, VAO;
};
