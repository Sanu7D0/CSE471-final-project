#include "Game.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <format>
#include <ctime>
#include <cstdlib>

#include "Enemy.hpp"



class EnemySpawner
{
	double lastSpawnTime = 0.0;
	double spawnDuration = 5.0;

public:
	void SpawnEnemy()
	{
		if (!Globals::spawnEnemy)
			return;

		const auto currentTime = static_cast<double>(std::clock());
		if ((currentTime - lastSpawnTime) / CLOCKS_PER_SEC < spawnDuration)
			return;

		lastSpawnTime = currentTime;
		auto* enemy = new Enemy(
			Transform(
				glm::vec3(rand() % 20, 0.0f, rand() % 20),
				glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
				glm::vec3(0.7f, 0.7f, 0.7f)),
			Model("resource/model/enemy/thomas2.obj"));
		EnemyContainer::Instance()->addEnemy(enemy);
	}
};

EnemySpawner enemySpawner = EnemySpawner();

Game::Game(double firstX, double firstY, int width, int height)
	: viewController(
		  MouseControl(firstX, firstY),
		  CameraControl(glm::vec3(0.0f, 0.0f, -5.0f), width, height),
		  gViewMatrix,
		  gProjectionMatrix),
	baseShader(Shader("resource/shader/Base.vert", "resource/shader/Base.frag")),
	textShader(Shader("resource/shader/Text.vert", "resource/shader/Text.frag")),
	textRenderer(static_cast<float>(width), static_cast<float>(height), "resource/font/DroidSansMono.ttf")
{	
	InitAxesShader();
	//soundEngine = irrklang::createIrrKlangDevice();
}

Game::~Game()
{
	DeleteAxesShader();
	//delete soundEngine;
}

void Game::init()
{
	_player = std::make_shared<Player>(
		Transform(glm::vec3(0.0f, 0.0f, -5.0f)),
		Model("resource/model/magma_block.obj"));
	_gameObjects.push_back(_player);

	_player->flashLight = LightManager::Instance()->addSpotLight(
		SpotLight(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.7f, 0.7f, 0.7f),
			_player->transform.position,
			glm::vec3(0.0f, 0.0f, 1.0f),
			1.0f, 0.09f, 0.032f,
			glm::cos(glm::radians(20.0f)),
			glm::cos(glm::radians(30.0f))),
		baseShader
	);
	_player->gun.muzzleFlash = LightManager::Instance()->addPointLight(
		PointLight(
			glm::vec3(0.3f, 0.3f, 0.3f),
			glm::vec3(0.6f, 0.6f, 0.3f),
			glm::vec3(0.3f, 0.3f, 0.3f),
			_player->gun.transform.position,
			1.0f, 0.09f, 0.032f
		),
		baseShader
	);
	_player->gun.muzzleFlash->enabled = false;

	terrainManager.init();

	LightManager::Instance()->setDirLight(DirLight(
		glm::vec3(0.01f, 0.01f, 0.01f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-0.2f, -1.0f, -0.3f))
		, baseShader);
	float soundcontrol = 0;
	Globals::SoundEngine->play2D("resource/audio/bgm.mp3", true);
}

void Game::reset()
{
	auto& enemies = EnemyContainer::Instance()->getContainer();
	auto iter = enemies.begin();
	while (iter != enemies.end())
	{
		delete (*iter);
		iter = enemies.erase(iter);
	}

	_player->hp = 100.0f;
	_player->gun.ammo = _player->gun.magazineCapacity;
	_player->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
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
	case GameEnd:
		if (keys[GLFW_KEY_ENTER] && !keysProcessed[GLFW_KEY_ENTER])
		{
			reset();
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
		{
			float horizontalInput = 0.0f;
			float forwardInput = 0.0f;
			float yinput = 0.0f;
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

			if (horizontalInput != 0.0f || forwardInput != 0.0f)
			{
				_player->rigidBody.velocity = normalize(glm::vec3(horizontalInput, 0.0f, forwardInput));
				const auto currentTime = static_cast<double>(std::clock());
				if ((currentTime - lastplayTime) / CLOCKS_PER_SEC > 1.0f)
				{
					Globals::SoundEngine->play2D("resource/audio/footsteps.mp3", false);
					lastplayTime = std::clock();
				}

			}
			else
			{
				_player->rigidBody.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}

	// FPS / TPS toggle
		if (keys[GLFW_KEY_V] && !keysProcessed[GLFW_KEY_V])
		{
			keysProcessed[GLFW_KEY_V] = true;
			switch (viewController.cameraControl.mode)
			{
			case FPS:
				viewController.cameraControl.mode = TPS;
				break;
			case TPS:
				viewController.cameraControl.mode = FPS;
				break;
			}
		}

	// Gun shooting
		if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT])
		{
			_player->gun.tryShoot();
		}
		if (keys[GLFW_KEY_R])
		{
			_player->gun.tryReload();
		}

		// Others
		if (keys[GLFW_KEY_F] && !keysProcessed[GLFW_KEY_F])
		{
			keysProcessed[GLFW_KEY_F] = true;
			if (const auto flashLight = _player->flashLight.lock(); flashLight != nullptr)
			{
				flashLight->enabled = !flashLight->enabled;
			}
		}

		if (keys[GLFW_KEY_F4] && !keysProcessed[GLFW_KEY_F4])
		{
			keysProcessed[GLFW_KEY_F4] = true;
			Globals::spawnEnemy = !Globals::spawnEnemy;
		}
	}
}

