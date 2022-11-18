#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>
#include <vector>

#include "rendering/Shader.hpp"
#include "rendering/Model.hpp"

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

class GameObject
{
public:
	std::weak_ptr<GameObject> parent;
	std::vector<std::shared_ptr<GameObject>> children;

	Transform transform;
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	GameObject(const std::shared_ptr<GameObject>& parent, Transform transform, Model model);

	virtual void draw(const Shader& shader) const;
	virtual void move(const float dt);

protected:
	Model model;
};
