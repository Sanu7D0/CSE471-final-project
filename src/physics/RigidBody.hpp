#pragma once
#include <glm/glm.hpp>

#include "../Transform.hpp"

class RigidBody
{
public:
	float speed = 1.0f;
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	explicit RigidBody(Transform& transform)
		: _transform(transform)
	{}
	
	void move(const float dt)
	{
		const auto forward = _transform.forward();
		const auto right
			= cross(glm::vec3(0.0f, -1.0f, 0.0f), forward); // right hand rule
		const auto up = cross(forward, right);

		_transform.position += dt * speed * (right * velocity.x + up * velocity.y + forward * velocity.z);
	}

private:
	Transform& _transform;
};