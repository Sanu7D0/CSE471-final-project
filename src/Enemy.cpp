#include "Enemy.hpp"

#include <glm/gtx/string_cast.hpp>

EnemyContainer* EnemyContainer::_instance = nullptr;

Enemy::Enemy(Transform transform, Model model)
	: GameObject(nullptr, transform, std::move(model)),
	RigidBody(
		this->transform, 
		BoxCollider(glm::vec3(2.5f, 2.5f, 2.5f), transform.position))
{
	speed = 1.0f;
}

void Enemy::update(const float dt)
{
	collider.center = transform.position;

	// Player chasing
	const glm::vec3 chaseDir = normalize(targetPosition - transform.position);

	const auto forward = glm::vec3(0.0f, 0.0f, 1.0f);
	const float theta = glm::acos(glm::dot(chaseDir, forward) / (glm::length(chaseDir) * glm::length(forward)));
	//transform.rotation = glm::quat(glm::vec3(0.0f, theta, 0.0f)) * forward;
	transform.rotation = glm::quat(chaseDir, glm::vec3(0.0f, 0.0f, 1.0f));

	transform.position += chaseDir * speed * dt;

	//RigidBody::update(dt);
}

void Enemy::draw(const Shader& shader) const
{
	GameObject::draw(shader);

	if (Globals::debug)
		collider.draw(gAxesShader, transform.getModelMatrix());
}

void Enemy::takeDamage(float damage)
{
	hp -= damage;
	if (hp <= 0.0f)
	{
		EnemyContainer::Instance()->removeEnemy(this);
	}
}
