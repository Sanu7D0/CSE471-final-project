#include "Shooting.hpp"

#include <ctime>
#include <iostream>
#include <future>
#include <thread>
#include <utility>
#include <iostream>
#include <algorithm>

#include "Enemy.hpp"
#include "physics/Collision.hpp"

Gun::Gun(const std::shared_ptr<GameObject>& parent, Transform transform, Model model)
	: GameObject(parent, transform, std::move(model))
{
	float laserVertices[6] =
	{
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 5.0f
	};

	glGenVertexArrays(1, &laserVAO);
	glGenBuffers(1, &laserVBO);
	glBindVertexArray(laserVAO);
	glBindBuffer(GL_ARRAY_BUFFER, laserVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(laserVertices), &laserVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}

void Gun::draw(const Shader& shader) const
{
	GameObject::draw(shader);

	if (Globals::debug)
	{
		// Draw laser pointer
		gAxesShader->use();
		gAxesShader->setMat4("model", transform.getModelMatrix());
		gAxesShader->setVec3("color", glm::vec3(1.0f, 0.0f, 1.0f)); // purple

		glBindVertexArray(laserVAO);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
	}
}

bool Gun::tryShoot()
{
	if (ammo <= 0 || state != EGunState::Idle) 
		return false;

	const auto currentTime = static_cast<double>(std::clock());
	if ((currentTime - lastShootTime) / CLOCKS_PER_SEC < shootingCooltime)
		return false;

	shoot();
	return true;
}

void Gun::shoot()
{
	const auto ray = Ray(transform.position, transform.forward());

	// Find all hit enemy with ray
	std::vector<Enemy*> hitEnemies;
	for (const auto enemy : EnemyContainer::Instance()->getContainer())
	{
		if (intersect(enemy->collider, ray))
		{
			hitEnemies.push_back(enemy);
		}
	}

	if (hitEnemies.size() > 0)
	{
		// Find nearest hit enemy
		std::ranges::sort(hitEnemies,
          [&transform = transform](const Enemy* a, const Enemy* b) -> bool
          {
              return distance(transform.position, a->transform.position) < distance(
                  transform.position, b->transform.position);
          });

		hitEnemies[0]->takeDamage(damage);
	}


	// Effect
	std::thread muzzleFlashEffect([&muzzleFlash = muzzleFlash]()
	{
		// enable muzzle light
		muzzleFlash->bEnabled = true;

		// disable muzzle light
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		muzzleFlash->bEnabled = false;

	});
	muzzleFlashEffect.detach();

	lastShootTime = static_cast<double>(std::clock());
	ammo -= 1;

	Globals::SoundEngine->play2D("resource/audio/shotgun_shot_01.wav", false);
}


bool Gun::tryReload()
{
	if (state != EGunState::Idle)
		return false;

	state = EGunState::Reloading;
	std::thread reload([&ammo = ammo, &state = state]()
	{
		Globals:: SoundEngine->play2D("resource/audio/shotgun_load_bullet_01.wav");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		Globals::SoundEngine->play2D("resource/audio/shotgun_load_bullet_02.wav");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		Globals::SoundEngine->play2D("resource/audio/shotgun_cock_01.wav");
		ammo = 2;
		state = Idle;
	});
	reload.detach(); // Not to block when get out of scope

	return true;
}