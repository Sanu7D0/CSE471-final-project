#pragma once
#include "GameObject.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shooting.hpp"
#include "rendering/Light.hpp"

class Player : public GameObject
{
public:
	Gun gun = Gun(10);
	std::weak_ptr<SpotLight> flashLight;

	Player(Transform t, Model model_);
	void update(const float dt);

private:
	float _speed = 3.0f;

	void move(const float dt) override;
};
