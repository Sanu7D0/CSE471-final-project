#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
		: position(position), rotation(rotation), scale(scale)
	{
	}

	Transform(glm::vec3 position)
		: position(position), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f, 1.0f, 1.0f))
	{
	}

	[[nodiscard]] glm::mat4 getModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position) * glm::mat4_cast(rotation) * glm::scale(model, scale); // T * R * S
		return model;
	}
};