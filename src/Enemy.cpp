#include "Enemy.hpp"

#include <glm/gtx/string_cast.hpp>

EnemyContainer* EnemyContainer::_instance = nullptr;

Enemy::Enemy(Transform transform, Model model)
	: GameObject(nullptr, transform, std::move(model)),
	RigidBody(this->transform, BoxCollider(glm::vec3(1.0f, 1.0f, 1.0f)))
{
	speed = 0.1f;
	velocity = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Enemy::update(const float dt)
{
	const glm::vec3 chaseDir = normalize(targetPosition - transform.position);

	const auto forward = glm::vec3(0.0f, 0.0f, 1.0f);
	const float theta = glm::acos(glm::dot(chaseDir, forward) / (glm::length(chaseDir) * glm::length(forward)));
	transform.rotation = glm::quat(glm::vec3(0.0f, theta, 0.0f)) * forward;

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
