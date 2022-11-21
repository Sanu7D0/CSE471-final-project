#include "Game.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>
#include <string>

Game::Game(double firstX, double firstY, int width, int height)
	: controller(
		MouseControl(firstX, firstY),
		CameraControl(glm::vec3(0.0f, 0.0f, -5.0f), width, height),
		gViewMatrix,
		gProjectionMatrix),
	baseShader(Shader("resource/shader/Base.vert", "resource/shader/Base.frag"))
{
	InitAxesShader();
}

Game::~Game()
{
	DeleteAxesShader();
}

void Game::init()
{
	_player = std::make_shared<Player>(
		Transform(glm::vec3(0.0f, 0.0f, -5.0f)), 
		Model("resource/model/magma_block.obj"));

	controller.init(_player);

	/*_gameObjects.push_back(std::make_shared<GameObject>(
		nullptr, 
		Transform(glm::vec3(0.0f, 0.0f, 0.0f)),
		Model("resource/model/gorilla.obj")));*/
	_gameObjects.push_back(std::make_shared<GameObject>(
		nullptr,
		Transform(glm::vec3(5.0f, 3.0f, -5.0f)),
		Model("resource/model/magma_block.obj")));
	_gameObjects.push_back(std::make_shared<GameObject>(
		nullptr,
		Transform(glm::vec3(-5.0f, 1.0f, 0.0f)),
		Model("resource/model/magma_block.obj")));
}

void Game::processInput(const float dt)
{
	switch (state)
	{
	case GameIdle:
		if (keys[GLFW_KEY_ENTER] && !keysProcessed[GLFW_KEY_ENTER])
		{
			state = GameActive;
			keysProcessed[GLFW_KEY_ENTER] = true;
		}
		break;
	case GameActive:
		// Player movement
		float horizontalInput = 0.0f;
		float forwardInput = 0.0f;
		if (keys[GLFW_KEY_W])
		{
			// move forward
			forwardInput += 1.0f;
		}
		if (keys[GLFW_KEY_A])
		{
			// move left
			horizontalInput -= 1.0f;
		}
		if (keys[GLFW_KEY_S])
		{
			// move backward
			forwardInput -= 1.0f;
		}
		if (keys[GLFW_KEY_D])
		{
			// move right
			horizontalInput += 1.0f;
		}

		_player->velocity = ((horizontalInput != 0.0f || forwardInput != 0.0f)
			                     ? normalize(glm::vec3(horizontalInput, 0.0f, forwardInput))
			                     : glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void Game::update(const float dt)
{
	_player->move(dt);

	controller.update(dt);

	collisions();
}

void Game::render()
{
	//if (state == EGameState::GameActive)

	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); // Cull triangles which normal is not towards the camera

	//baseShader.use();
	baseShader.setMat4("viewProj", gProjectionMatrix * gViewMatrix);
	gAxesShader->setMat4("viewProj", gProjectionMatrix * gViewMatrix);

	_player->draw(baseShader);
	for (const auto& go : _gameObjects)
	{
		go->draw(baseShader);
	}
}

void Game::collisions()
{
}
