#pragma once
#include <glm/glm.hpp>

#include <optional>

#include "../Transform.hpp"
#include "Collider.hpp"

class RigidBody
{
public:
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	float speed = 1.0f;
	std::optional<BoxCollider> collider;

	RigidBody(Transform& transform, const std::optional<BoxCollider> collider)
		: collider(collider), _transform(transform)
	{}

	void update(const float dt)
	{
		move(dt);
	}

private:
	Transform& _transform;

	void move(const float dt)
	{
		const auto forward = _transform.forward();
		const auto right
			= cross(glm::vec3(0.0f, -1.0f, 0.0f), forward); // right hand rule
		const auto up = cross(forward, right);

		_transform.position += dt * speed * (right * velocity.x + up * velocity.y + forward * velocity.z);
	}
};