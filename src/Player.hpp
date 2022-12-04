#pragma once
#include <glm/glm.hpp>

#include "GameObject.hpp"
#include "Shooting.hpp"
#include "physics/RigidBody.hpp"
#include "rendering/Light.hpp"

class Player : public GameObject, public RigidBody
{
public:
	Gun gun;
	std::weak_ptr<SpotLight> flashLight;
	BoxCollider collider = BoxCollider(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Player(Transform t, Model model);
	void draw(const Shader& shader) const override;
	void update(const float dt) override;
};
