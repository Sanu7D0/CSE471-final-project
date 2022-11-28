#pragma once
#include <glm/glm.hpp>

class BoxCollider
{
public:
	glm::vec3 min;
	glm::vec3 max;

	BoxCollider(glm::vec3 size)
	{
		min = -size / 2.0f;
		max = size / 2.0f;
	}
};