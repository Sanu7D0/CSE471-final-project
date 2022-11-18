#include "GameObject.hpp"

GameObject::GameObject(const std::shared_ptr<GameObject>& parent, Transform transform, Model model)
	: parent(parent), transform(transform), model(std::move(model))
{
}

void GameObject::draw(const Shader& shader) const
{
	shader.setMat4("model", transform.getModelMatrix());
	model.draw(shader);

	// TODO: recursive draw for children
	/*for (auto child : children)
	{
		children->draw(shader);
	}*/
}

void GameObject::move(const float dt)
{

}
