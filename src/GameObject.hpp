#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>
#include <vector>

#include "Transform.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Model.hpp"
#include "physics/Collider.hpp"
#include "Globals.hpp"

inline constexpr float gAxesPoints[36] = {
	// 3 vertex, 3 color
	0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
	2.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
	0.0f, 2.0f, 0.0f,  0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 2.0f,  0.0f, 0.0f, 1.0f
};
inline Shader* gAxesShader;
inline GLuint gAxesVAO, gAxesVBO;

inline void InitAxesShader()
{
	gAxesShader = new Shader("resource/shader/Line.vert", "resource/shader/Line.frag");

	glGenVertexArrays(1, &gAxesVAO);
	glGenBuffers(1, &gAxesVBO);

	glBindVertexArray(gAxesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gAxesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gAxesPoints), &gAxesPoints[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1); // colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
}

inline void DeleteAxesShader()
{
	delete gAxesShader;
}

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	std::weak_ptr<GameObject> parent;
	std::vector<std::shared_ptr<GameObject>> children;
	Transform transform;

	GameObject(const std::shared_ptr<GameObject>& parent, Transform transform, Model model);

	virtual void draw(const Shader& shader) const;
	virtual void draw(const Shader& shader, glm::mat4 parentTransform);
	void drawAxes() const;

	void addChild(const std::shared_ptr<GameObject>& child);

protected:
	Model model;
};
