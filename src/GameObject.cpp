#include "GameObject.hpp"

GameObject::GameObject(const std::shared_ptr<GameObject>& parent, Transform transform)
	: parent(parent), transform(transform)
{
}
