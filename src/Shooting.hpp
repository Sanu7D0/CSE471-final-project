#pragma once
#include <glm/glm.hpp>

#include "Transform.hpp"

enum EGunState
{
	Idle,
	Reloading
};

class Gun
{
public:
	Transform transform = Transform(glm::vec3(0.0f, 0.0f, 0.0f));
	glm::vec3 shootPointOffset = glm::vec3(0.0f, 0.0f, 0.0f);

	float damage = 10.0f;
	double shootingCooltime = 0.1; // in seconds
	uint32_t ammo;

	EGunState state = EGunState::Idle;

	Gun(uint32_t ammo);
	bool tryShoot();
	bool tryReload();

private:
	double lastShootTime = 0.0;

	void shoot();
	void asyncReload();
};