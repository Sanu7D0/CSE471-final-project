#include "Enemy.hpp"

EnemyContainer* EnemyContainer::_instance = nullptr;

void Enemy::update(const float dt)
{
	glm::vec3 chaseDir = targetPosition - transform.position;
	chaseDir = glm::normalize(chaseDir);
	theta = glm::acos(glm::dot(chaseDir, (0.0f, 0.0f, 1.0f))/(glm::length(chaseDir)*glm::length(0.0f,0.0f,1.0f)))
	transform.rotation = glm::quat(glm::vec3(0.0f,glm::radians(theta), 0,0f)*glm::vec3(0.0f, 0.0f, 1.0f))

	RigidBody::update(dt);
}

void Enemy::takeDamage(float damage)
{
	hp -= damage;
	if (hp <= 0.0f)
	{
		EnemyContainer::Instance()->removeEnemy(this);
	}
}
