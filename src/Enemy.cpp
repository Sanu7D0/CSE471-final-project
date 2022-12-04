#include "Enemy.hpp"

#include <glm/gtx/string_cast.hpp>

EnemyContainer* EnemyContainer::_instance = nullptr;

Enemy::Enemy(Transform transform, Model model)
	: GameObject(nullptr, transform, std::move(model)),
	rigidBody(transform, BoxCollider(glm::vec3(2.5f, 2.5f, 2.5f), transform.position))
{
	rigidBody.speed = 2.5f;
}

void Enemy::update(const float dt)
{
	collider.center = transform.position;

	// Player chasing
	// direction to rotation
	// https://stackoverflow.com/questions/18558910/direction-vector-to-rotation-matrix
	const auto forward = normalize(targetPosition - transform.position);
	const auto right = cross(forward, glm::vec3(0.0f, -1.0f, 0.0f));
	const auto up = cross(forward, right);
	auto mat = glm::mat3(1.0f);
	mat[0] = right;
	mat[1] = up;
	mat[2] = forward;

	transform.position += forward * rigidBody.speed * dt;
	transform.rotation = glm::quat_cast(mat);

	//RigidBody::update(dt);
}

void Enemy::draw(const Shader& shader) const
{
	GameObject::draw(shader);

	if (Globals::debug)
		collider.draw(gAxesShader, translate(glm::mat4(1.0f), transform.position));
}

void Enemy::takeDamage(float damage)
{
	hp -= damage;
	if (hp <= 0.0f)
	{
		EnemyContainer::Instance()->removeEnemy(this);
	}
}
