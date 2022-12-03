#include "Enemy.hpp"

void Enemy::update(const float dt)
{
	glm::vec3 chaseDir = targetPosition - transform.position;
	chaseDir = glm::normalize(chaseDir);
	//transform.rotation = 

	RigidBody::update(dt);
}
