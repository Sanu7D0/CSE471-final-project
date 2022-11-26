#include "Shooting.hpp"

#include <ctime>
#include <iostream>
#include <chrono>
#include <future>
#include <thread>

//#include "physics/Collider.hpp"
//#include "physics/"

Gun::Gun(uint32_t ammo) : ammo(ammo)
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

	lastShootTime = static_cast<double>(std::clock());
	ammo -= 1;
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
	ammo = 10;
	state = EGunState::Idle;

	return true;
}

void Gun::asyncReload()
{

	ammo = 10;
}