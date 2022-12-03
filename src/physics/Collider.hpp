#pragma once
#include <glm/glm.hpp>

#include <vector>

#include "rendering/Shader.hpp"

class BoxCollider
{
public:
	glm::vec3 min;
	glm::vec3 max;

	explicit BoxCollider(glm::vec3 size)
		: min(-size / 2.0f), max(size / 2.0f)
	{
		const float vertices[48] =
		{
			min.x, min.y, min.z,
			max.x, min.y, min.z,
			max.x, min.y, max.z,
			min.x, min.y, max.z,
			min.x, min.y, min.z,

			min.x, max.y, min.z,
			max.x, max.y, min.z,
			max.x, max.y, max.z,
			min.x, max.y, max.z,
			min.x, max.y, min.z,

			max.x, min.y, min.z,
			max.x, max.y, min.z,

			max.x, min.y, max.z,
			max.x, max.y, max.z,

			min.x, min.y, max.z,
			min.x, max.y, max.z,
		};


		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);
	}

	void draw(const Shader* shader, const glm::mat4 &model) const
	{
		shader->use();
		shader->setMat4("model", model);
		shader->setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f)); // yellow

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, 16);
		glBindVertexArray(0);
	}

private:
	GLuint VAO = 0, VBO = 0;
};