#pragma once
#include "GameObject.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shooting.hpp"

class Player : public GameObject
{
public:
	Gun gun = Gun(10);

	Player(Transform t, Model model_);
	void move(const float dt) override;

private:
	float _speed = 3.0f;
};
