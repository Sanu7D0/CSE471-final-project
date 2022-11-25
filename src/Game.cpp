#include "Game.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <format>

Game::Game(double firstX, double firstY, int width, int height)
	: controller(
		MouseControl(firstX, firstY),
		CameraControl(glm::vec3(0.0f, 0.0f, -5.0f), width, height),
		gViewMatrix,
		gProjectionMatrix),
	baseShader(Shader("resource/shader/Base.vert", "resource/shader/Base.frag")),
	textShader(Shader("resource/shader/Text.vert", "resource/shader/Text.frag")),
	textRenderer(static_cast<float>(width), static_cast<float>(height), "resource/font/DroidSansMono.ttf")
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
		Model("resource/model/gorilla.obj"));
	_gameObjects.push_back(_player);

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
		// Game options
		if (keys[GLFW_KEY_F3] && !keysProcessed[GLFW_KEY_F3])
		{
			keysProcessed[GLFW_KEY_F3] = true;
			Globals::debug = !Globals::debug;
		}

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

		if (keys[GLFW_KEY_V] && !keysProcessed[GLFW_KEY_V])
		{
			keysProcessed[GLFW_KEY_V] = true;
			switch (controller.cameraControl.mode)
			{
			case ECameraMode::FPS:
				controller.cameraControl.mode = ECameraMode::TPS;
				break;
			case ECameraMode::TPS:
				controller.cameraControl.mode = ECameraMode::FPS;
				break;
			}
		}
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

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); // Cull triangles which normal is not towards the camera

	baseShader.use();
	baseShader.setMat4("viewProj", gProjectionMatrix * gViewMatrix);
	gAxesShader->use();
	gAxesShader->setMat4("viewProj", gProjectionMatrix * gViewMatrix);
	for (const auto& go : _gameObjects)
	{
		go->draw(baseShader);
	}

	if (Globals::debug)
	{
		drawDebugInfo();
	}
}

void Game::collisions()
{
}

void Game::drawDebugInfo()
{
	//const auto width = static_cast<float>(controller.cameraControl.width);
	const auto height = static_cast<float>(controller.cameraControl.height);

	textRenderer.renderText(textShader, "Back To the CG",
		0.0f, height - 50.0f, 1.0f);
	textRenderer.renderText(textShader, std::format("{} fps", Globals::fps),
		0.0f, height - 100.0f, 1.0f);

	textRenderer.renderText(textShader, std::format("x: {:.2f}", _player->transform.position.x),
		0.0f, height - 200.0f, 1.0f);
	textRenderer.renderText(textShader, std::format("y: {:.2f}", _player->transform.position.y),
		0.0f, height - 250.0f, 1.0f);
	textRenderer.renderText(textShader, std::format("z: {:.2f}", _player->transform.position.z),
		0.0f, height - 300.0f, 1.0f);
}
