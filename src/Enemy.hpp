#pragma once

#include "GameObject.hpp"
#include "physics/RigidBody.hpp"
#include "physics/Collider.hpp"

class Enemy : public GameObject, public RigidBody, public BoxCollider
{
public:
	float hp = 10.0f;
	glm::vec3 targetPosition;
	void update(const float dt) override;
};