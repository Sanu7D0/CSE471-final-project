#pragma once

#include "GameObject.hpp"
#include "physics/RigidBody.hpp"
#include "physics/Collider.hpp"

class Enemy : public GameObject, public RigidBody, public BoxCollider
{
public:
	glm::vec3 targetPosition;
	void update(const float dt) override;
};