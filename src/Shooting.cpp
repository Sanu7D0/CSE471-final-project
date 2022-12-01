#include "Shooting.hpp"

#include <ctime>
#include <iostream>
#include <future>
#include <thread>
#include <iostream>

#include "physics/Collision.hpp"

Gun::Gun()
{
	//muzzleFlash = LightManager.
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

	//const auto ray = Ray(transform.position, transform.forward());

	//for (const auto& go : gameOjbects)
	//{
	//	if (intersect(go->collider, ray))
	//	{
	//		// do something
	//	}
	//}

	// Effect
	std::thread muzzleFlash([]()
	{
		// enable muzzle light

		// disable muzzle light
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	});
	muzzleFlash.detach();

	lastShootTime = static_cast<double>(std::clock());
	ammo -= 1;

	//soundEngine->play2D(shotClips[rand() % 2].c_str());
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