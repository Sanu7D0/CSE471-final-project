#include "Shooting.hpp"

#include <ctime>
#include <iostream>
#include <future>
#include <thread>

#include "physics/Collision.hpp"

Gun::Gun()
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

	//const auto ray = Ray(transform.position, transform.forward());

	//for (const auto& go : gameOjbects)
	//{
	//	if (intersect(go->collider, ray))
	//	{
	//		// do something
	//	}
	//}

	lastShootTime = static_cast<double>(std::clock());
	ammo -= 1;

	//soundEngine->play2D(shotClips[rand() % 2].c_str());
}


bool Gun::tryReload()
{
	if (state != EGunState::Idle)
		return false;

	state = EGunState::Reloading;
	//std::thread r([&ammo = ammo, &state = state]()
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(1)); // reload for 1 seconds

	//	ammo = 10;
	//	state = EGunState::Idle;
	//});

	// thread is still synchronous...
	asyncReload();
	state = EGunState::Idle;

	return true;
}

void Gun::asyncReload()
{
	//for (int i = 0; i < magazineCapacity; ++i)
	//{
	//	soundEngine->play2D(reloadClips[rand() % 2].c_str());
	//	// wait
	//}
	//soundEngine->play2D(cockClip.c_str());

	ammo = magazineCapacity;
}