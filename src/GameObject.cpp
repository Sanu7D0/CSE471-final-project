#include "GameObject.hpp"

GameObject::GameObject(const std::shared_ptr<GameObject>& parent, Transform transform, Model model)
	: parent(parent), transform(transform), model(std::move(model))
{
}

void GameObject::update(const float dt)
{
}

void GameObject::draw(const Shader& shader) const
{
	const auto modelMat = transform.getModelMatrix();

	shader.use();
	shader.setMat4("model", modelMat);
	shader.setMat4("model_inverse", glm::inverse(modelMat));

	model.draw(shader);

	if (Globals::debug)
	{
		drawAxes();
	}
}

void GameObject::draw(const Shader& shader, glm::mat4 parentTransform)
{
	const auto modelMat = parentTransform * transform.getModelMatrix();

	shader.use();
	shader.setMat4("model", modelMat);
	shader.setMat4("model_inverse", glm::inverse(modelMat));

	model.draw(shader);
	if (Globals::debug) drawAxes();

	for (const auto& child : children)
		child->draw(shader, modelMat);
}

void GameObject::drawAxes() const
{
	gAxesShader->use();
	gAxesShader->setMat4("model", transform.getModelMatrix());

	glBindVertexArray(gAxesVAO);
	gAxesShader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_LINES, 0, 2); // x
	gAxesShader->setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
	glDrawArrays(GL_LINES, 2, 2); // y
	gAxesShader->setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_LINES, 4, 2); // z

	glBindVertexArray(0);
}

void GameObject::addChild(GameObject& child)
{
	const auto ptr = std::make_shared<GameObject>(child);
	children.push_back(ptr);
	child.parent = ptr;
}
