#include "GameObject.hpp"

GameObject::GameObject(const std::shared_ptr<GameObject>& parent, Transform transform, Model model)
	: parent(parent), transform(transform), model(std::move(model))
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

void GameObject::drawAxes() const
{
	gAxesShader->use();
	gAxesShader->setMat4("model", transform.getModelMatrix());

	glBindVertexArray(gAxesVAO);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);
}

void GameObject::move(const float dt)
{

}
