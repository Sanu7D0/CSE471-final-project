#pragma once
#include <glm/glm.hpp>
#include <irrklang/irrKlang.h>

namespace Globals
{
	inline bool debug = true;
	inline double fps = 0.0;
	inline glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);
	inline bool spawnEnemy = true;
	inline irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
}