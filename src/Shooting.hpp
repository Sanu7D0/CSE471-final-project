#pragma once
#include <glm/glm.hpp>

#include <atomic>

#include "GameObject.hpp"
#include "rendering/Model.hpp"

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
	double shootingCooltime = 0.2; // in seconds
	uint32_t ammo = 2;
	uint32_t magazineCapacity = 2;

	//std::atomic<EGunState> state = EGunState::Idle;
	EGunState state = EGunState::Idle;

	std::string shotClips[2] = { "resource/audio/shotgun_shot_01.wav", "resource/audio/shotgun_shot_01.wav" };
	std::string reloadClips[2] = { "resource/audio/shotgun_load_bullet_01.wav", "resource/audio/shotgun_load_bullet_02.wav" };
	std::string cockClip = "resource/audio/shotgun_cock_02.wav";

	//Gun(irrklang::ISoundEngine* soundEngine);
	Gun();
	bool tryShoot();
	bool tryReload();

private:
	double lastShootTime = 0.0;

	void shoot();
};