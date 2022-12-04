#pragma once
#include <glm/glm.hpp>

#include <optional>

#include "../Transform.hpp"
#include "Collider.hpp"
#include "Collision.hpp"
#include "Terrain.hpp"

class RigidBody
{
public:
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	float speed = 1.0f;
	BoxCollider collider;

	RigidBody(Transform& transform, BoxCollider collider)
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
		//BoxCollider c = collider;
		
		//BoxCollider a = BoxCollider(glm::vec3(5.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		const auto temp = _transform.position;

		_transform.position += 5*dt * speed * (right * velocity.x + up * velocity.y + forward * velocity.z);
		collider.center = _transform.position;

		for (const auto& wall : TerrainContainer::Instance()->getContainer())
		{
			if (aabb(collider, (wall->collider))) {
				_transform.position = temp;
				collider.center = temp;
			}
		}
	}
};