void Game::update(const float dt)
{
	if (state == EGameState::GameIdle || state == EGameState::GameEnd)
		return;

	_player->update(dt);

	viewController.cameraControl.followingTarget = _player->transform.position;
	viewController.update(dt);

	// Rotate player body yaw along camera direction
	_player->transform.rotation =
		glm::quat(glm::vec3(0.0, glm::radians(viewController.cameraControl.yaw), 0.0f));
	// Rotate flash light along camera direction
	if (const auto flashLight = _player->flashLight.lock())
	{
		flashLight->direction =
			glm::quat(glm::vec3(
				glm::radians(viewController.cameraControl.pitch),
				glm::radians(viewController.cameraControl.yaw),
				0.0f))
			* glm::vec3(0.0f, 0.0f, 1.0f);
	}

	// Enemies
	enemySpawner.SpawnEnemy();
	for (const auto enemy : EnemyContainer::Instance()->getContainer())
	{
		enemy->targetPosition = _player->transform.position;
		enemy->update(dt);

		// Collision check for enemy attack
		if (aabb(_player->rigidBody.collider, enemy->collider))
		{
			_player->hp -= enemy->dealDamage();
		}
	}

	LightManager::Instance()->update(baseShader);

	if (_player->hp <= 0.0f)
	{
		state = GameEnd;
	}
}

void Game::render()
{
	if (state == EGameState::GameActive)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE); // Cull triangles which normal is not towards the camera

		baseShader.use();
		baseShader.setMat4("view", gViewMatrix);
		baseShader.setMat4("projection", gProjectionMatrix);
		baseShader.setVec3("viewPos", viewController.cameraControl.eye);

		gAxesShader->use();
		gAxesShader->setMat4("view", gViewMatrix);
		gAxesShader->setMat4("projection", gProjectionMatrix);

		for (const auto& go : _gameObjects)
			go->draw(baseShader);

		for (const auto enemy : EnemyContainer::Instance()->getContainer())
			enemy->draw(baseShader);
		
		terrainManager.draw(baseShader);

		drawUI();
		if (Globals::debug)
		{
			drawDebugInfo();
		}
	}
	else if (state == EGameState::GameIdle)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		textRenderer.renderText(textShader, "Press ENTER to start...",
			viewController.cameraControl.width / 3, viewController.cameraControl.height / 2, 2.0f);
	}
	else if (state == EGameState::GameEnd)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawUI();
		textRenderer.renderText(textShader, "Press ENTER to restart...",
			viewController.cameraControl.width / 3, viewController.cameraControl.height / 2, 2.0f);
	}
}

void Game::drawUI()
{
	const auto width = static_cast<float>(viewController.cameraControl.width);
	//const auto height = static_cast<float>(controller.cameraControl.height);

	textRenderer.renderText(textShader, std::format("Ammo: {}", _player->gun.ammo),
	                        width - 400.0f, 50.0f, 1.5f);
	textRenderer.renderText(textShader, std::format("HP: {:.0f}", _player->hp),
		50.0f, 50.0f, 1.5f);
}

void Game::drawDebugInfo()
{
	//const auto width = static_cast<float>(controller.cameraControl.width);
	const auto height = static_cast<float>(viewController.cameraControl.height);

	textRenderer.renderText(textShader, "TOOM Eternal",
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
