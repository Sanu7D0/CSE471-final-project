#pragma once
#include <glad/glad.h>
//#include <AL/al.h>

#include <memory>
#include <vector>

#include "ViewController.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Text.hpp"
#include "rendering/Light.hpp"
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
	ViewController viewController;

	EGameState state = GameActive;
	bool keys[1024] = {};
	bool keysProcessed[1024] = {};
	bool mouseButtons[8] = {};

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
	LightManager lightManager = LightManager();
	//irrklang::ISoundEngine* soundEngine;

	void drawUI();
	void drawDebugInfo();
};
