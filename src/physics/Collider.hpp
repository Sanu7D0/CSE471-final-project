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
		//const float vertices[48] = 
		//{
		//	min.x, min.y, min.z,  1.0f, 1.0f, 0.0f,
		//	min.x, min.y, min.z,  1.0f, 1.0f, 0.0f,
		//	min.x, min.y, min.z,  1.0f, 1.0f, 0.0f,
		//	min.x, min.y, min.z,  1.0f, 1.0f, 0.0f,
		//	min.x, min.y, min.z,  1.0f, 1.0f, 0.0f,
		//	min.x, min.y, min.z,  1.0f, 1.0f, 0.0f,
		//	min.x, min.y, min.z,  1.0f, 1.0f, 0.0f,
		//	min.x, min.y, min.z,  1.0f, 1.0f, 0.0f,
		//	min.x, min.y, min.z,  1.0f, 1.0f, 0.0f,
		//}


		//glGenVertexArrays(1, &VAO);
		//glGenBuffers(1, &VBO);

		//glBindVertexArray(VAO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

		//glEnableVertexAttribArray(0); // positions
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		//glEnableVertexAttribArray(1); // colors
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		//glBindVertexArray(0);
	}

	void draw(const Shader& shader, const glm::mat4 &model) const
	{
		shader.use();
		shader.setMat4("model", model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 16);
		glBindVertexArray(0);
	}

private:
	GLuint VAO = 0, VBO = 0;
};