#pragma once
#include "GameObject.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shooting.hpp"
#include "physics/RigidBody.hpp"
#include "rendering/Light.hpp"

class Player : public GameObject, public RigidBody
{
public:
	Gun gun;
	std::weak_ptr<SpotLight> flashLight;

	Player(Transform t, Model model_, Gun gun);
	void draw(const Shader& shader) const override;
	void update(const float dt);
};
