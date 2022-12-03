#include "Shooting.hpp"

#include <ctime>
#include <iostream>
#include <future>
#include <thread>
#include <utility>
#include <iostream>
#include <algorithm>
#include <irrklang/irrKlang.h>
#include "Enemy.hpp"
#include "physics/Collision.hpp"
using namespace irrklang;

ISoundEngine* SoundEngine = createIrrKlangDevice();

Gun::Gun(const std::shared_ptr<GameObject>& parent, Transform transform, Model model)
	: GameObject(parent, transform, std::move(model))
{
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
	//BoxCollider& hitCollider = raycast();
	//hitCollider.gameObject->

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

	SoundEngine->play2D("resource/audio/shotgun_shot_01.wav", false);
}


bool Gun::tryReload()
{
	if (state != EGunState::Idle)
		return false;

	state = EGunState::Reloading;
	std::thread reload([&ammo = ammo, &state = state]()
	{
		// TODO: mutex?
		std::this_thread::sleep_for(std::chrono::milliseconds(700));
		ammo = 2;
		state = Idle;
	});
	reload.detach(); // Not to block when get out of scope

	return true;
}