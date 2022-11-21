#pragma once
#include "GameObject.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

class Player : public GameObject
{
public:
	//glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);

	explicit Player(Transform t, Model model_);
	void move(const float dt) override;

private:
	float _speed = 3.0f;
};
